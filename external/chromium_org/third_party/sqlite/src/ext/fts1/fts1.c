#if (!defined(SQLITE_CORE) || defined(SQLITE_ENABLE_FTS1)) \
        && !defined(SQLITE_ENABLE_BROKEN_FTS1)
#error fts1 has a design flaw and has been deprecated.
#endif

/* The author disclaims copyright to this source code.
 *
 * This is an SQLite module implementing full-text search.
 */

#if !defined(SQLITE_CORE) || defined(SQLITE_ENABLE_FTS1)

#if defined(SQLITE_ENABLE_FTS1) && !defined(SQLITE_CORE)
# define SQLITE_CORE 1
#endif

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "fts1.h"
#include "fts1_hash.h"
#include "fts1_tokenizer.h"
#include "sqlite3.h"
#include "sqlite3ext.h"
SQLITE_EXTENSION_INIT1


#if 0
# define TRACE(A)  printf A; fflush(stdout)
#else
# define TRACE(A)
#endif


typedef struct StringBuffer {
  int len;      
  int alloced;   
  char *s;      
} StringBuffer;

static void initStringBuffer(StringBuffer *sb){
  sb->len = 0;
  sb->alloced = 100;
  sb->s = malloc(100);
  sb->s[0] = '\0';
}

static void nappend(StringBuffer *sb, const char *zFrom, int nFrom){
  if( sb->len + nFrom >= sb->alloced ){
    sb->alloced = sb->len + nFrom + 100;
    sb->s = realloc(sb->s, sb->alloced+1);
    if( sb->s==0 ){
      initStringBuffer(sb);
      return;
    }
  }
  memcpy(sb->s + sb->len, zFrom, nFrom);
  sb->len += nFrom;
  sb->s[sb->len] = 0;
}
static void append(StringBuffer *sb, const char *zFrom){
  nappend(sb, zFrom, strlen(zFrom));
}


#define VARINT_MAX 10

/* Write a 64-bit variable-length integer to memory starting at p[0].
 * The length of data written will be between 1 and VARINT_MAX bytes.
 * The number of bytes written is returned. */
static int putVarint(char *p, sqlite_int64 v){
  unsigned char *q = (unsigned char *) p;
  sqlite_uint64 vu = v;
  do{
    *q++ = (unsigned char) ((vu & 0x7f) | 0x80);
    vu >>= 7;
  }while( vu!=0 );
  q[-1] &= 0x7f;  
  assert( q - (unsigned char *)p <= VARINT_MAX );
  return (int) (q - (unsigned char *)p);
}

static int getVarint(const char *p, sqlite_int64 *v){
  const unsigned char *q = (const unsigned char *) p;
  sqlite_uint64 x = 0, y = 1;
  while( (*q & 0x80) == 0x80 ){
    x += y * (*q++ & 0x7f);
    y <<= 7;
    if( q - (unsigned char *)p >= VARINT_MAX ){  
      assert( 0 );
      return 0;
    }
  }
  x += y * (*q++);
  *v = (sqlite_int64) x;
  return (int) (q - (unsigned char *)p);
}

static int getVarint32(const char *p, int *pi){
 sqlite_int64 i;
 int ret = getVarint(p, &i);
 *pi = (int) i;
 assert( *pi==i );
 return ret;
}


static int safe_isspace(char c){
  return (c&0x80)==0 ? isspace(c) : 0;
}
static int safe_tolower(char c){
  return (c&0x80)==0 ? tolower(c) : c;
}
static int safe_isalnum(char c){
  return (c&0x80)==0 ? isalnum(c) : 0;
}

typedef enum DocListType {
  DL_DOCIDS,              
  DL_POSITIONS,           
  DL_POSITIONS_OFFSETS    
} DocListType;

#ifndef DL_DEFAULT
# define DL_DEFAULT DL_POSITIONS
#endif

typedef struct DocList {
  char *pData;
  int nData;
  DocListType iType;
  int iLastColumn;    /* the last column written */
  int iLastPos;       /* the last position written */
  int iLastOffset;    /* the last start offset written */
} DocList;

enum {
  POS_END = 0,        
  POS_COLUMN,         
  POS_BASE
};

static void docListInit(DocList *d, DocListType iType,
                        const char *pData, int nData){
  d->nData = nData;
  if( nData>0 ){
    d->pData = malloc(nData);
    memcpy(d->pData, pData, nData);
  } else {
    d->pData = NULL;
  }
  d->iType = iType;
  d->iLastColumn = 0;
  d->iLastPos = d->iLastOffset = 0;
}

static DocList *docListNew(DocListType iType){
  DocList *d = (DocList *) malloc(sizeof(DocList));
  docListInit(d, iType, 0, 0);
  return d;
}

static void docListDestroy(DocList *d){
  free(d->pData);
#ifndef NDEBUG
  memset(d, 0x55, sizeof(*d));
#endif
}

static void docListDelete(DocList *d){
  docListDestroy(d);
  free(d);
}

static char *docListEnd(DocList *d){
  return d->pData + d->nData;
}

static void appendVarint(DocList *d, sqlite_int64 i){
  char c[VARINT_MAX];
  int n = putVarint(c, i);
  d->pData = realloc(d->pData, d->nData + n);
  memcpy(d->pData + d->nData, c, n);
  d->nData += n;
}

static void docListAddDocid(DocList *d, sqlite_int64 iDocid){
  appendVarint(d, iDocid);
  if( d->iType>=DL_POSITIONS ){
    appendVarint(d, POS_END);  
    d->iLastColumn = 0;
    d->iLastPos = d->iLastOffset = 0;
  }
}

static void addPos(DocList *d, int iColumn, int iPos){
  assert( d->nData>0 );
  --d->nData;  
  if( iColumn!=d->iLastColumn ){
    assert( iColumn>d->iLastColumn );
    appendVarint(d, POS_COLUMN);
    appendVarint(d, iColumn);
    d->iLastColumn = iColumn;
    d->iLastPos = d->iLastOffset = 0;
  }
  assert( iPos>=d->iLastPos );
  appendVarint(d, iPos-d->iLastPos+POS_BASE);
  d->iLastPos = iPos;
}

static void docListAddPos(DocList *d, int iColumn, int iPos){
  assert( d->iType==DL_POSITIONS );
  addPos(d, iColumn, iPos);
  appendVarint(d, POS_END);  
}

static void docListAddPosOffset(
  DocList *d,             
  int iColumn,            
  int iPos,               
  int iStartOffset,       
  int iEndOffset          
){
  assert( d->iType>=DL_POSITIONS );
  addPos(d, iColumn, iPos);
  if( d->iType==DL_POSITIONS_OFFSETS ){
    assert( iStartOffset>=d->iLastOffset );
    appendVarint(d, iStartOffset-d->iLastOffset);
    d->iLastOffset = iStartOffset;
    assert( iEndOffset>=iStartOffset );
    appendVarint(d, iEndOffset-iStartOffset);
  }
  appendVarint(d, POS_END);  
}

typedef struct DocListReader {
  DocList *pDoclist;  
  char *p;            
  int iLastColumn;
  int iLastPos;  
} DocListReader;

static void readerInit(DocListReader *r, DocList *pDoclist){
  r->pDoclist = pDoclist;
  if( pDoclist!=NULL ){
    r->p = pDoclist->pData;
  }
  r->iLastColumn = -1;
  r->iLastPos = -1;
}

static int atEnd(DocListReader *pReader){
  return pReader->pDoclist==0 || (pReader->p >= docListEnd(pReader->pDoclist));
}

static sqlite_int64 peekDocid(DocListReader *pReader){
  sqlite_int64 ret;
  assert( !atEnd(pReader) );
  assert( pReader->iLastPos==-1 );
  getVarint(pReader->p, &ret);
  return ret;
}

static sqlite_int64 readDocid(DocListReader *pReader){
  sqlite_int64 ret;
  assert( !atEnd(pReader) );
  assert( pReader->iLastPos==-1 );
  pReader->p += getVarint(pReader->p, &ret);
  if( pReader->pDoclist->iType>=DL_POSITIONS ){
    pReader->iLastColumn = 0;
    pReader->iLastPos = 0;
  }
  return ret;
}

static int readPosition(DocListReader *pReader, int *iColumn){
  int i;
  int iType = pReader->pDoclist->iType;

  if( pReader->iLastPos==-1 ){
    return -1;
  }
  assert( !atEnd(pReader) );

  if( iType<DL_POSITIONS ){
    return -1;
  }
  pReader->p += getVarint32(pReader->p, &i);
  if( i==POS_END ){
    pReader->iLastColumn = pReader->iLastPos = -1;
    *iColumn = -1;
    return -1;
  }
  if( i==POS_COLUMN ){
    pReader->p += getVarint32(pReader->p, &pReader->iLastColumn);
    pReader->iLastPos = 0;
    pReader->p += getVarint32(pReader->p, &i);
    assert( i>=POS_BASE );
  }
  pReader->iLastPos += ((int) i)-POS_BASE;
  if( iType>=DL_POSITIONS_OFFSETS ){
    
    int iStart, iEnd;
    pReader->p += getVarint32(pReader->p, &iStart);
    pReader->p += getVarint32(pReader->p, &iEnd);
  }
  *iColumn = pReader->iLastColumn;
  return pReader->iLastPos;
}

static void skipPositionList(DocListReader *pReader){
  DocList *p = pReader->pDoclist;
  if( p && p->iType>=DL_POSITIONS ){
    int iColumn;
    while( readPosition(pReader, &iColumn)!=-1 ){}
  }
}

static void skipDocument(DocListReader *pReader){
  readDocid(pReader);
  skipPositionList(pReader);
}

static int skipToDocid(DocListReader *pReader, sqlite_int64 iDocid){
  sqlite_int64 d = 0;
  while( !atEnd(pReader) && (d=peekDocid(pReader))<iDocid ){
    skipDocument(pReader);
  }
  return !atEnd(pReader) && d==iDocid;
}

static sqlite_int64 firstDocid(DocList *d){
  DocListReader r;
  readerInit(&r, d);
  return readDocid(&r);
}

#ifdef SQLITE_DEBUG
static void printDoclist(DocList *p){
  DocListReader r;
  const char *zSep = "";

  readerInit(&r, p);
  while( !atEnd(&r) ){
    sqlite_int64 docid = readDocid(&r);
    if( docid==0 ){
      skipPositionList(&r);
      continue;
    }
    printf("%s%lld", zSep, docid);
    zSep =  ",";
    if( p->iType>=DL_POSITIONS ){
      int iPos, iCol;
      const char *zDiv = "";
      printf("(");
      while( (iPos = readPosition(&r, &iCol))>=0 ){
        printf("%s%d:%d", zDiv, iCol, iPos);
        zDiv = ":";
      }
      printf(")");
    }
  }
  printf("\n");
  fflush(stdout);
}
#endif 

static void docListRestrictColumn(DocList *in, int iRestrictColumn){
  DocListReader r;
  DocList out;

  assert( in->iType>=DL_POSITIONS );
  readerInit(&r, in);
  docListInit(&out, DL_POSITIONS, NULL, 0);

  while( !atEnd(&r) ){
    sqlite_int64 iDocid = readDocid(&r);
    int iPos, iColumn;

    docListAddDocid(&out, iDocid);
    while( (iPos = readPosition(&r, &iColumn)) != -1 ){
      if( iColumn==iRestrictColumn ){
        docListAddPos(&out, iColumn, iPos);
      }
    }
  }

  docListDestroy(in);
  *in = out;
}

static void docListDiscardEmpty(DocList *in) {
  DocListReader r;
  DocList out;

  assert( in->iType>=DL_POSITIONS );
  readerInit(&r, in);
  docListInit(&out, DL_POSITIONS, NULL, 0);

  while( !atEnd(&r) ){
    sqlite_int64 iDocid = readDocid(&r);
    int match = 0;
    int iPos, iColumn;
    while( (iPos = readPosition(&r, &iColumn)) != -1 ){
      if( !match ){
        docListAddDocid(&out, iDocid);
        match = 1;
      }
      docListAddPos(&out, iColumn, iPos);
    }
  }

  docListDestroy(in);
  *in = out;
}

static void docListSpliceElement(DocListReader *r, sqlite_int64 iDocid,
                                 const char *pSource, int nSource){
  DocList *d = r->pDoclist;
  char *pTarget;
  int nTarget, found;

  found = skipToDocid(r, iDocid);

  
  pTarget = r->p;
  if( found ){
    skipDocument(r);
    nTarget = r->p-pTarget;
  }else{
    nTarget = 0;
  }

  if( nTarget>nSource ){
    memmove(pTarget+nSource, pTarget+nTarget, docListEnd(d)-(pTarget+nTarget));
  }
  if( nTarget!=nSource ){
    int iDoclist = pTarget-d->pData;
    d->pData = realloc(d->pData, d->nData+nSource-nTarget);
    pTarget = d->pData+iDoclist;
  }
  if( nTarget<nSource ){
    memmove(pTarget+nSource, pTarget+nTarget, docListEnd(d)-(pTarget+nTarget));
  }

  memcpy(pTarget, pSource, nSource);
  d->nData += nSource-nTarget;
  r->p = pTarget+nSource;
}

static void docListUpdate(DocList *d, DocList *pUpdate){
  DocListReader reader;

  assert( d!=NULL && pUpdate!=NULL );
  assert( d->iType==pUpdate->iType);

  readerInit(&reader, d);
  docListSpliceElement(&reader, firstDocid(pUpdate),
                       pUpdate->pData, pUpdate->nData);
}

static void docListAccumulate(DocList *pAcc, DocList *pUpdate){
  DocListReader accReader, updateReader;

  
  assert( pAcc!=NULL );
  if( pUpdate==NULL || pUpdate->nData==0 ) return;
  if( pAcc->nData==0 ){
    pAcc->pData = malloc(pUpdate->nData);
    memcpy(pAcc->pData, pUpdate->pData, pUpdate->nData);
    pAcc->nData = pUpdate->nData;
    return;
  }

  readerInit(&accReader, pAcc);
  readerInit(&updateReader, pUpdate);

  while( !atEnd(&updateReader) ){
    char *pSource = updateReader.p;
    sqlite_int64 iDocid = readDocid(&updateReader);
    skipPositionList(&updateReader);
    docListSpliceElement(&accReader, iDocid, pSource, updateReader.p-pSource);
  }
}

static sqlite_int64 nextDocid(DocListReader *pIn){
  skipPositionList(pIn);
  return atEnd(pIn) ? 0 : readDocid(pIn);
}

static void mergePosList(
  DocListReader *pLeft,    
  DocListReader *pRight,   
  sqlite_int64 iDocid,     
  DocList *pOut            
){
  int iLeftCol, iLeftPos = readPosition(pLeft, &iLeftCol);
  int iRightCol, iRightPos = readPosition(pRight, &iRightCol);
  int match = 0;

  
  while( iLeftPos!=-1 && iRightPos!=-1 ){
    if( iLeftCol==iRightCol && iLeftPos+1==iRightPos ){
      if( !match ){
        docListAddDocid(pOut, iDocid);
        match = 1;
      }
      if( pOut->iType>=DL_POSITIONS ){
        docListAddPos(pOut, iRightCol, iRightPos);
      }
      iLeftPos = readPosition(pLeft, &iLeftCol);
      iRightPos = readPosition(pRight, &iRightCol);
    }else if( iRightCol<iLeftCol ||
              (iRightCol==iLeftCol && iRightPos<iLeftPos+1) ){
      iRightPos = readPosition(pRight, &iRightCol);
    }else{
      iLeftPos = readPosition(pLeft, &iLeftCol);
    }
  }
  if( iLeftPos>=0 ) skipPositionList(pLeft);
  if( iRightPos>=0 ) skipPositionList(pRight);
}

static void docListPhraseMerge(
  DocList *pLeft,    
  DocList *pRight,   
  DocList *pOut      
){
  DocListReader left, right;
  sqlite_int64 docidLeft, docidRight;

  readerInit(&left, pLeft);
  readerInit(&right, pRight);
  docidLeft = nextDocid(&left);
  docidRight = nextDocid(&right);

  while( docidLeft>0 && docidRight>0 ){
    if( docidLeft<docidRight ){
      docidLeft = nextDocid(&left);
    }else if( docidRight<docidLeft ){
      docidRight = nextDocid(&right);
    }else{
      mergePosList(&left, &right, docidLeft, pOut);
      docidLeft = nextDocid(&left);
      docidRight = nextDocid(&right);
    }
  }
}

static void docListAndMerge(
  DocList *pLeft,    
  DocList *pRight,   
  DocList *pOut      
){
  DocListReader left, right;
  sqlite_int64 docidLeft, docidRight;

  assert( pOut->iType<DL_POSITIONS );

  readerInit(&left, pLeft);
  readerInit(&right, pRight);
  docidLeft = nextDocid(&left);
  docidRight = nextDocid(&right);

  while( docidLeft>0 && docidRight>0 ){
    if( docidLeft<docidRight ){
      docidLeft = nextDocid(&left);
    }else if( docidRight<docidLeft ){
      docidRight = nextDocid(&right);
    }else{
      docListAddDocid(pOut, docidLeft);
      docidLeft = nextDocid(&left);
      docidRight = nextDocid(&right);
    }
  }
}

static void docListOrMerge(
  DocList *pLeft,    
  DocList *pRight,   
  DocList *pOut      
){
  DocListReader left, right;
  sqlite_int64 docidLeft, docidRight, priorLeft;

  readerInit(&left, pLeft);
  readerInit(&right, pRight);
  docidLeft = nextDocid(&left);
  docidRight = nextDocid(&right);

  while( docidLeft>0 && docidRight>0 ){
    if( docidLeft<=docidRight ){
      docListAddDocid(pOut, docidLeft);
    }else{
      docListAddDocid(pOut, docidRight);
    }
    priorLeft = docidLeft;
    if( docidLeft<=docidRight ){
      docidLeft = nextDocid(&left);
    }
    if( docidRight>0 && docidRight<=priorLeft ){
      docidRight = nextDocid(&right);
    }
  }
  while( docidLeft>0 ){
    docListAddDocid(pOut, docidLeft);
    docidLeft = nextDocid(&left);
  }
  while( docidRight>0 ){
    docListAddDocid(pOut, docidRight);
    docidRight = nextDocid(&right);
  }
}

static void docListExceptMerge(
  DocList *pLeft,    
  DocList *pRight,   
  DocList *pOut      
){
  DocListReader left, right;
  sqlite_int64 docidLeft, docidRight, priorLeft;

  readerInit(&left, pLeft);
  readerInit(&right, pRight);
  docidLeft = nextDocid(&left);
  docidRight = nextDocid(&right);

  while( docidLeft>0 && docidRight>0 ){
    priorLeft = docidLeft;
    if( docidLeft<docidRight ){
      docListAddDocid(pOut, docidLeft);
    }
    if( docidLeft<=docidRight ){
      docidLeft = nextDocid(&left);
    }
    if( docidRight>0 && docidRight<=priorLeft ){
      docidRight = nextDocid(&right);
    }
  }
  while( docidLeft>0 ){
    docListAddDocid(pOut, docidLeft);
    docidLeft = nextDocid(&left);
  }
}

static char *string_dup_n(const char *s, int n){
  char *str = malloc(n + 1);
  memcpy(str, s, n);
  str[n] = '\0';
  return str;
}

static char *string_dup(const char *s){
  return string_dup_n(s, strlen(s));
}

static char *string_format(const char *zFormat,
                           const char *zDb, const char *zName){
  const char *p;
  size_t len = 0;
  size_t nDb = strlen(zDb);
  size_t nName = strlen(zName);
  size_t nFullTableName = nDb+1+nName;
  char *result;
  char *r;

  
  for(p = zFormat ; *p ; ++p){
    len += (*p=='%' ? nFullTableName : 1);
  }
  len += 1;  

  r = result = malloc(len);
  for(p = zFormat; *p; ++p){
    if( *p=='%' ){
      memcpy(r, zDb, nDb);
      r += nDb;
      *r++ = '.';
      memcpy(r, zName, nName);
      r += nName;
    } else {
      *r++ = *p;
    }
  }
  *r++ = '\0';
  assert( r == result + len );
  return result;
}

static int sql_exec(sqlite3 *db, const char *zDb, const char *zName,
                    const char *zFormat){
  char *zCommand = string_format(zFormat, zDb, zName);
  int rc;
  TRACE(("FTS1 sql: %s\n", zCommand));
  rc = sqlite3_exec(db, zCommand, NULL, 0, NULL);
  free(zCommand);
  return rc;
}

static int sql_prepare(sqlite3 *db, const char *zDb, const char *zName,
                       sqlite3_stmt **ppStmt, const char *zFormat){
  char *zCommand = string_format(zFormat, zDb, zName);
  int rc;
  TRACE(("FTS1 prepare: %s\n", zCommand));
  rc = sqlite3_prepare(db, zCommand, -1, ppStmt, NULL);
  free(zCommand);
  return rc;
}


typedef struct fulltext_vtab fulltext_vtab;

typedef struct QueryTerm {
  short int nPhrase; 
  short int iPhrase; 
  short int iColumn; 
  signed char isOr;  
  signed char isNot; 
  char *pTerm;       
  int nTerm;         
} QueryTerm;


typedef struct Query {
  fulltext_vtab *pFts;  
  int nTerms;           
  QueryTerm *pTerms;    
  int nextIsOr;         
  int nextColumn;       
  int dfltColumn;       
} Query;


typedef struct Snippet {
  int nMatch;     
  int nAlloc;     
  struct snippetMatch { 
    char snStatus;       
    short int iCol;      
    short int iTerm;     
    short int nByte;     
    int iStart;          
  } *aMatch;      
  char *zOffset;  
  int nOffset;    
  char *zSnippet; 
  int nSnippet;   
} Snippet;


typedef enum QueryType {
  QUERY_GENERIC,   
  QUERY_ROWID,     
  QUERY_FULLTEXT   
} QueryType;

/* TODO(shess) CHUNK_MAX controls how much data we allow in segment 0
** before we start aggregating into larger segments.  Lower CHUNK_MAX
** means that for a given input we have more individual segments per
** term, which means more rows in the table and a bigger index (due to
** both more rows and bigger rowids).  But it also reduces the average
** cost of adding new elements to the segment 0 doclist, and it seems
** to reduce the number of pages read and written during inserts.  256
** was chosen by measuring insertion times for a certain input (first
** 10k documents of Enron corpus), though including query performance
** in the decision may argue for a larger value.
*/
#define CHUNK_MAX 256

typedef enum fulltext_statement {
  CONTENT_INSERT_STMT,
  CONTENT_SELECT_STMT,
  CONTENT_UPDATE_STMT,
  CONTENT_DELETE_STMT,

  TERM_SELECT_STMT,
  TERM_SELECT_ALL_STMT,
  TERM_INSERT_STMT,
  TERM_UPDATE_STMT,
  TERM_DELETE_STMT,

  MAX_STMT                     
} fulltext_statement;

static const char *const fulltext_zStatement[MAX_STMT] = {
   NULL,  
   "select * from %_content where rowid = ?",
   NULL,  
   "delete from %_content where rowid = ?",

  
  "select rowid, doclist from %_term where term = ? and segment = ?",
  
  "select doclist from %_term where term = ? order by segment",
  
  "insert into %_term (rowid, term, segment, doclist) values (?, ?, ?, ?)",
   "update %_term set doclist = ? where rowid = ?",
   "delete from %_term where rowid = ?",
};

struct fulltext_vtab {
  sqlite3_vtab base;               
  sqlite3 *db;                     
  const char *zDb;                 
  const char *zName;               
  int nColumn;                     
  char **azColumn;                 
  char **azContentColumn;          
  sqlite3_tokenizer *pTokenizer;   

  sqlite3_stmt *pFulltextStatements[MAX_STMT];
};

typedef struct fulltext_cursor {
  sqlite3_vtab_cursor base;        
  QueryType iCursorType;           
  sqlite3_stmt *pStmt;             
  int eof;                         
  Query q;                         
  Snippet snippet;                 
  int iColumn;                     
  DocListReader result;   
} fulltext_cursor;

static struct fulltext_vtab *cursor_vtab(fulltext_cursor *c){
  return (fulltext_vtab *) c->base.pVtab;
}

static const sqlite3_module fulltextModule;   

static void appendList(StringBuffer *sb, int nString, char **azString){
  int i;
  for(i=0; i<nString; ++i){
    if( i>0 ) append(sb, ", ");
    append(sb, azString[i]);
  }
}

static const char *contentInsertStatement(fulltext_vtab *v){
  StringBuffer sb;
  int i;

  initStringBuffer(&sb);
  append(&sb, "insert into %_content (rowid, ");
  appendList(&sb, v->nColumn, v->azContentColumn);
  append(&sb, ") values (?");
  for(i=0; i<v->nColumn; ++i)
    append(&sb, ", ?");
  append(&sb, ")");
  return sb.s;
}

static const char *contentUpdateStatement(fulltext_vtab *v){
  StringBuffer sb;
  int i;

  initStringBuffer(&sb);
  append(&sb, "update %_content set ");
  for(i=0; i<v->nColumn; ++i) {
    if( i>0 ){
      append(&sb, ", ");
    }
    append(&sb, v->azContentColumn[i]);
    append(&sb, " = ?");
  }
  append(&sb, " where rowid = ?");
  return sb.s;
}

static int sql_get_statement(fulltext_vtab *v, fulltext_statement iStmt,
                             sqlite3_stmt **ppStmt){
  assert( iStmt<MAX_STMT );
  if( v->pFulltextStatements[iStmt]==NULL ){
    const char *zStmt;
    int rc;
    switch( iStmt ){
      case CONTENT_INSERT_STMT:
        zStmt = contentInsertStatement(v); break;
      case CONTENT_UPDATE_STMT:
        zStmt = contentUpdateStatement(v); break;
      default:
        zStmt = fulltext_zStatement[iStmt];
    }
    rc = sql_prepare(v->db, v->zDb, v->zName, &v->pFulltextStatements[iStmt],
                         zStmt);
    if( zStmt != fulltext_zStatement[iStmt]) free((void *) zStmt);
    if( rc!=SQLITE_OK ) return rc;
  } else {
    int rc = sqlite3_reset(v->pFulltextStatements[iStmt]);
    if( rc!=SQLITE_OK ) return rc;
  }

  *ppStmt = v->pFulltextStatements[iStmt];
  return SQLITE_OK;
}

static int sql_step_statement(fulltext_vtab *v, fulltext_statement iStmt,
                              sqlite3_stmt **ppStmt){
  int rc;
  sqlite3_stmt *s = *ppStmt;
  assert( iStmt<MAX_STMT );
  assert( s==v->pFulltextStatements[iStmt] );

  while( (rc=sqlite3_step(s))!=SQLITE_DONE && rc!=SQLITE_ROW ){
    if( rc==SQLITE_BUSY ) continue;
    if( rc!=SQLITE_ERROR ) return rc;

    v->pFulltextStatements[iStmt] = NULL;
    rc = sqlite3_finalize(s);
    break;
  }
  return rc;

 err:
  sqlite3_finalize(s);
  return rc;
}

static int sql_single_step_statement(fulltext_vtab *v,
                                     fulltext_statement iStmt,
                                     sqlite3_stmt **ppStmt){
  int rc = sql_step_statement(v, iStmt, ppStmt);
  return (rc==SQLITE_DONE) ? SQLITE_OK : rc;
}

static int content_insert(fulltext_vtab *v, sqlite3_value *rowid,
                          sqlite3_value **pValues){
  sqlite3_stmt *s;
  int i;
  int rc = sql_get_statement(v, CONTENT_INSERT_STMT, &s);
  if( rc!=SQLITE_OK ) return rc;

  rc = sqlite3_bind_value(s, 1, rowid);
  if( rc!=SQLITE_OK ) return rc;

  for(i=0; i<v->nColumn; ++i){
    rc = sqlite3_bind_value(s, 2+i, pValues[i]);
    if( rc!=SQLITE_OK ) return rc;
  }

  return sql_single_step_statement(v, CONTENT_INSERT_STMT, &s);
}

static int content_update(fulltext_vtab *v, sqlite3_value **pValues,
                          sqlite_int64 iRowid){
  sqlite3_stmt *s;
  int i;
  int rc = sql_get_statement(v, CONTENT_UPDATE_STMT, &s);
  if( rc!=SQLITE_OK ) return rc;

  for(i=0; i<v->nColumn; ++i){
    rc = sqlite3_bind_value(s, 1+i, pValues[i]);
    if( rc!=SQLITE_OK ) return rc;
  }

  rc = sqlite3_bind_int64(s, 1+v->nColumn, iRowid);
  if( rc!=SQLITE_OK ) return rc;

  return sql_single_step_statement(v, CONTENT_UPDATE_STMT, &s);
}

static void freeStringArray(int nString, const char **pString){
  int i;

  for (i=0 ; i < nString ; ++i) {
    if( pString[i]!=NULL ) free((void *) pString[i]);
  }
  free((void *) pString);
}

static int content_select(fulltext_vtab *v, sqlite_int64 iRow,
                          const char ***pValues){
  sqlite3_stmt *s;
  const char **values;
  int i;
  int rc;

  *pValues = NULL;

  rc = sql_get_statement(v, CONTENT_SELECT_STMT, &s);
  if( rc!=SQLITE_OK ) return rc;

  rc = sqlite3_bind_int64(s, 1, iRow);
  if( rc!=SQLITE_OK ) return rc;

  rc = sql_step_statement(v, CONTENT_SELECT_STMT, &s);
  if( rc!=SQLITE_ROW ) return rc;

  values = (const char **) malloc(v->nColumn * sizeof(const char *));
  for(i=0; i<v->nColumn; ++i){
    if( sqlite3_column_type(s, i)==SQLITE_NULL ){
      values[i] = NULL;
    }else{
      values[i] = string_dup((char*)sqlite3_column_text(s, i));
    }
  }

  rc = sqlite3_step(s);
  if( rc==SQLITE_DONE ){
    *pValues = values;
    return SQLITE_OK;
  }

  freeStringArray(v->nColumn, values);
  return rc;
}

static int content_delete(fulltext_vtab *v, sqlite_int64 iRow){
  sqlite3_stmt *s;
  int rc = sql_get_statement(v, CONTENT_DELETE_STMT, &s);
  if( rc!=SQLITE_OK ) return rc;

  rc = sqlite3_bind_int64(s, 1, iRow);
  if( rc!=SQLITE_OK ) return rc;

  return sql_single_step_statement(v, CONTENT_DELETE_STMT, &s);
}

static int term_select(fulltext_vtab *v, const char *pTerm, int nTerm,
                       int iSegment,
                       sqlite_int64 *rowid, DocList *out){
  sqlite3_stmt *s;
  int rc = sql_get_statement(v, TERM_SELECT_STMT, &s);
  if( rc!=SQLITE_OK ) return rc;

  rc = sqlite3_bind_text(s, 1, pTerm, nTerm, SQLITE_STATIC);
  if( rc!=SQLITE_OK ) return rc;

  rc = sqlite3_bind_int(s, 2, iSegment);
  if( rc!=SQLITE_OK ) return rc;

  rc = sql_step_statement(v, TERM_SELECT_STMT, &s);
  if( rc!=SQLITE_ROW ) return rc;

  *rowid = sqlite3_column_int64(s, 0);
  docListInit(out, DL_DEFAULT,
              sqlite3_column_blob(s, 1), sqlite3_column_bytes(s, 1));

  rc = sqlite3_step(s);
  return rc==SQLITE_DONE ? SQLITE_ROW : rc;
}

static int term_select_all(
  fulltext_vtab *v,     
  int iColumn,          
  const char *pTerm,    
  int nTerm,            
  DocList *out          
){
  DocList doclist;
  sqlite3_stmt *s;
  int rc = sql_get_statement(v, TERM_SELECT_ALL_STMT, &s);
  if( rc!=SQLITE_OK ) return rc;

  rc = sqlite3_bind_text(s, 1, pTerm, nTerm, SQLITE_STATIC);
  if( rc!=SQLITE_OK ) return rc;

  docListInit(&doclist, DL_DEFAULT, 0, 0);

  
  while( (rc=sql_step_statement(v, TERM_SELECT_ALL_STMT, &s))==SQLITE_ROW ){
    DocList old;

    docListInit(&old, DL_DEFAULT,
                sqlite3_column_blob(s, 0), sqlite3_column_bytes(s, 0));

    if( iColumn<v->nColumn ){   
      docListRestrictColumn(&old, iColumn);
    }

    docListAccumulate(&old, &doclist);
    docListDestroy(&doclist);
    doclist = old;
  }
  if( rc!=SQLITE_DONE ){
    docListDestroy(&doclist);
    return rc;
  }

  docListDiscardEmpty(&doclist);
  *out = doclist;
  return SQLITE_OK;
}

static int term_insert(fulltext_vtab *v, sqlite_int64 *piRowid,
                       const char *pTerm, int nTerm,
                       int iSegment, DocList *doclist){
  sqlite3_stmt *s;
  int rc = sql_get_statement(v, TERM_INSERT_STMT, &s);
  if( rc!=SQLITE_OK ) return rc;

  if( piRowid==NULL ){
    rc = sqlite3_bind_null(s, 1);
  }else{
    rc = sqlite3_bind_int64(s, 1, *piRowid);
  }
  if( rc!=SQLITE_OK ) return rc;

  rc = sqlite3_bind_text(s, 2, pTerm, nTerm, SQLITE_STATIC);
  if( rc!=SQLITE_OK ) return rc;

  rc = sqlite3_bind_int(s, 3, iSegment);
  if( rc!=SQLITE_OK ) return rc;

  rc = sqlite3_bind_blob(s, 4, doclist->pData, doclist->nData, SQLITE_STATIC);
  if( rc!=SQLITE_OK ) return rc;

  return sql_single_step_statement(v, TERM_INSERT_STMT, &s);
}

static int term_update(fulltext_vtab *v, sqlite_int64 rowid,
                       DocList *doclist){
  sqlite3_stmt *s;
  int rc = sql_get_statement(v, TERM_UPDATE_STMT, &s);
  if( rc!=SQLITE_OK ) return rc;

  rc = sqlite3_bind_blob(s, 1, doclist->pData, doclist->nData, SQLITE_STATIC);
  if( rc!=SQLITE_OK ) return rc;

  rc = sqlite3_bind_int64(s, 2, rowid);
  if( rc!=SQLITE_OK ) return rc;

  return sql_single_step_statement(v, TERM_UPDATE_STMT, &s);
}

static int term_delete(fulltext_vtab *v, sqlite_int64 rowid){
  sqlite3_stmt *s;
  int rc = sql_get_statement(v, TERM_DELETE_STMT, &s);
  if( rc!=SQLITE_OK ) return rc;

  rc = sqlite3_bind_int64(s, 1, rowid);
  if( rc!=SQLITE_OK ) return rc;

  return sql_single_step_statement(v, TERM_DELETE_STMT, &s);
}

static void fulltext_vtab_destroy(fulltext_vtab *v){
  int iStmt, i;

  TRACE(("FTS1 Destroy %p\n", v));
  for( iStmt=0; iStmt<MAX_STMT; iStmt++ ){
    if( v->pFulltextStatements[iStmt]!=NULL ){
      sqlite3_finalize(v->pFulltextStatements[iStmt]);
      v->pFulltextStatements[iStmt] = NULL;
    }
  }

  if( v->pTokenizer!=NULL ){
    v->pTokenizer->pModule->xDestroy(v->pTokenizer);
    v->pTokenizer = NULL;
  }
  
  free(v->azColumn);
  for(i = 0; i < v->nColumn; ++i) {
    sqlite3_free(v->azContentColumn[i]);
  }
  free(v->azContentColumn);
  free(v);
}

#define TOKEN_EOF         0    
#define TOKEN_SPACE       1    
#define TOKEN_ID          2    
#define TOKEN_STRING      3    
#define TOKEN_PUNCT       4    

static const char isIdChar[] = {
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,  
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1,  
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,  
};
#define IdChar(C)  (((c=C)&0x80)!=0 || (c>0x1f && isIdChar[c-0x20]))


static int getToken(const char *z, int *tokenType){
  int i, c;
  switch( *z ){
    case 0: {
      *tokenType = TOKEN_EOF;
      return 0;
    }
    case ' ': case '\t': case '\n': case '\f': case '\r': {
      for(i=1; safe_isspace(z[i]); i++){}
      *tokenType = TOKEN_SPACE;
      return i;
    }
    case '`':
    case '\'':
    case '"': {
      int delim = z[0];
      for(i=1; (c=z[i])!=0; i++){
        if( c==delim ){
          if( z[i+1]==delim ){
            i++;
          }else{
            break;
          }
        }
      }
      *tokenType = TOKEN_STRING;
      return i + (c!=0);
    }
    case '[': {
      for(i=1, c=z[0]; c!=']' && (c=z[i])!=0; i++){}
      *tokenType = TOKEN_ID;
      return i;
    }
    default: {
      if( !IdChar(*z) ){
        break;
      }
      for(i=1; IdChar(z[i]); i++){}
      *tokenType = TOKEN_ID;
      return i;
    }
  }
  *tokenType = TOKEN_PUNCT;
  return 1;
}

typedef struct Token {
  const char *z;       
  short int n;         
} Token;

static char **tokenizeString(const char *z, int *pnToken){
  int nToken = 0;
  Token *aToken = malloc( strlen(z) * sizeof(aToken[0]) );
  int n = 1;
  int e, i;
  int totalSize = 0;
  char **azToken;
  char *zCopy;
  while( n>0 ){
    n = getToken(z, &e);
    if( e!=TOKEN_SPACE ){
      aToken[nToken].z = z;
      aToken[nToken].n = n;
      nToken++;
      totalSize += n+1;
    }
    z += n;
  }
  azToken = (char**)malloc( nToken*sizeof(char*) + totalSize );
  zCopy = (char*)&azToken[nToken];
  nToken--;
  for(i=0; i<nToken; i++){
    azToken[i] = zCopy;
    n = aToken[i].n;
    memcpy(zCopy, aToken[i].z, n);
    zCopy[n] = 0;
    zCopy += n+1;
  }
  azToken[nToken] = 0;
  free(aToken);
  *pnToken = nToken;
  return azToken;
}

static void dequoteString(char *z){
  int quote;
  int i, j;
  if( z==0 ) return;
  quote = z[0];
  switch( quote ){
    case '\'':  break;
    case '"':   break;
    case '`':   break;                
    case '[':   quote = ']';  break;  
    default:    return;
  }
  for(i=1, j=0; z[i]; i++){
    if( z[i]==quote ){
      if( z[i+1]==quote ){
        z[j++] = quote;
        i++;
      }else{
        z[j++] = 0;
        break;
      }
    }else{
      z[j++] = z[i];
    }
  }
}

static void tokenListToIdList(char **azIn){
  int i, j;
  if( azIn ){
    for(i=0, j=-1; azIn[i]; i++){
      if( safe_isalnum(azIn[i][0]) || azIn[i][1] ){
        dequoteString(azIn[i]);
        if( j>=0 ){
          azIn[j] = azIn[i];
        }
        j++;
      }
    }
    azIn[j] = 0;
  }
}


static char *firstToken(char *zIn, char **pzTail){
  int n, ttype;
  while(1){
    n = getToken(zIn, &ttype);
    if( ttype==TOKEN_SPACE ){
      zIn += n;
    }else if( ttype==TOKEN_EOF ){
      *pzTail = zIn;
      return 0;
    }else{
      zIn[n] = 0;
      *pzTail = &zIn[1];
      dequoteString(zIn);
      return zIn;
    }
  }
  
}

static int startsWith(const char *s, const char *t){
  while( safe_isspace(*s) ){ s++; }
  while( *t ){
    if( safe_tolower(*s++)!=safe_tolower(*t++) ) return 0;
  }
  return *s!='_' && !safe_isalnum(*s);
}

typedef struct TableSpec {
  const char *zDb;         
  const char *zName;       
  int nColumn;             
  char **azColumn;         
  char **azContentColumn;  
  char **azTokenizer;      
} TableSpec;

static void clearTableSpec(TableSpec *p) {
  free(p->azColumn);
  free(p->azContentColumn);
  free(p->azTokenizer);
}

static int parseSpec(TableSpec *pSpec, int argc, const char *const*argv,
                     char**pzErr){
  int i, n;
  char *z, *zDummy;
  char **azArg;
  const char *zTokenizer = 0;    

  assert( argc>=3 );

  memset(pSpec, 0, sizeof(*pSpec));
  for(i=n=0; i<argc; i++){
    n += strlen(argv[i]) + 1;
  }
  azArg = malloc( sizeof(char*)*argc + n );
  if( azArg==0 ){
    return SQLITE_NOMEM;
  }
  z = (char*)&azArg[argc];
  for(i=0; i<argc; i++){
    azArg[i] = z;
    strcpy(z, argv[i]);
    z += strlen(z)+1;
  }

  pSpec->zDb = azArg[1];
  pSpec->zName = azArg[2];
  pSpec->nColumn = 0;
  pSpec->azColumn = azArg;
  zTokenizer = "tokenize simple";
  for(i=3; i<argc; ++i){
    if( startsWith(azArg[i],"tokenize") ){
      zTokenizer = azArg[i];
    }else{
      z = azArg[pSpec->nColumn] = firstToken(azArg[i], &zDummy);
      pSpec->nColumn++;
    }
  }
  if( pSpec->nColumn==0 ){
    azArg[0] = "content";
    pSpec->nColumn = 1;
  }

  pSpec->azContentColumn = malloc( pSpec->nColumn * sizeof(char *) );
  if( pSpec->azContentColumn==0 ){
    clearTableSpec(pSpec);
    return SQLITE_NOMEM;
  }
  for(i=0; i<pSpec->nColumn; i++){
    char *p;
    pSpec->azContentColumn[i] = sqlite3_mprintf("c%d%s", i, azArg[i]);
    for (p = pSpec->azContentColumn[i]; *p ; ++p) {
      if( !safe_isalnum(*p) ) *p = '_';
    }
  }

  pSpec->azTokenizer = tokenizeString(zTokenizer, &n);
  tokenListToIdList(pSpec->azTokenizer);

  return SQLITE_OK;
}

static char *fulltextSchema(
  int nColumn,                  
  const char *const* azColumn,  
  const char *zTableName        
){
  int i;
  char *zSchema, *zNext;
  const char *zSep = "(";
  zSchema = sqlite3_mprintf("CREATE TABLE x");
  for(i=0; i<nColumn; i++){
    zNext = sqlite3_mprintf("%s%s%Q", zSchema, zSep, azColumn[i]);
    sqlite3_free(zSchema);
    zSchema = zNext;
    zSep = ",";
  }
  zNext = sqlite3_mprintf("%s,%Q)", zSchema, zTableName);
  sqlite3_free(zSchema);
  return zNext;
}

static int constructVtab(
  sqlite3 *db,              
  TableSpec *spec,          
  sqlite3_vtab **ppVTab,    
  char **pzErr              
){
  int rc;
  int n;
  fulltext_vtab *v = 0;
  const sqlite3_tokenizer_module *m = NULL;
  char *schema;

  v = (fulltext_vtab *) malloc(sizeof(fulltext_vtab));
  if( v==0 ) return SQLITE_NOMEM;
  memset(v, 0, sizeof(*v));
  
  v->db = db;
  v->zDb = spec->zDb;       
  v->zName = spec->zName;   
  v->nColumn = spec->nColumn;
  v->azContentColumn = spec->azContentColumn;
  spec->azContentColumn = 0;
  v->azColumn = spec->azColumn;
  spec->azColumn = 0;

  if( spec->azTokenizer==0 ){
    return SQLITE_NOMEM;
  }
  
  if( spec->azTokenizer[0]==0 || startsWith(spec->azTokenizer[0], "simple") ){
    sqlite3Fts1SimpleTokenizerModule(&m);
  }else if( startsWith(spec->azTokenizer[0], "porter") ){
    sqlite3Fts1PorterTokenizerModule(&m);
  }else{
    *pzErr = sqlite3_mprintf("unknown tokenizer: %s", spec->azTokenizer[0]);
    rc = SQLITE_ERROR;
    goto err;
  }
  for(n=0; spec->azTokenizer[n]; n++){}
  if( n ){
    rc = m->xCreate(n-1, (const char*const*)&spec->azTokenizer[1],
                    &v->pTokenizer);
  }else{
    rc = m->xCreate(0, 0, &v->pTokenizer);
  }
  if( rc!=SQLITE_OK ) goto err;
  v->pTokenizer->pModule = m;

  

  schema = fulltextSchema(v->nColumn, (const char*const*)v->azColumn,
                          spec->zName);
  rc = sqlite3_declare_vtab(db, schema);
  sqlite3_free(schema);
  if( rc!=SQLITE_OK ) goto err;

  memset(v->pFulltextStatements, 0, sizeof(v->pFulltextStatements));

  *ppVTab = &v->base;
  TRACE(("FTS1 Connect %p\n", v));

  return rc;

err:
  fulltext_vtab_destroy(v);
  return rc;
}

static int fulltextConnect(
  sqlite3 *db,
  void *pAux,
  int argc, const char *const*argv,
  sqlite3_vtab **ppVTab,
  char **pzErr
){
  TableSpec spec;
  int rc = parseSpec(&spec, argc, argv, pzErr);
  if( rc!=SQLITE_OK ) return rc;

  rc = constructVtab(db, &spec, ppVTab, pzErr);
  clearTableSpec(&spec);
  return rc;
}

  /* The %_content table holds the text of each document, with
  ** the rowid used as the docid.
  **
  ** The %_term table maps each term to a document list blob
  ** containing elements sorted by ascending docid, each element
  ** encoded as:
  **
  **   docid varint-encoded
  **   token elements:
  **     position+1 varint-encoded as delta from previous position
  **     start offset varint-encoded as delta from previous start offset
  **     end offset varint-encoded as delta from start offset
  **
  ** The sentinel position of 0 indicates the end of the token list.
  **
  ** Additionally, doclist blobs are chunked into multiple segments,
  ** using segment to order the segments.  New elements are added to
  ** the segment at segment 0, until it exceeds CHUNK_MAX.  Then
  ** segment 0 is deleted, and the doclist is inserted at segment 1.
  ** If there is already a doclist at segment 1, the segment 0 doclist
  ** is merged with it, the segment 1 doclist is deleted, and the
  ** merged doclist is inserted at segment 2, repeating those
  ** operations until an insert succeeds.
  **
  ** Since this structure doesn't allow us to update elements in place
  ** in case of deletion or update, these are simply written to
  ** segment 0 (with an empty token list in case of deletion), with
  ** docListAccumulate() taking care to retain lower-segment
  ** information in preference to higher-segment information.
  */
static int fulltextCreate(sqlite3 *db, void *pAux,
                          int argc, const char * const *argv,
                          sqlite3_vtab **ppVTab, char **pzErr){
  int rc;
  TableSpec spec;
  StringBuffer schema;
  TRACE(("FTS1 Create\n"));

  rc = parseSpec(&spec, argc, argv, pzErr);
  if( rc!=SQLITE_OK ) return rc;

  initStringBuffer(&schema);
  append(&schema, "CREATE TABLE %_content(");
  appendList(&schema, spec.nColumn, spec.azContentColumn);
  append(&schema, ")");
  rc = sql_exec(db, spec.zDb, spec.zName, schema.s);
  free(schema.s);
  if( rc!=SQLITE_OK ) goto out;

  rc = sql_exec(db, spec.zDb, spec.zName,
    "create table %_term(term text, segment integer, doclist blob, "
                        "primary key(term, segment));");
  if( rc!=SQLITE_OK ) goto out;

  rc = constructVtab(db, &spec, ppVTab, pzErr);

out:
  clearTableSpec(&spec);
  return rc;
}

static int fulltextBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info *pInfo){
  int i;
  TRACE(("FTS1 BestIndex\n"));

  for(i=0; i<pInfo->nConstraint; ++i){
    const struct sqlite3_index_constraint *pConstraint;
    pConstraint = &pInfo->aConstraint[i];
    if( pConstraint->usable ) {
      if( pConstraint->iColumn==-1 &&
          pConstraint->op==SQLITE_INDEX_CONSTRAINT_EQ ){
        pInfo->idxNum = QUERY_ROWID;      
        TRACE(("FTS1 QUERY_ROWID\n"));
      } else if( pConstraint->iColumn>=0 &&
                 pConstraint->op==SQLITE_INDEX_CONSTRAINT_MATCH ){
        
        pInfo->idxNum = QUERY_FULLTEXT + pConstraint->iColumn;
        TRACE(("FTS1 QUERY_FULLTEXT %d\n", pConstraint->iColumn));
      } else continue;

      pInfo->aConstraintUsage[i].argvIndex = 1;
      pInfo->aConstraintUsage[i].omit = 1;

      pInfo->estimatedCost = 1.0;   

      return SQLITE_OK;
    }
  }
  pInfo->idxNum = QUERY_GENERIC;
  return SQLITE_OK;
}

static int fulltextDisconnect(sqlite3_vtab *pVTab){
  TRACE(("FTS1 Disconnect %p\n", pVTab));
  fulltext_vtab_destroy((fulltext_vtab *)pVTab);
  return SQLITE_OK;
}

static int fulltextDestroy(sqlite3_vtab *pVTab){
  fulltext_vtab *v = (fulltext_vtab *)pVTab;
  int rc;

  TRACE(("FTS1 Destroy %p\n", pVTab));
  rc = sql_exec(v->db, v->zDb, v->zName,
                "drop table if exists %_content;"
                "drop table if exists %_term;"
                );
  if( rc!=SQLITE_OK ) return rc;

  fulltext_vtab_destroy((fulltext_vtab *)pVTab);
  return SQLITE_OK;
}

static int fulltextOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor){
  fulltext_cursor *c;

  c = (fulltext_cursor *) calloc(sizeof(fulltext_cursor), 1);
  
  *ppCursor = &c->base;
  TRACE(("FTS1 Open %p: %p\n", pVTab, c));

  return SQLITE_OK;
}


static void queryClear(Query *q){
  int i;
  for(i = 0; i < q->nTerms; ++i){
    free(q->pTerms[i].pTerm);
  }
  free(q->pTerms);
  memset(q, 0, sizeof(*q));
}

static void snippetClear(Snippet *p){
  free(p->aMatch);
  free(p->zOffset);
  free(p->zSnippet);
  memset(p, 0, sizeof(*p));
}
static void snippetAppendMatch(
  Snippet *p,               
  int iCol, int iTerm,      
  int iStart, int nByte     
){
  int i;
  struct snippetMatch *pMatch;
  if( p->nMatch+1>=p->nAlloc ){
    p->nAlloc = p->nAlloc*2 + 10;
    p->aMatch = realloc(p->aMatch, p->nAlloc*sizeof(p->aMatch[0]) );
    if( p->aMatch==0 ){
      p->nMatch = 0;
      p->nAlloc = 0;
      return;
    }
  }
  i = p->nMatch++;
  pMatch = &p->aMatch[i];
  pMatch->iCol = iCol;
  pMatch->iTerm = iTerm;
  pMatch->iStart = iStart;
  pMatch->nByte = nByte;
}

#define FTS1_ROTOR_SZ   (32)
#define FTS1_ROTOR_MASK (FTS1_ROTOR_SZ-1)

static void snippetOffsetsOfColumn(
  Query *pQuery,
  Snippet *pSnippet,
  int iColumn,
  const char *zDoc,
  int nDoc
){
  const sqlite3_tokenizer_module *pTModule;  
  sqlite3_tokenizer *pTokenizer;             
  sqlite3_tokenizer_cursor *pTCursor;        
  fulltext_vtab *pVtab;                
  int nColumn;                         
  const QueryTerm *aTerm;              
  int nTerm;                             
  int i, j;                            
  int rc;                              
  unsigned int match, prevMatch;       
  const char *zToken;                  
  int nToken;                          
  int iBegin, iEnd, iPos;              

  unsigned int iRotor = 0;             
  int iRotorBegin[FTS1_ROTOR_SZ];      
  int iRotorLen[FTS1_ROTOR_SZ];        

  pVtab = pQuery->pFts;
  nColumn = pVtab->nColumn;
  pTokenizer = pVtab->pTokenizer;
  pTModule = pTokenizer->pModule;
  rc = pTModule->xOpen(pTokenizer, zDoc, nDoc, &pTCursor);
  if( rc ) return;
  pTCursor->pTokenizer = pTokenizer;
  aTerm = pQuery->pTerms;
  nTerm = pQuery->nTerms;
  if( nTerm>=FTS1_ROTOR_SZ ){
    nTerm = FTS1_ROTOR_SZ - 1;
  }
  prevMatch = 0;
  while(1){
    rc = pTModule->xNext(pTCursor, &zToken, &nToken, &iBegin, &iEnd, &iPos);
    if( rc ) break;
    iRotorBegin[iRotor&FTS1_ROTOR_MASK] = iBegin;
    iRotorLen[iRotor&FTS1_ROTOR_MASK] = iEnd-iBegin;
    match = 0;
    for(i=0; i<nTerm; i++){
      int iCol;
      iCol = aTerm[i].iColumn;
      if( iCol>=0 && iCol<nColumn && iCol!=iColumn ) continue;
      if( aTerm[i].nTerm!=nToken ) continue;
      if( memcmp(aTerm[i].pTerm, zToken, nToken) ) continue;
      if( aTerm[i].iPhrase>1 && (prevMatch & (1<<i))==0 ) continue;
      match |= 1<<i;
      if( i==nTerm-1 || aTerm[i+1].iPhrase==1 ){
        for(j=aTerm[i].iPhrase-1; j>=0; j--){
          int k = (iRotor-j) & FTS1_ROTOR_MASK;
          snippetAppendMatch(pSnippet, iColumn, i-j,
                iRotorBegin[k], iRotorLen[k]);
        }
      }
    }
    prevMatch = match<<1;
    iRotor++;
  }
  pTModule->xClose(pTCursor);  
}


static void snippetAllOffsets(fulltext_cursor *p){
  int nColumn;
  int iColumn, i;
  int iFirst, iLast;
  fulltext_vtab *pFts;

  if( p->snippet.nMatch ) return;
  if( p->q.nTerms==0 ) return;
  pFts = p->q.pFts;
  nColumn = pFts->nColumn;
  iColumn = p->iCursorType - QUERY_FULLTEXT;
  if( iColumn<0 || iColumn>=nColumn ){
    iFirst = 0;
    iLast = nColumn-1;
  }else{
    iFirst = iColumn;
    iLast = iColumn;
  }
  for(i=iFirst; i<=iLast; i++){
    const char *zDoc;
    int nDoc;
    zDoc = (const char*)sqlite3_column_text(p->pStmt, i+1);
    nDoc = sqlite3_column_bytes(p->pStmt, i+1);
    snippetOffsetsOfColumn(&p->q, &p->snippet, i, zDoc, nDoc);
  }
}

static void snippetOffsetText(Snippet *p){
  int i;
  int cnt = 0;
  StringBuffer sb;
  char zBuf[200];
  if( p->zOffset ) return;
  initStringBuffer(&sb);
  for(i=0; i<p->nMatch; i++){
    struct snippetMatch *pMatch = &p->aMatch[i];
    zBuf[0] = ' ';
    sqlite3_snprintf(sizeof(zBuf)-1, &zBuf[cnt>0], "%d %d %d %d",
        pMatch->iCol, pMatch->iTerm, pMatch->iStart, pMatch->nByte);
    append(&sb, zBuf);
    cnt++;
  }
  p->zOffset = sb.s;
  p->nOffset = sb.len;
}

static int wordBoundary(
  int iBreak,                   
  const char *zDoc,             
  int nDoc,                     
  struct snippetMatch *aMatch,  
  int nMatch,                   
  int iCol                      
){
  int i;
  if( iBreak<=10 ){
    return 0;
  }
  if( iBreak>=nDoc-10 ){
    return nDoc;
  }
  for(i=0; i<nMatch && aMatch[i].iCol<iCol; i++){}
  while( i<nMatch && aMatch[i].iStart+aMatch[i].nByte<iBreak ){ i++; }
  if( i<nMatch ){
    if( aMatch[i].iStart<iBreak+10 ){
      return aMatch[i].iStart;
    }
    if( i>0 && aMatch[i-1].iStart+aMatch[i-1].nByte>=iBreak ){
      return aMatch[i-1].iStart;
    }
  }
  for(i=1; i<=10; i++){
    if( safe_isspace(zDoc[iBreak-i]) ){
      return iBreak - i + 1;
    }
    if( safe_isspace(zDoc[iBreak+i]) ){
      return iBreak + i + 1;
    }
  }
  return iBreak;
}

static void appendWhiteSpace(StringBuffer *p){
  if( p->len==0 ) return;
  if( safe_isspace(p->s[p->len-1]) ) return;
  append(p, " ");
}

static void trimWhiteSpace(StringBuffer *p){
  while( p->len>0 && safe_isspace(p->s[p->len-1]) ){
    p->len--;
  }
}



#define SNIPPET_IGNORE  0   
#define SNIPPET_DESIRED 1   

static void snippetText(
  fulltext_cursor *pCursor,   
  const char *zStartMark,     
  const char *zEndMark,       
  const char *zEllipsis       
){
  int i, j;
  struct snippetMatch *aMatch;
  int nMatch;
  int nDesired;
  StringBuffer sb;
  int tailCol;
  int tailOffset;
  int iCol;
  int nDoc;
  const char *zDoc;
  int iStart, iEnd;
  int tailEllipsis = 0;
  int iMatch;
  

  free(pCursor->snippet.zSnippet);
  pCursor->snippet.zSnippet = 0;
  aMatch = pCursor->snippet.aMatch;
  nMatch = pCursor->snippet.nMatch;
  initStringBuffer(&sb);

  for(i=0; i<nMatch; i++){
    aMatch[i].snStatus = SNIPPET_IGNORE;
  }
  nDesired = 0;
  for(i=0; i<pCursor->q.nTerms; i++){
    for(j=0; j<nMatch; j++){
      if( aMatch[j].iTerm==i ){
        aMatch[j].snStatus = SNIPPET_DESIRED;
        nDesired++;
        break;
      }
    }
  }

  iMatch = 0;
  tailCol = -1;
  tailOffset = 0;
  for(i=0; i<nMatch && nDesired>0; i++){
    if( aMatch[i].snStatus!=SNIPPET_DESIRED ) continue;
    nDesired--;
    iCol = aMatch[i].iCol;
    zDoc = (const char*)sqlite3_column_text(pCursor->pStmt, iCol+1);
    nDoc = sqlite3_column_bytes(pCursor->pStmt, iCol+1);
    iStart = aMatch[i].iStart - 40;
    iStart = wordBoundary(iStart, zDoc, nDoc, aMatch, nMatch, iCol);
    if( iStart<=10 ){
      iStart = 0;
    }
    if( iCol==tailCol && iStart<=tailOffset+20 ){
      iStart = tailOffset;
    }
    if( (iCol!=tailCol && tailCol>=0) || iStart!=tailOffset ){
      trimWhiteSpace(&sb);
      appendWhiteSpace(&sb);
      append(&sb, zEllipsis);
      appendWhiteSpace(&sb);
    }
    iEnd = aMatch[i].iStart + aMatch[i].nByte + 40;
    iEnd = wordBoundary(iEnd, zDoc, nDoc, aMatch, nMatch, iCol);
    if( iEnd>=nDoc-10 ){
      iEnd = nDoc;
      tailEllipsis = 0;
    }else{
      tailEllipsis = 1;
    }
    while( iMatch<nMatch && aMatch[iMatch].iCol<iCol ){ iMatch++; }
    while( iStart<iEnd ){
      while( iMatch<nMatch && aMatch[iMatch].iStart<iStart
             && aMatch[iMatch].iCol<=iCol ){
        iMatch++;
      }
      if( iMatch<nMatch && aMatch[iMatch].iStart<iEnd
             && aMatch[iMatch].iCol==iCol ){
        nappend(&sb, &zDoc[iStart], aMatch[iMatch].iStart - iStart);
        iStart = aMatch[iMatch].iStart;
        append(&sb, zStartMark);
        nappend(&sb, &zDoc[iStart], aMatch[iMatch].nByte);
        append(&sb, zEndMark);
        iStart += aMatch[iMatch].nByte;
        for(j=iMatch+1; j<nMatch; j++){
          if( aMatch[j].iTerm==aMatch[iMatch].iTerm
              && aMatch[j].snStatus==SNIPPET_DESIRED ){
            nDesired--;
            aMatch[j].snStatus = SNIPPET_IGNORE;
          }
        }
      }else{
        nappend(&sb, &zDoc[iStart], iEnd - iStart);
        iStart = iEnd;
      }
    }
    tailCol = iCol;
    tailOffset = iEnd;
  }
  trimWhiteSpace(&sb);
  if( tailEllipsis ){
    appendWhiteSpace(&sb);
    append(&sb, zEllipsis);
  }
  pCursor->snippet.zSnippet = sb.s;
  pCursor->snippet.nSnippet = sb.len;  
}


static int fulltextClose(sqlite3_vtab_cursor *pCursor){
  fulltext_cursor *c = (fulltext_cursor *) pCursor;
  TRACE(("FTS1 Close %p\n", c));
  sqlite3_finalize(c->pStmt);
  queryClear(&c->q);
  snippetClear(&c->snippet);
  if( c->result.pDoclist!=NULL ){
    docListDelete(c->result.pDoclist);
  }
  free(c);
  return SQLITE_OK;
}

static int fulltextNext(sqlite3_vtab_cursor *pCursor){
  fulltext_cursor *c = (fulltext_cursor *) pCursor;
  sqlite_int64 iDocid;
  int rc;

  TRACE(("FTS1 Next %p\n", pCursor));
  snippetClear(&c->snippet);
  if( c->iCursorType < QUERY_FULLTEXT ){
    
    rc = sqlite3_step(c->pStmt);
    switch( rc ){
      case SQLITE_ROW:
        c->eof = 0;
        return SQLITE_OK;
      case SQLITE_DONE:
        c->eof = 1;
        return SQLITE_OK;
      default:
        c->eof = 1;
        return rc;
    }
  } else {  
    rc = sqlite3_reset(c->pStmt);
    if( rc!=SQLITE_OK ) return rc;

    iDocid = nextDocid(&c->result);
    if( iDocid==0 ){
      c->eof = 1;
      return SQLITE_OK;
    }
    rc = sqlite3_bind_int64(c->pStmt, 1, iDocid);
    if( rc!=SQLITE_OK ) return rc;
    
    rc = sqlite3_step(c->pStmt);
    if( rc==SQLITE_ROW ){   
      c->eof = 0;
      return SQLITE_OK;
    }
    
    return rc==SQLITE_DONE ? SQLITE_ERROR : rc;
  }
}


static int docListOfTerm(
  fulltext_vtab *v,     
  int iColumn,          
  QueryTerm *pQTerm,    
  DocList **ppResult    
){
  DocList *pLeft, *pRight, *pNew;
  int i, rc;

  pLeft = docListNew(DL_POSITIONS);
  rc = term_select_all(v, iColumn, pQTerm->pTerm, pQTerm->nTerm, pLeft);
  if( rc ){
    docListDelete(pLeft);
    return rc;
  }
  for(i=1; i<=pQTerm->nPhrase; i++){
    pRight = docListNew(DL_POSITIONS);
    rc = term_select_all(v, iColumn, pQTerm[i].pTerm, pQTerm[i].nTerm, pRight);
    if( rc ){
      docListDelete(pLeft);
      return rc;
    }
    pNew = docListNew(i<pQTerm->nPhrase ? DL_POSITIONS : DL_DOCIDS);
    docListPhraseMerge(pLeft, pRight, pNew);
    docListDelete(pLeft);
    docListDelete(pRight);
    pLeft = pNew;
  }
  *ppResult = pLeft;
  return SQLITE_OK;
}

static void queryAdd(Query *q, const char *pTerm, int nTerm){
  QueryTerm *t;
  ++q->nTerms;
  q->pTerms = realloc(q->pTerms, q->nTerms * sizeof(q->pTerms[0]));
  if( q->pTerms==0 ){
    q->nTerms = 0;
    return;
  }
  t = &q->pTerms[q->nTerms - 1];
  memset(t, 0, sizeof(*t));
  t->pTerm = malloc(nTerm+1);
  memcpy(t->pTerm, pTerm, nTerm);
  t->pTerm[nTerm] = 0;
  t->nTerm = nTerm;
  t->isOr = q->nextIsOr;
  q->nextIsOr = 0;
  t->iColumn = q->nextColumn;
  q->nextColumn = q->dfltColumn;
}

static int checkColumnSpecifier(
  fulltext_vtab *pVtab,    
  const char *zToken,      
  int nToken               
){
  int i;
  for(i=0; i<pVtab->nColumn; i++){
    if( memcmp(pVtab->azColumn[i], zToken, nToken)==0
        && pVtab->azColumn[i][nToken]==0 ){
      return i;
    }
  }
  return -1;
}

static int tokenizeSegment(
  sqlite3_tokenizer *pTokenizer,          
  const char *pSegment, int nSegment,     
  int inPhrase,                           
  Query *pQuery                           
){
  const sqlite3_tokenizer_module *pModule = pTokenizer->pModule;
  sqlite3_tokenizer_cursor *pCursor;
  int firstIndex = pQuery->nTerms;
  int iCol;
  int nTerm = 1;
  
  int rc = pModule->xOpen(pTokenizer, pSegment, nSegment, &pCursor);
  if( rc!=SQLITE_OK ) return rc;
  pCursor->pTokenizer = pTokenizer;

  while( 1 ){
    const char *pToken;
    int nToken, iBegin, iEnd, iPos;

    rc = pModule->xNext(pCursor,
                        &pToken, &nToken,
                        &iBegin, &iEnd, &iPos);
    if( rc!=SQLITE_OK ) break;
    if( !inPhrase &&
        pSegment[iEnd]==':' &&
         (iCol = checkColumnSpecifier(pQuery->pFts, pToken, nToken))>=0 ){
      pQuery->nextColumn = iCol;
      continue;
    }
    if( !inPhrase && pQuery->nTerms>0 && nToken==2
         && pSegment[iBegin]=='O' && pSegment[iBegin+1]=='R' ){
      pQuery->nextIsOr = 1;
      continue;
    }
    queryAdd(pQuery, pToken, nToken);
    if( !inPhrase && iBegin>0 && pSegment[iBegin-1]=='-' ){
      pQuery->pTerms[pQuery->nTerms-1].isNot = 1;
    }
    pQuery->pTerms[pQuery->nTerms-1].iPhrase = nTerm;
    if( inPhrase ){
      nTerm++;
    }
  }

  if( inPhrase && pQuery->nTerms>firstIndex ){
    pQuery->pTerms[firstIndex].nPhrase = pQuery->nTerms - firstIndex - 1;
  }

  return pModule->xClose(pCursor);
}

static int parseQuery(
  fulltext_vtab *v,        
  const char *zInput,      
  int nInput,              
  int dfltColumn,          
  Query *pQuery            
){
  int iInput, inPhrase = 0;

  if( zInput==0 ) nInput = 0;
  if( nInput<0 ) nInput = strlen(zInput);
  pQuery->nTerms = 0;
  pQuery->pTerms = NULL;
  pQuery->nextIsOr = 0;
  pQuery->nextColumn = dfltColumn;
  pQuery->dfltColumn = dfltColumn;
  pQuery->pFts = v;

  for(iInput=0; iInput<nInput; ++iInput){
    int i;
    for(i=iInput; i<nInput && zInput[i]!='"'; ++i){}
    if( i>iInput ){
      tokenizeSegment(v->pTokenizer, zInput+iInput, i-iInput, inPhrase,
                       pQuery);
    }
    iInput = i;
    if( i<nInput ){
      assert( zInput[i]=='"' );
      inPhrase = !inPhrase;
    }
  }

  if( inPhrase ){
    
    queryClear(pQuery);
    return SQLITE_ERROR;
  }
  return SQLITE_OK;
}

static int fulltextQuery(
  fulltext_vtab *v,      
  int iColumn,           
  const char *zInput,    
  int nInput,            
  DocList **pResult,     
  Query *pQuery          
){
  int i, iNext, rc;
  DocList *pLeft = NULL;
  DocList *pRight, *pNew, *pOr;
  int nNot = 0;
  QueryTerm *aTerm;

  rc = parseQuery(v, zInput, nInput, iColumn, pQuery);
  if( rc!=SQLITE_OK ) return rc;

  
  aTerm = pQuery->pTerms;
  for(i = 0; i<pQuery->nTerms; i=iNext){
    if( aTerm[i].isNot ){
      
      nNot++;
      iNext = i + aTerm[i].nPhrase+1;
      continue;
    }
    iNext = i + aTerm[i].nPhrase + 1;
    rc = docListOfTerm(v, aTerm[i].iColumn, &aTerm[i], &pRight);
    if( rc ){
      queryClear(pQuery);
      return rc;
    }
    while( iNext<pQuery->nTerms && aTerm[iNext].isOr ){
      rc = docListOfTerm(v, aTerm[iNext].iColumn, &aTerm[iNext], &pOr);
      iNext += aTerm[iNext].nPhrase + 1;
      if( rc ){
        queryClear(pQuery);
        return rc;
      }
      pNew = docListNew(DL_DOCIDS);
      docListOrMerge(pRight, pOr, pNew);
      docListDelete(pRight);
      docListDelete(pOr);
      pRight = pNew;
    }
    if( pLeft==0 ){
      pLeft = pRight;
    }else{
      pNew = docListNew(DL_DOCIDS);
      docListAndMerge(pLeft, pRight, pNew);
      docListDelete(pRight);
      docListDelete(pLeft);
      pLeft = pNew;
    }
  }

  if( nNot && pLeft==0 ){
    
    return SQLITE_ERROR;
  }

  
  for(i=0; i<pQuery->nTerms;  i += aTerm[i].nPhrase + 1){
    if( !aTerm[i].isNot ) continue;
    rc = docListOfTerm(v, aTerm[i].iColumn, &aTerm[i], &pRight);
    if( rc ){
      queryClear(pQuery);
      docListDelete(pLeft);
      return rc;
    }
    pNew = docListNew(DL_DOCIDS);
    docListExceptMerge(pLeft, pRight, pNew);
    docListDelete(pRight);
    docListDelete(pLeft);
    pLeft = pNew;
  }

  *pResult = pLeft;
  return rc;
}

static int fulltextFilter(
  sqlite3_vtab_cursor *pCursor,     
  int idxNum, const char *idxStr,   
  int argc, sqlite3_value **argv    
){
  fulltext_cursor *c = (fulltext_cursor *) pCursor;
  fulltext_vtab *v = cursor_vtab(c);
  int rc;
  char *zSql;

  TRACE(("FTS1 Filter %p\n",pCursor));

  zSql = sqlite3_mprintf("select rowid, * from %%_content %s",
                          idxNum==QUERY_GENERIC ? "" : "where rowid=?");
  sqlite3_finalize(c->pStmt);
  rc = sql_prepare(v->db, v->zDb, v->zName, &c->pStmt, zSql);
  sqlite3_free(zSql);
  if( rc!=SQLITE_OK ) return rc;

  c->iCursorType = idxNum;
  switch( idxNum ){
    case QUERY_GENERIC:
      break;

    case QUERY_ROWID:
      rc = sqlite3_bind_int64(c->pStmt, 1, sqlite3_value_int64(argv[0]));
      if( rc!=SQLITE_OK ) return rc;
      break;

    default:   
    {
      const char *zQuery = (const char *)sqlite3_value_text(argv[0]);
      DocList *pResult;
      assert( idxNum<=QUERY_FULLTEXT+v->nColumn);
      assert( argc==1 );
      queryClear(&c->q);
      rc = fulltextQuery(v, idxNum-QUERY_FULLTEXT, zQuery, -1, &pResult, &c->q);
      if( rc!=SQLITE_OK ) return rc;
      if( c->result.pDoclist!=NULL ) docListDelete(c->result.pDoclist);
      readerInit(&c->result, pResult);
      break;
    }
  }

  return fulltextNext(pCursor);
}

static int fulltextEof(sqlite3_vtab_cursor *pCursor){
  fulltext_cursor *c = (fulltext_cursor *) pCursor;
  return c->eof;
}

static int fulltextColumn(sqlite3_vtab_cursor *pCursor,
                          sqlite3_context *pContext, int idxCol){
  fulltext_cursor *c = (fulltext_cursor *) pCursor;
  fulltext_vtab *v = cursor_vtab(c);

  if( idxCol<v->nColumn ){
    sqlite3_value *pVal = sqlite3_column_value(c->pStmt, idxCol+1);
    sqlite3_result_value(pContext, pVal);
  }else if( idxCol==v->nColumn ){
    sqlite3_result_blob(pContext, &c, sizeof(c), SQLITE_TRANSIENT);
  }
  return SQLITE_OK;
}

/* This is the xRowid method.  The SQLite core calls this routine to
** retrive the rowid for the current row of the result set.  The
** rowid should be written to *pRowid.
*/
static int fulltextRowid(sqlite3_vtab_cursor *pCursor, sqlite_int64 *pRowid){
  fulltext_cursor *c = (fulltext_cursor *) pCursor;

  *pRowid = sqlite3_column_int64(c->pStmt, 0);
  return SQLITE_OK;
}

static int buildTerms(fulltext_vtab *v, fts1Hash *terms, sqlite_int64 iDocid,
                      const char *zText, int iColumn){
  sqlite3_tokenizer *pTokenizer = v->pTokenizer;
  sqlite3_tokenizer_cursor *pCursor;
  const char *pToken;
  int nTokenBytes;
  int iStartOffset, iEndOffset, iPosition;
  int rc;

  rc = pTokenizer->pModule->xOpen(pTokenizer, zText, -1, &pCursor);
  if( rc!=SQLITE_OK ) return rc;

  pCursor->pTokenizer = pTokenizer;
  while( SQLITE_OK==pTokenizer->pModule->xNext(pCursor,
                                               &pToken, &nTokenBytes,
                                               &iStartOffset, &iEndOffset,
                                               &iPosition) ){
    DocList *p;

    
    if( iPosition<0 ){
      pTokenizer->pModule->xClose(pCursor);
      return SQLITE_ERROR;
    }

    p = fts1HashFind(terms, pToken, nTokenBytes);
    if( p==NULL ){
      p = docListNew(DL_DEFAULT);
      docListAddDocid(p, iDocid);
      fts1HashInsert(terms, pToken, nTokenBytes, p);
    }
    if( iColumn>=0 ){
      docListAddPosOffset(p, iColumn, iPosition, iStartOffset, iEndOffset);
    }
  }

  pTokenizer->pModule->xClose(pCursor);
  return rc;
}

static int index_insert_term(fulltext_vtab *v, const char *pTerm, int nTerm,
                             DocList *d){
  sqlite_int64 iIndexRow;
  DocList doclist;
  int iSegment = 0, rc;

  rc = term_select(v, pTerm, nTerm, iSegment, &iIndexRow, &doclist);
  if( rc==SQLITE_DONE ){
    docListInit(&doclist, DL_DEFAULT, 0, 0);
    docListUpdate(&doclist, d);
    
    rc = term_insert(v, NULL, pTerm, nTerm, iSegment, &doclist);
    goto err;
  }
  if( rc!=SQLITE_ROW ) return SQLITE_ERROR;

  docListUpdate(&doclist, d);
  if( doclist.nData<=CHUNK_MAX ){
    rc = term_update(v, iIndexRow, &doclist);
    goto err;
  }

  rc = term_delete(v, iIndexRow);
  if( rc!=SQLITE_OK ) goto err;

  iSegment++;
  while( (rc=term_insert(v, &iIndexRow, pTerm, nTerm, iSegment,
                         &doclist))!=SQLITE_OK ){
    sqlite_int64 iSegmentRow;
    DocList old;
    int rc2;

    rc2 = term_select(v, pTerm, nTerm, iSegment, &iSegmentRow, &old);
    if( rc2!=SQLITE_ROW ) goto err;

    rc = term_delete(v, iSegmentRow);
    if( rc!=SQLITE_OK ) goto err;

    
    if( iSegmentRow<iIndexRow ) iIndexRow = iSegmentRow;

    docListAccumulate(&old, &doclist);
    docListDestroy(&doclist);
    doclist = old;

    iSegment++;
  }

 err:
  docListDestroy(&doclist);
  return rc;
}

static int insertTerms(fulltext_vtab *v, fts1Hash *terms, sqlite_int64 iRowid,
                sqlite3_value **pValues){
  int i;
  for(i = 0; i < v->nColumn ; ++i){
    char *zText = (char*)sqlite3_value_text(pValues[i]);
    int rc = buildTerms(v, terms, iRowid, zText, i);
    if( rc!=SQLITE_OK ) return rc;
  }
  return SQLITE_OK;
}

static int deleteTerms(fulltext_vtab *v, fts1Hash *pTerms, sqlite_int64 iRowid){
  const char **pValues;
  int i;

  int rc = content_select(v, iRowid, &pValues);
  if( rc!=SQLITE_OK ) return rc;

  for(i = 0 ; i < v->nColumn; ++i) {
    rc = buildTerms(v, pTerms, iRowid, pValues[i], -1);
    if( rc!=SQLITE_OK ) break;
  }

  freeStringArray(v->nColumn, pValues);
  return SQLITE_OK;
}

static int index_insert(fulltext_vtab *v, sqlite3_value *pRequestRowid,
                        sqlite3_value **pValues,
                        sqlite_int64 *piRowid, fts1Hash *pTerms){
  int rc;

  rc = content_insert(v, pRequestRowid, pValues);  
  if( rc!=SQLITE_OK ) return rc;
  *piRowid = sqlite3_last_insert_rowid(v->db);
  return insertTerms(v, pTerms, *piRowid, pValues);
}

/* Delete a row from the %_content table; fill [pTerms] with empty doclists
 * to be written to the %_term table. */
static int index_delete(fulltext_vtab *v, sqlite_int64 iRow, fts1Hash *pTerms){
  int rc = deleteTerms(v, pTerms, iRow);
  if( rc!=SQLITE_OK ) return rc;
  return content_delete(v, iRow);  
}

static int index_update(fulltext_vtab *v, sqlite_int64 iRow,
                        sqlite3_value **pValues, fts1Hash *pTerms){
  int rc = deleteTerms(v, pTerms, iRow);
  if( rc!=SQLITE_OK ) return rc;

  rc = content_update(v, pValues, iRow);  
  if( rc!=SQLITE_OK ) return rc;

  
  return insertTerms(v, pTerms, iRow, pValues);
}

static int fulltextUpdate(sqlite3_vtab *pVtab, int nArg, sqlite3_value **ppArg,
                   sqlite_int64 *pRowid){
  fulltext_vtab *v = (fulltext_vtab *) pVtab;
  fts1Hash terms;   
  int rc;
  fts1HashElem *e;

  TRACE(("FTS1 Update %p\n", pVtab));
  
  fts1HashInit(&terms, FTS1_HASH_STRING, 1);

  if( nArg<2 ){
    rc = index_delete(v, sqlite3_value_int64(ppArg[0]), &terms);
  } else if( sqlite3_value_type(ppArg[0]) != SQLITE_NULL ){
    sqlite_int64 rowid = sqlite3_value_int64(ppArg[0]);
    if( sqlite3_value_type(ppArg[1]) != SQLITE_INTEGER ||
      sqlite3_value_int64(ppArg[1]) != rowid ){
      rc = SQLITE_ERROR;  
    } else {
      assert( nArg==2+v->nColumn+1);
      rc = index_update(v, rowid, &ppArg[2], &terms);
    }
  } else {
    assert( nArg==2+v->nColumn+1);
    rc = index_insert(v, ppArg[1], &ppArg[2], pRowid, &terms);
  }

  if( rc==SQLITE_OK ){
    
    for(e=fts1HashFirst(&terms); e; e=fts1HashNext(e)){
      DocList *p = fts1HashData(e);
      rc = index_insert_term(v, fts1HashKey(e), fts1HashKeysize(e), p);
      if( rc!=SQLITE_OK ) break;
    }
  }

  
  for(e=fts1HashFirst(&terms); e; e=fts1HashNext(e)){
    DocList *p = fts1HashData(e);
    docListDelete(p);
  }
  fts1HashClear(&terms);

  return rc;
}

static void snippetFunc(
  sqlite3_context *pContext,
  int argc,
  sqlite3_value **argv
){
  fulltext_cursor *pCursor;
  if( argc<1 ) return;
  if( sqlite3_value_type(argv[0])!=SQLITE_BLOB ||
      sqlite3_value_bytes(argv[0])!=sizeof(pCursor) ){
    sqlite3_result_error(pContext, "illegal first argument to html_snippet",-1);
  }else{
    const char *zStart = "<b>";
    const char *zEnd = "</b>";
    const char *zEllipsis = "<b>...</b>";
    memcpy(&pCursor, sqlite3_value_blob(argv[0]), sizeof(pCursor));
    if( argc>=2 ){
      zStart = (const char*)sqlite3_value_text(argv[1]);
      if( argc>=3 ){
        zEnd = (const char*)sqlite3_value_text(argv[2]);
        if( argc>=4 ){
          zEllipsis = (const char*)sqlite3_value_text(argv[3]);
        }
      }
    }
    snippetAllOffsets(pCursor);
    snippetText(pCursor, zStart, zEnd, zEllipsis);
    sqlite3_result_text(pContext, pCursor->snippet.zSnippet,
                        pCursor->snippet.nSnippet, SQLITE_STATIC);
  }
}

static void snippetOffsetsFunc(
  sqlite3_context *pContext,
  int argc,
  sqlite3_value **argv
){
  fulltext_cursor *pCursor;
  if( argc<1 ) return;
  if( sqlite3_value_type(argv[0])!=SQLITE_BLOB ||
      sqlite3_value_bytes(argv[0])!=sizeof(pCursor) ){
    sqlite3_result_error(pContext, "illegal first argument to offsets",-1);
  }else{
    memcpy(&pCursor, sqlite3_value_blob(argv[0]), sizeof(pCursor));
    snippetAllOffsets(pCursor);
    snippetOffsetText(&pCursor->snippet);
    sqlite3_result_text(pContext,
                        pCursor->snippet.zOffset, pCursor->snippet.nOffset,
                        SQLITE_STATIC);
  }
}

static int fulltextFindFunction(
  sqlite3_vtab *pVtab,
  int nArg,
  const char *zName,
  void (**pxFunc)(sqlite3_context*,int,sqlite3_value**),
  void **ppArg
){
  if( strcmp(zName,"snippet")==0 ){
    *pxFunc = snippetFunc;
    return 1;
  }else if( strcmp(zName,"offsets")==0 ){
    *pxFunc = snippetOffsetsFunc;
    return 1;
  }
  return 0;
}

static int fulltextRename(
  sqlite3_vtab *pVtab,
  const char *zName
){
  fulltext_vtab *p = (fulltext_vtab *)pVtab;
  int rc = SQLITE_NOMEM;
  char *zSql = sqlite3_mprintf(
    "ALTER TABLE %Q.'%q_content'  RENAME TO '%q_content';"
    "ALTER TABLE %Q.'%q_term' RENAME TO '%q_term';"
    , p->zDb, p->zName, zName
    , p->zDb, p->zName, zName
  );
  if( zSql ){
    rc = sqlite3_exec(p->db, zSql, 0, 0, 0);
    sqlite3_free(zSql);
  }
  return rc;
}

static const sqlite3_module fulltextModule = {
   0,
   fulltextCreate,
   fulltextConnect,
   fulltextBestIndex,
   fulltextDisconnect,
   fulltextDestroy,
   fulltextOpen,
   fulltextClose,
   fulltextFilter,
   fulltextNext,
   fulltextEof,
   fulltextColumn,
   fulltextRowid,
   fulltextUpdate,
   0, 
   0,
   0,
   0,
   fulltextFindFunction,
   fulltextRename,
};

int sqlite3Fts1Init(sqlite3 *db){
  sqlite3_overload_function(db, "snippet", -1);
  sqlite3_overload_function(db, "offsets", -1);
  return sqlite3_create_module(db, "fts1", &fulltextModule, 0);
}

#if !SQLITE_CORE
int sqlite3_extension_init(sqlite3 *db, char **pzErrMsg,
                           const sqlite3_api_routines *pApi){
  SQLITE_EXTENSION_INIT2(pApi)
  return sqlite3Fts1Init(db);
}
#endif

#endif 

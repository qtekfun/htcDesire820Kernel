/*
** 2009 Nov 12
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
******************************************************************************
**
*/

#ifndef _FTSINT_H
#define _FTSINT_H

#if !defined(NDEBUG) && !defined(SQLITE_DEBUG) 
# define NDEBUG 1
#endif

#include "sqlite3.h"
#include "fts3_tokenizer.h"
#include "fts3_hash.h"

#define FTS3_MERGE_COUNT 16

#define FTS3_MAX_PENDING_DATA (1*1024*1024)

#define SizeofArray(X) ((int)(sizeof(X)/sizeof(X[0])))

#define FTS3_VARINT_MAX 10

#ifndef testcase
# define testcase(X)
#endif

#define POS_COLUMN  (1)     
#define POS_END     (0)      

#ifndef SQLITE_AMALGAMATION
#ifdef SQLITE_COVERAGE_TEST
# define ALWAYS(x) (1)
# define NEVER(X)  (0)
#else
# define ALWAYS(x) (x)
# define NEVER(X)  (x)
#endif

typedef unsigned char u8;         
typedef short int i16;            
typedef unsigned int u32;         
typedef sqlite3_uint64 u64;       
#define UNUSED_PARAMETER(x) (void)(x)
#endif

typedef struct Fts3Table Fts3Table;
typedef struct Fts3Cursor Fts3Cursor;
typedef struct Fts3Expr Fts3Expr;
typedef struct Fts3Phrase Fts3Phrase;
typedef struct Fts3PhraseToken Fts3PhraseToken;

typedef struct Fts3SegFilter Fts3SegFilter;
typedef struct Fts3DeferredToken Fts3DeferredToken;
typedef struct Fts3SegReader Fts3SegReader;
typedef struct Fts3SegReaderCursor Fts3SegReaderCursor;

struct Fts3Table {
  sqlite3_vtab base;              
  sqlite3 *db;                    
  const char *zDb;                
  const char *zName;              
  int nColumn;                    
  char **azColumn;                
  sqlite3_tokenizer *pTokenizer;  

  sqlite3_stmt *aStmt[24];

  char *zReadExprlist;
  char *zWriteExprlist;

  int nNodeSize;                  
  u8 bHasStat;                    
  u8 bHasDocsize;                 
  int nPgsz;                      
  char *zSegmentsTbl;             
  sqlite3_blob *pSegments;        

  int nMaxPendingData;
  int nPendingData;
  sqlite_int64 iPrevDocid;
  Fts3Hash pendingTerms;
};

struct Fts3Cursor {
  sqlite3_vtab_cursor base;       
  i16 eSearch;                    
  u8 isEof;                       
  u8 isRequireSeek;               
  sqlite3_stmt *pStmt;            
  Fts3Expr *pExpr;                
  int nPhrase;                    
  Fts3DeferredToken *pDeferred;   
  sqlite3_int64 iPrevId;          
  char *pNextId;                  
  char *aDoclist;                 
  int nDoclist;                   
  int eEvalmode;                  
  int nRowAvg;                    

  int isMatchinfoNeeded;          
  u32 *aMatchinfo;                
  int nMatchinfo;                 
  char *zMatchinfo;               
};

#define FTS3_EVAL_FILTER    0
#define FTS3_EVAL_NEXT      1
#define FTS3_EVAL_MATCHINFO 2

#define FTS3_FULLSCAN_SEARCH 0    
#define FTS3_DOCID_SEARCH    1    
#define FTS3_FULLTEXT_SEARCH 2    

struct Fts3PhraseToken {
  char *z;                        
  int n;                          
  int isPrefix;                   
  int bFulltext;                  
  Fts3SegReaderCursor *pSegcsr;   
  Fts3DeferredToken *pDeferred;   
};

struct Fts3Phrase {
  
  int nToken;                
  int iColumn;               
  int isNot;                 
  Fts3PhraseToken aToken[1]; 
};

struct Fts3Expr {
  int eType;                 
  int nNear;                 
  Fts3Expr *pParent;         
  Fts3Expr *pLeft;           
  Fts3Expr *pRight;          
  Fts3Phrase *pPhrase;       

  int isLoaded;              
  char *aDoclist;            
  int nDoclist;              

  sqlite3_int64 iCurrent;
  char *pCurrent;
};

#define FTSQUERY_NEAR   1
#define FTSQUERY_NOT    2
#define FTSQUERY_AND    3
#define FTSQUERY_OR     4
#define FTSQUERY_PHRASE 5


int sqlite3Fts3UpdateMethod(sqlite3_vtab*,int,sqlite3_value**,sqlite3_int64*);
int sqlite3Fts3PendingTermsFlush(Fts3Table *);
void sqlite3Fts3PendingTermsClear(Fts3Table *);
int sqlite3Fts3Optimize(Fts3Table *);
int sqlite3Fts3SegReaderNew(int, sqlite3_int64,
  sqlite3_int64, sqlite3_int64, const char *, int, Fts3SegReader**);
int sqlite3Fts3SegReaderPending(Fts3Table*,const char*,int,int,Fts3SegReader**);
void sqlite3Fts3SegReaderFree(Fts3SegReader *);
int sqlite3Fts3SegReaderCost(Fts3Cursor *, Fts3SegReader *, int *);
int sqlite3Fts3AllSegdirs(Fts3Table*, int, sqlite3_stmt **);
int sqlite3Fts3ReadLock(Fts3Table *);
int sqlite3Fts3ReadBlock(Fts3Table*, sqlite3_int64, char **, int*);

int sqlite3Fts3SelectDoctotal(Fts3Table *, sqlite3_stmt **);
int sqlite3Fts3SelectDocsize(Fts3Table *, sqlite3_int64, sqlite3_stmt **);

void sqlite3Fts3FreeDeferredTokens(Fts3Cursor *);
int sqlite3Fts3DeferToken(Fts3Cursor *, Fts3PhraseToken *, int);
int sqlite3Fts3CacheDeferredDoclists(Fts3Cursor *);
void sqlite3Fts3FreeDeferredDoclists(Fts3Cursor *);
char *sqlite3Fts3DeferredDoclist(Fts3DeferredToken *, int *);
void sqlite3Fts3SegmentsClose(Fts3Table *);

#define FTS3_SEGCURSOR_PENDING -1
#define FTS3_SEGCURSOR_ALL     -2

int sqlite3Fts3SegReaderStart(Fts3Table*, Fts3SegReaderCursor*, Fts3SegFilter*);
int sqlite3Fts3SegReaderStep(Fts3Table *, Fts3SegReaderCursor *);
void sqlite3Fts3SegReaderFinish(Fts3SegReaderCursor *);
int sqlite3Fts3SegReaderCursor(
    Fts3Table *, int, const char *, int, int, int, Fts3SegReaderCursor *);

#define FTS3_SEGMENT_REQUIRE_POS   0x00000001
#define FTS3_SEGMENT_IGNORE_EMPTY  0x00000002
#define FTS3_SEGMENT_COLUMN_FILTER 0x00000004
#define FTS3_SEGMENT_PREFIX        0x00000008
#define FTS3_SEGMENT_SCAN          0x00000010

struct Fts3SegFilter {
  const char *zTerm;
  int nTerm;
  int iCol;
  int flags;
};

struct Fts3SegReaderCursor {
  
  Fts3SegReader **apSegment;      
  int nSegment;                   
  int nAdvance;                   
  Fts3SegFilter *pFilter;         
  char *aBuffer;                  
  int nBuffer;                    

  
  int nCost;

  
  char *zTerm;                    
  int nTerm;                      
  char *aDoclist;                 
  int nDoclist;                   
};

int sqlite3Fts3PutVarint(char *, sqlite3_int64);
int sqlite3Fts3GetVarint(const char *, sqlite_int64 *);
int sqlite3Fts3GetVarint32(const char *, int *);
int sqlite3Fts3VarintLen(sqlite3_uint64);
void sqlite3Fts3Dequote(char *);

char *sqlite3Fts3FindPositions(Fts3Expr *, sqlite3_int64, int);
int sqlite3Fts3ExprLoadDoclist(Fts3Cursor *, Fts3Expr *);
int sqlite3Fts3ExprLoadFtDoclist(Fts3Cursor *, Fts3Expr *, char **, int *);
int sqlite3Fts3ExprNearTrim(Fts3Expr *, Fts3Expr *, int);

const char *sqlite3Fts3NextToken(const char *, int *);
int sqlite3Fts3InitHashTable(sqlite3 *, Fts3Hash *, const char *);
int sqlite3Fts3InitTokenizer(Fts3Hash *pHash, const char *, 
    sqlite3_tokenizer **, char **
);
int sqlite3Fts3IsIdChar(char);

void sqlite3Fts3Offsets(sqlite3_context*, Fts3Cursor*);
void sqlite3Fts3Snippet(sqlite3_context *, Fts3Cursor *, const char *,
  const char *, const char *, int, int
);
void sqlite3Fts3Matchinfo(sqlite3_context *, Fts3Cursor *, const char *);

int sqlite3Fts3ExprParse(sqlite3_tokenizer *, 
  char **, int, int, const char *, int, Fts3Expr **
);
void sqlite3Fts3ExprFree(Fts3Expr *);
#ifdef SQLITE_TEST
int sqlite3Fts3ExprInitTestInterface(sqlite3 *db);
#endif

int sqlite3Fts3InitAux(sqlite3 *db);

#endif 

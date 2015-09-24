/*
** 2009 Oct 23
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
** This file is part of the SQLite FTS3 extension module. Specifically,
** this file contains code to insert, update and delete rows from FTS3
** tables. It also contains code to merge FTS3 b-tree segments. Some
** of the sub-routines used to merge segments are also used by the query 
** code in fts3.c.
*/

#if !defined(SQLITE_CORE) || defined(SQLITE_ENABLE_FTS3)

#include "fts3Int.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define FTS3_NODE_PADDING (FTS3_VARINT_MAX*2)

typedef struct PendingList PendingList;
typedef struct SegmentNode SegmentNode;
typedef struct SegmentWriter SegmentWriter;

struct PendingList {
  int nData;
  char *aData;
  int nSpace;
  sqlite3_int64 iLastDocid;
  sqlite3_int64 iLastCol;
  sqlite3_int64 iLastPos;
};


struct Fts3DeferredToken {
  Fts3PhraseToken *pToken;        
  int iCol;                       
  Fts3DeferredToken *pNext;       
  PendingList *pList;             
};

struct Fts3SegReader {
  int iIdx;                       

  sqlite3_int64 iStartBlock;      
  sqlite3_int64 iLeafEndBlock;    
  sqlite3_int64 iEndBlock;        
  sqlite3_int64 iCurrentBlock;    

  char *aNode;                    
  int nNode;                      
  Fts3HashElem **ppNextElem;

  int nTerm;                      
  char *zTerm;                    
  int nTermAlloc;                 
  char *aDoclist;                 
  int nDoclist;                   

  
  char *pOffsetList;
  sqlite3_int64 iDocid;
};

#define fts3SegReaderIsPending(p) ((p)->ppNextElem!=0)
#define fts3SegReaderIsRootOnly(p) ((p)->aNode==(char *)&(p)[1])

struct SegmentWriter {
  SegmentNode *pTree;             
  sqlite3_int64 iFirst;           /* First slot in %_segments written */
  sqlite3_int64 iFree;            
  char *zTerm;                    
  int nTerm;                      
  int nMalloc;                    
  char *zMalloc;                  
  int nSize;                      
  int nData;                      
  char *aData;                    
};

struct SegmentNode {
  SegmentNode *pParent;           
  SegmentNode *pRight;            
  SegmentNode *pLeftmost;         
  int nEntry;                     /* Number of terms written to node so far */
  char *zTerm;                    
  int nTerm;                      
  int nMalloc;                    
  char *zMalloc;                  
  int nData;                      
  char *aData;                    
};

#define SQL_DELETE_CONTENT             0
#define SQL_IS_EMPTY                   1
#define SQL_DELETE_ALL_CONTENT         2 
#define SQL_DELETE_ALL_SEGMENTS        3
#define SQL_DELETE_ALL_SEGDIR          4
#define SQL_DELETE_ALL_DOCSIZE         5
#define SQL_DELETE_ALL_STAT            6
#define SQL_SELECT_CONTENT_BY_ROWID    7
#define SQL_NEXT_SEGMENT_INDEX         8
#define SQL_INSERT_SEGMENTS            9
#define SQL_NEXT_SEGMENTS_ID          10
#define SQL_INSERT_SEGDIR             11
#define SQL_SELECT_LEVEL              12
#define SQL_SELECT_ALL_LEVEL          13
#define SQL_SELECT_LEVEL_COUNT        14
#define SQL_SELECT_SEGDIR_COUNT_MAX   15
#define SQL_DELETE_SEGDIR_BY_LEVEL    16
#define SQL_DELETE_SEGMENTS_RANGE     17
#define SQL_CONTENT_INSERT            18
#define SQL_DELETE_DOCSIZE            19
#define SQL_REPLACE_DOCSIZE           20
#define SQL_SELECT_DOCSIZE            21
#define SQL_SELECT_DOCTOTAL           22
#define SQL_REPLACE_DOCTOTAL          23

static int fts3SqlStmt(
  Fts3Table *p,                   
  int eStmt,                      
  sqlite3_stmt **pp,              
  sqlite3_value **apVal           
){
  const char *azSql[] = {
  "DELETE FROM %Q.'%q_content' WHERE rowid = ?",
  "SELECT NOT EXISTS(SELECT docid FROM %Q.'%q_content' WHERE rowid!=?)",
  "DELETE FROM %Q.'%q_content'",
  "DELETE FROM %Q.'%q_segments'",
  "DELETE FROM %Q.'%q_segdir'",
  "DELETE FROM %Q.'%q_docsize'",
  "DELETE FROM %Q.'%q_stat'",
  "SELECT %s FROM %Q.'%q_content' AS x WHERE rowid=?",
  "SELECT (SELECT max(idx) FROM %Q.'%q_segdir' WHERE level = ?) + 1",
  "INSERT INTO %Q.'%q_segments'(blockid, block) VALUES(?, ?)",
  "SELECT coalesce((SELECT max(blockid) FROM %Q.'%q_segments') + 1, 1)",
  "INSERT INTO %Q.'%q_segdir' VALUES(?,?,?,?,?,?)",

           
  "SELECT idx, start_block, leaves_end_block, end_block, root "
            "FROM %Q.'%q_segdir' WHERE level = ? ORDER BY idx ASC",
  "SELECT idx, start_block, leaves_end_block, end_block, root "
            "FROM %Q.'%q_segdir' ORDER BY level DESC, idx ASC",

  "SELECT count(*) FROM %Q.'%q_segdir' WHERE level = ?",
  "SELECT count(*), max(level) FROM %Q.'%q_segdir'",

  "DELETE FROM %Q.'%q_segdir' WHERE level = ?",
  "DELETE FROM %Q.'%q_segments' WHERE blockid BETWEEN ? AND ?",
  "INSERT INTO %Q.'%q_content' VALUES(%s)",
  "DELETE FROM %Q.'%q_docsize' WHERE docid = ?",
  "REPLACE INTO %Q.'%q_docsize' VALUES(?,?)",
  "SELECT size FROM %Q.'%q_docsize' WHERE docid=?",
  "SELECT value FROM %Q.'%q_stat' WHERE id=0",
  "REPLACE INTO %Q.'%q_stat' VALUES(0,?)",
  };
  int rc = SQLITE_OK;
  sqlite3_stmt *pStmt;

  assert( SizeofArray(azSql)==SizeofArray(p->aStmt) );
  assert( eStmt<SizeofArray(azSql) && eStmt>=0 );
  
  pStmt = p->aStmt[eStmt];
  if( !pStmt ){
    char *zSql;
    if( eStmt==SQL_CONTENT_INSERT ){
      zSql = sqlite3_mprintf(azSql[eStmt], p->zDb, p->zName, p->zWriteExprlist);
    }else if( eStmt==SQL_SELECT_CONTENT_BY_ROWID ){
      zSql = sqlite3_mprintf(azSql[eStmt], p->zReadExprlist, p->zDb, p->zName);
    }else{
      zSql = sqlite3_mprintf(azSql[eStmt], p->zDb, p->zName);
    }
    if( !zSql ){
      rc = SQLITE_NOMEM;
    }else{
      rc = sqlite3_prepare_v2(p->db, zSql, -1, &pStmt, NULL);
      sqlite3_free(zSql);
      assert( rc==SQLITE_OK || pStmt==0 );
      p->aStmt[eStmt] = pStmt;
    }
  }
  if( apVal ){
    int i;
    int nParam = sqlite3_bind_parameter_count(pStmt);
    for(i=0; rc==SQLITE_OK && i<nParam; i++){
      rc = sqlite3_bind_value(pStmt, i+1, apVal[i]);
    }
  }
  *pp = pStmt;
  return rc;
}

static int fts3SelectDocsize(
  Fts3Table *pTab,                
  int eStmt,                      
  sqlite3_int64 iDocid,           
  sqlite3_stmt **ppStmt           
){
  sqlite3_stmt *pStmt = 0;        
  int rc;                         

  assert( eStmt==SQL_SELECT_DOCSIZE || eStmt==SQL_SELECT_DOCTOTAL );

  rc = fts3SqlStmt(pTab, eStmt, &pStmt, 0);
  if( rc==SQLITE_OK ){
    if( eStmt==SQL_SELECT_DOCSIZE ){
      sqlite3_bind_int64(pStmt, 1, iDocid);
    }
    rc = sqlite3_step(pStmt);
    if( rc!=SQLITE_ROW || sqlite3_column_type(pStmt, 0)!=SQLITE_BLOB ){
      rc = sqlite3_reset(pStmt);
      if( rc==SQLITE_OK ) rc = SQLITE_CORRUPT;
      pStmt = 0;
    }else{
      rc = SQLITE_OK;
    }
  }

  *ppStmt = pStmt;
  return rc;
}

int sqlite3Fts3SelectDoctotal(
  Fts3Table *pTab,                
  sqlite3_stmt **ppStmt           
){
  return fts3SelectDocsize(pTab, SQL_SELECT_DOCTOTAL, 0, ppStmt);
}

int sqlite3Fts3SelectDocsize(
  Fts3Table *pTab,                
  sqlite3_int64 iDocid,           
  sqlite3_stmt **ppStmt           
){
  return fts3SelectDocsize(pTab, SQL_SELECT_DOCSIZE, iDocid, ppStmt);
}

static void fts3SqlExec(
  int *pRC,                
  Fts3Table *p,            
  int eStmt,               
  sqlite3_value **apVal    
){
  sqlite3_stmt *pStmt;
  int rc;
  if( *pRC ) return;
  rc = fts3SqlStmt(p, eStmt, &pStmt, apVal); 
  if( rc==SQLITE_OK ){
    sqlite3_step(pStmt);
    rc = sqlite3_reset(pStmt);
  }
  *pRC = rc;
}


int sqlite3Fts3ReadLock(Fts3Table *p){
  int rc;                         
  sqlite3_stmt *pStmt;            

  rc = fts3SqlStmt(p, SQL_SELECT_CONTENT_BY_ROWID, &pStmt, 0);
  if( rc==SQLITE_OK ){
    sqlite3_bind_null(pStmt, 1);
    sqlite3_step(pStmt);
    rc = sqlite3_reset(pStmt);
  }
  return rc;
}

int sqlite3Fts3AllSegdirs(Fts3Table *p, int iLevel, sqlite3_stmt **ppStmt){
  int rc;
  sqlite3_stmt *pStmt = 0;
  if( iLevel<0 ){
    rc = fts3SqlStmt(p, SQL_SELECT_ALL_LEVEL, &pStmt, 0);
  }else{
    rc = fts3SqlStmt(p, SQL_SELECT_LEVEL, &pStmt, 0);
    if( rc==SQLITE_OK ) sqlite3_bind_int(pStmt, 1, iLevel);
  }
  *ppStmt = pStmt;
  return rc;
}


static int fts3PendingListAppendVarint(
  PendingList **pp,               
  sqlite3_int64 i                 
){
  PendingList *p = *pp;

  
  if( !p ){
    p = sqlite3_malloc(sizeof(*p) + 100);
    if( !p ){
      return SQLITE_NOMEM;
    }
    p->nSpace = 100;
    p->aData = (char *)&p[1];
    p->nData = 0;
  }
  else if( p->nData+FTS3_VARINT_MAX+1>p->nSpace ){
    int nNew = p->nSpace * 2;
    p = sqlite3_realloc(p, sizeof(*p) + nNew);
    if( !p ){
      sqlite3_free(*pp);
      *pp = 0;
      return SQLITE_NOMEM;
    }
    p->nSpace = nNew;
    p->aData = (char *)&p[1];
  }

  
  p->nData += sqlite3Fts3PutVarint(&p->aData[p->nData], i);
  p->aData[p->nData] = '\0';
  *pp = p;
  return SQLITE_OK;
}

static int fts3PendingListAppend(
  PendingList **pp,               
  sqlite3_int64 iDocid,           
  sqlite3_int64 iCol,             
  sqlite3_int64 iPos,             
  int *pRc                        
){
  PendingList *p = *pp;
  int rc = SQLITE_OK;

  assert( !p || p->iLastDocid<=iDocid );

  if( !p || p->iLastDocid!=iDocid ){
    sqlite3_int64 iDelta = iDocid - (p ? p->iLastDocid : 0);
    if( p ){
      assert( p->nData<p->nSpace );
      assert( p->aData[p->nData]==0 );
      p->nData++;
    }
    if( SQLITE_OK!=(rc = fts3PendingListAppendVarint(&p, iDelta)) ){
      goto pendinglistappend_out;
    }
    p->iLastCol = -1;
    p->iLastPos = 0;
    p->iLastDocid = iDocid;
  }
  if( iCol>0 && p->iLastCol!=iCol ){
    if( SQLITE_OK!=(rc = fts3PendingListAppendVarint(&p, 1))
     || SQLITE_OK!=(rc = fts3PendingListAppendVarint(&p, iCol))
    ){
      goto pendinglistappend_out;
    }
    p->iLastCol = iCol;
    p->iLastPos = 0;
  }
  if( iCol>=0 ){
    assert( iPos>p->iLastPos || (iPos==0 && p->iLastPos==0) );
    rc = fts3PendingListAppendVarint(&p, 2+iPos-p->iLastPos);
    if( rc==SQLITE_OK ){
      p->iLastPos = iPos;
    }
  }

 pendinglistappend_out:
  *pRc = rc;
  if( p!=*pp ){
    *pp = p;
    return 1;
  }
  return 0;
}

static int fts3PendingTermsAdd(
  Fts3Table *p,                   
  const char *zText,              
  int iCol,                       
  u32 *pnWord                     
){
  int rc;
  int iStart;
  int iEnd;
  int iPos;
  int nWord = 0;

  char const *zToken;
  int nToken;

  sqlite3_tokenizer *pTokenizer = p->pTokenizer;
  sqlite3_tokenizer_module const *pModule = pTokenizer->pModule;
  sqlite3_tokenizer_cursor *pCsr;
  int (*xNext)(sqlite3_tokenizer_cursor *pCursor,
      const char**,int*,int*,int*,int*);

  assert( pTokenizer && pModule );

  rc = pModule->xOpen(pTokenizer, zText, -1, &pCsr);
  if( rc!=SQLITE_OK ){
    return rc;
  }
  pCsr->pTokenizer = pTokenizer;

  xNext = pModule->xNext;
  while( SQLITE_OK==rc
      && SQLITE_OK==(rc = xNext(pCsr, &zToken, &nToken, &iStart, &iEnd, &iPos))
  ){
    PendingList *pList;
 
    if( iPos>=nWord ) nWord = iPos+1;

    if( iPos<0 || !zToken || nToken<=0 ){
      rc = SQLITE_ERROR;
      break;
    }

    pList = (PendingList *)fts3HashFind(&p->pendingTerms, zToken, nToken);
    if( pList ){
      p->nPendingData -= (pList->nData + nToken + sizeof(Fts3HashElem));
    }
    if( fts3PendingListAppend(&pList, p->iPrevDocid, iCol, iPos, &rc) ){
      if( pList==fts3HashInsert(&p->pendingTerms, zToken, nToken, pList) ){
        assert( 0==fts3HashFind(&p->pendingTerms, zToken, nToken) );
        sqlite3_free(pList);
        rc = SQLITE_NOMEM;
      }
    }
    if( rc==SQLITE_OK ){
      p->nPendingData += (pList->nData + nToken + sizeof(Fts3HashElem));
    }
  }

  pModule->xClose(pCsr);
  *pnWord = nWord;
  return (rc==SQLITE_DONE ? SQLITE_OK : rc);
}

static int fts3PendingTermsDocid(Fts3Table *p, sqlite_int64 iDocid){
  if( iDocid<=p->iPrevDocid || p->nPendingData>p->nMaxPendingData ){
    int rc = sqlite3Fts3PendingTermsFlush(p);
    if( rc!=SQLITE_OK ) return rc;
  }
  p->iPrevDocid = iDocid;
  return SQLITE_OK;
}

void sqlite3Fts3PendingTermsClear(Fts3Table *p){
  Fts3HashElem *pElem;
  for(pElem=fts3HashFirst(&p->pendingTerms); pElem; pElem=fts3HashNext(pElem)){
    sqlite3_free(fts3HashData(pElem));
  }
  fts3HashClear(&p->pendingTerms);
  p->nPendingData = 0;
}

static int fts3InsertTerms(Fts3Table *p, sqlite3_value **apVal, u32 *aSz){
  int i;                          
  for(i=2; i<p->nColumn+2; i++){
    const char *zText = (const char *)sqlite3_value_text(apVal[i]);
    if( zText ){
      int rc = fts3PendingTermsAdd(p, zText, i-2, &aSz[i-2]);
      if( rc!=SQLITE_OK ){
        return rc;
      }
    }
    aSz[p->nColumn] += sqlite3_value_bytes(apVal[i]);
  }
  return SQLITE_OK;
}

static int fts3InsertData(
  Fts3Table *p,                   
  sqlite3_value **apVal,          
  sqlite3_int64 *piDocid          
){
  int rc;                         
  sqlite3_stmt *pContentInsert;   

  rc = fts3SqlStmt(p, SQL_CONTENT_INSERT, &pContentInsert, &apVal[1]);
  if( rc!=SQLITE_OK ){
    return rc;
  }

  if( SQLITE_NULL!=sqlite3_value_type(apVal[3+p->nColumn]) ){
    if( SQLITE_NULL==sqlite3_value_type(apVal[0])
     && SQLITE_NULL!=sqlite3_value_type(apVal[1])
    ){
      
      return SQLITE_ERROR;
    }
    rc = sqlite3_bind_value(pContentInsert, 1, apVal[3+p->nColumn]);
    if( rc!=SQLITE_OK ) return rc;
  }

  sqlite3_step(pContentInsert);
  rc = sqlite3_reset(pContentInsert);

  *piDocid = sqlite3_last_insert_rowid(p->db);
  return rc;
}



static int fts3DeleteAll(Fts3Table *p){
  int rc = SQLITE_OK;             

  
  sqlite3Fts3PendingTermsClear(p);

  
  fts3SqlExec(&rc, p, SQL_DELETE_ALL_CONTENT, 0);
  fts3SqlExec(&rc, p, SQL_DELETE_ALL_SEGMENTS, 0);
  fts3SqlExec(&rc, p, SQL_DELETE_ALL_SEGDIR, 0);
  if( p->bHasDocsize ){
    fts3SqlExec(&rc, p, SQL_DELETE_ALL_DOCSIZE, 0);
  }
  if( p->bHasStat ){
    fts3SqlExec(&rc, p, SQL_DELETE_ALL_STAT, 0);
  }
  return rc;
}

static void fts3DeleteTerms( 
  int *pRC,               
  Fts3Table *p,           
  sqlite3_value **apVal,  
  u32 *aSz                /* Sizes of deleted document written here */
){
  int rc;
  sqlite3_stmt *pSelect;

  if( *pRC ) return;
  rc = fts3SqlStmt(p, SQL_SELECT_CONTENT_BY_ROWID, &pSelect, apVal);
  if( rc==SQLITE_OK ){
    if( SQLITE_ROW==sqlite3_step(pSelect) ){
      int i;
      for(i=1; i<=p->nColumn; i++){
        const char *zText = (const char *)sqlite3_column_text(pSelect, i);
        rc = fts3PendingTermsAdd(p, zText, -1, &aSz[i-1]);
        if( rc!=SQLITE_OK ){
          sqlite3_reset(pSelect);
          *pRC = rc;
          return;
        }
        aSz[p->nColumn] += sqlite3_column_bytes(pSelect, i);
      }
    }
    rc = sqlite3_reset(pSelect);
  }else{
    sqlite3_reset(pSelect);
  }
  *pRC = rc;
}

static int fts3SegmentMerge(Fts3Table *, int);

static int fts3AllocateSegdirIdx(Fts3Table *p, int iLevel, int *piIdx){
  int rc;                         
  sqlite3_stmt *pNextIdx;         
  int iNext = 0;                  

  
  rc = fts3SqlStmt(p, SQL_NEXT_SEGMENT_INDEX, &pNextIdx, 0);
  if( rc==SQLITE_OK ){
    sqlite3_bind_int(pNextIdx, 1, iLevel);
    if( SQLITE_ROW==sqlite3_step(pNextIdx) ){
      iNext = sqlite3_column_int(pNextIdx, 0);
    }
    rc = sqlite3_reset(pNextIdx);
  }

  if( rc==SQLITE_OK ){
    if( iNext>=FTS3_MERGE_COUNT ){
      rc = fts3SegmentMerge(p, iLevel);
      *piIdx = 0;
    }else{
      *piIdx = iNext;
    }
  }

  return rc;
}

int sqlite3Fts3ReadBlock(
  Fts3Table *p,                   
  sqlite3_int64 iBlockid,         
  char **paBlob,                  
  int *pnBlob                     
){
  int rc;                         

  
  assert( pnBlob);

  if( p->pSegments ){
    rc = sqlite3_blob_reopen(p->pSegments, iBlockid);
  }else{
    if( 0==p->zSegmentsTbl ){
      p->zSegmentsTbl = sqlite3_mprintf("%s_segments", p->zName);
      if( 0==p->zSegmentsTbl ) return SQLITE_NOMEM;
    }
    rc = sqlite3_blob_open(
       p->db, p->zDb, p->zSegmentsTbl, "block", iBlockid, 0, &p->pSegments
    );
  }

  if( rc==SQLITE_OK ){
    int nByte = sqlite3_blob_bytes(p->pSegments);
    if( paBlob ){
      char *aByte = sqlite3_malloc(nByte + FTS3_NODE_PADDING);
      if( !aByte ){
        rc = SQLITE_NOMEM;
      }else{
        rc = sqlite3_blob_read(p->pSegments, aByte, nByte, 0);
        memset(&aByte[nByte], 0, FTS3_NODE_PADDING);
        if( rc!=SQLITE_OK ){
          sqlite3_free(aByte);
          aByte = 0;
        }
      }
      *paBlob = aByte;
    }
    *pnBlob = nByte;
  }

  return rc;
}

void sqlite3Fts3SegmentsClose(Fts3Table *p){
  sqlite3_blob_close(p->pSegments);
  p->pSegments = 0;
}

static int fts3SegReaderNext(Fts3Table *p, Fts3SegReader *pReader){
  char *pNext;                    
  int nPrefix;                    
  int nSuffix;                    

  if( !pReader->aDoclist ){
    pNext = pReader->aNode;
  }else{
    pNext = &pReader->aDoclist[pReader->nDoclist];
  }

  if( !pNext || pNext>=&pReader->aNode[pReader->nNode] ){
    int rc;                       

    if( fts3SegReaderIsPending(pReader) ){
      Fts3HashElem *pElem = *(pReader->ppNextElem);
      if( pElem==0 ){
        pReader->aNode = 0;
      }else{
        PendingList *pList = (PendingList *)fts3HashData(pElem);
        pReader->zTerm = (char *)fts3HashKey(pElem);
        pReader->nTerm = fts3HashKeysize(pElem);
        pReader->nNode = pReader->nDoclist = pList->nData + 1;
        pReader->aNode = pReader->aDoclist = pList->aData;
        pReader->ppNextElem++;
        assert( pReader->aNode );
      }
      return SQLITE_OK;
    }

    if( !fts3SegReaderIsRootOnly(pReader) ){
      sqlite3_free(pReader->aNode);
    }
    pReader->aNode = 0;

    assert( pReader->iCurrentBlock<=pReader->iLeafEndBlock );
    if( pReader->iCurrentBlock>=pReader->iLeafEndBlock ){
      return SQLITE_OK;
    }

    rc = sqlite3Fts3ReadBlock(
        p, ++pReader->iCurrentBlock, &pReader->aNode, &pReader->nNode
    );
    if( rc!=SQLITE_OK ) return rc;
    pNext = pReader->aNode;
  }
  
  pNext += sqlite3Fts3GetVarint32(pNext, &nPrefix);
  pNext += sqlite3Fts3GetVarint32(pNext, &nSuffix);
  if( nPrefix<0 || nSuffix<=0 
   || &pNext[nSuffix]>&pReader->aNode[pReader->nNode] 
  ){
    return SQLITE_CORRUPT;
  }

  if( nPrefix+nSuffix>pReader->nTermAlloc ){
    int nNew = (nPrefix+nSuffix)*2;
    char *zNew = sqlite3_realloc(pReader->zTerm, nNew);
    if( !zNew ){
      return SQLITE_NOMEM;
    }
    pReader->zTerm = zNew;
    pReader->nTermAlloc = nNew;
  }
  memcpy(&pReader->zTerm[nPrefix], pNext, nSuffix);
  pReader->nTerm = nPrefix+nSuffix;
  pNext += nSuffix;
  pNext += sqlite3Fts3GetVarint32(pNext, &pReader->nDoclist);
  pReader->aDoclist = pNext;
  pReader->pOffsetList = 0;

  if( &pReader->aDoclist[pReader->nDoclist]>&pReader->aNode[pReader->nNode] 
   || pReader->aDoclist[pReader->nDoclist-1]
  ){
    return SQLITE_CORRUPT;
  }
  return SQLITE_OK;
}

static void fts3SegReaderFirstDocid(Fts3SegReader *pReader){
  int n;
  assert( pReader->aDoclist );
  assert( !pReader->pOffsetList );
  n = sqlite3Fts3GetVarint(pReader->aDoclist, &pReader->iDocid);
  pReader->pOffsetList = &pReader->aDoclist[n];
}

static void fts3SegReaderNextDocid(
  Fts3SegReader *pReader,
  char **ppOffsetList,
  int *pnOffsetList
){
  char *p = pReader->pOffsetList;
  char c = 0;

  while( *p | c ) c = *p++ & 0x80;
  p++;

  if( ppOffsetList ){
    *ppOffsetList = pReader->pOffsetList;
    *pnOffsetList = (int)(p - pReader->pOffsetList - 1);
  }

  if( p>=&pReader->aDoclist[pReader->nDoclist] ){
    pReader->pOffsetList = 0;
  }else{
    sqlite3_int64 iDelta;
    pReader->pOffsetList = p + sqlite3Fts3GetVarint(p, &iDelta);
    pReader->iDocid += iDelta;
  }
}

int sqlite3Fts3SegReaderCost(
  Fts3Cursor *pCsr,               
  Fts3SegReader *pReader,         
  int *pnCost                     
){
  Fts3Table *p = (Fts3Table*)pCsr->base.pVtab;
  int rc = SQLITE_OK;             
  int nCost = 0;                  
  int pgsz = p->nPgsz;            

  if( p->bHasStat 
   && !fts3SegReaderIsPending(pReader) 
   && !fts3SegReaderIsRootOnly(pReader) 
  ){
    int nBlob = 0;
    sqlite3_int64 iBlock;

    if( pCsr->nRowAvg==0 ){
      sqlite3_stmt *pStmt;
      sqlite3_int64 nDoc = 0;
      sqlite3_int64 nByte = 0;
      const char *pEnd;
      const char *a;

      rc = sqlite3Fts3SelectDoctotal(p, &pStmt);
      if( rc!=SQLITE_OK ) return rc;
      a = sqlite3_column_blob(pStmt, 0);
      assert( a );

      pEnd = &a[sqlite3_column_bytes(pStmt, 0)];
      a += sqlite3Fts3GetVarint(a, &nDoc);
      while( a<pEnd ){
        a += sqlite3Fts3GetVarint(a, &nByte);
      }
      if( nDoc==0 || nByte==0 ){
        sqlite3_reset(pStmt);
        return SQLITE_CORRUPT;
      }

      pCsr->nRowAvg = (int)(((nByte / nDoc) + pgsz) / pgsz);
      assert( pCsr->nRowAvg>0 ); 
      rc = sqlite3_reset(pStmt);
      if( rc!=SQLITE_OK ) return rc;
    }

    for(iBlock=pReader->iStartBlock; iBlock<=pReader->iLeafEndBlock; iBlock++){
      rc = sqlite3Fts3ReadBlock(p, iBlock, 0, &nBlob);
      if( rc!=SQLITE_OK ) break;
      if( (nBlob+35)>pgsz ){
        int nOvfl = (nBlob + 34)/pgsz;
        nCost += ((nOvfl + pCsr->nRowAvg - 1)/pCsr->nRowAvg);
      }
    }
  }

  *pnCost += nCost;
  return rc;
}

void sqlite3Fts3SegReaderFree(Fts3SegReader *pReader){
  if( pReader && !fts3SegReaderIsPending(pReader) ){
    sqlite3_free(pReader->zTerm);
    if( !fts3SegReaderIsRootOnly(pReader) ){
      sqlite3_free(pReader->aNode);
    }
  }
  sqlite3_free(pReader);
}

int sqlite3Fts3SegReaderNew(
  int iAge,                       
  sqlite3_int64 iStartLeaf,       
  sqlite3_int64 iEndLeaf,         
  sqlite3_int64 iEndBlock,        
  const char *zRoot,              
  int nRoot,                      
  Fts3SegReader **ppReader        
){
  int rc = SQLITE_OK;             
  Fts3SegReader *pReader;         
  int nExtra = 0;                 

  assert( iStartLeaf<=iEndLeaf );
  if( iStartLeaf==0 ){
    nExtra = nRoot + FTS3_NODE_PADDING;
  }

  pReader = (Fts3SegReader *)sqlite3_malloc(sizeof(Fts3SegReader) + nExtra);
  if( !pReader ){
    return SQLITE_NOMEM;
  }
  memset(pReader, 0, sizeof(Fts3SegReader));
  pReader->iIdx = iAge;
  pReader->iStartBlock = iStartLeaf;
  pReader->iLeafEndBlock = iEndLeaf;
  pReader->iEndBlock = iEndBlock;

  if( nExtra ){
    
    pReader->aNode = (char *)&pReader[1];
    pReader->nNode = nRoot;
    memcpy(pReader->aNode, zRoot, nRoot);
    memset(&pReader->aNode[nRoot], 0, FTS3_NODE_PADDING);
  }else{
    pReader->iCurrentBlock = iStartLeaf-1;
  }

  if( rc==SQLITE_OK ){
    *ppReader = pReader;
  }else{
    sqlite3Fts3SegReaderFree(pReader);
  }
  return rc;
}

static int fts3CompareElemByTerm(const void *lhs, const void *rhs){
  char *z1 = fts3HashKey(*(Fts3HashElem **)lhs);
  char *z2 = fts3HashKey(*(Fts3HashElem **)rhs);
  int n1 = fts3HashKeysize(*(Fts3HashElem **)lhs);
  int n2 = fts3HashKeysize(*(Fts3HashElem **)rhs);

  int n = (n1<n2 ? n1 : n2);
  int c = memcmp(z1, z2, n);
  if( c==0 ){
    c = n1 - n2;
  }
  return c;
}

int sqlite3Fts3SegReaderPending(
  Fts3Table *p,                   
  const char *zTerm,              
  int nTerm,                      
  int isPrefix,                   
  Fts3SegReader **ppReader        
){
  Fts3SegReader *pReader = 0;     
  Fts3HashElem *pE;               
  Fts3HashElem **aElem = 0;       
  int nElem = 0;                  
  int rc = SQLITE_OK;             

  if( isPrefix ){
    int nAlloc = 0;               

    for(pE=fts3HashFirst(&p->pendingTerms); pE; pE=fts3HashNext(pE)){
      char *zKey = (char *)fts3HashKey(pE);
      int nKey = fts3HashKeysize(pE);
      if( nTerm==0 || (nKey>=nTerm && 0==memcmp(zKey, zTerm, nTerm)) ){
        if( nElem==nAlloc ){
          Fts3HashElem **aElem2;
          nAlloc += 16;
          aElem2 = (Fts3HashElem **)sqlite3_realloc(
              aElem, nAlloc*sizeof(Fts3HashElem *)
          );
          if( !aElem2 ){
            rc = SQLITE_NOMEM;
            nElem = 0;
            break;
          }
          aElem = aElem2;
        }
        aElem[nElem++] = pE;
      }
    }

    if( nElem>1 ){
      qsort(aElem, nElem, sizeof(Fts3HashElem *), fts3CompareElemByTerm);
    }

  }else{
    pE = fts3HashFindElem(&p->pendingTerms, zTerm, nTerm);
    if( pE ){
      aElem = &pE;
      nElem = 1;
    }
  }

  if( nElem>0 ){
    int nByte = sizeof(Fts3SegReader) + (nElem+1)*sizeof(Fts3HashElem *);
    pReader = (Fts3SegReader *)sqlite3_malloc(nByte);
    if( !pReader ){
      rc = SQLITE_NOMEM;
    }else{
      memset(pReader, 0, nByte);
      pReader->iIdx = 0x7FFFFFFF;
      pReader->ppNextElem = (Fts3HashElem **)&pReader[1];
      memcpy(pReader->ppNextElem, aElem, nElem*sizeof(Fts3HashElem *));
    }
  }

  if( isPrefix ){
    sqlite3_free(aElem);
  }
  *ppReader = pReader;
  return rc;
}

static int fts3SegReaderCmp(Fts3SegReader *pLhs, Fts3SegReader *pRhs){
  int rc;
  if( pLhs->aNode && pRhs->aNode ){
    int rc2 = pLhs->nTerm - pRhs->nTerm;
    if( rc2<0 ){
      rc = memcmp(pLhs->zTerm, pRhs->zTerm, pLhs->nTerm);
    }else{
      rc = memcmp(pLhs->zTerm, pRhs->zTerm, pRhs->nTerm);
    }
    if( rc==0 ){
      rc = rc2;
    }
  }else{
    rc = (pLhs->aNode==0) - (pRhs->aNode==0);
  }
  if( rc==0 ){
    rc = pRhs->iIdx - pLhs->iIdx;
  }
  assert( rc!=0 );
  return rc;
}

static int fts3SegReaderDoclistCmp(Fts3SegReader *pLhs, Fts3SegReader *pRhs){
  int rc = (pLhs->pOffsetList==0)-(pRhs->pOffsetList==0);
  if( rc==0 ){
    if( pLhs->iDocid==pRhs->iDocid ){
      rc = pRhs->iIdx - pLhs->iIdx;
    }else{
      rc = (pLhs->iDocid > pRhs->iDocid) ? 1 : -1;
    }
  }
  assert( pLhs->aNode && pRhs->aNode );
  return rc;
}

static int fts3SegReaderTermCmp(
  Fts3SegReader *pSeg,            
  const char *zTerm,              
  int nTerm                       
){
  int res = 0;
  if( pSeg->aNode ){
    if( pSeg->nTerm>nTerm ){
      res = memcmp(pSeg->zTerm, zTerm, nTerm);
    }else{
      res = memcmp(pSeg->zTerm, zTerm, pSeg->nTerm);
    }
    if( res==0 ){
      res = pSeg->nTerm-nTerm;
    }
  }
  return res;
}

static void fts3SegReaderSort(
  Fts3SegReader **apSegment,                     
  int nSegment,                                  
  int nSuspect,                                  
  int (*xCmp)(Fts3SegReader *, Fts3SegReader *)  
){
  int i;                          

  assert( nSuspect<=nSegment );

  if( nSuspect==nSegment ) nSuspect--;
  for(i=nSuspect-1; i>=0; i--){
    int j;
    for(j=i; j<(nSegment-1); j++){
      Fts3SegReader *pTmp;
      if( xCmp(apSegment[j], apSegment[j+1])<0 ) break;
      pTmp = apSegment[j+1];
      apSegment[j+1] = apSegment[j];
      apSegment[j] = pTmp;
    }
  }

#ifndef NDEBUG
  
  for(i=0; i<(nSuspect-1); i++){
    assert( xCmp(apSegment[i], apSegment[i+1])<0 );
  }
#endif
}

static int fts3WriteSegment(
  Fts3Table *p,                   
  sqlite3_int64 iBlock,           
  char *z,                        
  int n                           
){
  sqlite3_stmt *pStmt;
  int rc = fts3SqlStmt(p, SQL_INSERT_SEGMENTS, &pStmt, 0);
  if( rc==SQLITE_OK ){
    sqlite3_bind_int64(pStmt, 1, iBlock);
    sqlite3_bind_blob(pStmt, 2, z, n, SQLITE_STATIC);
    sqlite3_step(pStmt);
    rc = sqlite3_reset(pStmt);
  }
  return rc;
}

static int fts3WriteSegdir(
  Fts3Table *p,                   
  int iLevel,                     
  int iIdx,                       
  sqlite3_int64 iStartBlock,      
  sqlite3_int64 iLeafEndBlock,    
  sqlite3_int64 iEndBlock,        
  char *zRoot,                    
  int nRoot                       
){
  sqlite3_stmt *pStmt;
  int rc = fts3SqlStmt(p, SQL_INSERT_SEGDIR, &pStmt, 0);
  if( rc==SQLITE_OK ){
    sqlite3_bind_int(pStmt, 1, iLevel);
    sqlite3_bind_int(pStmt, 2, iIdx);
    sqlite3_bind_int64(pStmt, 3, iStartBlock);
    sqlite3_bind_int64(pStmt, 4, iLeafEndBlock);
    sqlite3_bind_int64(pStmt, 5, iEndBlock);
    sqlite3_bind_blob(pStmt, 6, zRoot, nRoot, SQLITE_STATIC);
    sqlite3_step(pStmt);
    rc = sqlite3_reset(pStmt);
  }
  return rc;
}

static int fts3PrefixCompress(
  const char *zPrev,              
  int nPrev,                      
  const char *zNext,              
  int nNext                       
){
  int n;
  UNUSED_PARAMETER(nNext);
  for(n=0; n<nPrev && zPrev[n]==zNext[n]; n++);
  return n;
}

static int fts3NodeAddTerm(
  Fts3Table *p,                   
  SegmentNode **ppTree,            
  int isCopyTerm,                 
  const char *zTerm,              
  int nTerm                       
){
  SegmentNode *pTree = *ppTree;
  int rc;
  SegmentNode *pNew;

  if( pTree ){
    int nData = pTree->nData;     
    int nReq = nData;             
    int nPrefix;                  
    int nSuffix;                  

    nPrefix = fts3PrefixCompress(pTree->zTerm, pTree->nTerm, zTerm, nTerm);
    nSuffix = nTerm-nPrefix;

    nReq += sqlite3Fts3VarintLen(nPrefix)+sqlite3Fts3VarintLen(nSuffix)+nSuffix;
    if( nReq<=p->nNodeSize || !pTree->zTerm ){

      if( nReq>p->nNodeSize ){
        assert( pTree->aData==(char *)&pTree[1] );
        pTree->aData = (char *)sqlite3_malloc(nReq);
        if( !pTree->aData ){
          return SQLITE_NOMEM;
        }
      }

      if( pTree->zTerm ){
        
        nData += sqlite3Fts3PutVarint(&pTree->aData[nData], nPrefix);
      }

      nData += sqlite3Fts3PutVarint(&pTree->aData[nData], nSuffix);
      memcpy(&pTree->aData[nData], &zTerm[nPrefix], nSuffix);
      pTree->nData = nData + nSuffix;
      pTree->nEntry++;

      if( isCopyTerm ){
        if( pTree->nMalloc<nTerm ){
          char *zNew = sqlite3_realloc(pTree->zMalloc, nTerm*2);
          if( !zNew ){
            return SQLITE_NOMEM;
          }
          pTree->nMalloc = nTerm*2;
          pTree->zMalloc = zNew;
        }
        pTree->zTerm = pTree->zMalloc;
        memcpy(pTree->zTerm, zTerm, nTerm);
        pTree->nTerm = nTerm;
      }else{
        pTree->zTerm = (char *)zTerm;
        pTree->nTerm = nTerm;
      }
      return SQLITE_OK;
    }
  }

  pNew = (SegmentNode *)sqlite3_malloc(sizeof(SegmentNode) + p->nNodeSize);
  if( !pNew ){
    return SQLITE_NOMEM;
  }
  memset(pNew, 0, sizeof(SegmentNode));
  pNew->nData = 1 + FTS3_VARINT_MAX;
  pNew->aData = (char *)&pNew[1];

  if( pTree ){
    SegmentNode *pParent = pTree->pParent;
    rc = fts3NodeAddTerm(p, &pParent, isCopyTerm, zTerm, nTerm);
    if( pTree->pParent==0 ){
      pTree->pParent = pParent;
    }
    pTree->pRight = pNew;
    pNew->pLeftmost = pTree->pLeftmost;
    pNew->pParent = pParent;
    pNew->zMalloc = pTree->zMalloc;
    pNew->nMalloc = pTree->nMalloc;
    pTree->zMalloc = 0;
  }else{
    pNew->pLeftmost = pNew;
    rc = fts3NodeAddTerm(p, &pNew, isCopyTerm, zTerm, nTerm); 
  }

  *ppTree = pNew;
  return rc;
}

static int fts3TreeFinishNode(
  SegmentNode *pTree, 
  int iHeight, 
  sqlite3_int64 iLeftChild
){
  int nStart;
  assert( iHeight>=1 && iHeight<128 );
  nStart = FTS3_VARINT_MAX - sqlite3Fts3VarintLen(iLeftChild);
  pTree->aData[nStart] = (char)iHeight;
  sqlite3Fts3PutVarint(&pTree->aData[nStart+1], iLeftChild);
  return nStart;
}

/*
** Write the buffer for the segment node pTree and all of its peers to the
** database. Then call this function recursively to write the parent of 
** pTree and its peers to the database. 
**
** Except, if pTree is a root node, do not write it to the database. Instead,
** set output variables *paRoot and *pnRoot to contain the root node.
**
** If successful, SQLITE_OK is returned and output variable *piLast is
** set to the largest blockid written to the database (or zero if no
** blocks were written to the db). Otherwise, an SQLite error code is 
** returned.
*/
static int fts3NodeWrite(
  Fts3Table *p,                   
  SegmentNode *pTree,             
  int iHeight,                    
  sqlite3_int64 iLeaf,            
  sqlite3_int64 iFree,            
  sqlite3_int64 *piLast,          /* OUT: Block id of last entry written */
  char **paRoot,                  
  int *pnRoot                     
){
  int rc = SQLITE_OK;

  if( !pTree->pParent ){
    
    int nStart = fts3TreeFinishNode(pTree, iHeight, iLeaf);
    *piLast = iFree-1;
    *pnRoot = pTree->nData - nStart;
    *paRoot = &pTree->aData[nStart];
  }else{
    SegmentNode *pIter;
    sqlite3_int64 iNextFree = iFree;
    sqlite3_int64 iNextLeaf = iLeaf;
    for(pIter=pTree->pLeftmost; pIter && rc==SQLITE_OK; pIter=pIter->pRight){
      int nStart = fts3TreeFinishNode(pIter, iHeight, iNextLeaf);
      int nWrite = pIter->nData - nStart;
  
      rc = fts3WriteSegment(p, iNextFree, &pIter->aData[nStart], nWrite);
      iNextFree++;
      iNextLeaf += (pIter->nEntry+1);
    }
    if( rc==SQLITE_OK ){
      assert( iNextLeaf==iFree );
      rc = fts3NodeWrite(
          p, pTree->pParent, iHeight+1, iFree, iNextFree, piLast, paRoot, pnRoot
      );
    }
  }

  return rc;
}

static void fts3NodeFree(SegmentNode *pTree){
  if( pTree ){
    SegmentNode *p = pTree->pLeftmost;
    fts3NodeFree(p->pParent);
    while( p ){
      SegmentNode *pRight = p->pRight;
      if( p->aData!=(char *)&p[1] ){
        sqlite3_free(p->aData);
      }
      assert( pRight==0 || p->zMalloc==0 );
      sqlite3_free(p->zMalloc);
      sqlite3_free(p);
      p = pRight;
    }
  }
}

static int fts3SegWriterAdd(
  Fts3Table *p,                   
  SegmentWriter **ppWriter,        
  int isCopyTerm,                 
  const char *zTerm,              
  int nTerm,                      
  const char *aDoclist,           
  int nDoclist                    
){
  int nPrefix;                    
  int nSuffix;                    
  int nReq;                       
  int nData;
  SegmentWriter *pWriter = *ppWriter;

  if( !pWriter ){
    int rc;
    sqlite3_stmt *pStmt;

    
    pWriter = (SegmentWriter *)sqlite3_malloc(sizeof(SegmentWriter));
    if( !pWriter ) return SQLITE_NOMEM;
    memset(pWriter, 0, sizeof(SegmentWriter));
    *ppWriter = pWriter;

    
    pWriter->aData = (char *)sqlite3_malloc(p->nNodeSize);
    if( !pWriter->aData ) return SQLITE_NOMEM;
    pWriter->nSize = p->nNodeSize;

    
    rc = fts3SqlStmt(p, SQL_NEXT_SEGMENTS_ID, &pStmt, 0);
    if( rc!=SQLITE_OK ) return rc;
    if( SQLITE_ROW==sqlite3_step(pStmt) ){
      pWriter->iFree = sqlite3_column_int64(pStmt, 0);
      pWriter->iFirst = pWriter->iFree;
    }
    rc = sqlite3_reset(pStmt);
    if( rc!=SQLITE_OK ) return rc;
  }
  nData = pWriter->nData;

  nPrefix = fts3PrefixCompress(pWriter->zTerm, pWriter->nTerm, zTerm, nTerm);
  nSuffix = nTerm-nPrefix;

  
  nReq = sqlite3Fts3VarintLen(nPrefix) +    
    sqlite3Fts3VarintLen(nSuffix) +         
    nSuffix +                               
    sqlite3Fts3VarintLen(nDoclist) +        
    nDoclist;                               

  if( nData>0 && nData+nReq>p->nNodeSize ){
    int rc;

    
    rc = fts3WriteSegment(p, pWriter->iFree++, pWriter->aData, nData);
    if( rc!=SQLITE_OK ) return rc;

    /* Add the current term to the interior node tree. The term added to
    ** the interior tree must:
    **
    **   a) be greater than the largest term on the leaf node just written
    **      to the database (still available in pWriter->zTerm), and
    **
    **   b) be less than or equal to the term about to be added to the new
    **      leaf node (zTerm/nTerm).
    **
    ** In other words, it must be the prefix of zTerm 1 byte longer than
    ** the common prefix (if any) of zTerm and pWriter->zTerm.
    */
    assert( nPrefix<nTerm );
    rc = fts3NodeAddTerm(p, &pWriter->pTree, isCopyTerm, zTerm, nPrefix+1);
    if( rc!=SQLITE_OK ) return rc;

    nData = 0;
    pWriter->nTerm = 0;

    nPrefix = 0;
    nSuffix = nTerm;
    nReq = 1 +                              
      sqlite3Fts3VarintLen(nTerm) +         
      nTerm +                               
      sqlite3Fts3VarintLen(nDoclist) +      
      nDoclist;                             
  }

  if( nReq>pWriter->nSize ){
    char *aNew = sqlite3_realloc(pWriter->aData, nReq);
    if( !aNew ) return SQLITE_NOMEM;
    pWriter->aData = aNew;
    pWriter->nSize = nReq;
  }
  assert( nData+nReq<=pWriter->nSize );

  
  nData += sqlite3Fts3PutVarint(&pWriter->aData[nData], nPrefix);
  nData += sqlite3Fts3PutVarint(&pWriter->aData[nData], nSuffix);
  memcpy(&pWriter->aData[nData], &zTerm[nPrefix], nSuffix);
  nData += nSuffix;
  nData += sqlite3Fts3PutVarint(&pWriter->aData[nData], nDoclist);
  memcpy(&pWriter->aData[nData], aDoclist, nDoclist);
  pWriter->nData = nData + nDoclist;

  if( isCopyTerm ){
    if( nTerm>pWriter->nMalloc ){
      char *zNew = sqlite3_realloc(pWriter->zMalloc, nTerm*2);
      if( !zNew ){
        return SQLITE_NOMEM;
      }
      pWriter->nMalloc = nTerm*2;
      pWriter->zMalloc = zNew;
      pWriter->zTerm = zNew;
    }
    assert( pWriter->zTerm==pWriter->zMalloc );
    memcpy(pWriter->zTerm, zTerm, nTerm);
  }else{
    pWriter->zTerm = (char *)zTerm;
  }
  pWriter->nTerm = nTerm;

  return SQLITE_OK;
}

static int fts3SegWriterFlush(
  Fts3Table *p,                   
  SegmentWriter *pWriter,         
  int iLevel,                     
  int iIdx                        
){
  int rc;                         
  if( pWriter->pTree ){
    sqlite3_int64 iLast = 0;      /* Largest block id written to database */
    sqlite3_int64 iLastLeaf;      /* Largest leaf block id written to db */
    char *zRoot = NULL;           
    int nRoot = 0;                

    iLastLeaf = pWriter->iFree;
    rc = fts3WriteSegment(p, pWriter->iFree++, pWriter->aData, pWriter->nData);
    if( rc==SQLITE_OK ){
      rc = fts3NodeWrite(p, pWriter->pTree, 1,
          pWriter->iFirst, pWriter->iFree, &iLast, &zRoot, &nRoot);
    }
    if( rc==SQLITE_OK ){
      rc = fts3WriteSegdir(
          p, iLevel, iIdx, pWriter->iFirst, iLastLeaf, iLast, zRoot, nRoot);
    }
  }else{
    
    rc = fts3WriteSegdir(
        p, iLevel, iIdx, 0, 0, 0, pWriter->aData, pWriter->nData);
  }
  return rc;
}

static void fts3SegWriterFree(SegmentWriter *pWriter){
  if( pWriter ){
    sqlite3_free(pWriter->aData);
    sqlite3_free(pWriter->zMalloc);
    fts3NodeFree(pWriter->pTree);
    sqlite3_free(pWriter);
  }
}

static int fts3IsEmpty(Fts3Table *p, sqlite3_value **apVal, int *pisEmpty){
  sqlite3_stmt *pStmt;
  int rc;
  rc = fts3SqlStmt(p, SQL_IS_EMPTY, &pStmt, apVal);
  if( rc==SQLITE_OK ){
    if( SQLITE_ROW==sqlite3_step(pStmt) ){
      *pisEmpty = sqlite3_column_int(pStmt, 0);
    }
    rc = sqlite3_reset(pStmt);
  }
  return rc;
}

static int fts3SegmentCountMax(Fts3Table *p, int *pnSegment, int *pnMax){
  sqlite3_stmt *pStmt;
  int rc;

  rc = fts3SqlStmt(p, SQL_SELECT_SEGDIR_COUNT_MAX, &pStmt, 0);
  if( rc!=SQLITE_OK ) return rc;
  if( SQLITE_ROW==sqlite3_step(pStmt) ){
    *pnSegment = sqlite3_column_int(pStmt, 0);
    *pnMax = sqlite3_column_int(pStmt, 1);
  }
  return sqlite3_reset(pStmt);
}

static int fts3DeleteSegdir(
  Fts3Table *p,                   
  int iLevel,                     
  Fts3SegReader **apSegment,      
  int nReader                     
){
  int rc;                         
  int i;                          
  sqlite3_stmt *pDelete;          

  rc = fts3SqlStmt(p, SQL_DELETE_SEGMENTS_RANGE, &pDelete, 0);
  for(i=0; rc==SQLITE_OK && i<nReader; i++){
    Fts3SegReader *pSegment = apSegment[i];
    if( pSegment->iStartBlock ){
      sqlite3_bind_int64(pDelete, 1, pSegment->iStartBlock);
      sqlite3_bind_int64(pDelete, 2, pSegment->iEndBlock);
      sqlite3_step(pDelete);
      rc = sqlite3_reset(pDelete);
    }
  }
  if( rc!=SQLITE_OK ){
    return rc;
  }

  if( iLevel==FTS3_SEGCURSOR_ALL ){
    fts3SqlExec(&rc, p, SQL_DELETE_ALL_SEGDIR, 0);
  }else if( iLevel==FTS3_SEGCURSOR_PENDING ){
    sqlite3Fts3PendingTermsClear(p);
  }else{
    assert( iLevel>=0 );
    rc = fts3SqlStmt(p, SQL_DELETE_SEGDIR_BY_LEVEL, &pDelete, 0);
    if( rc==SQLITE_OK ){
      sqlite3_bind_int(pDelete, 1, iLevel);
      sqlite3_step(pDelete);
      rc = sqlite3_reset(pDelete);
    }
  }

  return rc;
}

static void fts3ColumnFilter(
  int iCol,                       
  char **ppList,                  
  int *pnList                     
){
  char *pList = *ppList;
  int nList = *pnList;
  char *pEnd = &pList[nList];
  int iCurrent = 0;
  char *p = pList;

  assert( iCol>=0 );
  while( 1 ){
    char c = 0;
    while( p<pEnd && (c | *p)&0xFE ) c = *p++ & 0x80;
  
    if( iCol==iCurrent ){
      nList = (int)(p - pList);
      break;
    }

    nList -= (int)(p - pList);
    pList = p;
    if( nList==0 ){
      break;
    }
    p = &pList[1];
    p += sqlite3Fts3GetVarint32(p, &iCurrent);
  }

  *ppList = pList;
  *pnList = nList;
}

int sqlite3Fts3SegReaderStart(
  Fts3Table *p,                   
  Fts3SegReaderCursor *pCsr,      
  Fts3SegFilter *pFilter          
){
  int i;

  
  pCsr->pFilter = pFilter;

  for(i=0; i<pCsr->nSegment; i++){
    int nTerm = pFilter->nTerm;
    const char *zTerm = pFilter->zTerm;
    Fts3SegReader *pSeg = pCsr->apSegment[i];
    do {
      int rc = fts3SegReaderNext(p, pSeg);
      if( rc!=SQLITE_OK ) return rc;
    }while( zTerm && fts3SegReaderTermCmp(pSeg, zTerm, nTerm)<0 );
  }
  fts3SegReaderSort(
      pCsr->apSegment, pCsr->nSegment, pCsr->nSegment, fts3SegReaderCmp);

  return SQLITE_OK;
}

int sqlite3Fts3SegReaderStep(
  Fts3Table *p,                   
  Fts3SegReaderCursor *pCsr       
){
  int rc = SQLITE_OK;

  int isIgnoreEmpty =  (pCsr->pFilter->flags & FTS3_SEGMENT_IGNORE_EMPTY);
  int isRequirePos =   (pCsr->pFilter->flags & FTS3_SEGMENT_REQUIRE_POS);
  int isColFilter =    (pCsr->pFilter->flags & FTS3_SEGMENT_COLUMN_FILTER);
  int isPrefix =       (pCsr->pFilter->flags & FTS3_SEGMENT_PREFIX);
  int isScan =         (pCsr->pFilter->flags & FTS3_SEGMENT_SCAN);

  Fts3SegReader **apSegment = pCsr->apSegment;
  int nSegment = pCsr->nSegment;
  Fts3SegFilter *pFilter = pCsr->pFilter;

  if( pCsr->nSegment==0 ) return SQLITE_OK;

  do {
    int nMerge;
    int i;
  
    for(i=0; i<pCsr->nAdvance; i++){
      rc = fts3SegReaderNext(p, apSegment[i]);
      if( rc!=SQLITE_OK ) return rc;
    }
    fts3SegReaderSort(apSegment, nSegment, pCsr->nAdvance, fts3SegReaderCmp);
    pCsr->nAdvance = 0;

    
    assert( rc==SQLITE_OK );
    if( apSegment[0]->aNode==0 ) break;

    pCsr->nTerm = apSegment[0]->nTerm;
    pCsr->zTerm = apSegment[0]->zTerm;

    if( pFilter->zTerm && !isScan ){
      if( pCsr->nTerm<pFilter->nTerm 
       || (!isPrefix && pCsr->nTerm>pFilter->nTerm)
       || memcmp(pCsr->zTerm, pFilter->zTerm, pFilter->nTerm) 
      ){
        break;
      }
    }

    nMerge = 1;
    while( nMerge<nSegment 
        && apSegment[nMerge]->aNode
        && apSegment[nMerge]->nTerm==pCsr->nTerm 
        && 0==memcmp(pCsr->zTerm, apSegment[nMerge]->zTerm, pCsr->nTerm)
    ){
      nMerge++;
    }

    assert( isIgnoreEmpty || (isRequirePos && !isColFilter) );
    if( nMerge==1 && !isIgnoreEmpty ){
      pCsr->aDoclist = apSegment[0]->aDoclist;
      pCsr->nDoclist = apSegment[0]->nDoclist;
      rc = SQLITE_ROW;
    }else{
      int nDoclist = 0;           
      sqlite3_int64 iPrev = 0;    

      for(i=0; i<nMerge; i++){
        fts3SegReaderFirstDocid(apSegment[i]);
      }
      fts3SegReaderSort(apSegment, nMerge, nMerge, fts3SegReaderDoclistCmp);
      while( apSegment[0]->pOffsetList ){
        int j;                    
        char *pList;
        int nList;
        int nByte;
        sqlite3_int64 iDocid = apSegment[0]->iDocid;
        fts3SegReaderNextDocid(apSegment[0], &pList, &nList);
        j = 1;
        while( j<nMerge
            && apSegment[j]->pOffsetList
            && apSegment[j]->iDocid==iDocid
        ){
          fts3SegReaderNextDocid(apSegment[j], 0, 0);
          j++;
        }

        if( isColFilter ){
          fts3ColumnFilter(pFilter->iCol, &pList, &nList);
        }

        if( !isIgnoreEmpty || nList>0 ){
          nByte = sqlite3Fts3VarintLen(iDocid-iPrev) + (isRequirePos?nList+1:0);
          if( nDoclist+nByte>pCsr->nBuffer ){
            char *aNew;
            pCsr->nBuffer = (nDoclist+nByte)*2;
            aNew = sqlite3_realloc(pCsr->aBuffer, pCsr->nBuffer);
            if( !aNew ){
              return SQLITE_NOMEM;
            }
            pCsr->aBuffer = aNew;
          }
          nDoclist += sqlite3Fts3PutVarint(
              &pCsr->aBuffer[nDoclist], iDocid-iPrev
          );
          iPrev = iDocid;
          if( isRequirePos ){
            memcpy(&pCsr->aBuffer[nDoclist], pList, nList);
            nDoclist += nList;
            pCsr->aBuffer[nDoclist++] = '\0';
          }
        }

        fts3SegReaderSort(apSegment, nMerge, j, fts3SegReaderDoclistCmp);
      }
      if( nDoclist>0 ){
        pCsr->aDoclist = pCsr->aBuffer;
        pCsr->nDoclist = nDoclist;
        rc = SQLITE_ROW;
      }
    }
    pCsr->nAdvance = nMerge;
  }while( rc==SQLITE_OK );

  return rc;
}

void sqlite3Fts3SegReaderFinish(
  Fts3SegReaderCursor *pCsr       
){
  if( pCsr ){
    int i;
    for(i=0; i<pCsr->nSegment; i++){
      sqlite3Fts3SegReaderFree(pCsr->apSegment[i]);
    }
    sqlite3_free(pCsr->apSegment);
    sqlite3_free(pCsr->aBuffer);

    pCsr->nSegment = 0;
    pCsr->apSegment = 0;
    pCsr->aBuffer = 0;
  }
}

static int fts3SegmentMerge(Fts3Table *p, int iLevel){
  int rc;                         
  int iIdx = 0;                   
  int iNewLevel = 0;              
  SegmentWriter *pWriter = 0;     
  Fts3SegFilter filter;           
  Fts3SegReaderCursor csr;        

  rc = sqlite3Fts3SegReaderCursor(p, iLevel, 0, 0, 1, 0, &csr);
  if( rc!=SQLITE_OK || csr.nSegment==0 ) goto finished;

  if( iLevel==FTS3_SEGCURSOR_ALL ){
    int nDummy; 
    if( csr.nSegment==1 ){
      rc = SQLITE_DONE;
      goto finished;
    }
    rc = fts3SegmentCountMax(p, &nDummy, &iNewLevel);
  }else{
    iNewLevel = iLevel+1;
    rc = fts3AllocateSegdirIdx(p, iNewLevel, &iIdx);
  }
  if( rc!=SQLITE_OK ) goto finished;
  assert( csr.nSegment>0 );
  assert( iNewLevel>=0 );

  memset(&filter, 0, sizeof(Fts3SegFilter));
  filter.flags = FTS3_SEGMENT_REQUIRE_POS;
  filter.flags |= (iLevel==FTS3_SEGCURSOR_ALL ? FTS3_SEGMENT_IGNORE_EMPTY : 0);

  rc = sqlite3Fts3SegReaderStart(p, &csr, &filter);
  while( SQLITE_OK==rc ){
    rc = sqlite3Fts3SegReaderStep(p, &csr);
    if( rc!=SQLITE_ROW ) break;
    rc = fts3SegWriterAdd(p, &pWriter, 1, 
        csr.zTerm, csr.nTerm, csr.aDoclist, csr.nDoclist);
  }
  if( rc!=SQLITE_OK ) goto finished;
  assert( pWriter );

  rc = fts3DeleteSegdir(p, iLevel, csr.apSegment, csr.nSegment);
  if( rc!=SQLITE_OK ) goto finished;
  rc = fts3SegWriterFlush(p, pWriter, iNewLevel, iIdx);

 finished:
  fts3SegWriterFree(pWriter);
  sqlite3Fts3SegReaderFinish(&csr);
  return rc;
}


int sqlite3Fts3PendingTermsFlush(Fts3Table *p){
  return fts3SegmentMerge(p, FTS3_SEGCURSOR_PENDING);
}

static void fts3EncodeIntArray(
  int N,             
  u32 *a,            
  char *zBuf,        
  int *pNBuf         
){
  int i, j;
  for(i=j=0; i<N; i++){
    j += sqlite3Fts3PutVarint(&zBuf[j], (sqlite3_int64)a[i]);
  }
  *pNBuf = j;
}

static void fts3DecodeIntArray(
  int N,             
  u32 *a,            
  const char *zBuf,  
  int nBuf           
){
  int i, j;
  UNUSED_PARAMETER(nBuf);
  for(i=j=0; i<N; i++){
    sqlite3_int64 x;
    j += sqlite3Fts3GetVarint(&zBuf[j], &x);
    assert(j<=nBuf);
    a[i] = (u32)(x & 0xffffffff);
  }
}

static void fts3InsertDocsize(
  int *pRC,         
  Fts3Table *p,     
  u32 *aSz          
){
  char *pBlob;             
  int nBlob;               
  sqlite3_stmt *pStmt;     
  int rc;                  

  if( *pRC ) return;
  pBlob = sqlite3_malloc( 10*p->nColumn );
  if( pBlob==0 ){
    *pRC = SQLITE_NOMEM;
    return;
  }
  fts3EncodeIntArray(p->nColumn, aSz, pBlob, &nBlob);
  rc = fts3SqlStmt(p, SQL_REPLACE_DOCSIZE, &pStmt, 0);
  if( rc ){
    sqlite3_free(pBlob);
    *pRC = rc;
    return;
  }
  sqlite3_bind_int64(pStmt, 1, p->iPrevDocid);
  sqlite3_bind_blob(pStmt, 2, pBlob, nBlob, sqlite3_free);
  sqlite3_step(pStmt);
  *pRC = sqlite3_reset(pStmt);
}

static void fts3UpdateDocTotals(
  int *pRC,                       
  Fts3Table *p,                   
  u32 *aSzIns,                    
  u32 *aSzDel,                    
  int nChng                       
){
  char *pBlob;             /* Storage for BLOB written into %_stat */
  int nBlob;               /* Size of BLOB written into %_stat */
  u32 *a;                  
  sqlite3_stmt *pStmt;     
  int i;                   
  int rc;                  

  const int nStat = p->nColumn+2;

  if( *pRC ) return;
  a = sqlite3_malloc( (sizeof(u32)+10)*nStat );
  if( a==0 ){
    *pRC = SQLITE_NOMEM;
    return;
  }
  pBlob = (char*)&a[nStat];
  rc = fts3SqlStmt(p, SQL_SELECT_DOCTOTAL, &pStmt, 0);
  if( rc ){
    sqlite3_free(a);
    *pRC = rc;
    return;
  }
  if( sqlite3_step(pStmt)==SQLITE_ROW ){
    fts3DecodeIntArray(nStat, a,
         sqlite3_column_blob(pStmt, 0),
         sqlite3_column_bytes(pStmt, 0));
  }else{
    memset(a, 0, sizeof(u32)*(nStat) );
  }
  sqlite3_reset(pStmt);
  if( nChng<0 && a[0]<(u32)(-nChng) ){
    a[0] = 0;
  }else{
    a[0] += nChng;
  }
  for(i=0; i<p->nColumn+1; i++){
    u32 x = a[i+1];
    if( x+aSzIns[i] < aSzDel[i] ){
      x = 0;
    }else{
      x = x + aSzIns[i] - aSzDel[i];
    }
    a[i+1] = x;
  }
  fts3EncodeIntArray(nStat, a, pBlob, &nBlob);
  rc = fts3SqlStmt(p, SQL_REPLACE_DOCTOTAL, &pStmt, 0);
  if( rc ){
    sqlite3_free(a);
    *pRC = rc;
    return;
  }
  sqlite3_bind_blob(pStmt, 1, pBlob, nBlob, SQLITE_STATIC);
  sqlite3_step(pStmt);
  *pRC = sqlite3_reset(pStmt);
  sqlite3_free(a);
}

static int fts3SpecialInsert(Fts3Table *p, sqlite3_value *pVal){
  int rc;                         
  const char *zVal = (const char *)sqlite3_value_text(pVal);
  int nVal = sqlite3_value_bytes(pVal);

  if( !zVal ){
    return SQLITE_NOMEM;
  }else if( nVal==8 && 0==sqlite3_strnicmp(zVal, "optimize", 8) ){
    rc = fts3SegmentMerge(p, FTS3_SEGCURSOR_ALL);
    if( rc==SQLITE_DONE ){
      rc = SQLITE_OK;
    }else{
      sqlite3Fts3PendingTermsClear(p);
    }
#ifdef SQLITE_TEST
  }else if( nVal>9 && 0==sqlite3_strnicmp(zVal, "nodesize=", 9) ){
    p->nNodeSize = atoi(&zVal[9]);
    rc = SQLITE_OK;
  }else if( nVal>11 && 0==sqlite3_strnicmp(zVal, "maxpending=", 9) ){
    p->nMaxPendingData = atoi(&zVal[11]);
    rc = SQLITE_OK;
#endif
  }else{
    rc = SQLITE_ERROR;
  }

  sqlite3Fts3SegmentsClose(p);
  return rc;
}

char *sqlite3Fts3DeferredDoclist(Fts3DeferredToken *pDeferred, int *pnByte){
  if( pDeferred->pList ){
    *pnByte = pDeferred->pList->nData;
    return pDeferred->pList->aData;
  }
  *pnByte = 0;
  return 0;
}

static void fts3DeferredDoclistClear(Fts3Expr *pExpr){
  if( pExpr ){
    fts3DeferredDoclistClear(pExpr->pLeft);
    fts3DeferredDoclistClear(pExpr->pRight);
    if( pExpr->isLoaded ){
      sqlite3_free(pExpr->aDoclist);
      pExpr->isLoaded = 0;
      pExpr->aDoclist = 0;
      pExpr->nDoclist = 0;
      pExpr->pCurrent = 0;
      pExpr->iCurrent = 0;
    }
  }
}

void sqlite3Fts3FreeDeferredDoclists(Fts3Cursor *pCsr){
  Fts3DeferredToken *pDef;
  for(pDef=pCsr->pDeferred; pDef; pDef=pDef->pNext){
    sqlite3_free(pDef->pList);
    pDef->pList = 0;
  }
  if( pCsr->pDeferred ){
    fts3DeferredDoclistClear(pCsr->pExpr);
  }
}

void sqlite3Fts3FreeDeferredTokens(Fts3Cursor *pCsr){
  Fts3DeferredToken *pDef;
  Fts3DeferredToken *pNext;
  for(pDef=pCsr->pDeferred; pDef; pDef=pNext){
    pNext = pDef->pNext;
    sqlite3_free(pDef->pList);
    sqlite3_free(pDef);
  }
  pCsr->pDeferred = 0;
}

int sqlite3Fts3CacheDeferredDoclists(Fts3Cursor *pCsr){
  int rc = SQLITE_OK;             
  if( pCsr->pDeferred ){
    int i;                        
    sqlite3_int64 iDocid;         
    Fts3DeferredToken *pDef;      
  
    Fts3Table *p = (Fts3Table *)pCsr->base.pVtab;
    sqlite3_tokenizer *pT = p->pTokenizer;
    sqlite3_tokenizer_module const *pModule = pT->pModule;
   
    assert( pCsr->isRequireSeek==0 );
    iDocid = sqlite3_column_int64(pCsr->pStmt, 0);
  
    for(i=0; i<p->nColumn && rc==SQLITE_OK; i++){
      const char *zText = (const char *)sqlite3_column_text(pCsr->pStmt, i+1);
      sqlite3_tokenizer_cursor *pTC = 0;
  
      rc = pModule->xOpen(pT, zText, -1, &pTC);
      while( rc==SQLITE_OK ){
        char const *zToken;       
        int nToken;               
        int iDum1, iDum2;         
        int iPos;                 
  
        pTC->pTokenizer = pT;
        rc = pModule->xNext(pTC, &zToken, &nToken, &iDum1, &iDum2, &iPos);
        for(pDef=pCsr->pDeferred; pDef && rc==SQLITE_OK; pDef=pDef->pNext){
          Fts3PhraseToken *pPT = pDef->pToken;
          if( (pDef->iCol>=p->nColumn || pDef->iCol==i)
           && (pPT->n==nToken || (pPT->isPrefix && pPT->n<nToken))
           && (0==memcmp(zToken, pPT->z, pPT->n))
          ){
            fts3PendingListAppend(&pDef->pList, iDocid, i, iPos, &rc);
          }
        }
      }
      if( pTC ) pModule->xClose(pTC);
      if( rc==SQLITE_DONE ) rc = SQLITE_OK;
    }
  
    for(pDef=pCsr->pDeferred; pDef && rc==SQLITE_OK; pDef=pDef->pNext){
      if( pDef->pList ){
        rc = fts3PendingListAppendVarint(&pDef->pList, 0);
      }
    }
  }

  return rc;
}

int sqlite3Fts3DeferToken(
  Fts3Cursor *pCsr,               
  Fts3PhraseToken *pToken,        
  int iCol                        
){
  Fts3DeferredToken *pDeferred;
  pDeferred = sqlite3_malloc(sizeof(*pDeferred));
  if( !pDeferred ){
    return SQLITE_NOMEM;
  }
  memset(pDeferred, 0, sizeof(*pDeferred));
  pDeferred->pToken = pToken;
  pDeferred->pNext = pCsr->pDeferred; 
  pDeferred->iCol = iCol;
  pCsr->pDeferred = pDeferred;

  assert( pToken->pDeferred==0 );
  pToken->pDeferred = pDeferred;

  return SQLITE_OK;
}


int sqlite3Fts3UpdateMethod(
  sqlite3_vtab *pVtab,            
  int nArg,                       
  sqlite3_value **apVal,          
  sqlite_int64 *pRowid            
){
  Fts3Table *p = (Fts3Table *)pVtab;
  int rc = SQLITE_OK;             
  int isRemove = 0;               
  sqlite3_int64 iRemove = 0;      
  u32 *aSzIns;                    
  u32 *aSzDel;                    
  int nChng = 0;                  

  assert( p->pSegments==0 );

  
  aSzIns = sqlite3_malloc( sizeof(aSzIns[0])*(p->nColumn+1)*2 );
  if( aSzIns==0 ) return SQLITE_NOMEM;
  aSzDel = &aSzIns[p->nColumn+1];
  memset(aSzIns, 0, sizeof(aSzIns[0])*(p->nColumn+1)*2);

  
  if( sqlite3_value_type(apVal[0])!=SQLITE_NULL ){
    int isEmpty = 0;
    rc = fts3IsEmpty(p, apVal, &isEmpty);
    if( rc==SQLITE_OK ){
      if( isEmpty ){
        rc = fts3DeleteAll(p);
      }else{
        isRemove = 1;
        iRemove = sqlite3_value_int64(apVal[0]);
        rc = fts3PendingTermsDocid(p, iRemove);
        fts3DeleteTerms(&rc, p, apVal, aSzDel);
        fts3SqlExec(&rc, p, SQL_DELETE_CONTENT, apVal);
        if( p->bHasDocsize ){
          fts3SqlExec(&rc, p, SQL_DELETE_DOCSIZE, apVal);
        }
        nChng--;
      }
    }
  }else if( sqlite3_value_type(apVal[p->nColumn+2])!=SQLITE_NULL ){
    sqlite3_free(aSzIns);
    return fts3SpecialInsert(p, apVal[p->nColumn+2]);
  }
  
  
  if( nArg>1 && rc==SQLITE_OK ){
    rc = fts3InsertData(p, apVal, pRowid);
    if( rc==SQLITE_OK && (!isRemove || *pRowid!=iRemove) ){
      rc = fts3PendingTermsDocid(p, *pRowid);
    }
    if( rc==SQLITE_OK ){
      rc = fts3InsertTerms(p, apVal, aSzIns);
    }
    if( p->bHasDocsize ){
      fts3InsertDocsize(&rc, p, aSzIns);
    }
    nChng++;
  }

  if( p->bHasStat ){
    fts3UpdateDocTotals(&rc, p, aSzIns, aSzDel, nChng);
  }

  sqlite3_free(aSzIns);
  sqlite3Fts3SegmentsClose(p);
  return rc;
}

int sqlite3Fts3Optimize(Fts3Table *p){
  int rc;
  rc = sqlite3_exec(p->db, "SAVEPOINT fts3", 0, 0, 0);
  if( rc==SQLITE_OK ){
    rc = fts3SegmentMerge(p, FTS3_SEGCURSOR_ALL);
    if( rc==SQLITE_OK ){
      rc = sqlite3_exec(p->db, "RELEASE fts3", 0, 0, 0);
      if( rc==SQLITE_OK ){
        sqlite3Fts3PendingTermsClear(p);
      }
    }else{
      sqlite3_exec(p->db, "ROLLBACK TO fts3", 0, 0, 0);
      sqlite3_exec(p->db, "RELEASE fts3", 0, 0, 0);
    }
  }
  sqlite3Fts3SegmentsClose(p);
  return rc;
}

#endif

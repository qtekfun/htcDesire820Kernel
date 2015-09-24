/*
** 2011 Jan 27
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

#if !defined(SQLITE_CORE) || defined(SQLITE_ENABLE_FTS3)

#include "fts3Int.h"
#include <string.h>
#include <assert.h>

typedef struct Fts3auxTable Fts3auxTable;
typedef struct Fts3auxCursor Fts3auxCursor;

struct Fts3auxTable {
  sqlite3_vtab base;              
  Fts3Table *pFts3Tab;
};

struct Fts3auxCursor {
  sqlite3_vtab_cursor base;       
  Fts3SegReaderCursor csr;        
  Fts3SegFilter filter;
  char *zStop;
  int nStop;                      
  int isEof;                      
  sqlite3_int64 iRowid;           

  int iCol;                       
  int nStat;                      
  struct Fts3auxColstats {
    sqlite3_int64 nDoc;           
    sqlite3_int64 nOcc;           
  } *aStat;
};

#define FTS3_TERMS_SCHEMA "CREATE TABLE x(term, col, documents, occurrences)"

static int fts3auxConnectMethod(
  sqlite3 *db,                    
  void *pUnused,                  
  int argc,                       
  const char * const *argv,       
  sqlite3_vtab **ppVtab,          
  char **pzErr                    
){
  char const *zDb;                
  char const *zFts3;              
  int nDb;                        
  int nFts3;                      
  int nByte;                      
  int rc;                         
  Fts3auxTable *p;                

  UNUSED_PARAMETER(pUnused);

  
  if( argc!=4 ){
    *pzErr = sqlite3_mprintf(
        "wrong number of arguments to fts4aux constructor"
    );
    return SQLITE_ERROR;
  }

  zDb = argv[1]; 
  nDb = strlen(zDb);
  zFts3 = argv[3];
  nFts3 = strlen(zFts3);

  rc = sqlite3_declare_vtab(db, FTS3_TERMS_SCHEMA);
  if( rc!=SQLITE_OK ) return rc;

  nByte = sizeof(Fts3auxTable) + sizeof(Fts3Table) + nDb + nFts3 + 2;
  p = (Fts3auxTable *)sqlite3_malloc(nByte);
  if( !p ) return SQLITE_NOMEM;
  memset(p, 0, nByte);

  p->pFts3Tab = (Fts3Table *)&p[1];
  p->pFts3Tab->zDb = (char *)&p->pFts3Tab[1];
  p->pFts3Tab->zName = &p->pFts3Tab->zDb[nDb+1];
  p->pFts3Tab->db = db;

  memcpy((char *)p->pFts3Tab->zDb, zDb, nDb);
  memcpy((char *)p->pFts3Tab->zName, zFts3, nFts3);
  sqlite3Fts3Dequote((char *)p->pFts3Tab->zName);

  *ppVtab = (sqlite3_vtab *)p;
  return SQLITE_OK;
}

static int fts3auxDisconnectMethod(sqlite3_vtab *pVtab){
  Fts3auxTable *p = (Fts3auxTable *)pVtab;
  Fts3Table *pFts3 = p->pFts3Tab;
  int i;

  
  for(i=0; i<SizeofArray(pFts3->aStmt); i++){
    sqlite3_finalize(pFts3->aStmt[i]);
  }
  sqlite3_free(pFts3->zSegmentsTbl);
  sqlite3_free(p);
  return SQLITE_OK;
}

#define FTS4AUX_EQ_CONSTRAINT 1
#define FTS4AUX_GE_CONSTRAINT 2
#define FTS4AUX_LE_CONSTRAINT 4

static int fts3auxBestIndexMethod(
  sqlite3_vtab *pVTab, 
  sqlite3_index_info *pInfo
){
  int i;
  int iEq = -1;
  int iGe = -1;
  int iLe = -1;

  UNUSED_PARAMETER(pVTab);

  
  if( pInfo->nOrderBy==1 
   && pInfo->aOrderBy[0].iColumn==0 
   && pInfo->aOrderBy[0].desc==0
  ){
    pInfo->orderByConsumed = 1;
  }

  
  for(i=0; i<pInfo->nConstraint; i++){
    if( pInfo->aConstraint[i].usable && pInfo->aConstraint[i].iColumn==0 ){
      int op = pInfo->aConstraint[i].op;
      if( op==SQLITE_INDEX_CONSTRAINT_EQ ) iEq = i;
      if( op==SQLITE_INDEX_CONSTRAINT_LT ) iLe = i;
      if( op==SQLITE_INDEX_CONSTRAINT_LE ) iLe = i;
      if( op==SQLITE_INDEX_CONSTRAINT_GT ) iGe = i;
      if( op==SQLITE_INDEX_CONSTRAINT_GE ) iGe = i;
    }
  }

  if( iEq>=0 ){
    pInfo->idxNum = FTS4AUX_EQ_CONSTRAINT;
    pInfo->aConstraintUsage[iEq].argvIndex = 1;
    pInfo->estimatedCost = 5;
  }else{
    pInfo->idxNum = 0;
    pInfo->estimatedCost = 20000;
    if( iGe>=0 ){
      pInfo->idxNum += FTS4AUX_GE_CONSTRAINT;
      pInfo->aConstraintUsage[iGe].argvIndex = 1;
      pInfo->estimatedCost /= 2;
    }
    if( iLe>=0 ){
      pInfo->idxNum += FTS4AUX_LE_CONSTRAINT;
      pInfo->aConstraintUsage[iLe].argvIndex = 1 + (iGe>=0);
      pInfo->estimatedCost /= 2;
    }
  }

  return SQLITE_OK;
}

static int fts3auxOpenMethod(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCsr){
  Fts3auxCursor *pCsr;            

  UNUSED_PARAMETER(pVTab);

  pCsr = (Fts3auxCursor *)sqlite3_malloc(sizeof(Fts3auxCursor));
  if( !pCsr ) return SQLITE_NOMEM;
  memset(pCsr, 0, sizeof(Fts3auxCursor));

  *ppCsr = (sqlite3_vtab_cursor *)pCsr;
  return SQLITE_OK;
}

static int fts3auxCloseMethod(sqlite3_vtab_cursor *pCursor){
  Fts3Table *pFts3 = ((Fts3auxTable *)pCursor->pVtab)->pFts3Tab;
  Fts3auxCursor *pCsr = (Fts3auxCursor *)pCursor;

  sqlite3Fts3SegmentsClose(pFts3);
  sqlite3Fts3SegReaderFinish(&pCsr->csr);
  sqlite3_free((void *)pCsr->filter.zTerm);
  sqlite3_free(pCsr->zStop);
  sqlite3_free(pCsr->aStat);
  sqlite3_free(pCsr);
  return SQLITE_OK;
}

static int fts3auxGrowStatArray(Fts3auxCursor *pCsr, int nSize){
  if( nSize>pCsr->nStat ){
    struct Fts3auxColstats *aNew;
    aNew = (struct Fts3auxColstats *)sqlite3_realloc(pCsr->aStat, 
        sizeof(struct Fts3auxColstats) * nSize
    );
    if( aNew==0 ) return SQLITE_NOMEM;
    memset(&aNew[pCsr->nStat], 0, 
        sizeof(struct Fts3auxColstats) * (nSize - pCsr->nStat)
    );
    pCsr->aStat = aNew;
    pCsr->nStat = nSize;
  }
  return SQLITE_OK;
}

static int fts3auxNextMethod(sqlite3_vtab_cursor *pCursor){
  Fts3auxCursor *pCsr = (Fts3auxCursor *)pCursor;
  Fts3Table *pFts3 = ((Fts3auxTable *)pCursor->pVtab)->pFts3Tab;
  int rc;

  
  pCsr->iRowid++;

  for(pCsr->iCol++; pCsr->iCol<pCsr->nStat; pCsr->iCol++){
    if( pCsr->aStat[pCsr->iCol].nDoc>0 ) return SQLITE_OK;
  }

  rc = sqlite3Fts3SegReaderStep(pFts3, &pCsr->csr);
  if( rc==SQLITE_ROW ){
    int i = 0;
    int nDoclist = pCsr->csr.nDoclist;
    char *aDoclist = pCsr->csr.aDoclist;
    int iCol;

    int eState = 0;

    if( pCsr->zStop ){
      int n = (pCsr->nStop<pCsr->csr.nTerm) ? pCsr->nStop : pCsr->csr.nTerm;
      int mc = memcmp(pCsr->zStop, pCsr->csr.zTerm, n);
      if( mc<0 || (mc==0 && pCsr->csr.nTerm>pCsr->nStop) ){
        pCsr->isEof = 1;
        return SQLITE_OK;
      }
    }

    if( fts3auxGrowStatArray(pCsr, 2) ) return SQLITE_NOMEM;
    memset(pCsr->aStat, 0, sizeof(struct Fts3auxColstats) * pCsr->nStat);
    iCol = 0;

    while( i<nDoclist ){
      sqlite3_int64 v = 0;

      i += sqlite3Fts3GetVarint(&aDoclist[i], &v);
      switch( eState ){
        
        case 0:
          pCsr->aStat[0].nDoc++;
          eState = 1;
          iCol = 0;
          break;

        case 1:
          assert( iCol==0 );
          if( v>1 ){
            pCsr->aStat[1].nDoc++;
          }
          eState = 2;
          

        case 2:
          if( v==0 ){       
            eState = 0;
          }else if( v==1 ){ 
            eState = 3;
          }else{            
            pCsr->aStat[iCol+1].nOcc++;
            pCsr->aStat[0].nOcc++;
          }
          break;

        
        default: assert( eState==3 );
          iCol = (int)v;
          if( fts3auxGrowStatArray(pCsr, iCol+2) ) return SQLITE_NOMEM;
          pCsr->aStat[iCol+1].nDoc++;
          eState = 2;
          break;
      }
    }

    pCsr->iCol = 0;
    rc = SQLITE_OK;
  }else{
    pCsr->isEof = 1;
  }
  return rc;
}

static int fts3auxFilterMethod(
  sqlite3_vtab_cursor *pCursor,   
  int idxNum,                     
  const char *idxStr,             
  int nVal,                       
  sqlite3_value **apVal           
){
  Fts3auxCursor *pCsr = (Fts3auxCursor *)pCursor;
  Fts3Table *pFts3 = ((Fts3auxTable *)pCursor->pVtab)->pFts3Tab;
  int rc;
  int isScan;

  UNUSED_PARAMETER(nVal);

  assert( idxStr==0 );
  assert( idxNum==FTS4AUX_EQ_CONSTRAINT || idxNum==0
       || idxNum==FTS4AUX_LE_CONSTRAINT || idxNum==FTS4AUX_GE_CONSTRAINT
       || idxNum==(FTS4AUX_LE_CONSTRAINT|FTS4AUX_GE_CONSTRAINT)
  );
  isScan = (idxNum!=FTS4AUX_EQ_CONSTRAINT);

  
  testcase(pCsr->filter.zTerm);
  sqlite3Fts3SegReaderFinish(&pCsr->csr);
  sqlite3_free((void *)pCsr->filter.zTerm);
  sqlite3_free(pCsr->aStat);
  memset(&pCsr->csr, 0, ((u8*)&pCsr[1]) - (u8*)&pCsr->csr);

  pCsr->filter.flags = FTS3_SEGMENT_REQUIRE_POS|FTS3_SEGMENT_IGNORE_EMPTY;
  if( isScan ) pCsr->filter.flags |= FTS3_SEGMENT_SCAN;

  if( idxNum&(FTS4AUX_EQ_CONSTRAINT|FTS4AUX_GE_CONSTRAINT) ){
    const unsigned char *zStr = sqlite3_value_text(apVal[0]);
    if( zStr ){
      pCsr->filter.zTerm = sqlite3_mprintf("%s", zStr);
      pCsr->filter.nTerm = sqlite3_value_bytes(apVal[0]);
      if( pCsr->filter.zTerm==0 ) return SQLITE_NOMEM;
    }
  }
  if( idxNum&FTS4AUX_LE_CONSTRAINT ){
    int iIdx = (idxNum&FTS4AUX_GE_CONSTRAINT) ? 1 : 0;
    pCsr->zStop = sqlite3_mprintf("%s", sqlite3_value_text(apVal[iIdx]));
    pCsr->nStop = sqlite3_value_bytes(apVal[iIdx]);
    if( pCsr->zStop==0 ) return SQLITE_NOMEM;
  }

  rc = sqlite3Fts3SegReaderCursor(pFts3, FTS3_SEGCURSOR_ALL,
      pCsr->filter.zTerm, pCsr->filter.nTerm, 0, isScan, &pCsr->csr
  );
  if( rc==SQLITE_OK ){
    rc = sqlite3Fts3SegReaderStart(pFts3, &pCsr->csr, &pCsr->filter);
  }

  if( rc==SQLITE_OK ) rc = fts3auxNextMethod(pCursor);
  return rc;
}

static int fts3auxEofMethod(sqlite3_vtab_cursor *pCursor){
  Fts3auxCursor *pCsr = (Fts3auxCursor *)pCursor;
  return pCsr->isEof;
}

static int fts3auxColumnMethod(
  sqlite3_vtab_cursor *pCursor,   
  sqlite3_context *pContext,      
  int iCol                        
){
  Fts3auxCursor *p = (Fts3auxCursor *)pCursor;

  assert( p->isEof==0 );
  if( iCol==0 ){        
    sqlite3_result_text(pContext, p->csr.zTerm, p->csr.nTerm, SQLITE_TRANSIENT);
  }else if( iCol==1 ){  
    if( p->iCol ){
      sqlite3_result_int(pContext, p->iCol-1);
    }else{
      sqlite3_result_text(pContext, "*", -1, SQLITE_STATIC);
    }
  }else if( iCol==2 ){  
    sqlite3_result_int64(pContext, p->aStat[p->iCol].nDoc);
  }else{                
    sqlite3_result_int64(pContext, p->aStat[p->iCol].nOcc);
  }

  return SQLITE_OK;
}

static int fts3auxRowidMethod(
  sqlite3_vtab_cursor *pCursor,   
  sqlite_int64 *pRowid            
){
  Fts3auxCursor *pCsr = (Fts3auxCursor *)pCursor;
  *pRowid = pCsr->iRowid;
  return SQLITE_OK;
}

int sqlite3Fts3InitAux(sqlite3 *db){
  static const sqlite3_module fts3aux_module = {
     0,                           
     fts3auxConnectMethod,        
     fts3auxConnectMethod,        
     fts3auxBestIndexMethod,      
     fts3auxDisconnectMethod,     
     fts3auxDisconnectMethod,     
     fts3auxOpenMethod,           
     fts3auxCloseMethod,          
     fts3auxFilterMethod,         
     fts3auxNextMethod,           
     fts3auxEofMethod,            
     fts3auxColumnMethod,         
     fts3auxRowidMethod,          
     0,                           
     0,                           
     0,                           
     0,                           
     0,                           
     0,                           
     0                            
  };
  int rc;                         

  rc = sqlite3_create_module(db, "fts4aux", &fts3aux_module, 0);
  return rc;
}

#endif 

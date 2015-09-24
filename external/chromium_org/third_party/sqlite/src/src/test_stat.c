/*
** 2010 July 12
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
** This file contains an implementation of the "dbstat" virtual table.
**
** The dbstat virtual table is used to extract low-level formatting
** information from an SQLite database in order to implement the
** "sqlite3_analyzer" utility.  See the ../tool/spaceanal.tcl script
** for an example implementation.
*/

#include "sqliteInt.h"

#ifndef SQLITE_OMIT_VIRTUALTABLE

#define VTAB_SCHEMA                                                         \
  "CREATE TABLE xx( "                                                       \
  "  name       STRING,           /* Name of table or index */"             \
  "  path       INTEGER,          /* Path to page from root */"             \
  "  pageno     INTEGER,          /* Page number */"                        \
  "  pagetype   STRING,           /* 'internal', 'leaf' or 'overflow' */"   \
  "  ncell      INTEGER,          /* Cells on page (0 for overflow) */"     \
  "  payload    INTEGER,          /* Bytes of payload on this page */"      \
  "  unused     INTEGER,          /* Bytes of unused space on this page */" \
  "  mx_payload INTEGER           /* Largest payload size of all cells */"  \
  ");"

#if 0
#define VTAB_SCHEMA2                                                        \
  "CREATE TABLE yy( "                                                       \
  "  pageno   INTEGER,            /* B-tree page number */"                 \
  "  cellno   INTEGER,            /* Cell number within page */"            \
  "  local    INTEGER,            /* Bytes of content stored locally */"    \
  "  payload  INTEGER,            /* Total cell payload size */"            \
  "  novfl    INTEGER             /* Number of overflow pages */"           \
  ");"
#endif


typedef struct StatTable StatTable;
typedef struct StatCursor StatCursor;
typedef struct StatPage StatPage;
typedef struct StatCell StatCell;

struct StatCell {
  int nLocal;                     
  u32 iChildPg;                   
  int nOvfl;                      
  u32 *aOvfl;                     
  int nLastOvfl;                  
  int iOvfl;                      
};

struct StatPage {
  u32 iPgno;
  DbPage *pPg;
  int iCell;

  char *zPath;                    

  
  u8 flags;                       
  int nCell;                      
  int nUnused;                    
  StatCell *aCell;                
  u32 iRightChildPg;              
  int nMxPayload;                 
};

struct StatCursor {
  sqlite3_vtab_cursor base;
  sqlite3_stmt *pStmt;            
  int isEof;                      

  StatPage aPage[32];
  int iPage;                      

  
  char *zName;                    
  char *zPath;                    
  u32 iPageno;                    
  char *zPagetype;                
  int nCell;                      
  int nPayload;                   
  int nUnused;                    
  int nMxPayload;                 
};

struct StatTable {
  sqlite3_vtab base;
  sqlite3 *db;
};

#ifndef get2byte
# define get2byte(x)   ((x)[0]<<8 | (x)[1])
#endif

static int statConnect(
  sqlite3 *db,
  void *pAux,
  int argc, const char *const*argv,
  sqlite3_vtab **ppVtab,
  char **pzErr
){
  StatTable *pTab;

  pTab = (StatTable *)sqlite3_malloc(sizeof(StatTable));
  memset(pTab, 0, sizeof(StatTable));
  pTab->db = db;

  sqlite3_declare_vtab(db, VTAB_SCHEMA);
  *ppVtab = &pTab->base;
  return SQLITE_OK;
}

static int statDisconnect(sqlite3_vtab *pVtab){
  sqlite3_free(pVtab);
  return SQLITE_OK;
}

static int statBestIndex(sqlite3_vtab *tab, sqlite3_index_info *pIdxInfo){

  if( ( pIdxInfo->nOrderBy==1
     && pIdxInfo->aOrderBy[0].iColumn==0
     && pIdxInfo->aOrderBy[0].desc==0
     ) ||
      ( pIdxInfo->nOrderBy==2
     && pIdxInfo->aOrderBy[0].iColumn==0
     && pIdxInfo->aOrderBy[0].desc==0
     && pIdxInfo->aOrderBy[1].iColumn==1
     && pIdxInfo->aOrderBy[1].desc==0
     )
  ){
    pIdxInfo->orderByConsumed = 1;
  }

  pIdxInfo->estimatedCost = 10.0;
  return SQLITE_OK;
}

static int statOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor){
  StatTable *pTab = (StatTable *)pVTab;
  StatCursor *pCsr;
  int rc;

  pCsr = (StatCursor *)sqlite3_malloc(sizeof(StatCursor));
  memset(pCsr, 0, sizeof(StatCursor));
  pCsr->base.pVtab = pVTab;

  rc = sqlite3_prepare_v2(pTab->db, 
      "SELECT 'sqlite_master' AS name, 1 AS rootpage, 'table' AS type"
      "  UNION ALL  "
      "SELECT name, rootpage, type FROM sqlite_master WHERE rootpage!=0"
      "  ORDER BY name", -1,
      &pCsr->pStmt, 0
  );
  if( rc!=SQLITE_OK ){
    sqlite3_free(pCsr);
    return rc;
  }

  *ppCursor = (sqlite3_vtab_cursor *)pCsr;
  return SQLITE_OK;
}

static void statClearPage(StatPage *p){
  int i;
  for(i=0; i<p->nCell; i++){
    sqlite3_free(p->aCell[i].aOvfl);
  }
  sqlite3PagerUnref(p->pPg);
  sqlite3_free(p->aCell);
  sqlite3_free(p->zPath);
  memset(p, 0, sizeof(StatPage));
}

static void statResetCsr(StatCursor *pCsr){
  int i;
  sqlite3_reset(pCsr->pStmt);
  for(i=0; i<ArraySize(pCsr->aPage); i++){
    statClearPage(&pCsr->aPage[i]);
  }
  pCsr->iPage = 0;
  sqlite3_free(pCsr->zPath);
  pCsr->zPath = 0;
}

static int statClose(sqlite3_vtab_cursor *pCursor){
  StatCursor *pCsr = (StatCursor *)pCursor;
  statResetCsr(pCsr);
  sqlite3_finalize(pCsr->pStmt);
  sqlite3_free(pCsr);
  return SQLITE_OK;
}

static void getLocalPayload(
  int nUsable,                    
  u8 flags,                       
  int nTotal,                     
  int *pnLocal                    
){
  int nLocal;
  int nMinLocal;
  int nMaxLocal;
 
  if( flags==0x0D ){              
    nMinLocal = (nUsable - 12) * 32 / 255 - 23;
    nMaxLocal = nUsable - 35;
  }else{                          
    nMinLocal = (nUsable - 12) * 32 / 255 - 23;
    nMaxLocal = (nUsable - 12) * 64 / 255 - 23;
  }

  nLocal = nMinLocal + (nTotal - nMinLocal) % (nUsable - 4);
  if( nLocal>nMaxLocal ) nLocal = nMinLocal;
  *pnLocal = nLocal;
}

static int statDecodePage(Btree *pBt, StatPage *p){
  int nUnused;
  int iOff;
  int nHdr;
  int isLeaf;

  u8 *aData = sqlite3PagerGetData(p->pPg);
  u8 *aHdr = &aData[p->iPgno==1 ? 100 : 0];

  p->flags = aHdr[0];
  p->nCell = get2byte(&aHdr[3]);
  p->nMxPayload = 0;

  isLeaf = (p->flags==0x0A || p->flags==0x0D);
  nHdr = 12 - isLeaf*4 + (p->iPgno==1)*100;

  nUnused = get2byte(&aHdr[5]) - nHdr - 2*p->nCell;
  nUnused += (int)aHdr[7];
  iOff = get2byte(&aHdr[1]);
  while( iOff ){
    nUnused += get2byte(&aData[iOff+2]);
    iOff = get2byte(&aData[iOff]);
  }
  p->nUnused = nUnused;
  p->iRightChildPg = isLeaf ? 0 : sqlite3Get4byte(&aHdr[8]);

  if( p->nCell ){
    int i;                        
    int nUsable = sqlite3BtreeGetPageSize(pBt) - sqlite3BtreeGetReserve(pBt);

    p->aCell = sqlite3_malloc((p->nCell+1) * sizeof(StatCell));
    memset(p->aCell, 0, (p->nCell+1) * sizeof(StatCell));

    for(i=0; i<p->nCell; i++){
      StatCell *pCell = &p->aCell[i];

      iOff = get2byte(&aData[nHdr+i*2]);
      if( !isLeaf ){
        pCell->iChildPg = sqlite3Get4byte(&aData[iOff]);
        iOff += 4;
      }
      if( p->flags==0x05 ){
        
      }else{
        u32 nPayload;             
        int nLocal;               
        iOff += getVarint32(&aData[iOff], nPayload);
        if( p->flags==0x0D ){
          u64 dummy;
          iOff += sqlite3GetVarint(&aData[iOff], &dummy);
        }
        if( nPayload>p->nMxPayload ) p->nMxPayload = nPayload;
        getLocalPayload(nUsable, p->flags, nPayload, &nLocal);
        pCell->nLocal = nLocal;
        assert( nPayload>=nLocal );
        assert( nLocal<=(nUsable-35) );
        if( nPayload>nLocal ){
          int j;
          int nOvfl = ((nPayload - nLocal) + nUsable-4 - 1) / (nUsable - 4);
          pCell->nLastOvfl = (nPayload-nLocal) - (nOvfl-1) * (nUsable-4);
          pCell->nOvfl = nOvfl;
          pCell->aOvfl = sqlite3_malloc(sizeof(u32)*nOvfl);
          pCell->aOvfl[0] = sqlite3Get4byte(&aData[iOff+nLocal]);
          for(j=1; j<nOvfl; j++){
            int rc;
            u32 iPrev = pCell->aOvfl[j-1];
            DbPage *pPg = 0;
            rc = sqlite3PagerGet(sqlite3BtreePager(pBt), iPrev, &pPg);
            if( rc!=SQLITE_OK ){
              assert( pPg==0 );
              return rc;
            } 
            pCell->aOvfl[j] = sqlite3Get4byte(sqlite3PagerGetData(pPg));
            sqlite3PagerUnref(pPg);
          }
        }
      }
    }
  }

  return SQLITE_OK;
}

static int statNext(sqlite3_vtab_cursor *pCursor){
  int rc;
  int nPayload;
  StatCursor *pCsr = (StatCursor *)pCursor;
  StatTable *pTab = (StatTable *)pCursor->pVtab;
  Btree *pBt = pTab->db->aDb[0].pBt;
  Pager *pPager = sqlite3BtreePager(pBt);

  sqlite3_free(pCsr->zPath);
  pCsr->zPath = 0;

  if( pCsr->aPage[0].pPg==0 ){
    rc = sqlite3_step(pCsr->pStmt);
    if( rc==SQLITE_ROW ){
      int nPage;
      u32 iRoot = sqlite3_column_int64(pCsr->pStmt, 1);
      sqlite3PagerPagecount(pPager, &nPage);
      if( nPage==0 ){
        pCsr->isEof = 1;
        return sqlite3_reset(pCsr->pStmt);
      }
      rc = sqlite3PagerGet(pPager, iRoot, &pCsr->aPage[0].pPg);
      pCsr->aPage[0].iPgno = iRoot;
      pCsr->aPage[0].iCell = 0;
      pCsr->aPage[0].zPath = sqlite3_mprintf("/");
      pCsr->iPage = 0;
    }else{
      pCsr->isEof = 1;
      return sqlite3_reset(pCsr->pStmt);
    }
  }else{

    
    StatPage *p = &pCsr->aPage[pCsr->iPage];

    while( p->iCell<p->nCell ){
      StatCell *pCell = &p->aCell[p->iCell];
      if( pCell->iOvfl<pCell->nOvfl ){
        int nUsable = sqlite3BtreeGetPageSize(pBt)-sqlite3BtreeGetReserve(pBt);
        pCsr->zName = (char *)sqlite3_column_text(pCsr->pStmt, 0);
        pCsr->iPageno = pCell->aOvfl[pCell->iOvfl];
        pCsr->zPagetype = "overflow";
        pCsr->nCell = 0;
        pCsr->nMxPayload = 0;
        pCsr->zPath = sqlite3_mprintf(
            "%s%.3x+%.6x", p->zPath, p->iCell, pCell->iOvfl
        );
        if( pCell->iOvfl<pCell->nOvfl-1 ){
          pCsr->nUnused = 0;
          pCsr->nPayload = nUsable - 4;
        }else{
          pCsr->nPayload = pCell->nLastOvfl;
          pCsr->nUnused = nUsable - 4 - pCsr->nPayload;
        }
        pCell->iOvfl++;
        return SQLITE_OK;
      }
      if( p->iRightChildPg ) break;
      p->iCell++;
    }

    while( !p->iRightChildPg || p->iCell>p->nCell ){
      statClearPage(p);
      if( pCsr->iPage==0 ) return statNext(pCursor);
      pCsr->iPage--;
      p = &pCsr->aPage[pCsr->iPage];
    }
    pCsr->iPage++;
    assert( p==&pCsr->aPage[pCsr->iPage-1] );

    if( p->iCell==p->nCell ){
      p[1].iPgno = p->iRightChildPg;
    }else{
      p[1].iPgno = p->aCell[p->iCell].iChildPg;
    }
    rc = sqlite3PagerGet(pPager, p[1].iPgno, &p[1].pPg);
    p[1].iCell = 0;
    p[1].zPath = sqlite3_mprintf("%s%.3x/", p->zPath, p->iCell);
    p->iCell++;
  }


  if( rc==SQLITE_OK ){
    int i;
    StatPage *p = &pCsr->aPage[pCsr->iPage];
    pCsr->zName = (char *)sqlite3_column_text(pCsr->pStmt, 0);
    pCsr->iPageno = p->iPgno;

    statDecodePage(pBt, p);

    switch( p->flags ){
      case 0x05:             
      case 0x02:             
        pCsr->zPagetype = "internal";
        break;
      case 0x0D:             
      case 0x0A:             
        pCsr->zPagetype = "leaf";
        break;
      default:
        pCsr->zPagetype = "corrupted";
        break;
    }
    pCsr->nCell = p->nCell;
    pCsr->nUnused = p->nUnused;
    pCsr->nMxPayload = p->nMxPayload;
    pCsr->zPath = sqlite3_mprintf("%s", p->zPath);
    nPayload = 0;
    for(i=0; i<p->nCell; i++){
      nPayload += p->aCell[i].nLocal;
    }
    pCsr->nPayload = nPayload;
  }

  return rc;
}

static int statEof(sqlite3_vtab_cursor *pCursor){
  StatCursor *pCsr = (StatCursor *)pCursor;
  return pCsr->isEof;
}

static int statFilter(
  sqlite3_vtab_cursor *pCursor, 
  int idxNum, const char *idxStr,
  int argc, sqlite3_value **argv
){
  StatCursor *pCsr = (StatCursor *)pCursor;

  statResetCsr(pCsr);
  return statNext(pCursor);
}

static int statColumn(
  sqlite3_vtab_cursor *pCursor, 
  sqlite3_context *ctx, 
  int i
){
  StatCursor *pCsr = (StatCursor *)pCursor;
  switch( i ){
    case 0:            
      sqlite3_result_text(ctx, pCsr->zName, -1, SQLITE_STATIC);
      break;
    case 1:            
      sqlite3_result_text(ctx, pCsr->zPath, -1, SQLITE_TRANSIENT);
      break;
    case 2:            
      sqlite3_result_int64(ctx, pCsr->iPageno);
      break;
    case 3:            
      sqlite3_result_text(ctx, pCsr->zPagetype, -1, SQLITE_STATIC);
      break;
    case 4:            
      sqlite3_result_int(ctx, pCsr->nCell);
      break;
    case 5:            
      sqlite3_result_int(ctx, pCsr->nPayload);
      break;
    case 6:            
      sqlite3_result_int(ctx, pCsr->nUnused);
      break;
    case 7:            
      sqlite3_result_int(ctx, pCsr->nMxPayload);
      break;
  }
  return SQLITE_OK;
}

static int statRowid(sqlite3_vtab_cursor *pCursor, sqlite_int64 *pRowid){
  StatCursor *pCsr = (StatCursor *)pCursor;
  *pRowid = pCsr->iPageno;
  return SQLITE_OK;
}

int sqlite3_dbstat_register(sqlite3 *db){
  static sqlite3_module dbstat_module = {
    0,                            
    statConnect,                  
    statConnect,                  
    statBestIndex,                
    statDisconnect,               
    statDisconnect,               
    statOpen,                     
    statClose,                    
    statFilter,                   
    statNext,                     
    statEof,                      
    statColumn,                   
    statRowid,                    
    0,                            
    0,                            
    0,                            
    0,                            
    0,                            
    0,                            
    0,                            
  };
  sqlite3_create_module(db, "dbstat", &dbstat_module, 0);
  return SQLITE_OK;
}

#endif

#ifdef SQLITE_TEST
#include <tcl.h>

static int test_dbstat(
  void *clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
#ifdef SQLITE_OMIT_VIRTUALTABLE
  Tcl_AppendResult(interp, "dbstat not available because of "
                           "SQLITE_OMIT_VIRTUALTABLE", (void*)0);
  return TCL_ERROR;
#else
  struct SqliteDb { sqlite3 *db; };
  char *zDb;
  Tcl_CmdInfo cmdInfo;

  if( objc!=2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "DB");
    return TCL_ERROR;
  }

  zDb = Tcl_GetString(objv[1]);
  if( Tcl_GetCommandInfo(interp, zDb, &cmdInfo) ){
    sqlite3* db = ((struct SqliteDb*)cmdInfo.objClientData)->db;
    sqlite3_dbstat_register(db);
  }
  return TCL_OK;
#endif
}

int SqlitetestStat_Init(Tcl_Interp *interp){
  Tcl_CreateObjCommand(interp, "register_dbstat_vtab", test_dbstat, 0, 0);
  return TCL_OK;
}
#endif

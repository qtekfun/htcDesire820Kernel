/*
** 2007 May 1
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
**
** This file contains code used to implement incremental BLOB I/O.
*/

#include "sqliteInt.h"
#include "vdbeInt.h"

#ifndef SQLITE_OMIT_INCRBLOB

typedef struct Incrblob Incrblob;
struct Incrblob {
  int flags;              
  int nByte;              
  int iOffset;            
  int iCol;               
  BtCursor *pCsr;         
  sqlite3_stmt *pStmt;    
  sqlite3 *db;            
};


static int blobSeekToRow(Incrblob *p, sqlite3_int64 iRow, char **pzErr){
  int rc;                         
  char *zErr = 0;                 
  Vdbe *v = (Vdbe *)p->pStmt;

  assert( v->aVar[0].flags&MEM_Int );
  v->aVar[0].u.i = iRow;

  rc = sqlite3_step(p->pStmt);
  if( rc==SQLITE_ROW ){
    u32 type = v->apCsr[0]->aType[p->iCol];
    if( type<12 ){
      zErr = sqlite3MPrintf(p->db, "cannot open value of type %s",
          type==0?"null": type==7?"real": "integer"
      );
      rc = SQLITE_ERROR;
      sqlite3_finalize(p->pStmt);
      p->pStmt = 0;
    }else{
      p->iOffset = v->apCsr[0]->aOffset[p->iCol];
      p->nByte = sqlite3VdbeSerialTypeLen(type);
      p->pCsr =  v->apCsr[0]->pCursor;
      sqlite3BtreeEnterCursor(p->pCsr);
      sqlite3BtreeCacheOverflow(p->pCsr);
      sqlite3BtreeLeaveCursor(p->pCsr);
    }
  }

  if( rc==SQLITE_ROW ){
    rc = SQLITE_OK;
  }else if( p->pStmt ){
    rc = sqlite3_finalize(p->pStmt);
    p->pStmt = 0;
    if( rc==SQLITE_OK ){
      zErr = sqlite3MPrintf(p->db, "no such rowid: %lld", iRow);
      rc = SQLITE_ERROR;
    }else{
      zErr = sqlite3MPrintf(p->db, "%s", sqlite3_errmsg(p->db));
    }
  }

  assert( rc!=SQLITE_OK || zErr==0 );
  assert( rc!=SQLITE_ROW && rc!=SQLITE_DONE );

  *pzErr = zErr;
  return rc;
}

int sqlite3_blob_open(
  sqlite3* db,            
  const char *zDb,        
  const char *zTable,     
  const char *zColumn,    
  sqlite_int64 iRow,      
  int flags,              
  sqlite3_blob **ppBlob   
){
  int nAttempt = 0;
  int iCol;               

  static const VdbeOpList openBlob[] = {
    {OP_Transaction, 0, 0, 0},     
    {OP_VerifyCookie, 0, 0, 0},    
    {OP_TableLock, 0, 0, 0},       

    
    {OP_OpenRead, 0, 0, 0},        
    {OP_OpenWrite, 0, 0, 0},       

    {OP_Variable, 1, 1, 1},        
    {OP_NotExists, 0, 10, 1},      
    {OP_Column, 0, 0, 1},          
    {OP_ResultRow, 1, 0, 0},       
    {OP_Goto, 0, 5, 0},            
    {OP_Close, 0, 0, 0},           
    {OP_Halt, 0, 0, 0},            
  };

  int rc = SQLITE_OK;
  char *zErr = 0;
  Table *pTab;
  Parse *pParse = 0;
  Incrblob *pBlob = 0;

  flags = !!flags;                
  *ppBlob = 0;

  sqlite3_mutex_enter(db->mutex);

  pBlob = (Incrblob *)sqlite3DbMallocZero(db, sizeof(Incrblob));
  if( !pBlob ) goto blob_open_out;
  pParse = sqlite3StackAllocRaw(db, sizeof(*pParse));
  if( !pParse ) goto blob_open_out;

  do {
    memset(pParse, 0, sizeof(Parse));
    pParse->db = db;
    sqlite3DbFree(db, zErr);
    zErr = 0;

    sqlite3BtreeEnterAll(db);
    pTab = sqlite3LocateTable(pParse, 0, zTable, zDb);
    if( pTab && IsVirtual(pTab) ){
      pTab = 0;
      sqlite3ErrorMsg(pParse, "cannot open virtual table: %s", zTable);
    }
#ifndef SQLITE_OMIT_VIEW
    if( pTab && pTab->pSelect ){
      pTab = 0;
      sqlite3ErrorMsg(pParse, "cannot open view: %s", zTable);
    }
#endif
    if( !pTab ){
      if( pParse->zErrMsg ){
        sqlite3DbFree(db, zErr);
        zErr = pParse->zErrMsg;
        pParse->zErrMsg = 0;
      }
      rc = SQLITE_ERROR;
      sqlite3BtreeLeaveAll(db);
      goto blob_open_out;
    }

    
    for(iCol=0; iCol<pTab->nCol; iCol++) {
      if( sqlite3StrICmp(pTab->aCol[iCol].zName, zColumn)==0 ){
        break;
      }
    }
    if( iCol==pTab->nCol ){
      sqlite3DbFree(db, zErr);
      zErr = sqlite3MPrintf(db, "no such column: \"%s\"", zColumn);
      rc = SQLITE_ERROR;
      sqlite3BtreeLeaveAll(db);
      goto blob_open_out;
    }

    if( flags ){
      const char *zFault = 0;
      Index *pIdx;
#ifndef SQLITE_OMIT_FOREIGN_KEY
      if( db->flags&SQLITE_ForeignKeys ){
        FKey *pFKey;
        for(pFKey=pTab->pFKey; pFKey; pFKey=pFKey->pNextFrom){
          int j;
          for(j=0; j<pFKey->nCol; j++){
            if( pFKey->aCol[j].iFrom==iCol ){
              zFault = "foreign key";
            }
          }
        }
      }
#endif
      for(pIdx=pTab->pIndex; pIdx; pIdx=pIdx->pNext){
        int j;
        for(j=0; j<pIdx->nColumn; j++){
          if( pIdx->aiColumn[j]==iCol ){
            zFault = "indexed";
          }
        }
      }
      if( zFault ){
        sqlite3DbFree(db, zErr);
        zErr = sqlite3MPrintf(db, "cannot open %s column for writing", zFault);
        rc = SQLITE_ERROR;
        sqlite3BtreeLeaveAll(db);
        goto blob_open_out;
      }
    }

    pBlob->pStmt = (sqlite3_stmt *)sqlite3VdbeCreate(db);
    assert( pBlob->pStmt || db->mallocFailed );
    if( pBlob->pStmt ){
      Vdbe *v = (Vdbe *)pBlob->pStmt;
      int iDb = sqlite3SchemaToIndex(db, pTab->pSchema);

      sqlite3VdbeAddOpList(v, sizeof(openBlob)/sizeof(VdbeOpList), openBlob);


      
      sqlite3VdbeChangeP1(v, 0, iDb);
      sqlite3VdbeChangeP2(v, 0, flags);

      
      sqlite3VdbeChangeP1(v, 1, iDb);
      sqlite3VdbeChangeP2(v, 1, pTab->pSchema->schema_cookie);
      sqlite3VdbeChangeP3(v, 1, pTab->pSchema->iGeneration);

      
      sqlite3VdbeUsesBtree(v, iDb); 

      
#ifdef SQLITE_OMIT_SHARED_CACHE
      sqlite3VdbeChangeToNoop(v, 2, 1);
#else
      sqlite3VdbeChangeP1(v, 2, iDb);
      sqlite3VdbeChangeP2(v, 2, pTab->tnum);
      sqlite3VdbeChangeP3(v, 2, flags);
      sqlite3VdbeChangeP4(v, 2, pTab->zName, P4_TRANSIENT);
#endif

      sqlite3VdbeChangeToNoop(v, 4 - flags, 1);
      sqlite3VdbeChangeP2(v, 3 + flags, pTab->tnum);
      sqlite3VdbeChangeP3(v, 3 + flags, iDb);

      sqlite3VdbeChangeP4(v, 3+flags, SQLITE_INT_TO_PTR(pTab->nCol+1),P4_INT32);
      sqlite3VdbeChangeP2(v, 7, pTab->nCol);
      if( !db->mallocFailed ){
        sqlite3VdbeMakeReady(v, 1, 1, 1, 0, 0, 0);
      }
    }
   
    pBlob->flags = flags;
    pBlob->iCol = iCol;
    pBlob->db = db;
    sqlite3BtreeLeaveAll(db);
    if( db->mallocFailed ){
      goto blob_open_out;
    }
    sqlite3_bind_int64(pBlob->pStmt, 1, iRow);
    rc = blobSeekToRow(pBlob, iRow, &zErr);
  } while( (++nAttempt)<5 && rc==SQLITE_SCHEMA );

blob_open_out:
  if( rc==SQLITE_OK && db->mallocFailed==0 ){
    *ppBlob = (sqlite3_blob *)pBlob;
  }else{
    if( pBlob && pBlob->pStmt ) sqlite3VdbeFinalize((Vdbe *)pBlob->pStmt);
    sqlite3DbFree(db, pBlob);
  }
  sqlite3Error(db, rc, (zErr ? "%s" : 0), zErr);
  sqlite3DbFree(db, zErr);
  sqlite3StackFree(db, pParse);
  rc = sqlite3ApiExit(db, rc);
  sqlite3_mutex_leave(db->mutex);
  return rc;
}

int sqlite3_blob_close(sqlite3_blob *pBlob){
  Incrblob *p = (Incrblob *)pBlob;
  int rc;
  sqlite3 *db;

  if( p ){
    db = p->db;
    sqlite3_mutex_enter(db->mutex);
    rc = sqlite3_finalize(p->pStmt);
    sqlite3DbFree(db, p);
    sqlite3_mutex_leave(db->mutex);
  }else{
    rc = SQLITE_OK;
  }
  return rc;
}

static int blobReadWrite(
  sqlite3_blob *pBlob, 
  void *z, 
  int n, 
  int iOffset, 
  int (*xCall)(BtCursor*, u32, u32, void*)
){
  int rc;
  Incrblob *p = (Incrblob *)pBlob;
  Vdbe *v;
  sqlite3 *db;

  if( p==0 ) return SQLITE_MISUSE_BKPT;
  db = p->db;
  sqlite3_mutex_enter(db->mutex);
  v = (Vdbe*)p->pStmt;

  if( n<0 || iOffset<0 || (iOffset+n)>p->nByte ){
    
    rc = SQLITE_ERROR;
    sqlite3Error(db, SQLITE_ERROR, 0);
  }else if( v==0 ){
    rc = SQLITE_ABORT;
  }else{
    assert( db == v->db );
    sqlite3BtreeEnterCursor(p->pCsr);
    rc = xCall(p->pCsr, iOffset+p->iOffset, n, z);
    sqlite3BtreeLeaveCursor(p->pCsr);
    if( rc==SQLITE_ABORT ){
      sqlite3VdbeFinalize(v);
      p->pStmt = 0;
    }else{
      db->errCode = rc;
      v->rc = rc;
    }
  }
  rc = sqlite3ApiExit(db, rc);
  sqlite3_mutex_leave(db->mutex);
  return rc;
}

int sqlite3_blob_read(sqlite3_blob *pBlob, void *z, int n, int iOffset){
  return blobReadWrite(pBlob, z, n, iOffset, sqlite3BtreeData);
}

int sqlite3_blob_write(sqlite3_blob *pBlob, const void *z, int n, int iOffset){
  return blobReadWrite(pBlob, (void *)z, n, iOffset, sqlite3BtreePutData);
}

int sqlite3_blob_bytes(sqlite3_blob *pBlob){
  Incrblob *p = (Incrblob *)pBlob;
  return (p && p->pStmt) ? p->nByte : 0;
}

int sqlite3_blob_reopen(sqlite3_blob *pBlob, sqlite3_int64 iRow){
  int rc;
  Incrblob *p = (Incrblob *)pBlob;
  sqlite3 *db;

  if( p==0 ) return SQLITE_MISUSE_BKPT;
  db = p->db;
  sqlite3_mutex_enter(db->mutex);

  if( p->pStmt==0 ){
    rc = SQLITE_ABORT;
  }else{
    char *zErr;
    rc = blobSeekToRow(p, iRow, &zErr);
    if( rc!=SQLITE_OK ){
      sqlite3Error(db, rc, (zErr ? "%s" : 0), zErr);
      sqlite3DbFree(db, zErr);
    }
    assert( rc!=SQLITE_SCHEMA );
  }

  rc = sqlite3ApiExit(db, rc);
  assert( rc==SQLITE_OK || p->pStmt==0 );
  sqlite3_mutex_leave(db->mutex);
  return rc;
}

#endif 

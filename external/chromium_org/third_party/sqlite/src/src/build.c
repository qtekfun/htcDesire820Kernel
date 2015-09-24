/*
** 2001 September 15
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This file contains C code routines that are called by the SQLite parser
** when syntax rules are reduced.  The routines in this file handle the
** following kinds of SQL syntax:
**
**     CREATE TABLE
**     DROP TABLE
**     CREATE INDEX
**     DROP INDEX
**     creating ID lists
**     BEGIN TRANSACTION
**     COMMIT
**     ROLLBACK
*/
#include "sqliteInt.h"

#include "pager.h"
#include "btree.h"

void sqlite3BeginParse(Parse *pParse, int explainFlag){
  pParse->explain = (u8)explainFlag;
  pParse->nVar = 0;
}

#ifndef SQLITE_OMIT_SHARED_CACHE
struct TableLock {
  int iDb;             
  int iTab;            
  u8 isWriteLock;      
  const char *zName;   
};

void sqlite3TableLock(
  Parse *pParse,     
  int iDb,           
  int iTab,          
  u8 isWriteLock,    
  const char *zName  
){
  Parse *pToplevel = sqlite3ParseToplevel(pParse);
  int i;
  int nBytes;
  TableLock *p;
  assert( iDb>=0 );

  for(i=0; i<pToplevel->nTableLock; i++){
    p = &pToplevel->aTableLock[i];
    if( p->iDb==iDb && p->iTab==iTab ){
      p->isWriteLock = (p->isWriteLock || isWriteLock);
      return;
    }
  }

  nBytes = sizeof(TableLock) * (pToplevel->nTableLock+1);
  pToplevel->aTableLock =
      sqlite3DbReallocOrFree(pToplevel->db, pToplevel->aTableLock, nBytes);
  if( pToplevel->aTableLock ){
    p = &pToplevel->aTableLock[pToplevel->nTableLock++];
    p->iDb = iDb;
    p->iTab = iTab;
    p->isWriteLock = isWriteLock;
    p->zName = zName;
  }else{
    pToplevel->nTableLock = 0;
    pToplevel->db->mallocFailed = 1;
  }
}

static void codeTableLocks(Parse *pParse){
  int i;
  Vdbe *pVdbe; 

  pVdbe = sqlite3GetVdbe(pParse);
  assert( pVdbe!=0 ); 

  for(i=0; i<pParse->nTableLock; i++){
    TableLock *p = &pParse->aTableLock[i];
    int p1 = p->iDb;
    sqlite3VdbeAddOp4(pVdbe, OP_TableLock, p1, p->iTab, p->isWriteLock,
                      p->zName, P4_STATIC);
  }
}
#else
  #define codeTableLocks(x)
#endif

void sqlite3FinishCoding(Parse *pParse){
  sqlite3 *db;
  Vdbe *v;

  db = pParse->db;
  if( db->mallocFailed ) return;
  if( pParse->nested ) return;
  if( pParse->nErr ) return;

  v = sqlite3GetVdbe(pParse);
  assert( !pParse->isMultiWrite 
       || sqlite3VdbeAssertMayAbort(v, pParse->mayAbort));
  if( v ){
    sqlite3VdbeAddOp0(v, OP_Halt);

    if( pParse->cookieGoto>0 ){
      yDbMask mask;
      int iDb;
      sqlite3VdbeJumpHere(v, pParse->cookieGoto-1);
      for(iDb=0, mask=1; iDb<db->nDb; mask<<=1, iDb++){
        if( (mask & pParse->cookieMask)==0 ) continue;
        sqlite3VdbeUsesBtree(v, iDb);
        sqlite3VdbeAddOp2(v,OP_Transaction, iDb, (mask & pParse->writeMask)!=0);
        if( db->init.busy==0 ){
          assert( sqlite3SchemaMutexHeld(db, iDb, 0) );
          sqlite3VdbeAddOp3(v, OP_VerifyCookie,
                            iDb, pParse->cookieValue[iDb],
                            db->aDb[iDb].pSchema->iGeneration);
        }
      }
#ifndef SQLITE_OMIT_VIRTUALTABLE
      {
        int i;
        for(i=0; i<pParse->nVtabLock; i++){
          char *vtab = (char *)sqlite3GetVTable(db, pParse->apVtabLock[i]);
          sqlite3VdbeAddOp4(v, OP_VBegin, 0, 0, 0, vtab, P4_VTAB);
        }
        pParse->nVtabLock = 0;
      }
#endif

      codeTableLocks(pParse);

      sqlite3AutoincrementBegin(pParse);

      
      sqlite3VdbeAddOp2(v, OP_Goto, 0, pParse->cookieGoto);
    }
  }


  if( v && ALWAYS(pParse->nErr==0) && !db->mallocFailed ){
#ifdef SQLITE_DEBUG
    FILE *trace = (db->flags & SQLITE_VdbeTrace)!=0 ? stdout : 0;
    sqlite3VdbeTrace(v, trace);
#endif
    assert( pParse->iCacheLevel==0 );  
    if( pParse->pAinc!=0 && pParse->nTab==0 ) pParse->nTab = 1;
    sqlite3VdbeMakeReady(v, pParse->nVar, pParse->nMem,
                         pParse->nTab, pParse->nMaxArg, pParse->explain,
                         pParse->isMultiWrite && pParse->mayAbort);
    pParse->rc = SQLITE_DONE;
    pParse->colNamesSet = 0;
  }else{
    pParse->rc = SQLITE_ERROR;
  }
  pParse->nTab = 0;
  pParse->nMem = 0;
  pParse->nSet = 0;
  pParse->nVar = 0;
  pParse->cookieMask = 0;
  pParse->cookieGoto = 0;
}

void sqlite3NestedParse(Parse *pParse, const char *zFormat, ...){
  va_list ap;
  char *zSql;
  char *zErrMsg = 0;
  sqlite3 *db = pParse->db;
# define SAVE_SZ  (sizeof(Parse) - offsetof(Parse,nVar))
  char saveBuf[SAVE_SZ];

  if( pParse->nErr ) return;
  assert( pParse->nested<10 );  
  va_start(ap, zFormat);
  zSql = sqlite3VMPrintf(db, zFormat, ap);
  va_end(ap);
  if( zSql==0 ){
    return;   
  }
  pParse->nested++;
  memcpy(saveBuf, &pParse->nVar, SAVE_SZ);
  memset(&pParse->nVar, 0, SAVE_SZ);
  sqlite3RunParser(pParse, zSql, &zErrMsg);
  sqlite3DbFree(db, zErrMsg);
  sqlite3DbFree(db, zSql);
  memcpy(&pParse->nVar, saveBuf, SAVE_SZ);
  pParse->nested--;
}

Table *sqlite3FindTable(sqlite3 *db, const char *zName, const char *zDatabase){
  Table *p = 0;
  int i;
  int nName;
  assert( zName!=0 );
  nName = sqlite3Strlen30(zName);
  
  assert( zDatabase!=0 || sqlite3BtreeHoldsAllMutexes(db) );
  for(i=OMIT_TEMPDB; i<db->nDb; i++){
    int j = (i<2) ? i^1 : i;   
    if( zDatabase!=0 && sqlite3StrICmp(zDatabase, db->aDb[j].zName) ) continue;
    assert( sqlite3SchemaMutexHeld(db, j, 0) );
    p = sqlite3HashFind(&db->aDb[j].pSchema->tblHash, zName, nName);
    if( p ) break;
  }
  return p;
}

Table *sqlite3LocateTable(
  Parse *pParse,         
  int isView,            
  const char *zName,     
  const char *zDbase     
){
  Table *p;

  if( SQLITE_OK!=sqlite3ReadSchema(pParse) ){
    return 0;
  }

  p = sqlite3FindTable(pParse->db, zName, zDbase);
  if( p==0 ){
    const char *zMsg = isView ? "no such view" : "no such table";
    if( zDbase ){
      sqlite3ErrorMsg(pParse, "%s: %s.%s", zMsg, zDbase, zName);
    }else{
      sqlite3ErrorMsg(pParse, "%s: %s", zMsg, zName);
    }
    pParse->checkSchema = 1;
  }
  return p;
}

Index *sqlite3FindIndex(sqlite3 *db, const char *zName, const char *zDb){
  Index *p = 0;
  int i;
  int nName = sqlite3Strlen30(zName);
  
  assert( zDb!=0 || sqlite3BtreeHoldsAllMutexes(db) );
  for(i=OMIT_TEMPDB; i<db->nDb; i++){
    int j = (i<2) ? i^1 : i;  
    Schema *pSchema = db->aDb[j].pSchema;
    assert( pSchema );
    if( zDb && sqlite3StrICmp(zDb, db->aDb[j].zName) ) continue;
    assert( sqlite3SchemaMutexHeld(db, j, 0) );
    p = sqlite3HashFind(&pSchema->idxHash, zName, nName);
    if( p ) break;
  }
  return p;
}

static void freeIndex(sqlite3 *db, Index *p){
#ifndef SQLITE_OMIT_ANALYZE
  sqlite3DeleteIndexSamples(db, p);
#endif
  sqlite3DbFree(db, p->zColAff);
  sqlite3DbFree(db, p);
}

void sqlite3UnlinkAndDeleteIndex(sqlite3 *db, int iDb, const char *zIdxName){
  Index *pIndex;
  int len;
  Hash *pHash;

  assert( sqlite3SchemaMutexHeld(db, iDb, 0) );
  pHash = &db->aDb[iDb].pSchema->idxHash;
  len = sqlite3Strlen30(zIdxName);
  pIndex = sqlite3HashInsert(pHash, zIdxName, len, 0);
  if( ALWAYS(pIndex) ){
    if( pIndex->pTable->pIndex==pIndex ){
      pIndex->pTable->pIndex = pIndex->pNext;
    }else{
      Index *p;
      p = pIndex->pTable->pIndex;
      while( ALWAYS(p) && p->pNext!=pIndex ){ p = p->pNext; }
      if( ALWAYS(p && p->pNext==pIndex) ){
        p->pNext = pIndex->pNext;
      }
    }
    freeIndex(db, pIndex);
  }
  db->flags |= SQLITE_InternChanges;
}

void sqlite3ResetInternalSchema(sqlite3 *db, int iDb){
  int i, j;
  assert( iDb<db->nDb );

  if( iDb>=0 ){
    
    Db *pDb = &db->aDb[iDb];
    assert( sqlite3SchemaMutexHeld(db, iDb, 0) );
    assert( pDb->pSchema!=0 );
    sqlite3SchemaClear(pDb->pSchema);

    if( iDb!=1 ){
      pDb = &db->aDb[1];
      assert( pDb->pSchema!=0 );
      sqlite3SchemaClear(pDb->pSchema);
    }
    return;
  }
  assert( iDb<0 );
  sqlite3BtreeEnterAll(db);
  for(i=0; i<db->nDb; i++){
    Db *pDb = &db->aDb[i];
    if( pDb->pSchema ){
      sqlite3SchemaClear(pDb->pSchema);
    }
  }
  db->flags &= ~SQLITE_InternChanges;
  sqlite3VtabUnlockList(db);
  sqlite3BtreeLeaveAll(db);

  for(i=j=2; i<db->nDb; i++){
    struct Db *pDb = &db->aDb[i];
    if( pDb->pBt==0 ){
      sqlite3DbFree(db, pDb->zName);
      pDb->zName = 0;
      continue;
    }
    if( j<i ){
      db->aDb[j] = db->aDb[i];
    }
    j++;
  }
  memset(&db->aDb[j], 0, (db->nDb-j)*sizeof(db->aDb[j]));
  db->nDb = j;
  if( db->nDb<=2 && db->aDb!=db->aDbStatic ){
    memcpy(db->aDbStatic, db->aDb, 2*sizeof(db->aDb[0]));
    sqlite3DbFree(db, db->aDb);
    db->aDb = db->aDbStatic;
  }
}

void sqlite3CommitInternalChanges(sqlite3 *db){
  db->flags &= ~SQLITE_InternChanges;
}

static void sqliteDeleteColumnNames(sqlite3 *db, Table *pTable){
  int i;
  Column *pCol;
  assert( pTable!=0 );
  if( (pCol = pTable->aCol)!=0 ){
    for(i=0; i<pTable->nCol; i++, pCol++){
      sqlite3DbFree(db, pCol->zName);
      sqlite3ExprDelete(db, pCol->pDflt);
      sqlite3DbFree(db, pCol->zDflt);
      sqlite3DbFree(db, pCol->zType);
      sqlite3DbFree(db, pCol->zColl);
    }
    sqlite3DbFree(db, pTable->aCol);
  }
}

void sqlite3DeleteTable(sqlite3 *db, Table *pTable){
  Index *pIndex, *pNext;

  assert( !pTable || pTable->nRef>0 );

  
  if( !pTable ) return;
  if( ((!db || db->pnBytesFreed==0) && (--pTable->nRef)>0) ) return;

  
  for(pIndex = pTable->pIndex; pIndex; pIndex=pNext){
    pNext = pIndex->pNext;
    assert( pIndex->pSchema==pTable->pSchema );
    if( !db || db->pnBytesFreed==0 ){
      char *zName = pIndex->zName; 
      TESTONLY ( Index *pOld = ) sqlite3HashInsert(
	  &pIndex->pSchema->idxHash, zName, sqlite3Strlen30(zName), 0
      );
      assert( db==0 || sqlite3SchemaMutexHeld(db, 0, pIndex->pSchema) );
      assert( pOld==pIndex || pOld==0 );
    }
    freeIndex(db, pIndex);
  }

  
  sqlite3FkDelete(db, pTable);

  sqliteDeleteColumnNames(db, pTable);
  sqlite3DbFree(db, pTable->zName);
  sqlite3DbFree(db, pTable->zColAff);
  sqlite3SelectDelete(db, pTable->pSelect);
#ifndef SQLITE_OMIT_CHECK
  sqlite3ExprDelete(db, pTable->pCheck);
#endif
#ifndef SQLITE_OMIT_VIRTUALTABLE
  sqlite3VtabClear(db, pTable);
#endif
  sqlite3DbFree(db, pTable);
}

void sqlite3UnlinkAndDeleteTable(sqlite3 *db, int iDb, const char *zTabName){
  Table *p;
  Db *pDb;

  assert( db!=0 );
  assert( iDb>=0 && iDb<db->nDb );
  assert( zTabName );
  assert( sqlite3SchemaMutexHeld(db, iDb, 0) );
  testcase( zTabName[0]==0 );  
  pDb = &db->aDb[iDb];
  p = sqlite3HashInsert(&pDb->pSchema->tblHash, zTabName,
                        sqlite3Strlen30(zTabName),0);
  sqlite3DeleteTable(db, p);
  db->flags |= SQLITE_InternChanges;
}

char *sqlite3NameFromToken(sqlite3 *db, Token *pName){
  char *zName;
  if( pName ){
    zName = sqlite3DbStrNDup(db, (char*)pName->z, pName->n);
    sqlite3Dequote(zName);
  }else{
    zName = 0;
  }
  return zName;
}

void sqlite3OpenMasterTable(Parse *p, int iDb){
  Vdbe *v = sqlite3GetVdbe(p);
  sqlite3TableLock(p, iDb, MASTER_ROOT, 1, SCHEMA_TABLE(iDb));
  sqlite3VdbeAddOp3(v, OP_OpenWrite, 0, MASTER_ROOT, iDb);
  sqlite3VdbeChangeP4(v, -1, (char *)5, P4_INT32);  
  if( p->nTab==0 ){
    p->nTab = 1;
  }
}

int sqlite3FindDbName(sqlite3 *db, const char *zName){
  int i = -1;         
  if( zName ){
    Db *pDb;
    int n = sqlite3Strlen30(zName);
    for(i=(db->nDb-1), pDb=&db->aDb[i]; i>=0; i--, pDb--){
      if( (!OMIT_TEMPDB || i!=1 ) && n==sqlite3Strlen30(pDb->zName) && 
          0==sqlite3StrICmp(pDb->zName, zName) ){
        break;
      }
    }
  }
  return i;
}

int sqlite3FindDb(sqlite3 *db, Token *pName){
  int i;                               
  char *zName;                         
  zName = sqlite3NameFromToken(db, pName);
  i = sqlite3FindDbName(db, zName);
  sqlite3DbFree(db, zName);
  return i;
}

int sqlite3TwoPartName(
  Parse *pParse,      
  Token *pName1,      
  Token *pName2,      
  Token **pUnqual     
){
  int iDb;                    
  sqlite3 *db = pParse->db;

  if( ALWAYS(pName2!=0) && pName2->n>0 ){
    if( db->init.busy ) {
      sqlite3ErrorMsg(pParse, "corrupt database");
      pParse->nErr++;
      return -1;
    }
    *pUnqual = pName2;
    iDb = sqlite3FindDb(db, pName1);
    if( iDb<0 ){
      sqlite3ErrorMsg(pParse, "unknown database %T", pName1);
      pParse->nErr++;
      return -1;
    }
  }else{
    assert( db->init.iDb==0 || db->init.busy );
    iDb = db->init.iDb;
    *pUnqual = pName1;
  }
  return iDb;
}

int sqlite3CheckObjectName(Parse *pParse, const char *zName){
  if( !pParse->db->init.busy && pParse->nested==0 
          && (pParse->db->flags & SQLITE_WriteSchema)==0
          && 0==sqlite3StrNICmp(zName, "sqlite_", 7) ){
    sqlite3ErrorMsg(pParse, "object name reserved for internal use: %s", zName);
    return SQLITE_ERROR;
  }
  return SQLITE_OK;
}

void sqlite3StartTable(
  Parse *pParse,   
  Token *pName1,   
  Token *pName2,   
  int isTemp,      
  int isView,      
  int isVirtual,   
  int noErr        
){
  Table *pTable;
  char *zName = 0; 
  sqlite3 *db = pParse->db;
  Vdbe *v;
  int iDb;         
  Token *pName;    

  iDb = sqlite3TwoPartName(pParse, pName1, pName2, &pName);
  if( iDb<0 ) return;
  if( !OMIT_TEMPDB && isTemp && pName2->n>0 && iDb!=1 ){
    sqlite3ErrorMsg(pParse, "temporary table name must be unqualified");
    return;
  }
  if( !OMIT_TEMPDB && isTemp ) iDb = 1;

  pParse->sNameToken = *pName;
  zName = sqlite3NameFromToken(db, pName);
  if( zName==0 ) return;
  if( SQLITE_OK!=sqlite3CheckObjectName(pParse, zName) ){
    goto begin_table_error;
  }
  if( db->init.iDb==1 ) isTemp = 1;
#ifndef SQLITE_OMIT_AUTHORIZATION
  assert( (isTemp & 1)==isTemp );
  {
    int code;
    char *zDb = db->aDb[iDb].zName;
    if( sqlite3AuthCheck(pParse, SQLITE_INSERT, SCHEMA_TABLE(isTemp), 0, zDb) ){
      goto begin_table_error;
    }
    if( isView ){
      if( !OMIT_TEMPDB && isTemp ){
        code = SQLITE_CREATE_TEMP_VIEW;
      }else{
        code = SQLITE_CREATE_VIEW;
      }
    }else{
      if( !OMIT_TEMPDB && isTemp ){
        code = SQLITE_CREATE_TEMP_TABLE;
      }else{
        code = SQLITE_CREATE_TABLE;
      }
    }
    if( !isVirtual && sqlite3AuthCheck(pParse, code, zName, 0, zDb) ){
      goto begin_table_error;
    }
  }
#endif

  if( !IN_DECLARE_VTAB ){
    char *zDb = db->aDb[iDb].zName;
    if( SQLITE_OK!=sqlite3ReadSchema(pParse) ){
      goto begin_table_error;
    }
    pTable = sqlite3FindTable(db, zName, zDb);
    if( pTable ){
      if( !noErr ){
        sqlite3ErrorMsg(pParse, "table %T already exists", pName);
      }else{
        assert( !db->init.busy );
        sqlite3CodeVerifySchema(pParse, iDb);
      }
      goto begin_table_error;
    }
    if( sqlite3FindIndex(db, zName, zDb)!=0 ){
      sqlite3ErrorMsg(pParse, "there is already an index named %s", zName);
      goto begin_table_error;
    }
  }

  pTable = sqlite3DbMallocZero(db, sizeof(Table));
  if( pTable==0 ){
    db->mallocFailed = 1;
    pParse->rc = SQLITE_NOMEM;
    pParse->nErr++;
    goto begin_table_error;
  }
  pTable->zName = zName;
  pTable->iPKey = -1;
  pTable->pSchema = db->aDb[iDb].pSchema;
  pTable->nRef = 1;
  pTable->nRowEst = 1000000;
  assert( pParse->pNewTable==0 );
  pParse->pNewTable = pTable;

#ifndef SQLITE_OMIT_AUTOINCREMENT
  if( !pParse->nested && strcmp(zName, "sqlite_sequence")==0 ){
    assert( sqlite3SchemaMutexHeld(db, iDb, 0) );
    pTable->pSchema->pSeqTab = pTable;
  }
#endif

  if( !db->init.busy && (v = sqlite3GetVdbe(pParse))!=0 ){
    int j1;
    int fileFormat;
    int reg1, reg2, reg3;
    sqlite3BeginWriteOperation(pParse, 0, iDb);

#ifndef SQLITE_OMIT_VIRTUALTABLE
    if( isVirtual ){
      sqlite3VdbeAddOp0(v, OP_VBegin);
    }
#endif

    reg1 = pParse->regRowid = ++pParse->nMem;
    reg2 = pParse->regRoot = ++pParse->nMem;
    reg3 = ++pParse->nMem;
    sqlite3VdbeAddOp3(v, OP_ReadCookie, iDb, reg3, BTREE_FILE_FORMAT);
    sqlite3VdbeUsesBtree(v, iDb);
    j1 = sqlite3VdbeAddOp1(v, OP_If, reg3);
    fileFormat = (db->flags & SQLITE_LegacyFileFmt)!=0 ?
                  1 : SQLITE_MAX_FILE_FORMAT;
    sqlite3VdbeAddOp2(v, OP_Integer, fileFormat, reg3);
    sqlite3VdbeAddOp3(v, OP_SetCookie, iDb, BTREE_FILE_FORMAT, reg3);
    sqlite3VdbeAddOp2(v, OP_Integer, ENC(db), reg3);
    sqlite3VdbeAddOp3(v, OP_SetCookie, iDb, BTREE_TEXT_ENCODING, reg3);
    sqlite3VdbeJumpHere(v, j1);

#if !defined(SQLITE_OMIT_VIEW) || !defined(SQLITE_OMIT_VIRTUALTABLE)
    if( isView || isVirtual ){
      sqlite3VdbeAddOp2(v, OP_Integer, 0, reg2);
    }else
#endif
    {
      sqlite3VdbeAddOp2(v, OP_CreateTable, iDb, reg2);
    }
    sqlite3OpenMasterTable(pParse, iDb);
    sqlite3VdbeAddOp2(v, OP_NewRowid, 0, reg1);
    sqlite3VdbeAddOp2(v, OP_Null, 0, reg3);
    sqlite3VdbeAddOp3(v, OP_Insert, 0, reg3, reg1);
    sqlite3VdbeChangeP5(v, OPFLAG_APPEND);
    sqlite3VdbeAddOp0(v, OP_Close);
  }

  
  return;

  
begin_table_error:
  sqlite3DbFree(db, zName);
  return;
}

#define STRICMP(x, y) (\
sqlite3UpperToLower[*(unsigned char *)(x)]==   \
sqlite3UpperToLower[*(unsigned char *)(y)]     \
&& sqlite3StrICmp((x)+1,(y)+1)==0 )

void sqlite3AddColumn(Parse *pParse, Token *pName){
  Table *p;
  int i;
  char *z;
  Column *pCol;
  sqlite3 *db = pParse->db;
  if( (p = pParse->pNewTable)==0 ) return;
#if SQLITE_MAX_COLUMN
  if( p->nCol+1>db->aLimit[SQLITE_LIMIT_COLUMN] ){
    sqlite3ErrorMsg(pParse, "too many columns on %s", p->zName);
    return;
  }
#endif
  z = sqlite3NameFromToken(db, pName);
  if( z==0 ) return;
  for(i=0; i<p->nCol; i++){
    if( STRICMP(z, p->aCol[i].zName) ){
      sqlite3ErrorMsg(pParse, "duplicate column name: %s", z);
      sqlite3DbFree(db, z);
      return;
    }
  }
  if( (p->nCol & 0x7)==0 ){
    Column *aNew;
    aNew = sqlite3DbRealloc(db,p->aCol,(p->nCol+8)*sizeof(p->aCol[0]));
    if( aNew==0 ){
      sqlite3DbFree(db, z);
      return;
    }
    p->aCol = aNew;
  }
  pCol = &p->aCol[p->nCol];
  memset(pCol, 0, sizeof(p->aCol[0]));
  pCol->zName = z;
 
  pCol->affinity = SQLITE_AFF_NONE;
  p->nCol++;
}

void sqlite3AddNotNull(Parse *pParse, int onError){
  Table *p;
  p = pParse->pNewTable;
  if( p==0 || NEVER(p->nCol<1) ) return;
  p->aCol[p->nCol-1].notNull = (u8)onError;
}

char sqlite3AffinityType(const char *zIn){
  u32 h = 0;
  char aff = SQLITE_AFF_NUMERIC;

  if( zIn ) while( zIn[0] ){
    h = (h<<8) + sqlite3UpperToLower[(*zIn)&0xff];
    zIn++;
    if( h==(('c'<<24)+('h'<<16)+('a'<<8)+'r') ){             
      aff = SQLITE_AFF_TEXT; 
    }else if( h==(('c'<<24)+('l'<<16)+('o'<<8)+'b') ){       
      aff = SQLITE_AFF_TEXT;
    }else if( h==(('t'<<24)+('e'<<16)+('x'<<8)+'t') ){       
      aff = SQLITE_AFF_TEXT;
    }else if( h==(('b'<<24)+('l'<<16)+('o'<<8)+'b')          
        && (aff==SQLITE_AFF_NUMERIC || aff==SQLITE_AFF_REAL) ){
      aff = SQLITE_AFF_NONE;
#ifndef SQLITE_OMIT_FLOATING_POINT
    }else if( h==(('r'<<24)+('e'<<16)+('a'<<8)+'l')          
        && aff==SQLITE_AFF_NUMERIC ){
      aff = SQLITE_AFF_REAL;
    }else if( h==(('f'<<24)+('l'<<16)+('o'<<8)+'a')          
        && aff==SQLITE_AFF_NUMERIC ){
      aff = SQLITE_AFF_REAL;
    }else if( h==(('d'<<24)+('o'<<16)+('u'<<8)+'b')          
        && aff==SQLITE_AFF_NUMERIC ){
      aff = SQLITE_AFF_REAL;
#endif
    }else if( (h&0x00FFFFFF)==(('i'<<16)+('n'<<8)+'t') ){    
      aff = SQLITE_AFF_INTEGER;
      break;
    }
  }

  return aff;
}

 
void sqlite3AddColumnType(Parse *pParse, Token *pType){
  Table *p;
  Column *pCol;

  p = pParse->pNewTable;
  if( p==0 || NEVER(p->nCol<1) ) return;
  pCol = &p->aCol[p->nCol-1];
  assert( pCol->zType==0 );
  pCol->zType = sqlite3NameFromToken(pParse->db, pType);
  pCol->affinity = sqlite3AffinityType(pCol->zType);
}

void sqlite3AddDefaultValue(Parse *pParse, ExprSpan *pSpan){
  Table *p;
  Column *pCol;
  sqlite3 *db = pParse->db;
  p = pParse->pNewTable;
  if( p!=0 ){
    pCol = &(p->aCol[p->nCol-1]);
    if( !sqlite3ExprIsConstantOrFunction(pSpan->pExpr) ){
      sqlite3ErrorMsg(pParse, "default value of column [%s] is not constant",
          pCol->zName);
    }else{
      sqlite3ExprDelete(db, pCol->pDflt);
      pCol->pDflt = sqlite3ExprDup(db, pSpan->pExpr, EXPRDUP_REDUCE);
      sqlite3DbFree(db, pCol->zDflt);
      pCol->zDflt = sqlite3DbStrNDup(db, (char*)pSpan->zStart,
                                     (int)(pSpan->zEnd - pSpan->zStart));
    }
  }
  sqlite3ExprDelete(db, pSpan->pExpr);
}

void sqlite3AddPrimaryKey(
  Parse *pParse,    
  ExprList *pList,  
  int onError,      
  int autoInc,      
  int sortOrder     
){
  Table *pTab = pParse->pNewTable;
  char *zType = 0;
  int iCol = -1, i;
  if( pTab==0 || IN_DECLARE_VTAB ) goto primary_key_exit;
  if( pTab->tabFlags & TF_HasPrimaryKey ){
    sqlite3ErrorMsg(pParse, 
      "table \"%s\" has more than one primary key", pTab->zName);
    goto primary_key_exit;
  }
  pTab->tabFlags |= TF_HasPrimaryKey;
  if( pList==0 ){
    iCol = pTab->nCol - 1;
    pTab->aCol[iCol].isPrimKey = 1;
  }else{
    for(i=0; i<pList->nExpr; i++){
      for(iCol=0; iCol<pTab->nCol; iCol++){
        if( sqlite3StrICmp(pList->a[i].zName, pTab->aCol[iCol].zName)==0 ){
          break;
        }
      }
      if( iCol<pTab->nCol ){
        pTab->aCol[iCol].isPrimKey = 1;
      }
    }
    if( pList->nExpr>1 ) iCol = -1;
  }
  if( iCol>=0 && iCol<pTab->nCol ){
    zType = pTab->aCol[iCol].zType;
  }
  if( zType && sqlite3StrICmp(zType, "INTEGER")==0
        && sortOrder==SQLITE_SO_ASC ){
    pTab->iPKey = iCol;
    pTab->keyConf = (u8)onError;
    assert( autoInc==0 || autoInc==1 );
    pTab->tabFlags |= autoInc*TF_Autoincrement;
  }else if( autoInc ){
#ifndef SQLITE_OMIT_AUTOINCREMENT
    sqlite3ErrorMsg(pParse, "AUTOINCREMENT is only allowed on an "
       "INTEGER PRIMARY KEY");
#endif
  }else{
    Index *p;
    p = sqlite3CreateIndex(pParse, 0, 0, 0, pList, onError, 0, 0, sortOrder, 0);
    if( p ){
      p->autoIndex = 2;
    }
    pList = 0;
  }

primary_key_exit:
  sqlite3ExprListDelete(pParse->db, pList);
  return;
}

void sqlite3AddCheckConstraint(
  Parse *pParse,    
  Expr *pCheckExpr  
){
  sqlite3 *db = pParse->db;
#ifndef SQLITE_OMIT_CHECK
  Table *pTab = pParse->pNewTable;
  if( pTab && !IN_DECLARE_VTAB ){
    pTab->pCheck = sqlite3ExprAnd(db, pTab->pCheck, pCheckExpr);
  }else
#endif
  {
    sqlite3ExprDelete(db, pCheckExpr);
  }
}

void sqlite3AddCollateType(Parse *pParse, Token *pToken){
  Table *p;
  int i;
  char *zColl;              
  sqlite3 *db;

  if( (p = pParse->pNewTable)==0 ) return;
  i = p->nCol-1;
  db = pParse->db;
  zColl = sqlite3NameFromToken(db, pToken);
  if( !zColl ) return;

  if( sqlite3LocateCollSeq(pParse, zColl) ){
    Index *pIdx;
    p->aCol[i].zColl = zColl;
  
    for(pIdx=p->pIndex; pIdx; pIdx=pIdx->pNext){
      assert( pIdx->nColumn==1 );
      if( pIdx->aiColumn[0]==i ){
        pIdx->azColl[0] = p->aCol[i].zColl;
      }
    }
  }else{
    sqlite3DbFree(db, zColl);
  }
}

/*
** This function returns the collation sequence for database native text
** encoding identified by the string zName, length nName.
**
** If the requested collation sequence is not available, or not available
** in the database native encoding, the collation factory is invoked to
** request it. If the collation factory does not supply such a sequence,
** and the sequence is available in another text encoding, then that is
** returned instead.
**
** If no versions of the requested collations sequence are available, or
** another error occurs, NULL is returned and an error message written into
** pParse.
**
** This routine is a wrapper around sqlite3FindCollSeq().  This routine
** invokes the collation factory if the named collation cannot be found
** and generates an error message.
**
** See also: sqlite3FindCollSeq(), sqlite3GetCollSeq()
*/
CollSeq *sqlite3LocateCollSeq(Parse *pParse, const char *zName){
  sqlite3 *db = pParse->db;
  u8 enc = ENC(db);
  u8 initbusy = db->init.busy;
  CollSeq *pColl;

  pColl = sqlite3FindCollSeq(db, enc, zName, initbusy);
  if( !initbusy && (!pColl || !pColl->xCmp) ){
    pColl = sqlite3GetCollSeq(db, enc, pColl, zName);
    if( !pColl ){
      sqlite3ErrorMsg(pParse, "no such collation sequence: %s", zName);
    }
  }

  return pColl;
}


void sqlite3ChangeCookie(Parse *pParse, int iDb){
  int r1 = sqlite3GetTempReg(pParse);
  sqlite3 *db = pParse->db;
  Vdbe *v = pParse->pVdbe;
  assert( sqlite3SchemaMutexHeld(db, iDb, 0) );
  sqlite3VdbeAddOp2(v, OP_Integer, db->aDb[iDb].pSchema->schema_cookie+1, r1);
  sqlite3VdbeAddOp3(v, OP_SetCookie, iDb, BTREE_SCHEMA_VERSION, r1);
  sqlite3ReleaseTempReg(pParse, r1);
}

static int identLength(const char *z){
  int n;
  for(n=0; *z; n++, z++){
    if( *z=='"' ){ n++; }
  }
  return n + 2;
}

/*
** The first parameter is a pointer to an output buffer. The second 
** parameter is a pointer to an integer that contains the offset at
** which to write into the output buffer. This function copies the
** nul-terminated string pointed to by the third parameter, zSignedIdent,
** to the specified offset in the buffer and updates *pIdx to refer
** to the first byte after the last byte written before returning.
** 
** If the string zSignedIdent consists entirely of alpha-numeric
** characters, does not begin with a digit and is not an SQL keyword,
** then it is copied to the output buffer exactly as it is. Otherwise,
** it is quoted using double-quotes.
*/
static void identPut(char *z, int *pIdx, char *zSignedIdent){
  unsigned char *zIdent = (unsigned char*)zSignedIdent;
  int i, j, needQuote;
  i = *pIdx;

  for(j=0; zIdent[j]; j++){
    if( !sqlite3Isalnum(zIdent[j]) && zIdent[j]!='_' ) break;
  }
  needQuote = sqlite3Isdigit(zIdent[0]) || sqlite3KeywordCode(zIdent, j)!=TK_ID;
  if( !needQuote ){
    needQuote = zIdent[j];
  }

  if( needQuote ) z[i++] = '"';
  for(j=0; zIdent[j]; j++){
    z[i++] = zIdent[j];
    if( zIdent[j]=='"' ) z[i++] = '"';
  }
  if( needQuote ) z[i++] = '"';
  z[i] = 0;
  *pIdx = i;
}

static char *createTableStmt(sqlite3 *db, Table *p){
  int i, k, n;
  char *zStmt;
  char *zSep, *zSep2, *zEnd;
  Column *pCol;
  n = 0;
  for(pCol = p->aCol, i=0; i<p->nCol; i++, pCol++){
    n += identLength(pCol->zName) + 5;
  }
  n += identLength(p->zName);
  if( n<50 ){ 
    zSep = "";
    zSep2 = ",";
    zEnd = ")";
  }else{
    zSep = "\n  ";
    zSep2 = ",\n  ";
    zEnd = "\n)";
  }
  n += 35 + 6*p->nCol;
  zStmt = sqlite3DbMallocRaw(0, n);
  if( zStmt==0 ){
    db->mallocFailed = 1;
    return 0;
  }
  sqlite3_snprintf(n, zStmt, "CREATE TABLE ");
  k = sqlite3Strlen30(zStmt);
  identPut(zStmt, &k, p->zName);
  zStmt[k++] = '(';
  for(pCol=p->aCol, i=0; i<p->nCol; i++, pCol++){
    static const char * const azType[] = {
         " TEXT",
         "",
         " NUM",
         " INT",
         " REAL"
    };
    int len;
    const char *zType;

    sqlite3_snprintf(n-k, &zStmt[k], zSep);
    k += sqlite3Strlen30(&zStmt[k]);
    zSep = zSep2;
    identPut(zStmt, &k, pCol->zName);
    assert( pCol->affinity-SQLITE_AFF_TEXT >= 0 );
    assert( pCol->affinity-SQLITE_AFF_TEXT < ArraySize(azType) );
    testcase( pCol->affinity==SQLITE_AFF_TEXT );
    testcase( pCol->affinity==SQLITE_AFF_NONE );
    testcase( pCol->affinity==SQLITE_AFF_NUMERIC );
    testcase( pCol->affinity==SQLITE_AFF_INTEGER );
    testcase( pCol->affinity==SQLITE_AFF_REAL );
    
    zType = azType[pCol->affinity - SQLITE_AFF_TEXT];
    len = sqlite3Strlen30(zType);
    assert( pCol->affinity==SQLITE_AFF_NONE 
            || pCol->affinity==sqlite3AffinityType(zType) );
    memcpy(&zStmt[k], zType, len);
    k += len;
    assert( k<=n );
  }
  sqlite3_snprintf(n-k, &zStmt[k], "%s", zEnd);
  return zStmt;
}

void sqlite3EndTable(
  Parse *pParse,          
  Token *pCons,           
  Token *pEnd,            
  Select *pSelect         
){
  Table *p;
  sqlite3 *db = pParse->db;
  int iDb;

  if( (pEnd==0 && pSelect==0) || db->mallocFailed ){
    return;
  }
  p = pParse->pNewTable;
  if( p==0 ) return;

  assert( !db->init.busy || !pSelect );

  iDb = sqlite3SchemaToIndex(db, p->pSchema);

#ifndef SQLITE_OMIT_CHECK
  if( p->pCheck ){
    SrcList sSrc;                   
    NameContext sNC;                

    memset(&sNC, 0, sizeof(sNC));
    memset(&sSrc, 0, sizeof(sSrc));
    sSrc.nSrc = 1;
    sSrc.a[0].zName = p->zName;
    sSrc.a[0].pTab = p;
    sSrc.a[0].iCursor = -1;
    sNC.pParse = pParse;
    sNC.pSrcList = &sSrc;
    sNC.isCheck = 1;
    if( sqlite3ResolveExprNames(&sNC, p->pCheck) ){
      return;
    }
  }
#endif 

  if( db->init.busy ){
    p->tnum = db->init.newTnum;
  }

  if( !db->init.busy ){
    int n;
    Vdbe *v;
    char *zType;    
    char *zType2;   
    char *zStmt;    

    v = sqlite3GetVdbe(pParse);
    if( NEVER(v==0) ) return;

    sqlite3VdbeAddOp1(v, OP_Close, 0);

    if( p->pSelect==0 ){
      
      zType = "table";
      zType2 = "TABLE";
#ifndef SQLITE_OMIT_VIEW
    }else{
      
      zType = "view";
      zType2 = "VIEW";
#endif
    }

    if( pSelect ){
      SelectDest dest;
      Table *pSelTab;

      assert(pParse->nTab==1);
      sqlite3VdbeAddOp3(v, OP_OpenWrite, 1, pParse->regRoot, iDb);
      sqlite3VdbeChangeP5(v, 1);
      pParse->nTab = 2;
      sqlite3SelectDestInit(&dest, SRT_Table, 1);
      sqlite3Select(pParse, pSelect, &dest);
      sqlite3VdbeAddOp1(v, OP_Close, 1);
      if( pParse->nErr==0 ){
        pSelTab = sqlite3ResultSetOfSelect(pParse, pSelect);
        if( pSelTab==0 ) return;
        assert( p->aCol==0 );
        p->nCol = pSelTab->nCol;
        p->aCol = pSelTab->aCol;
        pSelTab->nCol = 0;
        pSelTab->aCol = 0;
        sqlite3DeleteTable(db, pSelTab);
      }
    }

    
    if( pSelect ){
      zStmt = createTableStmt(db, p);
    }else{
      n = (int)(pEnd->z - pParse->sNameToken.z) + 1;
      zStmt = sqlite3MPrintf(db, 
          "CREATE %s %.*s", zType2, n, pParse->sNameToken.z
      );
    }

    sqlite3NestedParse(pParse,
      "UPDATE %Q.%s "
         "SET type='%s', name=%Q, tbl_name=%Q, rootpage=#%d, sql=%Q "
       "WHERE rowid=#%d",
      db->aDb[iDb].zName, SCHEMA_TABLE(iDb),
      zType,
      p->zName,
      p->zName,
      pParse->regRoot,
      zStmt,
      pParse->regRowid
    );
    sqlite3DbFree(db, zStmt);
    sqlite3ChangeCookie(pParse, iDb);

#ifndef SQLITE_OMIT_AUTOINCREMENT
    if( p->tabFlags & TF_Autoincrement ){
      Db *pDb = &db->aDb[iDb];
      assert( sqlite3SchemaMutexHeld(db, iDb, 0) );
      if( pDb->pSchema->pSeqTab==0 ){
        sqlite3NestedParse(pParse,
          "CREATE TABLE %Q.sqlite_sequence(name,seq)",
          pDb->zName
        );
      }
    }
#endif

    
    sqlite3VdbeAddOp4(v, OP_ParseSchema, iDb, 0, 0,
        sqlite3MPrintf(db, "tbl_name='%q'",p->zName), P4_DYNAMIC);
  }


  if( db->init.busy ){
    Table *pOld;
    Schema *pSchema = p->pSchema;
    assert( sqlite3SchemaMutexHeld(db, iDb, 0) );
    pOld = sqlite3HashInsert(&pSchema->tblHash, p->zName,
                             sqlite3Strlen30(p->zName),p);
    if( pOld ){
      assert( p==pOld );  
      db->mallocFailed = 1;
      return;
    }
    pParse->pNewTable = 0;
    db->nTable++;
    db->flags |= SQLITE_InternChanges;

#ifndef SQLITE_OMIT_ALTERTABLE
    if( !p->pSelect ){
      const char *zName = (const char *)pParse->sNameToken.z;
      int nName;
      assert( !pSelect && pCons && pEnd );
      if( pCons->z==0 ){
        pCons = pEnd;
      }
      nName = (int)((const char *)pCons->z - zName);
      p->addColOffset = 13 + sqlite3Utf8CharLen(zName, nName);
    }
#endif
  }
}

#ifndef SQLITE_OMIT_VIEW
void sqlite3CreateView(
  Parse *pParse,     
  Token *pBegin,     
  Token *pName1,     
  Token *pName2,     
  Select *pSelect,   
  int isTemp,        
  int noErr          
){
  Table *p;
  int n;
  const char *z;
  Token sEnd;
  DbFixer sFix;
  Token *pName;
  int iDb;
  sqlite3 *db = pParse->db;

  if( pParse->nVar>0 ){
    sqlite3ErrorMsg(pParse, "parameters are not allowed in views");
    sqlite3SelectDelete(db, pSelect);
    return;
  }
  sqlite3StartTable(pParse, pName1, pName2, isTemp, 1, 0, noErr);
  p = pParse->pNewTable;
  if( p==0 || pParse->nErr ){
    sqlite3SelectDelete(db, pSelect);
    return;
  }
  sqlite3TwoPartName(pParse, pName1, pName2, &pName);
  iDb = sqlite3SchemaToIndex(db, p->pSchema);
  if( sqlite3FixInit(&sFix, pParse, iDb, "view", pName)
    && sqlite3FixSelect(&sFix, pSelect)
  ){
    sqlite3SelectDelete(db, pSelect);
    return;
  }

  p->pSelect = sqlite3SelectDup(db, pSelect, EXPRDUP_REDUCE);
  sqlite3SelectDelete(db, pSelect);
  if( db->mallocFailed ){
    return;
  }
  if( !db->init.busy ){
    sqlite3ViewGetColumnNames(pParse, p);
  }

  sEnd = pParse->sLastToken;
  if( ALWAYS(sEnd.z[0]!=0) && sEnd.z[0]!=';' ){
    sEnd.z += sEnd.n;
  }
  sEnd.n = 0;
  n = (int)(sEnd.z - pBegin->z);
  z = pBegin->z;
  while( ALWAYS(n>0) && sqlite3Isspace(z[n-1]) ){ n--; }
  sEnd.z = &z[n-1];
  sEnd.n = 1;

  
  sqlite3EndTable(pParse, 0, &sEnd, 0);
  return;
}
#endif 

#if !defined(SQLITE_OMIT_VIEW) || !defined(SQLITE_OMIT_VIRTUALTABLE)
int sqlite3ViewGetColumnNames(Parse *pParse, Table *pTable){
  Table *pSelTab;   
  Select *pSel;     
  int nErr = 0;     
  int n;            
  sqlite3 *db = pParse->db;  
  int (*xAuth)(void*,int,const char*,const char*,const char*,const char*);

  assert( pTable );

#ifndef SQLITE_OMIT_VIRTUALTABLE
  if( sqlite3VtabCallConnect(pParse, pTable) ){
    return SQLITE_ERROR;
  }
  if( IsVirtual(pTable) ) return 0;
#endif

#ifndef SQLITE_OMIT_VIEW
  if( pTable->nCol>0 ) return 0;

  if( pTable->nCol<0 ){
    sqlite3ErrorMsg(pParse, "view %s is circularly defined", pTable->zName);
    return 1;
  }
  assert( pTable->nCol>=0 );

  assert( pTable->pSelect );
  pSel = sqlite3SelectDup(db, pTable->pSelect, 0);
  if( pSel ){
    u8 enableLookaside = db->lookaside.bEnabled;
    n = pParse->nTab;
    sqlite3SrcListAssignCursors(pParse, pSel->pSrc);
    pTable->nCol = -1;
    db->lookaside.bEnabled = 0;
#ifndef SQLITE_OMIT_AUTHORIZATION
    xAuth = db->xAuth;
    db->xAuth = 0;
    pSelTab = sqlite3ResultSetOfSelect(pParse, pSel);
    db->xAuth = xAuth;
#else
    pSelTab = sqlite3ResultSetOfSelect(pParse, pSel);
#endif
    db->lookaside.bEnabled = enableLookaside;
    pParse->nTab = n;
    if( pSelTab ){
      assert( pTable->aCol==0 );
      pTable->nCol = pSelTab->nCol;
      pTable->aCol = pSelTab->aCol;
      pSelTab->nCol = 0;
      pSelTab->aCol = 0;
      sqlite3DeleteTable(db, pSelTab);
      assert( sqlite3SchemaMutexHeld(db, 0, pTable->pSchema) );
      pTable->pSchema->flags |= DB_UnresetViews;
    }else{
      pTable->nCol = 0;
      nErr++;
    }
    sqlite3SelectDelete(db, pSel);
  } else {
    nErr++;
  }
#endif 
  return nErr;  
}
#endif 

#ifndef SQLITE_OMIT_VIEW
static void sqliteViewResetAll(sqlite3 *db, int idx){
  HashElem *i;
  assert( sqlite3SchemaMutexHeld(db, idx, 0) );
  if( !DbHasProperty(db, idx, DB_UnresetViews) ) return;
  for(i=sqliteHashFirst(&db->aDb[idx].pSchema->tblHash); i;i=sqliteHashNext(i)){
    Table *pTab = sqliteHashData(i);
    if( pTab->pSelect ){
      sqliteDeleteColumnNames(db, pTab);
      pTab->aCol = 0;
      pTab->nCol = 0;
    }
  }
  DbClearProperty(db, idx, DB_UnresetViews);
}
#else
# define sqliteViewResetAll(A,B)
#endif 

#ifndef SQLITE_OMIT_AUTOVACUUM
void sqlite3RootPageMoved(sqlite3 *db, int iDb, int iFrom, int iTo){
  HashElem *pElem;
  Hash *pHash;
  Db *pDb;

  assert( sqlite3SchemaMutexHeld(db, iDb, 0) );
  pDb = &db->aDb[iDb];
  pHash = &pDb->pSchema->tblHash;
  for(pElem=sqliteHashFirst(pHash); pElem; pElem=sqliteHashNext(pElem)){
    Table *pTab = sqliteHashData(pElem);
    if( pTab->tnum==iFrom ){
      pTab->tnum = iTo;
    }
  }
  pHash = &pDb->pSchema->idxHash;
  for(pElem=sqliteHashFirst(pHash); pElem; pElem=sqliteHashNext(pElem)){
    Index *pIdx = sqliteHashData(pElem);
    if( pIdx->tnum==iFrom ){
      pIdx->tnum = iTo;
    }
  }
}
#endif

 
static void destroyRootPage(Parse *pParse, int iTable, int iDb){
  Vdbe *v = sqlite3GetVdbe(pParse);
  int r1 = sqlite3GetTempReg(pParse);
  sqlite3VdbeAddOp3(v, OP_Destroy, iTable, r1, iDb);
  sqlite3MayAbort(pParse);
#ifndef SQLITE_OMIT_AUTOVACUUM
  sqlite3NestedParse(pParse, 
     "UPDATE %Q.%s SET rootpage=%d WHERE #%d AND rootpage=#%d",
     pParse->db->aDb[iDb].zName, SCHEMA_TABLE(iDb), iTable, r1, r1);
#endif
  sqlite3ReleaseTempReg(pParse, r1);
}

static void destroyTable(Parse *pParse, Table *pTab){
#ifdef SQLITE_OMIT_AUTOVACUUM
  Index *pIdx;
  int iDb = sqlite3SchemaToIndex(pParse->db, pTab->pSchema);
  destroyRootPage(pParse, pTab->tnum, iDb);
  for(pIdx=pTab->pIndex; pIdx; pIdx=pIdx->pNext){
    destroyRootPage(pParse, pIdx->tnum, iDb);
  }
#else
  int iTab = pTab->tnum;
  int iDestroyed = 0;

  while( 1 ){
    Index *pIdx;
    int iLargest = 0;

    if( iDestroyed==0 || iTab<iDestroyed ){
      iLargest = iTab;
    }
    for(pIdx=pTab->pIndex; pIdx; pIdx=pIdx->pNext){
      int iIdx = pIdx->tnum;
      assert( pIdx->pSchema==pTab->pSchema );
      if( (iDestroyed==0 || (iIdx<iDestroyed)) && iIdx>iLargest ){
        iLargest = iIdx;
      }
    }
    if( iLargest==0 ){
      return;
    }else{
      int iDb = sqlite3SchemaToIndex(pParse->db, pTab->pSchema);
      destroyRootPage(pParse, iLargest, iDb);
      iDestroyed = iLargest;
    }
  }
#endif
}

void sqlite3DropTable(Parse *pParse, SrcList *pName, int isView, int noErr){
  Table *pTab;
  Vdbe *v;
  sqlite3 *db = pParse->db;
  int iDb;

  if( db->mallocFailed ){
    goto exit_drop_table;
  }
  assert( pParse->nErr==0 );
  assert( pName->nSrc==1 );
  if( noErr ) db->suppressErr++;
  pTab = sqlite3LocateTable(pParse, isView, 
                            pName->a[0].zName, pName->a[0].zDatabase);
  if( noErr ) db->suppressErr--;

  if( pTab==0 ){
    if( noErr ) sqlite3CodeVerifyNamedSchema(pParse, pName->a[0].zDatabase);
    goto exit_drop_table;
  }
  iDb = sqlite3SchemaToIndex(db, pTab->pSchema);
  assert( iDb>=0 && iDb<db->nDb );

  if( IsVirtual(pTab) && sqlite3ViewGetColumnNames(pParse, pTab) ){
    goto exit_drop_table;
  }
#ifndef SQLITE_OMIT_AUTHORIZATION
  {
    int code;
    const char *zTab = SCHEMA_TABLE(iDb);
    const char *zDb = db->aDb[iDb].zName;
    const char *zArg2 = 0;
    if( sqlite3AuthCheck(pParse, SQLITE_DELETE, zTab, 0, zDb)){
      goto exit_drop_table;
    }
    if( isView ){
      if( !OMIT_TEMPDB && iDb==1 ){
        code = SQLITE_DROP_TEMP_VIEW;
      }else{
        code = SQLITE_DROP_VIEW;
      }
#ifndef SQLITE_OMIT_VIRTUALTABLE
    }else if( IsVirtual(pTab) ){
      code = SQLITE_DROP_VTABLE;
      zArg2 = sqlite3GetVTable(db, pTab)->pMod->zName;
#endif
    }else{
      if( !OMIT_TEMPDB && iDb==1 ){
        code = SQLITE_DROP_TEMP_TABLE;
      }else{
        code = SQLITE_DROP_TABLE;
      }
    }
    if( sqlite3AuthCheck(pParse, code, pTab->zName, zArg2, zDb) ){
      goto exit_drop_table;
    }
    if( sqlite3AuthCheck(pParse, SQLITE_DELETE, pTab->zName, 0, zDb) ){
      goto exit_drop_table;
    }
  }
#endif
  if( sqlite3StrNICmp(pTab->zName, "sqlite_", 7)==0 ){
    sqlite3ErrorMsg(pParse, "table %s may not be dropped", pTab->zName);
    goto exit_drop_table;
  }

#ifndef SQLITE_OMIT_VIEW
  if( isView && pTab->pSelect==0 ){
    sqlite3ErrorMsg(pParse, "use DROP TABLE to delete table %s", pTab->zName);
    goto exit_drop_table;
  }
  if( !isView && pTab->pSelect ){
    sqlite3ErrorMsg(pParse, "use DROP VIEW to delete view %s", pTab->zName);
    goto exit_drop_table;
  }
#endif

  v = sqlite3GetVdbe(pParse);
  if( v ){
    Trigger *pTrigger;
    Db *pDb = &db->aDb[iDb];
    sqlite3BeginWriteOperation(pParse, 1, iDb);

#ifndef SQLITE_OMIT_VIRTUALTABLE
    if( IsVirtual(pTab) ){
      sqlite3VdbeAddOp0(v, OP_VBegin);
    }
#endif
    sqlite3FkDropTable(pParse, pName, pTab);

    pTrigger = sqlite3TriggerList(pParse, pTab);
    while( pTrigger ){
      assert( pTrigger->pSchema==pTab->pSchema || 
          pTrigger->pSchema==db->aDb[1].pSchema );
      sqlite3DropTriggerPtr(pParse, pTrigger);
      pTrigger = pTrigger->pNext;
    }

#ifndef SQLITE_OMIT_AUTOINCREMENT
    if( pTab->tabFlags & TF_Autoincrement ){
      sqlite3NestedParse(pParse,
        "DELETE FROM %s.sqlite_sequence WHERE name=%Q",
        pDb->zName, pTab->zName
      );
    }
#endif

    sqlite3NestedParse(pParse, 
        "DELETE FROM %Q.%s WHERE tbl_name=%Q and type!='trigger'",
        pDb->zName, SCHEMA_TABLE(iDb), pTab->zName);

    
    if( sqlite3FindTable(db, "sqlite_stat1", db->aDb[iDb].zName) ){
      sqlite3NestedParse(pParse,
        "DELETE FROM %Q.sqlite_stat1 WHERE tbl=%Q", pDb->zName, pTab->zName
      );
    }

    if( !isView && !IsVirtual(pTab) ){
      destroyTable(pParse, pTab);
    }

    if( IsVirtual(pTab) ){
      sqlite3VdbeAddOp4(v, OP_VDestroy, iDb, 0, 0, pTab->zName, 0);
    }
    sqlite3VdbeAddOp4(v, OP_DropTable, iDb, 0, 0, pTab->zName, 0);
    sqlite3ChangeCookie(pParse, iDb);
  }
  sqliteViewResetAll(db, iDb);

exit_drop_table:
  sqlite3SrcListDelete(db, pName);
}

void sqlite3CreateForeignKey(
  Parse *pParse,       
  ExprList *pFromCol,  
  Token *pTo,          
  ExprList *pToCol,    
  int flags            
){
  sqlite3 *db = pParse->db;
#ifndef SQLITE_OMIT_FOREIGN_KEY
  FKey *pFKey = 0;
  FKey *pNextTo;
  Table *p = pParse->pNewTable;
  int nByte;
  int i;
  int nCol;
  char *z;

  assert( pTo!=0 );
  if( p==0 || IN_DECLARE_VTAB ) goto fk_end;
  if( pFromCol==0 ){
    int iCol = p->nCol-1;
    if( NEVER(iCol<0) ) goto fk_end;
    if( pToCol && pToCol->nExpr!=1 ){
      sqlite3ErrorMsg(pParse, "foreign key on %s"
         " should reference only one column of table %T",
         p->aCol[iCol].zName, pTo);
      goto fk_end;
    }
    nCol = 1;
  }else if( pToCol && pToCol->nExpr!=pFromCol->nExpr ){
    sqlite3ErrorMsg(pParse,
        "number of columns in foreign key does not match the number of "
        "columns in the referenced table");
    goto fk_end;
  }else{
    nCol = pFromCol->nExpr;
  }
  nByte = sizeof(*pFKey) + (nCol-1)*sizeof(pFKey->aCol[0]) + pTo->n + 1;
  if( pToCol ){
    for(i=0; i<pToCol->nExpr; i++){
      nByte += sqlite3Strlen30(pToCol->a[i].zName) + 1;
    }
  }
  pFKey = sqlite3DbMallocZero(db, nByte );
  if( pFKey==0 ){
    goto fk_end;
  }
  pFKey->pFrom = p;
  pFKey->pNextFrom = p->pFKey;
  z = (char*)&pFKey->aCol[nCol];
  pFKey->zTo = z;
  memcpy(z, pTo->z, pTo->n);
  z[pTo->n] = 0;
  sqlite3Dequote(z);
  z += pTo->n+1;
  pFKey->nCol = nCol;
  if( pFromCol==0 ){
    pFKey->aCol[0].iFrom = p->nCol-1;
  }else{
    for(i=0; i<nCol; i++){
      int j;
      for(j=0; j<p->nCol; j++){
        if( sqlite3StrICmp(p->aCol[j].zName, pFromCol->a[i].zName)==0 ){
          pFKey->aCol[i].iFrom = j;
          break;
        }
      }
      if( j>=p->nCol ){
        sqlite3ErrorMsg(pParse, 
          "unknown column \"%s\" in foreign key definition", 
          pFromCol->a[i].zName);
        goto fk_end;
      }
    }
  }
  if( pToCol ){
    for(i=0; i<nCol; i++){
      int n = sqlite3Strlen30(pToCol->a[i].zName);
      pFKey->aCol[i].zCol = z;
      memcpy(z, pToCol->a[i].zName, n);
      z[n] = 0;
      z += n+1;
    }
  }
  pFKey->isDeferred = 0;
  pFKey->aAction[0] = (u8)(flags & 0xff);            
  pFKey->aAction[1] = (u8)((flags >> 8 ) & 0xff);    

  assert( sqlite3SchemaMutexHeld(db, 0, p->pSchema) );
  pNextTo = (FKey *)sqlite3HashInsert(&p->pSchema->fkeyHash, 
      pFKey->zTo, sqlite3Strlen30(pFKey->zTo), (void *)pFKey
  );
  if( pNextTo==pFKey ){
    db->mallocFailed = 1;
    goto fk_end;
  }
  if( pNextTo ){
    assert( pNextTo->pPrevTo==0 );
    pFKey->pNextTo = pNextTo;
    pNextTo->pPrevTo = pFKey;
  }

  p->pFKey = pFKey;
  pFKey = 0;

fk_end:
  sqlite3DbFree(db, pFKey);
#endif 
  sqlite3ExprListDelete(db, pFromCol);
  sqlite3ExprListDelete(db, pToCol);
}

void sqlite3DeferForeignKey(Parse *pParse, int isDeferred){
#ifndef SQLITE_OMIT_FOREIGN_KEY
  Table *pTab;
  FKey *pFKey;
  if( (pTab = pParse->pNewTable)==0 || (pFKey = pTab->pFKey)==0 ) return;
  assert( isDeferred==0 || isDeferred==1 ); 
  pFKey->isDeferred = (u8)isDeferred;
#endif
}

static void sqlite3RefillIndex(Parse *pParse, Index *pIndex, int memRootPage){
  Table *pTab = pIndex->pTable;  
  int iTab = pParse->nTab++;     
  int iIdx = pParse->nTab++;     
  int addr1;                     
  int tnum;                      
  Vdbe *v;                       
  KeyInfo *pKey;                 
  int regIdxKey;                 
  int regRecord;                 
  sqlite3 *db = pParse->db;      
  int iDb = sqlite3SchemaToIndex(db, pIndex->pSchema);

#ifndef SQLITE_OMIT_AUTHORIZATION
  if( sqlite3AuthCheck(pParse, SQLITE_REINDEX, pIndex->zName, 0,
      db->aDb[iDb].zName ) ){
    return;
  }
#endif

  
  sqlite3TableLock(pParse, iDb, pTab->tnum, 1, pTab->zName);

  v = sqlite3GetVdbe(pParse);
  if( v==0 ) return;
  if( memRootPage>=0 ){
    tnum = memRootPage;
  }else{
    tnum = pIndex->tnum;
    sqlite3VdbeAddOp2(v, OP_Clear, tnum, iDb);
  }
  pKey = sqlite3IndexKeyinfo(pParse, pIndex);
  sqlite3VdbeAddOp4(v, OP_OpenWrite, iIdx, tnum, iDb, 
                    (char *)pKey, P4_KEYINFO_HANDOFF);
  if( memRootPage>=0 ){
    sqlite3VdbeChangeP5(v, 1);
  }
  sqlite3OpenTable(pParse, iTab, iDb, pTab, OP_OpenRead);
  addr1 = sqlite3VdbeAddOp2(v, OP_Rewind, iTab, 0);
  regRecord = sqlite3GetTempReg(pParse);
  regIdxKey = sqlite3GenerateIndexKey(pParse, pIndex, iTab, regRecord, 1);
  if( pIndex->onError!=OE_None ){
    const int regRowid = regIdxKey + pIndex->nColumn;
    const int j2 = sqlite3VdbeCurrentAddr(v) + 2;
    void * const pRegKey = SQLITE_INT_TO_PTR(regIdxKey);

    sqlite3VdbeAddOp4(v, OP_IsUnique, iIdx, j2, regRowid, pRegKey, P4_INT32);
    sqlite3HaltConstraint(
        pParse, OE_Abort, "indexed columns are not unique", P4_STATIC);
  }
  sqlite3VdbeAddOp2(v, OP_IdxInsert, iIdx, regRecord);
  sqlite3VdbeChangeP5(v, OPFLAG_USESEEKRESULT);
  sqlite3ReleaseTempReg(pParse, regRecord);
  sqlite3VdbeAddOp2(v, OP_Next, iTab, addr1+1);
  sqlite3VdbeJumpHere(v, addr1);
  sqlite3VdbeAddOp1(v, OP_Close, iTab);
  sqlite3VdbeAddOp1(v, OP_Close, iIdx);
}

Index *sqlite3CreateIndex(
  Parse *pParse,     
  Token *pName1,     
  Token *pName2,     
  SrcList *pTblName, 
  ExprList *pList,   
  int onError,       
  Token *pStart,     
  Token *pEnd,       
  int sortOrder,     
  int ifNotExist     
){
  Index *pRet = 0;     
  Table *pTab = 0;     
  Index *pIndex = 0;   
  char *zName = 0;     
  int nName;           
  int i, j;
  Token nullId;        
  DbFixer sFix;        
  int sortOrderMask;   
  sqlite3 *db = pParse->db;
  Db *pDb;             
  int iDb;             /* Index of the database that is being written */
  Token *pName = 0;    
  struct ExprList_item *pListItem; 
  int nCol;
  int nExtra = 0;
  char *zExtra;

  assert( pStart==0 || pEnd!=0 ); 
  assert( pParse->nErr==0 );      
  if( db->mallocFailed || IN_DECLARE_VTAB ){
    goto exit_create_index;
  }
  if( SQLITE_OK!=sqlite3ReadSchema(pParse) ){
    goto exit_create_index;
  }

  if( pTblName!=0 ){

    assert( pName1 && pName2 );
    iDb = sqlite3TwoPartName(pParse, pName1, pName2, &pName);
    if( iDb<0 ) goto exit_create_index;

#ifndef SQLITE_OMIT_TEMPDB
    if( !db->init.busy ){
      pTab = sqlite3SrcListLookup(pParse, pTblName);
      if( pName2->n==0 && pTab && pTab->pSchema==db->aDb[1].pSchema ){
        iDb = 1;
      }
    }
#endif

    if( sqlite3FixInit(&sFix, pParse, iDb, "index", pName) &&
        sqlite3FixSrcList(&sFix, pTblName)
    ){
      assert(0);
    }
    pTab = sqlite3LocateTable(pParse, 0, pTblName->a[0].zName, 
        pTblName->a[0].zDatabase);
    if( !pTab || db->mallocFailed ) goto exit_create_index;
    assert( db->aDb[iDb].pSchema==pTab->pSchema );
  }else{
    assert( pName==0 );
    pTab = pParse->pNewTable;
    if( !pTab ) goto exit_create_index;
    iDb = sqlite3SchemaToIndex(db, pTab->pSchema);
  }
  pDb = &db->aDb[iDb];

  assert( pTab!=0 );
  assert( pParse->nErr==0 );
  if( sqlite3StrNICmp(pTab->zName, "sqlite_", 7)==0 
       && sqlite3StrNICmp(&pTab->zName[7],"altertab_",9)!=0 ){
    sqlite3ErrorMsg(pParse, "table %s may not be indexed", pTab->zName);
    goto exit_create_index;
  }
#ifndef SQLITE_OMIT_VIEW
  if( pTab->pSelect ){
    sqlite3ErrorMsg(pParse, "views may not be indexed");
    goto exit_create_index;
  }
#endif
#ifndef SQLITE_OMIT_VIRTUALTABLE
  if( IsVirtual(pTab) ){
    sqlite3ErrorMsg(pParse, "virtual tables may not be indexed");
    goto exit_create_index;
  }
#endif

  if( pName ){
    zName = sqlite3NameFromToken(db, pName);
    if( zName==0 ) goto exit_create_index;
    if( SQLITE_OK!=sqlite3CheckObjectName(pParse, zName) ){
      goto exit_create_index;
    }
    if( !db->init.busy ){
      if( sqlite3FindTable(db, zName, 0)!=0 ){
        sqlite3ErrorMsg(pParse, "there is already a table named %s", zName);
        goto exit_create_index;
      }
    }
    if( sqlite3FindIndex(db, zName, pDb->zName)!=0 ){
      if( !ifNotExist ){
        sqlite3ErrorMsg(pParse, "index %s already exists", zName);
      }else{
        assert( !db->init.busy );
        sqlite3CodeVerifySchema(pParse, iDb);
      }
      goto exit_create_index;
    }
  }else{
    int n;
    Index *pLoop;
    for(pLoop=pTab->pIndex, n=1; pLoop; pLoop=pLoop->pNext, n++){}
    zName = sqlite3MPrintf(db, "sqlite_autoindex_%s_%d", pTab->zName, n);
    if( zName==0 ){
      goto exit_create_index;
    }
  }

#ifndef SQLITE_OMIT_AUTHORIZATION
  {
    const char *zDb = pDb->zName;
    if( sqlite3AuthCheck(pParse, SQLITE_INSERT, SCHEMA_TABLE(iDb), 0, zDb) ){
      goto exit_create_index;
    }
    i = SQLITE_CREATE_INDEX;
    if( !OMIT_TEMPDB && iDb==1 ) i = SQLITE_CREATE_TEMP_INDEX;
    if( sqlite3AuthCheck(pParse, i, zName, pTab->zName, zDb) ){
      goto exit_create_index;
    }
  }
#endif

  if( pList==0 ){
    nullId.z = pTab->aCol[pTab->nCol-1].zName;
    nullId.n = sqlite3Strlen30((char*)nullId.z);
    pList = sqlite3ExprListAppend(pParse, 0, 0);
    if( pList==0 ) goto exit_create_index;
    sqlite3ExprListSetName(pParse, pList, &nullId, 0);
    pList->a[0].sortOrder = (u8)sortOrder;
  }

  for(i=0; i<pList->nExpr; i++){
    Expr *pExpr = pList->a[i].pExpr;
    if( pExpr ){
      CollSeq *pColl = pExpr->pColl;
      if( ALWAYS(pColl) ){
        nExtra += (1 + sqlite3Strlen30(pColl->zName));
      }
    }
  }

  nName = sqlite3Strlen30(zName);
  nCol = pList->nExpr;
  pIndex = sqlite3DbMallocZero(db, 
      sizeof(Index) +              
      sizeof(int)*nCol +           
      sizeof(int)*(nCol+1) +       
      sizeof(char *)*nCol +        
      sizeof(u8)*nCol +            
      nName + 1 +                  
      nExtra                       
  );
  if( db->mallocFailed ){
    goto exit_create_index;
  }
  pIndex->azColl = (char**)(&pIndex[1]);
  pIndex->aiColumn = (int *)(&pIndex->azColl[nCol]);
  pIndex->aiRowEst = (unsigned *)(&pIndex->aiColumn[nCol]);
  pIndex->aSortOrder = (u8 *)(&pIndex->aiRowEst[nCol+1]);
  pIndex->zName = (char *)(&pIndex->aSortOrder[nCol]);
  zExtra = (char *)(&pIndex->zName[nName+1]);
  memcpy(pIndex->zName, zName, nName+1);
  pIndex->pTable = pTab;
  pIndex->nColumn = pList->nExpr;
  pIndex->onError = (u8)onError;
  pIndex->autoIndex = (u8)(pName==0);
  pIndex->pSchema = db->aDb[iDb].pSchema;
  assert( sqlite3SchemaMutexHeld(db, iDb, 0) );

  if( pDb->pSchema->file_format>=4 ){
    sortOrderMask = -1;   
  }else{
    sortOrderMask = 0;    
  }

  for(i=0, pListItem=pList->a; i<pList->nExpr; i++, pListItem++){
    const char *zColName = pListItem->zName;
    Column *pTabCol;
    int requestedSortOrder;
    char *zColl;                   

    for(j=0, pTabCol=pTab->aCol; j<pTab->nCol; j++, pTabCol++){
      if( sqlite3StrICmp(zColName, pTabCol->zName)==0 ) break;
    }
    if( j>=pTab->nCol ){
      sqlite3ErrorMsg(pParse, "table %s has no column named %s",
        pTab->zName, zColName);
      pParse->checkSchema = 1;
      goto exit_create_index;
    }
    pIndex->aiColumn[i] = j;
    if( pListItem->pExpr && ALWAYS(pListItem->pExpr->pColl) ){
      int nColl;
      zColl = pListItem->pExpr->pColl->zName;
      nColl = sqlite3Strlen30(zColl) + 1;
      assert( nExtra>=nColl );
      memcpy(zExtra, zColl, nColl);
      zColl = zExtra;
      zExtra += nColl;
      nExtra -= nColl;
    }else{
      zColl = pTab->aCol[j].zColl;
      if( !zColl ){
        zColl = db->pDfltColl->zName;
      }
    }
    if( !db->init.busy && !sqlite3LocateCollSeq(pParse, zColl) ){
      goto exit_create_index;
    }
    pIndex->azColl[i] = zColl;
    requestedSortOrder = pListItem->sortOrder & sortOrderMask;
    pIndex->aSortOrder[i] = (u8)requestedSortOrder;
  }
  sqlite3DefaultRowEst(pIndex);

  if( pTab==pParse->pNewTable ){
    Index *pIdx;
    for(pIdx=pTab->pIndex; pIdx; pIdx=pIdx->pNext){
      int k;
      assert( pIdx->onError!=OE_None );
      assert( pIdx->autoIndex );
      assert( pIndex->onError!=OE_None );

      if( pIdx->nColumn!=pIndex->nColumn ) continue;
      for(k=0; k<pIdx->nColumn; k++){
        const char *z1;
        const char *z2;
        if( pIdx->aiColumn[k]!=pIndex->aiColumn[k] ) break;
        z1 = pIdx->azColl[k];
        z2 = pIndex->azColl[k];
        if( z1!=z2 && sqlite3StrICmp(z1, z2) ) break;
      }
      if( k==pIdx->nColumn ){
        if( pIdx->onError!=pIndex->onError ){
          if( !(pIdx->onError==OE_Default || pIndex->onError==OE_Default) ){
            sqlite3ErrorMsg(pParse, 
                "conflicting ON CONFLICT clauses specified", 0);
          }
          if( pIdx->onError==OE_Default ){
            pIdx->onError = pIndex->onError;
          }
        }
        goto exit_create_index;
      }
    }
  }

  if( db->init.busy ){
    Index *p;
    assert( sqlite3SchemaMutexHeld(db, 0, pIndex->pSchema) );
    p = sqlite3HashInsert(&pIndex->pSchema->idxHash, 
                          pIndex->zName, sqlite3Strlen30(pIndex->zName),
                          pIndex);
    if( p ){
      assert( p==pIndex );  
      db->mallocFailed = 1;
      goto exit_create_index;
    }
    db->flags |= SQLITE_InternChanges;
    if( pTblName!=0 ){
      pIndex->tnum = db->init.newTnum;
    }
  }

  else{ 
    Vdbe *v;
    char *zStmt;
    int iMem = ++pParse->nMem;

    v = sqlite3GetVdbe(pParse);
    if( v==0 ) goto exit_create_index;


    sqlite3BeginWriteOperation(pParse, 1, iDb);
    sqlite3VdbeAddOp2(v, OP_CreateIndex, iDb, iMem);

    if( pStart ){
      assert( pEnd!=0 );
      
      zStmt = sqlite3MPrintf(db, "CREATE%s INDEX %.*s",
        onError==OE_None ? "" : " UNIQUE",
        pEnd->z - pName->z + 1,
        pName->z);
    }else{
      
      
      zStmt = 0;
    }

    sqlite3NestedParse(pParse, 
        "INSERT INTO %Q.%s VALUES('index',%Q,%Q,#%d,%Q);",
        db->aDb[iDb].zName, SCHEMA_TABLE(iDb),
        pIndex->zName,
        pTab->zName,
        iMem,
        zStmt
    );
    sqlite3DbFree(db, zStmt);

    if( pTblName ){
      sqlite3RefillIndex(pParse, pIndex, iMem);
      sqlite3ChangeCookie(pParse, iDb);
      sqlite3VdbeAddOp4(v, OP_ParseSchema, iDb, 0, 0,
         sqlite3MPrintf(db, "name='%q' AND type='index'", pIndex->zName), 
         P4_DYNAMIC);
      sqlite3VdbeAddOp1(v, OP_Expire, 0);
    }
  }

  if( db->init.busy || pTblName==0 ){
    if( onError!=OE_Replace || pTab->pIndex==0
         || pTab->pIndex->onError==OE_Replace){
      pIndex->pNext = pTab->pIndex;
      pTab->pIndex = pIndex;
    }else{
      Index *pOther = pTab->pIndex;
      while( pOther->pNext && pOther->pNext->onError!=OE_Replace ){
        pOther = pOther->pNext;
      }
      pIndex->pNext = pOther->pNext;
      pOther->pNext = pIndex;
    }
    pRet = pIndex;
    pIndex = 0;
  }

  
exit_create_index:
  if( pIndex ){
    sqlite3DbFree(db, pIndex->zColAff);
    sqlite3DbFree(db, pIndex);
  }
  sqlite3ExprListDelete(db, pList);
  sqlite3SrcListDelete(db, pTblName);
  sqlite3DbFree(db, zName);
  return pRet;
}

void sqlite3DefaultRowEst(Index *pIdx){
  unsigned *a = pIdx->aiRowEst;
  int i;
  unsigned n;
  assert( a!=0 );
  a[0] = pIdx->pTable->nRowEst;
  if( a[0]<10 ) a[0] = 10;
  n = 10;
  for(i=1; i<=pIdx->nColumn; i++){
    a[i] = n;
    if( n>5 ) n--;
  }
  if( pIdx->onError!=OE_None ){
    a[pIdx->nColumn] = 1;
  }
}

void sqlite3DropIndex(Parse *pParse, SrcList *pName, int ifExists){
  Index *pIndex;
  Vdbe *v;
  sqlite3 *db = pParse->db;
  int iDb;

  assert( pParse->nErr==0 );   
  if( db->mallocFailed ){
    goto exit_drop_index;
  }
  assert( pName->nSrc==1 );
  if( SQLITE_OK!=sqlite3ReadSchema(pParse) ){
    goto exit_drop_index;
  }
  pIndex = sqlite3FindIndex(db, pName->a[0].zName, pName->a[0].zDatabase);
  if( pIndex==0 ){
    if( !ifExists ){
      sqlite3ErrorMsg(pParse, "no such index: %S", pName, 0);
    }else{
      sqlite3CodeVerifyNamedSchema(pParse, pName->a[0].zDatabase);
    }
    pParse->checkSchema = 1;
    goto exit_drop_index;
  }
  if( pIndex->autoIndex ){
    sqlite3ErrorMsg(pParse, "index associated with UNIQUE "
      "or PRIMARY KEY constraint cannot be dropped", 0);
    goto exit_drop_index;
  }
  iDb = sqlite3SchemaToIndex(db, pIndex->pSchema);
#ifndef SQLITE_OMIT_AUTHORIZATION
  {
    int code = SQLITE_DROP_INDEX;
    Table *pTab = pIndex->pTable;
    const char *zDb = db->aDb[iDb].zName;
    const char *zTab = SCHEMA_TABLE(iDb);
    if( sqlite3AuthCheck(pParse, SQLITE_DELETE, zTab, 0, zDb) ){
      goto exit_drop_index;
    }
    if( !OMIT_TEMPDB && iDb ) code = SQLITE_DROP_TEMP_INDEX;
    if( sqlite3AuthCheck(pParse, code, pIndex->zName, pTab->zName, zDb) ){
      goto exit_drop_index;
    }
  }
#endif

  
  v = sqlite3GetVdbe(pParse);
  if( v ){
    sqlite3BeginWriteOperation(pParse, 1, iDb);
    sqlite3NestedParse(pParse,
       "DELETE FROM %Q.%s WHERE name=%Q AND type='index'",
       db->aDb[iDb].zName, SCHEMA_TABLE(iDb),
       pIndex->zName
    );
    if( sqlite3FindTable(db, "sqlite_stat1", db->aDb[iDb].zName) ){
      sqlite3NestedParse(pParse,
        "DELETE FROM %Q.sqlite_stat1 WHERE idx=%Q",
        db->aDb[iDb].zName, pIndex->zName
      );
    }
    sqlite3ChangeCookie(pParse, iDb);
    destroyRootPage(pParse, pIndex->tnum, iDb);
    sqlite3VdbeAddOp4(v, OP_DropIndex, iDb, 0, 0, pIndex->zName, 0);
  }

exit_drop_index:
  sqlite3SrcListDelete(db, pName);
}

void *sqlite3ArrayAllocate(
  sqlite3 *db,      
  void *pArray,     
  int szEntry,      
  int initSize,     
  int *pnEntry,     
  int *pnAlloc,     
  int *pIdx         
){
  char *z;
  if( *pnEntry >= *pnAlloc ){
    void *pNew;
    int newSize;
    newSize = (*pnAlloc)*2 + initSize;
    pNew = sqlite3DbRealloc(db, pArray, newSize*szEntry);
    if( pNew==0 ){
      *pIdx = -1;
      return pArray;
    }
    *pnAlloc = sqlite3DbMallocSize(db, pNew)/szEntry;
    pArray = pNew;
  }
  z = (char*)pArray;
  memset(&z[*pnEntry * szEntry], 0, szEntry);
  *pIdx = *pnEntry;
  ++*pnEntry;
  return pArray;
}

IdList *sqlite3IdListAppend(sqlite3 *db, IdList *pList, Token *pToken){
  int i;
  if( pList==0 ){
    pList = sqlite3DbMallocZero(db, sizeof(IdList) );
    if( pList==0 ) return 0;
    pList->nAlloc = 0;
  }
  pList->a = sqlite3ArrayAllocate(
      db,
      pList->a,
      sizeof(pList->a[0]),
      5,
      &pList->nId,
      &pList->nAlloc,
      &i
  );
  if( i<0 ){
    sqlite3IdListDelete(db, pList);
    return 0;
  }
  pList->a[i].zName = sqlite3NameFromToken(db, pToken);
  return pList;
}

void sqlite3IdListDelete(sqlite3 *db, IdList *pList){
  int i;
  if( pList==0 ) return;
  for(i=0; i<pList->nId; i++){
    sqlite3DbFree(db, pList->a[i].zName);
  }
  sqlite3DbFree(db, pList->a);
  sqlite3DbFree(db, pList);
}

int sqlite3IdListIndex(IdList *pList, const char *zName){
  int i;
  if( pList==0 ) return -1;
  for(i=0; i<pList->nId; i++){
    if( sqlite3StrICmp(pList->a[i].zName, zName)==0 ) return i;
  }
  return -1;
}

SrcList *sqlite3SrcListEnlarge(
  sqlite3 *db,       
  SrcList *pSrc,     
  int nExtra,        
  int iStart         
){
  int i;

  
  assert( iStart>=0 );
  assert( nExtra>=1 );
  assert( pSrc!=0 );
  assert( iStart<=pSrc->nSrc );

  
  if( pSrc->nSrc+nExtra>pSrc->nAlloc ){
    SrcList *pNew;
    int nAlloc = pSrc->nSrc+nExtra;
    int nGot;
    pNew = sqlite3DbRealloc(db, pSrc,
               sizeof(*pSrc) + (nAlloc-1)*sizeof(pSrc->a[0]) );
    if( pNew==0 ){
      assert( db->mallocFailed );
      return pSrc;
    }
    pSrc = pNew;
    nGot = (sqlite3DbMallocSize(db, pNew) - sizeof(*pSrc))/sizeof(pSrc->a[0])+1;
    pSrc->nAlloc = (u16)nGot;
  }

  for(i=pSrc->nSrc-1; i>=iStart; i--){
    pSrc->a[i+nExtra] = pSrc->a[i];
  }
  pSrc->nSrc += (i16)nExtra;

  
  memset(&pSrc->a[iStart], 0, sizeof(pSrc->a[0])*nExtra);
  for(i=iStart; i<iStart+nExtra; i++){
    pSrc->a[i].iCursor = -1;
  }

  
  return pSrc;
}


SrcList *sqlite3SrcListAppend(
  sqlite3 *db,        
  SrcList *pList,     
  Token *pTable,      
  Token *pDatabase    
){
  struct SrcList_item *pItem;
  assert( pDatabase==0 || pTable!=0 );  
  if( pList==0 ){
    pList = sqlite3DbMallocZero(db, sizeof(SrcList) );
    if( pList==0 ) return 0;
    pList->nAlloc = 1;
  }
  pList = sqlite3SrcListEnlarge(db, pList, 1, pList->nSrc);
  if( db->mallocFailed ){
    sqlite3SrcListDelete(db, pList);
    return 0;
  }
  pItem = &pList->a[pList->nSrc-1];
  if( pDatabase && pDatabase->z==0 ){
    pDatabase = 0;
  }
  if( pDatabase ){
    Token *pTemp = pDatabase;
    pDatabase = pTable;
    pTable = pTemp;
  }
  pItem->zName = sqlite3NameFromToken(db, pTable);
  pItem->zDatabase = sqlite3NameFromToken(db, pDatabase);
  return pList;
}

void sqlite3SrcListAssignCursors(Parse *pParse, SrcList *pList){
  int i;
  struct SrcList_item *pItem;
  assert(pList || pParse->db->mallocFailed );
  if( pList ){
    for(i=0, pItem=pList->a; i<pList->nSrc; i++, pItem++){
      if( pItem->iCursor>=0 ) break;
      pItem->iCursor = pParse->nTab++;
      if( pItem->pSelect ){
        sqlite3SrcListAssignCursors(pParse, pItem->pSelect->pSrc);
      }
    }
  }
}

void sqlite3SrcListDelete(sqlite3 *db, SrcList *pList){
  int i;
  struct SrcList_item *pItem;
  if( pList==0 ) return;
  for(pItem=pList->a, i=0; i<pList->nSrc; i++, pItem++){
    sqlite3DbFree(db, pItem->zDatabase);
    sqlite3DbFree(db, pItem->zName);
    sqlite3DbFree(db, pItem->zAlias);
    sqlite3DbFree(db, pItem->zIndex);
    sqlite3DeleteTable(db, pItem->pTab);
    sqlite3SelectDelete(db, pItem->pSelect);
    sqlite3ExprDelete(db, pItem->pOn);
    sqlite3IdListDelete(db, pItem->pUsing);
  }
  sqlite3DbFree(db, pList);
}

SrcList *sqlite3SrcListAppendFromTerm(
  Parse *pParse,          
  SrcList *p,             
  Token *pTable,          
  Token *pDatabase,       
  Token *pAlias,          
  Select *pSubquery,      
  Expr *pOn,              
  IdList *pUsing          
){
  struct SrcList_item *pItem;
  sqlite3 *db = pParse->db;
  if( !p && (pOn || pUsing) ){
    sqlite3ErrorMsg(pParse, "a JOIN clause is required before %s", 
      (pOn ? "ON" : "USING")
    );
    goto append_from_error;
  }
  p = sqlite3SrcListAppend(db, p, pTable, pDatabase);
  if( p==0 || NEVER(p->nSrc==0) ){
    goto append_from_error;
  }
  pItem = &p->a[p->nSrc-1];
  assert( pAlias!=0 );
  if( pAlias->n ){
    pItem->zAlias = sqlite3NameFromToken(db, pAlias);
  }
  pItem->pSelect = pSubquery;
  pItem->pOn = pOn;
  pItem->pUsing = pUsing;
  return p;

 append_from_error:
  assert( p==0 );
  sqlite3ExprDelete(db, pOn);
  sqlite3IdListDelete(db, pUsing);
  sqlite3SelectDelete(db, pSubquery);
  return 0;
}

void sqlite3SrcListIndexedBy(Parse *pParse, SrcList *p, Token *pIndexedBy){
  assert( pIndexedBy!=0 );
  if( p && ALWAYS(p->nSrc>0) ){
    struct SrcList_item *pItem = &p->a[p->nSrc-1];
    assert( pItem->notIndexed==0 && pItem->zIndex==0 );
    if( pIndexedBy->n==1 && !pIndexedBy->z ){
      pItem->notIndexed = 1;
    }else{
      pItem->zIndex = sqlite3NameFromToken(pParse->db, pIndexedBy);
    }
  }
}

void sqlite3SrcListShiftJoinType(SrcList *p){
  if( p && p->a ){
    int i;
    for(i=p->nSrc-1; i>0; i--){
      p->a[i].jointype = p->a[i-1].jointype;
    }
    p->a[0].jointype = 0;
  }
}

void sqlite3BeginTransaction(Parse *pParse, int type){
  sqlite3 *db;
  Vdbe *v;
  int i;

  assert( pParse!=0 );
  db = pParse->db;
  assert( db!=0 );
  if( sqlite3AuthCheck(pParse, SQLITE_TRANSACTION, "BEGIN", 0, 0) ){
    return;
  }
  v = sqlite3GetVdbe(pParse);
  if( !v ) return;
  if( type!=TK_DEFERRED ){
    for(i=0; i<db->nDb; i++){
      sqlite3VdbeAddOp2(v, OP_Transaction, i, (type==TK_EXCLUSIVE)+1);
      sqlite3VdbeUsesBtree(v, i);
    }
  }
  sqlite3VdbeAddOp2(v, OP_AutoCommit, 0, 0);
}

void sqlite3CommitTransaction(Parse *pParse){
  sqlite3 *db;
  Vdbe *v;

  assert( pParse!=0 );
  db = pParse->db;
  assert( db!=0 );
  if( sqlite3AuthCheck(pParse, SQLITE_TRANSACTION, "COMMIT", 0, 0) ){
    return;
  }
  v = sqlite3GetVdbe(pParse);
  if( v ){
    sqlite3VdbeAddOp2(v, OP_AutoCommit, 1, 0);
  }
}

void sqlite3RollbackTransaction(Parse *pParse){
  sqlite3 *db;
  Vdbe *v;

  assert( pParse!=0 );
  db = pParse->db;
  assert( db!=0 );
  if( sqlite3AuthCheck(pParse, SQLITE_TRANSACTION, "ROLLBACK", 0, 0) ){
    return;
  }
  v = sqlite3GetVdbe(pParse);
  if( v ){
    sqlite3VdbeAddOp2(v, OP_AutoCommit, 1, 1);
  }
}

void sqlite3Savepoint(Parse *pParse, int op, Token *pName){
  char *zName = sqlite3NameFromToken(pParse->db, pName);
  if( zName ){
    Vdbe *v = sqlite3GetVdbe(pParse);
#ifndef SQLITE_OMIT_AUTHORIZATION
    static const char * const az[] = { "BEGIN", "RELEASE", "ROLLBACK" };
    assert( !SAVEPOINT_BEGIN && SAVEPOINT_RELEASE==1 && SAVEPOINT_ROLLBACK==2 );
#endif
    if( !v || sqlite3AuthCheck(pParse, SQLITE_SAVEPOINT, az[op], zName, 0) ){
      sqlite3DbFree(pParse->db, zName);
      return;
    }
    sqlite3VdbeAddOp4(v, OP_Savepoint, op, 0, 0, zName, P4_DYNAMIC);
  }
}

int sqlite3OpenTempDatabase(Parse *pParse){
  sqlite3 *db = pParse->db;
  if( db->aDb[1].pBt==0 && !pParse->explain ){
    int rc;
    Btree *pBt;
    static const int flags = 
          SQLITE_OPEN_READWRITE |
          SQLITE_OPEN_CREATE |
          SQLITE_OPEN_EXCLUSIVE |
          SQLITE_OPEN_DELETEONCLOSE |
          SQLITE_OPEN_TEMP_DB;

    rc = sqlite3BtreeOpen(0, db, &pBt, 0, flags);
    if( rc!=SQLITE_OK ){
      sqlite3ErrorMsg(pParse, "unable to open a temporary database "
        "file for storing temporary tables");
      pParse->rc = rc;
      return 1;
    }
    db->aDb[1].pBt = pBt;
    assert( db->aDb[1].pSchema );
    if( SQLITE_NOMEM==sqlite3BtreeSetPageSize(pBt, db->nextPagesize, -1, 0) ){
      db->mallocFailed = 1;
      return 1;
    }
  }
  return 0;
}

void sqlite3CodeVerifySchema(Parse *pParse, int iDb){
  Parse *pToplevel = sqlite3ParseToplevel(pParse);

  if( pToplevel->cookieGoto==0 ){
    Vdbe *v = sqlite3GetVdbe(pToplevel);
    if( v==0 ) return;  
    pToplevel->cookieGoto = sqlite3VdbeAddOp2(v, OP_Goto, 0, 0)+1;
  }
  if( iDb>=0 ){
    sqlite3 *db = pToplevel->db;
    yDbMask mask;

    assert( iDb<db->nDb );
    assert( db->aDb[iDb].pBt!=0 || iDb==1 );
    assert( iDb<SQLITE_MAX_ATTACHED+2 );
    assert( sqlite3SchemaMutexHeld(db, iDb, 0) );
    mask = ((yDbMask)1)<<iDb;
    if( (pToplevel->cookieMask & mask)==0 ){
      pToplevel->cookieMask |= mask;
      pToplevel->cookieValue[iDb] = db->aDb[iDb].pSchema->schema_cookie;
      if( !OMIT_TEMPDB && iDb==1 ){
        sqlite3OpenTempDatabase(pToplevel);
      }
    }
  }
}

void sqlite3CodeVerifyNamedSchema(Parse *pParse, const char *zDb){
  sqlite3 *db = pParse->db;
  int i;
  for(i=0; i<db->nDb; i++){
    Db *pDb = &db->aDb[i];
    if( pDb->pBt && (!zDb || 0==sqlite3StrICmp(zDb, pDb->zName)) ){
      sqlite3CodeVerifySchema(pParse, i);
    }
  }
}

void sqlite3BeginWriteOperation(Parse *pParse, int setStatement, int iDb){
  Parse *pToplevel = sqlite3ParseToplevel(pParse);
  sqlite3CodeVerifySchema(pParse, iDb);
  pToplevel->writeMask |= ((yDbMask)1)<<iDb;
  pToplevel->isMultiWrite |= setStatement;
}

void sqlite3MultiWrite(Parse *pParse){
  Parse *pToplevel = sqlite3ParseToplevel(pParse);
  pToplevel->isMultiWrite = 1;
}

void sqlite3MayAbort(Parse *pParse){
  Parse *pToplevel = sqlite3ParseToplevel(pParse);
  pToplevel->mayAbort = 1;
}

void sqlite3HaltConstraint(Parse *pParse, int onError, char *p4, int p4type){
  Vdbe *v = sqlite3GetVdbe(pParse);
  if( onError==OE_Abort ){
    sqlite3MayAbort(pParse);
  }
  sqlite3VdbeAddOp4(v, OP_Halt, SQLITE_CONSTRAINT, onError, 0, p4, p4type);
}

#ifndef SQLITE_OMIT_REINDEX
static int collationMatch(const char *zColl, Index *pIndex){
  int i;
  assert( zColl!=0 );
  for(i=0; i<pIndex->nColumn; i++){
    const char *z = pIndex->azColl[i];
    assert( z!=0 );
    if( 0==sqlite3StrICmp(z, zColl) ){
      return 1;
    }
  }
  return 0;
}
#endif

#ifndef SQLITE_OMIT_REINDEX
static void reindexTable(Parse *pParse, Table *pTab, char const *zColl){
  Index *pIndex;              

  for(pIndex=pTab->pIndex; pIndex; pIndex=pIndex->pNext){
    if( zColl==0 || collationMatch(zColl, pIndex) ){
      int iDb = sqlite3SchemaToIndex(pParse->db, pTab->pSchema);
      sqlite3BeginWriteOperation(pParse, 0, iDb);
      sqlite3RefillIndex(pParse, pIndex, -1);
    }
  }
}
#endif

#ifndef SQLITE_OMIT_REINDEX
static void reindexDatabases(Parse *pParse, char const *zColl){
  Db *pDb;                    
  int iDb;                    
  sqlite3 *db = pParse->db;   
  HashElem *k;                
  Table *pTab;                

  assert( sqlite3BtreeHoldsAllMutexes(db) );  
  for(iDb=0, pDb=db->aDb; iDb<db->nDb; iDb++, pDb++){
    assert( pDb!=0 );
    for(k=sqliteHashFirst(&pDb->pSchema->tblHash);  k; k=sqliteHashNext(k)){
      pTab = (Table*)sqliteHashData(k);
      reindexTable(pParse, pTab, zColl);
    }
  }
}
#endif

#ifndef SQLITE_OMIT_REINDEX
void sqlite3Reindex(Parse *pParse, Token *pName1, Token *pName2){
  CollSeq *pColl;             
  char *z;                    
  const char *zDb;            
  Table *pTab;                
  Index *pIndex;              
  int iDb;                    
  sqlite3 *db = pParse->db;   
  Token *pObjName;            

  if( SQLITE_OK!=sqlite3ReadSchema(pParse) ){
    return;
  }

  if( pName1==0 ){
    reindexDatabases(pParse, 0);
    return;
  }else if( NEVER(pName2==0) || pName2->z==0 ){
    char *zColl;
    assert( pName1->z );
    zColl = sqlite3NameFromToken(pParse->db, pName1);
    if( !zColl ) return;
    pColl = sqlite3FindCollSeq(db, ENC(db), zColl, 0);
    if( pColl ){
      reindexDatabases(pParse, zColl);
      sqlite3DbFree(db, zColl);
      return;
    }
    sqlite3DbFree(db, zColl);
  }
  iDb = sqlite3TwoPartName(pParse, pName1, pName2, &pObjName);
  if( iDb<0 ) return;
  z = sqlite3NameFromToken(db, pObjName);
  if( z==0 ) return;
  zDb = db->aDb[iDb].zName;
  pTab = sqlite3FindTable(db, z, zDb);
  if( pTab ){
    reindexTable(pParse, pTab, 0);
    sqlite3DbFree(db, z);
    return;
  }
  pIndex = sqlite3FindIndex(db, z, zDb);
  sqlite3DbFree(db, z);
  if( pIndex ){
    sqlite3BeginWriteOperation(pParse, 0, iDb);
    sqlite3RefillIndex(pParse, pIndex, -1);
    return;
  }
  sqlite3ErrorMsg(pParse, "unable to identify the object to be reindexed");
}
#endif

KeyInfo *sqlite3IndexKeyinfo(Parse *pParse, Index *pIdx){
  int i;
  int nCol = pIdx->nColumn;
  int nBytes = sizeof(KeyInfo) + (nCol-1)*sizeof(CollSeq*) + nCol;
  sqlite3 *db = pParse->db;
  KeyInfo *pKey = (KeyInfo *)sqlite3DbMallocZero(db, nBytes);

  if( pKey ){
    pKey->db = pParse->db;
    pKey->aSortOrder = (u8 *)&(pKey->aColl[nCol]);
    assert( &pKey->aSortOrder[nCol]==&(((u8 *)pKey)[nBytes]) );
    for(i=0; i<nCol; i++){
      char *zColl = pIdx->azColl[i];
      assert( zColl );
      pKey->aColl[i] = sqlite3LocateCollSeq(pParse, zColl);
      pKey->aSortOrder[i] = pIdx->aSortOrder[i];
    }
    pKey->nField = (u16)nCol;
  }

  if( pParse->nErr ){
    sqlite3DbFree(db, pKey);
    pKey = 0;
  }
  return pKey;
}

int sqlite3_preload(sqlite3 *db)
{
  Pager *pPager;
  Btree *pBt;
  int rc;
  int i;
  int dbsLoaded = 0;

  for(i=0; i<db->nDb; i++) {
    pBt = db->aDb[i].pBt;
    if( !pBt )
      continue;
    pPager = sqlite3BtreePager(pBt);
    if( pPager ) {
      rc = sqlite3PagerLoadall(pPager);
      if (rc == SQLITE_OK)
        dbsLoaded++;
    }
  }
  if (dbsLoaded == 0)
    return SQLITE_ERROR;
  return SQLITE_OK;
}

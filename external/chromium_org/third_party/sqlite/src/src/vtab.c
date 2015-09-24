/*
** 2006 June 10
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This file contains code used to help implement virtual tables.
*/
#ifndef SQLITE_OMIT_VIRTUALTABLE
#include "sqliteInt.h"

static int createModule(
  sqlite3 *db,                    
  const char *zName,              
  const sqlite3_module *pModule,  
  void *pAux,                     
  void (*xDestroy)(void *)        
){
  int rc, nName;
  Module *pMod;

  sqlite3_mutex_enter(db->mutex);
  nName = sqlite3Strlen30(zName);
  pMod = (Module *)sqlite3DbMallocRaw(db, sizeof(Module) + nName + 1);
  if( pMod ){
    Module *pDel;
    char *zCopy = (char *)(&pMod[1]);
    memcpy(zCopy, zName, nName+1);
    pMod->zName = zCopy;
    pMod->pModule = pModule;
    pMod->pAux = pAux;
    pMod->xDestroy = xDestroy;
    pDel = (Module *)sqlite3HashInsert(&db->aModule, zCopy, nName, (void*)pMod);
    if( pDel && pDel->xDestroy ){
      pDel->xDestroy(pDel->pAux);
    }
    sqlite3DbFree(db, pDel);
    if( pDel==pMod ){
      db->mallocFailed = 1;
    }
    sqlite3ResetInternalSchema(db, -1);
  }else if( xDestroy ){
    xDestroy(pAux);
  }
  rc = sqlite3ApiExit(db, SQLITE_OK);
  sqlite3_mutex_leave(db->mutex);
  return rc;
}


int sqlite3_create_module(
  sqlite3 *db,                    
  const char *zName,              
  const sqlite3_module *pModule,  
  void *pAux                      
){
  return createModule(db, zName, pModule, pAux, 0);
}

int sqlite3_create_module_v2(
  sqlite3 *db,                    
  const char *zName,              
  const sqlite3_module *pModule,  
  void *pAux,                     
  void (*xDestroy)(void *)        
){
  return createModule(db, zName, pModule, pAux, xDestroy);
}

void sqlite3VtabLock(VTable *pVTab){
  pVTab->nRef++;
}


VTable *sqlite3GetVTable(sqlite3 *db, Table *pTab){
  VTable *pVtab;
  assert( IsVirtual(pTab) );
  for(pVtab=pTab->pVTable; pVtab && pVtab->db!=db; pVtab=pVtab->pNext);
  return pVtab;
}

void sqlite3VtabUnlock(VTable *pVTab){
  sqlite3 *db = pVTab->db;

  assert( db );
  assert( pVTab->nRef>0 );
  assert( sqlite3SafetyCheckOk(db) );

  pVTab->nRef--;
  if( pVTab->nRef==0 ){
    sqlite3_vtab *p = pVTab->pVtab;
    if( p ){
      p->pModule->xDisconnect(p);
    }
    sqlite3DbFree(db, pVTab);
  }
}

static VTable *vtabDisconnectAll(sqlite3 *db, Table *p){
  VTable *pRet = 0;
  VTable *pVTable = p->pVTable;
  p->pVTable = 0;

  assert( db==0 || sqlite3SchemaMutexHeld(db, 0, p->pSchema) );

  while( pVTable ){
    sqlite3 *db2 = pVTable->db;
    VTable *pNext = pVTable->pNext;
    assert( db2 );
    if( db2==db ){
      pRet = pVTable;
      p->pVTable = pRet;
      pRet->pNext = 0;
    }else{
      pVTable->pNext = db2->pDisconnect;
      db2->pDisconnect = pVTable;
    }
    pVTable = pNext;
  }

  assert( !db || pRet );
  return pRet;
}


void sqlite3VtabUnlockList(sqlite3 *db){
  VTable *p = db->pDisconnect;
  db->pDisconnect = 0;

  assert( sqlite3BtreeHoldsAllMutexes(db) );
  assert( sqlite3_mutex_held(db->mutex) );

  if( p ){
    sqlite3ExpirePreparedStatements(db);
    do {
      VTable *pNext = p->pNext;
      sqlite3VtabUnlock(p);
      p = pNext;
    }while( p );
  }
}

void sqlite3VtabClear(sqlite3 *db, Table *p){
  if( !db || db->pnBytesFreed==0 ) vtabDisconnectAll(0, p);
  if( p->azModuleArg ){
    int i;
    for(i=0; i<p->nModuleArg; i++){
      sqlite3DbFree(db, p->azModuleArg[i]);
    }
    sqlite3DbFree(db, p->azModuleArg);
  }
}

static void addModuleArgument(sqlite3 *db, Table *pTable, char *zArg){
  int i = pTable->nModuleArg++;
  int nBytes = sizeof(char *)*(1+pTable->nModuleArg);
  char **azModuleArg;
  azModuleArg = sqlite3DbRealloc(db, pTable->azModuleArg, nBytes);
  if( azModuleArg==0 ){
    int j;
    for(j=0; j<i; j++){
      sqlite3DbFree(db, pTable->azModuleArg[j]);
    }
    sqlite3DbFree(db, zArg);
    sqlite3DbFree(db, pTable->azModuleArg);
    pTable->nModuleArg = 0;
  }else{
    azModuleArg[i] = zArg;
    azModuleArg[i+1] = 0;
  }
  pTable->azModuleArg = azModuleArg;
}

void sqlite3VtabBeginParse(
  Parse *pParse,        
  Token *pName1,        
  Token *pName2,        
  Token *pModuleName    
){
  int iDb;              
  Table *pTable;        
  sqlite3 *db;          

  sqlite3StartTable(pParse, pName1, pName2, 0, 0, 1, 0);
  pTable = pParse->pNewTable;
  if( pTable==0 ) return;
  assert( 0==pTable->pIndex );

  db = pParse->db;
  iDb = sqlite3SchemaToIndex(db, pTable->pSchema);
  assert( iDb>=0 );

  pTable->tabFlags |= TF_Virtual;
  pTable->nModuleArg = 0;
  addModuleArgument(db, pTable, sqlite3NameFromToken(db, pModuleName));
  addModuleArgument(db, pTable, sqlite3DbStrDup(db, db->aDb[iDb].zName));
  addModuleArgument(db, pTable, sqlite3DbStrDup(db, pTable->zName));
  pParse->sNameToken.n = (int)(&pModuleName->z[pModuleName->n] - pName1->z);

#ifndef SQLITE_OMIT_AUTHORIZATION
  if( pTable->azModuleArg ){
    sqlite3AuthCheck(pParse, SQLITE_CREATE_VTABLE, pTable->zName, 
            pTable->azModuleArg[0], pParse->db->aDb[iDb].zName);
  }
#endif
}

static void addArgumentToVtab(Parse *pParse){
  if( pParse->sArg.z && ALWAYS(pParse->pNewTable) ){
    const char *z = (const char*)pParse->sArg.z;
    int n = pParse->sArg.n;
    sqlite3 *db = pParse->db;
    addModuleArgument(db, pParse->pNewTable, sqlite3DbStrNDup(db, z, n));
  }
}

void sqlite3VtabFinishParse(Parse *pParse, Token *pEnd){
  Table *pTab = pParse->pNewTable;  
  sqlite3 *db = pParse->db;         

  if( pTab==0 ) return;
  addArgumentToVtab(pParse);
  pParse->sArg.z = 0;
  if( pTab->nModuleArg<1 ) return;
  
  if( !db->init.busy ){
    char *zStmt;
    char *zWhere;
    int iDb;
    Vdbe *v;

    
    if( pEnd ){
      pParse->sNameToken.n = (int)(pEnd->z - pParse->sNameToken.z) + pEnd->n;
    }
    zStmt = sqlite3MPrintf(db, "CREATE VIRTUAL TABLE %T", &pParse->sNameToken);

    iDb = sqlite3SchemaToIndex(db, pTab->pSchema);
    sqlite3NestedParse(pParse,
      "UPDATE %Q.%s "
         "SET type='table', name=%Q, tbl_name=%Q, rootpage=0, sql=%Q "
       "WHERE rowid=#%d",
      db->aDb[iDb].zName, SCHEMA_TABLE(iDb),
      pTab->zName,
      pTab->zName,
      zStmt,
      pParse->regRowid
    );
    sqlite3DbFree(db, zStmt);
    v = sqlite3GetVdbe(pParse);
    sqlite3ChangeCookie(pParse, iDb);

    sqlite3VdbeAddOp2(v, OP_Expire, 0, 0);
    zWhere = sqlite3MPrintf(db, "name='%q' AND type='table'", pTab->zName);
    sqlite3VdbeAddOp4(v, OP_ParseSchema, iDb, 0, 0, zWhere, P4_DYNAMIC);
    sqlite3VdbeAddOp4(v, OP_VCreate, iDb, 0, 0, 
                         pTab->zName, sqlite3Strlen30(pTab->zName) + 1);
  }

  else {
    Table *pOld;
    Schema *pSchema = pTab->pSchema;
    const char *zName = pTab->zName;
    int nName = sqlite3Strlen30(zName);
    assert( sqlite3SchemaMutexHeld(db, 0, pSchema) );
    pOld = sqlite3HashInsert(&pSchema->tblHash, zName, nName, pTab);
    if( pOld ){
      db->mallocFailed = 1;
      assert( pTab==pOld );  
      return;
    }
    pParse->pNewTable = 0;
  }
}

void sqlite3VtabArgInit(Parse *pParse){
  addArgumentToVtab(pParse);
  pParse->sArg.z = 0;
  pParse->sArg.n = 0;
}

void sqlite3VtabArgExtend(Parse *pParse, Token *p){
  Token *pArg = &pParse->sArg;
  if( pArg->z==0 ){
    pArg->z = p->z;
    pArg->n = p->n;
  }else{
    assert(pArg->z < p->z);
    pArg->n = (int)(&p->z[p->n] - pArg->z);
  }
}

static int vtabCallConstructor(
  sqlite3 *db, 
  Table *pTab,
  Module *pMod,
  int (*xConstruct)(sqlite3*,void*,int,const char*const*,sqlite3_vtab**,char**),
  char **pzErr
){
  VTable *pVTable;
  int rc;
  const char *const*azArg = (const char *const*)pTab->azModuleArg;
  int nArg = pTab->nModuleArg;
  char *zErr = 0;
  char *zModuleName = sqlite3MPrintf(db, "%s", pTab->zName);

  if( !zModuleName ){
    return SQLITE_NOMEM;
  }

  pVTable = sqlite3DbMallocZero(db, sizeof(VTable));
  if( !pVTable ){
    sqlite3DbFree(db, zModuleName);
    return SQLITE_NOMEM;
  }
  pVTable->db = db;
  pVTable->pMod = pMod;

  assert( !db->pVTab );
  assert( xConstruct );
  db->pVTab = pTab;

  
  rc = xConstruct(db, pMod->pAux, nArg, azArg, &pVTable->pVtab, &zErr);
  if( rc==SQLITE_NOMEM ) db->mallocFailed = 1;

  if( SQLITE_OK!=rc ){
    if( zErr==0 ){
      *pzErr = sqlite3MPrintf(db, "vtable constructor failed: %s", zModuleName);
    }else {
      *pzErr = sqlite3MPrintf(db, "%s", zErr);
      sqlite3_free(zErr);
    }
    sqlite3DbFree(db, pVTable);
  }else if( ALWAYS(pVTable->pVtab) ){
    pVTable->pVtab->pModule = pMod->pModule;
    pVTable->nRef = 1;
    if( db->pVTab ){
      const char *zFormat = "vtable constructor did not declare schema: %s";
      *pzErr = sqlite3MPrintf(db, zFormat, pTab->zName);
      sqlite3VtabUnlock(pVTable);
      rc = SQLITE_ERROR;
    }else{
      int iCol;
      pVTable->pNext = pTab->pVTable;
      pTab->pVTable = pVTable;

      for(iCol=0; iCol<pTab->nCol; iCol++){
        char *zType = pTab->aCol[iCol].zType;
        int nType;
        int i = 0;
        if( !zType ) continue;
        nType = sqlite3Strlen30(zType);
        if( sqlite3StrNICmp("hidden", zType, 6)||(zType[6] && zType[6]!=' ') ){
          for(i=0; i<nType; i++){
            if( (0==sqlite3StrNICmp(" hidden", &zType[i], 7))
             && (zType[i+7]=='\0' || zType[i+7]==' ')
            ){
              i++;
              break;
            }
          }
        }
        if( i<nType ){
          int j;
          int nDel = 6 + (zType[i+6] ? 1 : 0);
          for(j=i; (j+nDel)<=nType; j++){
            zType[j] = zType[j+nDel];
          }
          if( zType[i]=='\0' && i>0 ){
            assert(zType[i-1]==' ');
            zType[i-1] = '\0';
          }
          pTab->aCol[iCol].isHidden = 1;
        }
      }
    }
  }

  sqlite3DbFree(db, zModuleName);
  db->pVTab = 0;
  return rc;
}

int sqlite3VtabCallConnect(Parse *pParse, Table *pTab){
  sqlite3 *db = pParse->db;
  const char *zMod;
  Module *pMod;
  int rc;

  assert( pTab );
  if( (pTab->tabFlags & TF_Virtual)==0 || sqlite3GetVTable(db, pTab) ){
    return SQLITE_OK;
  }

  
  zMod = pTab->azModuleArg[0];
  pMod = (Module*)sqlite3HashFind(&db->aModule, zMod, sqlite3Strlen30(zMod));

  if( !pMod ){
    const char *zModule = pTab->azModuleArg[0];
    sqlite3ErrorMsg(pParse, "no such module: %s", zModule);
    rc = SQLITE_ERROR;
  }else{
    char *zErr = 0;
    rc = vtabCallConstructor(db, pTab, pMod, pMod->pModule->xConnect, &zErr);
    if( rc!=SQLITE_OK ){
      sqlite3ErrorMsg(pParse, "%s", zErr);
    }
    sqlite3DbFree(db, zErr);
  }

  return rc;
}

static int addToVTrans(sqlite3 *db, VTable *pVTab){
  const int ARRAY_INCR = 5;

  
  if( (db->nVTrans%ARRAY_INCR)==0 ){
    VTable **aVTrans;
    int nBytes = sizeof(sqlite3_vtab *) * (db->nVTrans + ARRAY_INCR);
    aVTrans = sqlite3DbRealloc(db, (void *)db->aVTrans, nBytes);
    if( !aVTrans ){
      return SQLITE_NOMEM;
    }
    memset(&aVTrans[db->nVTrans], 0, sizeof(sqlite3_vtab *)*ARRAY_INCR);
    db->aVTrans = aVTrans;
  }

  
  db->aVTrans[db->nVTrans++] = pVTab;
  sqlite3VtabLock(pVTab);
  return SQLITE_OK;
}

int sqlite3VtabCallCreate(sqlite3 *db, int iDb, const char *zTab, char **pzErr){
  int rc = SQLITE_OK;
  Table *pTab;
  Module *pMod;
  const char *zMod;

  pTab = sqlite3FindTable(db, zTab, db->aDb[iDb].zName);
  assert( pTab && (pTab->tabFlags & TF_Virtual)!=0 && !pTab->pVTable );

  
  zMod = pTab->azModuleArg[0];
  pMod = (Module*)sqlite3HashFind(&db->aModule, zMod, sqlite3Strlen30(zMod));

  if( !pMod ){
    *pzErr = sqlite3MPrintf(db, "no such module: %s", zMod);
    rc = SQLITE_ERROR;
  }else{
    rc = vtabCallConstructor(db, pTab, pMod, pMod->pModule->xCreate, pzErr);
  }

  if( rc==SQLITE_OK && ALWAYS(sqlite3GetVTable(db, pTab)) ){
      rc = addToVTrans(db, sqlite3GetVTable(db, pTab));
  }

  return rc;
}

int sqlite3_declare_vtab(sqlite3 *db, const char *zCreateTable){
  Parse *pParse;

  int rc = SQLITE_OK;
  Table *pTab;
  char *zErr = 0;

  sqlite3_mutex_enter(db->mutex);
  pTab = db->pVTab;
  if( !pTab ){
    sqlite3Error(db, SQLITE_MISUSE, 0);
    sqlite3_mutex_leave(db->mutex);
    return SQLITE_MISUSE_BKPT;
  }
  assert( (pTab->tabFlags & TF_Virtual)!=0 );

  pParse = sqlite3StackAllocZero(db, sizeof(*pParse));
  if( pParse==0 ){
    rc = SQLITE_NOMEM;
  }else{
    pParse->declareVtab = 1;
    pParse->db = db;
    pParse->nQueryLoop = 1;
  
    if( SQLITE_OK==sqlite3RunParser(pParse, zCreateTable, &zErr) 
     && pParse->pNewTable
     && !db->mallocFailed
     && !pParse->pNewTable->pSelect
     && (pParse->pNewTable->tabFlags & TF_Virtual)==0
    ){
      if( !pTab->aCol ){
        pTab->aCol = pParse->pNewTable->aCol;
        pTab->nCol = pParse->pNewTable->nCol;
        pParse->pNewTable->nCol = 0;
        pParse->pNewTable->aCol = 0;
      }
      db->pVTab = 0;
    }else{
      sqlite3Error(db, SQLITE_ERROR, (zErr ? "%s" : 0), zErr);
      sqlite3DbFree(db, zErr);
      rc = SQLITE_ERROR;
    }
    pParse->declareVtab = 0;
  
    if( pParse->pVdbe ){
      sqlite3VdbeFinalize(pParse->pVdbe);
    }
    sqlite3DeleteTable(db, pParse->pNewTable);
    sqlite3StackFree(db, pParse);
  }

  assert( (rc&0xff)==rc );
  rc = sqlite3ApiExit(db, rc);
  sqlite3_mutex_leave(db->mutex);
  return rc;
}

int sqlite3VtabCallDestroy(sqlite3 *db, int iDb, const char *zTab){
  int rc = SQLITE_OK;
  Table *pTab;

  pTab = sqlite3FindTable(db, zTab, db->aDb[iDb].zName);
  if( ALWAYS(pTab!=0 && pTab->pVTable!=0) ){
    VTable *p = vtabDisconnectAll(db, pTab);

    assert( rc==SQLITE_OK );
    rc = p->pMod->pModule->xDestroy(p->pVtab);

    
    if( rc==SQLITE_OK ){
      assert( pTab->pVTable==p && p->pNext==0 );
      p->pVtab = 0;
      pTab->pVTable = 0;
      sqlite3VtabUnlock(p);
    }
  }

  return rc;
}

static void callFinaliser(sqlite3 *db, int offset){
  int i;
  if( db->aVTrans ){
    for(i=0; i<db->nVTrans; i++){
      VTable *pVTab = db->aVTrans[i];
      sqlite3_vtab *p = pVTab->pVtab;
      if( p ){
        int (*x)(sqlite3_vtab *);
        x = *(int (**)(sqlite3_vtab *))((char *)p->pModule + offset);
        if( x ) x(p);
      }
      sqlite3VtabUnlock(pVTab);
    }
    sqlite3DbFree(db, db->aVTrans);
    db->nVTrans = 0;
    db->aVTrans = 0;
  }
}

int sqlite3VtabSync(sqlite3 *db, char **pzErrmsg){
  int i;
  int rc = SQLITE_OK;
  VTable **aVTrans = db->aVTrans;

  db->aVTrans = 0;
  for(i=0; rc==SQLITE_OK && i<db->nVTrans; i++){
    int (*x)(sqlite3_vtab *);
    sqlite3_vtab *pVtab = aVTrans[i]->pVtab;
    if( pVtab && (x = pVtab->pModule->xSync)!=0 ){
      rc = x(pVtab);
      sqlite3DbFree(db, *pzErrmsg);
      *pzErrmsg = sqlite3DbStrDup(db, pVtab->zErrMsg);
      sqlite3_free(pVtab->zErrMsg);
    }
  }
  db->aVTrans = aVTrans;
  return rc;
}

int sqlite3VtabRollback(sqlite3 *db){
  callFinaliser(db, offsetof(sqlite3_module,xRollback));
  return SQLITE_OK;
}

int sqlite3VtabCommit(sqlite3 *db){
  callFinaliser(db, offsetof(sqlite3_module,xCommit));
  return SQLITE_OK;
}

int sqlite3VtabBegin(sqlite3 *db, VTable *pVTab){
  int rc = SQLITE_OK;
  const sqlite3_module *pModule;

  if( sqlite3VtabInSync(db) ){
    return SQLITE_LOCKED;
  }
  if( !pVTab ){
    return SQLITE_OK;
  } 
  pModule = pVTab->pVtab->pModule;

  if( pModule->xBegin ){
    int i;


    
    for(i=0; i<db->nVTrans; i++){
      if( db->aVTrans[i]==pVTab ){
        return SQLITE_OK;
      }
    }

    
    rc = pModule->xBegin(pVTab->pVtab);
    if( rc==SQLITE_OK ){
      rc = addToVTrans(db, pVTab);
    }
  }
  return rc;
}

FuncDef *sqlite3VtabOverloadFunction(
  sqlite3 *db,    
  FuncDef *pDef,  
  int nArg,       
  Expr *pExpr     
){
  Table *pTab;
  sqlite3_vtab *pVtab;
  sqlite3_module *pMod;
  void (*xFunc)(sqlite3_context*,int,sqlite3_value**) = 0;
  void *pArg = 0;
  FuncDef *pNew;
  int rc = 0;
  char *zLowerName;
  unsigned char *z;


  
  if( NEVER(pExpr==0) ) return pDef;
  if( pExpr->op!=TK_COLUMN ) return pDef;
  pTab = pExpr->pTab;
  if( NEVER(pTab==0) ) return pDef;
  if( (pTab->tabFlags & TF_Virtual)==0 ) return pDef;
  pVtab = sqlite3GetVTable(db, pTab)->pVtab;
  assert( pVtab!=0 );
  assert( pVtab->pModule!=0 );
  pMod = (sqlite3_module *)pVtab->pModule;
  if( pMod->xFindFunction==0 ) return pDef;
 
  zLowerName = sqlite3DbStrDup(db, pDef->zName);
  if( zLowerName ){
    for(z=(unsigned char*)zLowerName; *z; z++){
      *z = sqlite3UpperToLower[*z];
    }
    rc = pMod->xFindFunction(pVtab, nArg, zLowerName, &xFunc, &pArg);
    sqlite3DbFree(db, zLowerName);
  }
  if( rc==0 ){
    return pDef;
  }

  pNew = sqlite3DbMallocZero(db, sizeof(*pNew)
                             + sqlite3Strlen30(pDef->zName) + 1);
  if( pNew==0 ){
    return pDef;
  }
  *pNew = *pDef;
  pNew->zName = (char *)&pNew[1];
  memcpy(pNew->zName, pDef->zName, sqlite3Strlen30(pDef->zName)+1);
  pNew->xFunc = xFunc;
  pNew->pUserData = pArg;
  pNew->flags |= SQLITE_FUNC_EPHEM;
  return pNew;
}

void sqlite3VtabMakeWritable(Parse *pParse, Table *pTab){
  Parse *pToplevel = sqlite3ParseToplevel(pParse);
  int i, n;
  Table **apVtabLock;

  assert( IsVirtual(pTab) );
  for(i=0; i<pToplevel->nVtabLock; i++){
    if( pTab==pToplevel->apVtabLock[i] ) return;
  }
  n = (pToplevel->nVtabLock+1)*sizeof(pToplevel->apVtabLock[0]);
  apVtabLock = sqlite3_realloc(pToplevel->apVtabLock, n);
  if( apVtabLock ){
    pToplevel->apVtabLock = apVtabLock;
    pToplevel->apVtabLock[pToplevel->nVtabLock++] = pTab;
  }else{
    pToplevel->db->mallocFailed = 1;
  }
}

#endif 

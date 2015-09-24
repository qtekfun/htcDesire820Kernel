/*
** 2003 April 6
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This file contains code used to implement the ATTACH and DETACH commands.
*/
#include "sqliteInt.h"

#ifndef SQLITE_OMIT_ATTACH
static int resolveAttachExpr(NameContext *pName, Expr *pExpr)
{
  int rc = SQLITE_OK;
  if( pExpr ){
    if( pExpr->op!=TK_ID ){
      rc = sqlite3ResolveExprNames(pName, pExpr);
      if( rc==SQLITE_OK && !sqlite3ExprIsConstant(pExpr) ){
        sqlite3ErrorMsg(pName->pParse, "invalid name: \"%s\"", pExpr->u.zToken);
        return SQLITE_ERROR;
      }
    }else{
      pExpr->op = TK_STRING;
    }
  }
  return rc;
}

static void attachFunc(
  sqlite3_context *context,
  int NotUsed,
  sqlite3_value **argv
){
  int i;
  int rc = 0;
  sqlite3 *db = sqlite3_context_db_handle(context);
  const char *zName;
  const char *zFile;
  Db *aNew;
  char *zErrDyn = 0;

  UNUSED_PARAMETER(NotUsed);

  zFile = (const char *)sqlite3_value_text(argv[0]);
  zName = (const char *)sqlite3_value_text(argv[1]);
  if( zFile==0 ) zFile = "";
  if( zName==0 ) zName = "";

  if( db->nDb>=db->aLimit[SQLITE_LIMIT_ATTACHED]+2 ){
    zErrDyn = sqlite3MPrintf(db, "too many attached databases - max %d", 
      db->aLimit[SQLITE_LIMIT_ATTACHED]
    );
    goto attach_error;
  }
  if( !db->autoCommit ){
    zErrDyn = sqlite3MPrintf(db, "cannot ATTACH database within transaction");
    goto attach_error;
  }
  for(i=0; i<db->nDb; i++){
    char *z = db->aDb[i].zName;
    assert( z && zName );
    if( sqlite3StrICmp(z, zName)==0 ){
      zErrDyn = sqlite3MPrintf(db, "database %s is already in use", zName);
      goto attach_error;
    }
  }

  if( db->aDb==db->aDbStatic ){
    aNew = sqlite3DbMallocRaw(db, sizeof(db->aDb[0])*3 );
    if( aNew==0 ) return;
    memcpy(aNew, db->aDb, sizeof(db->aDb[0])*2);
  }else{
    aNew = sqlite3DbRealloc(db, db->aDb, sizeof(db->aDb[0])*(db->nDb+1) );
    if( aNew==0 ) return;
  }
  db->aDb = aNew;
  aNew = &db->aDb[db->nDb];
  memset(aNew, 0, sizeof(*aNew));

  rc = sqlite3BtreeOpen(zFile, db, &aNew->pBt, 0,
                        db->openFlags | SQLITE_OPEN_MAIN_DB);
  db->nDb++;
  if( rc==SQLITE_CONSTRAINT ){
    rc = SQLITE_ERROR;
    zErrDyn = sqlite3MPrintf(db, "database is already attached");
  }else if( rc==SQLITE_OK ){
    Pager *pPager;
    aNew->pSchema = sqlite3SchemaGet(db, aNew->pBt);
    if( !aNew->pSchema ){
      rc = SQLITE_NOMEM;
    }else if( aNew->pSchema->file_format && aNew->pSchema->enc!=ENC(db) ){
      zErrDyn = sqlite3MPrintf(db, 
        "attached databases must use the same text encoding as main database");
      rc = SQLITE_ERROR;
    }
    pPager = sqlite3BtreePager(aNew->pBt);
    sqlite3PagerLockingMode(pPager, db->dfltLockMode);
    sqlite3BtreeSecureDelete(aNew->pBt,
                             sqlite3BtreeSecureDelete(db->aDb[0].pBt,-1) );
  }
  aNew->safety_level = 3;
  aNew->zName = sqlite3DbStrDup(db, zName);
  if( rc==SQLITE_OK && aNew->zName==0 ){
    rc = SQLITE_NOMEM;
  }


#ifdef SQLITE_HAS_CODEC
  if( rc==SQLITE_OK ){
    extern int sqlite3CodecAttach(sqlite3*, int, const void*, int);
    extern void sqlite3CodecGetKey(sqlite3*, int, void**, int*);
    int nKey;
    char *zKey;
    int t = sqlite3_value_type(argv[2]);
    switch( t ){
      case SQLITE_INTEGER:
      case SQLITE_FLOAT:
        zErrDyn = sqlite3DbStrDup(db, "Invalid key value");
        rc = SQLITE_ERROR;
        break;
        
      case SQLITE_TEXT:
      case SQLITE_BLOB:
        nKey = sqlite3_value_bytes(argv[2]);
        zKey = (char *)sqlite3_value_blob(argv[2]);
        rc = sqlite3CodecAttach(db, db->nDb-1, zKey, nKey);
        break;

      case SQLITE_NULL:
        
        sqlite3CodecGetKey(db, 0, (void**)&zKey, &nKey);
        if( nKey>0 || sqlite3BtreeGetReserve(db->aDb[0].pBt)>0 ){
          rc = sqlite3CodecAttach(db, db->nDb-1, zKey, nKey);
        }
        break;
    }
  }
#endif

  if( rc==SQLITE_OK ){
    sqlite3BtreeEnterAll(db);
    rc = sqlite3Init(db, &zErrDyn);
    sqlite3BtreeLeaveAll(db);
  }
  if( rc ){
    int iDb = db->nDb - 1;
    assert( iDb>=2 );
    if( db->aDb[iDb].pBt ){
      sqlite3BtreeClose(db->aDb[iDb].pBt);
      db->aDb[iDb].pBt = 0;
      db->aDb[iDb].pSchema = 0;
    }
    sqlite3ResetInternalSchema(db, -1);
    db->nDb = iDb;
    if( rc==SQLITE_NOMEM || rc==SQLITE_IOERR_NOMEM ){
      db->mallocFailed = 1;
      sqlite3DbFree(db, zErrDyn);
      zErrDyn = sqlite3MPrintf(db, "out of memory");
    }else if( zErrDyn==0 ){
      zErrDyn = sqlite3MPrintf(db, "unable to open database: %s", zFile);
    }
    goto attach_error;
  }
  
  return;

attach_error:
  
  if( zErrDyn ){
    sqlite3_result_error(context, zErrDyn, -1);
    sqlite3DbFree(db, zErrDyn);
  }
  if( rc ) sqlite3_result_error_code(context, rc);
}

static void detachFunc(
  sqlite3_context *context,
  int NotUsed,
  sqlite3_value **argv
){
  const char *zName = (const char *)sqlite3_value_text(argv[0]);
  sqlite3 *db = sqlite3_context_db_handle(context);
  int i;
  Db *pDb = 0;
  char zErr[128];

  UNUSED_PARAMETER(NotUsed);

  if( zName==0 ) zName = "";
  for(i=0; i<db->nDb; i++){
    pDb = &db->aDb[i];
    if( pDb->pBt==0 ) continue;
    if( sqlite3StrICmp(pDb->zName, zName)==0 ) break;
  }

  if( i>=db->nDb ){
    sqlite3_snprintf(sizeof(zErr),zErr, "no such database: %s", zName);
    goto detach_error;
  }
  if( i<2 ){
    sqlite3_snprintf(sizeof(zErr),zErr, "cannot detach database %s", zName);
    goto detach_error;
  }
  if( !db->autoCommit ){
    sqlite3_snprintf(sizeof(zErr), zErr,
                     "cannot DETACH database within transaction");
    goto detach_error;
  }
  if( sqlite3BtreeIsInReadTrans(pDb->pBt) || sqlite3BtreeIsInBackup(pDb->pBt) ){
    sqlite3_snprintf(sizeof(zErr),zErr, "database %s is locked", zName);
    goto detach_error;
  }

  sqlite3BtreeClose(pDb->pBt);
  pDb->pBt = 0;
  pDb->pSchema = 0;
  sqlite3ResetInternalSchema(db, -1);
  return;

detach_error:
  sqlite3_result_error(context, zErr, -1);
}

static void codeAttach(
  Parse *pParse,       
  int type,            
  FuncDef const *pFunc,
  Expr *pAuthArg,      
  Expr *pFilename,     
  Expr *pDbname,       
  Expr *pKey           
){
  int rc;
  NameContext sName;
  Vdbe *v;
  sqlite3* db = pParse->db;
  int regArgs;

  memset(&sName, 0, sizeof(NameContext));
  sName.pParse = pParse;

  if( 
      SQLITE_OK!=(rc = resolveAttachExpr(&sName, pFilename)) ||
      SQLITE_OK!=(rc = resolveAttachExpr(&sName, pDbname)) ||
      SQLITE_OK!=(rc = resolveAttachExpr(&sName, pKey))
  ){
    pParse->nErr++;
    goto attach_end;
  }

#ifndef SQLITE_OMIT_AUTHORIZATION
  if( pAuthArg ){
    char *zAuthArg;
    if( pAuthArg->op==TK_STRING ){
      zAuthArg = pAuthArg->u.zToken;
    }else{
      zAuthArg = 0;
    }
    rc = sqlite3AuthCheck(pParse, type, zAuthArg, 0, 0);
    if(rc!=SQLITE_OK ){
      goto attach_end;
    }
  }
#endif 


  v = sqlite3GetVdbe(pParse);
  regArgs = sqlite3GetTempRange(pParse, 4);
  sqlite3ExprCode(pParse, pFilename, regArgs);
  sqlite3ExprCode(pParse, pDbname, regArgs+1);
  sqlite3ExprCode(pParse, pKey, regArgs+2);

  assert( v || db->mallocFailed );
  if( v ){
    sqlite3VdbeAddOp3(v, OP_Function, 0, regArgs+3-pFunc->nArg, regArgs+3);
    assert( pFunc->nArg==-1 || (pFunc->nArg&0xff)==pFunc->nArg );
    sqlite3VdbeChangeP5(v, (u8)(pFunc->nArg));
    sqlite3VdbeChangeP4(v, -1, (char *)pFunc, P4_FUNCDEF);

    sqlite3VdbeAddOp1(v, OP_Expire, (type==SQLITE_ATTACH));
  }
  
attach_end:
  sqlite3ExprDelete(db, pFilename);
  sqlite3ExprDelete(db, pDbname);
  sqlite3ExprDelete(db, pKey);
}

void sqlite3Detach(Parse *pParse, Expr *pDbname){
  static const FuncDef detach_func = {
    1,                
    SQLITE_UTF8,      
    0,                
    0,                
    0,                
    detachFunc,       
    0,                
    0,                
    "sqlite_detach",  
    0,                
    0                 
  };
  codeAttach(pParse, SQLITE_DETACH, &detach_func, pDbname, 0, 0, pDbname);
}

void sqlite3Attach(Parse *pParse, Expr *p, Expr *pDbname, Expr *pKey){
  static const FuncDef attach_func = {
    3,                
    SQLITE_UTF8,      
    0,                
    0,                
    0,                
    attachFunc,       
    0,                
    0,                
    "sqlite_attach",  
    0,                
    0                 
  };
  codeAttach(pParse, SQLITE_ATTACH, &attach_func, p, p, pDbname, pKey);
}
#endif 

int sqlite3FixInit(
  DbFixer *pFix,      
  Parse *pParse,      /* Error messages will be written here */
  int iDb,            
  const char *zType,  
  const Token *pName  
){
  sqlite3 *db;

  if( NEVER(iDb<0) || iDb==1 ) return 0;
  db = pParse->db;
  assert( db->nDb>iDb );
  pFix->pParse = pParse;
  pFix->zDb = db->aDb[iDb].zName;
  pFix->zType = zType;
  pFix->pName = pName;
  return 1;
}

int sqlite3FixSrcList(
  DbFixer *pFix,       
  SrcList *pList       
){
  int i;
  const char *zDb;
  struct SrcList_item *pItem;

  if( NEVER(pList==0) ) return 0;
  zDb = pFix->zDb;
  for(i=0, pItem=pList->a; i<pList->nSrc; i++, pItem++){
    if( pItem->zDatabase==0 ){
      pItem->zDatabase = sqlite3DbStrDup(pFix->pParse->db, zDb);
    }else if( sqlite3StrICmp(pItem->zDatabase,zDb)!=0 ){
      sqlite3ErrorMsg(pFix->pParse,
         "%s %T cannot reference objects in database %s",
         pFix->zType, pFix->pName, pItem->zDatabase);
      return 1;
    }
#if !defined(SQLITE_OMIT_VIEW) || !defined(SQLITE_OMIT_TRIGGER)
    if( sqlite3FixSelect(pFix, pItem->pSelect) ) return 1;
    if( sqlite3FixExpr(pFix, pItem->pOn) ) return 1;
#endif
  }
  return 0;
}
#if !defined(SQLITE_OMIT_VIEW) || !defined(SQLITE_OMIT_TRIGGER)
int sqlite3FixSelect(
  DbFixer *pFix,       
  Select *pSelect      
){
  while( pSelect ){
    if( sqlite3FixExprList(pFix, pSelect->pEList) ){
      return 1;
    }
    if( sqlite3FixSrcList(pFix, pSelect->pSrc) ){
      return 1;
    }
    if( sqlite3FixExpr(pFix, pSelect->pWhere) ){
      return 1;
    }
    if( sqlite3FixExpr(pFix, pSelect->pHaving) ){
      return 1;
    }
    pSelect = pSelect->pPrior;
  }
  return 0;
}
int sqlite3FixExpr(
  DbFixer *pFix,     
  Expr *pExpr        
){
  while( pExpr ){
    if( ExprHasAnyProperty(pExpr, EP_TokenOnly) ) break;
    if( ExprHasProperty(pExpr, EP_xIsSelect) ){
      if( sqlite3FixSelect(pFix, pExpr->x.pSelect) ) return 1;
    }else{
      if( sqlite3FixExprList(pFix, pExpr->x.pList) ) return 1;
    }
    if( sqlite3FixExpr(pFix, pExpr->pRight) ){
      return 1;
    }
    pExpr = pExpr->pLeft;
  }
  return 0;
}
int sqlite3FixExprList(
  DbFixer *pFix,     
  ExprList *pList    
){
  int i;
  struct ExprList_item *pItem;
  if( pList==0 ) return 0;
  for(i=0, pItem=pList->a; i<pList->nExpr; i++, pItem++){
    if( sqlite3FixExpr(pFix, pItem->pExpr) ){
      return 1;
    }
  }
  return 0;
}
#endif

#ifndef SQLITE_OMIT_TRIGGER
int sqlite3FixTriggerStep(
  DbFixer *pFix,     
  TriggerStep *pStep 
){
  while( pStep ){
    if( sqlite3FixSelect(pFix, pStep->pSelect) ){
      return 1;
    }
    if( sqlite3FixExpr(pFix, pStep->pWhere) ){
      return 1;
    }
    if( sqlite3FixExprList(pFix, pStep->pExprList) ){
      return 1;
    }
    pStep = pStep->pNext;
  }
  return 0;
}
#endif

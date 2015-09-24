/*
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This file contains the implementation for TRIGGERs
*/
#include "sqliteInt.h"

#ifndef SQLITE_OMIT_TRIGGER
void sqlite3DeleteTriggerStep(sqlite3 *db, TriggerStep *pTriggerStep){
  while( pTriggerStep ){
    TriggerStep * pTmp = pTriggerStep;
    pTriggerStep = pTriggerStep->pNext;

    sqlite3ExprDelete(db, pTmp->pWhere);
    sqlite3ExprListDelete(db, pTmp->pExprList);
    sqlite3SelectDelete(db, pTmp->pSelect);
    sqlite3IdListDelete(db, pTmp->pIdList);

    sqlite3DbFree(db, pTmp);
  }
}

Trigger *sqlite3TriggerList(Parse *pParse, Table *pTab){
  Schema * const pTmpSchema = pParse->db->aDb[1].pSchema;
  Trigger *pList = 0;                  

  if( pParse->disableTriggers ){
    return 0;
  }

  if( pTmpSchema!=pTab->pSchema ){
    HashElem *p;
    assert( sqlite3SchemaMutexHeld(pParse->db, 0, pTmpSchema) );
    for(p=sqliteHashFirst(&pTmpSchema->trigHash); p; p=sqliteHashNext(p)){
      Trigger *pTrig = (Trigger *)sqliteHashData(p);
      if( pTrig->pTabSchema==pTab->pSchema
       && 0==sqlite3StrICmp(pTrig->table, pTab->zName) 
      ){
        pTrig->pNext = (pList ? pList : pTab->pTrigger);
        pList = pTrig;
      }
    }
  }

  return (pList ? pList : pTab->pTrigger);
}

void sqlite3BeginTrigger(
  Parse *pParse,      
  Token *pName1,      
  Token *pName2,      
  int tr_tm,          
  int op,             
  IdList *pColumns,   
  SrcList *pTableName,
  Expr *pWhen,        
  int isTemp,         
  int noErr           
){
  Trigger *pTrigger = 0;  
  Table *pTab;            
  char *zName = 0;        
  sqlite3 *db = pParse->db;  
  int iDb;                
  Token *pName;           
  DbFixer sFix;           
  int iTabDb;             

  assert( pName1!=0 );   
  assert( pName2!=0 );
  assert( op==TK_INSERT || op==TK_UPDATE || op==TK_DELETE );
  assert( op>0 && op<0xff );
  if( isTemp ){
    
    if( pName2->n>0 ){
      sqlite3ErrorMsg(pParse, "temporary trigger may not have qualified name");
      goto trigger_cleanup;
    }
    iDb = 1;
    pName = pName1;
  }else{
    
    iDb = sqlite3TwoPartName(pParse, pName1, pName2, &pName);
    if( iDb<0 ){
      goto trigger_cleanup;
    }
  }

  if( !pTableName || db->mallocFailed ){
    goto trigger_cleanup;
  }
  pTab = sqlite3SrcListLookup(pParse, pTableName);
  if( db->init.busy==0 && pName2->n==0 && pTab
        && pTab->pSchema==db->aDb[1].pSchema ){
    iDb = 1;
  }

  
  if( db->mallocFailed ) goto trigger_cleanup;
  assert( pTableName->nSrc==1 );
  if( sqlite3FixInit(&sFix, pParse, iDb, "trigger", pName) && 
      sqlite3FixSrcList(&sFix, pTableName) ){
    goto trigger_cleanup;
  }
  pTab = sqlite3SrcListLookup(pParse, pTableName);
  if( !pTab ){
    
    if( db->init.iDb==1 ){
      db->init.orphanTrigger = 1;
    }
    goto trigger_cleanup;
  }
  if( IsVirtual(pTab) ){
    sqlite3ErrorMsg(pParse, "cannot create triggers on virtual tables");
    goto trigger_cleanup;
  }

  zName = sqlite3NameFromToken(db, pName);
  if( !zName || SQLITE_OK!=sqlite3CheckObjectName(pParse, zName) ){
    goto trigger_cleanup;
  }
  assert( sqlite3SchemaMutexHeld(db, iDb, 0) );
  if( sqlite3HashFind(&(db->aDb[iDb].pSchema->trigHash),
                      zName, sqlite3Strlen30(zName)) ){
    if( !noErr ){
      sqlite3ErrorMsg(pParse, "trigger %T already exists", pName);
    }else{
      assert( !db->init.busy );
      sqlite3CodeVerifySchema(pParse, iDb);
    }
    goto trigger_cleanup;
  }

  
  if( sqlite3StrNICmp(pTab->zName, "sqlite_", 7)==0 ){
    sqlite3ErrorMsg(pParse, "cannot create trigger on system table");
    pParse->nErr++;
    goto trigger_cleanup;
  }

  if( pTab->pSelect && tr_tm!=TK_INSTEAD ){
    sqlite3ErrorMsg(pParse, "cannot create %s trigger on view: %S", 
        (tr_tm == TK_BEFORE)?"BEFORE":"AFTER", pTableName, 0);
    goto trigger_cleanup;
  }
  if( !pTab->pSelect && tr_tm==TK_INSTEAD ){
    sqlite3ErrorMsg(pParse, "cannot create INSTEAD OF"
        " trigger on table: %S", pTableName, 0);
    goto trigger_cleanup;
  }
  iTabDb = sqlite3SchemaToIndex(db, pTab->pSchema);

#ifndef SQLITE_OMIT_AUTHORIZATION
  {
    int code = SQLITE_CREATE_TRIGGER;
    const char *zDb = db->aDb[iTabDb].zName;
    const char *zDbTrig = isTemp ? db->aDb[1].zName : zDb;
    if( iTabDb==1 || isTemp ) code = SQLITE_CREATE_TEMP_TRIGGER;
    if( sqlite3AuthCheck(pParse, code, zName, pTab->zName, zDbTrig) ){
      goto trigger_cleanup;
    }
    if( sqlite3AuthCheck(pParse, SQLITE_INSERT, SCHEMA_TABLE(iTabDb),0,zDb)){
      goto trigger_cleanup;
    }
  }
#endif

  if (tr_tm == TK_INSTEAD){
    tr_tm = TK_BEFORE;
  }

  
  pTrigger = (Trigger*)sqlite3DbMallocZero(db, sizeof(Trigger));
  if( pTrigger==0 ) goto trigger_cleanup;
  pTrigger->zName = zName;
  zName = 0;
  pTrigger->table = sqlite3DbStrDup(db, pTableName->a[0].zName);
  pTrigger->pSchema = db->aDb[iDb].pSchema;
  pTrigger->pTabSchema = pTab->pSchema;
  pTrigger->op = (u8)op;
  pTrigger->tr_tm = tr_tm==TK_BEFORE ? TRIGGER_BEFORE : TRIGGER_AFTER;
  pTrigger->pWhen = sqlite3ExprDup(db, pWhen, EXPRDUP_REDUCE);
  pTrigger->pColumns = sqlite3IdListDup(db, pColumns);
  assert( pParse->pNewTrigger==0 );
  pParse->pNewTrigger = pTrigger;

trigger_cleanup:
  sqlite3DbFree(db, zName);
  sqlite3SrcListDelete(db, pTableName);
  sqlite3IdListDelete(db, pColumns);
  sqlite3ExprDelete(db, pWhen);
  if( !pParse->pNewTrigger ){
    sqlite3DeleteTrigger(db, pTrigger);
  }else{
    assert( pParse->pNewTrigger==pTrigger );
  }
}

void sqlite3FinishTrigger(
  Parse *pParse,          
  TriggerStep *pStepList, 
  Token *pAll             
){
  Trigger *pTrig = pParse->pNewTrigger;   
  char *zName;                            
  sqlite3 *db = pParse->db;               
  DbFixer sFix;                           
  int iDb;                                
  Token nameToken;                        

  pParse->pNewTrigger = 0;
  if( NEVER(pParse->nErr) || !pTrig ) goto triggerfinish_cleanup;
  zName = pTrig->zName;
  iDb = sqlite3SchemaToIndex(pParse->db, pTrig->pSchema);
  pTrig->step_list = pStepList;
  while( pStepList ){
    pStepList->pTrig = pTrig;
    pStepList = pStepList->pNext;
  }
  nameToken.z = pTrig->zName;
  nameToken.n = sqlite3Strlen30(nameToken.z);
  if( sqlite3FixInit(&sFix, pParse, iDb, "trigger", &nameToken) 
          && sqlite3FixTriggerStep(&sFix, pTrig->step_list) ){
    goto triggerfinish_cleanup;
  }

  if( !db->init.busy ){
    Vdbe *v;
    char *z;

    
    v = sqlite3GetVdbe(pParse);
    if( v==0 ) goto triggerfinish_cleanup;
    sqlite3BeginWriteOperation(pParse, 0, iDb);
    z = sqlite3DbStrNDup(db, (char*)pAll->z, pAll->n);
    sqlite3NestedParse(pParse,
       "INSERT INTO %Q.%s VALUES('trigger',%Q,%Q,0,'CREATE TRIGGER %q')",
       db->aDb[iDb].zName, SCHEMA_TABLE(iDb), zName,
       pTrig->table, z);
    sqlite3DbFree(db, z);
    sqlite3ChangeCookie(pParse, iDb);
    sqlite3VdbeAddOp4(v, OP_ParseSchema, iDb, 0, 0, sqlite3MPrintf(
        db, "type='trigger' AND name='%q'", zName), P4_DYNAMIC
    );
  }

  if( db->init.busy ){
    Trigger *pLink = pTrig;
    Hash *pHash = &db->aDb[iDb].pSchema->trigHash;
    assert( sqlite3SchemaMutexHeld(db, iDb, 0) );
    pTrig = sqlite3HashInsert(pHash, zName, sqlite3Strlen30(zName), pTrig);
    if( pTrig ){
      db->mallocFailed = 1;
    }else if( pLink->pSchema==pLink->pTabSchema ){
      Table *pTab;
      int n = sqlite3Strlen30(pLink->table);
      pTab = sqlite3HashFind(&pLink->pTabSchema->tblHash, pLink->table, n);
      assert( pTab!=0 );
      pLink->pNext = pTab->pTrigger;
      pTab->pTrigger = pLink;
    }
  }

triggerfinish_cleanup:
  sqlite3DeleteTrigger(db, pTrig);
  assert( !pParse->pNewTrigger );
  sqlite3DeleteTriggerStep(db, pStepList);
}

TriggerStep *sqlite3TriggerSelectStep(sqlite3 *db, Select *pSelect){
  TriggerStep *pTriggerStep = sqlite3DbMallocZero(db, sizeof(TriggerStep));
  if( pTriggerStep==0 ) {
    sqlite3SelectDelete(db, pSelect);
    return 0;
  }
  pTriggerStep->op = TK_SELECT;
  pTriggerStep->pSelect = pSelect;
  pTriggerStep->orconf = OE_Default;
  return pTriggerStep;
}

static TriggerStep *triggerStepAllocate(
  sqlite3 *db,                
  u8 op,                      
  Token *pName                
){
  TriggerStep *pTriggerStep;

  pTriggerStep = sqlite3DbMallocZero(db, sizeof(TriggerStep) + pName->n);
  if( pTriggerStep ){
    char *z = (char*)&pTriggerStep[1];
    memcpy(z, pName->z, pName->n);
    pTriggerStep->target.z = z;
    pTriggerStep->target.n = pName->n;
    pTriggerStep->op = op;
  }
  return pTriggerStep;
}

TriggerStep *sqlite3TriggerInsertStep(
  sqlite3 *db,        
  Token *pTableName,  
  IdList *pColumn,    
  ExprList *pEList,   
  Select *pSelect,    
  u8 orconf           
){
  TriggerStep *pTriggerStep;

  assert(pEList == 0 || pSelect == 0);
  assert(pEList != 0 || pSelect != 0 || db->mallocFailed);

  pTriggerStep = triggerStepAllocate(db, TK_INSERT, pTableName);
  if( pTriggerStep ){
    pTriggerStep->pSelect = sqlite3SelectDup(db, pSelect, EXPRDUP_REDUCE);
    pTriggerStep->pIdList = pColumn;
    pTriggerStep->pExprList = sqlite3ExprListDup(db, pEList, EXPRDUP_REDUCE);
    pTriggerStep->orconf = orconf;
  }else{
    sqlite3IdListDelete(db, pColumn);
  }
  sqlite3ExprListDelete(db, pEList);
  sqlite3SelectDelete(db, pSelect);

  return pTriggerStep;
}

TriggerStep *sqlite3TriggerUpdateStep(
  sqlite3 *db,         
  Token *pTableName,   
  ExprList *pEList,    
  Expr *pWhere,        
  u8 orconf            
){
  TriggerStep *pTriggerStep;

  pTriggerStep = triggerStepAllocate(db, TK_UPDATE, pTableName);
  if( pTriggerStep ){
    pTriggerStep->pExprList = sqlite3ExprListDup(db, pEList, EXPRDUP_REDUCE);
    pTriggerStep->pWhere = sqlite3ExprDup(db, pWhere, EXPRDUP_REDUCE);
    pTriggerStep->orconf = orconf;
  }
  sqlite3ExprListDelete(db, pEList);
  sqlite3ExprDelete(db, pWhere);
  return pTriggerStep;
}

TriggerStep *sqlite3TriggerDeleteStep(
  sqlite3 *db,            
  Token *pTableName,      
  Expr *pWhere            
){
  TriggerStep *pTriggerStep;

  pTriggerStep = triggerStepAllocate(db, TK_DELETE, pTableName);
  if( pTriggerStep ){
    pTriggerStep->pWhere = sqlite3ExprDup(db, pWhere, EXPRDUP_REDUCE);
    pTriggerStep->orconf = OE_Default;
  }
  sqlite3ExprDelete(db, pWhere);
  return pTriggerStep;
}

void sqlite3DeleteTrigger(sqlite3 *db, Trigger *pTrigger){
  if( pTrigger==0 ) return;
  sqlite3DeleteTriggerStep(db, pTrigger->step_list);
  sqlite3DbFree(db, pTrigger->zName);
  sqlite3DbFree(db, pTrigger->table);
  sqlite3ExprDelete(db, pTrigger->pWhen);
  sqlite3IdListDelete(db, pTrigger->pColumns);
  sqlite3DbFree(db, pTrigger);
}

void sqlite3DropTrigger(Parse *pParse, SrcList *pName, int noErr){
  Trigger *pTrigger = 0;
  int i;
  const char *zDb;
  const char *zName;
  int nName;
  sqlite3 *db = pParse->db;

  if( db->mallocFailed ) goto drop_trigger_cleanup;
  if( SQLITE_OK!=sqlite3ReadSchema(pParse) ){
    goto drop_trigger_cleanup;
  }

  assert( pName->nSrc==1 );
  zDb = pName->a[0].zDatabase;
  zName = pName->a[0].zName;
  nName = sqlite3Strlen30(zName);
  assert( zDb!=0 || sqlite3BtreeHoldsAllMutexes(db) );
  for(i=OMIT_TEMPDB; i<db->nDb; i++){
    int j = (i<2) ? i^1 : i;  
    if( zDb && sqlite3StrICmp(db->aDb[j].zName, zDb) ) continue;
    assert( sqlite3SchemaMutexHeld(db, j, 0) );
    pTrigger = sqlite3HashFind(&(db->aDb[j].pSchema->trigHash), zName, nName);
    if( pTrigger ) break;
  }
  if( !pTrigger ){
    if( !noErr ){
      sqlite3ErrorMsg(pParse, "no such trigger: %S", pName, 0);
    }else{
      sqlite3CodeVerifyNamedSchema(pParse, zDb);
    }
    pParse->checkSchema = 1;
    goto drop_trigger_cleanup;
  }
  sqlite3DropTriggerPtr(pParse, pTrigger);

drop_trigger_cleanup:
  sqlite3SrcListDelete(db, pName);
}

static Table *tableOfTrigger(Trigger *pTrigger){
  int n = sqlite3Strlen30(pTrigger->table);
  return sqlite3HashFind(&pTrigger->pTabSchema->tblHash, pTrigger->table, n);
}


void sqlite3DropTriggerPtr(Parse *pParse, Trigger *pTrigger){
  Table   *pTable;
  Vdbe *v;
  sqlite3 *db = pParse->db;
  int iDb;

  iDb = sqlite3SchemaToIndex(pParse->db, pTrigger->pSchema);
  assert( iDb>=0 && iDb<db->nDb );
  pTable = tableOfTrigger(pTrigger);
  assert( pTable );
  assert( pTable->pSchema==pTrigger->pSchema || iDb==1 );
#ifndef SQLITE_OMIT_AUTHORIZATION
  {
    int code = SQLITE_DROP_TRIGGER;
    const char *zDb = db->aDb[iDb].zName;
    const char *zTab = SCHEMA_TABLE(iDb);
    if( iDb==1 ) code = SQLITE_DROP_TEMP_TRIGGER;
    if( sqlite3AuthCheck(pParse, code, pTrigger->zName, pTable->zName, zDb) ||
      sqlite3AuthCheck(pParse, SQLITE_DELETE, zTab, 0, zDb) ){
      return;
    }
  }
#endif

  assert( pTable!=0 );
  if( (v = sqlite3GetVdbe(pParse))!=0 ){
    int base;
    static const VdbeOpList dropTrigger[] = {
      { OP_Rewind,     0, ADDR(9),  0},
      { OP_String8,    0, 1,        0}, 
      { OP_Column,     0, 1,        2},
      { OP_Ne,         2, ADDR(8),  1},
      { OP_String8,    0, 1,        0}, 
      { OP_Column,     0, 0,        2},
      { OP_Ne,         2, ADDR(8),  1},
      { OP_Delete,     0, 0,        0},
      { OP_Next,       0, ADDR(1),  0}, 
    };

    sqlite3BeginWriteOperation(pParse, 0, iDb);
    sqlite3OpenMasterTable(pParse, iDb);
    base = sqlite3VdbeAddOpList(v,  ArraySize(dropTrigger), dropTrigger);
    sqlite3VdbeChangeP4(v, base+1, pTrigger->zName, P4_TRANSIENT);
    sqlite3VdbeChangeP4(v, base+4, "trigger", P4_STATIC);
    sqlite3ChangeCookie(pParse, iDb);
    sqlite3VdbeAddOp2(v, OP_Close, 0, 0);
    sqlite3VdbeAddOp4(v, OP_DropTrigger, iDb, 0, 0, pTrigger->zName, 0);
    if( pParse->nMem<3 ){
      pParse->nMem = 3;
    }
  }
}

void sqlite3UnlinkAndDeleteTrigger(sqlite3 *db, int iDb, const char *zName){
  Trigger *pTrigger;
  Hash *pHash;

  assert( sqlite3SchemaMutexHeld(db, iDb, 0) );
  pHash = &(db->aDb[iDb].pSchema->trigHash);
  pTrigger = sqlite3HashInsert(pHash, zName, sqlite3Strlen30(zName), 0);
  if( ALWAYS(pTrigger) ){
    if( pTrigger->pSchema==pTrigger->pTabSchema ){
      Table *pTab = tableOfTrigger(pTrigger);
      Trigger **pp;
      for(pp=&pTab->pTrigger; *pp!=pTrigger; pp=&((*pp)->pNext));
      *pp = (*pp)->pNext;
    }
    sqlite3DeleteTrigger(db, pTrigger);
    db->flags |= SQLITE_InternChanges;
  }
}

static int checkColumnOverlap(IdList *pIdList, ExprList *pEList){
  int e;
  if( pIdList==0 || NEVER(pEList==0) ) return 1;
  for(e=0; e<pEList->nExpr; e++){
    if( sqlite3IdListIndex(pIdList, pEList->a[e].zName)>=0 ) return 1;
  }
  return 0; 
}

Trigger *sqlite3TriggersExist(
  Parse *pParse,          
  Table *pTab,            
  int op,                 
  ExprList *pChanges,     
  int *pMask              
){
  int mask = 0;
  Trigger *pList = 0;
  Trigger *p;

  if( (pParse->db->flags & SQLITE_EnableTrigger)!=0 ){
    pList = sqlite3TriggerList(pParse, pTab);
  }
  assert( pList==0 || IsVirtual(pTab)==0 );
  for(p=pList; p; p=p->pNext){
    if( p->op==op && checkColumnOverlap(p->pColumns, pChanges) ){
      mask |= p->tr_tm;
    }
  }
  if( pMask ){
    *pMask = mask;
  }
  return (mask ? pList : 0);
}

static SrcList *targetSrcList(
  Parse *pParse,       
  TriggerStep *pStep   
){
  int iDb;             
  SrcList *pSrc;       

  pSrc = sqlite3SrcListAppend(pParse->db, 0, &pStep->target, 0);
  if( pSrc ){
    assert( pSrc->nSrc>0 );
    assert( pSrc->a!=0 );
    iDb = sqlite3SchemaToIndex(pParse->db, pStep->pTrig->pSchema);
    if( iDb==0 || iDb>=2 ){
      sqlite3 *db = pParse->db;
      assert( iDb<pParse->db->nDb );
      pSrc->a[pSrc->nSrc-1].zDatabase = sqlite3DbStrDup(db, db->aDb[iDb].zName);
    }
  }
  return pSrc;
}

static int codeTriggerProgram(
  Parse *pParse,            
  TriggerStep *pStepList,   
  int orconf                  
){
  TriggerStep *pStep;
  Vdbe *v = pParse->pVdbe;
  sqlite3 *db = pParse->db;

  assert( pParse->pTriggerTab && pParse->pToplevel );
  assert( pStepList );
  assert( v!=0 );
  for(pStep=pStepList; pStep; pStep=pStep->pNext){
    pParse->eOrconf = (orconf==OE_Default)?pStep->orconf:(u8)orconf;

    switch( pStep->op ){
      case TK_UPDATE: {
        sqlite3Update(pParse, 
          targetSrcList(pParse, pStep),
          sqlite3ExprListDup(db, pStep->pExprList, 0), 
          sqlite3ExprDup(db, pStep->pWhere, 0), 
          pParse->eOrconf
        );
        break;
      }
      case TK_INSERT: {
        sqlite3Insert(pParse, 
          targetSrcList(pParse, pStep),
          sqlite3ExprListDup(db, pStep->pExprList, 0), 
          sqlite3SelectDup(db, pStep->pSelect, 0), 
          sqlite3IdListDup(db, pStep->pIdList), 
          pParse->eOrconf
        );
        break;
      }
      case TK_DELETE: {
        sqlite3DeleteFrom(pParse, 
          targetSrcList(pParse, pStep),
          sqlite3ExprDup(db, pStep->pWhere, 0)
        );
        break;
      }
      default: assert( pStep->op==TK_SELECT ); {
        SelectDest sDest;
        Select *pSelect = sqlite3SelectDup(db, pStep->pSelect, 0);
        sqlite3SelectDestInit(&sDest, SRT_Discard, 0);
        sqlite3Select(pParse, pSelect, &sDest);
        sqlite3SelectDelete(db, pSelect);
        break;
      }
    } 
    if( pStep->op!=TK_SELECT ){
      sqlite3VdbeAddOp0(v, OP_ResetCount);
    }
  }

  return 0;
}

#ifdef SQLITE_DEBUG
static const char *onErrorText(int onError){
  switch( onError ){
    case OE_Abort:    return "abort";
    case OE_Rollback: return "rollback";
    case OE_Fail:     return "fail";
    case OE_Replace:  return "replace";
    case OE_Ignore:   return "ignore";
    case OE_Default:  return "default";
  }
  return "n/a";
}
#endif

static void transferParseError(Parse *pTo, Parse *pFrom){
  assert( pFrom->zErrMsg==0 || pFrom->nErr );
  assert( pTo->zErrMsg==0 || pTo->nErr );
  if( pTo->nErr==0 ){
    pTo->zErrMsg = pFrom->zErrMsg;
    pTo->nErr = pFrom->nErr;
  }else{
    sqlite3DbFree(pFrom->db, pFrom->zErrMsg);
  }
}

static TriggerPrg *codeRowTrigger(
  Parse *pParse,       
  Trigger *pTrigger,   
  Table *pTab,         
  int orconf           
){
  Parse *pTop = sqlite3ParseToplevel(pParse);
  sqlite3 *db = pParse->db;   
  TriggerPrg *pPrg;           
  Expr *pWhen = 0;            
  Vdbe *v;                    
  NameContext sNC;            
  SubProgram *pProgram = 0;   
  Parse *pSubParse;           
  int iEndTrigger = 0;        

  assert( pTrigger->zName==0 || pTab==tableOfTrigger(pTrigger) );
  assert( pTop->pVdbe );

  pPrg = sqlite3DbMallocZero(db, sizeof(TriggerPrg));
  if( !pPrg ) return 0;
  pPrg->pNext = pTop->pTriggerPrg;
  pTop->pTriggerPrg = pPrg;
  pPrg->pProgram = pProgram = sqlite3DbMallocZero(db, sizeof(SubProgram));
  if( !pProgram ) return 0;
  sqlite3VdbeLinkSubProgram(pTop->pVdbe, pProgram);
  pPrg->pTrigger = pTrigger;
  pPrg->orconf = orconf;
  pPrg->aColmask[0] = 0xffffffff;
  pPrg->aColmask[1] = 0xffffffff;

  pSubParse = sqlite3StackAllocZero(db, sizeof(Parse));
  if( !pSubParse ) return 0;
  memset(&sNC, 0, sizeof(sNC));
  sNC.pParse = pSubParse;
  pSubParse->db = db;
  pSubParse->pTriggerTab = pTab;
  pSubParse->pToplevel = pTop;
  pSubParse->zAuthContext = pTrigger->zName;
  pSubParse->eTriggerOp = pTrigger->op;
  pSubParse->nQueryLoop = pParse->nQueryLoop;

  v = sqlite3GetVdbe(pSubParse);
  if( v ){
    VdbeComment((v, "Start: %s.%s (%s %s%s%s ON %s)", 
      pTrigger->zName, onErrorText(orconf),
      (pTrigger->tr_tm==TRIGGER_BEFORE ? "BEFORE" : "AFTER"),
        (pTrigger->op==TK_UPDATE ? "UPDATE" : ""),
        (pTrigger->op==TK_INSERT ? "INSERT" : ""),
        (pTrigger->op==TK_DELETE ? "DELETE" : ""),
      pTab->zName
    ));
#ifndef SQLITE_OMIT_TRACE
    sqlite3VdbeChangeP4(v, -1, 
      sqlite3MPrintf(db, "-- TRIGGER %s", pTrigger->zName), P4_DYNAMIC
    );
#endif

    if( pTrigger->pWhen ){
      pWhen = sqlite3ExprDup(db, pTrigger->pWhen, 0);
      if( SQLITE_OK==sqlite3ResolveExprNames(&sNC, pWhen) 
       && db->mallocFailed==0 
      ){
        iEndTrigger = sqlite3VdbeMakeLabel(v);
        sqlite3ExprIfFalse(pSubParse, pWhen, iEndTrigger, SQLITE_JUMPIFNULL);
      }
      sqlite3ExprDelete(db, pWhen);
    }

    
    codeTriggerProgram(pSubParse, pTrigger->step_list, orconf);

    
    if( iEndTrigger ){
      sqlite3VdbeResolveLabel(v, iEndTrigger);
    }
    sqlite3VdbeAddOp0(v, OP_Halt);
    VdbeComment((v, "End: %s.%s", pTrigger->zName, onErrorText(orconf)));

    transferParseError(pParse, pSubParse);
    if( db->mallocFailed==0 ){
      pProgram->aOp = sqlite3VdbeTakeOpArray(v, &pProgram->nOp, &pTop->nMaxArg);
    }
    pProgram->nMem = pSubParse->nMem;
    pProgram->nCsr = pSubParse->nTab;
    pProgram->token = (void *)pTrigger;
    pPrg->aColmask[0] = pSubParse->oldmask;
    pPrg->aColmask[1] = pSubParse->newmask;
    sqlite3VdbeDelete(v);
  }

  assert( !pSubParse->pAinc       && !pSubParse->pZombieTab );
  assert( !pSubParse->pTriggerPrg && !pSubParse->nMaxArg );
  sqlite3StackFree(db, pSubParse);

  return pPrg;
}
    
static TriggerPrg *getRowTrigger(
  Parse *pParse,       
  Trigger *pTrigger,   
  Table *pTab,         
  int orconf           
){
  Parse *pRoot = sqlite3ParseToplevel(pParse);
  TriggerPrg *pPrg;

  assert( pTrigger->zName==0 || pTab==tableOfTrigger(pTrigger) );

  for(pPrg=pRoot->pTriggerPrg; 
      pPrg && (pPrg->pTrigger!=pTrigger || pPrg->orconf!=orconf); 
      pPrg=pPrg->pNext
  );

  
  if( !pPrg ){
    pPrg = codeRowTrigger(pParse, pTrigger, pTab, orconf);
  }

  return pPrg;
}

void sqlite3CodeRowTriggerDirect(
  Parse *pParse,       
  Trigger *p,          
  Table *pTab,         
  int reg,             
  int orconf,          
  int ignoreJump       
){
  Vdbe *v = sqlite3GetVdbe(pParse); 
  TriggerPrg *pPrg;
  pPrg = getRowTrigger(pParse, p, pTab, orconf);
  assert( pPrg || pParse->nErr || pParse->db->mallocFailed );

  if( pPrg ){
    int bRecursive = (p->zName && 0==(pParse->db->flags&SQLITE_RecTriggers));

    sqlite3VdbeAddOp3(v, OP_Program, reg, ignoreJump, ++pParse->nMem);
    sqlite3VdbeChangeP4(v, -1, (const char *)pPrg->pProgram, P4_SUBPROGRAM);
    VdbeComment(
        (v, "Call: %s.%s", (p->zName?p->zName:"fkey"), onErrorText(orconf)));

    sqlite3VdbeChangeP5(v, (u8)bRecursive);
  }
}

void sqlite3CodeRowTrigger(
  Parse *pParse,       
  Trigger *pTrigger,   
  int op,              
  ExprList *pChanges,  
  int tr_tm,           
  Table *pTab,         
  int reg,             
  int orconf,          
  int ignoreJump       
){
  Trigger *p;          

  assert( op==TK_UPDATE || op==TK_INSERT || op==TK_DELETE );
  assert( tr_tm==TRIGGER_BEFORE || tr_tm==TRIGGER_AFTER );
  assert( (op==TK_UPDATE)==(pChanges!=0) );

  for(p=pTrigger; p; p=p->pNext){

    assert( p->pSchema!=0 );
    assert( p->pTabSchema!=0 );
    assert( p->pSchema==p->pTabSchema 
         || p->pSchema==pParse->db->aDb[1].pSchema );

    
    if( p->op==op 
     && p->tr_tm==tr_tm 
     && checkColumnOverlap(p->pColumns, pChanges)
    ){
      sqlite3CodeRowTriggerDirect(pParse, p, pTab, reg, orconf, ignoreJump);
    }
  }
}

u32 sqlite3TriggerColmask(
  Parse *pParse,       
  Trigger *pTrigger,   
  ExprList *pChanges,  
  int isNew,           
  int tr_tm,           
  Table *pTab,         
  int orconf           
){
  const int op = pChanges ? TK_UPDATE : TK_DELETE;
  u32 mask = 0;
  Trigger *p;

  assert( isNew==1 || isNew==0 );
  for(p=pTrigger; p; p=p->pNext){
    if( p->op==op && (tr_tm&p->tr_tm)
     && checkColumnOverlap(p->pColumns,pChanges)
    ){
      TriggerPrg *pPrg;
      pPrg = getRowTrigger(pParse, p, pTab, orconf);
      if( pPrg ){
        mask |= pPrg->aColmask[isNew];
      }
    }
  }

  return mask;
}

#endif 

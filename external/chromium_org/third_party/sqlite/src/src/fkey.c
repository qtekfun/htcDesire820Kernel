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
** This file contains code used by the compiler to add foreign key
** support to compiled SQL statements.
*/
#include "sqliteInt.h"

#ifndef SQLITE_OMIT_FOREIGN_KEY
#ifndef SQLITE_OMIT_TRIGGER



static int locateFkeyIndex(
  Parse *pParse,                  
  Table *pParent,                 
  FKey *pFKey,                    
  Index **ppIdx,                  
  int **paiCol                    
){
  Index *pIdx = 0;                    
  int *aiCol = 0;                     
  int nCol = pFKey->nCol;             
  char *zKey = pFKey->aCol[0].zCol;   

  
  assert( ppIdx && *ppIdx==0 );
  assert( !paiCol || *paiCol==0 );
  assert( pParse );

  if( nCol==1 ){
    if( pParent->iPKey>=0 ){
      if( !zKey ) return 0;
      if( !sqlite3StrICmp(pParent->aCol[pParent->iPKey].zName, zKey) ) return 0;
    }
  }else if( paiCol ){
    assert( nCol>1 );
    aiCol = (int *)sqlite3DbMallocRaw(pParse->db, nCol*sizeof(int));
    if( !aiCol ) return 1;
    *paiCol = aiCol;
  }

  for(pIdx=pParent->pIndex; pIdx; pIdx=pIdx->pNext){
    if( pIdx->nColumn==nCol && pIdx->onError!=OE_None ){ 

      if( zKey==0 ){
        if( pIdx->autoIndex==2 ){
          if( aiCol ){
            int i;
            for(i=0; i<nCol; i++) aiCol[i] = pFKey->aCol[i].iFrom;
          }
          break;
        }
      }else{
        int i, j;
        for(i=0; i<nCol; i++){
          int iCol = pIdx->aiColumn[i];     
          char *zDfltColl;                  
          char *zIdxCol;                    

          zDfltColl = pParent->aCol[iCol].zColl;
          if( !zDfltColl ){
            zDfltColl = "BINARY";
          }
          if( sqlite3StrICmp(pIdx->azColl[i], zDfltColl) ) break;

          zIdxCol = pParent->aCol[iCol].zName;
          for(j=0; j<nCol; j++){
            if( sqlite3StrICmp(pFKey->aCol[j].zCol, zIdxCol)==0 ){
              if( aiCol ) aiCol[i] = pFKey->aCol[j].iFrom;
              break;
            }
          }
          if( j==nCol ) break;
        }
        if( i==nCol ) break;      
      }
    }
  }

  if( !pIdx ){
    if( !pParse->disableTriggers ){
      sqlite3ErrorMsg(pParse, "foreign key mismatch");
    }
    sqlite3DbFree(pParse->db, aiCol);
    return 1;
  }

  *ppIdx = pIdx;
  return 0;
}

static void fkLookupParent(
  Parse *pParse,        
  int iDb,              
  Table *pTab,          
  Index *pIdx,          
  FKey *pFKey,          
  int *aiCol,           
  int regData,          
  int nIncr,            
  int isIgnore          
){
  int i;                                    
  Vdbe *v = sqlite3GetVdbe(pParse);         
  int iCur = pParse->nTab - 1;              
  int iOk = sqlite3VdbeMakeLabel(v);        

  if( nIncr<0 ){
    sqlite3VdbeAddOp2(v, OP_FkIfZero, pFKey->isDeferred, iOk);
  }
  for(i=0; i<pFKey->nCol; i++){
    int iReg = aiCol[i] + regData + 1;
    sqlite3VdbeAddOp2(v, OP_IsNull, iReg, iOk);
  }

  if( isIgnore==0 ){
    if( pIdx==0 ){
      int iMustBeInt;               
      int regTemp = sqlite3GetTempReg(pParse);
  
      sqlite3VdbeAddOp2(v, OP_SCopy, aiCol[0]+1+regData, regTemp);
      iMustBeInt = sqlite3VdbeAddOp2(v, OP_MustBeInt, regTemp, 0);
  
      if( pTab==pFKey->pFrom && nIncr==1 ){
        sqlite3VdbeAddOp3(v, OP_Eq, regData, iOk, regTemp);
      }
  
      sqlite3OpenTable(pParse, iCur, iDb, pTab, OP_OpenRead);
      sqlite3VdbeAddOp3(v, OP_NotExists, iCur, 0, regTemp);
      sqlite3VdbeAddOp2(v, OP_Goto, 0, iOk);
      sqlite3VdbeJumpHere(v, sqlite3VdbeCurrentAddr(v)-2);
      sqlite3VdbeJumpHere(v, iMustBeInt);
      sqlite3ReleaseTempReg(pParse, regTemp);
    }else{
      int nCol = pFKey->nCol;
      int regTemp = sqlite3GetTempRange(pParse, nCol);
      int regRec = sqlite3GetTempReg(pParse);
      KeyInfo *pKey = sqlite3IndexKeyinfo(pParse, pIdx);
  
      sqlite3VdbeAddOp3(v, OP_OpenRead, iCur, pIdx->tnum, iDb);
      sqlite3VdbeChangeP4(v, -1, (char*)pKey, P4_KEYINFO_HANDOFF);
      for(i=0; i<nCol; i++){
        sqlite3VdbeAddOp2(v, OP_Copy, aiCol[i]+1+regData, regTemp+i);
      }
  
      if( pTab==pFKey->pFrom && nIncr==1 ){
        int iJump = sqlite3VdbeCurrentAddr(v) + nCol + 1;
        for(i=0; i<nCol; i++){
          int iChild = aiCol[i]+1+regData;
          int iParent = pIdx->aiColumn[i]+1+regData;
          sqlite3VdbeAddOp3(v, OP_Ne, iChild, iJump, iParent);
        }
        sqlite3VdbeAddOp2(v, OP_Goto, 0, iOk);
      }
  
      sqlite3VdbeAddOp3(v, OP_MakeRecord, regTemp, nCol, regRec);
      sqlite3VdbeChangeP4(v, -1, sqlite3IndexAffinityStr(v,pIdx), P4_TRANSIENT);
      sqlite3VdbeAddOp4Int(v, OP_Found, iCur, iOk, regRec, 0);
  
      sqlite3ReleaseTempReg(pParse, regRec);
      sqlite3ReleaseTempRange(pParse, regTemp, nCol);
    }
  }

  if( !pFKey->isDeferred && !pParse->pToplevel && !pParse->isMultiWrite ){
    assert( nIncr==1 );
    sqlite3HaltConstraint(
        pParse, OE_Abort, "foreign key constraint failed", P4_STATIC
    );
  }else{
    if( nIncr>0 && pFKey->isDeferred==0 ){
      sqlite3ParseToplevel(pParse)->mayAbort = 1;
    }
    sqlite3VdbeAddOp2(v, OP_FkCounter, pFKey->isDeferred, nIncr);
  }

  sqlite3VdbeResolveLabel(v, iOk);
  sqlite3VdbeAddOp1(v, OP_Close, iCur);
}

static void fkScanChildren(
  Parse *pParse,                  
  SrcList *pSrc,                  
  Table *pTab,
  Index *pIdx,                    
  FKey *pFKey,                    
  int *aiCol,                     
  int regData,                    
  int nIncr                       
){
  sqlite3 *db = pParse->db;       
  int i;                          
  Expr *pWhere = 0;               
  NameContext sNameContext;       
  WhereInfo *pWInfo;              
  int iFkIfZero = 0;              
  Vdbe *v = sqlite3GetVdbe(pParse);

  assert( !pIdx || pIdx->pTable==pTab );

  if( nIncr<0 ){
    iFkIfZero = sqlite3VdbeAddOp2(v, OP_FkIfZero, pFKey->isDeferred, 0);
  }

  for(i=0; i<pFKey->nCol; i++){
    Expr *pLeft;                  
    Expr *pRight;                 
    Expr *pEq;                    
    int iCol;                      
    const char *zCol;             

    pLeft = sqlite3Expr(db, TK_REGISTER, 0);
    if( pLeft ){
      if( pIdx ){
        Column *pCol;
        iCol = pIdx->aiColumn[i];
        pCol = &pTab->aCol[iCol];
        if( pTab->iPKey==iCol ) iCol = -1;
        pLeft->iTable = regData+iCol+1;
        pLeft->affinity = pCol->affinity;
        pLeft->pColl = sqlite3LocateCollSeq(pParse, pCol->zColl);
      }else{
        pLeft->iTable = regData;
        pLeft->affinity = SQLITE_AFF_INTEGER;
      }
    }
    iCol = aiCol ? aiCol[i] : pFKey->aCol[0].iFrom;
    assert( iCol>=0 );
    zCol = pFKey->pFrom->aCol[iCol].zName;
    pRight = sqlite3Expr(db, TK_ID, zCol);
    pEq = sqlite3PExpr(pParse, TK_EQ, pLeft, pRight, 0);
    pWhere = sqlite3ExprAnd(db, pWhere, pEq);
  }

  if( pTab==pFKey->pFrom && nIncr>0 ){
    Expr *pEq;                    
    Expr *pLeft;                  
    Expr *pRight;                 
    pLeft = sqlite3Expr(db, TK_REGISTER, 0);
    pRight = sqlite3Expr(db, TK_COLUMN, 0);
    if( pLeft && pRight ){
      pLeft->iTable = regData;
      pLeft->affinity = SQLITE_AFF_INTEGER;
      pRight->iTable = pSrc->a[0].iCursor;
      pRight->iColumn = -1;
    }
    pEq = sqlite3PExpr(pParse, TK_NE, pLeft, pRight, 0);
    pWhere = sqlite3ExprAnd(db, pWhere, pEq);
  }

  
  memset(&sNameContext, 0, sizeof(NameContext));
  sNameContext.pSrcList = pSrc;
  sNameContext.pParse = pParse;
  sqlite3ResolveExprNames(&sNameContext, pWhere);

  pWInfo = sqlite3WhereBegin(pParse, pSrc, pWhere, 0, 0);
  if( nIncr>0 && pFKey->isDeferred==0 ){
    sqlite3ParseToplevel(pParse)->mayAbort = 1;
  }
  sqlite3VdbeAddOp2(v, OP_FkCounter, pFKey->isDeferred, nIncr);
  if( pWInfo ){
    sqlite3WhereEnd(pWInfo);
  }

  
  sqlite3ExprDelete(db, pWhere);
  if( iFkIfZero ){
    sqlite3VdbeJumpHere(v, iFkIfZero);
  }
}

FKey *sqlite3FkReferences(Table *pTab){
  int nName = sqlite3Strlen30(pTab->zName);
  return (FKey *)sqlite3HashFind(&pTab->pSchema->fkeyHash, pTab->zName, nName);
}

static void fkTriggerDelete(sqlite3 *dbMem, Trigger *p){
  if( p ){
    TriggerStep *pStep = p->step_list;
    sqlite3ExprDelete(dbMem, pStep->pWhere);
    sqlite3ExprListDelete(dbMem, pStep->pExprList);
    sqlite3SelectDelete(dbMem, pStep->pSelect);
    sqlite3ExprDelete(dbMem, p->pWhen);
    sqlite3DbFree(dbMem, p);
  }
}

void sqlite3FkDropTable(Parse *pParse, SrcList *pName, Table *pTab){
  sqlite3 *db = pParse->db;
  if( (db->flags&SQLITE_ForeignKeys) && !IsVirtual(pTab) && !pTab->pSelect ){
    int iSkip = 0;
    Vdbe *v = sqlite3GetVdbe(pParse);

    assert( v );                  
    if( sqlite3FkReferences(pTab)==0 ){
      FKey *p;
      for(p=pTab->pFKey; p; p=p->pNextFrom){
        if( p->isDeferred ) break;
      }
      if( !p ) return;
      iSkip = sqlite3VdbeMakeLabel(v);
      sqlite3VdbeAddOp2(v, OP_FkIfZero, 1, iSkip);
    }

    pParse->disableTriggers = 1;
    sqlite3DeleteFrom(pParse, sqlite3SrcListDup(db, pName, 0), 0);
    pParse->disableTriggers = 0;

    sqlite3VdbeAddOp2(v, OP_FkIfZero, 0, sqlite3VdbeCurrentAddr(v)+2);
    sqlite3HaltConstraint(
        pParse, OE_Abort, "foreign key constraint failed", P4_STATIC
    );

    if( iSkip ){
      sqlite3VdbeResolveLabel(v, iSkip);
    }
  }
}

void sqlite3FkCheck(
  Parse *pParse,                  
  Table *pTab,                     
  int regOld,                     
  int regNew                      
){
  sqlite3 *db = pParse->db;       
  FKey *pFKey;                    
  int iDb;                        
  const char *zDb;                
  int isIgnoreErrors = pParse->disableTriggers;

  
  assert( (regOld==0)!=(regNew==0) );

  
  if( (db->flags&SQLITE_ForeignKeys)==0 ) return;

  iDb = sqlite3SchemaToIndex(db, pTab->pSchema);
  zDb = db->aDb[iDb].zName;

  for(pFKey=pTab->pFKey; pFKey; pFKey=pFKey->pNextFrom){
    Table *pTo;                   
    Index *pIdx = 0;              
    int *aiFree = 0;
    int *aiCol;
    int iCol;
    int i;
    int isIgnore = 0;

    if( pParse->disableTriggers ){
      pTo = sqlite3FindTable(db, pFKey->zTo, zDb);
    }else{
      pTo = sqlite3LocateTable(pParse, 0, pFKey->zTo, zDb);
    }
    if( !pTo || locateFkeyIndex(pParse, pTo, pFKey, &pIdx, &aiFree) ){
      if( !isIgnoreErrors || db->mallocFailed ) return;
      continue;
    }
    assert( pFKey->nCol==1 || (aiFree && pIdx) );

    if( aiFree ){
      aiCol = aiFree;
    }else{
      iCol = pFKey->aCol[0].iFrom;
      aiCol = &iCol;
    }
    for(i=0; i<pFKey->nCol; i++){
      if( aiCol[i]==pTab->iPKey ){
        aiCol[i] = -1;
      }
#ifndef SQLITE_OMIT_AUTHORIZATION
      if( db->xAuth ){
        int rcauth;
        char *zCol = pTo->aCol[pIdx ? pIdx->aiColumn[i] : pTo->iPKey].zName;
        rcauth = sqlite3AuthReadCol(pParse, pTo->zName, zCol, iDb);
        isIgnore = (rcauth==SQLITE_IGNORE);
      }
#endif
    }

    sqlite3TableLock(pParse, iDb, pTo->tnum, 0, pTo->zName);
    pParse->nTab++;

    if( regOld!=0 ){
      fkLookupParent(pParse, iDb, pTo, pIdx, pFKey, aiCol, regOld, -1,isIgnore);
    }
    if( regNew!=0 ){
 
      fkLookupParent(pParse, iDb, pTo, pIdx, pFKey, aiCol, regNew, +1,isIgnore);
    }

    sqlite3DbFree(db, aiFree);
  }

  
  for(pFKey = sqlite3FkReferences(pTab); pFKey; pFKey=pFKey->pNextTo){
    Index *pIdx = 0;              
    SrcList *pSrc;
    int *aiCol = 0;

    if( !pFKey->isDeferred && !pParse->pToplevel && !pParse->isMultiWrite ){
      assert( regOld==0 && regNew!=0 );
      continue;
    }

    if( locateFkeyIndex(pParse, pTab, pFKey, &pIdx, &aiCol) ){
      if( !isIgnoreErrors || db->mallocFailed ) return;
      continue;
    }
    assert( aiCol || pFKey->nCol==1 );

    pSrc = sqlite3SrcListAppend(db, 0, 0, 0);
    if( pSrc ){
      struct SrcList_item *pItem = pSrc->a;
      pItem->pTab = pFKey->pFrom;
      pItem->zName = pFKey->pFrom->zName;
      pItem->pTab->nRef++;
      pItem->iCursor = pParse->nTab++;
  
      if( regNew!=0 ){
        fkScanChildren(pParse, pSrc, pTab, pIdx, pFKey, aiCol, regNew, -1);
      }
      if( regOld!=0 ){
        fkScanChildren(pParse, pSrc, pTab, pIdx, pFKey, aiCol, regOld, 1);
      }
      pItem->zName = 0;
      sqlite3SrcListDelete(db, pSrc);
    }
    sqlite3DbFree(db, aiCol);
  }
}

#define COLUMN_MASK(x) (((x)>31) ? 0xffffffff : ((u32)1<<(x)))

u32 sqlite3FkOldmask(
  Parse *pParse,                  
  Table *pTab                     
){
  u32 mask = 0;
  if( pParse->db->flags&SQLITE_ForeignKeys ){
    FKey *p;
    int i;
    for(p=pTab->pFKey; p; p=p->pNextFrom){
      for(i=0; i<p->nCol; i++) mask |= COLUMN_MASK(p->aCol[i].iFrom);
    }
    for(p=sqlite3FkReferences(pTab); p; p=p->pNextTo){
      Index *pIdx = 0;
      locateFkeyIndex(pParse, pTab, p, &pIdx, 0);
      if( pIdx ){
        for(i=0; i<pIdx->nColumn; i++) mask |= COLUMN_MASK(pIdx->aiColumn[i]);
      }
    }
  }
  return mask;
}

int sqlite3FkRequired(
  Parse *pParse,                  
  Table *pTab,                    
  int *aChange,                   
  int chngRowid                   
){
  if( pParse->db->flags&SQLITE_ForeignKeys ){
    if( !aChange ){
      return (sqlite3FkReferences(pTab) || pTab->pFKey);
    }else{
      int i;
      FKey *p;

      
      for(p=pTab->pFKey; p; p=p->pNextFrom){
        for(i=0; i<p->nCol; i++){
          int iChildKey = p->aCol[i].iFrom;
          if( aChange[iChildKey]>=0 ) return 1;
          if( iChildKey==pTab->iPKey && chngRowid ) return 1;
        }
      }

      
      for(p=sqlite3FkReferences(pTab); p; p=p->pNextTo){
        for(i=0; i<p->nCol; i++){
          char *zKey = p->aCol[i].zCol;
          int iKey;
          for(iKey=0; iKey<pTab->nCol; iKey++){
            Column *pCol = &pTab->aCol[iKey];
            if( (zKey ? !sqlite3StrICmp(pCol->zName, zKey) : pCol->isPrimKey) ){
              if( aChange[iKey]>=0 ) return 1;
              if( iKey==pTab->iPKey && chngRowid ) return 1;
            }
          }
        }
      }
    }
  }
  return 0;
}

static Trigger *fkActionTrigger(
  Parse *pParse,                  
  Table *pTab,                    
  FKey *pFKey,                    
  ExprList *pChanges              
){
  sqlite3 *db = pParse->db;       
  int action;                     
  Trigger *pTrigger;              
  int iAction = (pChanges!=0);    

  action = pFKey->aAction[iAction];
  pTrigger = pFKey->apTrigger[iAction];

  if( action!=OE_None && !pTrigger ){
    u8 enableLookaside;           
    char const *zFrom;            
    int nFrom;                    
    Index *pIdx = 0;              
    int *aiCol = 0;               
    TriggerStep *pStep = 0;        
    Expr *pWhere = 0;             
    ExprList *pList = 0;          
    Select *pSelect = 0;          
    int i;                        
    Expr *pWhen = 0;              

    if( locateFkeyIndex(pParse, pTab, pFKey, &pIdx, &aiCol) ) return 0;
    assert( aiCol || pFKey->nCol==1 );

    for(i=0; i<pFKey->nCol; i++){
      Token tOld = { "old", 3 };  
      Token tNew = { "new", 3 };  
      Token tFromCol;             
      Token tToCol;               
      int iFromCol;               
      Expr *pEq;                  

      iFromCol = aiCol ? aiCol[i] : pFKey->aCol[0].iFrom;
      assert( iFromCol>=0 );
      tToCol.z = pIdx ? pTab->aCol[pIdx->aiColumn[i]].zName : "oid";
      tFromCol.z = pFKey->pFrom->aCol[iFromCol].zName;

      tToCol.n = sqlite3Strlen30(tToCol.z);
      tFromCol.n = sqlite3Strlen30(tFromCol.z);

      pEq = sqlite3PExpr(pParse, TK_EQ,
          sqlite3PExpr(pParse, TK_DOT, 
            sqlite3PExpr(pParse, TK_ID, 0, 0, &tOld),
            sqlite3PExpr(pParse, TK_ID, 0, 0, &tToCol)
          , 0),
          sqlite3PExpr(pParse, TK_ID, 0, 0, &tFromCol)
      , 0);
      pWhere = sqlite3ExprAnd(db, pWhere, pEq);

      if( pChanges ){
        pEq = sqlite3PExpr(pParse, TK_IS,
            sqlite3PExpr(pParse, TK_DOT, 
              sqlite3PExpr(pParse, TK_ID, 0, 0, &tOld),
              sqlite3PExpr(pParse, TK_ID, 0, 0, &tToCol),
              0),
            sqlite3PExpr(pParse, TK_DOT, 
              sqlite3PExpr(pParse, TK_ID, 0, 0, &tNew),
              sqlite3PExpr(pParse, TK_ID, 0, 0, &tToCol),
              0),
            0);
        pWhen = sqlite3ExprAnd(db, pWhen, pEq);
      }
  
      if( action!=OE_Restrict && (action!=OE_Cascade || pChanges) ){
        Expr *pNew;
        if( action==OE_Cascade ){
          pNew = sqlite3PExpr(pParse, TK_DOT, 
            sqlite3PExpr(pParse, TK_ID, 0, 0, &tNew),
            sqlite3PExpr(pParse, TK_ID, 0, 0, &tToCol)
          , 0);
        }else if( action==OE_SetDflt ){
          Expr *pDflt = pFKey->pFrom->aCol[iFromCol].pDflt;
          if( pDflt ){
            pNew = sqlite3ExprDup(db, pDflt, 0);
          }else{
            pNew = sqlite3PExpr(pParse, TK_NULL, 0, 0, 0);
          }
        }else{
          pNew = sqlite3PExpr(pParse, TK_NULL, 0, 0, 0);
        }
        pList = sqlite3ExprListAppend(pParse, pList, pNew);
        sqlite3ExprListSetName(pParse, pList, &tFromCol, 0);
      }
    }
    sqlite3DbFree(db, aiCol);

    zFrom = pFKey->pFrom->zName;
    nFrom = sqlite3Strlen30(zFrom);

    if( action==OE_Restrict ){
      Token tFrom;
      Expr *pRaise; 

      tFrom.z = zFrom;
      tFrom.n = nFrom;
      pRaise = sqlite3Expr(db, TK_RAISE, "foreign key constraint failed");
      if( pRaise ){
        pRaise->affinity = OE_Abort;
      }
      pSelect = sqlite3SelectNew(pParse, 
          sqlite3ExprListAppend(pParse, 0, pRaise),
          sqlite3SrcListAppend(db, 0, &tFrom, 0),
          pWhere,
          0, 0, 0, 0, 0, 0
      );
      pWhere = 0;
    }

    
    enableLookaside = db->lookaside.bEnabled;
    db->lookaside.bEnabled = 0;

    pTrigger = (Trigger *)sqlite3DbMallocZero(db, 
        sizeof(Trigger) +         
        sizeof(TriggerStep) +     
        nFrom + 1                 
    );
    if( pTrigger ){
      pStep = pTrigger->step_list = (TriggerStep *)&pTrigger[1];
      pStep->target.z = (char *)&pStep[1];
      pStep->target.n = nFrom;
      memcpy((char *)pStep->target.z, zFrom, nFrom);
  
      pStep->pWhere = sqlite3ExprDup(db, pWhere, EXPRDUP_REDUCE);
      pStep->pExprList = sqlite3ExprListDup(db, pList, EXPRDUP_REDUCE);
      pStep->pSelect = sqlite3SelectDup(db, pSelect, EXPRDUP_REDUCE);
      if( pWhen ){
        pWhen = sqlite3PExpr(pParse, TK_NOT, pWhen, 0, 0);
        pTrigger->pWhen = sqlite3ExprDup(db, pWhen, EXPRDUP_REDUCE);
      }
    }

    
    db->lookaside.bEnabled = enableLookaside;

    sqlite3ExprDelete(db, pWhere);
    sqlite3ExprDelete(db, pWhen);
    sqlite3ExprListDelete(db, pList);
    sqlite3SelectDelete(db, pSelect);
    if( db->mallocFailed==1 ){
      fkTriggerDelete(db, pTrigger);
      return 0;
    }

    switch( action ){
      case OE_Restrict:
        pStep->op = TK_SELECT; 
        break;
      case OE_Cascade: 
        if( !pChanges ){ 
          pStep->op = TK_DELETE; 
          break; 
        }
      default:
        pStep->op = TK_UPDATE;
    }
    pStep->pTrig = pTrigger;
    pTrigger->pSchema = pTab->pSchema;
    pTrigger->pTabSchema = pTab->pSchema;
    pFKey->apTrigger[iAction] = pTrigger;
    pTrigger->op = (pChanges ? TK_UPDATE : TK_DELETE);
  }

  return pTrigger;
}

void sqlite3FkActions(
  Parse *pParse,                  
  Table *pTab,                    
  ExprList *pChanges,             
  int regOld                      
){
  if( pParse->db->flags&SQLITE_ForeignKeys ){
    FKey *pFKey;                  
    for(pFKey = sqlite3FkReferences(pTab); pFKey; pFKey=pFKey->pNextTo){
      Trigger *pAction = fkActionTrigger(pParse, pTab, pFKey, pChanges);
      if( pAction ){
        sqlite3CodeRowTriggerDirect(pParse, pAction, pTab, regOld, OE_Abort, 0);
      }
    }
  }
}

#endif 

void sqlite3FkDelete(sqlite3 *db, Table *pTab){
  FKey *pFKey;                    
  FKey *pNext;                    

  assert( db==0 || sqlite3SchemaMutexHeld(db, 0, pTab->pSchema) );
  for(pFKey=pTab->pFKey; pFKey; pFKey=pNext){

    
    if( !db || db->pnBytesFreed==0 ){
      if( pFKey->pPrevTo ){
        pFKey->pPrevTo->pNextTo = pFKey->pNextTo;
      }else{
        void *p = (void *)pFKey->pNextTo;
        const char *z = (p ? pFKey->pNextTo->zTo : pFKey->zTo);
        sqlite3HashInsert(&pTab->pSchema->fkeyHash, z, sqlite3Strlen30(z), p);
      }
      if( pFKey->pNextTo ){
        pFKey->pNextTo->pPrevTo = pFKey->pPrevTo;
      }
    }

    assert( pFKey->isDeferred==0 || pFKey->isDeferred==1 );

    
#ifndef SQLITE_OMIT_TRIGGER
    fkTriggerDelete(db, pFKey->apTrigger[0]);
    fkTriggerDelete(db, pFKey->apTrigger[1]);
#endif

    pNext = pFKey->pNextFrom;
    sqlite3DbFree(db, pFKey);
  }
}
#endif 

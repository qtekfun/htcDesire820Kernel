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
** This file contains C code routines that are called by the parser
** to handle SELECT statements in SQLite.
*/
#include "sqliteInt.h"


static void clearSelect(sqlite3 *db, Select *p){
  sqlite3ExprListDelete(db, p->pEList);
  sqlite3SrcListDelete(db, p->pSrc);
  sqlite3ExprDelete(db, p->pWhere);
  sqlite3ExprListDelete(db, p->pGroupBy);
  sqlite3ExprDelete(db, p->pHaving);
  sqlite3ExprListDelete(db, p->pOrderBy);
  sqlite3SelectDelete(db, p->pPrior);
  sqlite3ExprDelete(db, p->pLimit);
  sqlite3ExprDelete(db, p->pOffset);
}

void sqlite3SelectDestInit(SelectDest *pDest, int eDest, int iParm){
  pDest->eDest = (u8)eDest;
  pDest->iParm = iParm;
  pDest->affinity = 0;
  pDest->iMem = 0;
  pDest->nMem = 0;
}


Select *sqlite3SelectNew(
  Parse *pParse,        
  ExprList *pEList,     
  SrcList *pSrc,        
  Expr *pWhere,         
  ExprList *pGroupBy,   
  Expr *pHaving,        
  ExprList *pOrderBy,   
  int isDistinct,       
  Expr *pLimit,         
  Expr *pOffset         
){
  Select *pNew;
  Select standin;
  sqlite3 *db = pParse->db;
  pNew = sqlite3DbMallocZero(db, sizeof(*pNew) );
  assert( db->mallocFailed || !pOffset || pLimit ); 
  if( pNew==0 ){
    pNew = &standin;
    memset(pNew, 0, sizeof(*pNew));
  }
  if( pEList==0 ){
    pEList = sqlite3ExprListAppend(pParse, 0, sqlite3Expr(db,TK_ALL,0));
  }
  pNew->pEList = pEList;
  pNew->pSrc = pSrc;
  pNew->pWhere = pWhere;
  pNew->pGroupBy = pGroupBy;
  pNew->pHaving = pHaving;
  pNew->pOrderBy = pOrderBy;
  pNew->selFlags = isDistinct ? SF_Distinct : 0;
  pNew->op = TK_SELECT;
  pNew->pLimit = pLimit;
  pNew->pOffset = pOffset;
  assert( pOffset==0 || pLimit!=0 );
  pNew->addrOpenEphm[0] = -1;
  pNew->addrOpenEphm[1] = -1;
  pNew->addrOpenEphm[2] = -1;
  if( db->mallocFailed ) {
    clearSelect(db, pNew);
    if( pNew!=&standin ) sqlite3DbFree(db, pNew);
    pNew = 0;
  }
  return pNew;
}

void sqlite3SelectDelete(sqlite3 *db, Select *p){
  if( p ){
    clearSelect(db, p);
    sqlite3DbFree(db, p);
  }
}

int sqlite3JoinType(Parse *pParse, Token *pA, Token *pB, Token *pC){
  int jointype = 0;
  Token *apAll[3];
  Token *p;
                             
  static const char zKeyText[] = "naturaleftouterightfullinnercross";
  static const struct {
    u8 i;        
    u8 nChar;    
    u8 code;     
  } aKeyword[] = {
     { 0,  7, JT_NATURAL                },
     { 6,  4, JT_LEFT|JT_OUTER          },
     { 10, 5, JT_OUTER                  },
     { 14, 5, JT_RIGHT|JT_OUTER         },
     { 19, 4, JT_LEFT|JT_RIGHT|JT_OUTER },
     { 23, 5, JT_INNER                  },
     { 28, 5, JT_INNER|JT_CROSS         },
  };
  int i, j;
  apAll[0] = pA;
  apAll[1] = pB;
  apAll[2] = pC;
  for(i=0; i<3 && apAll[i]; i++){
    p = apAll[i];
    for(j=0; j<ArraySize(aKeyword); j++){
      if( p->n==aKeyword[j].nChar 
          && sqlite3StrNICmp((char*)p->z, &zKeyText[aKeyword[j].i], p->n)==0 ){
        jointype |= aKeyword[j].code;
        break;
      }
    }
    testcase( j==0 || j==1 || j==2 || j==3 || j==4 || j==5 || j==6 );
    if( j>=ArraySize(aKeyword) ){
      jointype |= JT_ERROR;
      break;
    }
  }
  if(
     (jointype & (JT_INNER|JT_OUTER))==(JT_INNER|JT_OUTER) ||
     (jointype & JT_ERROR)!=0
  ){
    const char *zSp = " ";
    assert( pB!=0 );
    if( pC==0 ){ zSp++; }
    sqlite3ErrorMsg(pParse, "unknown or unsupported join type: "
       "%T %T%s%T", pA, pB, zSp, pC);
    jointype = JT_INNER;
  }else if( (jointype & JT_OUTER)!=0 
         && (jointype & (JT_LEFT|JT_RIGHT))!=JT_LEFT ){
    sqlite3ErrorMsg(pParse, 
      "RIGHT and FULL OUTER JOINs are not currently supported");
    jointype = JT_INNER;
  }
  return jointype;
}

static int columnIndex(Table *pTab, const char *zCol){
  int i;
  for(i=0; i<pTab->nCol; i++){
    if( sqlite3StrICmp(pTab->aCol[i].zName, zCol)==0 ) return i;
  }
  return -1;
}

static int tableAndColumnIndex(
  SrcList *pSrc,       
  int N,               
  const char *zCol,    
  int *piTab,          
  int *piCol           
){
  int i;               
  int iCol;            

  assert( (piTab==0)==(piCol==0) );  
  for(i=0; i<N; i++){
    iCol = columnIndex(pSrc->a[i].pTab, zCol);
    if( iCol>=0 ){
      if( piTab ){
        *piTab = i;
        *piCol = iCol;
      }
      return 1;
    }
  }
  return 0;
}

static void addWhereTerm(
  Parse *pParse,                  
  SrcList *pSrc,                  
  int iLeft,                      
  int iColLeft,                   
  int iRight,                     
  int iColRight,                  
  int isOuterJoin,                
  Expr **ppWhere                  
){
  sqlite3 *db = pParse->db;
  Expr *pE1;
  Expr *pE2;
  Expr *pEq;

  assert( iLeft<iRight );
  assert( pSrc->nSrc>iRight );
  assert( pSrc->a[iLeft].pTab );
  assert( pSrc->a[iRight].pTab );

  pE1 = sqlite3CreateColumnExpr(db, pSrc, iLeft, iColLeft);
  pE2 = sqlite3CreateColumnExpr(db, pSrc, iRight, iColRight);

  pEq = sqlite3PExpr(pParse, TK_EQ, pE1, pE2, 0);
  if( pEq && isOuterJoin ){
    ExprSetProperty(pEq, EP_FromJoin);
    assert( !ExprHasAnyProperty(pEq, EP_TokenOnly|EP_Reduced) );
    ExprSetIrreducible(pEq);
    pEq->iRightJoinTable = (i16)pE2->iTable;
  }
  *ppWhere = sqlite3ExprAnd(db, *ppWhere, pEq);
}

static void setJoinExpr(Expr *p, int iTable){
  while( p ){
    ExprSetProperty(p, EP_FromJoin);
    assert( !ExprHasAnyProperty(p, EP_TokenOnly|EP_Reduced) );
    ExprSetIrreducible(p);
    p->iRightJoinTable = (i16)iTable;
    setJoinExpr(p->pLeft, iTable);
    p = p->pRight;
  } 
}

static int sqliteProcessJoin(Parse *pParse, Select *p){
  SrcList *pSrc;                  
  int i, j;                       
  struct SrcList_item *pLeft;     
  struct SrcList_item *pRight;    

  pSrc = p->pSrc;
  pLeft = &pSrc->a[0];
  pRight = &pLeft[1];
  for(i=0; i<pSrc->nSrc-1; i++, pRight++, pLeft++){
    Table *pLeftTab = pLeft->pTab;
    Table *pRightTab = pRight->pTab;
    int isOuter;

    if( NEVER(pLeftTab==0 || pRightTab==0) ) continue;
    isOuter = (pRight->jointype & JT_OUTER)!=0;

    if( pRight->jointype & JT_NATURAL ){
      if( pRight->pOn || pRight->pUsing ){
        sqlite3ErrorMsg(pParse, "a NATURAL join may not have "
           "an ON or USING clause", 0);
        return 1;
      }
      for(j=0; j<pRightTab->nCol; j++){
        char *zName;   
        int iLeft;     
        int iLeftCol;  

        zName = pRightTab->aCol[j].zName;
        if( tableAndColumnIndex(pSrc, i+1, zName, &iLeft, &iLeftCol) ){
          addWhereTerm(pParse, pSrc, iLeft, iLeftCol, i+1, j,
                       isOuter, &p->pWhere);
        }
      }
    }

    if( pRight->pOn && pRight->pUsing ){
      sqlite3ErrorMsg(pParse, "cannot have both ON and USING "
        "clauses in the same join");
      return 1;
    }

    if( pRight->pOn ){
      if( isOuter ) setJoinExpr(pRight->pOn, pRight->iCursor);
      p->pWhere = sqlite3ExprAnd(pParse->db, p->pWhere, pRight->pOn);
      pRight->pOn = 0;
    }

    if( pRight->pUsing ){
      IdList *pList = pRight->pUsing;
      for(j=0; j<pList->nId; j++){
        char *zName;     
        int iLeft;       
        int iLeftCol;    
        int iRightCol;   

        zName = pList->a[j].zName;
        iRightCol = columnIndex(pRightTab, zName);
        if( iRightCol<0
         || !tableAndColumnIndex(pSrc, i+1, zName, &iLeft, &iLeftCol)
        ){
          sqlite3ErrorMsg(pParse, "cannot join using column %s - column "
            "not present in both tables", zName);
          return 1;
        }
        addWhereTerm(pParse, pSrc, iLeft, iLeftCol, i+1, iRightCol,
                     isOuter, &p->pWhere);
      }
    }
  }
  return 0;
}

static void pushOntoSorter(
  Parse *pParse,         
  ExprList *pOrderBy,    
  Select *pSelect,       
  int regData            
){
  Vdbe *v = pParse->pVdbe;
  int nExpr = pOrderBy->nExpr;
  int regBase = sqlite3GetTempRange(pParse, nExpr+2);
  int regRecord = sqlite3GetTempReg(pParse);
  sqlite3ExprCacheClear(pParse);
  sqlite3ExprCodeExprList(pParse, pOrderBy, regBase, 0);
  sqlite3VdbeAddOp2(v, OP_Sequence, pOrderBy->iECursor, regBase+nExpr);
  sqlite3ExprCodeMove(pParse, regData, regBase+nExpr+1, 1);
  sqlite3VdbeAddOp3(v, OP_MakeRecord, regBase, nExpr + 2, regRecord);
  sqlite3VdbeAddOp2(v, OP_IdxInsert, pOrderBy->iECursor, regRecord);
  sqlite3ReleaseTempReg(pParse, regRecord);
  sqlite3ReleaseTempRange(pParse, regBase, nExpr+2);
  if( pSelect->iLimit ){
    int addr1, addr2;
    int iLimit;
    if( pSelect->iOffset ){
      iLimit = pSelect->iOffset+1;
    }else{
      iLimit = pSelect->iLimit;
    }
    addr1 = sqlite3VdbeAddOp1(v, OP_IfZero, iLimit);
    sqlite3VdbeAddOp2(v, OP_AddImm, iLimit, -1);
    addr2 = sqlite3VdbeAddOp0(v, OP_Goto);
    sqlite3VdbeJumpHere(v, addr1);
    sqlite3VdbeAddOp1(v, OP_Last, pOrderBy->iECursor);
    sqlite3VdbeAddOp1(v, OP_Delete, pOrderBy->iECursor);
    sqlite3VdbeJumpHere(v, addr2);
  }
}

static void codeOffset(
  Vdbe *v,          
  Select *p,        
  int iContinue     
){
  if( p->iOffset && iContinue!=0 ){
    int addr;
    sqlite3VdbeAddOp2(v, OP_AddImm, p->iOffset, -1);
    addr = sqlite3VdbeAddOp1(v, OP_IfNeg, p->iOffset);
    sqlite3VdbeAddOp2(v, OP_Goto, 0, iContinue);
    VdbeComment((v, "skip OFFSET records"));
    sqlite3VdbeJumpHere(v, addr);
  }
}

static void codeDistinct(
  Parse *pParse,     
  int iTab,          
  int addrRepeat,    
  int N,             
  int iMem           
){
  Vdbe *v;
  int r1;

  v = pParse->pVdbe;
  r1 = sqlite3GetTempReg(pParse);
  sqlite3VdbeAddOp4Int(v, OP_Found, iTab, addrRepeat, iMem, N);
  sqlite3VdbeAddOp3(v, OP_MakeRecord, iMem, N, r1);
  sqlite3VdbeAddOp2(v, OP_IdxInsert, iTab, r1);
  sqlite3ReleaseTempReg(pParse, r1);
}

#ifndef SQLITE_OMIT_SUBQUERY
static int checkForMultiColumnSelectError(
  Parse *pParse,       
  SelectDest *pDest,   
  int nExpr            
){
  int eDest = pDest->eDest;
  if( nExpr>1 && (eDest==SRT_Mem || eDest==SRT_Set) ){
    sqlite3ErrorMsg(pParse, "only a single result allowed for "
       "a SELECT that is part of an expression");
    return 1;
  }else{
    return 0;
  }
}
#endif

static void selectInnerLoop(
  Parse *pParse,          
  Select *p,              
  ExprList *pEList,       
  int srcTab,             
  int nColumn,            
  ExprList *pOrderBy,     
  int distinct,           
  SelectDest *pDest,      
  int iContinue,          
  int iBreak              
){
  Vdbe *v = pParse->pVdbe;
  int i;
  int hasDistinct;        
  int regResult;              
  int eDest = pDest->eDest;   
  int iParm = pDest->iParm;   
  int nResultCol;             

  assert( v );
  if( NEVER(v==0) ) return;
  assert( pEList!=0 );
  hasDistinct = distinct>=0;
  if( pOrderBy==0 && !hasDistinct ){
    codeOffset(v, p, iContinue);
  }

  if( nColumn>0 ){
    nResultCol = nColumn;
  }else{
    nResultCol = pEList->nExpr;
  }
  if( pDest->iMem==0 ){
    pDest->iMem = pParse->nMem+1;
    pDest->nMem = nResultCol;
    pParse->nMem += nResultCol;
  }else{ 
    assert( pDest->nMem==nResultCol );
  }
  regResult = pDest->iMem;
  if( nColumn>0 ){
    for(i=0; i<nColumn; i++){
      sqlite3VdbeAddOp3(v, OP_Column, srcTab, i, regResult+i);
    }
  }else if( eDest!=SRT_Exists ){
    sqlite3ExprCacheClear(pParse);
    sqlite3ExprCodeExprList(pParse, pEList, regResult, eDest==SRT_Output);
  }
  nColumn = nResultCol;

  if( hasDistinct ){
    assert( pEList!=0 );
    assert( pEList->nExpr==nColumn );
    codeDistinct(pParse, distinct, iContinue, nColumn, regResult);
    if( pOrderBy==0 ){
      codeOffset(v, p, iContinue);
    }
  }

  switch( eDest ){
#ifndef SQLITE_OMIT_COMPOUND_SELECT
    case SRT_Union: {
      int r1;
      r1 = sqlite3GetTempReg(pParse);
      sqlite3VdbeAddOp3(v, OP_MakeRecord, regResult, nColumn, r1);
      sqlite3VdbeAddOp2(v, OP_IdxInsert, iParm, r1);
      sqlite3ReleaseTempReg(pParse, r1);
      break;
    }

    case SRT_Except: {
      sqlite3VdbeAddOp3(v, OP_IdxDelete, iParm, regResult, nColumn);
      break;
    }
#endif

    case SRT_Table:
    case SRT_EphemTab: {
      int r1 = sqlite3GetTempReg(pParse);
      testcase( eDest==SRT_Table );
      testcase( eDest==SRT_EphemTab );
      sqlite3VdbeAddOp3(v, OP_MakeRecord, regResult, nColumn, r1);
      if( pOrderBy ){
        pushOntoSorter(pParse, pOrderBy, p, r1);
      }else{
        int r2 = sqlite3GetTempReg(pParse);
        sqlite3VdbeAddOp2(v, OP_NewRowid, iParm, r2);
        sqlite3VdbeAddOp3(v, OP_Insert, iParm, r1, r2);
        sqlite3VdbeChangeP5(v, OPFLAG_APPEND);
        sqlite3ReleaseTempReg(pParse, r2);
      }
      sqlite3ReleaseTempReg(pParse, r1);
      break;
    }

#ifndef SQLITE_OMIT_SUBQUERY
    case SRT_Set: {
      assert( nColumn==1 );
      p->affinity = sqlite3CompareAffinity(pEList->a[0].pExpr, pDest->affinity);
      if( pOrderBy ){
        pushOntoSorter(pParse, pOrderBy, p, regResult);
      }else{
        int r1 = sqlite3GetTempReg(pParse);
        sqlite3VdbeAddOp4(v, OP_MakeRecord, regResult, 1, r1, &p->affinity, 1);
        sqlite3ExprCacheAffinityChange(pParse, regResult, 1);
        sqlite3VdbeAddOp2(v, OP_IdxInsert, iParm, r1);
        sqlite3ReleaseTempReg(pParse, r1);
      }
      break;
    }

    case SRT_Exists: {
      sqlite3VdbeAddOp2(v, OP_Integer, 1, iParm);
      
      break;
    }

    case SRT_Mem: {
      assert( nColumn==1 );
      if( pOrderBy ){
        pushOntoSorter(pParse, pOrderBy, p, regResult);
      }else{
        sqlite3ExprCodeMove(pParse, regResult, iParm, 1);
        
      }
      break;
    }
#endif 

    case SRT_Coroutine:
    case SRT_Output: {
      testcase( eDest==SRT_Coroutine );
      testcase( eDest==SRT_Output );
      if( pOrderBy ){
        int r1 = sqlite3GetTempReg(pParse);
        sqlite3VdbeAddOp3(v, OP_MakeRecord, regResult, nColumn, r1);
        pushOntoSorter(pParse, pOrderBy, p, r1);
        sqlite3ReleaseTempReg(pParse, r1);
      }else if( eDest==SRT_Coroutine ){
        sqlite3VdbeAddOp1(v, OP_Yield, pDest->iParm);
      }else{
        sqlite3VdbeAddOp2(v, OP_ResultRow, regResult, nColumn);
        sqlite3ExprCacheAffinityChange(pParse, regResult, nColumn);
      }
      break;
    }

#if !defined(SQLITE_OMIT_TRIGGER)
    default: {
      assert( eDest==SRT_Discard );
      break;
    }
#endif
  }

  if( pOrderBy==0 && p->iLimit ){
    sqlite3VdbeAddOp3(v, OP_IfZero, p->iLimit, iBreak, -1);
  }
}

static KeyInfo *keyInfoFromExprList(Parse *pParse, ExprList *pList){
  sqlite3 *db = pParse->db;
  int nExpr;
  KeyInfo *pInfo;
  struct ExprList_item *pItem;
  int i;

  nExpr = pList->nExpr;
  pInfo = sqlite3DbMallocZero(db, sizeof(*pInfo) + nExpr*(sizeof(CollSeq*)+1) );
  if( pInfo ){
    pInfo->aSortOrder = (u8*)&pInfo->aColl[nExpr];
    pInfo->nField = (u16)nExpr;
    pInfo->enc = ENC(db);
    pInfo->db = db;
    for(i=0, pItem=pList->a; i<nExpr; i++, pItem++){
      CollSeq *pColl;
      pColl = sqlite3ExprCollSeq(pParse, pItem->pExpr);
      if( !pColl ){
        pColl = db->pDfltColl;
      }
      pInfo->aColl[i] = pColl;
      pInfo->aSortOrder[i] = pItem->sortOrder;
    }
  }
  return pInfo;
}

#ifndef SQLITE_OMIT_COMPOUND_SELECT
static const char *selectOpName(int id){
  char *z;
  switch( id ){
    case TK_ALL:       z = "UNION ALL";   break;
    case TK_INTERSECT: z = "INTERSECT";   break;
    case TK_EXCEPT:    z = "EXCEPT";      break;
    default:           z = "UNION";       break;
  }
  return z;
}
#endif 

#ifndef SQLITE_OMIT_EXPLAIN
static void explainTempTable(Parse *pParse, const char *zUsage){
  if( pParse->explain==2 ){
    Vdbe *v = pParse->pVdbe;
    char *zMsg = sqlite3MPrintf(pParse->db, "USE TEMP B-TREE FOR %s", zUsage);
    sqlite3VdbeAddOp4(v, OP_Explain, pParse->iSelectId, 0, 0, zMsg, P4_DYNAMIC);
  }
}

# define explainSetInteger(a, b) a = b

#else
# define explainTempTable(y,z)
# define explainSetInteger(y,z)
#endif

#if !defined(SQLITE_OMIT_EXPLAIN) && !defined(SQLITE_OMIT_COMPOUND_SELECT)
static void explainComposite(
  Parse *pParse,                  
  int op,                         
  int iSub1,                      
  int iSub2,                      
  int bUseTmp                     
){
  assert( op==TK_UNION || op==TK_EXCEPT || op==TK_INTERSECT || op==TK_ALL );
  if( pParse->explain==2 ){
    Vdbe *v = pParse->pVdbe;
    char *zMsg = sqlite3MPrintf(
        pParse->db, "COMPOUND SUBQUERIES %d AND %d %s(%s)", iSub1, iSub2,
        bUseTmp?"USING TEMP B-TREE ":"", selectOpName(op)
    );
    sqlite3VdbeAddOp4(v, OP_Explain, pParse->iSelectId, 0, 0, zMsg, P4_DYNAMIC);
  }
}
#else
# define explainComposite(v,w,x,y,z)
#endif

static void generateSortTail(
  Parse *pParse,    
  Select *p,        
  Vdbe *v,          
  int nColumn,      
  SelectDest *pDest 
){
  int addrBreak = sqlite3VdbeMakeLabel(v);     
  int addrContinue = sqlite3VdbeMakeLabel(v);  
  int addr;
  int iTab;
  int pseudoTab = 0;
  ExprList *pOrderBy = p->pOrderBy;

  int eDest = pDest->eDest;
  int iParm = pDest->iParm;

  int regRow;
  int regRowid;

  iTab = pOrderBy->iECursor;
  regRow = sqlite3GetTempReg(pParse);
  if( eDest==SRT_Output || eDest==SRT_Coroutine ){
    pseudoTab = pParse->nTab++;
    sqlite3VdbeAddOp3(v, OP_OpenPseudo, pseudoTab, regRow, nColumn);
    regRowid = 0;
  }else{
    regRowid = sqlite3GetTempReg(pParse);
  }
  addr = 1 + sqlite3VdbeAddOp2(v, OP_Sort, iTab, addrBreak);
  codeOffset(v, p, addrContinue);
  sqlite3VdbeAddOp3(v, OP_Column, iTab, pOrderBy->nExpr + 1, regRow);
  switch( eDest ){
    case SRT_Table:
    case SRT_EphemTab: {
      testcase( eDest==SRT_Table );
      testcase( eDest==SRT_EphemTab );
      sqlite3VdbeAddOp2(v, OP_NewRowid, iParm, regRowid);
      sqlite3VdbeAddOp3(v, OP_Insert, iParm, regRow, regRowid);
      sqlite3VdbeChangeP5(v, OPFLAG_APPEND);
      break;
    }
#ifndef SQLITE_OMIT_SUBQUERY
    case SRT_Set: {
      assert( nColumn==1 );
      sqlite3VdbeAddOp4(v, OP_MakeRecord, regRow, 1, regRowid, &p->affinity, 1);
      sqlite3ExprCacheAffinityChange(pParse, regRow, 1);
      sqlite3VdbeAddOp2(v, OP_IdxInsert, iParm, regRowid);
      break;
    }
    case SRT_Mem: {
      assert( nColumn==1 );
      sqlite3ExprCodeMove(pParse, regRow, iParm, 1);
      
      break;
    }
#endif
    default: {
      int i;
      assert( eDest==SRT_Output || eDest==SRT_Coroutine ); 
      testcase( eDest==SRT_Output );
      testcase( eDest==SRT_Coroutine );
      for(i=0; i<nColumn; i++){
        assert( regRow!=pDest->iMem+i );
        sqlite3VdbeAddOp3(v, OP_Column, pseudoTab, i, pDest->iMem+i);
        if( i==0 ){
          sqlite3VdbeChangeP5(v, OPFLAG_CLEARCACHE);
        }
      }
      if( eDest==SRT_Output ){
        sqlite3VdbeAddOp2(v, OP_ResultRow, pDest->iMem, nColumn);
        sqlite3ExprCacheAffinityChange(pParse, pDest->iMem, nColumn);
      }else{
        sqlite3VdbeAddOp1(v, OP_Yield, pDest->iParm);
      }
      break;
    }
  }
  sqlite3ReleaseTempReg(pParse, regRow);
  sqlite3ReleaseTempReg(pParse, regRowid);

  sqlite3VdbeResolveLabel(v, addrContinue);
  sqlite3VdbeAddOp2(v, OP_Next, iTab, addr);
  sqlite3VdbeResolveLabel(v, addrBreak);
  if( eDest==SRT_Output || eDest==SRT_Coroutine ){
    sqlite3VdbeAddOp2(v, OP_Close, pseudoTab, 0);
  }
}

static const char *columnType(
  NameContext *pNC, 
  Expr *pExpr,
  const char **pzOriginDb,
  const char **pzOriginTab,
  const char **pzOriginCol
){
  char const *zType = 0;
  char const *zOriginDb = 0;
  char const *zOriginTab = 0;
  char const *zOriginCol = 0;
  int j;
  if( NEVER(pExpr==0) || pNC->pSrcList==0 ) return 0;

  switch( pExpr->op ){
    case TK_AGG_COLUMN:
    case TK_COLUMN: {
      Table *pTab = 0;            
      Select *pS = 0;             
      int iCol = pExpr->iColumn;  
      testcase( pExpr->op==TK_AGG_COLUMN );
      testcase( pExpr->op==TK_COLUMN );
      while( pNC && !pTab ){
        SrcList *pTabList = pNC->pSrcList;
        for(j=0;j<pTabList->nSrc && pTabList->a[j].iCursor!=pExpr->iTable;j++);
        if( j<pTabList->nSrc ){
          pTab = pTabList->a[j].pTab;
          pS = pTabList->a[j].pSelect;
        }else{
          pNC = pNC->pNext;
        }
      }

      if( pTab==0 ){
        break;
      }

      assert( pTab && pExpr->pTab==pTab );
      if( pS ){
        if( iCol>=0 && ALWAYS(iCol<pS->pEList->nExpr) ){
          NameContext sNC;
          Expr *p = pS->pEList->a[iCol].pExpr;
          sNC.pSrcList = pS->pSrc;
          sNC.pNext = pNC;
          sNC.pParse = pNC->pParse;
          zType = columnType(&sNC, p, &zOriginDb, &zOriginTab, &zOriginCol); 
        }
      }else if( ALWAYS(pTab->pSchema) ){
        
        assert( !pS );
        if( iCol<0 ) iCol = pTab->iPKey;
        assert( iCol==-1 || (iCol>=0 && iCol<pTab->nCol) );
        if( iCol<0 ){
          zType = "INTEGER";
          zOriginCol = "rowid";
        }else{
          zType = pTab->aCol[iCol].zType;
          zOriginCol = pTab->aCol[iCol].zName;
        }
        zOriginTab = pTab->zName;
        if( pNC->pParse ){
          int iDb = sqlite3SchemaToIndex(pNC->pParse->db, pTab->pSchema);
          zOriginDb = pNC->pParse->db->aDb[iDb].zName;
        }
      }
      break;
    }
#ifndef SQLITE_OMIT_SUBQUERY
    case TK_SELECT: {
      NameContext sNC;
      Select *pS = pExpr->x.pSelect;
      Expr *p = pS->pEList->a[0].pExpr;
      assert( ExprHasProperty(pExpr, EP_xIsSelect) );
      sNC.pSrcList = pS->pSrc;
      sNC.pNext = pNC;
      sNC.pParse = pNC->pParse;
      zType = columnType(&sNC, p, &zOriginDb, &zOriginTab, &zOriginCol); 
      break;
    }
#endif
  }
  
  if( pzOriginDb ){
    assert( pzOriginTab && pzOriginCol );
    *pzOriginDb = zOriginDb;
    *pzOriginTab = zOriginTab;
    *pzOriginCol = zOriginCol;
  }
  return zType;
}

static void generateColumnTypes(
  Parse *pParse,      
  SrcList *pTabList,  
  ExprList *pEList    
){
#ifndef SQLITE_OMIT_DECLTYPE
  Vdbe *v = pParse->pVdbe;
  int i;
  NameContext sNC;
  sNC.pSrcList = pTabList;
  sNC.pParse = pParse;
  for(i=0; i<pEList->nExpr; i++){
    Expr *p = pEList->a[i].pExpr;
    const char *zType;
#ifdef SQLITE_ENABLE_COLUMN_METADATA
    const char *zOrigDb = 0;
    const char *zOrigTab = 0;
    const char *zOrigCol = 0;
    zType = columnType(&sNC, p, &zOrigDb, &zOrigTab, &zOrigCol);

    sqlite3VdbeSetColName(v, i, COLNAME_DATABASE, zOrigDb, SQLITE_TRANSIENT);
    sqlite3VdbeSetColName(v, i, COLNAME_TABLE, zOrigTab, SQLITE_TRANSIENT);
    sqlite3VdbeSetColName(v, i, COLNAME_COLUMN, zOrigCol, SQLITE_TRANSIENT);
#else
    zType = columnType(&sNC, p, 0, 0, 0);
#endif
    sqlite3VdbeSetColName(v, i, COLNAME_DECLTYPE, zType, SQLITE_TRANSIENT);
  }
#endif 
}

static void generateColumnNames(
  Parse *pParse,      
  SrcList *pTabList,  
  ExprList *pEList    
){
  Vdbe *v = pParse->pVdbe;
  int i, j;
  sqlite3 *db = pParse->db;
  int fullNames, shortNames;

#ifndef SQLITE_OMIT_EXPLAIN
  
  if( pParse->explain ){
    return;
  }
#endif

  if( pParse->colNamesSet || NEVER(v==0) || db->mallocFailed ) return;
  pParse->colNamesSet = 1;
  fullNames = (db->flags & SQLITE_FullColNames)!=0;
  shortNames = (db->flags & SQLITE_ShortColNames)!=0;
  sqlite3VdbeSetNumCols(v, pEList->nExpr);
  for(i=0; i<pEList->nExpr; i++){
    Expr *p;
    p = pEList->a[i].pExpr;
    if( NEVER(p==0) ) continue;
    if( pEList->a[i].zName ){
      char *zName = pEList->a[i].zName;
      sqlite3VdbeSetColName(v, i, COLNAME_NAME, zName, SQLITE_TRANSIENT);
    }else if( (p->op==TK_COLUMN || p->op==TK_AGG_COLUMN) && pTabList ){
      Table *pTab;
      char *zCol;
      int iCol = p->iColumn;
      for(j=0; ALWAYS(j<pTabList->nSrc); j++){
        if( pTabList->a[j].iCursor==p->iTable ) break;
      }
      assert( j<pTabList->nSrc );
      pTab = pTabList->a[j].pTab;
      if( iCol<0 ) iCol = pTab->iPKey;
      assert( iCol==-1 || (iCol>=0 && iCol<pTab->nCol) );
      if( iCol<0 ){
        zCol = "rowid";
      }else{
        zCol = pTab->aCol[iCol].zName;
      }
      if( !shortNames && !fullNames ){
        sqlite3VdbeSetColName(v, i, COLNAME_NAME, 
            sqlite3DbStrDup(db, pEList->a[i].zSpan), SQLITE_DYNAMIC);
      }else if( fullNames ){
        char *zName = 0;
        zName = sqlite3MPrintf(db, "%s.%s", pTab->zName, zCol);
        sqlite3VdbeSetColName(v, i, COLNAME_NAME, zName, SQLITE_DYNAMIC);
      }else{
        sqlite3VdbeSetColName(v, i, COLNAME_NAME, zCol, SQLITE_TRANSIENT);
      }
    }else{
      sqlite3VdbeSetColName(v, i, COLNAME_NAME, 
          sqlite3DbStrDup(db, pEList->a[i].zSpan), SQLITE_DYNAMIC);
    }
  }
  generateColumnTypes(pParse, pTabList, pEList);
}

static int selectColumnsFromExprList(
  Parse *pParse,          
  ExprList *pEList,       
  int *pnCol,             
  Column **paCol          
){
  sqlite3 *db = pParse->db;   
  int i, j;                   
  int cnt;                    
  Column *aCol, *pCol;        
  int nCol;                   
  Expr *p;                    
  char *zName;                
  int nName;                  

  *pnCol = nCol = pEList->nExpr;
  aCol = *paCol = sqlite3DbMallocZero(db, sizeof(aCol[0])*nCol);
  if( aCol==0 ) return SQLITE_NOMEM;
  for(i=0, pCol=aCol; i<nCol; i++, pCol++){
    p = pEList->a[i].pExpr;
    assert( p->pRight==0 || ExprHasProperty(p->pRight, EP_IntValue)
               || p->pRight->u.zToken==0 || p->pRight->u.zToken[0]!=0 );
    if( (zName = pEList->a[i].zName)!=0 ){
      
      zName = sqlite3DbStrDup(db, zName);
    }else{
      Expr *pColExpr = p;  
      Table *pTab;         
      while( pColExpr->op==TK_DOT ) pColExpr = pColExpr->pRight;
      if( pColExpr->op==TK_COLUMN && ALWAYS(pColExpr->pTab!=0) ){
        
        int iCol = pColExpr->iColumn;
        pTab = pColExpr->pTab;
        if( iCol<0 ) iCol = pTab->iPKey;
        zName = sqlite3MPrintf(db, "%s",
                 iCol>=0 ? pTab->aCol[iCol].zName : "rowid");
      }else if( pColExpr->op==TK_ID ){
        assert( !ExprHasProperty(pColExpr, EP_IntValue) );
        zName = sqlite3MPrintf(db, "%s", pColExpr->u.zToken);
      }else{
        
        zName = sqlite3MPrintf(db, "%s", pEList->a[i].zSpan);
      }
    }
    if( db->mallocFailed ){
      sqlite3DbFree(db, zName);
      break;
    }

    nName = sqlite3Strlen30(zName);
    for(j=cnt=0; j<i; j++){
      if( sqlite3StrICmp(aCol[j].zName, zName)==0 ){
        char *zNewName;
        zName[nName] = 0;
        zNewName = sqlite3MPrintf(db, "%s:%d", zName, ++cnt);
        sqlite3DbFree(db, zName);
        zName = zNewName;
        j = -1;
        if( zName==0 ) break;
      }
    }
    pCol->zName = zName;
  }
  if( db->mallocFailed ){
    for(j=0; j<i; j++){
      sqlite3DbFree(db, aCol[j].zName);
    }
    sqlite3DbFree(db, aCol);
    *paCol = 0;
    *pnCol = 0;
    return SQLITE_NOMEM;
  }
  return SQLITE_OK;
}

static void selectAddColumnTypeAndCollation(
  Parse *pParse,        
  int nCol,             
  Column *aCol,         
  Select *pSelect       
){
  sqlite3 *db = pParse->db;
  NameContext sNC;
  Column *pCol;
  CollSeq *pColl;
  int i;
  Expr *p;
  struct ExprList_item *a;

  assert( pSelect!=0 );
  assert( (pSelect->selFlags & SF_Resolved)!=0 );
  assert( nCol==pSelect->pEList->nExpr || db->mallocFailed );
  if( db->mallocFailed ) return;
  memset(&sNC, 0, sizeof(sNC));
  sNC.pSrcList = pSelect->pSrc;
  a = pSelect->pEList->a;
  for(i=0, pCol=aCol; i<nCol; i++, pCol++){
    p = a[i].pExpr;
    pCol->zType = sqlite3DbStrDup(db, columnType(&sNC, p, 0, 0, 0));
    pCol->affinity = sqlite3ExprAffinity(p);
    if( pCol->affinity==0 ) pCol->affinity = SQLITE_AFF_NONE;
    pColl = sqlite3ExprCollSeq(pParse, p);
    if( pColl ){
      pCol->zColl = sqlite3DbStrDup(db, pColl->zName);
    }
  }
}

Table *sqlite3ResultSetOfSelect(Parse *pParse, Select *pSelect){
  Table *pTab;
  sqlite3 *db = pParse->db;
  int savedFlags;

  savedFlags = db->flags;
  db->flags &= ~SQLITE_FullColNames;
  db->flags |= SQLITE_ShortColNames;
  sqlite3SelectPrep(pParse, pSelect, 0);
  if( pParse->nErr ) return 0;
  while( pSelect->pPrior ) pSelect = pSelect->pPrior;
  db->flags = savedFlags;
  pTab = sqlite3DbMallocZero(db, sizeof(Table) );
  if( pTab==0 ){
    return 0;
  }
  assert( db->lookaside.bEnabled==0 );
  pTab->nRef = 1;
  pTab->zName = 0;
  pTab->nRowEst = 1000000;
  selectColumnsFromExprList(pParse, pSelect->pEList, &pTab->nCol, &pTab->aCol);
  selectAddColumnTypeAndCollation(pParse, pTab->nCol, pTab->aCol, pSelect);
  pTab->iPKey = -1;
  if( db->mallocFailed ){
    sqlite3DeleteTable(db, pTab);
    return 0;
  }
  return pTab;
}

Vdbe *sqlite3GetVdbe(Parse *pParse){
  Vdbe *v = pParse->pVdbe;
  if( v==0 ){
    v = pParse->pVdbe = sqlite3VdbeCreate(pParse->db);
#ifndef SQLITE_OMIT_TRACE
    if( v ){
      sqlite3VdbeAddOp0(v, OP_Trace);
    }
#endif
  }
  return v;
}


static void computeLimitRegisters(Parse *pParse, Select *p, int iBreak){
  Vdbe *v = 0;
  int iLimit = 0;
  int iOffset;
  int addr1, n;
  if( p->iLimit ) return;

  sqlite3ExprCacheClear(pParse);
  assert( p->pOffset==0 || p->pLimit!=0 );
  if( p->pLimit ){
    p->iLimit = iLimit = ++pParse->nMem;
    v = sqlite3GetVdbe(pParse);
    if( NEVER(v==0) ) return;  
    if( sqlite3ExprIsInteger(p->pLimit, &n) ){
      sqlite3VdbeAddOp2(v, OP_Integer, n, iLimit);
      VdbeComment((v, "LIMIT counter"));
      if( n==0 ){
        sqlite3VdbeAddOp2(v, OP_Goto, 0, iBreak);
      }else{
        if( p->nSelectRow > (double)n ) p->nSelectRow = (double)n;
      }
    }else{
      sqlite3ExprCode(pParse, p->pLimit, iLimit);
      sqlite3VdbeAddOp1(v, OP_MustBeInt, iLimit);
      VdbeComment((v, "LIMIT counter"));
      sqlite3VdbeAddOp2(v, OP_IfZero, iLimit, iBreak);
    }
    if( p->pOffset ){
      p->iOffset = iOffset = ++pParse->nMem;
      pParse->nMem++;   
      sqlite3ExprCode(pParse, p->pOffset, iOffset);
      sqlite3VdbeAddOp1(v, OP_MustBeInt, iOffset);
      VdbeComment((v, "OFFSET counter"));
      addr1 = sqlite3VdbeAddOp1(v, OP_IfPos, iOffset);
      sqlite3VdbeAddOp2(v, OP_Integer, 0, iOffset);
      sqlite3VdbeJumpHere(v, addr1);
      sqlite3VdbeAddOp3(v, OP_Add, iLimit, iOffset, iOffset+1);
      VdbeComment((v, "LIMIT+OFFSET"));
      addr1 = sqlite3VdbeAddOp1(v, OP_IfPos, iLimit);
      sqlite3VdbeAddOp2(v, OP_Integer, -1, iOffset+1);
      sqlite3VdbeJumpHere(v, addr1);
    }
  }
}

#ifndef SQLITE_OMIT_COMPOUND_SELECT
static CollSeq *multiSelectCollSeq(Parse *pParse, Select *p, int iCol){
  CollSeq *pRet;
  if( p->pPrior ){
    pRet = multiSelectCollSeq(pParse, p->pPrior, iCol);
  }else{
    pRet = 0;
  }
  assert( iCol>=0 );
  if( pRet==0 && iCol<p->pEList->nExpr ){
    pRet = sqlite3ExprCollSeq(pParse, p->pEList->a[iCol].pExpr);
  }
  return pRet;
}
#endif 

static int multiSelectOrderBy(
  Parse *pParse,        
  Select *p,            
  SelectDest *pDest     
);


#ifndef SQLITE_OMIT_COMPOUND_SELECT
/*
** This routine is called to process a compound query form from
** two or more separate queries using UNION, UNION ALL, EXCEPT, or
** INTERSECT
**
** "p" points to the right-most of the two queries.  the query on the
** left is p->pPrior.  The left query could also be a compound query
** in which case this routine will be called recursively. 
**
** The results of the total query are to be written into a destination
** of type eDest with parameter iParm.
**
** Example 1:  Consider a three-way compound SQL statement.
**
**     SELECT a FROM t1 UNION SELECT b FROM t2 UNION SELECT c FROM t3
**
** This statement is parsed up as follows:
**
**     SELECT c FROM t3
**      |
**      `----->  SELECT b FROM t2
**                |
**                `------>  SELECT a FROM t1
**
** The arrows in the diagram above represent the Select.pPrior pointer.
** So if this routine is called with p equal to the t3 query, then
** pPrior will be the t2 query.  p->op will be TK_UNION in this case.
**
** Notice that because of the way SQLite parses compound SELECTs, the
** individual selects always group from left to right.
*/
static int multiSelect(
  Parse *pParse,        
  Select *p,            
  SelectDest *pDest     
){
  int rc = SQLITE_OK;   
  Select *pPrior;       
  Vdbe *v;              
  SelectDest dest;      
  Select *pDelete = 0;  
  sqlite3 *db;          
#ifndef SQLITE_OMIT_EXPLAIN
  int iSub1;            
  int iSub2;            
#endif

  assert( p && p->pPrior );  
  db = pParse->db;
  pPrior = p->pPrior;
  assert( pPrior->pRightmost!=pPrior );
  assert( pPrior->pRightmost==p->pRightmost );
  dest = *pDest;
  if( pPrior->pOrderBy ){
    sqlite3ErrorMsg(pParse,"ORDER BY clause should come after %s not before",
      selectOpName(p->op));
    rc = 1;
    goto multi_select_end;
  }
  if( pPrior->pLimit ){
    sqlite3ErrorMsg(pParse,"LIMIT clause should come after %s not before",
      selectOpName(p->op));
    rc = 1;
    goto multi_select_end;
  }

  v = sqlite3GetVdbe(pParse);
  assert( v!=0 );  

  if( dest.eDest==SRT_EphemTab ){
    assert( p->pEList );
    sqlite3VdbeAddOp2(v, OP_OpenEphemeral, dest.iParm, p->pEList->nExpr);
    sqlite3VdbeChangeP5(v, BTREE_UNORDERED);
    dest.eDest = SRT_Table;
  }

  assert( p->pEList && pPrior->pEList );
  if( p->pEList->nExpr!=pPrior->pEList->nExpr ){
    sqlite3ErrorMsg(pParse, "SELECTs to the left and right of %s"
      " do not have the same number of result columns", selectOpName(p->op));
    rc = 1;
    goto multi_select_end;
  }

  if( p->pOrderBy ){
    return multiSelectOrderBy(pParse, p, pDest);
  }

  switch( p->op ){
    case TK_ALL: {
      int addr = 0;
      int nLimit;
      assert( !pPrior->pLimit );
      pPrior->pLimit = p->pLimit;
      pPrior->pOffset = p->pOffset;
      explainSetInteger(iSub1, pParse->iNextSelectId);
      rc = sqlite3Select(pParse, pPrior, &dest);
      p->pLimit = 0;
      p->pOffset = 0;
      if( rc ){
        goto multi_select_end;
      }
      p->pPrior = 0;
      p->iLimit = pPrior->iLimit;
      p->iOffset = pPrior->iOffset;
      if( p->iLimit ){
        addr = sqlite3VdbeAddOp1(v, OP_IfZero, p->iLimit);
        VdbeComment((v, "Jump ahead if LIMIT reached"));
      }
      explainSetInteger(iSub2, pParse->iNextSelectId);
      rc = sqlite3Select(pParse, p, &dest);
      testcase( rc!=SQLITE_OK );
      pDelete = p->pPrior;
      p->pPrior = pPrior;
      p->nSelectRow += pPrior->nSelectRow;
      if( pPrior->pLimit
       && sqlite3ExprIsInteger(pPrior->pLimit, &nLimit)
       && p->nSelectRow > (double)nLimit 
      ){
        p->nSelectRow = (double)nLimit;
      }
      if( addr ){
        sqlite3VdbeJumpHere(v, addr);
      }
      break;
    }
    case TK_EXCEPT:
    case TK_UNION: {
      int unionTab;    
      u8 op = 0;       
      int priorOp;     
      Expr *pLimit, *pOffset; 
      int addr;
      SelectDest uniondest;

      testcase( p->op==TK_EXCEPT );
      testcase( p->op==TK_UNION );
      priorOp = SRT_Union;
      if( dest.eDest==priorOp && ALWAYS(!p->pLimit &&!p->pOffset) ){
        assert( p->pRightmost!=p );  
        assert( p->pLimit==0 );      
        assert( p->pOffset==0 );     
        unionTab = dest.iParm;
      }else{
        unionTab = pParse->nTab++;
        assert( p->pOrderBy==0 );
        addr = sqlite3VdbeAddOp2(v, OP_OpenEphemeral, unionTab, 0);
        assert( p->addrOpenEphm[0] == -1 );
        p->addrOpenEphm[0] = addr;
        p->pRightmost->selFlags |= SF_UsesEphemeral;
        assert( p->pEList );
      }

      assert( !pPrior->pOrderBy );
      sqlite3SelectDestInit(&uniondest, priorOp, unionTab);
      explainSetInteger(iSub1, pParse->iNextSelectId);
      rc = sqlite3Select(pParse, pPrior, &uniondest);
      if( rc ){
        goto multi_select_end;
      }

      if( p->op==TK_EXCEPT ){
        op = SRT_Except;
      }else{
        assert( p->op==TK_UNION );
        op = SRT_Union;
      }
      p->pPrior = 0;
      pLimit = p->pLimit;
      p->pLimit = 0;
      pOffset = p->pOffset;
      p->pOffset = 0;
      uniondest.eDest = op;
      explainSetInteger(iSub2, pParse->iNextSelectId);
      rc = sqlite3Select(pParse, p, &uniondest);
      testcase( rc!=SQLITE_OK );
      sqlite3ExprListDelete(db, p->pOrderBy);
      pDelete = p->pPrior;
      p->pPrior = pPrior;
      p->pOrderBy = 0;
      if( p->op==TK_UNION ) p->nSelectRow += pPrior->nSelectRow;
      sqlite3ExprDelete(db, p->pLimit);
      p->pLimit = pLimit;
      p->pOffset = pOffset;
      p->iLimit = 0;
      p->iOffset = 0;

      assert( unionTab==dest.iParm || dest.eDest!=priorOp );
      if( dest.eDest!=priorOp ){
        int iCont, iBreak, iStart;
        assert( p->pEList );
        if( dest.eDest==SRT_Output ){
          Select *pFirst = p;
          while( pFirst->pPrior ) pFirst = pFirst->pPrior;
          generateColumnNames(pParse, 0, pFirst->pEList);
        }
        iBreak = sqlite3VdbeMakeLabel(v);
        iCont = sqlite3VdbeMakeLabel(v);
        computeLimitRegisters(pParse, p, iBreak);
        sqlite3VdbeAddOp2(v, OP_Rewind, unionTab, iBreak);
        iStart = sqlite3VdbeCurrentAddr(v);
        selectInnerLoop(pParse, p, p->pEList, unionTab, p->pEList->nExpr,
                        0, -1, &dest, iCont, iBreak);
        sqlite3VdbeResolveLabel(v, iCont);
        sqlite3VdbeAddOp2(v, OP_Next, unionTab, iStart);
        sqlite3VdbeResolveLabel(v, iBreak);
        sqlite3VdbeAddOp2(v, OP_Close, unionTab, 0);
      }
      break;
    }
    default: assert( p->op==TK_INTERSECT ); {
      int tab1, tab2;
      int iCont, iBreak, iStart;
      Expr *pLimit, *pOffset;
      int addr;
      SelectDest intersectdest;
      int r1;

      tab1 = pParse->nTab++;
      tab2 = pParse->nTab++;
      assert( p->pOrderBy==0 );

      addr = sqlite3VdbeAddOp2(v, OP_OpenEphemeral, tab1, 0);
      assert( p->addrOpenEphm[0] == -1 );
      p->addrOpenEphm[0] = addr;
      p->pRightmost->selFlags |= SF_UsesEphemeral;
      assert( p->pEList );

      sqlite3SelectDestInit(&intersectdest, SRT_Union, tab1);
      explainSetInteger(iSub1, pParse->iNextSelectId);
      rc = sqlite3Select(pParse, pPrior, &intersectdest);
      if( rc ){
        goto multi_select_end;
      }

      addr = sqlite3VdbeAddOp2(v, OP_OpenEphemeral, tab2, 0);
      assert( p->addrOpenEphm[1] == -1 );
      p->addrOpenEphm[1] = addr;
      p->pPrior = 0;
      pLimit = p->pLimit;
      p->pLimit = 0;
      pOffset = p->pOffset;
      p->pOffset = 0;
      intersectdest.iParm = tab2;
      explainSetInteger(iSub2, pParse->iNextSelectId);
      rc = sqlite3Select(pParse, p, &intersectdest);
      testcase( rc!=SQLITE_OK );
      pDelete = p->pPrior;
      p->pPrior = pPrior;
      if( p->nSelectRow>pPrior->nSelectRow ) p->nSelectRow = pPrior->nSelectRow;
      sqlite3ExprDelete(db, p->pLimit);
      p->pLimit = pLimit;
      p->pOffset = pOffset;

      assert( p->pEList );
      if( dest.eDest==SRT_Output ){
        Select *pFirst = p;
        while( pFirst->pPrior ) pFirst = pFirst->pPrior;
        generateColumnNames(pParse, 0, pFirst->pEList);
      }
      iBreak = sqlite3VdbeMakeLabel(v);
      iCont = sqlite3VdbeMakeLabel(v);
      computeLimitRegisters(pParse, p, iBreak);
      sqlite3VdbeAddOp2(v, OP_Rewind, tab1, iBreak);
      r1 = sqlite3GetTempReg(pParse);
      iStart = sqlite3VdbeAddOp2(v, OP_RowKey, tab1, r1);
      sqlite3VdbeAddOp4Int(v, OP_NotFound, tab2, iCont, r1, 0);
      sqlite3ReleaseTempReg(pParse, r1);
      selectInnerLoop(pParse, p, p->pEList, tab1, p->pEList->nExpr,
                      0, -1, &dest, iCont, iBreak);
      sqlite3VdbeResolveLabel(v, iCont);
      sqlite3VdbeAddOp2(v, OP_Next, tab1, iStart);
      sqlite3VdbeResolveLabel(v, iBreak);
      sqlite3VdbeAddOp2(v, OP_Close, tab2, 0);
      sqlite3VdbeAddOp2(v, OP_Close, tab1, 0);
      break;
    }
  }

  explainComposite(pParse, p->op, iSub1, iSub2, p->op!=TK_ALL);

  if( p->selFlags & SF_UsesEphemeral ){
    int i;                        
    KeyInfo *pKeyInfo;            
    Select *pLoop;                
    CollSeq **apColl;             
    int nCol;                     

    assert( p->pRightmost==p );
    nCol = p->pEList->nExpr;
    pKeyInfo = sqlite3DbMallocZero(db,
                       sizeof(*pKeyInfo)+nCol*(sizeof(CollSeq*) + 1));
    if( !pKeyInfo ){
      rc = SQLITE_NOMEM;
      goto multi_select_end;
    }

    pKeyInfo->enc = ENC(db);
    pKeyInfo->nField = (u16)nCol;

    for(i=0, apColl=pKeyInfo->aColl; i<nCol; i++, apColl++){
      *apColl = multiSelectCollSeq(pParse, p, i);
      if( 0==*apColl ){
        *apColl = db->pDfltColl;
      }
    }

    for(pLoop=p; pLoop; pLoop=pLoop->pPrior){
      for(i=0; i<2; i++){
        int addr = pLoop->addrOpenEphm[i];
        if( addr<0 ){
          assert( pLoop->addrOpenEphm[1]<0 );
          break;
        }
        sqlite3VdbeChangeP2(v, addr, nCol);
        sqlite3VdbeChangeP4(v, addr, (char*)pKeyInfo, P4_KEYINFO);
        pLoop->addrOpenEphm[i] = -1;
      }
    }
    sqlite3DbFree(db, pKeyInfo);
  }

multi_select_end:
  pDest->iMem = dest.iMem;
  pDest->nMem = dest.nMem;
  sqlite3SelectDelete(db, pDelete);
  return rc;
}
#endif 

static int generateOutputSubroutine(
  Parse *pParse,          
  Select *p,              
  SelectDest *pIn,        
  SelectDest *pDest,      
  int regReturn,          
  int regPrev,            
  KeyInfo *pKeyInfo,      
  int p4type,             
  int iBreak              
){
  Vdbe *v = pParse->pVdbe;
  int iContinue;
  int addr;

  addr = sqlite3VdbeCurrentAddr(v);
  iContinue = sqlite3VdbeMakeLabel(v);

  if( regPrev ){
    int j1, j2;
    j1 = sqlite3VdbeAddOp1(v, OP_IfNot, regPrev);
    j2 = sqlite3VdbeAddOp4(v, OP_Compare, pIn->iMem, regPrev+1, pIn->nMem,
                              (char*)pKeyInfo, p4type);
    sqlite3VdbeAddOp3(v, OP_Jump, j2+2, iContinue, j2+2);
    sqlite3VdbeJumpHere(v, j1);
    sqlite3ExprCodeCopy(pParse, pIn->iMem, regPrev+1, pIn->nMem);
    sqlite3VdbeAddOp2(v, OP_Integer, 1, regPrev);
  }
  if( pParse->db->mallocFailed ) return 0;

  codeOffset(v, p, iContinue);

  switch( pDest->eDest ){
    case SRT_Table:
    case SRT_EphemTab: {
      int r1 = sqlite3GetTempReg(pParse);
      int r2 = sqlite3GetTempReg(pParse);
      testcase( pDest->eDest==SRT_Table );
      testcase( pDest->eDest==SRT_EphemTab );
      sqlite3VdbeAddOp3(v, OP_MakeRecord, pIn->iMem, pIn->nMem, r1);
      sqlite3VdbeAddOp2(v, OP_NewRowid, pDest->iParm, r2);
      sqlite3VdbeAddOp3(v, OP_Insert, pDest->iParm, r1, r2);
      sqlite3VdbeChangeP5(v, OPFLAG_APPEND);
      sqlite3ReleaseTempReg(pParse, r2);
      sqlite3ReleaseTempReg(pParse, r1);
      break;
    }

#ifndef SQLITE_OMIT_SUBQUERY
    case SRT_Set: {
      int r1;
      assert( pIn->nMem==1 );
      p->affinity = 
         sqlite3CompareAffinity(p->pEList->a[0].pExpr, pDest->affinity);
      r1 = sqlite3GetTempReg(pParse);
      sqlite3VdbeAddOp4(v, OP_MakeRecord, pIn->iMem, 1, r1, &p->affinity, 1);
      sqlite3ExprCacheAffinityChange(pParse, pIn->iMem, 1);
      sqlite3VdbeAddOp2(v, OP_IdxInsert, pDest->iParm, r1);
      sqlite3ReleaseTempReg(pParse, r1);
      break;
    }

#if 0  
    case SRT_Exists: {
      sqlite3VdbeAddOp2(v, OP_Integer, 1, pDest->iParm);
      
      break;
    }
#endif

    case SRT_Mem: {
      assert( pIn->nMem==1 );
      sqlite3ExprCodeMove(pParse, pIn->iMem, pDest->iParm, 1);
      
      break;
    }
#endif 

    case SRT_Coroutine: {
      if( pDest->iMem==0 ){
        pDest->iMem = sqlite3GetTempRange(pParse, pIn->nMem);
        pDest->nMem = pIn->nMem;
      }
      sqlite3ExprCodeMove(pParse, pIn->iMem, pDest->iMem, pDest->nMem);
      sqlite3VdbeAddOp1(v, OP_Yield, pDest->iParm);
      break;
    }

    default: {
      assert( pDest->eDest==SRT_Output );
      sqlite3VdbeAddOp2(v, OP_ResultRow, pIn->iMem, pIn->nMem);
      sqlite3ExprCacheAffinityChange(pParse, pIn->iMem, pIn->nMem);
      break;
    }
  }

  if( p->iLimit ){
    sqlite3VdbeAddOp3(v, OP_IfZero, p->iLimit, iBreak, -1);
  }

  sqlite3VdbeResolveLabel(v, iContinue);
  sqlite3VdbeAddOp1(v, OP_Return, regReturn);

  return addr;
}

#ifndef SQLITE_OMIT_COMPOUND_SELECT
static int multiSelectOrderBy(
  Parse *pParse,        
  Select *p,            
  SelectDest *pDest     
){
  int i, j;             
  Select *pPrior;       
  Vdbe *v;              
  SelectDest destA;     
  SelectDest destB;     
  int regAddrA;         
  int regEofA;          
  int regAddrB;         
  int regEofB;          
  int addrSelectA;      
  int addrSelectB;      
  int regOutA;          
  int regOutB;          
  int addrOutA;         
  int addrOutB = 0;     
  int addrEofA;         
  int addrEofB;         
  int addrAltB;         
  int addrAeqB;         
  int addrAgtB;         
  int regLimitA;        
  int regLimitB;        
  int regPrev;          
  int savedLimit;       
  int savedOffset;      
  int labelCmpr;        
  int labelEnd;         
  int j1;               
  int op;               
  KeyInfo *pKeyDup = 0; 
  KeyInfo *pKeyMerge;   
  sqlite3 *db;          
  ExprList *pOrderBy;   
  int nOrderBy;         
  int *aPermute;        
#ifndef SQLITE_OMIT_EXPLAIN
  int iSub1;            
  int iSub2;            
#endif

  assert( p->pOrderBy!=0 );
  assert( pKeyDup==0 ); 
  db = pParse->db;
  v = pParse->pVdbe;
  assert( v!=0 );       
  labelEnd = sqlite3VdbeMakeLabel(v);
  labelCmpr = sqlite3VdbeMakeLabel(v);


  op = p->op;  
  pPrior = p->pPrior;
  assert( pPrior->pOrderBy==0 );
  pOrderBy = p->pOrderBy;
  assert( pOrderBy );
  nOrderBy = pOrderBy->nExpr;

  if( op!=TK_ALL ){
    for(i=1; db->mallocFailed==0 && i<=p->pEList->nExpr; i++){
      struct ExprList_item *pItem;
      for(j=0, pItem=pOrderBy->a; j<nOrderBy; j++, pItem++){
        assert( pItem->iCol>0 );
        if( pItem->iCol==i ) break;
      }
      if( j==nOrderBy ){
        Expr *pNew = sqlite3Expr(db, TK_INTEGER, 0);
        if( pNew==0 ) return SQLITE_NOMEM;
        pNew->flags |= EP_IntValue;
        pNew->u.iValue = i;
        pOrderBy = sqlite3ExprListAppend(pParse, pOrderBy, pNew);
        pOrderBy->a[nOrderBy++].iCol = (u16)i;
      }
    }
  }

  aPermute = sqlite3DbMallocRaw(db, sizeof(int)*nOrderBy);
  if( aPermute ){
    struct ExprList_item *pItem;
    for(i=0, pItem=pOrderBy->a; i<nOrderBy; i++, pItem++){
      assert( pItem->iCol>0  && pItem->iCol<=p->pEList->nExpr );
      aPermute[i] = pItem->iCol - 1;
    }
    pKeyMerge =
      sqlite3DbMallocRaw(db, sizeof(*pKeyMerge)+nOrderBy*(sizeof(CollSeq*)+1));
    if( pKeyMerge ){
      pKeyMerge->aSortOrder = (u8*)&pKeyMerge->aColl[nOrderBy];
      pKeyMerge->nField = (u16)nOrderBy;
      pKeyMerge->enc = ENC(db);
      for(i=0; i<nOrderBy; i++){
        CollSeq *pColl;
        Expr *pTerm = pOrderBy->a[i].pExpr;
        if( pTerm->flags & EP_ExpCollate ){
          pColl = pTerm->pColl;
        }else{
          pColl = multiSelectCollSeq(pParse, p, aPermute[i]);
          pTerm->flags |= EP_ExpCollate;
          pTerm->pColl = pColl;
        }
        pKeyMerge->aColl[i] = pColl;
        pKeyMerge->aSortOrder[i] = pOrderBy->a[i].sortOrder;
      }
    }
  }else{
    pKeyMerge = 0;
  }

  p->pOrderBy = pOrderBy;
  pPrior->pOrderBy = sqlite3ExprListDup(pParse->db, pOrderBy, 0);

  if( op==TK_ALL ){
    regPrev = 0;
  }else{
    int nExpr = p->pEList->nExpr;
    assert( nOrderBy>=nExpr || db->mallocFailed );
    regPrev = sqlite3GetTempRange(pParse, nExpr+1);
    sqlite3VdbeAddOp2(v, OP_Integer, 0, regPrev);
    pKeyDup = sqlite3DbMallocZero(db,
                  sizeof(*pKeyDup) + nExpr*(sizeof(CollSeq*)+1) );
    if( pKeyDup ){
      pKeyDup->aSortOrder = (u8*)&pKeyDup->aColl[nExpr];
      pKeyDup->nField = (u16)nExpr;
      pKeyDup->enc = ENC(db);
      for(i=0; i<nExpr; i++){
        pKeyDup->aColl[i] = multiSelectCollSeq(pParse, p, i);
        pKeyDup->aSortOrder[i] = 0;
      }
    }
  }
 
  p->pPrior = 0;
  sqlite3ResolveOrderGroupBy(pParse, p, p->pOrderBy, "ORDER");
  if( pPrior->pPrior==0 ){
    sqlite3ResolveOrderGroupBy(pParse, pPrior, pPrior->pOrderBy, "ORDER");
  }

  
  computeLimitRegisters(pParse, p, labelEnd);
  if( p->iLimit && op==TK_ALL ){
    regLimitA = ++pParse->nMem;
    regLimitB = ++pParse->nMem;
    sqlite3VdbeAddOp2(v, OP_Copy, p->iOffset ? p->iOffset+1 : p->iLimit,
                                  regLimitA);
    sqlite3VdbeAddOp2(v, OP_Copy, regLimitA, regLimitB);
  }else{
    regLimitA = regLimitB = 0;
  }
  sqlite3ExprDelete(db, p->pLimit);
  p->pLimit = 0;
  sqlite3ExprDelete(db, p->pOffset);
  p->pOffset = 0;

  regAddrA = ++pParse->nMem;
  regEofA = ++pParse->nMem;
  regAddrB = ++pParse->nMem;
  regEofB = ++pParse->nMem;
  regOutA = ++pParse->nMem;
  regOutB = ++pParse->nMem;
  sqlite3SelectDestInit(&destA, SRT_Coroutine, regAddrA);
  sqlite3SelectDestInit(&destB, SRT_Coroutine, regAddrB);

  j1 = sqlite3VdbeAddOp0(v, OP_Goto);
  addrSelectA = sqlite3VdbeCurrentAddr(v);


  VdbeNoopComment((v, "Begin coroutine for left SELECT"));
  pPrior->iLimit = regLimitA;
  explainSetInteger(iSub1, pParse->iNextSelectId);
  sqlite3Select(pParse, pPrior, &destA);
  sqlite3VdbeAddOp2(v, OP_Integer, 1, regEofA);
  sqlite3VdbeAddOp1(v, OP_Yield, regAddrA);
  VdbeNoopComment((v, "End coroutine for left SELECT"));

  addrSelectB = sqlite3VdbeCurrentAddr(v);
  VdbeNoopComment((v, "Begin coroutine for right SELECT"));
  savedLimit = p->iLimit;
  savedOffset = p->iOffset;
  p->iLimit = regLimitB;
  p->iOffset = 0;  
  explainSetInteger(iSub2, pParse->iNextSelectId);
  sqlite3Select(pParse, p, &destB);
  p->iLimit = savedLimit;
  p->iOffset = savedOffset;
  sqlite3VdbeAddOp2(v, OP_Integer, 1, regEofB);
  sqlite3VdbeAddOp1(v, OP_Yield, regAddrB);
  VdbeNoopComment((v, "End coroutine for right SELECT"));

  VdbeNoopComment((v, "Output routine for A"));
  addrOutA = generateOutputSubroutine(pParse,
                 p, &destA, pDest, regOutA,
                 regPrev, pKeyDup, P4_KEYINFO_HANDOFF, labelEnd);
  
  if( op==TK_ALL || op==TK_UNION ){
    VdbeNoopComment((v, "Output routine for B"));
    addrOutB = generateOutputSubroutine(pParse,
                 p, &destB, pDest, regOutB,
                 regPrev, pKeyDup, P4_KEYINFO_STATIC, labelEnd);
  }

  VdbeNoopComment((v, "eof-A subroutine"));
  if( op==TK_EXCEPT || op==TK_INTERSECT ){
    addrEofA = sqlite3VdbeAddOp2(v, OP_Goto, 0, labelEnd);
  }else{  
    addrEofA = sqlite3VdbeAddOp2(v, OP_If, regEofB, labelEnd);
    sqlite3VdbeAddOp2(v, OP_Gosub, regOutB, addrOutB);
    sqlite3VdbeAddOp1(v, OP_Yield, regAddrB);
    sqlite3VdbeAddOp2(v, OP_Goto, 0, addrEofA);
    p->nSelectRow += pPrior->nSelectRow;
  }

  if( op==TK_INTERSECT ){
    addrEofB = addrEofA;
    if( p->nSelectRow > pPrior->nSelectRow ) p->nSelectRow = pPrior->nSelectRow;
  }else{  
    VdbeNoopComment((v, "eof-B subroutine"));
    addrEofB = sqlite3VdbeAddOp2(v, OP_If, regEofA, labelEnd);
    sqlite3VdbeAddOp2(v, OP_Gosub, regOutA, addrOutA);
    sqlite3VdbeAddOp1(v, OP_Yield, regAddrA);
    sqlite3VdbeAddOp2(v, OP_Goto, 0, addrEofB);
  }

  VdbeNoopComment((v, "A-lt-B subroutine"));
  addrAltB = sqlite3VdbeAddOp2(v, OP_Gosub, regOutA, addrOutA);
  sqlite3VdbeAddOp1(v, OP_Yield, regAddrA);
  sqlite3VdbeAddOp2(v, OP_If, regEofA, addrEofA);
  sqlite3VdbeAddOp2(v, OP_Goto, 0, labelCmpr);

  if( op==TK_ALL ){
    addrAeqB = addrAltB;
  }else if( op==TK_INTERSECT ){
    addrAeqB = addrAltB;
    addrAltB++;
  }else{
    VdbeNoopComment((v, "A-eq-B subroutine"));
    addrAeqB =
    sqlite3VdbeAddOp1(v, OP_Yield, regAddrA);
    sqlite3VdbeAddOp2(v, OP_If, regEofA, addrEofA);
    sqlite3VdbeAddOp2(v, OP_Goto, 0, labelCmpr);
  }

  VdbeNoopComment((v, "A-gt-B subroutine"));
  addrAgtB = sqlite3VdbeCurrentAddr(v);
  if( op==TK_ALL || op==TK_UNION ){
    sqlite3VdbeAddOp2(v, OP_Gosub, regOutB, addrOutB);
  }
  sqlite3VdbeAddOp1(v, OP_Yield, regAddrB);
  sqlite3VdbeAddOp2(v, OP_If, regEofB, addrEofB);
  sqlite3VdbeAddOp2(v, OP_Goto, 0, labelCmpr);

  sqlite3VdbeJumpHere(v, j1);
  sqlite3VdbeAddOp2(v, OP_Integer, 0, regEofA);
  sqlite3VdbeAddOp2(v, OP_Integer, 0, regEofB);
  sqlite3VdbeAddOp2(v, OP_Gosub, regAddrA, addrSelectA);
  sqlite3VdbeAddOp2(v, OP_Gosub, regAddrB, addrSelectB);
  sqlite3VdbeAddOp2(v, OP_If, regEofA, addrEofA);
  sqlite3VdbeAddOp2(v, OP_If, regEofB, addrEofB);

  sqlite3VdbeResolveLabel(v, labelCmpr);
  sqlite3VdbeAddOp4(v, OP_Permutation, 0, 0, 0, (char*)aPermute, P4_INTARRAY);
  sqlite3VdbeAddOp4(v, OP_Compare, destA.iMem, destB.iMem, nOrderBy,
                         (char*)pKeyMerge, P4_KEYINFO_HANDOFF);
  sqlite3VdbeAddOp3(v, OP_Jump, addrAltB, addrAeqB, addrAgtB);

  if( regPrev ){
    sqlite3ReleaseTempRange(pParse, regPrev, nOrderBy+1);
  }

  sqlite3VdbeResolveLabel(v, labelEnd);

  if( pDest->eDest==SRT_Output ){
    Select *pFirst = pPrior;
    while( pFirst->pPrior ) pFirst = pFirst->pPrior;
    generateColumnNames(pParse, 0, pFirst->pEList);
  }

  if( p->pPrior ){
    sqlite3SelectDelete(db, p->pPrior);
  }
  p->pPrior = pPrior;

  explainComposite(pParse, p->op, iSub1, iSub2, 0);
  return SQLITE_OK;
}
#endif

#if !defined(SQLITE_OMIT_SUBQUERY) || !defined(SQLITE_OMIT_VIEW)
static void substExprList(sqlite3*, ExprList*, int, ExprList*);
static void substSelect(sqlite3*, Select *, int, ExprList *);

static Expr *substExpr(
  sqlite3 *db,        
  Expr *pExpr,        
  int iTable,         
  ExprList *pEList    
){
  if( pExpr==0 ) return 0;
  if( pExpr->op==TK_COLUMN && pExpr->iTable==iTable ){
    if( pExpr->iColumn<0 ){
      pExpr->op = TK_NULL;
    }else{
      Expr *pNew;
      assert( pEList!=0 && pExpr->iColumn<pEList->nExpr );
      assert( pExpr->pLeft==0 && pExpr->pRight==0 );
      pNew = sqlite3ExprDup(db, pEList->a[pExpr->iColumn].pExpr, 0);
      if( pNew && pExpr->pColl ){
        pNew->pColl = pExpr->pColl;
      }
      sqlite3ExprDelete(db, pExpr);
      pExpr = pNew;
    }
  }else{
    pExpr->pLeft = substExpr(db, pExpr->pLeft, iTable, pEList);
    pExpr->pRight = substExpr(db, pExpr->pRight, iTable, pEList);
    if( ExprHasProperty(pExpr, EP_xIsSelect) ){
      substSelect(db, pExpr->x.pSelect, iTable, pEList);
    }else{
      substExprList(db, pExpr->x.pList, iTable, pEList);
    }
  }
  return pExpr;
}
static void substExprList(
  sqlite3 *db,         
  ExprList *pList,     
  int iTable,          
  ExprList *pEList     
){
  int i;
  if( pList==0 ) return;
  for(i=0; i<pList->nExpr; i++){
    pList->a[i].pExpr = substExpr(db, pList->a[i].pExpr, iTable, pEList);
  }
}
static void substSelect(
  sqlite3 *db,         
  Select *p,           
  int iTable,          
  ExprList *pEList     
){
  SrcList *pSrc;
  struct SrcList_item *pItem;
  int i;
  if( !p ) return;
  substExprList(db, p->pEList, iTable, pEList);
  substExprList(db, p->pGroupBy, iTable, pEList);
  substExprList(db, p->pOrderBy, iTable, pEList);
  p->pHaving = substExpr(db, p->pHaving, iTable, pEList);
  p->pWhere = substExpr(db, p->pWhere, iTable, pEList);
  substSelect(db, p->pPrior, iTable, pEList);
  pSrc = p->pSrc;
  assert( pSrc );  
  if( ALWAYS(pSrc) ){
    for(i=pSrc->nSrc, pItem=pSrc->a; i>0; i--, pItem++){
      substSelect(db, pItem->pSelect, iTable, pEList);
    }
  }
}
#endif 

#if !defined(SQLITE_OMIT_SUBQUERY) || !defined(SQLITE_OMIT_VIEW)
static int flattenSubquery(
  Parse *pParse,       
  Select *p,           
  int iFrom,           
  int isAgg,           
  int subqueryIsAgg    
){
  const char *zSavedAuthContext = pParse->zAuthContext;
  Select *pParent;
  Select *pSub;       
  Select *pSub1;      
  SrcList *pSrc;      
  SrcList *pSubSrc;   
  ExprList *pList;    
  int iParent;        
  int i;              
  Expr *pWhere;                    
  struct SrcList_item *pSubitem;   
  sqlite3 *db = pParse->db;

  assert( p!=0 );
  assert( p->pPrior==0 );  
  if( db->flags & SQLITE_QueryFlattener ) return 0;
  pSrc = p->pSrc;
  assert( pSrc && iFrom>=0 && iFrom<pSrc->nSrc );
  pSubitem = &pSrc->a[iFrom];
  iParent = pSubitem->iCursor;
  pSub = pSubitem->pSelect;
  assert( pSub!=0 );
  if( isAgg && subqueryIsAgg ) return 0;                 
  if( subqueryIsAgg && pSrc->nSrc>1 ) return 0;          
  pSubSrc = pSub->pSrc;
  assert( pSubSrc );
  if( pSub->pLimit && p->pLimit ) return 0;              
  if( pSub->pOffset ) return 0;                          
  if( p->pRightmost && pSub->pLimit ){
    return 0;                                            
  }
  if( pSubSrc->nSrc==0 ) return 0;                       
  if( pSub->selFlags & SF_Distinct ) return 0;           
  if( pSub->pLimit && (pSrc->nSrc>1 || isAgg) ){
     return 0;         
  }
  if( (p->selFlags & SF_Distinct)!=0 && subqueryIsAgg ){
     return 0;         
  }
  if( p->pOrderBy && pSub->pOrderBy ){
     return 0;                                           
  }
  if( isAgg && pSub->pOrderBy ) return 0;                
  if( pSub->pLimit && p->pWhere ) return 0;              
  if( pSub->pLimit && (p->selFlags & SF_Distinct)!=0 ){
     return 0;         
  }

  if( (pSubitem->jointype & JT_OUTER)!=0 ){
    return 0;
  }

  if( pSub->pPrior ){
    if( pSub->pOrderBy ){
      return 0;  
    }
    if( isAgg || (p->selFlags & SF_Distinct)!=0 || pSrc->nSrc!=1 ){
      return 0;
    }
    for(pSub1=pSub; pSub1; pSub1=pSub1->pPrior){
      testcase( (pSub1->selFlags & (SF_Distinct|SF_Aggregate))==SF_Distinct );
      testcase( (pSub1->selFlags & (SF_Distinct|SF_Aggregate))==SF_Aggregate );
      if( (pSub1->selFlags & (SF_Distinct|SF_Aggregate))!=0
       || (pSub1->pPrior && pSub1->op!=TK_ALL) 
       || NEVER(pSub1->pSrc==0) || pSub1->pSrc->nSrc!=1
      ){
        return 0;
      }
    }

    
    if( p->pOrderBy ){
      int ii;
      for(ii=0; ii<p->pOrderBy->nExpr; ii++){
        if( p->pOrderBy->a[ii].iCol==0 ) return 0;
      }
    }
  }

  

  
  pParse->zAuthContext = pSubitem->zName;
  sqlite3AuthCheck(pParse, SQLITE_SELECT, 0, 0, 0);
  pParse->zAuthContext = zSavedAuthContext;

  for(pSub=pSub->pPrior; pSub; pSub=pSub->pPrior){
    Select *pNew;
    ExprList *pOrderBy = p->pOrderBy;
    Expr *pLimit = p->pLimit;
    Select *pPrior = p->pPrior;
    p->pOrderBy = 0;
    p->pSrc = 0;
    p->pPrior = 0;
    p->pLimit = 0;
    pNew = sqlite3SelectDup(db, p, 0);
    p->pLimit = pLimit;
    p->pOrderBy = pOrderBy;
    p->pSrc = pSrc;
    p->op = TK_ALL;
    p->pRightmost = 0;
    if( pNew==0 ){
      pNew = pPrior;
    }else{
      pNew->pPrior = pPrior;
      pNew->pRightmost = 0;
    }
    p->pPrior = pNew;
    if( db->mallocFailed ) return 1;
  }

  pSub = pSub1 = pSubitem->pSelect;

  sqlite3DbFree(db, pSubitem->zDatabase);
  sqlite3DbFree(db, pSubitem->zName);
  sqlite3DbFree(db, pSubitem->zAlias);
  pSubitem->zDatabase = 0;
  pSubitem->zName = 0;
  pSubitem->zAlias = 0;
  pSubitem->pSelect = 0;

  if( ALWAYS(pSubitem->pTab!=0) ){
    Table *pTabToDel = pSubitem->pTab;
    if( pTabToDel->nRef==1 ){
      Parse *pToplevel = sqlite3ParseToplevel(pParse);
      pTabToDel->pNextZombie = pToplevel->pZombieTab;
      pToplevel->pZombieTab = pTabToDel;
    }else{
      pTabToDel->nRef--;
    }
    pSubitem->pTab = 0;
  }

  for(pParent=p; pParent; pParent=pParent->pPrior, pSub=pSub->pPrior){
    int nSubSrc;
    u8 jointype = 0;
    pSubSrc = pSub->pSrc;     
    nSubSrc = pSubSrc->nSrc;  
    pSrc = pParent->pSrc;     

    if( pSrc ){
      assert( pParent==p );  
      jointype = pSubitem->jointype;
    }else{
      assert( pParent!=p );  
      pSrc = pParent->pSrc = sqlite3SrcListAppend(db, 0, 0, 0);
      if( pSrc==0 ){
        assert( db->mallocFailed );
        break;
      }
    }

    if( nSubSrc>1 ){
      pParent->pSrc = pSrc = sqlite3SrcListEnlarge(db, pSrc, nSubSrc-1,iFrom+1);
      if( db->mallocFailed ){
        break;
      }
    }

    for(i=0; i<nSubSrc; i++){
      sqlite3IdListDelete(db, pSrc->a[i+iFrom].pUsing);
      pSrc->a[i+iFrom] = pSubSrc->a[i];
      memset(&pSubSrc->a[i], 0, sizeof(pSubSrc->a[i]));
    }
    pSrc->a[iFrom].jointype = jointype;
  
    pList = pParent->pEList;
    for(i=0; i<pList->nExpr; i++){
      if( pList->a[i].zName==0 ){
        const char *zSpan = pList->a[i].zSpan;
        if( ALWAYS(zSpan) ){
          pList->a[i].zName = sqlite3DbStrDup(db, zSpan);
        }
      }
    }
    substExprList(db, pParent->pEList, iParent, pSub->pEList);
    if( isAgg ){
      substExprList(db, pParent->pGroupBy, iParent, pSub->pEList);
      pParent->pHaving = substExpr(db, pParent->pHaving, iParent, pSub->pEList);
    }
    if( pSub->pOrderBy ){
      assert( pParent->pOrderBy==0 );
      pParent->pOrderBy = pSub->pOrderBy;
      pSub->pOrderBy = 0;
    }else if( pParent->pOrderBy ){
      substExprList(db, pParent->pOrderBy, iParent, pSub->pEList);
    }
    if( pSub->pWhere ){
      pWhere = sqlite3ExprDup(db, pSub->pWhere, 0);
    }else{
      pWhere = 0;
    }
    if( subqueryIsAgg ){
      assert( pParent->pHaving==0 );
      pParent->pHaving = pParent->pWhere;
      pParent->pWhere = pWhere;
      pParent->pHaving = substExpr(db, pParent->pHaving, iParent, pSub->pEList);
      pParent->pHaving = sqlite3ExprAnd(db, pParent->pHaving, 
                                  sqlite3ExprDup(db, pSub->pHaving, 0));
      assert( pParent->pGroupBy==0 );
      pParent->pGroupBy = sqlite3ExprListDup(db, pSub->pGroupBy, 0);
    }else{
      pParent->pWhere = substExpr(db, pParent->pWhere, iParent, pSub->pEList);
      pParent->pWhere = sqlite3ExprAnd(db, pParent->pWhere, pWhere);
    }
  
    pParent->selFlags |= pSub->selFlags & SF_Distinct;
  
    if( pSub->pLimit ){
      pParent->pLimit = pSub->pLimit;
      pSub->pLimit = 0;
    }
  }

  sqlite3SelectDelete(db, pSub1);

  return 1;
}
#endif 

static u8 minMaxQuery(Select *p){
  Expr *pExpr;
  ExprList *pEList = p->pEList;

  if( pEList->nExpr!=1 ) return WHERE_ORDERBY_NORMAL;
  pExpr = pEList->a[0].pExpr;
  if( pExpr->op!=TK_AGG_FUNCTION ) return 0;
  if( NEVER(ExprHasProperty(pExpr, EP_xIsSelect)) ) return 0;
  pEList = pExpr->x.pList;
  if( pEList==0 || pEList->nExpr!=1 ) return 0;
  if( pEList->a[0].pExpr->op!=TK_AGG_COLUMN ) return WHERE_ORDERBY_NORMAL;
  assert( !ExprHasProperty(pExpr, EP_IntValue) );
  if( sqlite3StrICmp(pExpr->u.zToken,"min")==0 ){
    return WHERE_ORDERBY_MIN;
  }else if( sqlite3StrICmp(pExpr->u.zToken,"max")==0 ){
    return WHERE_ORDERBY_MAX;
  }
  return WHERE_ORDERBY_NORMAL;
}

static Table *isSimpleCount(Select *p, AggInfo *pAggInfo){
  Table *pTab;
  Expr *pExpr;

  assert( !p->pGroupBy );

  if( p->pWhere || p->pEList->nExpr!=1 
   || p->pSrc->nSrc!=1 || p->pSrc->a[0].pSelect
  ){
    return 0;
  }
  pTab = p->pSrc->a[0].pTab;
  pExpr = p->pEList->a[0].pExpr;
  assert( pTab && !pTab->pSelect && pExpr );

  if( IsVirtual(pTab) ) return 0;
  if( pExpr->op!=TK_AGG_FUNCTION ) return 0;
  if( (pAggInfo->aFunc[0].pFunc->flags&SQLITE_FUNC_COUNT)==0 ) return 0;
  if( pExpr->flags&EP_Distinct ) return 0;

  return pTab;
}

int sqlite3IndexedByLookup(Parse *pParse, struct SrcList_item *pFrom){
  if( pFrom->pTab && pFrom->zIndex ){
    Table *pTab = pFrom->pTab;
    char *zIndex = pFrom->zIndex;
    Index *pIdx;
    for(pIdx=pTab->pIndex; 
        pIdx && sqlite3StrICmp(pIdx->zName, zIndex); 
        pIdx=pIdx->pNext
    );
    if( !pIdx ){
      sqlite3ErrorMsg(pParse, "no such index: %s", zIndex, 0);
      pParse->checkSchema = 1;
      return SQLITE_ERROR;
    }
    pFrom->pIndex = pIdx;
  }
  return SQLITE_OK;
}

static int selectExpander(Walker *pWalker, Select *p){
  Parse *pParse = pWalker->pParse;
  int i, j, k;
  SrcList *pTabList;
  ExprList *pEList;
  struct SrcList_item *pFrom;
  sqlite3 *db = pParse->db;

  if( db->mallocFailed  ){
    return WRC_Abort;
  }
  if( NEVER(p->pSrc==0) || (p->selFlags & SF_Expanded)!=0 ){
    return WRC_Prune;
  }
  p->selFlags |= SF_Expanded;
  pTabList = p->pSrc;
  pEList = p->pEList;

  sqlite3SrcListAssignCursors(pParse, pTabList);

  for(i=0, pFrom=pTabList->a; i<pTabList->nSrc; i++, pFrom++){
    Table *pTab;
    if( pFrom->pTab!=0 ){
      assert( i==0 );
      return WRC_Prune;
    }
    if( pFrom->zName==0 ){
#ifndef SQLITE_OMIT_SUBQUERY
      Select *pSel = pFrom->pSelect;
      
      assert( pSel!=0 );
      assert( pFrom->pTab==0 );
      sqlite3WalkSelect(pWalker, pSel);
      pFrom->pTab = pTab = sqlite3DbMallocZero(db, sizeof(Table));
      if( pTab==0 ) return WRC_Abort;
      pTab->nRef = 1;
      pTab->zName = sqlite3MPrintf(db, "sqlite_subquery_%p_", (void*)pTab);
      while( pSel->pPrior ){ pSel = pSel->pPrior; }
      selectColumnsFromExprList(pParse, pSel->pEList, &pTab->nCol, &pTab->aCol);
      pTab->iPKey = -1;
      pTab->nRowEst = 1000000;
      pTab->tabFlags |= TF_Ephemeral;
#endif
    }else{
      
      assert( pFrom->pTab==0 );
      pFrom->pTab = pTab = 
        sqlite3LocateTable(pParse,0,pFrom->zName,pFrom->zDatabase);
      if( pTab==0 ) return WRC_Abort;
      pTab->nRef++;
#if !defined(SQLITE_OMIT_VIEW) || !defined (SQLITE_OMIT_VIRTUALTABLE)
      if( pTab->pSelect || IsVirtual(pTab) ){
        
        if( sqlite3ViewGetColumnNames(pParse, pTab) ) return WRC_Abort;
        assert( pFrom->pSelect==0 );
        pFrom->pSelect = sqlite3SelectDup(db, pTab->pSelect, 0);
        sqlite3WalkSelect(pWalker, pFrom->pSelect);
      }
#endif
    }

    
    if( sqlite3IndexedByLookup(pParse, pFrom) ){
      return WRC_Abort;
    }
  }

  if( db->mallocFailed || sqliteProcessJoin(pParse, p) ){
    return WRC_Abort;
  }

  for(k=0; k<pEList->nExpr; k++){
    Expr *pE = pEList->a[k].pExpr;
    if( pE->op==TK_ALL ) break;
    assert( pE->op!=TK_DOT || pE->pRight!=0 );
    assert( pE->op!=TK_DOT || (pE->pLeft!=0 && pE->pLeft->op==TK_ID) );
    if( pE->op==TK_DOT && pE->pRight->op==TK_ALL ) break;
  }
  if( k<pEList->nExpr ){
    struct ExprList_item *a = pEList->a;
    ExprList *pNew = 0;
    int flags = pParse->db->flags;
    int longNames = (flags & SQLITE_FullColNames)!=0
                      && (flags & SQLITE_ShortColNames)==0;

    for(k=0; k<pEList->nExpr; k++){
      Expr *pE = a[k].pExpr;
      assert( pE->op!=TK_DOT || pE->pRight!=0 );
      if( pE->op!=TK_ALL && (pE->op!=TK_DOT || pE->pRight->op!=TK_ALL) ){
        pNew = sqlite3ExprListAppend(pParse, pNew, a[k].pExpr);
        if( pNew ){
          pNew->a[pNew->nExpr-1].zName = a[k].zName;
          pNew->a[pNew->nExpr-1].zSpan = a[k].zSpan;
          a[k].zName = 0;
          a[k].zSpan = 0;
        }
        a[k].pExpr = 0;
      }else{
        int tableSeen = 0;      
        char *zTName;            
        if( pE->op==TK_DOT ){
          assert( pE->pLeft!=0 );
          assert( !ExprHasProperty(pE->pLeft, EP_IntValue) );
          zTName = pE->pLeft->u.zToken;
        }else{
          zTName = 0;
        }
        for(i=0, pFrom=pTabList->a; i<pTabList->nSrc; i++, pFrom++){
          Table *pTab = pFrom->pTab;
          char *zTabName = pFrom->zAlias;
          if( zTabName==0 ){
            zTabName = pTab->zName;
          }
          if( db->mallocFailed ) break;
          if( zTName && sqlite3StrICmp(zTName, zTabName)!=0 ){
            continue;
          }
          tableSeen = 1;
          for(j=0; j<pTab->nCol; j++){
            Expr *pExpr, *pRight;
            char *zName = pTab->aCol[j].zName;
            char *zColname;  
            char *zToFree;   
            Token sColname;  

            if( IsHiddenColumn(&pTab->aCol[j]) ){
              assert(IsVirtual(pTab));
              continue;
            }

            if( i>0 && zTName==0 ){
              if( (pFrom->jointype & JT_NATURAL)!=0
                && tableAndColumnIndex(pTabList, i, zName, 0, 0)
              ){
                continue;
              }
              if( sqlite3IdListIndex(pFrom->pUsing, zName)>=0 ){
                continue;
              }
            }
            pRight = sqlite3Expr(db, TK_ID, zName);
            zColname = zName;
            zToFree = 0;
            if( longNames || pTabList->nSrc>1 ){
              Expr *pLeft;
              pLeft = sqlite3Expr(db, TK_ID, zTabName);
              pExpr = sqlite3PExpr(pParse, TK_DOT, pLeft, pRight, 0);
              if( longNames ){
                zColname = sqlite3MPrintf(db, "%s.%s", zTabName, zName);
                zToFree = zColname;
              }
            }else{
              pExpr = pRight;
            }
            pNew = sqlite3ExprListAppend(pParse, pNew, pExpr);
            sColname.z = zColname;
            sColname.n = sqlite3Strlen30(zColname);
            sqlite3ExprListSetName(pParse, pNew, &sColname, 0);
            sqlite3DbFree(db, zToFree);
          }
        }
        if( !tableSeen ){
          if( zTName ){
            sqlite3ErrorMsg(pParse, "no such table: %s", zTName);
          }else{
            sqlite3ErrorMsg(pParse, "no tables specified");
          }
        }
      }
    }
    sqlite3ExprListDelete(db, pEList);
    p->pEList = pNew;
  }
#if SQLITE_MAX_COLUMN
  if( p->pEList && p->pEList->nExpr>db->aLimit[SQLITE_LIMIT_COLUMN] ){
    sqlite3ErrorMsg(pParse, "too many columns in result set");
  }
#endif
  return WRC_Continue;
}

static int exprWalkNoop(Walker *NotUsed, Expr *NotUsed2){
  UNUSED_PARAMETER2(NotUsed, NotUsed2);
  return WRC_Continue;
}

/*
** This routine "expands" a SELECT statement and all of its subqueries.
** For additional information on what it means to "expand" a SELECT
** statement, see the comment on the selectExpand worker callback above.
**
** Expanding a SELECT statement is the first step in processing a
** SELECT statement.  The SELECT statement must be expanded before
** name resolution is performed.
**
** If anything goes wrong, an error message is written into pParse.
** The calling function can detect the problem by looking at pParse->nErr
** and/or pParse->db->mallocFailed.
*/
static void sqlite3SelectExpand(Parse *pParse, Select *pSelect){
  Walker w;
  w.xSelectCallback = selectExpander;
  w.xExprCallback = exprWalkNoop;
  w.pParse = pParse;
  sqlite3WalkSelect(&w, pSelect);
}


#ifndef SQLITE_OMIT_SUBQUERY
static int selectAddSubqueryTypeInfo(Walker *pWalker, Select *p){
  Parse *pParse;
  int i;
  SrcList *pTabList;
  struct SrcList_item *pFrom;

  assert( p->selFlags & SF_Resolved );
  if( (p->selFlags & SF_HasTypeInfo)==0 ){
    p->selFlags |= SF_HasTypeInfo;
    pParse = pWalker->pParse;
    pTabList = p->pSrc;
    for(i=0, pFrom=pTabList->a; i<pTabList->nSrc; i++, pFrom++){
      Table *pTab = pFrom->pTab;
      if( ALWAYS(pTab!=0) && (pTab->tabFlags & TF_Ephemeral)!=0 ){
        
        Select *pSel = pFrom->pSelect;
        assert( pSel );
        while( pSel->pPrior ) pSel = pSel->pPrior;
        selectAddColumnTypeAndCollation(pParse, pTab->nCol, pTab->aCol, pSel);
      }
    }
  }
  return WRC_Continue;
}
#endif


static void sqlite3SelectAddTypeInfo(Parse *pParse, Select *pSelect){
#ifndef SQLITE_OMIT_SUBQUERY
  Walker w;
  w.xSelectCallback = selectAddSubqueryTypeInfo;
  w.xExprCallback = exprWalkNoop;
  w.pParse = pParse;
  sqlite3WalkSelect(&w, pSelect);
#endif
}


void sqlite3SelectPrep(
  Parse *pParse,         
  Select *p,             
  NameContext *pOuterNC  
){
  sqlite3 *db;
  if( NEVER(p==0) ) return;
  db = pParse->db;
  if( p->selFlags & SF_HasTypeInfo ) return;
  sqlite3SelectExpand(pParse, p);
  if( pParse->nErr || db->mallocFailed ) return;
  sqlite3ResolveSelectNames(pParse, p, pOuterNC);
  if( pParse->nErr || db->mallocFailed ) return;
  sqlite3SelectAddTypeInfo(pParse, p);
}

static void resetAccumulator(Parse *pParse, AggInfo *pAggInfo){
  Vdbe *v = pParse->pVdbe;
  int i;
  struct AggInfo_func *pFunc;
  if( pAggInfo->nFunc+pAggInfo->nColumn==0 ){
    return;
  }
  for(i=0; i<pAggInfo->nColumn; i++){
    sqlite3VdbeAddOp2(v, OP_Null, 0, pAggInfo->aCol[i].iMem);
  }
  for(pFunc=pAggInfo->aFunc, i=0; i<pAggInfo->nFunc; i++, pFunc++){
    sqlite3VdbeAddOp2(v, OP_Null, 0, pFunc->iMem);
    if( pFunc->iDistinct>=0 ){
      Expr *pE = pFunc->pExpr;
      assert( !ExprHasProperty(pE, EP_xIsSelect) );
      if( pE->x.pList==0 || pE->x.pList->nExpr!=1 ){
        sqlite3ErrorMsg(pParse, "DISTINCT aggregates must have exactly one "
           "argument");
        pFunc->iDistinct = -1;
      }else{
        KeyInfo *pKeyInfo = keyInfoFromExprList(pParse, pE->x.pList);
        sqlite3VdbeAddOp4(v, OP_OpenEphemeral, pFunc->iDistinct, 0, 0,
                          (char*)pKeyInfo, P4_KEYINFO_HANDOFF);
      }
    }
  }
}

static void finalizeAggFunctions(Parse *pParse, AggInfo *pAggInfo){
  Vdbe *v = pParse->pVdbe;
  int i;
  struct AggInfo_func *pF;
  for(i=0, pF=pAggInfo->aFunc; i<pAggInfo->nFunc; i++, pF++){
    ExprList *pList = pF->pExpr->x.pList;
    assert( !ExprHasProperty(pF->pExpr, EP_xIsSelect) );
    sqlite3VdbeAddOp4(v, OP_AggFinal, pF->iMem, pList ? pList->nExpr : 0, 0,
                      (void*)pF->pFunc, P4_FUNCDEF);
  }
}

static void updateAccumulator(Parse *pParse, AggInfo *pAggInfo){
  Vdbe *v = pParse->pVdbe;
  int i;
  struct AggInfo_func *pF;
  struct AggInfo_col *pC;

  pAggInfo->directMode = 1;
  sqlite3ExprCacheClear(pParse);
  for(i=0, pF=pAggInfo->aFunc; i<pAggInfo->nFunc; i++, pF++){
    int nArg;
    int addrNext = 0;
    int regAgg;
    ExprList *pList = pF->pExpr->x.pList;
    assert( !ExprHasProperty(pF->pExpr, EP_xIsSelect) );
    if( pList ){
      nArg = pList->nExpr;
      regAgg = sqlite3GetTempRange(pParse, nArg);
      sqlite3ExprCodeExprList(pParse, pList, regAgg, 1);
    }else{
      nArg = 0;
      regAgg = 0;
    }
    if( pF->iDistinct>=0 ){
      addrNext = sqlite3VdbeMakeLabel(v);
      assert( nArg==1 );
      codeDistinct(pParse, pF->iDistinct, addrNext, 1, regAgg);
    }
    if( pF->pFunc->flags & SQLITE_FUNC_NEEDCOLL ){
      CollSeq *pColl = 0;
      struct ExprList_item *pItem;
      int j;
      assert( pList!=0 );  
      for(j=0, pItem=pList->a; !pColl && j<nArg; j++, pItem++){
        pColl = sqlite3ExprCollSeq(pParse, pItem->pExpr);
      }
      if( !pColl ){
        pColl = pParse->db->pDfltColl;
      }
      sqlite3VdbeAddOp4(v, OP_CollSeq, 0, 0, 0, (char *)pColl, P4_COLLSEQ);
    }
    sqlite3VdbeAddOp4(v, OP_AggStep, 0, regAgg, pF->iMem,
                      (void*)pF->pFunc, P4_FUNCDEF);
    sqlite3VdbeChangeP5(v, (u8)nArg);
    sqlite3ExprCacheAffinityChange(pParse, regAgg, nArg);
    sqlite3ReleaseTempRange(pParse, regAgg, nArg);
    if( addrNext ){
      sqlite3VdbeResolveLabel(v, addrNext);
      sqlite3ExprCacheClear(pParse);
    }
  }

  sqlite3ExprCacheClear(pParse);
  for(i=0, pC=pAggInfo->aCol; i<pAggInfo->nAccumulator; i++, pC++){
    sqlite3ExprCode(pParse, pC->pExpr, pC->iMem);
  }
  pAggInfo->directMode = 0;
  sqlite3ExprCacheClear(pParse);
}

#ifndef SQLITE_OMIT_EXPLAIN
static void explainSimpleCount(
  Parse *pParse,                  
  Table *pTab,                    
  Index *pIdx                     
){
  if( pParse->explain==2 ){
    char *zEqp = sqlite3MPrintf(pParse->db, "SCAN TABLE %s %s%s(~%d rows)",
        pTab->zName, 
        pIdx ? "USING COVERING INDEX " : "",
        pIdx ? pIdx->zName : "",
        pTab->nRowEst
    );
    sqlite3VdbeAddOp4(
        pParse->pVdbe, OP_Explain, pParse->iSelectId, 0, 0, zEqp, P4_DYNAMIC
    );
  }
}
#else
# define explainSimpleCount(a,b,c)
#endif

int sqlite3Select(
  Parse *pParse,         
  Select *p,             
  SelectDest *pDest      
){
  int i, j;              
  WhereInfo *pWInfo;     
  Vdbe *v;               
  int isAgg;             
  ExprList *pEList;      
  SrcList *pTabList;     
  Expr *pWhere;          
  ExprList *pOrderBy;    
  ExprList *pGroupBy;    
  Expr *pHaving;         
  int isDistinct;        
  int distinct;          
  int rc = 1;            
  int addrSortIndex;     
  AggInfo sAggInfo;      
  int iEnd;              
  sqlite3 *db;           

#ifndef SQLITE_OMIT_EXPLAIN
  int iRestoreSelectId = pParse->iSelectId;
  pParse->iSelectId = pParse->iNextSelectId++;
#endif

  db = pParse->db;
  if( p==0 || db->mallocFailed || pParse->nErr ){
    return 1;
  }
  if( sqlite3AuthCheck(pParse, SQLITE_SELECT, 0, 0, 0) ) return 1;
  memset(&sAggInfo, 0, sizeof(sAggInfo));

  if( IgnorableOrderby(pDest) ){
    assert(pDest->eDest==SRT_Exists || pDest->eDest==SRT_Union || 
           pDest->eDest==SRT_Except || pDest->eDest==SRT_Discard);
    sqlite3ExprListDelete(db, p->pOrderBy);
    p->pOrderBy = 0;
    p->selFlags &= ~SF_Distinct;
  }
  sqlite3SelectPrep(pParse, p, 0);
  pOrderBy = p->pOrderBy;
  pTabList = p->pSrc;
  pEList = p->pEList;
  if( pParse->nErr || db->mallocFailed ){
    goto select_end;
  }
  isAgg = (p->selFlags & SF_Aggregate)!=0;
  assert( pEList!=0 );

  v = sqlite3GetVdbe(pParse);
  if( v==0 ) goto select_end;

#ifndef SQLITE_OMIT_SUBQUERY
  if( checkForMultiColumnSelectError(pParse, pDest, pEList->nExpr) ){
    goto select_end;
  }
#endif

#if !defined(SQLITE_OMIT_SUBQUERY) || !defined(SQLITE_OMIT_VIEW)
  for(i=0; !p->pPrior && i<pTabList->nSrc; i++){
    struct SrcList_item *pItem = &pTabList->a[i];
    SelectDest dest;
    Select *pSub = pItem->pSelect;
    int isAggSub;

    if( pSub==0 || pItem->isPopulated ) continue;

    pParse->nHeight += sqlite3SelectExprHeight(p);

    
    isAggSub = (pSub->selFlags & SF_Aggregate)!=0;
    if( flattenSubquery(pParse, p, i, isAgg, isAggSub) ){
      if( isAggSub ){
        isAgg = 1;
        p->selFlags |= SF_Aggregate;
      }
      i = -1;
    }else{
      sqlite3SelectDestInit(&dest, SRT_EphemTab, pItem->iCursor);
      assert( pItem->isPopulated==0 );
      explainSetInteger(pItem->iSelectId, (u8)pParse->iNextSelectId);
      sqlite3Select(pParse, pSub, &dest);
      pItem->isPopulated = 1;
      pItem->pTab->nRowEst = (unsigned)pSub->nSelectRow;
    }
    if(  db->mallocFailed ){
      goto select_end;
    }
    pParse->nHeight -= sqlite3SelectExprHeight(p);
    pTabList = p->pSrc;
    if( !IgnorableOrderby(pDest) ){
      pOrderBy = p->pOrderBy;
    }
  }
  pEList = p->pEList;
#endif
  pWhere = p->pWhere;
  pGroupBy = p->pGroupBy;
  pHaving = p->pHaving;
  isDistinct = (p->selFlags & SF_Distinct)!=0;

#ifndef SQLITE_OMIT_COMPOUND_SELECT
  if( p->pPrior ){
    if( p->pRightmost==0 ){
      Select *pLoop, *pRight = 0;
      int cnt = 0;
      int mxSelect;
      for(pLoop=p; pLoop; pLoop=pLoop->pPrior, cnt++){
        pLoop->pRightmost = p;
        pLoop->pNext = pRight;
        pRight = pLoop;
      }
      mxSelect = db->aLimit[SQLITE_LIMIT_COMPOUND_SELECT];
      if( mxSelect && cnt>mxSelect ){
        sqlite3ErrorMsg(pParse, "too many terms in compound SELECT");
        goto select_end;
      }
    }
    rc = multiSelect(pParse, p, pDest);
    explainSetInteger(pParse->iSelectId, iRestoreSelectId);
    return rc;
  }
#endif

  assert( p->pGroupBy==0 || (p->selFlags & SF_Aggregate)!=0 );
  if( (p->selFlags & (SF_Distinct|SF_Aggregate))==SF_Distinct ){
    p->pGroupBy = sqlite3ExprListDup(db, p->pEList, 0);
    pGroupBy = p->pGroupBy;
    p->selFlags &= ~SF_Distinct;
  }

  if( sqlite3ExprListCompare(p->pGroupBy, pOrderBy)==0
         && (db->flags & SQLITE_GroupByOrder)==0 ){
    pOrderBy = 0;
  }

  if( pOrderBy ){
    KeyInfo *pKeyInfo;
    pKeyInfo = keyInfoFromExprList(pParse, pOrderBy);
    pOrderBy->iECursor = pParse->nTab++;
    p->addrOpenEphm[2] = addrSortIndex =
      sqlite3VdbeAddOp4(v, OP_OpenEphemeral,
                           pOrderBy->iECursor, pOrderBy->nExpr+2, 0,
                           (char*)pKeyInfo, P4_KEYINFO_HANDOFF);
  }else{
    addrSortIndex = -1;
  }

  if( pDest->eDest==SRT_EphemTab ){
    sqlite3VdbeAddOp2(v, OP_OpenEphemeral, pDest->iParm, pEList->nExpr);
  }

  iEnd = sqlite3VdbeMakeLabel(v);
  p->nSelectRow = (double)LARGEST_INT64;
  computeLimitRegisters(pParse, p, iEnd);

  if( p->selFlags & SF_Distinct ){
    KeyInfo *pKeyInfo;
    assert( isAgg || pGroupBy );
    distinct = pParse->nTab++;
    pKeyInfo = keyInfoFromExprList(pParse, p->pEList);
    sqlite3VdbeAddOp4(v, OP_OpenEphemeral, distinct, 0, 0,
                        (char*)pKeyInfo, P4_KEYINFO_HANDOFF);
    sqlite3VdbeChangeP5(v, BTREE_UNORDERED);
  }else{
    distinct = -1;
  }

  
  if( !isAgg && pGroupBy==0 ){
    pWInfo = sqlite3WhereBegin(pParse, pTabList, pWhere, &pOrderBy, 0);
    if( pWInfo==0 ) goto select_end;
    if( pWInfo->nRowOut < p->nSelectRow ) p->nSelectRow = pWInfo->nRowOut;

    if( addrSortIndex>=0 && pOrderBy==0 ){
      sqlite3VdbeChangeToNoop(v, addrSortIndex, 1);
      p->addrOpenEphm[2] = -1;
    }

    assert(!isDistinct);
    selectInnerLoop(pParse, p, pEList, 0, 0, pOrderBy, -1, pDest,
                    pWInfo->iContinue, pWInfo->iBreak);

    sqlite3WhereEnd(pWInfo);
  }else{
    
    NameContext sNC;    
    int iAMem;          
    int iBMem;          
    int iUseFlag;       
    int iAbortFlag;     
    int groupBySort;    
    int addrEnd;        

    if( pGroupBy ){
      int k;                        
      struct ExprList_item *pItem;  

      for(k=p->pEList->nExpr, pItem=p->pEList->a; k>0; k--, pItem++){
        pItem->iAlias = 0;
      }
      for(k=pGroupBy->nExpr, pItem=pGroupBy->a; k>0; k--, pItem++){
        pItem->iAlias = 0;
      }
      if( p->nSelectRow>(double)100 ) p->nSelectRow = (double)100;
    }else{
      p->nSelectRow = (double)1;
    }

 
    
    addrEnd = sqlite3VdbeMakeLabel(v);

    memset(&sNC, 0, sizeof(sNC));
    sNC.pParse = pParse;
    sNC.pSrcList = pTabList;
    sNC.pAggInfo = &sAggInfo;
    sAggInfo.nSortingColumn = pGroupBy ? pGroupBy->nExpr+1 : 0;
    sAggInfo.pGroupBy = pGroupBy;
    sqlite3ExprAnalyzeAggList(&sNC, pEList);
    sqlite3ExprAnalyzeAggList(&sNC, pOrderBy);
    if( pHaving ){
      sqlite3ExprAnalyzeAggregates(&sNC, pHaving);
    }
    sAggInfo.nAccumulator = sAggInfo.nColumn;
    for(i=0; i<sAggInfo.nFunc; i++){
      assert( !ExprHasProperty(sAggInfo.aFunc[i].pExpr, EP_xIsSelect) );
      sqlite3ExprAnalyzeAggList(&sNC, sAggInfo.aFunc[i].pExpr->x.pList);
    }
    if( db->mallocFailed ) goto select_end;

    if( pGroupBy ){
      KeyInfo *pKeyInfo;  
      int j1;             
      int addrOutputRow;  
      int regOutputRow;   
      int addrSetAbort;   
      int addrTopOfLoop;  
      int addrSortingIdx; 
      int addrReset;      
      int regReset;       

      sAggInfo.sortingIdx = pParse->nTab++;
      pKeyInfo = keyInfoFromExprList(pParse, pGroupBy);
      addrSortingIdx = sqlite3VdbeAddOp4(v, OP_OpenEphemeral, 
          sAggInfo.sortingIdx, sAggInfo.nSortingColumn, 
          0, (char*)pKeyInfo, P4_KEYINFO_HANDOFF);

      iUseFlag = ++pParse->nMem;
      iAbortFlag = ++pParse->nMem;
      regOutputRow = ++pParse->nMem;
      addrOutputRow = sqlite3VdbeMakeLabel(v);
      regReset = ++pParse->nMem;
      addrReset = sqlite3VdbeMakeLabel(v);
      iAMem = pParse->nMem + 1;
      pParse->nMem += pGroupBy->nExpr;
      iBMem = pParse->nMem + 1;
      pParse->nMem += pGroupBy->nExpr;
      sqlite3VdbeAddOp2(v, OP_Integer, 0, iAbortFlag);
      VdbeComment((v, "clear abort flag"));
      sqlite3VdbeAddOp2(v, OP_Integer, 0, iUseFlag);
      VdbeComment((v, "indicate accumulator empty"));

      sqlite3VdbeAddOp2(v, OP_Gosub, regReset, addrReset);
      pWInfo = sqlite3WhereBegin(pParse, pTabList, pWhere, &pGroupBy, 0);
      if( pWInfo==0 ) goto select_end;
      if( pGroupBy==0 ){
        pGroupBy = p->pGroupBy;
        groupBySort = 0;
      }else{
        int regBase;
        int regRecord;
        int nCol;
        int nGroupBy;

        explainTempTable(pParse, 
            isDistinct && !(p->selFlags&SF_Distinct)?"DISTINCT":"GROUP BY");

        groupBySort = 1;
        nGroupBy = pGroupBy->nExpr;
        nCol = nGroupBy + 1;
        j = nGroupBy+1;
        for(i=0; i<sAggInfo.nColumn; i++){
          if( sAggInfo.aCol[i].iSorterColumn>=j ){
            nCol++;
            j++;
          }
        }
        regBase = sqlite3GetTempRange(pParse, nCol);
        sqlite3ExprCacheClear(pParse);
        sqlite3ExprCodeExprList(pParse, pGroupBy, regBase, 0);
        sqlite3VdbeAddOp2(v, OP_Sequence, sAggInfo.sortingIdx,regBase+nGroupBy);
        j = nGroupBy+1;
        for(i=0; i<sAggInfo.nColumn; i++){
          struct AggInfo_col *pCol = &sAggInfo.aCol[i];
          if( pCol->iSorterColumn>=j ){
            int r1 = j + regBase;
            int r2;

            r2 = sqlite3ExprCodeGetColumn(pParse, 
                               pCol->pTab, pCol->iColumn, pCol->iTable, r1);
            if( r1!=r2 ){
              sqlite3VdbeAddOp2(v, OP_SCopy, r2, r1);
            }
            j++;
          }
        }
        regRecord = sqlite3GetTempReg(pParse);
        sqlite3VdbeAddOp3(v, OP_MakeRecord, regBase, nCol, regRecord);
        sqlite3VdbeAddOp2(v, OP_IdxInsert, sAggInfo.sortingIdx, regRecord);
        sqlite3ReleaseTempReg(pParse, regRecord);
        sqlite3ReleaseTempRange(pParse, regBase, nCol);
        sqlite3WhereEnd(pWInfo);
        sqlite3VdbeAddOp2(v, OP_Sort, sAggInfo.sortingIdx, addrEnd);
        VdbeComment((v, "GROUP BY sort"));
        sAggInfo.useSortingIdx = 1;
        sqlite3ExprCacheClear(pParse);
      }

      addrTopOfLoop = sqlite3VdbeCurrentAddr(v);
      sqlite3ExprCacheClear(pParse);
      for(j=0; j<pGroupBy->nExpr; j++){
        if( groupBySort ){
          sqlite3VdbeAddOp3(v, OP_Column, sAggInfo.sortingIdx, j, iBMem+j);
        }else{
          sAggInfo.directMode = 1;
          sqlite3ExprCode(pParse, pGroupBy->a[j].pExpr, iBMem+j);
        }
      }
      sqlite3VdbeAddOp4(v, OP_Compare, iAMem, iBMem, pGroupBy->nExpr,
                          (char*)pKeyInfo, P4_KEYINFO);
      j1 = sqlite3VdbeCurrentAddr(v);
      sqlite3VdbeAddOp3(v, OP_Jump, j1+1, 0, j1+1);

      sqlite3ExprCodeMove(pParse, iBMem, iAMem, pGroupBy->nExpr);
      sqlite3VdbeAddOp2(v, OP_Gosub, regOutputRow, addrOutputRow);
      VdbeComment((v, "output one row"));
      sqlite3VdbeAddOp2(v, OP_IfPos, iAbortFlag, addrEnd);
      VdbeComment((v, "check abort flag"));
      sqlite3VdbeAddOp2(v, OP_Gosub, regReset, addrReset);
      VdbeComment((v, "reset accumulator"));

      sqlite3VdbeJumpHere(v, j1);
      updateAccumulator(pParse, &sAggInfo);
      sqlite3VdbeAddOp2(v, OP_Integer, 1, iUseFlag);
      VdbeComment((v, "indicate data in accumulator"));

      if( groupBySort ){
        sqlite3VdbeAddOp2(v, OP_Next, sAggInfo.sortingIdx, addrTopOfLoop);
      }else{
        sqlite3WhereEnd(pWInfo);
        sqlite3VdbeChangeToNoop(v, addrSortingIdx, 1);
      }

      sqlite3VdbeAddOp2(v, OP_Gosub, regOutputRow, addrOutputRow);
      VdbeComment((v, "output final row"));

      sqlite3VdbeAddOp2(v, OP_Goto, 0, addrEnd);

      addrSetAbort = sqlite3VdbeCurrentAddr(v);
      sqlite3VdbeAddOp2(v, OP_Integer, 1, iAbortFlag);
      VdbeComment((v, "set abort flag"));
      sqlite3VdbeAddOp1(v, OP_Return, regOutputRow);
      sqlite3VdbeResolveLabel(v, addrOutputRow);
      addrOutputRow = sqlite3VdbeCurrentAddr(v);
      sqlite3VdbeAddOp2(v, OP_IfPos, iUseFlag, addrOutputRow+2);
      VdbeComment((v, "Groupby result generator entry point"));
      sqlite3VdbeAddOp1(v, OP_Return, regOutputRow);
      finalizeAggFunctions(pParse, &sAggInfo);
      sqlite3ExprIfFalse(pParse, pHaving, addrOutputRow+1, SQLITE_JUMPIFNULL);
      selectInnerLoop(pParse, p, p->pEList, 0, 0, pOrderBy,
                      distinct, pDest,
                      addrOutputRow+1, addrSetAbort);
      sqlite3VdbeAddOp1(v, OP_Return, regOutputRow);
      VdbeComment((v, "end groupby result generator"));

      sqlite3VdbeResolveLabel(v, addrReset);
      resetAccumulator(pParse, &sAggInfo);
      sqlite3VdbeAddOp1(v, OP_Return, regReset);
     
    } 
    else {
      ExprList *pDel = 0;
#ifndef SQLITE_OMIT_BTREECOUNT
      Table *pTab;
      if( (pTab = isSimpleCount(p, &sAggInfo))!=0 ){
        const int iDb = sqlite3SchemaToIndex(pParse->db, pTab->pSchema);
        const int iCsr = pParse->nTab++;     
        Index *pIdx;                         
        KeyInfo *pKeyInfo = 0;               
        Index *pBest = 0;                    
        int iRoot = pTab->tnum;              

        sqlite3CodeVerifySchema(pParse, iDb);
        sqlite3TableLock(pParse, iDb, pTab->tnum, 0, pTab->zName);

        for(pIdx=pTab->pIndex; pIdx; pIdx=pIdx->pNext){
          if( !pBest || pIdx->nColumn<pBest->nColumn ){
            pBest = pIdx;
          }
        }
        if( pBest && pBest->nColumn<pTab->nCol ){
          iRoot = pBest->tnum;
          pKeyInfo = sqlite3IndexKeyinfo(pParse, pBest);
        }

        
        sqlite3VdbeAddOp3(v, OP_OpenRead, iCsr, iRoot, iDb);
        if( pKeyInfo ){
          sqlite3VdbeChangeP4(v, -1, (char *)pKeyInfo, P4_KEYINFO_HANDOFF);
        }
        sqlite3VdbeAddOp2(v, OP_Count, iCsr, sAggInfo.aFunc[0].iMem);
        sqlite3VdbeAddOp1(v, OP_Close, iCsr);
        explainSimpleCount(pParse, pTab, pBest);
      }else
#endif 
      {
        ExprList *pMinMax = 0;
        u8 flag = minMaxQuery(p);
        if( flag ){
          assert( !ExprHasProperty(p->pEList->a[0].pExpr, EP_xIsSelect) );
          pMinMax = sqlite3ExprListDup(db, p->pEList->a[0].pExpr->x.pList,0);
          pDel = pMinMax;
          if( pMinMax && !db->mallocFailed ){
            pMinMax->a[0].sortOrder = flag!=WHERE_ORDERBY_MIN ?1:0;
            pMinMax->a[0].pExpr->op = TK_COLUMN;
          }
        }
  
        resetAccumulator(pParse, &sAggInfo);
        pWInfo = sqlite3WhereBegin(pParse, pTabList, pWhere, &pMinMax, flag);
        if( pWInfo==0 ){
          sqlite3ExprListDelete(db, pDel);
          goto select_end;
        }
        updateAccumulator(pParse, &sAggInfo);
        if( !pMinMax && flag ){
          sqlite3VdbeAddOp2(v, OP_Goto, 0, pWInfo->iBreak);
          VdbeComment((v, "%s() by index",
                (flag==WHERE_ORDERBY_MIN?"min":"max")));
        }
        sqlite3WhereEnd(pWInfo);
        finalizeAggFunctions(pParse, &sAggInfo);
      }

      pOrderBy = 0;
      sqlite3ExprIfFalse(pParse, pHaving, addrEnd, SQLITE_JUMPIFNULL);
      selectInnerLoop(pParse, p, p->pEList, 0, 0, 0, -1, 
                      pDest, addrEnd, addrEnd);
      sqlite3ExprListDelete(db, pDel);
    }
    sqlite3VdbeResolveLabel(v, addrEnd);
    
  } 

  if( distinct>=0 ){
    explainTempTable(pParse, "DISTINCT");
  }

  if( pOrderBy ){
    explainTempTable(pParse, "ORDER BY");
    generateSortTail(pParse, p, v, pEList->nExpr, pDest);
  }

  sqlite3VdbeResolveLabel(v, iEnd);

  rc = 0;

select_end:
  explainSetInteger(pParse->iSelectId, iRestoreSelectId);

  if( rc==SQLITE_OK && pDest->eDest==SRT_Output ){
    generateColumnNames(pParse, pTabList, pEList);
  }

  sqlite3DbFree(db, sAggInfo.aCol);
  sqlite3DbFree(db, sAggInfo.aFunc);
  return rc;
}

#if defined(SQLITE_DEBUG)
void sqlite3PrintExpr(Expr *p){
  if( !ExprHasProperty(p, EP_IntValue) && p->u.zToken ){
    sqlite3DebugPrintf("(%s", p->u.zToken);
  }else{
    sqlite3DebugPrintf("(%d", p->op);
  }
  if( p->pLeft ){
    sqlite3DebugPrintf(" ");
    sqlite3PrintExpr(p->pLeft);
  }
  if( p->pRight ){
    sqlite3DebugPrintf(" ");
    sqlite3PrintExpr(p->pRight);
  }
  sqlite3DebugPrintf(")");
}
void sqlite3PrintExprList(ExprList *pList){
  int i;
  for(i=0; i<pList->nExpr; i++){
    sqlite3PrintExpr(pList->a[i].pExpr);
    if( i<pList->nExpr-1 ){
      sqlite3DebugPrintf(", ");
    }
  }
}
void sqlite3PrintSelect(Select *p, int indent){
  sqlite3DebugPrintf("%*sSELECT(%p) ", indent, "", p);
  sqlite3PrintExprList(p->pEList);
  sqlite3DebugPrintf("\n");
  if( p->pSrc ){
    char *zPrefix;
    int i;
    zPrefix = "FROM";
    for(i=0; i<p->pSrc->nSrc; i++){
      struct SrcList_item *pItem = &p->pSrc->a[i];
      sqlite3DebugPrintf("%*s ", indent+6, zPrefix);
      zPrefix = "";
      if( pItem->pSelect ){
        sqlite3DebugPrintf("(\n");
        sqlite3PrintSelect(pItem->pSelect, indent+10);
        sqlite3DebugPrintf("%*s)", indent+8, "");
      }else if( pItem->zName ){
        sqlite3DebugPrintf("%s", pItem->zName);
      }
      if( pItem->pTab ){
        sqlite3DebugPrintf("(table: %s)", pItem->pTab->zName);
      }
      if( pItem->zAlias ){
        sqlite3DebugPrintf(" AS %s", pItem->zAlias);
      }
      if( i<p->pSrc->nSrc-1 ){
        sqlite3DebugPrintf(",");
      }
      sqlite3DebugPrintf("\n");
    }
  }
  if( p->pWhere ){
    sqlite3DebugPrintf("%*s WHERE ", indent, "");
    sqlite3PrintExpr(p->pWhere);
    sqlite3DebugPrintf("\n");
  }
  if( p->pGroupBy ){
    sqlite3DebugPrintf("%*s GROUP BY ", indent, "");
    sqlite3PrintExprList(p->pGroupBy);
    sqlite3DebugPrintf("\n");
  }
  if( p->pHaving ){
    sqlite3DebugPrintf("%*s HAVING ", indent, "");
    sqlite3PrintExpr(p->pHaving);
    sqlite3DebugPrintf("\n");
  }
  if( p->pOrderBy ){
    sqlite3DebugPrintf("%*s ORDER BY ", indent, "");
    sqlite3PrintExprList(p->pOrderBy);
    sqlite3DebugPrintf("\n");
  }
}
#endif 

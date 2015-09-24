/*
** 2008 August 18
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
** This file contains routines used for walking the parser tree and
** resolve all identifiers by associating them with a particular
** table and column.
*/
#include "sqliteInt.h"
#include <stdlib.h>
#include <string.h>

static void resolveAlias(
  Parse *pParse,         
  ExprList *pEList,      
  int iCol,              
  Expr *pExpr,           
  const char *zType      
){
  Expr *pOrig;           
  Expr *pDup;            
  sqlite3 *db;           

  assert( iCol>=0 && iCol<pEList->nExpr );
  pOrig = pEList->a[iCol].pExpr;
  assert( pOrig!=0 );
  assert( pOrig->flags & EP_Resolved );
  db = pParse->db;
  if( pOrig->op!=TK_COLUMN && zType[0]!='G' ){
    pDup = sqlite3ExprDup(db, pOrig, 0);
    pDup = sqlite3PExpr(pParse, TK_AS, pDup, 0, 0);
    if( pDup==0 ) return;
    if( pEList->a[iCol].iAlias==0 ){
      pEList->a[iCol].iAlias = (u16)(++pParse->nAlias);
    }
    pDup->iTable = pEList->a[iCol].iAlias;
  }else if( ExprHasProperty(pOrig, EP_IntValue) || pOrig->u.zToken==0 ){
    pDup = sqlite3ExprDup(db, pOrig, 0);
    if( pDup==0 ) return;
  }else{
    char *zToken = pOrig->u.zToken;
    assert( zToken!=0 );
    pOrig->u.zToken = 0;
    pDup = sqlite3ExprDup(db, pOrig, 0);
    pOrig->u.zToken = zToken;
    if( pDup==0 ) return;
    assert( (pDup->flags & (EP_Reduced|EP_TokenOnly))==0 );
    pDup->flags2 |= EP2_MallocedToken;
    pDup->u.zToken = sqlite3DbStrDup(db, zToken);
  }
  if( pExpr->flags & EP_ExpCollate ){
    pDup->pColl = pExpr->pColl;
    pDup->flags |= EP_ExpCollate;
  }

  ExprSetProperty(pExpr, EP_Static);
  sqlite3ExprDelete(db, pExpr);
  memcpy(pExpr, pDup, sizeof(*pExpr));
  sqlite3DbFree(db, pDup);
}

static int lookupName(
  Parse *pParse,       
  const char *zDb,     
  const char *zTab,    
  const char *zCol,    
  NameContext *pNC,    
  Expr *pExpr          
){
  int i, j;            
  int cnt = 0;                      
  int cntTab = 0;                   
  sqlite3 *db = pParse->db;         
  struct SrcList_item *pItem;       
  struct SrcList_item *pMatch = 0;  
  NameContext *pTopNC = pNC;        
  Schema *pSchema = 0;              
  int isTrigger = 0;

  assert( pNC );     
  assert( zCol );    
  assert( ~ExprHasAnyProperty(pExpr, EP_TokenOnly|EP_Reduced) );

  
  pExpr->iTable = -1;
  pExpr->pTab = 0;
  ExprSetIrreducible(pExpr);

  
  while( pNC && cnt==0 ){
    ExprList *pEList;
    SrcList *pSrcList = pNC->pSrcList;

    if( pSrcList ){
      for(i=0, pItem=pSrcList->a; i<pSrcList->nSrc; i++, pItem++){
        Table *pTab;
        int iDb;
        Column *pCol;
  
        pTab = pItem->pTab;
        assert( pTab!=0 && pTab->zName!=0 );
        iDb = sqlite3SchemaToIndex(db, pTab->pSchema);
        assert( pTab->nCol>0 );
        if( zTab ){
          if( pItem->zAlias ){
            char *zTabName = pItem->zAlias;
            if( sqlite3StrICmp(zTabName, zTab)!=0 ) continue;
          }else{
            char *zTabName = pTab->zName;
            if( NEVER(zTabName==0) || sqlite3StrICmp(zTabName, zTab)!=0 ){
              continue;
            }
            if( zDb!=0 && sqlite3StrICmp(db->aDb[iDb].zName, zDb)!=0 ){
              continue;
            }
          }
        }
        if( 0==(cntTab++) ){
          pExpr->iTable = pItem->iCursor;
          pExpr->pTab = pTab;
          pSchema = pTab->pSchema;
          pMatch = pItem;
        }
        for(j=0, pCol=pTab->aCol; j<pTab->nCol; j++, pCol++){
          if( sqlite3StrICmp(pCol->zName, zCol)==0 ){
            IdList *pUsing;
            cnt++;
            pExpr->iTable = pItem->iCursor;
            pExpr->pTab = pTab;
            pMatch = pItem;
            pSchema = pTab->pSchema;
            
            pExpr->iColumn = j==pTab->iPKey ? -1 : (i16)j;
            if( i<pSrcList->nSrc-1 ){
              if( pItem[1].jointype & JT_NATURAL ){
                pItem++;
                i++;
              }else if( (pUsing = pItem[1].pUsing)!=0 ){
                int k;
                for(k=0; k<pUsing->nId; k++){
                  if( sqlite3StrICmp(pUsing->a[k].zName, zCol)==0 ){
                    pItem++;
                    i++;
                    break;
                  }
                }
              }
            }
            break;
          }
        }
      }
    }

#ifndef SQLITE_OMIT_TRIGGER
    if( zDb==0 && zTab!=0 && cnt==0 && pParse->pTriggerTab!=0 ){
      int op = pParse->eTriggerOp;
      Table *pTab = 0;
      assert( op==TK_DELETE || op==TK_UPDATE || op==TK_INSERT );
      if( op!=TK_DELETE && sqlite3StrICmp("new",zTab) == 0 ){
        pExpr->iTable = 1;
        pTab = pParse->pTriggerTab;
      }else if( op!=TK_INSERT && sqlite3StrICmp("old",zTab)==0 ){
        pExpr->iTable = 0;
        pTab = pParse->pTriggerTab;
      }

      if( pTab ){ 
        int iCol;
        pSchema = pTab->pSchema;
        cntTab++;
        for(iCol=0; iCol<pTab->nCol; iCol++){
          Column *pCol = &pTab->aCol[iCol];
          if( sqlite3StrICmp(pCol->zName, zCol)==0 ){
            if( iCol==pTab->iPKey ){
              iCol = -1;
            }
            break;
          }
        }
        if( iCol>=pTab->nCol && sqlite3IsRowid(zCol) ){
          iCol = -1;        
        }
        if( iCol<pTab->nCol ){
          cnt++;
          if( iCol<0 ){
            pExpr->affinity = SQLITE_AFF_INTEGER;
          }else if( pExpr->iTable==0 ){
            testcase( iCol==31 );
            testcase( iCol==32 );
            pParse->oldmask |= (iCol>=32 ? 0xffffffff : (((u32)1)<<iCol));
          }else{
            testcase( iCol==31 );
            testcase( iCol==32 );
            pParse->newmask |= (iCol>=32 ? 0xffffffff : (((u32)1)<<iCol));
          }
          pExpr->iColumn = (i16)iCol;
          pExpr->pTab = pTab;
          isTrigger = 1;
        }
      }
    }
#endif 

    if( cnt==0 && cntTab==1 && sqlite3IsRowid(zCol) ){
      cnt = 1;
      pExpr->iColumn = -1;     
      pExpr->affinity = SQLITE_AFF_INTEGER;
    }

    if( cnt==0 && (pEList = pNC->pEList)!=0 && zTab==0 ){
      for(j=0; j<pEList->nExpr; j++){
        char *zAs = pEList->a[j].zName;
        if( zAs!=0 && sqlite3StrICmp(zAs, zCol)==0 ){
          Expr *pOrig;
          assert( pExpr->pLeft==0 && pExpr->pRight==0 );
          assert( pExpr->x.pList==0 );
          assert( pExpr->x.pSelect==0 );
          pOrig = pEList->a[j].pExpr;
          if( !pNC->allowAgg && ExprHasProperty(pOrig, EP_Agg) ){
            sqlite3ErrorMsg(pParse, "misuse of aliased aggregate %s", zAs);
            return WRC_Abort;
          }
          resolveAlias(pParse, pEList, j, pExpr, "");
          cnt = 1;
          pMatch = 0;
          assert( zTab==0 && zDb==0 );
          goto lookupname_end;
        }
      } 
    }

    if( cnt==0 ){
      pNC = pNC->pNext;
    }
  }

  if( cnt==0 && zTab==0 && ExprHasProperty(pExpr,EP_DblQuoted) ){
    pExpr->op = TK_STRING;
    pExpr->pTab = 0;
    return WRC_Prune;
  }

  if( cnt!=1 ){
    const char *zErr;
    zErr = cnt==0 ? "no such column" : "ambiguous column name";
    if( zDb ){
      sqlite3ErrorMsg(pParse, "%s: %s.%s.%s", zErr, zDb, zTab, zCol);
    }else if( zTab ){
      sqlite3ErrorMsg(pParse, "%s: %s.%s", zErr, zTab, zCol);
    }else{
      sqlite3ErrorMsg(pParse, "%s: %s", zErr, zCol);
    }
    pParse->checkSchema = 1;
    pTopNC->nErr++;
  }

  if( pExpr->iColumn>=0 && pMatch!=0 ){
    int n = pExpr->iColumn;
    testcase( n==BMS-1 );
    if( n>=BMS ){
      n = BMS-1;
    }
    assert( pMatch->iCursor==pExpr->iTable );
    pMatch->colUsed |= ((Bitmask)1)<<n;
  }

  sqlite3ExprDelete(db, pExpr->pLeft);
  pExpr->pLeft = 0;
  sqlite3ExprDelete(db, pExpr->pRight);
  pExpr->pRight = 0;
  pExpr->op = (isTrigger ? TK_TRIGGER : TK_COLUMN);
lookupname_end:
  if( cnt==1 ){
    assert( pNC!=0 );
    sqlite3AuthRead(pParse, pExpr, pSchema, pNC->pSrcList);
    for(;;){
      assert( pTopNC!=0 );
      pTopNC->nRef++;
      if( pTopNC==pNC ) break;
      pTopNC = pTopNC->pNext;
    }
    return WRC_Prune;
  } else {
    return WRC_Abort;
  }
}

Expr *sqlite3CreateColumnExpr(sqlite3 *db, SrcList *pSrc, int iSrc, int iCol){
  Expr *p = sqlite3ExprAlloc(db, TK_COLUMN, 0, 0);
  if( p ){
    struct SrcList_item *pItem = &pSrc->a[iSrc];
    p->pTab = pItem->pTab;
    p->iTable = pItem->iCursor;
    if( p->pTab->iPKey==iCol ){
      p->iColumn = -1;
    }else{
      p->iColumn = (ynVar)iCol;
      testcase( iCol==BMS );
      testcase( iCol==BMS-1 );
      pItem->colUsed |= ((Bitmask)1)<<(iCol>=BMS ? BMS-1 : iCol);
    }
    ExprSetProperty(p, EP_Resolved);
  }
  return p;
}

static int resolveExprStep(Walker *pWalker, Expr *pExpr){
  NameContext *pNC;
  Parse *pParse;

  pNC = pWalker->u.pNC;
  assert( pNC!=0 );
  pParse = pNC->pParse;
  assert( pParse==pWalker->pParse );

  if( ExprHasAnyProperty(pExpr, EP_Resolved) ) return WRC_Prune;
  ExprSetProperty(pExpr, EP_Resolved);
#ifndef NDEBUG
  if( pNC->pSrcList && pNC->pSrcList->nAlloc>0 ){
    SrcList *pSrcList = pNC->pSrcList;
    int i;
    for(i=0; i<pNC->pSrcList->nSrc; i++){
      assert( pSrcList->a[i].iCursor>=0 && pSrcList->a[i].iCursor<pParse->nTab);
    }
  }
#endif
  switch( pExpr->op ){

#if defined(SQLITE_ENABLE_UPDATE_DELETE_LIMIT) && !defined(SQLITE_OMIT_SUBQUERY)
    case TK_ROW: {
      SrcList *pSrcList = pNC->pSrcList;
      struct SrcList_item *pItem;
      assert( pSrcList && pSrcList->nSrc==1 );
      pItem = pSrcList->a; 
      pExpr->op = TK_COLUMN;
      pExpr->pTab = pItem->pTab;
      pExpr->iTable = pItem->iCursor;
      pExpr->iColumn = -1;
      pExpr->affinity = SQLITE_AFF_INTEGER;
      break;
    }
#endif 

    case TK_ID: {
      return lookupName(pParse, 0, 0, pExpr->u.zToken, pNC, pExpr);
    }
  
    case TK_DOT: {
      const char *zColumn;
      const char *zTable;
      const char *zDb;
      Expr *pRight;

      
      pRight = pExpr->pRight;
      if( pRight->op==TK_ID ){
        zDb = 0;
        zTable = pExpr->pLeft->u.zToken;
        zColumn = pRight->u.zToken;
      }else{
        assert( pRight->op==TK_DOT );
        zDb = pExpr->pLeft->u.zToken;
        zTable = pRight->pLeft->u.zToken;
        zColumn = pRight->pRight->u.zToken;
      }
      return lookupName(pParse, zDb, zTable, zColumn, pNC, pExpr);
    }

    case TK_CONST_FUNC:
    case TK_FUNCTION: {
      ExprList *pList = pExpr->x.pList;    
      int n = pList ? pList->nExpr : 0;    
      int no_such_func = 0;       
      int wrong_num_args = 0;     
      int is_agg = 0;             
      int auth;                   
      int nId;                    
      const char *zId;            
      FuncDef *pDef;              
      u8 enc = ENC(pParse->db);   

      testcase( pExpr->op==TK_CONST_FUNC );
      assert( !ExprHasProperty(pExpr, EP_xIsSelect) );
      zId = pExpr->u.zToken;
      nId = sqlite3Strlen30(zId);
      pDef = sqlite3FindFunction(pParse->db, zId, nId, n, enc, 0);
      if( pDef==0 ){
        pDef = sqlite3FindFunction(pParse->db, zId, nId, -1, enc, 0);
        if( pDef==0 ){
          no_such_func = 1;
        }else{
          wrong_num_args = 1;
        }
      }else{
        is_agg = pDef->xFunc==0;
      }
#ifndef SQLITE_OMIT_AUTHORIZATION
      if( pDef ){
        auth = sqlite3AuthCheck(pParse, SQLITE_FUNCTION, 0, pDef->zName, 0);
        if( auth!=SQLITE_OK ){
          if( auth==SQLITE_DENY ){
            sqlite3ErrorMsg(pParse, "not authorized to use function: %s",
                                    pDef->zName);
            pNC->nErr++;
          }
          pExpr->op = TK_NULL;
          return WRC_Prune;
        }
      }
#endif
      if( is_agg && !pNC->allowAgg ){
        sqlite3ErrorMsg(pParse, "misuse of aggregate function %.*s()", nId,zId);
        pNC->nErr++;
        is_agg = 0;
      }else if( no_such_func ){
        sqlite3ErrorMsg(pParse, "no such function: %.*s", nId, zId);
        pNC->nErr++;
      }else if( wrong_num_args ){
        sqlite3ErrorMsg(pParse,"wrong number of arguments to function %.*s()",
             nId, zId);
        pNC->nErr++;
      }
      if( is_agg ){
        pExpr->op = TK_AGG_FUNCTION;
        pNC->hasAgg = 1;
      }
      if( is_agg ) pNC->allowAgg = 0;
      sqlite3WalkExprList(pWalker, pList);
      if( is_agg ) pNC->allowAgg = 1;
      return WRC_Prune;
    }
#ifndef SQLITE_OMIT_SUBQUERY
    case TK_SELECT:
    case TK_EXISTS:  testcase( pExpr->op==TK_EXISTS );
#endif
    case TK_IN: {
      testcase( pExpr->op==TK_IN );
      if( ExprHasProperty(pExpr, EP_xIsSelect) ){
        int nRef = pNC->nRef;
#ifndef SQLITE_OMIT_CHECK
        if( pNC->isCheck ){
          sqlite3ErrorMsg(pParse,"subqueries prohibited in CHECK constraints");
        }
#endif
        sqlite3WalkSelect(pWalker, pExpr->x.pSelect);
        assert( pNC->nRef>=nRef );
        if( nRef!=pNC->nRef ){
          ExprSetProperty(pExpr, EP_VarSelect);
        }
      }
      break;
    }
#ifndef SQLITE_OMIT_CHECK
    case TK_VARIABLE: {
      if( pNC->isCheck ){
        sqlite3ErrorMsg(pParse,"parameters prohibited in CHECK constraints");
      }
      break;
    }
#endif
  }
  return (pParse->nErr || pParse->db->mallocFailed) ? WRC_Abort : WRC_Continue;
}

static int resolveAsName(
  Parse *pParse,     
  ExprList *pEList,  
  Expr *pE           
){
  int i;             

  UNUSED_PARAMETER(pParse);

  if( pE->op==TK_ID ){
    char *zCol = pE->u.zToken;
    for(i=0; i<pEList->nExpr; i++){
      char *zAs = pEList->a[i].zName;
      if( zAs!=0 && sqlite3StrICmp(zAs, zCol)==0 ){
        return i+1;
      }
    }
  }
  return 0;
}

static int resolveOrderByTermToExprList(
  Parse *pParse,     
  Select *pSelect,   
  Expr *pE           
){
  int i;             
  ExprList *pEList;  
  NameContext nc;    
  sqlite3 *db;       
  int rc;            
  u8 savedSuppErr;   

  assert( sqlite3ExprIsInteger(pE, &i)==0 );
  pEList = pSelect->pEList;

  memset(&nc, 0, sizeof(nc));
  nc.pParse = pParse;
  nc.pSrcList = pSelect->pSrc;
  nc.pEList = pEList;
  nc.allowAgg = 1;
  nc.nErr = 0;
  db = pParse->db;
  savedSuppErr = db->suppressErr;
  db->suppressErr = 1;
  rc = sqlite3ResolveExprNames(&nc, pE);
  db->suppressErr = savedSuppErr;
  if( rc ) return 0;

  for(i=0; i<pEList->nExpr; i++){
    if( sqlite3ExprCompare(pEList->a[i].pExpr, pE)<2 ){
      return i+1;
    }
  }

  
  return 0;
}

static void resolveOutOfRangeError(
  Parse *pParse,         
  const char *zType,     
  int i,                 
  int mx                 
){
  sqlite3ErrorMsg(pParse, 
    "%r %s BY term out of range - should be "
    "between 1 and %d", i, zType, mx);
}

static int resolveCompoundOrderBy(
  Parse *pParse,        
  Select *pSelect       
){
  int i;
  ExprList *pOrderBy;
  ExprList *pEList;
  sqlite3 *db;
  int moreToDo = 1;

  pOrderBy = pSelect->pOrderBy;
  if( pOrderBy==0 ) return 0;
  db = pParse->db;
#if SQLITE_MAX_COLUMN
  if( pOrderBy->nExpr>db->aLimit[SQLITE_LIMIT_COLUMN] ){
    sqlite3ErrorMsg(pParse, "too many terms in ORDER BY clause");
    return 1;
  }
#endif
  for(i=0; i<pOrderBy->nExpr; i++){
    pOrderBy->a[i].done = 0;
  }
  pSelect->pNext = 0;
  while( pSelect->pPrior ){
    pSelect->pPrior->pNext = pSelect;
    pSelect = pSelect->pPrior;
  }
  while( pSelect && moreToDo ){
    struct ExprList_item *pItem;
    moreToDo = 0;
    pEList = pSelect->pEList;
    assert( pEList!=0 );
    for(i=0, pItem=pOrderBy->a; i<pOrderBy->nExpr; i++, pItem++){
      int iCol = -1;
      Expr *pE, *pDup;
      if( pItem->done ) continue;
      pE = pItem->pExpr;
      if( sqlite3ExprIsInteger(pE, &iCol) ){
        if( iCol<=0 || iCol>pEList->nExpr ){
          resolveOutOfRangeError(pParse, "ORDER", i+1, pEList->nExpr);
          return 1;
        }
      }else{
        iCol = resolveAsName(pParse, pEList, pE);
        if( iCol==0 ){
          pDup = sqlite3ExprDup(db, pE, 0);
          if( !db->mallocFailed ){
            assert(pDup);
            iCol = resolveOrderByTermToExprList(pParse, pSelect, pDup);
          }
          sqlite3ExprDelete(db, pDup);
        }
      }
      if( iCol>0 ){
        CollSeq *pColl = pE->pColl;
        int flags = pE->flags & EP_ExpCollate;
        sqlite3ExprDelete(db, pE);
        pItem->pExpr = pE = sqlite3Expr(db, TK_INTEGER, 0);
        if( pE==0 ) return 1;
        pE->pColl = pColl;
        pE->flags |= EP_IntValue | flags;
        pE->u.iValue = iCol;
        pItem->iCol = (u16)iCol;
        pItem->done = 1;
      }else{
        moreToDo = 1;
      }
    }
    pSelect = pSelect->pNext;
  }
  for(i=0; i<pOrderBy->nExpr; i++){
    if( pOrderBy->a[i].done==0 ){
      sqlite3ErrorMsg(pParse, "%r ORDER BY term does not match any "
            "column in the result set", i+1);
      return 1;
    }
  }
  return 0;
}

int sqlite3ResolveOrderGroupBy(
  Parse *pParse,        
  Select *pSelect,      
  ExprList *pOrderBy,   
  const char *zType     
){
  int i;
  sqlite3 *db = pParse->db;
  ExprList *pEList;
  struct ExprList_item *pItem;

  if( pOrderBy==0 || pParse->db->mallocFailed ) return 0;
#if SQLITE_MAX_COLUMN
  if( pOrderBy->nExpr>db->aLimit[SQLITE_LIMIT_COLUMN] ){
    sqlite3ErrorMsg(pParse, "too many terms in %s BY clause", zType);
    return 1;
  }
#endif
  pEList = pSelect->pEList;
  assert( pEList!=0 );  
  for(i=0, pItem=pOrderBy->a; i<pOrderBy->nExpr; i++, pItem++){
    if( pItem->iCol ){
      if( pItem->iCol>pEList->nExpr ){
        resolveOutOfRangeError(pParse, zType, i+1, pEList->nExpr);
        return 1;
      }
      resolveAlias(pParse, pEList, pItem->iCol-1, pItem->pExpr, zType);
    }
  }
  return 0;
}

static int resolveOrderGroupBy(
  NameContext *pNC,     
  Select *pSelect,      
  ExprList *pOrderBy,   
  const char *zType     
){
  int i;                         
  int iCol;                      
  struct ExprList_item *pItem;   
  Parse *pParse;                 
  int nResult;                   

  if( pOrderBy==0 ) return 0;
  nResult = pSelect->pEList->nExpr;
  pParse = pNC->pParse;
  for(i=0, pItem=pOrderBy->a; i<pOrderBy->nExpr; i++, pItem++){
    Expr *pE = pItem->pExpr;
    iCol = resolveAsName(pParse, pSelect->pEList, pE);
    if( iCol>0 ){
      pItem->iCol = (u16)iCol;
      continue;
    }
    if( sqlite3ExprIsInteger(pE, &iCol) ){
      if( iCol<1 ){
        resolveOutOfRangeError(pParse, zType, i+1, nResult);
        return 1;
      }
      pItem->iCol = (u16)iCol;
      continue;
    }

    
    pItem->iCol = 0;
    if( sqlite3ResolveExprNames(pNC, pE) ){
      return 1;
    }
  }
  return sqlite3ResolveOrderGroupBy(pParse, pSelect, pOrderBy, zType);
}

static int resolveSelectStep(Walker *pWalker, Select *p){
  NameContext *pOuterNC;  
  NameContext sNC;        
  int isCompound;         
  int nCompound;          
  Parse *pParse;          
  ExprList *pEList;       
  int i;                  
  ExprList *pGroupBy;     
  Select *pLeftmost;      
  sqlite3 *db;            
  

  assert( p!=0 );
  if( p->selFlags & SF_Resolved ){
    return WRC_Prune;
  }
  pOuterNC = pWalker->u.pNC;
  pParse = pWalker->pParse;
  db = pParse->db;

  if( (p->selFlags & SF_Expanded)==0 ){
    sqlite3SelectPrep(pParse, p, pOuterNC);
    return (pParse->nErr || db->mallocFailed) ? WRC_Abort : WRC_Prune;
  }

  isCompound = p->pPrior!=0;
  nCompound = 0;
  pLeftmost = p;
  while( p ){
    assert( (p->selFlags & SF_Expanded)!=0 );
    assert( (p->selFlags & SF_Resolved)==0 );
    p->selFlags |= SF_Resolved;

    memset(&sNC, 0, sizeof(sNC));
    sNC.pParse = pParse;
    if( sqlite3ResolveExprNames(&sNC, p->pLimit) ||
        sqlite3ResolveExprNames(&sNC, p->pOffset) ){
      return WRC_Abort;
    }
  
    sNC.allowAgg = 1;
    sNC.pSrcList = p->pSrc;
    sNC.pNext = pOuterNC;
  
    
    pEList = p->pEList;
    assert( pEList!=0 );
    for(i=0; i<pEList->nExpr; i++){
      Expr *pX = pEList->a[i].pExpr;
      if( sqlite3ResolveExprNames(&sNC, pX) ){
        return WRC_Abort;
      }
    }
  
    for(i=0; i<p->pSrc->nSrc; i++){
      struct SrcList_item *pItem = &p->pSrc->a[i];
      if( pItem->pSelect ){
        const char *zSavedContext = pParse->zAuthContext;
        if( pItem->zName ) pParse->zAuthContext = pItem->zName;
        sqlite3ResolveSelectNames(pParse, pItem->pSelect, pOuterNC);
        pParse->zAuthContext = zSavedContext;
        if( pParse->nErr || db->mallocFailed ) return WRC_Abort;
      }
    }
  
    assert( (p->selFlags & SF_Aggregate)==0 );
    pGroupBy = p->pGroupBy;
    if( pGroupBy || sNC.hasAgg ){
      p->selFlags |= SF_Aggregate;
    }else{
      sNC.allowAgg = 0;
    }
  
    if( p->pHaving && !pGroupBy ){
      sqlite3ErrorMsg(pParse, "a GROUP BY clause is required before HAVING");
      return WRC_Abort;
    }
  
    sNC.pEList = p->pEList;
    if( sqlite3ResolveExprNames(&sNC, p->pWhere) ||
       sqlite3ResolveExprNames(&sNC, p->pHaving)
    ){
      return WRC_Abort;
    }

    sNC.pNext = 0;
    sNC.allowAgg = 1;

    if( !isCompound && resolveOrderGroupBy(&sNC, p, p->pOrderBy, "ORDER") ){
      return WRC_Abort;
    }
    if( db->mallocFailed ){
      return WRC_Abort;
    }
  
    if( pGroupBy ){
      struct ExprList_item *pItem;
    
      if( resolveOrderGroupBy(&sNC, p, pGroupBy, "GROUP") || db->mallocFailed ){
        return WRC_Abort;
      }
      for(i=0, pItem=pGroupBy->a; i<pGroupBy->nExpr; i++, pItem++){
        if( ExprHasProperty(pItem->pExpr, EP_Agg) ){
          sqlite3ErrorMsg(pParse, "aggregate functions are not allowed in "
              "the GROUP BY clause");
          return WRC_Abort;
        }
      }
    }

    p = p->pPrior;
    nCompound++;
  }

  if( isCompound && resolveCompoundOrderBy(pParse, pLeftmost) ){
    return WRC_Abort;
  }

  return WRC_Prune;
}

int sqlite3ResolveExprNames( 
  NameContext *pNC,       
  Expr *pExpr             
){
  int savedHasAgg;
  Walker w;

  if( pExpr==0 ) return 0;
#if SQLITE_MAX_EXPR_DEPTH>0
  {
    Parse *pParse = pNC->pParse;
    if( sqlite3ExprCheckHeight(pParse, pExpr->nHeight+pNC->pParse->nHeight) ){
      return 1;
    }
    pParse->nHeight += pExpr->nHeight;
  }
#endif
  savedHasAgg = pNC->hasAgg;
  pNC->hasAgg = 0;
  w.xExprCallback = resolveExprStep;
  w.xSelectCallback = resolveSelectStep;
  w.pParse = pNC->pParse;
  w.u.pNC = pNC;
  sqlite3WalkExpr(&w, pExpr);
#if SQLITE_MAX_EXPR_DEPTH>0
  pNC->pParse->nHeight -= pExpr->nHeight;
#endif
  if( pNC->nErr>0 || w.pParse->nErr>0 ){
    ExprSetProperty(pExpr, EP_Error);
  }
  if( pNC->hasAgg ){
    ExprSetProperty(pExpr, EP_Agg);
  }else if( savedHasAgg ){
    pNC->hasAgg = 1;
  }
  return ExprHasProperty(pExpr, EP_Error);
}


void sqlite3ResolveSelectNames(
  Parse *pParse,         
  Select *p,             
  NameContext *pOuterNC  
){
  Walker w;

  assert( p!=0 );
  w.xExprCallback = resolveExprStep;
  w.xSelectCallback = resolveSelectStep;
  w.pParse = pParse;
  w.u.pNC = pOuterNC;
  sqlite3WalkSelect(&w, p);
}

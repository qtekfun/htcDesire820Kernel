/*
** 2008 August 16
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This file contains routines used for walking the parser tree for
** an SQL statement.
*/
#include "sqliteInt.h"
#include <stdlib.h>
#include <string.h>


int sqlite3WalkExpr(Walker *pWalker, Expr *pExpr){
  int rc;
  if( pExpr==0 ) return WRC_Continue;
  testcase( ExprHasProperty(pExpr, EP_TokenOnly) );
  testcase( ExprHasProperty(pExpr, EP_Reduced) );
  rc = pWalker->xExprCallback(pWalker, pExpr);
  if( rc==WRC_Continue
              && !ExprHasAnyProperty(pExpr,EP_TokenOnly) ){
    if( sqlite3WalkExpr(pWalker, pExpr->pLeft) ) return WRC_Abort;
    if( sqlite3WalkExpr(pWalker, pExpr->pRight) ) return WRC_Abort;
    if( ExprHasProperty(pExpr, EP_xIsSelect) ){
      if( sqlite3WalkSelect(pWalker, pExpr->x.pSelect) ) return WRC_Abort;
    }else{
      if( sqlite3WalkExprList(pWalker, pExpr->x.pList) ) return WRC_Abort;
    }
  }
  return rc & WRC_Abort;
}

int sqlite3WalkExprList(Walker *pWalker, ExprList *p){
  int i;
  struct ExprList_item *pItem;
  if( p ){
    for(i=p->nExpr, pItem=p->a; i>0; i--, pItem++){
      if( sqlite3WalkExpr(pWalker, pItem->pExpr) ) return WRC_Abort;
    }
  }
  return WRC_Continue;
}

int sqlite3WalkSelectExpr(Walker *pWalker, Select *p){
  if( sqlite3WalkExprList(pWalker, p->pEList) ) return WRC_Abort;
  if( sqlite3WalkExpr(pWalker, p->pWhere) ) return WRC_Abort;
  if( sqlite3WalkExprList(pWalker, p->pGroupBy) ) return WRC_Abort;
  if( sqlite3WalkExpr(pWalker, p->pHaving) ) return WRC_Abort;
  if( sqlite3WalkExprList(pWalker, p->pOrderBy) ) return WRC_Abort;
  if( sqlite3WalkExpr(pWalker, p->pLimit) ) return WRC_Abort;
  if( sqlite3WalkExpr(pWalker, p->pOffset) ) return WRC_Abort;
  return WRC_Continue;
}

int sqlite3WalkSelectFrom(Walker *pWalker, Select *p){
  SrcList *pSrc;
  int i;
  struct SrcList_item *pItem;

  pSrc = p->pSrc;
  if( ALWAYS(pSrc) ){
    for(i=pSrc->nSrc, pItem=pSrc->a; i>0; i--, pItem++){
      if( sqlite3WalkSelect(pWalker, pItem->pSelect) ){
        return WRC_Abort;
      }
    }
  }
  return WRC_Continue;
} 

int sqlite3WalkSelect(Walker *pWalker, Select *p){
  int rc;
  if( p==0 || pWalker->xSelectCallback==0 ) return WRC_Continue;
  rc = WRC_Continue;
  while( p  ){
    rc = pWalker->xSelectCallback(pWalker, p);
    if( rc ) break;
    if( sqlite3WalkSelectExpr(pWalker, p) ) return WRC_Abort;
    if( sqlite3WalkSelectFrom(pWalker, p) ) return WRC_Abort;
    p = p->pPrior;
  }
  return rc & WRC_Abort;
}

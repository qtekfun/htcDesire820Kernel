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
** in order to generate code for DELETE FROM statements.
*/
#include "sqliteInt.h"

Table *sqlite3SrcListLookup(Parse *pParse, SrcList *pSrc){
  struct SrcList_item *pItem = pSrc->a;
  Table *pTab;
  assert( pItem && pSrc->nSrc==1 );
  pTab = sqlite3LocateTable(pParse, 0, pItem->zName, pItem->zDatabase);
  sqlite3DeleteTable(pParse->db, pItem->pTab);
  pItem->pTab = pTab;
  if( pTab ){
    pTab->nRef++;
  }
  if( sqlite3IndexedByLookup(pParse, pItem) ){
    pTab = 0;
  }
  return pTab;
}

int sqlite3IsReadOnly(Parse *pParse, Table *pTab, int viewOk){
  if( ( IsVirtual(pTab) 
     && sqlite3GetVTable(pParse->db, pTab)->pMod->pModule->xUpdate==0 )
   || ( (pTab->tabFlags & TF_Readonly)!=0
     && (pParse->db->flags & SQLITE_WriteSchema)==0
     && pParse->nested==0 )
  ){
    sqlite3ErrorMsg(pParse, "table %s may not be modified", pTab->zName);
    return 1;
  }

#ifndef SQLITE_OMIT_VIEW
  if( !viewOk && pTab->pSelect ){
    sqlite3ErrorMsg(pParse,"cannot modify %s because it is a view",pTab->zName);
    return 1;
  }
#endif
  return 0;
}


#if !defined(SQLITE_OMIT_VIEW) && !defined(SQLITE_OMIT_TRIGGER)
void sqlite3MaterializeView(
  Parse *pParse,       
  Table *pView,        
  Expr *pWhere,        
  int iCur             
){
  SelectDest dest;
  Select *pDup;
  sqlite3 *db = pParse->db;

  pDup = sqlite3SelectDup(db, pView->pSelect, 0);
  if( pWhere ){
    SrcList *pFrom;
    
    pWhere = sqlite3ExprDup(db, pWhere, 0);
    pFrom = sqlite3SrcListAppend(db, 0, 0, 0);
    if( pFrom ){
      assert( pFrom->nSrc==1 );
      pFrom->a[0].zAlias = sqlite3DbStrDup(db, pView->zName);
      pFrom->a[0].pSelect = pDup;
      assert( pFrom->a[0].pOn==0 );
      assert( pFrom->a[0].pUsing==0 );
    }else{
      sqlite3SelectDelete(db, pDup);
    }
    pDup = sqlite3SelectNew(pParse, 0, pFrom, pWhere, 0, 0, 0, 0, 0, 0);
  }
  sqlite3SelectDestInit(&dest, SRT_EphemTab, iCur);
  sqlite3Select(pParse, pDup, &dest);
  sqlite3SelectDelete(db, pDup);
}
#endif 

#if defined(SQLITE_ENABLE_UPDATE_DELETE_LIMIT) && !defined(SQLITE_OMIT_SUBQUERY)
Expr *sqlite3LimitWhere(
  Parse *pParse,               
  SrcList *pSrc,               
  Expr *pWhere,                
  ExprList *pOrderBy,          
  Expr *pLimit,                
  Expr *pOffset,               
  char *zStmtType              
){
  Expr *pWhereRowid = NULL;    
  Expr *pInClause = NULL;      
  Expr *pSelectRowid = NULL;   
  ExprList *pEList = NULL;     
  SrcList *pSelectSrc = NULL;  
  Select *pSelect = NULL;      

  if( pOrderBy && (pLimit == 0) ) {
    sqlite3ErrorMsg(pParse, "ORDER BY without LIMIT on %s", zStmtType);
    pParse->parseError = 1;
    goto limit_where_cleanup_2;
  }

  if( pLimit == 0 ) {
    
    assert( pOffset == 0 );
    return pWhere;
  }


  pSelectRowid = sqlite3PExpr(pParse, TK_ROW, 0, 0, 0);
  if( pSelectRowid == 0 ) goto limit_where_cleanup_2;
  pEList = sqlite3ExprListAppend(pParse, 0, pSelectRowid);
  if( pEList == 0 ) goto limit_where_cleanup_2;

  pSelectSrc = sqlite3SrcListDup(pParse->db, pSrc, 0);
  if( pSelectSrc == 0 ) {
    sqlite3ExprListDelete(pParse->db, pEList);
    goto limit_where_cleanup_2;
  }

  
  pSelect = sqlite3SelectNew(pParse,pEList,pSelectSrc,pWhere,0,0,
                             pOrderBy,0,pLimit,pOffset);
  if( pSelect == 0 ) return 0;

  
  pWhereRowid = sqlite3PExpr(pParse, TK_ROW, 0, 0, 0);
  if( pWhereRowid == 0 ) goto limit_where_cleanup_1;
  pInClause = sqlite3PExpr(pParse, TK_IN, pWhereRowid, 0, 0);
  if( pInClause == 0 ) goto limit_where_cleanup_1;

  pInClause->x.pSelect = pSelect;
  pInClause->flags |= EP_xIsSelect;
  sqlite3ExprSetHeight(pParse, pInClause);
  return pInClause;

  
limit_where_cleanup_1:
  sqlite3SelectDelete(pParse->db, pSelect);
  return 0;

limit_where_cleanup_2:
  sqlite3ExprDelete(pParse->db, pWhere);
  sqlite3ExprListDelete(pParse->db, pOrderBy);
  sqlite3ExprDelete(pParse->db, pLimit);
  sqlite3ExprDelete(pParse->db, pOffset);
  return 0;
}
#endif 

void sqlite3DeleteFrom(
  Parse *pParse,         
  SrcList *pTabList,     
  Expr *pWhere           
){
  Vdbe *v;               
  Table *pTab;           
  const char *zDb;       
  int end, addr = 0;     
  int i;                 
  WhereInfo *pWInfo;     
  Index *pIdx;           
  int iCur;              
  sqlite3 *db;           
  AuthContext sContext;  
  NameContext sNC;       
  int iDb;               
  int memCnt = -1;       
  int rcauth;            

#ifndef SQLITE_OMIT_TRIGGER
  int isView;                  
  Trigger *pTrigger;           
#endif

  memset(&sContext, 0, sizeof(sContext));
  db = pParse->db;
  if( pParse->nErr || db->mallocFailed ){
    goto delete_from_cleanup;
  }
  assert( pTabList->nSrc==1 );

  pTab = sqlite3SrcListLookup(pParse, pTabList);
  if( pTab==0 )  goto delete_from_cleanup;

#ifndef SQLITE_OMIT_TRIGGER
  pTrigger = sqlite3TriggersExist(pParse, pTab, TK_DELETE, 0, 0);
  isView = pTab->pSelect!=0;
#else
# define pTrigger 0
# define isView 0
#endif
#ifdef SQLITE_OMIT_VIEW
# undef isView
# define isView 0
#endif

  if( sqlite3ViewGetColumnNames(pParse, pTab) ){
    goto delete_from_cleanup;
  }

  if( sqlite3IsReadOnly(pParse, pTab, (pTrigger?1:0)) ){
    goto delete_from_cleanup;
  }
  iDb = sqlite3SchemaToIndex(db, pTab->pSchema);
  assert( iDb<db->nDb );
  zDb = db->aDb[iDb].zName;
  rcauth = sqlite3AuthCheck(pParse, SQLITE_DELETE, pTab->zName, 0, zDb);
  assert( rcauth==SQLITE_OK || rcauth==SQLITE_DENY || rcauth==SQLITE_IGNORE );
  if( rcauth==SQLITE_DENY ){
    goto delete_from_cleanup;
  }
  assert(!isView || pTrigger);

  assert( pTabList->nSrc==1 );
  iCur = pTabList->a[0].iCursor = pParse->nTab++;
  for(pIdx=pTab->pIndex; pIdx; pIdx=pIdx->pNext){
    pParse->nTab++;
  }

  if( isView ){
    sqlite3AuthContextPush(pParse, &sContext, pTab->zName);
  }

  v = sqlite3GetVdbe(pParse);
  if( v==0 ){
    goto delete_from_cleanup;
  }
  if( pParse->nested==0 ) sqlite3VdbeCountChanges(v);
  sqlite3BeginWriteOperation(pParse, 1, iDb);

#if !defined(SQLITE_OMIT_VIEW) && !defined(SQLITE_OMIT_TRIGGER)
  if( isView ){
    sqlite3MaterializeView(pParse, pTab, pWhere, iCur);
  }
#endif

  memset(&sNC, 0, sizeof(sNC));
  sNC.pParse = pParse;
  sNC.pSrcList = pTabList;
  if( sqlite3ResolveExprNames(&sNC, pWhere) ){
    goto delete_from_cleanup;
  }

  if( db->flags & SQLITE_CountRows ){
    memCnt = ++pParse->nMem;
    sqlite3VdbeAddOp2(v, OP_Integer, 0, memCnt);
  }

#ifndef SQLITE_OMIT_TRUNCATE_OPTIMIZATION
  if( rcauth==SQLITE_OK && pWhere==0 && !pTrigger && !IsVirtual(pTab) 
   && 0==sqlite3FkRequired(pParse, pTab, 0, 0)
  ){
    assert( !isView );
    sqlite3VdbeAddOp4(v, OP_Clear, pTab->tnum, iDb, memCnt,
                      pTab->zName, P4_STATIC);
    for(pIdx=pTab->pIndex; pIdx; pIdx=pIdx->pNext){
      assert( pIdx->pSchema==pTab->pSchema );
      sqlite3VdbeAddOp2(v, OP_Clear, pIdx->tnum, iDb);
    }
  }else
#endif 
  {
    int iRowSet = ++pParse->nMem;   
    int iRowid = ++pParse->nMem;    
    int regRowid;                   

    sqlite3VdbeAddOp2(v, OP_Null, 0, iRowSet);
    pWInfo = sqlite3WhereBegin(pParse, pTabList, pWhere,0,WHERE_DUPLICATES_OK);
    if( pWInfo==0 ) goto delete_from_cleanup;
    regRowid = sqlite3ExprCodeGetColumn(pParse, pTab, -1, iCur, iRowid);
    sqlite3VdbeAddOp2(v, OP_RowSetAdd, iRowSet, regRowid);
    if( db->flags & SQLITE_CountRows ){
      sqlite3VdbeAddOp2(v, OP_AddImm, memCnt, 1);
    }
    sqlite3WhereEnd(pWInfo);

    /* Delete every item whose key was written to the list during the
    ** database scan.  We have to delete items after the scan is complete
    ** because deleting an item can change the scan order.  */
    end = sqlite3VdbeMakeLabel(v);

    if( !isView ){
      sqlite3OpenTableAndIndices(pParse, pTab, iCur, OP_OpenWrite);
    }

    addr = sqlite3VdbeAddOp3(v, OP_RowSetRead, iRowSet, end, iRowid);

    
#ifndef SQLITE_OMIT_VIRTUALTABLE
    if( IsVirtual(pTab) ){
      const char *pVTab = (const char *)sqlite3GetVTable(db, pTab);
      sqlite3VtabMakeWritable(pParse, pTab);
      sqlite3VdbeAddOp4(v, OP_VUpdate, 0, 1, iRowid, pVTab, P4_VTAB);
      sqlite3MayAbort(pParse);
    }else
#endif
    {
      int count = (pParse->nested==0);    
      sqlite3GenerateRowDelete(pParse, pTab, iCur, iRowid, count, pTrigger, OE_Default);
    }

    
    sqlite3VdbeAddOp2(v, OP_Goto, 0, addr);
    sqlite3VdbeResolveLabel(v, end);

    
    if( !isView && !IsVirtual(pTab) ){
      for(i=1, pIdx=pTab->pIndex; pIdx; i++, pIdx=pIdx->pNext){
        sqlite3VdbeAddOp2(v, OP_Close, iCur + i, pIdx->tnum);
      }
      sqlite3VdbeAddOp1(v, OP_Close, iCur);
    }
  }

  if( pParse->nested==0 && pParse->pTriggerTab==0 ){
    sqlite3AutoincrementEnd(pParse);
  }

  if( (db->flags&SQLITE_CountRows) && !pParse->nested && !pParse->pTriggerTab ){
    sqlite3VdbeAddOp2(v, OP_ResultRow, memCnt, 1);
    sqlite3VdbeSetNumCols(v, 1);
    sqlite3VdbeSetColName(v, 0, COLNAME_NAME, "rows deleted", SQLITE_STATIC);
  }

delete_from_cleanup:
  sqlite3AuthContextPop(&sContext);
  sqlite3SrcListDelete(db, pTabList);
  sqlite3ExprDelete(db, pWhere);
  return;
}
#ifdef isView
 #undef isView
#endif
#ifdef pTrigger
 #undef pTrigger
#endif

void sqlite3GenerateRowDelete(
  Parse *pParse,     
  Table *pTab,       
  int iCur,          
  int iRowid,        
  int count,         
  Trigger *pTrigger, 
  int onconf         
){
  Vdbe *v = pParse->pVdbe;        
  int iOld = 0;                   
  int iLabel;                     

  
  assert( v );

  iLabel = sqlite3VdbeMakeLabel(v);
  sqlite3VdbeAddOp3(v, OP_NotExists, iCur, iLabel, iRowid);
 
  if( sqlite3FkRequired(pParse, pTab, 0, 0) || pTrigger ){
    u32 mask;                     
    int iCol;                     

    mask = sqlite3TriggerColmask(
        pParse, pTrigger, 0, 0, TRIGGER_BEFORE|TRIGGER_AFTER, pTab, onconf
    );
    mask |= sqlite3FkOldmask(pParse, pTab);
    iOld = pParse->nMem+1;
    pParse->nMem += (1 + pTab->nCol);

    sqlite3VdbeAddOp2(v, OP_Copy, iRowid, iOld);
    for(iCol=0; iCol<pTab->nCol; iCol++){
      if( mask==0xffffffff || mask&(1<<iCol) ){
        sqlite3ExprCodeGetColumnOfTable(v, pTab, iCur, iCol, iOld+iCol+1);
      }
    }

    
    sqlite3CodeRowTrigger(pParse, pTrigger, 
        TK_DELETE, 0, TRIGGER_BEFORE, pTab, iOld, onconf, iLabel
    );

    sqlite3VdbeAddOp3(v, OP_NotExists, iCur, iLabel, iRowid);

    sqlite3FkCheck(pParse, pTab, iOld, 0);
  }

 
  if( pTab->pSelect==0 ){
    sqlite3GenerateRowIndexDelete(pParse, pTab, iCur, 0);
    sqlite3VdbeAddOp2(v, OP_Delete, iCur, (count?OPFLAG_NCHANGE:0));
    if( count ){
      sqlite3VdbeChangeP4(v, -1, pTab->zName, P4_TRANSIENT);
    }
  }

 
  sqlite3FkActions(pParse, pTab, 0, iOld);

  
  sqlite3CodeRowTrigger(pParse, pTrigger, 
      TK_DELETE, 0, TRIGGER_AFTER, pTab, iOld, onconf, iLabel
  );

  sqlite3VdbeResolveLabel(v, iLabel);
}

void sqlite3GenerateRowIndexDelete(
  Parse *pParse,     
  Table *pTab,       
  int iCur,          
  int *aRegIdx       
){
  int i;
  Index *pIdx;
  int r1;

  for(i=1, pIdx=pTab->pIndex; pIdx; i++, pIdx=pIdx->pNext){
    if( aRegIdx!=0 && aRegIdx[i-1]==0 ) continue;
    r1 = sqlite3GenerateIndexKey(pParse, pIdx, iCur, 0, 0);
    sqlite3VdbeAddOp3(pParse->pVdbe, OP_IdxDelete, iCur+i, r1,pIdx->nColumn+1);
  }
}

int sqlite3GenerateIndexKey(
  Parse *pParse,     
  Index *pIdx,       
  int iCur,          
  int regOut,        
  int doMakeRec      
){
  Vdbe *v = pParse->pVdbe;
  int j;
  Table *pTab = pIdx->pTable;
  int regBase;
  int nCol;

  nCol = pIdx->nColumn;
  regBase = sqlite3GetTempRange(pParse, nCol+1);
  sqlite3VdbeAddOp2(v, OP_Rowid, iCur, regBase+nCol);
  for(j=0; j<nCol; j++){
    int idx = pIdx->aiColumn[j];
    if( idx==pTab->iPKey ){
      sqlite3VdbeAddOp2(v, OP_SCopy, regBase+nCol, regBase+j);
    }else{
      sqlite3VdbeAddOp3(v, OP_Column, iCur, idx, regBase+j);
      sqlite3ColumnDefault(v, pTab, idx, -1);
    }
  }
  if( doMakeRec ){
    sqlite3VdbeAddOp3(v, OP_MakeRecord, regBase, nCol+1, regOut);
    sqlite3VdbeChangeP4(v, -1, sqlite3IndexAffinityStr(v, pIdx), P4_TRANSIENT);
  }
  sqlite3ReleaseTempRange(pParse, regBase, nCol+1);
  return regBase;
}

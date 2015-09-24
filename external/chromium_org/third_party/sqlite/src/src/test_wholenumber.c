/*
** 2011 April 02
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
** This file implements a virtual table that returns the whole numbers
** between 1 and 4294967295, inclusive.
**
** Example:
**
**     CREATE VIRTUAL TABLE nums USING wholenumber;
**     SELECT value FROM nums WHERE value<10;
**
** Results in:
**
**     1 2 3 4 5 6 7 8 9
*/
#include "sqlite3.h"
#include <assert.h>
#include <string.h>

#ifndef SQLITE_OMIT_VIRTUALTABLE


typedef struct wholenumber_cursor wholenumber_cursor;
struct wholenumber_cursor {
  sqlite3_vtab_cursor base;  
  unsigned iValue;           
  unsigned mxValue;          
};

static int wholenumberConnect(
  sqlite3 *db,
  void *pAux,
  int argc, const char *const*argv,
  sqlite3_vtab **ppVtab,
  char **pzErr
){
  sqlite3_vtab *pNew;
  pNew = *ppVtab = sqlite3_malloc( sizeof(*pNew) );
  if( pNew==0 ) return SQLITE_NOMEM;
  sqlite3_declare_vtab(db, "CREATE TABLE x(value)");
  memset(pNew, 0, sizeof(*pNew));
  return SQLITE_OK;
}

static int wholenumberDisconnect(sqlite3_vtab *pVtab){
  sqlite3_free(pVtab);
  return SQLITE_OK;
}


static int wholenumberOpen(sqlite3_vtab *p, sqlite3_vtab_cursor **ppCursor){
  wholenumber_cursor *pCur;
  pCur = sqlite3_malloc( sizeof(*pCur) );
  if( pCur==0 ) return SQLITE_NOMEM;
  memset(pCur, 0, sizeof(*pCur));
  *ppCursor = &pCur->base;
  return SQLITE_OK;
}

static int wholenumberClose(sqlite3_vtab_cursor *cur){
  sqlite3_free(cur);
  return SQLITE_OK;
}


static int wholenumberNext(sqlite3_vtab_cursor *cur){
  wholenumber_cursor *pCur = (wholenumber_cursor*)cur;
  pCur->iValue++;
  return SQLITE_OK;
}

static int wholenumberColumn(
  sqlite3_vtab_cursor *cur,
  sqlite3_context *ctx,
  int i
){
  wholenumber_cursor *pCur = (wholenumber_cursor*)cur;
  sqlite3_result_int64(ctx, pCur->iValue);
  return SQLITE_OK;
}

static int wholenumberRowid(sqlite3_vtab_cursor *cur, sqlite_int64 *pRowid){
  wholenumber_cursor *pCur = (wholenumber_cursor*)cur;
  *pRowid = pCur->iValue;
  return SQLITE_OK;
}

static int wholenumberEof(sqlite3_vtab_cursor *cur){
  wholenumber_cursor *pCur = (wholenumber_cursor*)cur;
  return pCur->iValue>pCur->mxValue || pCur->iValue==0;
}

static int wholenumberFilter(
  sqlite3_vtab_cursor *pVtabCursor, 
  int idxNum, const char *idxStr,
  int argc, sqlite3_value **argv
){
  wholenumber_cursor *pCur = (wholenumber_cursor *)pVtabCursor;
  sqlite3_int64 v;
  int i = 0;
  pCur->iValue = 1;
  pCur->mxValue = 0xffffffff;  
  if( idxNum & 3 ){
    v = sqlite3_value_int64(argv[0]) + (idxNum&1);
    if( v>pCur->iValue && v<=pCur->mxValue ) pCur->iValue = v;
    i++;
  }
  if( idxNum & 12 ){
    v = sqlite3_value_int64(argv[i]) - ((idxNum>>2)&1);
    if( v>=pCur->iValue && v<pCur->mxValue ) pCur->mxValue = v;
  }
  return SQLITE_OK;
}

static int wholenumberBestIndex(
  sqlite3_vtab *tab,
  sqlite3_index_info *pIdxInfo
){
  int i;
  int idxNum = 0;
  int argvIdx = 1;
  int ltIdx = -1;
  int gtIdx = -1;
  const struct sqlite3_index_constraint *pConstraint;
  pConstraint = pIdxInfo->aConstraint;
  for(i=0; i<pIdxInfo->nConstraint; i++, pConstraint++){
    if( pConstraint->usable==0 ) continue;
    if( (idxNum & 3)==0 && pConstraint->op==SQLITE_INDEX_CONSTRAINT_GT ){
      idxNum |= 1;
      ltIdx = i;
    }
    if( (idxNum & 3)==0 && pConstraint->op==SQLITE_INDEX_CONSTRAINT_GE ){
      idxNum |= 2;
      ltIdx = i;
    }
    if( (idxNum & 12)==0 && pConstraint->op==SQLITE_INDEX_CONSTRAINT_LT ){
      idxNum |= 4;
      gtIdx = i;
    }
    if( (idxNum & 12)==0 && pConstraint->op==SQLITE_INDEX_CONSTRAINT_LE ){
      idxNum |= 8;
      gtIdx = i;
    }
  }
  pIdxInfo->idxNum = idxNum;
  if( ltIdx>=0 ){
    pIdxInfo->aConstraintUsage[ltIdx].argvIndex = argvIdx++;
    pIdxInfo->aConstraintUsage[ltIdx].omit = 1;
  }
  if( gtIdx>=0 ){
    pIdxInfo->aConstraintUsage[gtIdx].argvIndex = argvIdx;
    pIdxInfo->aConstraintUsage[gtIdx].omit = 1;
  }
  if( pIdxInfo->nOrderBy==1
   && pIdxInfo->aOrderBy[0].desc==0
  ){
    pIdxInfo->orderByConsumed = 1;
  }
  pIdxInfo->estimatedCost = (double)1;
  return SQLITE_OK;
}

static sqlite3_module wholenumberModule = {
  0,                         
  wholenumberConnect,
  wholenumberConnect,
  wholenumberBestIndex,
  wholenumberDisconnect, 
  wholenumberDisconnect,
  wholenumberOpen,           
  wholenumberClose,          
  wholenumberFilter,         
  wholenumberNext,           
  wholenumberEof,            
  wholenumberColumn,         
  wholenumberRowid,          
  0,                         
  0,                         
  0,                         
  0,                         
  0,                         
  0,                         
  0,                         
};

#endif 


int wholenumber_register(sqlite3 *db){
  int rc = SQLITE_OK;
#ifndef SQLITE_OMIT_VIRTUALTABLE
  rc = sqlite3_create_module(db, "wholenumber", &wholenumberModule, 0);
#endif
  return rc;
}

#ifdef SQLITE_TEST
#include <tcl.h>
extern int getDbPointer(Tcl_Interp *interp, const char *zA, sqlite3 **ppDb);

static int register_wholenumber_module(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  sqlite3 *db;
  if( objc!=2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "DB");
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;
  wholenumber_register(db);
  return TCL_OK;
}


int Sqlitetestwholenumber_Init(Tcl_Interp *interp){
  static struct {
     char *zName;
     Tcl_ObjCmdProc *xProc;
     void *clientData;
  } aObjCmd[] = {
     { "register_wholenumber_module",   register_wholenumber_module, 0 },
  };
  int i;
  for(i=0; i<sizeof(aObjCmd)/sizeof(aObjCmd[0]); i++){
    Tcl_CreateObjCommand(interp, aObjCmd[i].zName, 
        aObjCmd[i].xProc, aObjCmd[i].clientData, 0);
  }
  return TCL_OK;
}

#endif 

/*
** 2006 June 13
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Code for testing the virtual table interfaces.  This code
** is not included in the SQLite library.  It is used for automated
** testing of the SQLite library.
**
** The emphasis of this file is a virtual table that provides
** access to TCL variables.
*/
#include "sqliteInt.h"
#include "tcl.h"
#include <stdlib.h>
#include <string.h>

#ifndef SQLITE_OMIT_VIRTUALTABLE

typedef struct tclvar_vtab tclvar_vtab;
typedef struct tclvar_cursor tclvar_cursor;

struct tclvar_vtab {
  sqlite3_vtab base;
  Tcl_Interp *interp;
};

struct tclvar_cursor {
  sqlite3_vtab_cursor base;

  Tcl_Obj *pList1;     
  Tcl_Obj *pList2;     
  int i1;              
  int i2;              
};

static int tclvarConnect(
  sqlite3 *db,
  void *pAux,
  int argc, const char *const*argv,
  sqlite3_vtab **ppVtab,
  char **pzErr
){
  tclvar_vtab *pVtab;
  static const char zSchema[] = 
     "CREATE TABLE whatever(name TEXT, arrayname TEXT, value TEXT)";
  pVtab = sqlite3MallocZero( sizeof(*pVtab) );
  if( pVtab==0 ) return SQLITE_NOMEM;
  *ppVtab = &pVtab->base;
  pVtab->interp = (Tcl_Interp *)pAux;
  sqlite3_declare_vtab(db, zSchema);
  return SQLITE_OK;
}

static int tclvarDisconnect(sqlite3_vtab *pVtab){
  sqlite3_free(pVtab);
  return SQLITE_OK;
}

static int tclvarOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor){
  tclvar_cursor *pCur;
  pCur = sqlite3MallocZero(sizeof(tclvar_cursor));
  *ppCursor = &pCur->base;
  return SQLITE_OK;
}

static int tclvarClose(sqlite3_vtab_cursor *cur){
  tclvar_cursor *pCur = (tclvar_cursor *)cur;
  if( pCur->pList1 ){
    Tcl_DecrRefCount(pCur->pList1);
  }
  if( pCur->pList2 ){
    Tcl_DecrRefCount(pCur->pList2);
  }
  sqlite3_free(pCur);
  return SQLITE_OK;
}

static int next2(Tcl_Interp *interp, tclvar_cursor *pCur, Tcl_Obj *pObj){
  Tcl_Obj *p;

  if( pObj ){
    if( !pCur->pList2 ){
      p = Tcl_NewStringObj("array names", -1);
      Tcl_IncrRefCount(p);
      Tcl_ListObjAppendElement(0, p, pObj);
      Tcl_EvalObjEx(interp, p, TCL_EVAL_GLOBAL);
      Tcl_DecrRefCount(p);
      pCur->pList2 = Tcl_GetObjResult(interp);
      Tcl_IncrRefCount(pCur->pList2);
      assert( pCur->i2==0 );
    }else{
      int n = 0;
      pCur->i2++;
      Tcl_ListObjLength(0, pCur->pList2, &n);
      if( pCur->i2>=n ){
        Tcl_DecrRefCount(pCur->pList2);
        pCur->pList2 = 0;
        pCur->i2 = 0;
        return 0;
      }
    }
  }

  return 1;
}

static int tclvarNext(sqlite3_vtab_cursor *cur){
  Tcl_Obj *pObj;
  int n = 0;
  int ok = 0;

  tclvar_cursor *pCur = (tclvar_cursor *)cur;
  Tcl_Interp *interp = ((tclvar_vtab *)(cur->pVtab))->interp;

  Tcl_ListObjLength(0, pCur->pList1, &n);
  while( !ok && pCur->i1<n ){
    Tcl_ListObjIndex(0, pCur->pList1, pCur->i1, &pObj);
    ok = next2(interp, pCur, pObj);
    if( !ok ){
      pCur->i1++;
    }
  }

  return 0;
}

static int tclvarFilter(
  sqlite3_vtab_cursor *pVtabCursor, 
  int idxNum, const char *idxStr,
  int argc, sqlite3_value **argv
){
  tclvar_cursor *pCur = (tclvar_cursor *)pVtabCursor;
  Tcl_Interp *interp = ((tclvar_vtab *)(pVtabCursor->pVtab))->interp;

  Tcl_Obj *p = Tcl_NewStringObj("info vars", -1);
  Tcl_IncrRefCount(p);

  assert( argc==0 || argc==1 );
  if( argc==1 ){
    Tcl_Obj *pArg = Tcl_NewStringObj((char*)sqlite3_value_text(argv[0]), -1);
    Tcl_ListObjAppendElement(0, p, pArg);
  }
  Tcl_EvalObjEx(interp, p, TCL_EVAL_GLOBAL);
  if( pCur->pList1 ){
    Tcl_DecrRefCount(pCur->pList1);
  }
  if( pCur->pList2 ){
    Tcl_DecrRefCount(pCur->pList2);
    pCur->pList2 = 0;
  }
  pCur->i1 = 0;
  pCur->i2 = 0;
  pCur->pList1 = Tcl_GetObjResult(interp);
  Tcl_IncrRefCount(pCur->pList1);
  assert( pCur->i1==0 && pCur->i2==0 && pCur->pList2==0 );

  Tcl_DecrRefCount(p);
  return tclvarNext(pVtabCursor);
}

static int tclvarColumn(sqlite3_vtab_cursor *cur, sqlite3_context *ctx, int i){
  Tcl_Obj *p1;
  Tcl_Obj *p2;
  const char *z1; 
  const char *z2 = "";
  tclvar_cursor *pCur = (tclvar_cursor*)cur;
  Tcl_Interp *interp = ((tclvar_vtab *)cur->pVtab)->interp;

  Tcl_ListObjIndex(interp, pCur->pList1, pCur->i1, &p1);
  Tcl_ListObjIndex(interp, pCur->pList2, pCur->i2, &p2);
  z1 = Tcl_GetString(p1);
  if( p2 ){
    z2 = Tcl_GetString(p2);
  }
  switch (i) {
    case 0: {
      sqlite3_result_text(ctx, z1, -1, SQLITE_TRANSIENT);
      break;
    }
    case 1: {
      sqlite3_result_text(ctx, z2, -1, SQLITE_TRANSIENT);
      break;
    }
    case 2: {
      Tcl_Obj *pVal = Tcl_GetVar2Ex(interp, z1, *z2?z2:0, TCL_GLOBAL_ONLY);
      sqlite3_result_text(ctx, Tcl_GetString(pVal), -1, SQLITE_TRANSIENT);
      break;
    }
  }
  return SQLITE_OK;
}

static int tclvarRowid(sqlite3_vtab_cursor *cur, sqlite_int64 *pRowid){
  *pRowid = 0;
  return SQLITE_OK;
}

static int tclvarEof(sqlite3_vtab_cursor *cur){
  tclvar_cursor *pCur = (tclvar_cursor*)cur;
  return (pCur->pList2?0:1);
}

static int tclvarBestIndex(sqlite3_vtab *tab, sqlite3_index_info *pIdxInfo){
  int ii;

  for(ii=0; ii<pIdxInfo->nConstraint; ii++){
    struct sqlite3_index_constraint const *pCons = &pIdxInfo->aConstraint[ii];
    if( pCons->iColumn==0 && pCons->usable
           && pCons->op==SQLITE_INDEX_CONSTRAINT_EQ ){
      struct sqlite3_index_constraint_usage *pUsage;
      pUsage = &pIdxInfo->aConstraintUsage[ii];
      pUsage->omit = 0;
      pUsage->argvIndex = 1;
      return SQLITE_OK;
    }
  }

  for(ii=0; ii<pIdxInfo->nConstraint; ii++){
    struct sqlite3_index_constraint const *pCons = &pIdxInfo->aConstraint[ii];
    if( pCons->iColumn==0 && pCons->usable
           && pCons->op==SQLITE_INDEX_CONSTRAINT_MATCH ){
      struct sqlite3_index_constraint_usage *pUsage;
      pUsage = &pIdxInfo->aConstraintUsage[ii];
      pUsage->omit = 1;
      pUsage->argvIndex = 1;
      return SQLITE_OK;
    }
  }

  return SQLITE_OK;
}

static sqlite3_module tclvarModule = {
  0,                         
  tclvarConnect,
  tclvarConnect,
  tclvarBestIndex,
  tclvarDisconnect, 
  tclvarDisconnect,
  tclvarOpen,                  
  tclvarClose,                 
  tclvarFilter,                
  tclvarNext,                  
  tclvarEof,                   
  tclvarColumn,                
  tclvarRowid,                 
  0,                           
  0,                           
  0,                           
  0,                           
  0,                           
  0,                           
  0,                           
};

extern int getDbPointer(Tcl_Interp *interp, const char *zA, sqlite3 **ppDb);

static int register_tclvar_module(
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
#ifndef SQLITE_OMIT_VIRTUALTABLE
  sqlite3_create_module(db, "tclvar", &tclvarModule, (void *)interp);
#endif
  return TCL_OK;
}

#endif


int Sqlitetesttclvar_Init(Tcl_Interp *interp){
#ifndef SQLITE_OMIT_VIRTUALTABLE
  static struct {
     char *zName;
     Tcl_ObjCmdProc *xProc;
     void *clientData;
  } aObjCmd[] = {
     { "register_tclvar_module",   register_tclvar_module, 0 },
  };
  int i;
  for(i=0; i<sizeof(aObjCmd)/sizeof(aObjCmd[0]); i++){
    Tcl_CreateObjCommand(interp, aObjCmd[i].zName, 
        aObjCmd[i].xProc, aObjCmd[i].clientData, 0);
  }
#endif
  return TCL_OK;
}

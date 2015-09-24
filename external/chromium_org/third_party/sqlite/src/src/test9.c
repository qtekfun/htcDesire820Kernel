/*
** 2007 March 29
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
** This file contains obscure tests of the C-interface required
** for completeness. Test code is written in C for these cases
** as there is not much point in binding to Tcl.
*/
#include "sqliteInt.h"
#include "tcl.h"
#include <stdlib.h>
#include <string.h>

static int c_collation_test(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  const char *zErrFunction = "N/A";
  sqlite3 *db;

  int rc;
  if( objc!=1 ){
    Tcl_WrongNumArgs(interp, 1, objv, "");
    return TCL_ERROR;
  }

  
  rc = sqlite3_open(":memory:", &db);
  if( rc!=SQLITE_OK ){
    zErrFunction = "sqlite3_open";
    goto error_out;
  }

  rc = sqlite3_create_collation(db, "collate", 456, 0, 0);
  if( rc!=SQLITE_MISUSE ){
    sqlite3_close(db);
    zErrFunction = "sqlite3_create_collation";
    goto error_out;
  }

  sqlite3_close(db);
  return TCL_OK;

error_out:
  Tcl_ResetResult(interp);
  Tcl_AppendResult(interp, "Error testing function: ", zErrFunction, 0);
  return TCL_ERROR;
}

static int c_realloc_test(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  void *p;
  const char *zErrFunction = "N/A";

  if( objc!=1 ){
    Tcl_WrongNumArgs(interp, 1, objv, "");
    return TCL_ERROR;
  }

  p = sqlite3_malloc(5);
  if( !p ){
    zErrFunction = "sqlite3_malloc";
    goto error_out;
  }

  p = sqlite3_realloc(p, -1);
  if( p ){
    zErrFunction = "sqlite3_realloc";
    goto error_out;
  }

  return TCL_OK;

error_out:
  Tcl_ResetResult(interp);
  Tcl_AppendResult(interp, "Error testing function: ", zErrFunction, 0);
  return TCL_ERROR;
}


static int c_misuse_test(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  const char *zErrFunction = "N/A";
  sqlite3 *db = 0;
  sqlite3_stmt *pStmt;
  int rc;

  if( objc!=1 ){
    Tcl_WrongNumArgs(interp, 1, objv, "");
    return TCL_ERROR;
  }

  rc = sqlite3_open(":memory:", &db);
  if( rc!=SQLITE_OK ){
    zErrFunction = "sqlite3_open";
    goto error_out;
  }
  sqlite3_close(db);


  rc = sqlite3_errcode(db);
  if( rc!=SQLITE_MISUSE ){
    zErrFunction = "sqlite3_errcode";
    goto error_out;
  }

  pStmt = (sqlite3_stmt*)1234;
  rc = sqlite3_prepare(db, 0, 0, &pStmt, 0);
  if( rc!=SQLITE_MISUSE ){
    zErrFunction = "sqlite3_prepare";
    goto error_out;
  }
  assert( pStmt==0 ); 

  pStmt = (sqlite3_stmt*)1234;
  rc = sqlite3_prepare_v2(db, 0, 0, &pStmt, 0);
  if( rc!=SQLITE_MISUSE ){
    zErrFunction = "sqlite3_prepare_v2";
    goto error_out;
  }
  assert( pStmt==0 );

#ifndef SQLITE_OMIT_UTF16
  pStmt = (sqlite3_stmt*)1234;
  rc = sqlite3_prepare16(db, 0, 0, &pStmt, 0);
  if( rc!=SQLITE_MISUSE ){
    zErrFunction = "sqlite3_prepare16";
    goto error_out;
  }
  assert( pStmt==0 );
  pStmt = (sqlite3_stmt*)1234;
  rc = sqlite3_prepare16_v2(db, 0, 0, &pStmt, 0);
  if( rc!=SQLITE_MISUSE ){
    zErrFunction = "sqlite3_prepare16_v2";
    goto error_out;
  }
  assert( pStmt==0 );
#endif

  return TCL_OK;

error_out:
  Tcl_ResetResult(interp);
  Tcl_AppendResult(interp, "Error testing function: ", zErrFunction, 0);
  return TCL_ERROR;
}

int Sqlitetest9_Init(Tcl_Interp *interp){
  static struct {
     char *zName;
     Tcl_ObjCmdProc *xProc;
     void *clientData;
  } aObjCmd[] = {
     { "c_misuse_test",    c_misuse_test, 0 },
     { "c_realloc_test",   c_realloc_test, 0 },
     { "c_collation_test", c_collation_test, 0 },
  };
  int i;
  for(i=0; i<sizeof(aObjCmd)/sizeof(aObjCmd[0]); i++){
    Tcl_CreateObjCommand(interp, aObjCmd[i].zName, 
        aObjCmd[i].xProc, aObjCmd[i].clientData, 0);
  }
  return TCL_OK;
}

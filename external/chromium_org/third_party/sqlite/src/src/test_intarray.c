/*
** 2009 November 10
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
** This file implements a read-only VIRTUAL TABLE that contains the
** content of a C-language array of integer values.  See the corresponding
** header file for full details.
*/
#include "test_intarray.h"
#include <string.h>
#include <assert.h>


struct sqlite3_intarray {
  int n;                    
  sqlite3_int64 *a;         
  void (*xFree)(void*);     
};

typedef struct intarray_vtab intarray_vtab;
typedef struct intarray_cursor intarray_cursor;

struct intarray_vtab {
  sqlite3_vtab base;            
  sqlite3_intarray *pContent;   
};

struct intarray_cursor {
  sqlite3_vtab_cursor base;    
  int i;                       
};

#ifndef SQLITE_OMIT_VIRTUALTABLE

static void intarrayFree(sqlite3_intarray *p){
  if( p->xFree ){
    p->xFree(p->a);
  }
  sqlite3_free(p);
}

static int intarrayDestroy(sqlite3_vtab *p){
  intarray_vtab *pVtab = (intarray_vtab*)p;
  sqlite3_free(pVtab);
  return 0;
}

static int intarrayCreate(
  sqlite3 *db,              
  void *pAux,               
  int argc,                 
  const char *const*argv,   
  sqlite3_vtab **ppVtab,    
  char **pzErr              
){
  int rc = SQLITE_NOMEM;
  intarray_vtab *pVtab = sqlite3_malloc(sizeof(intarray_vtab));

  if( pVtab ){
    memset(pVtab, 0, sizeof(intarray_vtab));
    pVtab->pContent = (sqlite3_intarray*)pAux;
    rc = sqlite3_declare_vtab(db, "CREATE TABLE x(value INTEGER PRIMARY KEY)");
  }
  *ppVtab = (sqlite3_vtab *)pVtab;
  return rc;
}

static int intarrayOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor){
  int rc = SQLITE_NOMEM;
  intarray_cursor *pCur;
  pCur = sqlite3_malloc(sizeof(intarray_cursor));
  if( pCur ){
    memset(pCur, 0, sizeof(intarray_cursor));
    *ppCursor = (sqlite3_vtab_cursor *)pCur;
    rc = SQLITE_OK;
  }
  return rc;
}

static int intarrayClose(sqlite3_vtab_cursor *cur){
  intarray_cursor *pCur = (intarray_cursor *)cur;
  sqlite3_free(pCur);
  return SQLITE_OK;
}

static int intarrayColumn(sqlite3_vtab_cursor *cur, sqlite3_context *ctx, int i){
  intarray_cursor *pCur = (intarray_cursor*)cur;
  intarray_vtab *pVtab = (intarray_vtab*)cur->pVtab;
  if( pCur->i>=0 && pCur->i<pVtab->pContent->n ){
    sqlite3_result_int64(ctx, pVtab->pContent->a[pCur->i]);
  }
  return SQLITE_OK;
}

static int intarrayRowid(sqlite3_vtab_cursor *cur, sqlite_int64 *pRowid){
  intarray_cursor *pCur = (intarray_cursor *)cur;
  *pRowid = pCur->i;
  return SQLITE_OK;
}

static int intarrayEof(sqlite3_vtab_cursor *cur){
  intarray_cursor *pCur = (intarray_cursor *)cur;
  intarray_vtab *pVtab = (intarray_vtab *)cur->pVtab;
  return pCur->i>=pVtab->pContent->n;
}

static int intarrayNext(sqlite3_vtab_cursor *cur){
  intarray_cursor *pCur = (intarray_cursor *)cur;
  pCur->i++;
  return SQLITE_OK;
}

static int intarrayFilter(
  sqlite3_vtab_cursor *pVtabCursor, 
  int idxNum, const char *idxStr,
  int argc, sqlite3_value **argv
){
  intarray_cursor *pCur = (intarray_cursor *)pVtabCursor;
  pCur->i = 0;
  return SQLITE_OK;
}

static int intarrayBestIndex(sqlite3_vtab *tab, sqlite3_index_info *pIdxInfo){
  return SQLITE_OK;
}

static sqlite3_module intarrayModule = {
  0,                           
  intarrayCreate,              
  intarrayCreate,              
  intarrayBestIndex,           
  intarrayDestroy,             
  intarrayDestroy,             
  intarrayOpen,                
  intarrayClose,               
  intarrayFilter,              
  intarrayNext,                
  intarrayEof,                 
  intarrayColumn,              
  intarrayRowid,               
  0,                           
  0,                           
  0,                           
  0,                           
  0,                           
  0,                           
  0,                           
};

#endif 

int sqlite3_intarray_create(
  sqlite3 *db,
  const char *zName,
  sqlite3_intarray **ppReturn
){
  int rc = SQLITE_OK;
#ifndef SQLITE_OMIT_VIRTUALTABLE
  sqlite3_intarray *p;

  *ppReturn = p = sqlite3_malloc( sizeof(*p) );
  if( p==0 ){
    return SQLITE_NOMEM;
  }
  memset(p, 0, sizeof(*p));
  rc = sqlite3_create_module_v2(db, zName, &intarrayModule, p,
                                (void(*)(void*))intarrayFree);
  if( rc==SQLITE_OK ){
    char *zSql;
    zSql = sqlite3_mprintf("CREATE VIRTUAL TABLE temp.%Q USING %Q",
                           zName, zName);
    rc = sqlite3_exec(db, zSql, 0, 0, 0);
    sqlite3_free(zSql);
  }
#endif
  return rc;
}

int sqlite3_intarray_bind(
  sqlite3_intarray *pIntArray,   
  int nElements,                 
  sqlite3_int64 *aElements,      
  void (*xFree)(void*)           
){
  if( pIntArray->xFree ){
    pIntArray->xFree(pIntArray->a);
  }
  pIntArray->n = nElements;
  pIntArray->a = aElements;
  pIntArray->xFree = xFree;
  return SQLITE_OK;
}


#ifdef SQLITE_TEST
#include <tcl.h>

extern int getDbPointer(Tcl_Interp *interp, const char *zA, sqlite3 **ppDb);
extern void *sqlite3TestTextToPtr(const char*);
extern int sqlite3TestMakePointerStr(Tcl_Interp*, char *zPtr, void*);
extern const char *sqlite3TestErrorName(int);

static int test_intarray_create(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  sqlite3 *db;
  const char *zName;
  sqlite3_intarray *pArray;
  int rc = SQLITE_OK;
  char zPtr[100];

  if( objc!=3 ){
    Tcl_WrongNumArgs(interp, 1, objv, "DB");
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;
  zName = Tcl_GetString(objv[2]);
#ifndef SQLITE_OMIT_VIRTUALTABLE
  rc = sqlite3_intarray_create(db, zName, &pArray);
#endif
  if( rc!=SQLITE_OK ){
    assert( pArray==0 );
    Tcl_AppendResult(interp, sqlite3TestErrorName(rc), (char*)0);
    return TCL_ERROR;
  }
  sqlite3TestMakePointerStr(interp, zPtr, pArray);
  Tcl_AppendResult(interp, zPtr, (char*)0);
  return TCL_OK;
}

static int test_intarray_bind(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  sqlite3_intarray *pArray;
  int rc = SQLITE_OK;
  int i, n;
  sqlite3_int64 *a;

  if( objc<2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "INTARRAY");
    return TCL_ERROR;
  }
  pArray = (sqlite3_intarray*)sqlite3TestTextToPtr(Tcl_GetString(objv[1]));
  n = objc - 2;
#ifndef SQLITE_OMIT_VIRTUALTABLE
  a = sqlite3_malloc( sizeof(a[0])*n );
  if( a==0 ){
    Tcl_AppendResult(interp, "SQLITE_NOMEM", (char*)0);
    return TCL_ERROR;
  }
  for(i=0; i<n; i++){
    a[i] = 0;
    Tcl_GetWideIntFromObj(0, objv[i+2], &a[i]);
  }
  rc = sqlite3_intarray_bind(pArray, n, a, sqlite3_free);
  if( rc!=SQLITE_OK ){
    Tcl_AppendResult(interp, sqlite3TestErrorName(rc), (char*)0);
    return TCL_ERROR;
  }
#endif
  return TCL_OK;
}

int Sqlitetestintarray_Init(Tcl_Interp *interp){
  static struct {
     char *zName;
     Tcl_ObjCmdProc *xProc;
     void *clientData;
  } aObjCmd[] = {
     { "sqlite3_intarray_create", test_intarray_create, 0 },
     { "sqlite3_intarray_bind", test_intarray_bind, 0 },
  };
  int i;
  for(i=0; i<sizeof(aObjCmd)/sizeof(aObjCmd[0]); i++){
    Tcl_CreateObjCommand(interp, aObjCmd[i].zName, 
        aObjCmd[i].xProc, aObjCmd[i].clientData, 0);
  }
  return TCL_OK;
}

#endif 

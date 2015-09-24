/*
** 2010 November 19
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Example code for obtaining an exclusive lock on an SQLite database
** file. This method is complicated, but works for both WAL and rollback
** mode database files. The interface to the example code in this file 
** consists of the following two functions:
**
**   sqlite3demo_superlock()
**   sqlite3demo_superunlock()
*/

#include <sqlite3.h>
#include <string.h>               
#include <assert.h>               

struct SuperlockBusy {
  int (*xBusy)(void*,int);        
  void *pBusyArg;                 
  int nBusy;                      
};
typedef struct SuperlockBusy SuperlockBusy;

struct Superlock {
  sqlite3 *db;                    
  int bWal;                       
};
typedef struct Superlock Superlock;

static int superlockBusyHandler(void *pCtx, int UNUSED){
  SuperlockBusy *pBusy = (SuperlockBusy *)pCtx;
  if( pBusy->xBusy==0 ) return 0;
  return pBusy->xBusy(pBusy->pBusyArg, pBusy->nBusy++);
}

static int superlockIsWal(Superlock *pLock){
  int rc;                         
  sqlite3_stmt *pStmt;            

  rc = sqlite3_prepare(pLock->db, "PRAGMA main.journal_mode", -1, &pStmt, 0);
  if( rc!=SQLITE_OK ) return rc;

  pLock->bWal = 0;
  if( SQLITE_ROW==sqlite3_step(pStmt) ){
    const char *zMode = (const char *)sqlite3_column_text(pStmt, 0);
    if( zMode && strlen(zMode)==3 && sqlite3_strnicmp("wal", zMode, 3)==0 ){
      pLock->bWal = 1;
    }
  }

  return sqlite3_finalize(pStmt);
}

static int superlockShmLock(
  sqlite3_file *fd,               
  int idx,                        
  int nByte,                      
  SuperlockBusy *pBusy            
){
  int rc;
  int (*xShmLock)(sqlite3_file*, int, int, int) = fd->pMethods->xShmLock;
  do {
    rc = xShmLock(fd, idx, nByte, SQLITE_SHM_LOCK|SQLITE_SHM_EXCLUSIVE);
  }while( rc==SQLITE_BUSY && superlockBusyHandler((void *)pBusy, 0) );
  return rc;
}

static int superlockWalLock(
  sqlite3 *db,                    
  SuperlockBusy *pBusy            
){
  int rc;                         
  sqlite3_file *fd = 0;           
  void volatile *p = 0;           

  
  rc = sqlite3_file_control(db, "main", SQLITE_FCNTL_FILE_POINTER, (void *)&fd);
  if( rc!=SQLITE_OK ) return rc;

  rc = superlockShmLock(fd, 2, 1, pBusy);
  if( rc!=SQLITE_OK ) return rc;

  rc = fd->pMethods->xShmMap(fd, 0, 32*1024, 1, &p);
  if( rc!=SQLITE_OK ) return rc;
  memset((void *)p, 0, 32);

  rc = superlockShmLock(fd, 3, SQLITE_SHM_NLOCK-3, pBusy);
  return rc;
}

void sqlite3demo_superunlock(void *pLock){
  Superlock *p = (Superlock *)pLock;
  if( p->bWal ){
    int rc;                         
    int flags = SQLITE_SHM_UNLOCK | SQLITE_SHM_EXCLUSIVE;
    sqlite3_file *fd = 0;
    rc = sqlite3_file_control(p->db, "main", SQLITE_FCNTL_FILE_POINTER, (void *)&fd);
    if( rc==SQLITE_OK ){
      fd->pMethods->xShmLock(fd, 2, 1, flags);
      fd->pMethods->xShmLock(fd, 3, SQLITE_SHM_NLOCK-3, flags);
    }
  }
  sqlite3_close(p->db);
  sqlite3_free(p);
}

int sqlite3demo_superlock(
  const char *zPath,              
  const char *zVfs,               
  int (*xBusy)(void*,int),        
  void *pBusyArg,                 
  void **ppLock                   
){
  SuperlockBusy busy = {0, 0, 0}; 
  int rc;                         
  Superlock *pLock;

  pLock = sqlite3_malloc(sizeof(Superlock));
  if( !pLock ) return SQLITE_NOMEM;
  memset(pLock, 0, sizeof(Superlock));

  
  rc = sqlite3_open_v2(
      zPath, &pLock->db, SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE, zVfs
  );

  if( rc==SQLITE_OK ){
    busy.xBusy = xBusy;
    busy.pBusyArg = pBusyArg;
    sqlite3_busy_handler(pLock->db, superlockBusyHandler, (void *)&busy);
    rc = sqlite3_exec(pLock->db, "BEGIN EXCLUSIVE", 0, 0, 0);
  }

  if( rc==SQLITE_OK ){
    if( SQLITE_OK==(rc = superlockIsWal(pLock)) && pLock->bWal ){
      rc = sqlite3_exec(pLock->db, "COMMIT", 0, 0, 0);
      if( rc==SQLITE_OK ){
        rc = superlockWalLock(pLock->db, &busy);
      }
    }
  }

  if( rc!=SQLITE_OK ){
    sqlite3demo_superunlock(pLock);
    *ppLock = 0;
  }else{
    *ppLock = pLock;
  }

  return rc;
}



#ifdef SQLITE_TEST

#include <tcl.h>

struct InterpAndScript {
  Tcl_Interp *interp;
  Tcl_Obj *pScript;
};
typedef struct InterpAndScript InterpAndScript;

static void superunlock_del(ClientData cd){
  sqlite3demo_superunlock((void *)cd);
}

static int superunlock_cmd(
  ClientData cd,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  if( objc!=1 ){
    Tcl_WrongNumArgs(interp, 1, objv, "");
    return TCL_ERROR;
  }
  Tcl_DeleteCommand(interp, Tcl_GetString(objv[0]));
  return TCL_OK;
}

static int superlock_busy(void *pCtx, int nBusy){
  InterpAndScript *p = (InterpAndScript *)pCtx;
  Tcl_Obj *pEval;                 
  int iVal = 0;                   

  pEval = Tcl_DuplicateObj(p->pScript);
  Tcl_IncrRefCount(pEval);
  Tcl_ListObjAppendElement(p->interp, pEval, Tcl_NewIntObj(nBusy));
  Tcl_EvalObjEx(p->interp, pEval, TCL_EVAL_GLOBAL);
  Tcl_GetIntFromObj(p->interp, Tcl_GetObjResult(p->interp), &iVal);
  Tcl_DecrRefCount(pEval);

  return iVal;
}

static int superlock_cmd(
  ClientData cd,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  void *pLock;                    
  char *zPath;
  char *zVfs = 0;
  InterpAndScript busy = {0, 0};
  int (*xBusy)(void*,int) = 0;    
  int rc;                         

  if( objc<3 || objc>5 ){
    Tcl_WrongNumArgs(
        interp, 1, objv, "CMDNAME PATH ?VFS? ?BUSY-HANDLER-SCRIPT?");
    return TCL_ERROR;
  }

  zPath = Tcl_GetString(objv[2]);

  if( objc>3 ){
    zVfs = Tcl_GetString(objv[3]);
    if( strlen(zVfs)==0 ) zVfs = 0;
  }
  if( objc>4 ){
    busy.interp = interp;
    busy.pScript = objv[4];
    xBusy = superlock_busy;
  }

  rc = sqlite3demo_superlock(zPath, zVfs, xBusy, &busy, &pLock);
  assert( rc==SQLITE_OK || pLock==0 );
  assert( rc!=SQLITE_OK || pLock!=0 );

  if( rc!=SQLITE_OK ){
    extern const char *sqlite3ErrStr(int);
    Tcl_ResetResult(interp);
    Tcl_AppendResult(interp, sqlite3ErrStr(rc), 0);
    return TCL_ERROR;
  }

  Tcl_CreateObjCommand(
      interp, Tcl_GetString(objv[1]), superunlock_cmd, pLock, superunlock_del
  );
  Tcl_SetObjResult(interp, objv[1]);
  return TCL_OK;
}

int SqliteSuperlock_Init(Tcl_Interp *interp){
  Tcl_CreateObjCommand(interp, "sqlite3demo_superlock", superlock_cmd, 0, 0);
  return TCL_OK;
}
#endif

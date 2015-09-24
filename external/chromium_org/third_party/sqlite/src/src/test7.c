/*
** 2006 January 09
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Code for testing the client/server version of the SQLite library.
** Derived from test4.c.
*/
#include "sqliteInt.h"
#include "tcl.h"

#if defined(SQLITE_SERVER) && !defined(SQLITE_OMIT_SHARED_CACHE) && \
    SQLITE_OS_UNIX && SQLITE_THREADSAFE

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sched.h>
#include <ctype.h>

int sqlite3_client_open(const char*, sqlite3**);
int sqlite3_client_prepare(sqlite3*,const char*,int,
                           sqlite3_stmt**,const char**);
int sqlite3_client_step(sqlite3_stmt*);
int sqlite3_client_reset(sqlite3_stmt*);
int sqlite3_client_finalize(sqlite3_stmt*);
int sqlite3_client_close(sqlite3*);
int sqlite3_server_start(void);
int sqlite3_server_stop(void);

typedef struct Thread Thread;
struct Thread {
  char *zFilename;         
  void (*xOp)(Thread*);    
  char *zArg;              
  volatile int opnum;      
  volatile int busy;       

  volatile int completed;  
  sqlite3 *db;             
  sqlite3_stmt *pStmt;     
  char *zErr;              
  char *zStaticErr;        
  int rc;                  
  int argc;                
  const char *argv[100];   
  const char *colv[100];   
};

#define N_THREAD 26
static Thread threadset[N_THREAD];

static void *client_main(void *pArg){
  Thread *p = (Thread*)pArg;
  if( p->db ){
    sqlite3_client_close(p->db);
  }
  sqlite3_client_open(p->zFilename, &p->db);
  if( SQLITE_OK!=sqlite3_errcode(p->db) ){
    p->zErr = strdup(sqlite3_errmsg(p->db));
    sqlite3_client_close(p->db);
    p->db = 0;
  }
  p->pStmt = 0;
  p->completed = 1;
  while( p->opnum<=p->completed ) sched_yield();
  while( p->xOp ){
    if( p->zErr && p->zErr!=p->zStaticErr ){
      sqlite3_free(p->zErr);
      p->zErr = 0;
    }
    (*p->xOp)(p);
    p->completed++;
    while( p->opnum<=p->completed ) sched_yield();
  }
  if( p->pStmt ){
    sqlite3_client_finalize(p->pStmt);
    p->pStmt = 0;
  }
  if( p->db ){
    sqlite3_client_close(p->db);
    p->db = 0;
  }
  if( p->zErr && p->zErr!=p->zStaticErr ){
    sqlite3_free(p->zErr);
    p->zErr = 0;
  }
  p->completed++;
#ifndef SQLITE_OMIT_DEPRECATED
  sqlite3_thread_cleanup();
#endif
  return 0;
}

static int parse_client_id(Tcl_Interp *interp, const char *zArg){
  if( zArg==0 || zArg[0]==0 || zArg[1]!=0 || !isupper((unsigned char)zArg[0]) ){
    Tcl_AppendResult(interp, "thread ID must be an upper case letter", 0);
    return -1;
  }
  return zArg[0] - 'A';
}

static int tcl_client_create(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  const char **argv      
){
  int i;
  pthread_t x;
  int rc;

  if( argc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " ID FILENAME", 0);
    return TCL_ERROR;
  }
  i = parse_client_id(interp, argv[1]);
  if( i<0 ) return TCL_ERROR;
  if( threadset[i].busy ){
    Tcl_AppendResult(interp, "thread ", argv[1], " is already running", 0);
    return TCL_ERROR;
  }
  threadset[i].busy = 1;
  sqlite3_free(threadset[i].zFilename);
  threadset[i].zFilename = sqlite3_mprintf("%s", argv[2]);
  threadset[i].opnum = 1;
  threadset[i].completed = 0;
  rc = pthread_create(&x, 0, client_main, &threadset[i]);
  if( rc ){
    Tcl_AppendResult(interp, "failed to create the thread", 0);
    sqlite3_free(threadset[i].zFilename);
    threadset[i].busy = 0;
    return TCL_ERROR;
  }
  pthread_detach(x);
  sqlite3_server_start();
  return TCL_OK;
}

static void client_wait(Thread *p){
  while( p->opnum>p->completed ) sched_yield();
}

static int tcl_client_wait(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  const char **argv      
){
  int i;

  if( argc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " ID", 0);
    return TCL_ERROR;
  }
  i = parse_client_id(interp, argv[1]);
  if( i<0 ) return TCL_ERROR;
  if( !threadset[i].busy ){
    Tcl_AppendResult(interp, "no such thread", 0);
    return TCL_ERROR;
  }
  client_wait(&threadset[i]);
  return TCL_OK;
}

static void stop_thread(Thread *p){
  client_wait(p);
  p->xOp = 0;
  p->opnum++;
  client_wait(p);
  sqlite3_free(p->zArg);
  p->zArg = 0;
  sqlite3_free(p->zFilename);
  p->zFilename = 0;
  p->busy = 0;
}

static int tcl_client_halt(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  const char **argv      
){
  int i;

  if( argc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " ID", 0);
    return TCL_ERROR;
  }
  if( argv[1][0]=='*' && argv[1][1]==0 ){
    for(i=0; i<N_THREAD; i++){
      if( threadset[i].busy ){
        stop_thread(&threadset[i]);
      }
    }
  }else{
    i = parse_client_id(interp, argv[1]);
    if( i<0 ) return TCL_ERROR;
    if( !threadset[i].busy ){
      Tcl_AppendResult(interp, "no such thread", 0);
      return TCL_ERROR;
    }
    stop_thread(&threadset[i]);
  }

  
  for(i=0; i<N_THREAD && threadset[i].busy==0; i++){}
  if( i>=N_THREAD ){
    sqlite3_server_stop();
  }
  return TCL_OK;
}

static int tcl_client_argc(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  const char **argv      
){
  int i;
  char zBuf[100];

  if( argc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " ID", 0);
    return TCL_ERROR;
  }
  i = parse_client_id(interp, argv[1]);
  if( i<0 ) return TCL_ERROR;
  if( !threadset[i].busy ){
    Tcl_AppendResult(interp, "no such thread", 0);
    return TCL_ERROR;
  }
  client_wait(&threadset[i]);
  sprintf(zBuf, "%d", threadset[i].argc);
  Tcl_AppendResult(interp, zBuf, 0);
  return TCL_OK;
}

static int tcl_client_argv(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  const char **argv      
){
  int i;
  int n;

  if( argc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " ID N", 0);
    return TCL_ERROR;
  }
  i = parse_client_id(interp, argv[1]);
  if( i<0 ) return TCL_ERROR;
  if( !threadset[i].busy ){
    Tcl_AppendResult(interp, "no such thread", 0);
    return TCL_ERROR;
  }
  if( Tcl_GetInt(interp, argv[2], &n) ) return TCL_ERROR;
  client_wait(&threadset[i]);
  if( n<0 || n>=threadset[i].argc ){
    Tcl_AppendResult(interp, "column number out of range", 0);
    return TCL_ERROR;
  }
  Tcl_AppendResult(interp, threadset[i].argv[n], 0);
  return TCL_OK;
}

static int tcl_client_colname(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  const char **argv      
){
  int i;
  int n;

  if( argc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " ID N", 0);
    return TCL_ERROR;
  }
  i = parse_client_id(interp, argv[1]);
  if( i<0 ) return TCL_ERROR;
  if( !threadset[i].busy ){
    Tcl_AppendResult(interp, "no such thread", 0);
    return TCL_ERROR;
  }
  if( Tcl_GetInt(interp, argv[2], &n) ) return TCL_ERROR;
  client_wait(&threadset[i]);
  if( n<0 || n>=threadset[i].argc ){
    Tcl_AppendResult(interp, "column number out of range", 0);
    return TCL_ERROR;
  }
  Tcl_AppendResult(interp, threadset[i].colv[n], 0);
  return TCL_OK;
}

static int tcl_client_result(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  const char **argv      
){
  int i;
  const char *zName;

  if( argc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " ID", 0);
    return TCL_ERROR;
  }
  i = parse_client_id(interp, argv[1]);
  if( i<0 ) return TCL_ERROR;
  if( !threadset[i].busy ){
    Tcl_AppendResult(interp, "no such thread", 0);
    return TCL_ERROR;
  }
  client_wait(&threadset[i]);
  switch( threadset[i].rc ){
    case SQLITE_OK:         zName = "SQLITE_OK";          break;
    case SQLITE_ERROR:      zName = "SQLITE_ERROR";       break;
    case SQLITE_PERM:       zName = "SQLITE_PERM";        break;
    case SQLITE_ABORT:      zName = "SQLITE_ABORT";       break;
    case SQLITE_BUSY:       zName = "SQLITE_BUSY";        break;
    case SQLITE_LOCKED:     zName = "SQLITE_LOCKED";      break;
    case SQLITE_NOMEM:      zName = "SQLITE_NOMEM";       break;
    case SQLITE_READONLY:   zName = "SQLITE_READONLY";    break;
    case SQLITE_INTERRUPT:  zName = "SQLITE_INTERRUPT";   break;
    case SQLITE_IOERR:      zName = "SQLITE_IOERR";       break;
    case SQLITE_CORRUPT:    zName = "SQLITE_CORRUPT";     break;
    case SQLITE_FULL:       zName = "SQLITE_FULL";        break;
    case SQLITE_CANTOPEN:   zName = "SQLITE_CANTOPEN";    break;
    case SQLITE_PROTOCOL:   zName = "SQLITE_PROTOCOL";    break;
    case SQLITE_EMPTY:      zName = "SQLITE_EMPTY";       break;
    case SQLITE_SCHEMA:     zName = "SQLITE_SCHEMA";      break;
    case SQLITE_CONSTRAINT: zName = "SQLITE_CONSTRAINT";  break;
    case SQLITE_MISMATCH:   zName = "SQLITE_MISMATCH";    break;
    case SQLITE_MISUSE:     zName = "SQLITE_MISUSE";      break;
    case SQLITE_NOLFS:      zName = "SQLITE_NOLFS";       break;
    case SQLITE_AUTH:       zName = "SQLITE_AUTH";        break;
    case SQLITE_FORMAT:     zName = "SQLITE_FORMAT";      break;
    case SQLITE_RANGE:      zName = "SQLITE_RANGE";       break;
    case SQLITE_ROW:        zName = "SQLITE_ROW";         break;
    case SQLITE_DONE:       zName = "SQLITE_DONE";        break;
    default:                zName = "SQLITE_Unknown";     break;
  }
  Tcl_AppendResult(interp, zName, 0);
  return TCL_OK;
}

static int tcl_client_error(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  const char **argv      
){
  int i;

  if( argc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " ID", 0);
    return TCL_ERROR;
  }
  i = parse_client_id(interp, argv[1]);
  if( i<0 ) return TCL_ERROR;
  if( !threadset[i].busy ){
    Tcl_AppendResult(interp, "no such thread", 0);
    return TCL_ERROR;
  }
  client_wait(&threadset[i]);
  Tcl_AppendResult(interp, threadset[i].zErr, 0);
  return TCL_OK;
}

static void do_compile(Thread *p){
  if( p->db==0 ){
    p->zErr = p->zStaticErr = "no database is open";
    p->rc = SQLITE_ERROR;
    return;
  }
  if( p->pStmt ){
    sqlite3_client_finalize(p->pStmt);
    p->pStmt = 0;
  }
  p->rc = sqlite3_client_prepare(p->db, p->zArg, -1, &p->pStmt, 0);
}

static int tcl_client_compile(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  const char **argv      
){
  int i;
  if( argc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " ID SQL", 0);
    return TCL_ERROR;
  }
  i = parse_client_id(interp, argv[1]);
  if( i<0 ) return TCL_ERROR;
  if( !threadset[i].busy ){
    Tcl_AppendResult(interp, "no such thread", 0);
    return TCL_ERROR;
  }
  client_wait(&threadset[i]);
  threadset[i].xOp = do_compile;
  sqlite3_free(threadset[i].zArg);
  threadset[i].zArg = sqlite3_mprintf("%s", argv[2]);
  threadset[i].opnum++;
  return TCL_OK;
}

static void do_step(Thread *p){
  int i;
  if( p->pStmt==0 ){
    p->zErr = p->zStaticErr = "no virtual machine available";
    p->rc = SQLITE_ERROR;
    return;
  }
  p->rc = sqlite3_client_step(p->pStmt);
  if( p->rc==SQLITE_ROW ){
    p->argc = sqlite3_column_count(p->pStmt);
    for(i=0; i<sqlite3_data_count(p->pStmt); i++){
      p->argv[i] = (char*)sqlite3_column_text(p->pStmt, i);
    }
    for(i=0; i<p->argc; i++){
      p->colv[i] = sqlite3_column_name(p->pStmt, i);
    }
  }
}

static int tcl_client_step(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  const char **argv      
){
  int i;
  if( argc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " IDL", 0);
    return TCL_ERROR;
  }
  i = parse_client_id(interp, argv[1]);
  if( i<0 ) return TCL_ERROR;
  if( !threadset[i].busy ){
    Tcl_AppendResult(interp, "no such thread", 0);
    return TCL_ERROR;
  }
  client_wait(&threadset[i]);
  threadset[i].xOp = do_step;
  threadset[i].opnum++;
  return TCL_OK;
}

static void do_finalize(Thread *p){
  if( p->pStmt==0 ){
    p->zErr = p->zStaticErr = "no virtual machine available";
    p->rc = SQLITE_ERROR;
    return;
  }
  p->rc = sqlite3_client_finalize(p->pStmt);
  p->pStmt = 0;
}

static int tcl_client_finalize(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  const char **argv      
){
  int i;
  if( argc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " IDL", 0);
    return TCL_ERROR;
  }
  i = parse_client_id(interp, argv[1]);
  if( i<0 ) return TCL_ERROR;
  if( !threadset[i].busy ){
    Tcl_AppendResult(interp, "no such thread", 0);
    return TCL_ERROR;
  }
  client_wait(&threadset[i]);
  threadset[i].xOp = do_finalize;
  sqlite3_free(threadset[i].zArg);
  threadset[i].zArg = 0;
  threadset[i].opnum++;
  return TCL_OK;
}

static void do_reset(Thread *p){
  if( p->pStmt==0 ){
    p->zErr = p->zStaticErr = "no virtual machine available";
    p->rc = SQLITE_ERROR;
    return;
  }
  p->rc = sqlite3_client_reset(p->pStmt);
  p->pStmt = 0;
}

static int tcl_client_reset(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  const char **argv      
){
  int i;
  if( argc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " IDL", 0);
    return TCL_ERROR;
  }
  i = parse_client_id(interp, argv[1]);
  if( i<0 ) return TCL_ERROR;
  if( !threadset[i].busy ){
    Tcl_AppendResult(interp, "no such thread", 0);
    return TCL_ERROR;
  }
  client_wait(&threadset[i]);
  threadset[i].xOp = do_reset;
  sqlite3_free(threadset[i].zArg);
  threadset[i].zArg = 0;
  threadset[i].opnum++;
  return TCL_OK;
}

static int tcl_client_swap(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  const char **argv      
){
  int i, j;
  sqlite3 *temp;
  if( argc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " ID1 ID2", 0);
    return TCL_ERROR;
  }
  i = parse_client_id(interp, argv[1]);
  if( i<0 ) return TCL_ERROR;
  if( !threadset[i].busy ){
    Tcl_AppendResult(interp, "no such thread", 0);
    return TCL_ERROR;
  }
  client_wait(&threadset[i]);
  j = parse_client_id(interp, argv[2]);
  if( j<0 ) return TCL_ERROR;
  if( !threadset[j].busy ){
    Tcl_AppendResult(interp, "no such thread", 0);
    return TCL_ERROR;
  }
  client_wait(&threadset[j]);
  temp = threadset[i].db;
  threadset[i].db = threadset[j].db;
  threadset[j].db = temp;
  return TCL_OK;
}

int Sqlitetest7_Init(Tcl_Interp *interp){
  static struct {
     char *zName;
     Tcl_CmdProc *xProc;
  } aCmd[] = {
     { "client_create",     (Tcl_CmdProc*)tcl_client_create     },
     { "client_wait",       (Tcl_CmdProc*)tcl_client_wait       },
     { "client_halt",       (Tcl_CmdProc*)tcl_client_halt       },
     { "client_argc",       (Tcl_CmdProc*)tcl_client_argc       },
     { "client_argv",       (Tcl_CmdProc*)tcl_client_argv       },
     { "client_colname",    (Tcl_CmdProc*)tcl_client_colname    },
     { "client_result",     (Tcl_CmdProc*)tcl_client_result     },
     { "client_error",      (Tcl_CmdProc*)tcl_client_error      },
     { "client_compile",    (Tcl_CmdProc*)tcl_client_compile    },
     { "client_step",       (Tcl_CmdProc*)tcl_client_step       },
     { "client_reset",      (Tcl_CmdProc*)tcl_client_reset      },
     { "client_finalize",   (Tcl_CmdProc*)tcl_client_finalize   },
     { "client_swap",       (Tcl_CmdProc*)tcl_client_swap       },
  };
  int i;

  for(i=0; i<sizeof(aCmd)/sizeof(aCmd[0]); i++){
    Tcl_CreateCommand(interp, aCmd[i].zName, aCmd[i].xProc, 0, 0);
  }
  return TCL_OK;
}
#else
int Sqlitetest7_Init(Tcl_Interp *interp){ return TCL_OK; }
#endif 

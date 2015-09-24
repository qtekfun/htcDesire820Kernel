/*
** 2007 August 28
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This file contains the C functions that implement mutexes for OS/2
*/
#include "sqliteInt.h"

#ifdef SQLITE_MUTEX_OS2


struct sqlite3_mutex {
  HMTX mutex;       
  int  id;          
#ifdef SQLITE_DEBUG
 int   trace;       
#endif
};

#ifdef SQLITE_DEBUG
#define SQLITE3_MUTEX_INITIALIZER { 0, 0, 0 }
#else
#define SQLITE3_MUTEX_INITIALIZER { 0, 0 }
#endif

static int os2MutexInit(void){ return SQLITE_OK; }
static int os2MutexEnd(void){ return SQLITE_OK; }

static sqlite3_mutex *os2MutexAlloc(int iType){
  sqlite3_mutex *p = NULL;
  switch( iType ){
    case SQLITE_MUTEX_FAST:
    case SQLITE_MUTEX_RECURSIVE: {
      p = sqlite3MallocZero( sizeof(*p) );
      if( p ){
        p->id = iType;
        if( DosCreateMutexSem( 0, &p->mutex, 0, FALSE ) != NO_ERROR ){
          sqlite3_free( p );
          p = NULL;
        }
      }
      break;
    }
    default: {
      static volatile int isInit = 0;
      static sqlite3_mutex staticMutexes[6] = {
        SQLITE3_MUTEX_INITIALIZER,
        SQLITE3_MUTEX_INITIALIZER,
        SQLITE3_MUTEX_INITIALIZER,
        SQLITE3_MUTEX_INITIALIZER,
        SQLITE3_MUTEX_INITIALIZER,
        SQLITE3_MUTEX_INITIALIZER,
      };
      if ( !isInit ){
        APIRET rc;
        PTIB ptib;
        PPIB ppib;
        HMTX mutex;
        char name[32];
        DosGetInfoBlocks( &ptib, &ppib );
        sqlite3_snprintf( sizeof(name), name, "\\SEM32\\SQLITE%04x",
                          ppib->pib_ulpid );
        while( !isInit ){
          mutex = 0;
          rc = DosCreateMutexSem( name, &mutex, 0, FALSE);
          if( rc == NO_ERROR ){
            unsigned int i;
            if( !isInit ){
              for( i = 0; i < sizeof(staticMutexes)/sizeof(staticMutexes[0]); i++ ){
                DosCreateMutexSem( 0, &staticMutexes[i].mutex, 0, FALSE );
              }
              isInit = 1;
            }
            DosCloseMutexSem( mutex );
          }else if( rc == ERROR_DUPLICATE_NAME ){
            DosSleep( 1 );
          }else{
            return p;
          }
        }
      }
      assert( iType-2 >= 0 );
      assert( iType-2 < sizeof(staticMutexes)/sizeof(staticMutexes[0]) );
      p = &staticMutexes[iType-2];
      p->id = iType;
      break;
    }
  }
  return p;
}


static void os2MutexFree(sqlite3_mutex *p){
#ifdef SQLITE_DEBUG
  TID tid;
  PID pid;
  ULONG ulCount;
  DosQueryMutexSem(p->mutex, &pid, &tid, &ulCount);
  assert( ulCount==0 );
  assert( p->id==SQLITE_MUTEX_FAST || p->id==SQLITE_MUTEX_RECURSIVE );
#endif
  DosCloseMutexSem( p->mutex );
  sqlite3_free( p );
}

#ifdef SQLITE_DEBUG
static int os2MutexHeld(sqlite3_mutex *p){
  TID tid;
  PID pid;
  ULONG ulCount;
  PTIB ptib;
  DosQueryMutexSem(p->mutex, &pid, &tid, &ulCount);
  if( ulCount==0 || ( ulCount>1 && p->id!=SQLITE_MUTEX_RECURSIVE ) )
    return 0;
  DosGetInfoBlocks(&ptib, NULL);
  return tid==ptib->tib_ptib2->tib2_ultid;
}
static int os2MutexNotheld(sqlite3_mutex *p){
  TID tid;
  PID pid;
  ULONG ulCount;
  PTIB ptib;
  DosQueryMutexSem(p->mutex, &pid, &tid, &ulCount);
  if( ulCount==0 )
    return 1;
  DosGetInfoBlocks(&ptib, NULL);
  return tid!=ptib->tib_ptib2->tib2_ultid;
}
static void os2MutexTrace(sqlite3_mutex *p, char *pAction){
  TID   tid;
  PID   pid;
  ULONG ulCount;
  DosQueryMutexSem(p->mutex, &pid, &tid, &ulCount);
  printf("%s mutex %p (%d) with nRef=%ld\n", pAction, (void*)p, p->trace, ulCount);
}
#endif

static void os2MutexEnter(sqlite3_mutex *p){
  assert( p->id==SQLITE_MUTEX_RECURSIVE || os2MutexNotheld(p) );
  DosRequestMutexSem(p->mutex, SEM_INDEFINITE_WAIT);
#ifdef SQLITE_DEBUG
  if( p->trace ) os2MutexTrace(p, "enter");
#endif
}
static int os2MutexTry(sqlite3_mutex *p){
  int rc = SQLITE_BUSY;
  assert( p->id==SQLITE_MUTEX_RECURSIVE || os2MutexNotheld(p) );
  if( DosRequestMutexSem(p->mutex, SEM_IMMEDIATE_RETURN) == NO_ERROR ) {
    rc = SQLITE_OK;
#ifdef SQLITE_DEBUG
    if( p->trace ) os2MutexTrace(p, "try");
#endif
  }
  return rc;
}

static void os2MutexLeave(sqlite3_mutex *p){
  assert( os2MutexHeld(p) );
  DosReleaseMutexSem(p->mutex);
#ifdef SQLITE_DEBUG
  if( p->trace ) os2MutexTrace(p, "leave");
#endif
}

sqlite3_mutex_methods const *sqlite3DefaultMutex(void){
  static const sqlite3_mutex_methods sMutex = {
    os2MutexInit,
    os2MutexEnd,
    os2MutexAlloc,
    os2MutexFree,
    os2MutexEnter,
    os2MutexTry,
    os2MutexLeave,
#ifdef SQLITE_DEBUG
    os2MutexHeld,
    os2MutexNotheld
#else
    0,
    0
#endif
  };

  return &sMutex;
}
#endif 

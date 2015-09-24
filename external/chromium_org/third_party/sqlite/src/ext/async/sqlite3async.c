/*
** 2005 December 14
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
** $Id: sqlite3async.c,v 1.7 2009/07/18 11:52:04 danielk1977 Exp $
**
** This file contains the implementation of an asynchronous IO backend 
** for SQLite.
*/

#if !defined(SQLITE_CORE) || defined(SQLITE_ENABLE_ASYNCIO)

#include "sqlite3async.h"
#include "sqlite3.h"
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#define MIN(x,y) ((x)<(y)?(x):(y))
#define MAX(x,y) ((x)>(y)?(x):(y))

#ifndef SQLITE_AMALGAMATION
#define UNUSED_PARAMETER(x) (void)(x)
#endif

typedef struct AsyncWrite AsyncWrite;
typedef struct AsyncFile AsyncFile;
typedef struct AsyncFileData AsyncFileData;
typedef struct AsyncFileLock AsyncFileLock;
typedef struct AsyncLock AsyncLock;

#ifndef NDEBUG
#include <stdio.h>
static int sqlite3async_trace = 0;
# define ASYNC_TRACE(X) if( sqlite3async_trace ) asyncTrace X
static void asyncTrace(const char *zFormat, ...){
  char *z;
  va_list ap;
  va_start(ap, zFormat);
  z = sqlite3_vmprintf(zFormat, ap);
  va_end(ap);
  fprintf(stderr, "[%d] %s", 0 , z);
  sqlite3_free(z);
}
#else
# define ASYNC_TRACE(X)
#endif

/*
** THREAD SAFETY NOTES
**
** Basic rules:
**
**     * Both read and write access to the global write-op queue must be 
**       protected by the async.queueMutex. As are the async.ioError and
**       async.nFile variables.
**
**     * The async.pLock list and all AsyncLock and AsyncFileLock
**       structures must be protected by the async.lockMutex mutex.
**
**     * The file handles from the underlying system are not assumed to 
**       be thread safe.
**
**     * See the last two paragraphs under "The Writer Thread" for
**       an assumption to do with file-handle synchronization by the Os.
**
** Deadlock prevention:
**
**     There are three mutex used by the system: the "writer" mutex, 
**     the "queue" mutex and the "lock" mutex. Rules are:
**
**     * It is illegal to block on the writer mutex when any other mutex
**       are held, and 
**
**     * It is illegal to block on the queue mutex when the lock mutex
**       is held.
**
**     i.e. mutex's must be grabbed in the order "writer", "queue", "lock".
**
** File system operations (invoked by SQLite thread):
**
**     xOpen
**     xDelete
**     xFileExists
**
** File handle operations (invoked by SQLite thread):
**
**         asyncWrite, asyncClose, asyncTruncate, asyncSync 
**    
**     The operations above add an entry to the global write-op list. They
**     prepare the entry, acquire the async.queueMutex momentarily while
**     list pointers are  manipulated to insert the new entry, then release
**     the mutex and signal the writer thread to wake up in case it happens
**     to be asleep.
**
**    
**         asyncRead, asyncFileSize.
**
**     Read operations. Both of these read from both the underlying file
**     first then adjust their result based on pending writes in the 
**     write-op queue.   So async.queueMutex is held for the duration
**     of these operations to prevent other threads from changing the
**     queue in mid operation.
**    
**
**         asyncLock, asyncUnlock, asyncCheckReservedLock
**    
**     These primitives implement in-process locking using a hash table
**     on the file name.  Files are locked correctly for connections coming
**     from the same process.  But other processes cannot see these locks
**     and will therefore not honor them.
**
**
** The writer thread:
**
**     The async.writerMutex is used to make sure only there is only
**     a single writer thread running at a time.
**
**     Inside the writer thread is a loop that works like this:
**
**         WHILE (write-op list is not empty)
**             Do IO operation at head of write-op list
**             Remove entry from head of write-op list
**         END WHILE
**
**     The async.queueMutex is always held during the <write-op list is 
**     not empty> test, and when the entry is removed from the head
**     of the write-op list. Sometimes it is held for the interim
**     period (while the IO is performed), and sometimes it is
**     relinquished. It is relinquished if (a) the IO op is an
**     ASYNC_CLOSE or (b) when the file handle was opened, two of
**     the underlying systems handles were opened on the same
**     file-system entry.
**
**     If condition (b) above is true, then one file-handle 
**     (AsyncFile.pBaseRead) is used exclusively by sqlite threads to read the
**     file, the other (AsyncFile.pBaseWrite) by sqlite3_async_flush() 
**     threads to perform write() operations. This means that read 
**     operations are not blocked by asynchronous writes (although 
**     asynchronous writes may still be blocked by reads).
**
**     This assumes that the OS keeps two handles open on the same file
**     properly in sync. That is, any read operation that starts after a
**     write operation on the same file system entry has completed returns
**     data consistent with the write. We also assume that if one thread 
**     reads a file while another is writing it all bytes other than the
**     ones actually being written contain valid data.
**
**     If the above assumptions are not true, set the preprocessor symbol
**     SQLITE_ASYNC_TWO_FILEHANDLES to 0.
*/


#ifndef NDEBUG
# define TESTONLY( X ) X
#else
# define TESTONLY( X )
#endif

static void async_mutex_enter(int eMutex);
static void async_mutex_leave(int eMutex);
static void async_cond_wait(int eCond, int eMutex);
static void async_cond_signal(int eCond);
static void async_sched_yield(void);

static int async_os_initialize(void);
static void async_os_shutdown(void);

#define ASYNC_MUTEX_LOCK    0
#define ASYNC_MUTEX_QUEUE   1
#define ASYNC_MUTEX_WRITER  2

#define ASYNC_COND_QUEUE    0

#if SQLITE_OS_WIN || defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)

#include <windows.h>


#define mutex_held(X) (GetCurrentThreadId()==primitives.aHolder[X])

static struct AsyncPrimitives {
  int isInit;
  DWORD aHolder[3];
  CRITICAL_SECTION aMutex[3];
  HANDLE aCond[1];
} primitives = { 0 };

static int async_os_initialize(void){
  if( !primitives.isInit ){
    primitives.aCond[0] = CreateEvent(NULL, TRUE, FALSE, 0);
    if( primitives.aCond[0]==NULL ){
      return 1;
    }
    InitializeCriticalSection(&primitives.aMutex[0]);
    InitializeCriticalSection(&primitives.aMutex[1]);
    InitializeCriticalSection(&primitives.aMutex[2]);
    primitives.isInit = 1;
  }
  return 0;
}
static void async_os_shutdown(void){
  if( primitives.isInit ){
    DeleteCriticalSection(&primitives.aMutex[0]);
    DeleteCriticalSection(&primitives.aMutex[1]);
    DeleteCriticalSection(&primitives.aMutex[2]);
    CloseHandle(primitives.aCond[0]);
    primitives.isInit = 0;
  }
}

static void async_mutex_enter(int eMutex){
  assert( eMutex==0 || eMutex==1 || eMutex==2 );
  assert( eMutex!=2 || (!mutex_held(0) && !mutex_held(1) && !mutex_held(2)) );
  assert( eMutex!=1 || (!mutex_held(0) && !mutex_held(1)) );
  assert( eMutex!=0 || (!mutex_held(0)) );
  EnterCriticalSection(&primitives.aMutex[eMutex]);
  TESTONLY( primitives.aHolder[eMutex] = GetCurrentThreadId(); )
}
static void async_mutex_leave(int eMutex){
  assert( eMutex==0 || eMutex==1 || eMutex==2 );
  assert( mutex_held(eMutex) );
  TESTONLY( primitives.aHolder[eMutex] = 0; )
  LeaveCriticalSection(&primitives.aMutex[eMutex]);
}
static void async_cond_wait(int eCond, int eMutex){
  ResetEvent(primitives.aCond[eCond]);
  async_mutex_leave(eMutex);
  WaitForSingleObject(primitives.aCond[eCond], INFINITE);
  async_mutex_enter(eMutex);
}
static void async_cond_signal(int eCond){
  assert( mutex_held(ASYNC_MUTEX_QUEUE) );
  SetEvent(primitives.aCond[eCond]);
}
static void async_sched_yield(void){
  Sleep(0);
}
#else

#include <pthread.h>
#include <sched.h>

#define mutex_held(X) pthread_equal(primitives.aHolder[X], pthread_self())

static int  async_os_initialize(void) {return 0;}
static void async_os_shutdown(void) {}

static struct AsyncPrimitives {
  pthread_mutex_t aMutex[3];
  pthread_cond_t aCond[1];
  pthread_t aHolder[3];
} primitives = {
  { PTHREAD_MUTEX_INITIALIZER, 
    PTHREAD_MUTEX_INITIALIZER, 
    PTHREAD_MUTEX_INITIALIZER
  } , {
    PTHREAD_COND_INITIALIZER
  } , { 0, 0, 0 }
};

static void async_mutex_enter(int eMutex){
  assert( eMutex==0 || eMutex==1 || eMutex==2 );
  assert( eMutex!=2 || (!mutex_held(0) && !mutex_held(1) && !mutex_held(2)) );
  assert( eMutex!=1 || (!mutex_held(0) && !mutex_held(1)) );
  assert( eMutex!=0 || (!mutex_held(0)) );
  pthread_mutex_lock(&primitives.aMutex[eMutex]);
  TESTONLY( primitives.aHolder[eMutex] = pthread_self(); )
}
static void async_mutex_leave(int eMutex){
  assert( eMutex==0 || eMutex==1 || eMutex==2 );
  assert( mutex_held(eMutex) );
  TESTONLY( primitives.aHolder[eMutex] = 0; )
  pthread_mutex_unlock(&primitives.aMutex[eMutex]);
}
static void async_cond_wait(int eCond, int eMutex){
  assert( eMutex==0 || eMutex==1 || eMutex==2 );
  assert( mutex_held(eMutex) );
  TESTONLY( primitives.aHolder[eMutex] = 0; )
  pthread_cond_wait(&primitives.aCond[eCond], &primitives.aMutex[eMutex]);
  TESTONLY( primitives.aHolder[eMutex] = pthread_self(); )
}
static void async_cond_signal(int eCond){
  assert( mutex_held(ASYNC_MUTEX_QUEUE) );
  pthread_cond_signal(&primitives.aCond[eCond]);
}
static void async_sched_yield(void){
  sched_yield();
}
#endif

#define assert_mutex_is_held(X) assert( mutex_held(X) )


#ifndef SQLITE_ASYNC_TWO_FILEHANDLES
#define SQLITE_ASYNC_TWO_FILEHANDLES 1
#endif

static struct TestAsyncStaticData {
  AsyncWrite *pQueueFirst;     
  AsyncWrite *pQueueLast;      
  AsyncLock *pLock;            
  volatile int ioDelay;        
  volatile int eHalt;          
  volatile int bLockFiles;     
  int ioError;                 
  int nFile;                   
} async = { 0,0,0,0,0,1,0,0 };

#define ASYNC_NOOP          0
#define ASYNC_WRITE         1
#define ASYNC_SYNC          2
#define ASYNC_TRUNCATE      3
#define ASYNC_CLOSE         4
#define ASYNC_DELETE        5
#define ASYNC_OPENEXCLUSIVE 6
#define ASYNC_UNLOCK        7

static const char *azOpcodeName[] = {
  "NOOP", "WRITE", "SYNC", "TRUNCATE", "CLOSE", "DELETE", "OPENEX", "UNLOCK"
};

struct AsyncWrite {
  AsyncFileData *pFileData;    
  int op;                      
  sqlite_int64 iOffset;        
  int nByte;          
  char *zBuf;         
  AsyncWrite *pNext;  
};

struct AsyncLock {
  char *zFile;
  int nFile;
  sqlite3_file *pFile;
  int eLock;
  AsyncFileLock *pList;
  AsyncLock *pNext;           
};

struct AsyncFileLock {
  int eLock;                
  int eAsyncLock;           
  AsyncFileLock *pNext;
};

struct AsyncFile {
  sqlite3_io_methods *pMethod;
  AsyncFileData *pData;
};
struct AsyncFileData {
  char *zName;               
  int nName;                 
  sqlite3_file *pBaseRead;   
  sqlite3_file *pBaseWrite;  
  AsyncFileLock lock;        
  AsyncLock *pLock;          
  AsyncWrite closeOp;        
};

static void addAsyncWrite(AsyncWrite *pWrite){
  
  if( pWrite->op!=ASYNC_UNLOCK ){
    async_mutex_enter(ASYNC_MUTEX_QUEUE);
  }

  
  assert( !pWrite->pNext );
  if( async.pQueueLast ){
    assert( async.pQueueFirst );
    async.pQueueLast->pNext = pWrite;
  }else{
    async.pQueueFirst = pWrite;
  }
  async.pQueueLast = pWrite;
  ASYNC_TRACE(("PUSH %p (%s %s %d)\n", pWrite, azOpcodeName[pWrite->op],
         pWrite->pFileData ? pWrite->pFileData->zName : "-", pWrite->iOffset));

  if( pWrite->op==ASYNC_CLOSE ){
    async.nFile--;
  }

  async_cond_signal(ASYNC_COND_QUEUE);

  
  if( pWrite->op!=ASYNC_UNLOCK ){
    async_mutex_leave(ASYNC_MUTEX_QUEUE);
  }
}

static void incrOpenFileCount(void){
  
  async_mutex_enter(ASYNC_MUTEX_QUEUE);
  if( async.nFile==0 ){
    async.ioError = SQLITE_OK;
  }
  async.nFile++;
  async_mutex_leave(ASYNC_MUTEX_QUEUE);
}

static int addNewAsyncWrite(
  AsyncFileData *pFileData, 
  int op, 
  sqlite3_int64 iOffset, 
  int nByte,
  const char *zByte
){
  AsyncWrite *p;
  if( op!=ASYNC_CLOSE && async.ioError ){
    return async.ioError;
  }
  p = sqlite3_malloc(sizeof(AsyncWrite) + (zByte?nByte:0));
  if( !p ){
    return SQLITE_IOERR;
  }
  p->op = op;
  p->iOffset = iOffset;
  p->nByte = nByte;
  p->pFileData = pFileData;
  p->pNext = 0;
  if( zByte ){
    p->zBuf = (char *)&p[1];
    memcpy(p->zBuf, zByte, nByte);
  }else{
    p->zBuf = 0;
  }
  addAsyncWrite(p);
  return SQLITE_OK;
}

static int asyncClose(sqlite3_file *pFile){
  AsyncFileData *p = ((AsyncFile *)pFile)->pData;

  
  async_mutex_enter(ASYNC_MUTEX_LOCK);
  p->lock.eLock = 0;
  async_mutex_leave(ASYNC_MUTEX_LOCK);

  addAsyncWrite(&p->closeOp);
  return SQLITE_OK;
}

static int asyncWrite(
  sqlite3_file *pFile, 
  const void *pBuf, 
  int amt, 
  sqlite3_int64 iOff
){
  AsyncFileData *p = ((AsyncFile *)pFile)->pData;
  return addNewAsyncWrite(p, ASYNC_WRITE, iOff, amt, pBuf);
}

static int asyncRead(
  sqlite3_file *pFile, 
  void *zOut, 
  int iAmt, 
  sqlite3_int64 iOffset
){
  AsyncFileData *p = ((AsyncFile *)pFile)->pData;
  int rc = SQLITE_OK;
  sqlite3_int64 filesize = 0;
  sqlite3_file *pBase = p->pBaseRead;
  sqlite3_int64 iAmt64 = (sqlite3_int64)iAmt;

  
  async_mutex_enter(ASYNC_MUTEX_QUEUE);

  if( async.ioError!=SQLITE_OK ){
    rc = async.ioError;
    goto asyncread_out;
  }

  if( pBase->pMethods ){
    sqlite3_int64 nRead;
    rc = pBase->pMethods->xFileSize(pBase, &filesize);
    if( rc!=SQLITE_OK ){
      goto asyncread_out;
    }
    nRead = MIN(filesize - iOffset, iAmt64);
    if( nRead>0 ){
      rc = pBase->pMethods->xRead(pBase, zOut, (int)nRead, iOffset);
      ASYNC_TRACE(("READ %s %d bytes at %d\n", p->zName, nRead, iOffset));
    }
  }

  if( rc==SQLITE_OK ){
    AsyncWrite *pWrite;
    char *zName = p->zName;

    for(pWrite=async.pQueueFirst; pWrite; pWrite = pWrite->pNext){
      if( pWrite->op==ASYNC_WRITE && (
        (pWrite->pFileData==p) ||
        (zName && pWrite->pFileData->zName==zName)
      )){
        sqlite3_int64 nCopy;
        sqlite3_int64 nByte64 = (sqlite3_int64)pWrite->nByte;

        sqlite3_int64 iBeginOut = (pWrite->iOffset-iOffset);
        sqlite3_int64 iBeginIn = -iBeginOut;
        if( iBeginIn<0 ) iBeginIn = 0;
        if( iBeginOut<0 ) iBeginOut = 0;

        filesize = MAX(filesize, pWrite->iOffset+nByte64);

        nCopy = MIN(nByte64-iBeginIn, iAmt64-iBeginOut);
        if( nCopy>0 ){
          memcpy(&((char *)zOut)[iBeginOut], &pWrite->zBuf[iBeginIn], (size_t)nCopy);
          ASYNC_TRACE(("OVERREAD %d bytes at %d\n", nCopy, iBeginOut+iOffset));
        }
      }
    }
  }

asyncread_out:
  async_mutex_leave(ASYNC_MUTEX_QUEUE);
  if( rc==SQLITE_OK && filesize<(iOffset+iAmt) ){
    rc = SQLITE_IOERR_SHORT_READ;
  }
  return rc;
}

static int asyncTruncate(sqlite3_file *pFile, sqlite3_int64 nByte){
  AsyncFileData *p = ((AsyncFile *)pFile)->pData;
  return addNewAsyncWrite(p, ASYNC_TRUNCATE, nByte, 0, 0);
}

static int asyncSync(sqlite3_file *pFile, int flags){
  AsyncFileData *p = ((AsyncFile *)pFile)->pData;
  return addNewAsyncWrite(p, ASYNC_SYNC, 0, flags, 0);
}

int asyncFileSize(sqlite3_file *pFile, sqlite3_int64 *piSize){
  AsyncFileData *p = ((AsyncFile *)pFile)->pData;
  int rc = SQLITE_OK;
  sqlite3_int64 s = 0;
  sqlite3_file *pBase;

  async_mutex_enter(ASYNC_MUTEX_QUEUE);

  pBase = p->pBaseRead;
  if( pBase->pMethods ){
    rc = pBase->pMethods->xFileSize(pBase, &s);
  }

  if( rc==SQLITE_OK ){
    AsyncWrite *pWrite;
    for(pWrite=async.pQueueFirst; pWrite; pWrite = pWrite->pNext){
      if( pWrite->op==ASYNC_DELETE 
       && p->zName 
       && strcmp(p->zName, pWrite->zBuf)==0 
      ){
        s = 0;
      }else if( pWrite->pFileData && (
          (pWrite->pFileData==p) 
       || (p->zName && pWrite->pFileData->zName==p->zName) 
      )){
        switch( pWrite->op ){
          case ASYNC_WRITE:
            s = MAX(pWrite->iOffset + (sqlite3_int64)(pWrite->nByte), s);
            break;
          case ASYNC_TRUNCATE:
            s = MIN(s, pWrite->iOffset);
            break;
        }
      }
    }
    *piSize = s;
  }
  async_mutex_leave(ASYNC_MUTEX_QUEUE);
  return rc;
}

static int getFileLock(AsyncLock *pLock){
  int rc = SQLITE_OK;
  AsyncFileLock *pIter;
  int eRequired = 0;

  if( pLock->pFile ){
    for(pIter=pLock->pList; pIter; pIter=pIter->pNext){
      assert(pIter->eAsyncLock>=pIter->eLock);
      if( pIter->eAsyncLock>eRequired ){
        eRequired = pIter->eAsyncLock;
        assert(eRequired>=0 && eRequired<=SQLITE_LOCK_EXCLUSIVE);
      }
    }

    if( eRequired>pLock->eLock ){
      rc = pLock->pFile->pMethods->xLock(pLock->pFile, eRequired);
      if( rc==SQLITE_OK ){
        pLock->eLock = eRequired;
      }
    }
    else if( eRequired<pLock->eLock && eRequired<=SQLITE_LOCK_SHARED ){
      rc = pLock->pFile->pMethods->xUnlock(pLock->pFile, eRequired);
      if( rc==SQLITE_OK ){
        pLock->eLock = eRequired;
      }
    }
  }

  return rc;
}

static AsyncLock *findLock(const char *zName, int nName){
  AsyncLock *p = async.pLock;
  while( p && (p->nFile!=nName || memcmp(p->zFile, zName, nName)) ){
    p = p->pNext;
  }
  return p;
}

static int asyncLock(sqlite3_file *pFile, int eLock){
  int rc = SQLITE_OK;
  AsyncFileData *p = ((AsyncFile *)pFile)->pData;

  if( p->zName ){
    async_mutex_enter(ASYNC_MUTEX_LOCK);
    if( p->lock.eLock<eLock ){
      AsyncLock *pLock = p->pLock;
      AsyncFileLock *pIter;
      assert(pLock && pLock->pList);
      for(pIter=pLock->pList; pIter; pIter=pIter->pNext){
        if( pIter!=&p->lock && (
          (eLock==SQLITE_LOCK_EXCLUSIVE && pIter->eLock>=SQLITE_LOCK_SHARED) ||
          (eLock==SQLITE_LOCK_PENDING && pIter->eLock>=SQLITE_LOCK_RESERVED) ||
          (eLock==SQLITE_LOCK_RESERVED && pIter->eLock>=SQLITE_LOCK_RESERVED) ||
          (eLock==SQLITE_LOCK_SHARED && pIter->eLock>=SQLITE_LOCK_PENDING)
        )){
          rc = SQLITE_BUSY;
        }
      }
      if( rc==SQLITE_OK ){
        p->lock.eLock = eLock;
        p->lock.eAsyncLock = MAX(p->lock.eAsyncLock, eLock);
      }
      assert(p->lock.eAsyncLock>=p->lock.eLock);
      if( rc==SQLITE_OK ){
        rc = getFileLock(pLock);
      }
    }
    async_mutex_leave(ASYNC_MUTEX_LOCK);
  }

  ASYNC_TRACE(("LOCK %d (%s) rc=%d\n", eLock, p->zName, rc));
  return rc;
}
static int asyncUnlock(sqlite3_file *pFile, int eLock){
  int rc = SQLITE_OK;
  AsyncFileData *p = ((AsyncFile *)pFile)->pData;
  if( p->zName ){
    AsyncFileLock *pLock = &p->lock;
    async_mutex_enter(ASYNC_MUTEX_QUEUE);
    async_mutex_enter(ASYNC_MUTEX_LOCK);
    pLock->eLock = MIN(pLock->eLock, eLock);
    rc = addNewAsyncWrite(p, ASYNC_UNLOCK, 0, eLock, 0);
    async_mutex_leave(ASYNC_MUTEX_LOCK);
    async_mutex_leave(ASYNC_MUTEX_QUEUE);
  }
  return rc;
}

static int asyncCheckReservedLock(sqlite3_file *pFile, int *pResOut){
  int ret = 0;
  AsyncFileLock *pIter;
  AsyncFileData *p = ((AsyncFile *)pFile)->pData;

  async_mutex_enter(ASYNC_MUTEX_LOCK);
  for(pIter=p->pLock->pList; pIter; pIter=pIter->pNext){
    if( pIter->eLock>=SQLITE_LOCK_RESERVED ){
      ret = 1;
      break;
    }
  }
  async_mutex_leave(ASYNC_MUTEX_LOCK);

  ASYNC_TRACE(("CHECK-LOCK %d (%s)\n", ret, p->zName));
  *pResOut = ret;
  return SQLITE_OK;
}

static int asyncFileControl(sqlite3_file *id, int op, void *pArg){
  switch( op ){
    case SQLITE_FCNTL_LOCKSTATE: {
      async_mutex_enter(ASYNC_MUTEX_LOCK);
      *(int*)pArg = ((AsyncFile*)id)->pData->lock.eLock;
      async_mutex_leave(ASYNC_MUTEX_LOCK);
      return SQLITE_OK;
    }
  }
  return SQLITE_ERROR;
}

static int asyncSectorSize(sqlite3_file *pFile){
  UNUSED_PARAMETER(pFile);
  return 512;
}
static int asyncDeviceCharacteristics(sqlite3_file *pFile){
  UNUSED_PARAMETER(pFile);
  return 0;
}

static int unlinkAsyncFile(AsyncFileData *pData){
  AsyncFileLock **ppIter;
  int rc = SQLITE_OK;

  if( pData->zName ){
    AsyncLock *pLock = pData->pLock;
    for(ppIter=&pLock->pList; *ppIter; ppIter=&((*ppIter)->pNext)){
      if( (*ppIter)==&pData->lock ){
        *ppIter = pData->lock.pNext;
        break;
      }
    }
    if( !pLock->pList ){
      AsyncLock **pp;
      if( pLock->pFile ){
        pLock->pFile->pMethods->xClose(pLock->pFile);
      }
      for(pp=&async.pLock; *pp!=pLock; pp=&((*pp)->pNext));
      *pp = pLock->pNext;
      sqlite3_free(pLock);
    }else{
      rc = getFileLock(pLock);
    }
  }

  return rc;
}

static int doAsynchronousOpen(int flags){
  return (flags&SQLITE_OPEN_CREATE) && (
      (flags&SQLITE_OPEN_MAIN_JOURNAL) ||
      (flags&SQLITE_OPEN_TEMP_JOURNAL) ||
      (flags&SQLITE_OPEN_DELETEONCLOSE)
  );
}

static int asyncOpen(
  sqlite3_vfs *pAsyncVfs,
  const char *zName,
  sqlite3_file *pFile,
  int flags,
  int *pOutFlags
){
  static sqlite3_io_methods async_methods = {
    1,                               
    asyncClose,                      
    asyncRead,                       
    asyncWrite,                      
    asyncTruncate,                   
    asyncSync,                       
    asyncFileSize,                   
    asyncLock,                       
    asyncUnlock,                     
    asyncCheckReservedLock,          
    asyncFileControl,                
    asyncSectorSize,                 
    asyncDeviceCharacteristics       
  };

  sqlite3_vfs *pVfs = (sqlite3_vfs *)pAsyncVfs->pAppData;
  AsyncFile *p = (AsyncFile *)pFile;
  int nName = 0;
  int rc = SQLITE_OK;
  int nByte;
  AsyncFileData *pData;
  AsyncLock *pLock = 0;
  char *z;
  int isAsyncOpen = doAsynchronousOpen(flags);

  
  if( zName ){
    nName = (int)strlen(zName)+1;
  }

  nByte = (
    sizeof(AsyncFileData) +        
    2 * pVfs->szOsFile +           
    nName                          
  ); 
  z = sqlite3_malloc(nByte);
  if( !z ){
    return SQLITE_NOMEM;
  }
  memset(z, 0, nByte);
  pData = (AsyncFileData*)z;
  z += sizeof(pData[0]);
  pData->pBaseRead = (sqlite3_file*)z;
  z += pVfs->szOsFile;
  pData->pBaseWrite = (sqlite3_file*)z;
  pData->closeOp.pFileData = pData;
  pData->closeOp.op = ASYNC_CLOSE;

  if( zName ){
    z += pVfs->szOsFile;
    pData->zName = z;
    pData->nName = nName;
    memcpy(pData->zName, zName, nName);
  }

  if( !isAsyncOpen ){
    int flagsout;
    rc = pVfs->xOpen(pVfs, pData->zName, pData->pBaseRead, flags, &flagsout);
    if( rc==SQLITE_OK 
     && (flagsout&SQLITE_OPEN_READWRITE) 
     && (flags&SQLITE_OPEN_EXCLUSIVE)==0
    ){
      rc = pVfs->xOpen(pVfs, pData->zName, pData->pBaseWrite, flags, 0);
    }
    if( pOutFlags ){
      *pOutFlags = flagsout;
    }
  }

  async_mutex_enter(ASYNC_MUTEX_LOCK);

  if( zName && rc==SQLITE_OK ){
    pLock = findLock(pData->zName, pData->nName);
    if( !pLock ){
      int nByte = pVfs->szOsFile + sizeof(AsyncLock) + pData->nName + 1; 
      pLock = (AsyncLock *)sqlite3_malloc(nByte);
      if( pLock ){
        memset(pLock, 0, nByte);
        if( async.bLockFiles && (flags&SQLITE_OPEN_MAIN_DB) ){
          pLock->pFile = (sqlite3_file *)&pLock[1];
          rc = pVfs->xOpen(pVfs, pData->zName, pLock->pFile, flags, 0);
          if( rc!=SQLITE_OK ){
            sqlite3_free(pLock);
            pLock = 0;
          }
        }
        if( pLock ){
          pLock->nFile = pData->nName;
          pLock->zFile = &((char *)(&pLock[1]))[pVfs->szOsFile];
          memcpy(pLock->zFile, pData->zName, pLock->nFile);
          pLock->pNext = async.pLock;
          async.pLock = pLock;
        }
      }else{
        rc = SQLITE_NOMEM;
      }
    }
  }

  if( rc==SQLITE_OK ){
    p->pMethod = &async_methods;
    p->pData = pData;

    if( zName ){
      pData->lock.pNext = pLock->pList;
      pLock->pList = &pData->lock;
      pData->zName = pLock->zFile;
    }
  }else{
    if( pData->pBaseRead->pMethods ){
      pData->pBaseRead->pMethods->xClose(pData->pBaseRead);
    }
    if( pData->pBaseWrite->pMethods ){
      pData->pBaseWrite->pMethods->xClose(pData->pBaseWrite);
    }
    sqlite3_free(pData);
  }

  async_mutex_leave(ASYNC_MUTEX_LOCK);

  if( rc==SQLITE_OK ){
    pData->pLock = pLock;
  }

  if( rc==SQLITE_OK && isAsyncOpen ){
    rc = addNewAsyncWrite(pData, ASYNC_OPENEXCLUSIVE, (sqlite3_int64)flags,0,0);
    if( rc==SQLITE_OK ){
      if( pOutFlags ) *pOutFlags = flags;
    }else{
      async_mutex_enter(ASYNC_MUTEX_LOCK);
      unlinkAsyncFile(pData);
      async_mutex_leave(ASYNC_MUTEX_LOCK);
      sqlite3_free(pData);
    }
  }
  if( rc!=SQLITE_OK ){
    p->pMethod = 0;
  }else{
    incrOpenFileCount();
  }

  return rc;
}

static int asyncDelete(sqlite3_vfs *pAsyncVfs, const char *z, int syncDir){
  UNUSED_PARAMETER(pAsyncVfs);
  return addNewAsyncWrite(0, ASYNC_DELETE, syncDir, (int)strlen(z)+1, z);
}

static int asyncAccess(
  sqlite3_vfs *pAsyncVfs, 
  const char *zName, 
  int flags,
  int *pResOut
){
  int rc;
  int ret;
  AsyncWrite *p;
  sqlite3_vfs *pVfs = (sqlite3_vfs *)pAsyncVfs->pAppData;

  assert(flags==SQLITE_ACCESS_READWRITE 
      || flags==SQLITE_ACCESS_READ 
      || flags==SQLITE_ACCESS_EXISTS 
  );

  async_mutex_enter(ASYNC_MUTEX_QUEUE);
  rc = pVfs->xAccess(pVfs, zName, flags, &ret);
  if( rc==SQLITE_OK && flags==SQLITE_ACCESS_EXISTS ){
    for(p=async.pQueueFirst; p; p = p->pNext){
      if( p->op==ASYNC_DELETE && 0==strcmp(p->zBuf, zName) ){
        ret = 0;
      }else if( p->op==ASYNC_OPENEXCLUSIVE 
             && p->pFileData->zName
             && 0==strcmp(p->pFileData->zName, zName) 
      ){
        ret = 1;
      }
    }
  }
  ASYNC_TRACE(("ACCESS(%s): %s = %d\n", 
    flags==SQLITE_ACCESS_READWRITE?"read-write":
    flags==SQLITE_ACCESS_READ?"read":"exists"
    , zName, ret)
  );
  async_mutex_leave(ASYNC_MUTEX_QUEUE);
  *pResOut = ret;
  return rc;
}

static int asyncFullPathname(
  sqlite3_vfs *pAsyncVfs, 
  const char *zPath, 
  int nPathOut,
  char *zPathOut
){
  int rc;
  sqlite3_vfs *pVfs = (sqlite3_vfs *)pAsyncVfs->pAppData;
  rc = pVfs->xFullPathname(pVfs, zPath, nPathOut, zPathOut);

  if( rc==SQLITE_OK ){
    int i, j;
    char *z = zPathOut;
    int n = (int)strlen(z);
    while( n>1 && z[n-1]=='/' ){ n--; }
    for(i=j=0; i<n; i++){
      if( z[i]=='/' ){
        if( z[i+1]=='/' ) continue;
        if( z[i+1]=='.' && i+2<n && z[i+2]=='/' ){
          i += 1;
          continue;
        }
        if( z[i+1]=='.' && i+3<n && z[i+2]=='.' && z[i+3]=='/' ){
          while( j>0 && z[j-1]!='/' ){ j--; }
          if( j>0 ){ j--; }
          i += 2;
          continue;
        }
      }
      z[j++] = z[i];
    }
    z[j] = 0;
  }

  return rc;
}
static void *asyncDlOpen(sqlite3_vfs *pAsyncVfs, const char *zPath){
  sqlite3_vfs *pVfs = (sqlite3_vfs *)pAsyncVfs->pAppData;
  return pVfs->xDlOpen(pVfs, zPath);
}
static void asyncDlError(sqlite3_vfs *pAsyncVfs, int nByte, char *zErrMsg){
  sqlite3_vfs *pVfs = (sqlite3_vfs *)pAsyncVfs->pAppData;
  pVfs->xDlError(pVfs, nByte, zErrMsg);
}
static void (*asyncDlSym(
  sqlite3_vfs *pAsyncVfs, 
  void *pHandle, 
  const char *zSymbol
))(void){
  sqlite3_vfs *pVfs = (sqlite3_vfs *)pAsyncVfs->pAppData;
  return pVfs->xDlSym(pVfs, pHandle, zSymbol);
}
static void asyncDlClose(sqlite3_vfs *pAsyncVfs, void *pHandle){
  sqlite3_vfs *pVfs = (sqlite3_vfs *)pAsyncVfs->pAppData;
  pVfs->xDlClose(pVfs, pHandle);
}
static int asyncRandomness(sqlite3_vfs *pAsyncVfs, int nByte, char *zBufOut){
  sqlite3_vfs *pVfs = (sqlite3_vfs *)pAsyncVfs->pAppData;
  return pVfs->xRandomness(pVfs, nByte, zBufOut);
}
static int asyncSleep(sqlite3_vfs *pAsyncVfs, int nMicro){
  sqlite3_vfs *pVfs = (sqlite3_vfs *)pAsyncVfs->pAppData;
  return pVfs->xSleep(pVfs, nMicro);
}
static int asyncCurrentTime(sqlite3_vfs *pAsyncVfs, double *pTimeOut){
  sqlite3_vfs *pVfs = (sqlite3_vfs *)pAsyncVfs->pAppData;
  return pVfs->xCurrentTime(pVfs, pTimeOut);
}

static sqlite3_vfs async_vfs = {
  1,                    
  sizeof(AsyncFile),    
  0,                    
  0,                    
  SQLITEASYNC_VFSNAME,  
  0,                    
  asyncOpen,            
  asyncDelete,          
  asyncAccess,          
  asyncFullPathname,    
  asyncDlOpen,          
  asyncDlError,         
  asyncDlSym,           
  asyncDlClose,         
  asyncRandomness,      
  asyncSleep,           
  asyncCurrentTime      
};

static void asyncWriterThread(void){
  sqlite3_vfs *pVfs = (sqlite3_vfs *)(async_vfs.pAppData);
  AsyncWrite *p = 0;
  int rc = SQLITE_OK;
  int holdingMutex = 0;

  async_mutex_enter(ASYNC_MUTEX_WRITER);

  while( async.eHalt!=SQLITEASYNC_HALT_NOW ){
    int doNotFree = 0;
    sqlite3_file *pBase = 0;

    if( !holdingMutex ){
      async_mutex_enter(ASYNC_MUTEX_QUEUE);
    }
    while( (p = async.pQueueFirst)==0 ){
      if( async.eHalt!=SQLITEASYNC_HALT_NEVER ){
        async_mutex_leave(ASYNC_MUTEX_QUEUE);
        break;
      }else{
        ASYNC_TRACE(("IDLE\n"));
        async_cond_wait(ASYNC_COND_QUEUE, ASYNC_MUTEX_QUEUE);
        ASYNC_TRACE(("WAKEUP\n"));
      }
    }
    if( p==0 ) break;
    holdingMutex = 1;

    if( async.ioError!=SQLITE_OK && p->op!=ASYNC_CLOSE ){
      p->op = ASYNC_NOOP;
    }
    if( p->pFileData ){
      pBase = p->pFileData->pBaseWrite;
      if( 
        p->op==ASYNC_CLOSE || 
        p->op==ASYNC_OPENEXCLUSIVE ||
        (pBase->pMethods && (p->op==ASYNC_SYNC || p->op==ASYNC_WRITE) ) 
      ){
        async_mutex_leave(ASYNC_MUTEX_QUEUE);
        holdingMutex = 0;
      }
      if( !pBase->pMethods ){
        pBase = p->pFileData->pBaseRead;
      }
    }

    switch( p->op ){
      case ASYNC_NOOP:
        break;

      case ASYNC_WRITE:
        assert( pBase );
        ASYNC_TRACE(("WRITE %s %d bytes at %d\n",
                p->pFileData->zName, p->nByte, p->iOffset));
        rc = pBase->pMethods->xWrite(pBase, (void *)(p->zBuf), p->nByte, p->iOffset);
        break;

      case ASYNC_SYNC:
        assert( pBase );
        ASYNC_TRACE(("SYNC %s\n", p->pFileData->zName));
        rc = pBase->pMethods->xSync(pBase, p->nByte);
        break;

      case ASYNC_TRUNCATE:
        assert( pBase );
        ASYNC_TRACE(("TRUNCATE %s to %d bytes\n", 
                p->pFileData->zName, p->iOffset));
        rc = pBase->pMethods->xTruncate(pBase, p->iOffset);
        break;

      case ASYNC_CLOSE: {
        AsyncFileData *pData = p->pFileData;
        ASYNC_TRACE(("CLOSE %s\n", p->pFileData->zName));
        if( pData->pBaseWrite->pMethods ){
          pData->pBaseWrite->pMethods->xClose(pData->pBaseWrite);
        }
        if( pData->pBaseRead->pMethods ){
          pData->pBaseRead->pMethods->xClose(pData->pBaseRead);
        }

        async_mutex_enter(ASYNC_MUTEX_LOCK);
        rc = unlinkAsyncFile(pData);
        async_mutex_leave(ASYNC_MUTEX_LOCK);

        if( !holdingMutex ){
          async_mutex_enter(ASYNC_MUTEX_QUEUE);
          holdingMutex = 1;
        }
        assert_mutex_is_held(ASYNC_MUTEX_QUEUE);
        async.pQueueFirst = p->pNext;
        sqlite3_free(pData);
        doNotFree = 1;
        break;
      }

      case ASYNC_UNLOCK: {
        AsyncWrite *pIter;
        AsyncFileData *pData = p->pFileData;
        int eLock = p->nByte;

 
        assert( holdingMutex==1 );
        assert( async.pQueueFirst==p );
        for(pIter=async.pQueueFirst->pNext; pIter; pIter=pIter->pNext){
          if( pIter->pFileData==pData && pIter->op==ASYNC_UNLOCK ) break;
        }
        if( !pIter ){
          async_mutex_enter(ASYNC_MUTEX_LOCK);
          pData->lock.eAsyncLock = MIN(
              pData->lock.eAsyncLock, MAX(pData->lock.eLock, eLock)
          );
          assert(pData->lock.eAsyncLock>=pData->lock.eLock);
          rc = getFileLock(pData->pLock);
          async_mutex_leave(ASYNC_MUTEX_LOCK);
        }
        break;
      }

      case ASYNC_DELETE:
        ASYNC_TRACE(("DELETE %s\n", p->zBuf));
        rc = pVfs->xDelete(pVfs, p->zBuf, (int)p->iOffset);
        break;

      case ASYNC_OPENEXCLUSIVE: {
        int flags = (int)p->iOffset;
        AsyncFileData *pData = p->pFileData;
        ASYNC_TRACE(("OPEN %s flags=%d\n", p->zBuf, (int)p->iOffset));
        assert(pData->pBaseRead->pMethods==0 && pData->pBaseWrite->pMethods==0);
        rc = pVfs->xOpen(pVfs, pData->zName, pData->pBaseRead, flags, 0);
        assert( holdingMutex==0 );
        async_mutex_enter(ASYNC_MUTEX_QUEUE);
        holdingMutex = 1;
        break;
      }

      default: assert(!"Illegal value for AsyncWrite.op");
    }

    if( !holdingMutex ){
      async_mutex_enter(ASYNC_MUTEX_QUEUE);
      holdingMutex = 1;
    }
    
    if( p==async.pQueueLast ){
      async.pQueueLast = 0;
    }
    if( !doNotFree ){
      assert_mutex_is_held(ASYNC_MUTEX_QUEUE);
      async.pQueueFirst = p->pNext;
      sqlite3_free(p);
    }
    assert( holdingMutex );

    if( rc!=SQLITE_OK ){
      async.ioError = rc;
    }

    if( async.ioError && !async.pQueueFirst ){
      async_mutex_enter(ASYNC_MUTEX_LOCK);
      if( 0==async.pLock ){
        async.ioError = SQLITE_OK;
      }
      async_mutex_leave(ASYNC_MUTEX_LOCK);
    }

    if( !async.pQueueFirst || !async.ioError ){
      async_mutex_leave(ASYNC_MUTEX_QUEUE);
      holdingMutex = 0;
      if( async.ioDelay>0 ){
        pVfs->xSleep(pVfs, async.ioDelay*1000);
      }else{
        async_sched_yield();
      }
    }
  }
  
  async_mutex_leave(ASYNC_MUTEX_WRITER);
  return;
}

 
int sqlite3async_initialize(const char *zParent, int isDefault){
  int rc = SQLITE_OK;
  if( async_vfs.pAppData==0 ){
    sqlite3_vfs *pParent = sqlite3_vfs_find(zParent);
    if( !pParent || async_os_initialize() ){
      rc = SQLITE_ERROR;
    }else if( SQLITE_OK!=(rc = sqlite3_vfs_register(&async_vfs, isDefault)) ){
      async_os_shutdown();
    }else{
      async_vfs.pAppData = (void *)pParent;
      async_vfs.mxPathname = ((sqlite3_vfs *)async_vfs.pAppData)->mxPathname;
    }
  }
  return rc;
}

void sqlite3async_shutdown(void){
  if( async_vfs.pAppData ){
    async_os_shutdown();
    sqlite3_vfs_unregister((sqlite3_vfs *)&async_vfs);
    async_vfs.pAppData = 0;
  }
}

void sqlite3async_run(void){
  asyncWriterThread();
}

int sqlite3async_control(int op, ...){
  va_list ap;
  va_start(ap, op);
  switch( op ){
    case SQLITEASYNC_HALT: {
      int eWhen = va_arg(ap, int);
      if( eWhen!=SQLITEASYNC_HALT_NEVER
       && eWhen!=SQLITEASYNC_HALT_NOW
       && eWhen!=SQLITEASYNC_HALT_IDLE
      ){
        return SQLITE_MISUSE;
      }
      async.eHalt = eWhen;
      async_mutex_enter(ASYNC_MUTEX_QUEUE);
      async_cond_signal(ASYNC_COND_QUEUE);
      async_mutex_leave(ASYNC_MUTEX_QUEUE);
      break;
    }

    case SQLITEASYNC_DELAY: {
      int iDelay = va_arg(ap, int);
      if( iDelay<0 ){
        return SQLITE_MISUSE;
      }
      async.ioDelay = iDelay;
      break;
    }

    case SQLITEASYNC_LOCKFILES: {
      int bLock = va_arg(ap, int);
      async_mutex_enter(ASYNC_MUTEX_QUEUE);
      if( async.nFile || async.pQueueFirst ){
        async_mutex_leave(ASYNC_MUTEX_QUEUE);
        return SQLITE_MISUSE;
      }
      async.bLockFiles = bLock;
      async_mutex_leave(ASYNC_MUTEX_QUEUE);
      break;
    }
      
    case SQLITEASYNC_GET_HALT: {
      int *peWhen = va_arg(ap, int *);
      *peWhen = async.eHalt;
      break;
    }
    case SQLITEASYNC_GET_DELAY: {
      int *piDelay = va_arg(ap, int *);
      *piDelay = async.ioDelay;
      break;
    }
    case SQLITEASYNC_GET_LOCKFILES: {
      int *piDelay = va_arg(ap, int *);
      *piDelay = async.bLockFiles;
      break;
    }

    default:
      return SQLITE_ERROR;
  }
  return SQLITE_OK;
}

#endif 


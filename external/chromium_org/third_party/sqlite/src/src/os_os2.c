/*
** 2006 Feb 14
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
******************************************************************************
**
** This file contains code that is specific to OS/2.
*/

#include "sqliteInt.h"

#if SQLITE_OS_OS2


#if defined(SQLITE_THREADSAFE) && SQLITE_THREADSAFE
# define SQLITE_OS2_THREADS 1
#endif

#include "os_common.h"

typedef struct os2File os2File;         
typedef struct os2ShmNode os2ShmNode;   
typedef struct os2ShmLink os2ShmLink;   

struct os2File {
  const sqlite3_io_methods *pMethod;  
  HFILE h;                  
  int flags;                
  int locktype;             
  int szChunk;              
  char *zFullPathCp;        
  os2ShmLink *pShmLink;     
};

#define LOCK_TIMEOUT 10L 

#ifndef OBJ_ANY
# define OBJ_ANY 0x00000400
#endif


static int os2Close( sqlite3_file *id ){
  APIRET rc;
  os2File *pFile = (os2File*)id;

  assert( id!=0 );
  OSTRACE(( "CLOSE %d (%s)\n", pFile->h, pFile->zFullPathCp ));

  rc = DosClose( pFile->h );

  if( pFile->flags & SQLITE_OPEN_DELETEONCLOSE )
    DosForceDelete( (PSZ)pFile->zFullPathCp );

  free( pFile->zFullPathCp );
  pFile->zFullPathCp = NULL;
  pFile->locktype = NO_LOCK;
  pFile->h = (HFILE)-1;
  pFile->flags = 0;

  OpenCounter( -1 );
  return rc == NO_ERROR ? SQLITE_OK : SQLITE_IOERR;
}

static int os2Read(
  sqlite3_file *id,               
  void *pBuf,                     
  int amt,                        
  sqlite3_int64 offset            
){
  ULONG fileLocation = 0L;
  ULONG got;
  os2File *pFile = (os2File*)id;
  assert( id!=0 );
  SimulateIOError( return SQLITE_IOERR_READ );
  OSTRACE(( "READ %d lock=%d\n", pFile->h, pFile->locktype ));
  if( DosSetFilePtr(pFile->h, offset, FILE_BEGIN, &fileLocation) != NO_ERROR ){
    return SQLITE_IOERR;
  }
  if( DosRead( pFile->h, pBuf, amt, &got ) != NO_ERROR ){
    return SQLITE_IOERR_READ;
  }
  if( got == (ULONG)amt )
    return SQLITE_OK;
  else {
    
    memset(&((char*)pBuf)[got], 0, amt-got);
    return SQLITE_IOERR_SHORT_READ;
  }
}

static int os2Write(
  sqlite3_file *id,               
  const void *pBuf,               /* The bytes to be written */
  int amt,                        
  sqlite3_int64 offset            
){
  ULONG fileLocation = 0L;
  APIRET rc = NO_ERROR;
  ULONG wrote;
  os2File *pFile = (os2File*)id;
  assert( id!=0 );
  SimulateIOError( return SQLITE_IOERR_WRITE );
  SimulateDiskfullError( return SQLITE_FULL );
  OSTRACE(( "WRITE %d lock=%d\n", pFile->h, pFile->locktype ));
  if( DosSetFilePtr(pFile->h, offset, FILE_BEGIN, &fileLocation) != NO_ERROR ){
    return SQLITE_IOERR;
  }
  assert( amt>0 );
  while( amt > 0 &&
         ( rc = DosWrite( pFile->h, (PVOID)pBuf, amt, &wrote ) ) == NO_ERROR &&
         wrote > 0
  ){
    amt -= wrote;
    pBuf = &((char*)pBuf)[wrote];
  }

  return ( rc != NO_ERROR || amt > (int)wrote ) ? SQLITE_FULL : SQLITE_OK;
}

static int os2Truncate( sqlite3_file *id, i64 nByte ){
  APIRET rc;
  os2File *pFile = (os2File*)id;
  assert( id!=0 );
  OSTRACE(( "TRUNCATE %d %lld\n", pFile->h, nByte ));
  SimulateIOError( return SQLITE_IOERR_TRUNCATE );

  if( pFile->szChunk ){
    nByte = ((nByte + pFile->szChunk - 1)/pFile->szChunk) * pFile->szChunk;
  }
  
  rc = DosSetFileSize( pFile->h, nByte );
  return rc == NO_ERROR ? SQLITE_OK : SQLITE_IOERR_TRUNCATE;
}

#ifdef SQLITE_TEST
int sqlite3_sync_count = 0;
int sqlite3_fullsync_count = 0;
#endif

static int os2Sync( sqlite3_file *id, int flags ){
  os2File *pFile = (os2File*)id;
  OSTRACE(( "SYNC %d lock=%d\n", pFile->h, pFile->locktype ));
#ifdef SQLITE_TEST
  if( flags & SQLITE_SYNC_FULL){
    sqlite3_fullsync_count++;
  }
  sqlite3_sync_count++;
#endif
#ifdef SQLITE_NO_SYNC
  UNUSED_PARAMETER(pFile);
  return SQLITE_OK;
#else
  return DosResetBuffer( pFile->h ) == NO_ERROR ? SQLITE_OK : SQLITE_IOERR;
#endif
}

static int os2FileSize( sqlite3_file *id, sqlite3_int64 *pSize ){
  APIRET rc = NO_ERROR;
  FILESTATUS3 fsts3FileInfo;
  memset(&fsts3FileInfo, 0, sizeof(fsts3FileInfo));
  assert( id!=0 );
  SimulateIOError( return SQLITE_IOERR_FSTAT );
  rc = DosQueryFileInfo( ((os2File*)id)->h, FIL_STANDARD, &fsts3FileInfo, sizeof(FILESTATUS3) );
  if( rc == NO_ERROR ){
    *pSize = fsts3FileInfo.cbFile;
    return SQLITE_OK;
  }else{
    return SQLITE_IOERR_FSTAT;
  }
}

static int getReadLock( os2File *pFile ){
  FILELOCK  LockArea,
            UnlockArea;
  APIRET res;
  memset(&LockArea, 0, sizeof(LockArea));
  memset(&UnlockArea, 0, sizeof(UnlockArea));
  LockArea.lOffset = SHARED_FIRST;
  LockArea.lRange = SHARED_SIZE;
  UnlockArea.lOffset = 0L;
  UnlockArea.lRange = 0L;
  res = DosSetFileLocks( pFile->h, &UnlockArea, &LockArea, LOCK_TIMEOUT, 1L );
  OSTRACE(( "GETREADLOCK %d res=%d\n", pFile->h, res ));
  return res;
}

static int unlockReadLock( os2File *id ){
  FILELOCK  LockArea,
            UnlockArea;
  APIRET res;
  memset(&LockArea, 0, sizeof(LockArea));
  memset(&UnlockArea, 0, sizeof(UnlockArea));
  LockArea.lOffset = 0L;
  LockArea.lRange = 0L;
  UnlockArea.lOffset = SHARED_FIRST;
  UnlockArea.lRange = SHARED_SIZE;
  res = DosSetFileLocks( id->h, &UnlockArea, &LockArea, LOCK_TIMEOUT, 1L );
  OSTRACE(( "UNLOCK-READLOCK file handle=%d res=%d?\n", id->h, res ));
  return res;
}

static int os2Lock( sqlite3_file *id, int locktype ){
  int rc = SQLITE_OK;       
  APIRET res = NO_ERROR;    
  int newLocktype;       
  int gotPendingLock = 0;
  FILELOCK  LockArea,
            UnlockArea;
  os2File *pFile = (os2File*)id;
  memset(&LockArea, 0, sizeof(LockArea));
  memset(&UnlockArea, 0, sizeof(UnlockArea));
  assert( pFile!=0 );
  OSTRACE(( "LOCK %d %d was %d\n", pFile->h, locktype, pFile->locktype ));

  if( pFile->locktype>=locktype ){
    OSTRACE(( "LOCK %d %d ok (already held)\n", pFile->h, locktype ));
    return SQLITE_OK;
  }

  assert( pFile->locktype!=NO_LOCK || locktype==SHARED_LOCK );
  assert( locktype!=PENDING_LOCK );
  assert( locktype!=RESERVED_LOCK || pFile->locktype==SHARED_LOCK );

  newLocktype = pFile->locktype;
  if( pFile->locktype==NO_LOCK
      || (locktype==EXCLUSIVE_LOCK && pFile->locktype==RESERVED_LOCK)
  ){
    LockArea.lOffset = PENDING_BYTE;
    LockArea.lRange = 1L;
    UnlockArea.lOffset = 0L;
    UnlockArea.lRange = 0L;

    
    res = DosSetFileLocks( pFile->h, &UnlockArea, &LockArea, 100L, 0L );
    if( res == NO_ERROR ){
      gotPendingLock = 1;
      OSTRACE(( "LOCK %d pending lock boolean set.  res=%d\n", pFile->h, res ));
    }
  }

  if( locktype==SHARED_LOCK && res == NO_ERROR ){
    assert( pFile->locktype==NO_LOCK );
    res = getReadLock(pFile);
    if( res == NO_ERROR ){
      newLocktype = SHARED_LOCK;
    }
    OSTRACE(( "LOCK %d acquire shared lock. res=%d\n", pFile->h, res ));
  }

  if( locktype==RESERVED_LOCK && res == NO_ERROR ){
    assert( pFile->locktype==SHARED_LOCK );
    LockArea.lOffset = RESERVED_BYTE;
    LockArea.lRange = 1L;
    UnlockArea.lOffset = 0L;
    UnlockArea.lRange = 0L;
    res = DosSetFileLocks( pFile->h, &UnlockArea, &LockArea, LOCK_TIMEOUT, 0L );
    if( res == NO_ERROR ){
      newLocktype = RESERVED_LOCK;
    }
    OSTRACE(( "LOCK %d acquire reserved lock. res=%d\n", pFile->h, res ));
  }

  if( locktype==EXCLUSIVE_LOCK && res == NO_ERROR ){
    newLocktype = PENDING_LOCK;
    gotPendingLock = 0;
    OSTRACE(( "LOCK %d acquire pending lock. pending lock boolean unset.\n",
               pFile->h ));
  }

  if( locktype==EXCLUSIVE_LOCK && res == NO_ERROR ){
    assert( pFile->locktype>=SHARED_LOCK );
    res = unlockReadLock(pFile);
    OSTRACE(( "unreadlock = %d\n", res ));
    LockArea.lOffset = SHARED_FIRST;
    LockArea.lRange = SHARED_SIZE;
    UnlockArea.lOffset = 0L;
    UnlockArea.lRange = 0L;
    res = DosSetFileLocks( pFile->h, &UnlockArea, &LockArea, LOCK_TIMEOUT, 0L );
    if( res == NO_ERROR ){
      newLocktype = EXCLUSIVE_LOCK;
    }else{
      OSTRACE(( "OS/2 error-code = %d\n", res ));
      getReadLock(pFile);
    }
    OSTRACE(( "LOCK %d acquire exclusive lock.  res=%d\n", pFile->h, res ));
  }

  if( gotPendingLock && locktype==SHARED_LOCK ){
    int r;
    LockArea.lOffset = 0L;
    LockArea.lRange = 0L;
    UnlockArea.lOffset = PENDING_BYTE;
    UnlockArea.lRange = 1L;
    r = DosSetFileLocks( pFile->h, &UnlockArea, &LockArea, LOCK_TIMEOUT, 0L );
    OSTRACE(( "LOCK %d unlocking pending/is shared. r=%d\n", pFile->h, r ));
  }

  if( res == NO_ERROR ){
    rc = SQLITE_OK;
  }else{
    OSTRACE(( "LOCK FAILED %d trying for %d but got %d\n", pFile->h,
              locktype, newLocktype ));
    rc = SQLITE_BUSY;
  }
  pFile->locktype = newLocktype;
  OSTRACE(( "LOCK %d now %d\n", pFile->h, pFile->locktype ));
  return rc;
}

static int os2CheckReservedLock( sqlite3_file *id, int *pOut ){
  int r = 0;
  os2File *pFile = (os2File*)id;
  assert( pFile!=0 );
  if( pFile->locktype>=RESERVED_LOCK ){
    r = 1;
    OSTRACE(( "TEST WR-LOCK %d %d (local)\n", pFile->h, r ));
  }else{
    FILELOCK  LockArea,
              UnlockArea;
    APIRET rc = NO_ERROR;
    memset(&LockArea, 0, sizeof(LockArea));
    memset(&UnlockArea, 0, sizeof(UnlockArea));
    LockArea.lOffset = RESERVED_BYTE;
    LockArea.lRange = 1L;
    UnlockArea.lOffset = 0L;
    UnlockArea.lRange = 0L;
    rc = DosSetFileLocks( pFile->h, &UnlockArea, &LockArea, LOCK_TIMEOUT, 0L );
    OSTRACE(( "TEST WR-LOCK %d lock reserved byte rc=%d\n", pFile->h, rc ));
    if( rc == NO_ERROR ){
      APIRET rcu = NO_ERROR; 
      LockArea.lOffset = 0L;
      LockArea.lRange = 0L;
      UnlockArea.lOffset = RESERVED_BYTE;
      UnlockArea.lRange = 1L;
      rcu = DosSetFileLocks( pFile->h, &UnlockArea, &LockArea, LOCK_TIMEOUT, 0L );
      OSTRACE(( "TEST WR-LOCK %d unlock reserved byte r=%d\n", pFile->h, rcu ));
    }
    r = !(rc == NO_ERROR);
    OSTRACE(( "TEST WR-LOCK %d %d (remote)\n", pFile->h, r ));
  }
  *pOut = r;
  return SQLITE_OK;
}

static int os2Unlock( sqlite3_file *id, int locktype ){
  int type;
  os2File *pFile = (os2File*)id;
  APIRET rc = SQLITE_OK;
  APIRET res = NO_ERROR;
  FILELOCK  LockArea,
            UnlockArea;
  memset(&LockArea, 0, sizeof(LockArea));
  memset(&UnlockArea, 0, sizeof(UnlockArea));
  assert( pFile!=0 );
  assert( locktype<=SHARED_LOCK );
  OSTRACE(( "UNLOCK %d to %d was %d\n", pFile->h, locktype, pFile->locktype ));
  type = pFile->locktype;
  if( type>=EXCLUSIVE_LOCK ){
    LockArea.lOffset = 0L;
    LockArea.lRange = 0L;
    UnlockArea.lOffset = SHARED_FIRST;
    UnlockArea.lRange = SHARED_SIZE;
    res = DosSetFileLocks( pFile->h, &UnlockArea, &LockArea, LOCK_TIMEOUT, 0L );
    OSTRACE(( "UNLOCK %d exclusive lock res=%d\n", pFile->h, res ));
    if( locktype==SHARED_LOCK && getReadLock(pFile) != NO_ERROR ){
      OSTRACE(( "UNLOCK %d to %d getReadLock() failed\n", pFile->h, locktype ));
      rc = SQLITE_IOERR_UNLOCK;
    }
  }
  if( type>=RESERVED_LOCK ){
    LockArea.lOffset = 0L;
    LockArea.lRange = 0L;
    UnlockArea.lOffset = RESERVED_BYTE;
    UnlockArea.lRange = 1L;
    res = DosSetFileLocks( pFile->h, &UnlockArea, &LockArea, LOCK_TIMEOUT, 0L );
    OSTRACE(( "UNLOCK %d reserved res=%d\n", pFile->h, res ));
  }
  if( locktype==NO_LOCK && type>=SHARED_LOCK ){
    res = unlockReadLock(pFile);
    OSTRACE(( "UNLOCK %d is %d want %d res=%d\n",
              pFile->h, type, locktype, res ));
  }
  if( type>=PENDING_LOCK ){
    LockArea.lOffset = 0L;
    LockArea.lRange = 0L;
    UnlockArea.lOffset = PENDING_BYTE;
    UnlockArea.lRange = 1L;
    res = DosSetFileLocks( pFile->h, &UnlockArea, &LockArea, LOCK_TIMEOUT, 0L );
    OSTRACE(( "UNLOCK %d pending res=%d\n", pFile->h, res ));
  }
  pFile->locktype = locktype;
  OSTRACE(( "UNLOCK %d now %d\n", pFile->h, pFile->locktype ));
  return rc;
}

static int os2FileControl(sqlite3_file *id, int op, void *pArg){
  switch( op ){
    case SQLITE_FCNTL_LOCKSTATE: {
      *(int*)pArg = ((os2File*)id)->locktype;
      OSTRACE(( "FCNTL_LOCKSTATE %d lock=%d\n",
                ((os2File*)id)->h, ((os2File*)id)->locktype ));
      return SQLITE_OK;
    }
    case SQLITE_FCNTL_CHUNK_SIZE: {
      ((os2File*)id)->szChunk = *(int*)pArg;
      return SQLITE_OK;
    }
    case SQLITE_FCNTL_SIZE_HINT: {
      sqlite3_int64 sz = *(sqlite3_int64*)pArg;
      SimulateIOErrorBenign(1);
      os2Truncate(id, sz);
      SimulateIOErrorBenign(0);
      return SQLITE_OK;
    }
    case SQLITE_FCNTL_SYNC_OMITTED: {
      return SQLITE_OK;
    }
  }
  return SQLITE_NOTFOUND;
}

static int os2SectorSize(sqlite3_file *id){
  UNUSED_PARAMETER(id);
  return SQLITE_DEFAULT_SECTOR_SIZE;
}

static int os2DeviceCharacteristics(sqlite3_file *id){
  UNUSED_PARAMETER(id);
  return SQLITE_IOCAP_UNDELETABLE_WHEN_OPEN;
}


static UconvObject ucUtf8 = NULL; 
static UconvObject uclCp = NULL;  

static void initUconvObjects( void ){
  if( UniCreateUconvObject( UTF_8, &ucUtf8 ) != ULS_SUCCESS )
    ucUtf8 = NULL;
  if ( UniCreateUconvObject( (UniChar *)L"@path=yes", &uclCp ) != ULS_SUCCESS )
    uclCp = NULL;
}

static void freeUconvObjects( void ){
  if ( ucUtf8 )
    UniFreeUconvObject( ucUtf8 );
  if ( uclCp )
    UniFreeUconvObject( uclCp );
  ucUtf8 = NULL;
  uclCp = NULL;
}

static char *convertUtf8PathToCp( const char *in ){
  UniChar tempPath[CCHMAXPATH];
  char *out = (char *)calloc( CCHMAXPATH, 1 );

  if( !out )
    return NULL;

  if( !ucUtf8 || !uclCp )
    initUconvObjects();

  
  if( UniStrToUcs( ucUtf8, tempPath, (char *)in, CCHMAXPATH ) != ULS_SUCCESS )
    return out; 

  
  UniStrFromUcs( uclCp, out, tempPath, CCHMAXPATH );

  return out;
}

char *convertCpPathToUtf8( const char *in ){
  UniChar tempPath[CCHMAXPATH];
  char *out = (char *)calloc( CCHMAXPATH, 1 );

  if( !out )
    return NULL;

  if( !ucUtf8 || !uclCp )
    initUconvObjects();

  
  if( UniStrToUcs( uclCp, tempPath, (char *)in, CCHMAXPATH ) != ULS_SUCCESS )
    return out; 

  
  UniStrFromUcs( ucUtf8, out, tempPath, CCHMAXPATH );

  return out;
}


#ifndef SQLITE_OMIT_WAL

/*
** Use main database file for interprocess locking. If un-defined
** a separate file is created for this purpose. The file will be
** used only to set file locks. There will be no data written to it.
*/
#define SQLITE_OS2_NO_WAL_LOCK_FILE     

#if 0
static void _ERR_TRACE( const char *fmt, ... ) {
  va_list  ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fflush(stderr);
}
#define ERR_TRACE(rc, msg)        \
        if( (rc) != SQLITE_OK ) _ERR_TRACE msg;
#else
#define ERR_TRACE(rc, msg)
#endif

static void os2ShmEnterMutex(void){
  sqlite3_mutex_enter(sqlite3MutexAlloc(SQLITE_MUTEX_STATIC_MASTER));
}
static void os2ShmLeaveMutex(void){
  sqlite3_mutex_leave(sqlite3MutexAlloc(SQLITE_MUTEX_STATIC_MASTER));
}
#ifdef SQLITE_DEBUG
static int os2ShmMutexHeld(void) {
  return sqlite3_mutex_held(sqlite3MutexAlloc(SQLITE_MUTEX_STATIC_MASTER));
}
int GetCurrentProcessId(void) {
  PPIB pib;
  DosGetInfoBlocks(NULL, &pib);
  return (int)pib->pib_ulpid;
}
#endif

struct os2ShmNode {
  sqlite3_mutex *mutex;      
  os2ShmNode *pNext;         

  int szRegion;              

  int nRegion;               
  void **apRegion;           

  int nRef;                  
  os2ShmLink *pFirst;        

  HFILE hLockFile;           
  char shmBaseName[1];       
};


struct os2ShmLink {
  os2ShmNode *pShmNode;      
  os2ShmLink *pNext;         
  u32 sharedMask;            
  u32 exclMask;              
#ifdef SQLITE_DEBUG
  u8 id;                     
#endif
};


static os2ShmNode *os2ShmNodeList = NULL;

#ifdef  SQLITE_OS2_NO_WAL_LOCK_FILE
#define OS2_SHM_BASE   (PENDING_BYTE + 0x10000)         
#else
#define OS2_SHM_BASE   ((22+SQLITE_SHM_NLOCK)*4)        
#endif

#define OS2_SHM_DMS    (OS2_SHM_BASE+SQLITE_SHM_NLOCK)  

#define _SHM_UNLCK  1   
#define _SHM_RDLCK  2   
#define _SHM_WRLCK  3   
#define _SHM_WRLCK_WAIT 4 
static int os2ShmSystemLock(
  os2ShmNode *pNode,    
  int lockType,         
  int ofst,             
  int nByte             
){
  APIRET rc;
  FILELOCK area;
  ULONG mode, timeout;

  
  assert( sqlite3_mutex_held(pNode->mutex) || pNode->nRef==0 );

  mode = 1;     
  timeout = 0;  
  area.lOffset = ofst;
  area.lRange = nByte;

  switch( lockType ) {
    case _SHM_WRLCK_WAIT:
      timeout = (ULONG)-1;      
    case _SHM_WRLCK:
      mode = 0;                 
    case _SHM_RDLCK:
      rc = DosSetFileLocks(pNode->hLockFile, 
                           NULL, &area, timeout, mode);
      break;
    
    default:
      rc = DosSetFileLocks(pNode->hLockFile, 
                           &area, NULL, 0, 0);
      break;
  }
                          
  OSTRACE(("SHM-LOCK %d %s %s 0x%08lx\n", 
           pNode->hLockFile,
           rc==SQLITE_OK ? "ok" : "failed",
           lockType==_SHM_UNLCK ? "Unlock" : "Lock",
           rc));

  ERR_TRACE(rc, ("os2ShmSystemLock: %d %s\n", rc, pNode->shmBaseName))

  return ( rc == 0 ) ?  SQLITE_OK : SQLITE_BUSY;
}

static int os2OpenSharedMemory( os2File *fd, int szRegion ) {
  os2ShmLink *pLink;
  os2ShmNode *pNode;
  int cbShmName, rc = SQLITE_OK;
  char shmName[CCHMAXPATH + 30];
#ifndef SQLITE_OS2_NO_WAL_LOCK_FILE
  ULONG action;
#endif
  
  
  cbShmName = sprintf(shmName, "\\SHAREMEM\\%s", fd->zFullPathCp );
  if( cbShmName >= CCHMAXPATH-8 )
    return SQLITE_IOERR_SHMOPEN; 

  
  shmName[10+1] = '!';

  
  pLink = sqlite3_malloc( sizeof(*pLink) );
  if( !pLink )
    return SQLITE_NOMEM;

  
  os2ShmEnterMutex();

  
  for( pNode = os2ShmNodeList; 
       pNode && stricmp(shmName, pNode->shmBaseName) != 0; 
       pNode = pNode->pNext )   ;

  
  if( !pNode ) {
    pNode = sqlite3_malloc( sizeof(*pNode) + cbShmName );
    if( pNode ) {
      memset(pNode, 0, sizeof(*pNode) );
      pNode->szRegion = szRegion;
      pNode->hLockFile = (HFILE)-1;      
      strcpy(pNode->shmBaseName, shmName);

#ifdef SQLITE_OS2_NO_WAL_LOCK_FILE
      if( DosDupHandle(fd->h, &pNode->hLockFile) != 0 ) {
#else
      sprintf(shmName, "%s-lck", fd->zFullPathCp);
      if( DosOpen((PSZ)shmName, &pNode->hLockFile, &action, 0, FILE_NORMAL, 
                  OPEN_ACTION_OPEN_IF_EXISTS | OPEN_ACTION_CREATE_IF_NEW,
                  OPEN_ACCESS_READWRITE | OPEN_SHARE_DENYNONE | 
                  OPEN_FLAGS_NOINHERIT | OPEN_FLAGS_FAIL_ON_ERROR,
                  NULL) != 0 ) {
#endif
        sqlite3_free(pNode);  
        rc = SQLITE_IOERR;
      } else {
        pNode->mutex = sqlite3_mutex_alloc(SQLITE_MUTEX_FAST);
        if( !pNode->mutex ) {
          sqlite3_free(pNode);  
          rc = SQLITE_NOMEM;
        }
      }   
    } else {
      rc = SQLITE_NOMEM;
    }
    
    if( rc == SQLITE_OK ) {
      pNode->pNext = os2ShmNodeList;
      os2ShmNodeList = pNode;
    } else {
      pNode = NULL;
    }
  } else if( pNode->szRegion != szRegion ) {
    rc = SQLITE_IOERR_SHMSIZE;
    pNode = NULL;
  }

  if( pNode ) {
    sqlite3_mutex_enter(pNode->mutex);

    memset(pLink, 0, sizeof(*pLink));

    pLink->pShmNode = pNode;
    pLink->pNext = pNode->pFirst;
    pNode->pFirst = pLink;
    pNode->nRef++;

    fd->pShmLink = pLink;

    sqlite3_mutex_leave(pNode->mutex);
    
  } else {
    
    sqlite3_free(pLink);  
  }

  os2ShmLeaveMutex();

  ERR_TRACE(rc, ("os2OpenSharedMemory: %d  %s\n", rc, fd->zFullPathCp))  
  
  return rc;
}

static void os2PurgeShmNodes( int deleteFlag ) {
  os2ShmNode *pNode;
  os2ShmNode **ppNode;

  os2ShmEnterMutex();
  
  ppNode = &os2ShmNodeList;

  while( *ppNode ) {
    pNode = *ppNode;

    if( pNode->nRef == 0 ) {
      *ppNode = pNode->pNext;   
     
      if( pNode->apRegion ) {
        
        os2ShmSystemLock(pNode, _SHM_WRLCK_WAIT, OS2_SHM_DMS, 1);

        while( pNode->nRegion-- ) {
#ifdef SQLITE_DEBUG
          int rc = 
#endif          
          DosFreeMem(pNode->apRegion[pNode->nRegion]);

          OSTRACE(("SHM-PURGE pid-%d unmap region=%d %s\n",
                  (int)GetCurrentProcessId(), pNode->nRegion,
                  rc == 0 ? "ok" : "failed"));
        }

        
        os2ShmSystemLock(pNode, _SHM_UNLCK, OS2_SHM_DMS, 1);

        sqlite3_free(pNode->apRegion);
      }  

      DosClose(pNode->hLockFile);
      
#ifndef SQLITE_OS2_NO_WAL_LOCK_FILE
      if( deleteFlag ) {
         char fileName[CCHMAXPATH];
         
         sprintf(fileName, "%s-lck", pNode->shmBaseName + 10);
         
         fileName[1] = ':';
         
         DosForceDelete(fileName); 
      }
#endif

      sqlite3_mutex_free(pNode->mutex);

      sqlite3_free(pNode);
      
    } else {
      ppNode = &pNode->pNext;
    }
  } 

  os2ShmLeaveMutex();
}

static int os2ShmMap(
  sqlite3_file *id,               
  int iRegion,                    
  int szRegion,                   
  int bExtend,                    
  void volatile **pp              
){
  PVOID pvTemp;
  void **apRegion;
  os2ShmNode *pNode;
  int n, rc = SQLITE_OK;
  char shmName[CCHMAXPATH];
  os2File *pFile = (os2File*)id;
  
  *pp = NULL;

  if( !pFile->pShmLink )
    rc = os2OpenSharedMemory( pFile, szRegion );
  
  if( rc == SQLITE_OK ) {
    pNode = pFile->pShmLink->pShmNode ;
    
    sqlite3_mutex_enter(pNode->mutex);
    
    assert( szRegion==pNode->szRegion );

    
    if( iRegion >= pNode->nRegion ) {
      
      os2ShmSystemLock(pNode, _SHM_WRLCK_WAIT, OS2_SHM_DMS, 1);

      apRegion = sqlite3_realloc(
        pNode->apRegion, (iRegion + 1) * sizeof(apRegion[0]));

      if( apRegion ) {
        pNode->apRegion = apRegion;

        while( pNode->nRegion <= iRegion ) {
          sprintf(shmName, "%s-%u", 
                  pNode->shmBaseName, pNode->nRegion);

          if( DosGetNamedSharedMem(&pvTemp, (PSZ)shmName, 
                PAG_READ | PAG_WRITE) != NO_ERROR ) {
            if( !bExtend )
              break;

            if( DosAllocSharedMem(&pvTemp, (PSZ)shmName, szRegion,
                  PAG_READ | PAG_WRITE | PAG_COMMIT | OBJ_ANY) != NO_ERROR && 
                DosAllocSharedMem(&pvTemp, (PSZ)shmName, szRegion,
                  PAG_READ | PAG_WRITE | PAG_COMMIT) != NO_ERROR ) { 
              rc = SQLITE_NOMEM;
              break;
            }
          }

          apRegion[pNode->nRegion++] = pvTemp;
        }

         
        for( n = pNode->nRegion; n <= iRegion; n++ )
          pNode->apRegion[n] = NULL;

        
        *pp = pNode->apRegion[iRegion];
      } else {
        rc = SQLITE_NOMEM;
      }

      
      os2ShmSystemLock(pNode, _SHM_UNLCK, OS2_SHM_DMS, 1);
      
    } else {
      
      *pp = pNode->apRegion[iRegion];
    }

    sqlite3_mutex_leave(pNode->mutex);
  } 

  ERR_TRACE(rc, ("os2ShmMap: %s iRgn = %d, szRgn = %d, bExt = %d : %d\n", 
                 pFile->zFullPathCp, iRegion, szRegion, bExtend, rc))
          
  return rc;
}

static int os2ShmUnmap(
  sqlite3_file *id,               
  int deleteFlag                  
){
  os2File *pFile = (os2File*)id;
  os2ShmLink *pLink = pFile->pShmLink;
  
  if( pLink ) {
    int nRef = -1;
    os2ShmLink **ppLink;
    os2ShmNode *pNode = pLink->pShmNode;

    sqlite3_mutex_enter(pNode->mutex);
    
    for( ppLink = &pNode->pFirst;
         *ppLink && *ppLink != pLink;
         ppLink = &(*ppLink)->pNext )   ;
         
    assert(*ppLink);

    if( *ppLink ) {
      *ppLink = pLink->pNext;
      nRef = --pNode->nRef;
    } else {
      ERR_TRACE(1, ("os2ShmUnmap: link not found ! %s\n", 
                    pNode->shmBaseName))
    }
    
    pFile->pShmLink = NULL;
    sqlite3_free(pLink);

    sqlite3_mutex_leave(pNode->mutex);
    
    if( nRef == 0 )
      os2PurgeShmNodes( deleteFlag );
  }

  return SQLITE_OK;
}

static int os2ShmLock(
  sqlite3_file *id,          
  int ofst,                  
  int n,                     
  int flags                  
){
  u32 mask;                             
  int rc = SQLITE_OK;                   
  os2File *pFile = (os2File*)id;
  os2ShmLink *p = pFile->pShmLink;      
  os2ShmLink *pX;                       
  os2ShmNode *pShmNode = p->pShmNode;   
  
  assert( ofst>=0 && ofst+n<=SQLITE_SHM_NLOCK );
  assert( n>=1 );
  assert( flags==(SQLITE_SHM_LOCK | SQLITE_SHM_SHARED)
       || flags==(SQLITE_SHM_LOCK | SQLITE_SHM_EXCLUSIVE)
       || flags==(SQLITE_SHM_UNLOCK | SQLITE_SHM_SHARED)
       || flags==(SQLITE_SHM_UNLOCK | SQLITE_SHM_EXCLUSIVE) );
  assert( n==1 || (flags & SQLITE_SHM_EXCLUSIVE)!=0 );

  mask = (u32)((1U<<(ofst+n)) - (1U<<ofst));
  assert( n>1 || mask==(1<<ofst) );


  sqlite3_mutex_enter(pShmNode->mutex);

  if( flags & SQLITE_SHM_UNLOCK ){
    u32 allMask = 0; 

    
    for(pX=pShmNode->pFirst; pX; pX=pX->pNext){
      if( pX==p ) continue;
      assert( (pX->exclMask & (p->exclMask|p->sharedMask))==0 );
      allMask |= pX->sharedMask;
    }

    
    if( (mask & allMask)==0 ){
      rc = os2ShmSystemLock(pShmNode, _SHM_UNLCK, ofst+OS2_SHM_BASE, n);
    }else{
      rc = SQLITE_OK;
    }

    
    if( rc==SQLITE_OK ){
      p->exclMask &= ~mask;
      p->sharedMask &= ~mask;
    } 
  }else if( flags & SQLITE_SHM_SHARED ){
    u32 allShared = 0;  

    for(pX=pShmNode->pFirst; pX; pX=pX->pNext){
      if( (pX->exclMask & mask)!=0 ){
        rc = SQLITE_BUSY;
        break;
      }
      allShared |= pX->sharedMask;
    }

    
    if( rc==SQLITE_OK ){
      if( (allShared & mask)==0 ){
        rc = os2ShmSystemLock(pShmNode, _SHM_RDLCK, ofst+OS2_SHM_BASE, n);
      }else{
        rc = SQLITE_OK;
      }
    }

    
    if( rc==SQLITE_OK ){
      p->sharedMask |= mask;
    }
  }else{
    for(pX=pShmNode->pFirst; pX; pX=pX->pNext){
      if( (pX->exclMask & mask)!=0 || (pX->sharedMask & mask)!=0 ){
        rc = SQLITE_BUSY;
        break;
      }
    }
  
    if( rc==SQLITE_OK ){
      rc = os2ShmSystemLock(pShmNode, _SHM_WRLCK, ofst+OS2_SHM_BASE, n);
      if( rc==SQLITE_OK ){
        assert( (p->sharedMask & mask)==0 );
        p->exclMask |= mask;
      }
    }
  }

  sqlite3_mutex_leave(pShmNode->mutex);
  
  OSTRACE(("SHM-LOCK shmid-%d, pid-%d got %03x,%03x %s\n",
           p->id, (int)GetCurrentProcessId(), p->sharedMask, p->exclMask,
           rc ? "failed" : "ok"));

  ERR_TRACE(rc, ("os2ShmLock: ofst = %d, n = %d, flags = 0x%x -> %d \n", 
                 ofst, n, flags, rc))
                  
  return rc; 
}

static void os2ShmBarrier(
  sqlite3_file *id                
){
  UNUSED_PARAMETER(id);
  os2ShmEnterMutex();
  os2ShmLeaveMutex();
}

#else
# define os2ShmMap     0
# define os2ShmLock    0
# define os2ShmBarrier 0
# define os2ShmUnmap   0
#endif 


static const sqlite3_io_methods os2IoMethod = {
  2,                              
  os2Close,                       
  os2Read,                        
  os2Write,                       
  os2Truncate,                    
  os2Sync,                        
  os2FileSize,                    
  os2Lock,                        
  os2Unlock,                      
  os2CheckReservedLock,           
  os2FileControl,                 
  os2SectorSize,                  
  os2DeviceCharacteristics,       
  os2ShmMap,                      
  os2ShmLock,                     
  os2ShmBarrier,                  
  os2ShmUnmap                     
};



static int getTempname(int nBuf, char *zBuf ){
  static const char zChars[] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "0123456789";
  int i, j;
  PSZ zTempPathCp;      
  char zTempPath[CCHMAXPATH];
  ULONG ulDriveNum, ulDriveMap;
  
  SimulateIOError( return SQLITE_IOERR );

  if( sqlite3_temp_directory ) {
    sqlite3_snprintf(CCHMAXPATH-30, zTempPath, "%s", sqlite3_temp_directory);
  } else if( DosScanEnv( (PSZ)"TEMP",   &zTempPathCp ) == NO_ERROR ||
             DosScanEnv( (PSZ)"TMP",    &zTempPathCp ) == NO_ERROR ||
             DosScanEnv( (PSZ)"TMPDIR", &zTempPathCp ) == NO_ERROR ) {
    char *zTempPathUTF = convertCpPathToUtf8( (char *)zTempPathCp );
    sqlite3_snprintf(CCHMAXPATH-30, zTempPath, "%s", zTempPathUTF);
    free( zTempPathUTF );
  } else if( DosQueryCurrentDisk( &ulDriveNum, &ulDriveMap ) == NO_ERROR ) {
    zTempPath[0] = (char)('A' + ulDriveNum - 1);
    zTempPath[1] = ':'; 
    zTempPath[2] = '\0'; 
  } else {
    zTempPath[0] = '\0'; 
  }
  
  j = sqlite3Strlen30(zTempPath);
  while( j > 0 && ( zTempPath[j-1] == '\\' || zTempPath[j-1] == '/' || 
                    zTempPath[j-1] == ' ' ) ){
    j--;
  }
  zTempPath[j] = '\0';
  
  
  sqlite3_snprintf(nBuf-22, zBuf,
                   "%s\\"SQLITE_TEMP_FILE_PREFIX, zTempPath);
  j = sqlite3Strlen30(zBuf);
  sqlite3_randomness( 20, &zBuf[j] );
  for( i = 0; i < 20; i++, j++ ){
    zBuf[j] = zChars[ ((unsigned char)zBuf[j])%(sizeof(zChars)-1) ];
  }
  zBuf[j] = 0;

  OSTRACE(( "TEMP FILENAME: %s\n", zBuf ));
  return SQLITE_OK;
}


static int os2FullPathname(
  sqlite3_vfs *pVfs,          
  const char *zRelative,      
  int nFull,                  
  char *zFull                 
){
  char *zRelativeCp = convertUtf8PathToCp( zRelative );
  char zFullCp[CCHMAXPATH] = "\0";
  char *zFullUTF;
  APIRET rc = DosQueryPathInfo( (PSZ)zRelativeCp, FIL_QUERYFULLNAME, 
                                zFullCp, CCHMAXPATH );
  free( zRelativeCp );
  zFullUTF = convertCpPathToUtf8( zFullCp );
  sqlite3_snprintf( nFull, zFull, zFullUTF );
  free( zFullUTF );
  return rc == NO_ERROR ? SQLITE_OK : SQLITE_IOERR;
}


static int os2Open(
  sqlite3_vfs *pVfs,            
  const char *zName,            
  sqlite3_file *id,             
  int flags,                    
  int *pOutFlags                
){
  HFILE h;
  ULONG ulOpenFlags = 0;
  ULONG ulOpenMode = 0;
  ULONG ulAction = 0;
  ULONG rc;
  os2File *pFile = (os2File*)id;
  const char *zUtf8Name = zName;
  char *zNameCp;
  char  zTmpname[CCHMAXPATH];

  int isExclusive  = (flags & SQLITE_OPEN_EXCLUSIVE);
  int isCreate     = (flags & SQLITE_OPEN_CREATE);
  int isReadWrite  = (flags & SQLITE_OPEN_READWRITE);
#ifndef NDEBUG
  int isDelete     = (flags & SQLITE_OPEN_DELETEONCLOSE);
  int isReadonly   = (flags & SQLITE_OPEN_READONLY);
  int eType        = (flags & 0xFFFFFF00);
  int isOpenJournal = (isCreate && (
        eType==SQLITE_OPEN_MASTER_JOURNAL 
     || eType==SQLITE_OPEN_MAIN_JOURNAL 
     || eType==SQLITE_OPEN_WAL
  ));
#endif

  UNUSED_PARAMETER(pVfs);
  assert( id!=0 );

  assert((isReadonly==0 || isReadWrite==0) && (isReadWrite || isReadonly));
  assert(isCreate==0 || isReadWrite);
  assert(isExclusive==0 || isCreate);
  assert(isDelete==0 || isCreate);

  assert( (!isDelete && zName) || eType!=SQLITE_OPEN_MAIN_DB );
  assert( (!isDelete && zName) || eType!=SQLITE_OPEN_MAIN_JOURNAL );
  assert( (!isDelete && zName) || eType!=SQLITE_OPEN_MASTER_JOURNAL );
  assert( (!isDelete && zName) || eType!=SQLITE_OPEN_WAL );

  
  assert( eType==SQLITE_OPEN_MAIN_DB      || eType==SQLITE_OPEN_TEMP_DB 
       || eType==SQLITE_OPEN_MAIN_JOURNAL || eType==SQLITE_OPEN_TEMP_JOURNAL 
       || eType==SQLITE_OPEN_SUBJOURNAL   || eType==SQLITE_OPEN_MASTER_JOURNAL 
       || eType==SQLITE_OPEN_TRANSIENT_DB || eType==SQLITE_OPEN_WAL
  );

  memset( pFile, 0, sizeof(*pFile) );
  pFile->h = (HFILE)-1;

  if( !zUtf8Name ){
    assert(isDelete && !isOpenJournal);
    rc = getTempname(CCHMAXPATH, zTmpname);
    if( rc!=SQLITE_OK ){
      return rc;
    }
    zUtf8Name = zTmpname;
  }

  if( isReadWrite ){
    ulOpenMode |= OPEN_ACCESS_READWRITE;
  }else{
    ulOpenMode |= OPEN_ACCESS_READONLY;
  }

  ulOpenMode |= OPEN_FLAGS_RANDOM | OPEN_SHARE_DENYNONE | 
                OPEN_FLAGS_NOINHERIT | OPEN_FLAGS_FAIL_ON_ERROR;

  if( isExclusive ){
    
    
    ulOpenFlags |= OPEN_ACTION_CREATE_IF_NEW | OPEN_ACTION_FAIL_IF_EXISTS;
  }else if( isCreate ){
    
    ulOpenFlags |= OPEN_ACTION_CREATE_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS;
  }else{
    
    ulOpenFlags |= OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS;
  }

  zNameCp = convertUtf8PathToCp( zUtf8Name );
  rc = DosOpen( (PSZ)zNameCp,
                &h,
                &ulAction,
                0L,
                FILE_NORMAL,
                ulOpenFlags,
                ulOpenMode,
                (PEAOP2)NULL );
  free( zNameCp );

  if( rc != NO_ERROR ){
    OSTRACE(( "OPEN Invalid handle rc=%d: zName=%s, ulAction=%#lx, ulFlags=%#lx, ulMode=%#lx\n",
              rc, zUtf8Name, ulAction, ulOpenFlags, ulOpenMode ));

    if( isReadWrite ){
      return os2Open( pVfs, zName, id,
                      ((flags|SQLITE_OPEN_READONLY)&~(SQLITE_OPEN_CREATE|SQLITE_OPEN_READWRITE)),
                      pOutFlags );
    }else{
      return SQLITE_CANTOPEN;
    }
  }

  if( pOutFlags ){
    *pOutFlags = isReadWrite ? SQLITE_OPEN_READWRITE : SQLITE_OPEN_READONLY;
  }

  os2FullPathname( pVfs, zUtf8Name, sizeof( zTmpname ), zTmpname );
  pFile->zFullPathCp = convertUtf8PathToCp( zTmpname );
  pFile->pMethod = &os2IoMethod;
  pFile->flags = flags;
  pFile->h = h;

  OpenCounter(+1);
  OSTRACE(( "OPEN %d pOutFlags=%d\n", pFile->h, pOutFlags ));
  return SQLITE_OK;
}

static int os2Delete(
  sqlite3_vfs *pVfs,                     
  const char *zFilename,                 
  int syncDir                            
){
  APIRET rc;
  char *zFilenameCp;
  SimulateIOError( return SQLITE_IOERR_DELETE );
  zFilenameCp = convertUtf8PathToCp( zFilename );
  rc = DosDelete( (PSZ)zFilenameCp );
  free( zFilenameCp );
  OSTRACE(( "DELETE \"%s\"\n", zFilename ));
  return (rc == NO_ERROR ||
          rc == ERROR_FILE_NOT_FOUND ||
          rc == ERROR_PATH_NOT_FOUND ) ? SQLITE_OK : SQLITE_IOERR_DELETE;
}

static int os2Access(
  sqlite3_vfs *pVfs,        
  const char *zFilename,    
  int flags,                
  int *pOut                 
){
  APIRET rc;
  FILESTATUS3 fsts3ConfigInfo;
  char *zFilenameCp;

  UNUSED_PARAMETER(pVfs);
  SimulateIOError( return SQLITE_IOERR_ACCESS; );
  
  zFilenameCp = convertUtf8PathToCp( zFilename );
  rc = DosQueryPathInfo( (PSZ)zFilenameCp, FIL_STANDARD,
                         &fsts3ConfigInfo, sizeof(FILESTATUS3) );
  free( zFilenameCp );
  OSTRACE(( "ACCESS fsts3ConfigInfo.attrFile=%d flags=%d rc=%d\n",
            fsts3ConfigInfo.attrFile, flags, rc ));

  switch( flags ){
    case SQLITE_ACCESS_EXISTS:
      if( fsts3ConfigInfo.cbFile == 0 ) 
        rc = ERROR_FILE_NOT_FOUND;
      break;
    case SQLITE_ACCESS_READ:
      break;
    case SQLITE_ACCESS_READWRITE:
      if( fsts3ConfigInfo.attrFile & FILE_READONLY )
        rc = ERROR_ACCESS_DENIED;
      break;
    default:
      rc = ERROR_FILE_NOT_FOUND;
      assert( !"Invalid flags argument" );
  }

  *pOut = (rc == NO_ERROR);
  OSTRACE(( "ACCESS %s flags %d: rc=%d\n", zFilename, flags, *pOut ));

  return SQLITE_OK;
}


#ifndef SQLITE_OMIT_LOAD_EXTENSION
static void *os2DlOpen(sqlite3_vfs *pVfs, const char *zFilename){
  HMODULE hmod;
  APIRET rc;
  char *zFilenameCp = convertUtf8PathToCp(zFilename);
  rc = DosLoadModule(NULL, 0, (PSZ)zFilenameCp, &hmod);
  free(zFilenameCp);
  return rc != NO_ERROR ? 0 : (void*)hmod;
}
static void os2DlError(sqlite3_vfs *pVfs, int nBuf, char *zBufOut){
}
static void (*os2DlSym(sqlite3_vfs *pVfs, void *pHandle, const char *zSymbol))(void){
  PFN pfn;
  APIRET rc;
  rc = DosQueryProcAddr((HMODULE)pHandle, 0L, (PSZ)zSymbol, &pfn);
  if( rc != NO_ERROR ){
    char _zSymbol[256] = "_";
    strncat(_zSymbol, zSymbol, 254);
    rc = DosQueryProcAddr((HMODULE)pHandle, 0L, (PSZ)_zSymbol, &pfn);
  }
  return rc != NO_ERROR ? 0 : (void(*)(void))pfn;
}
static void os2DlClose(sqlite3_vfs *pVfs, void *pHandle){
  DosFreeModule((HMODULE)pHandle);
}
#else 
  #define os2DlOpen 0
  #define os2DlError 0
  #define os2DlSym 0
  #define os2DlClose 0
#endif


static int os2Randomness(sqlite3_vfs *pVfs, int nBuf, char *zBuf ){
  int n = 0;
#if defined(SQLITE_TEST)
  n = nBuf;
  memset(zBuf, 0, nBuf);
#else
  int i;                           
  PPIB ppib;
  PTIB ptib;
  DATETIME dt; 
  static unsigned c = 0;
  
  static ULONG svIdx[6] = { QSV_MS_COUNT, QSV_TIME_LOW,
                            QSV_MAXPRMEM, QSV_MAXSHMEM,
                            QSV_TOTAVAILMEM, QSV_TOTRESMEM };

  
  if( (int)sizeof(dt)-3 <= nBuf - n ){
    c += 0x0100;
    DosGetDateTime(&dt);
    dt.year = (USHORT)((dt.year - 1900) | c);
    memcpy(&zBuf[n], &dt, sizeof(dt)-3);
    n += sizeof(dt)-3;
  }

  
  if( (int)sizeof(ULONG) <= nBuf - n ){
    DosGetInfoBlocks(&ptib, &ppib);
    *(PULONG)&zBuf[n] = MAKELONG(ppib->pib_ulpid,
                                 ptib->tib_ptib2->tib2_ultid);
    n += sizeof(ULONG);
  }

  
  for( i = 0; i < 6 && (int)sizeof(ULONG) <= nBuf - n; i++ ){
    DosQuerySysInfo(svIdx[i], svIdx[i], 
                    (PULONG)&zBuf[n], sizeof(ULONG));
    n += sizeof(ULONG);
  } 
#endif

  return n;
}

static int os2Sleep( sqlite3_vfs *pVfs, int microsec ){
  DosSleep( (microsec/1000) );
  return microsec;
}

#ifdef SQLITE_TEST
int sqlite3_current_time = 0;
#endif

static int os2CurrentTimeInt64(sqlite3_vfs *pVfs, sqlite3_int64 *piNow){
#ifdef SQLITE_TEST
  static const sqlite3_int64 unixEpoch = 24405875*(sqlite3_int64)8640000;
#endif
  int year, month, datepart, timepart;
 
  DATETIME dt;
  DosGetDateTime( &dt );

  year = dt.year;
  month = dt.month;

  datepart = (int)dt.day - 32076 +
    1461*(year + 4800 + (month - 14)/12)/4 +
    367*(month - 2 - (month - 14)/12*12)/12 -
    3*((year + 4900 + (month - 14)/12)/100)/4;

  
  timepart = 12*3600*1000 + dt.hundredths*10 + dt.seconds*1000 +
    ((int)dt.minutes + dt.timezone)*60*1000 + dt.hours*3600*1000;

  *piNow = (sqlite3_int64)datepart*86400*1000 + timepart;
   
#ifdef SQLITE_TEST
  if( sqlite3_current_time ){
    *piNow = 1000*(sqlite3_int64)sqlite3_current_time + unixEpoch;
  }
#endif

  UNUSED_PARAMETER(pVfs);
  return 0;
}

static int os2CurrentTime( sqlite3_vfs *pVfs, double *prNow ){
  int rc;
  sqlite3_int64 i;
  rc = os2CurrentTimeInt64(pVfs, &i);
  if( !rc ){
    *prNow = i/86400000.0;
  }
  return rc;
}

static int os2GetLastError(sqlite3_vfs *pVfs, int nBuf, char *zBuf){
  assert(zBuf[0]=='\0');
  return 0;
}

int sqlite3_os_init(void){
  static sqlite3_vfs os2Vfs = {
    3,                 
    sizeof(os2File),   
    CCHMAXPATH,        
    0,                 
    "os2",             
    0,                 

    os2Open,           
    os2Delete,         
    os2Access,         
    os2FullPathname,   
    os2DlOpen,         
    os2DlError,        
    os2DlSym,          
    os2DlClose,        
    os2Randomness,     
    os2Sleep,          
    os2CurrentTime,    
    os2GetLastError,   
    os2CurrentTimeInt64, 
    0,                 
    0,                 
    0                  
  };
  sqlite3_vfs_register(&os2Vfs, 1);
  initUconvObjects();
  return SQLITE_OK;
}
int sqlite3_os_end(void){
  freeUconvObjects();
  return SQLITE_OK;
}

#endif 

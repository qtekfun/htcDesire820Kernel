/*
** 2010 October 28
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
** This file contains a VFS "shim" - a layer that sits in between the
** pager and the real VFS.
**
** This particular shim enforces a multiplex system on DB files.  
** This shim shards/partitions a single DB file into smaller 
** "chunks" such that the total DB file size may exceed the maximum
** file size of the underlying file system.
**
*/
#include "sqlite3.h"
#include <string.h>
#include <assert.h>
#include "test_multiplex.h"

#ifndef SQLITE_CORE
  #define SQLITE_CORE 1  
#endif
#include "sqlite3ext.h"

#define UNUSED_PARAMETER(x) (void)(x)
#define MAX_PAGE_SIZE       0x10000
#define DEFAULT_SECTOR_SIZE 0x1000

#if defined(SQLITE_THREADSAFE) && SQLITE_THREADSAFE==0
#define sqlite3_mutex_alloc(X)    ((sqlite3_mutex*)8)
#define sqlite3_mutex_free(X)
#define sqlite3_mutex_enter(X)
#define sqlite3_mutex_try(X)      SQLITE_OK
#define sqlite3_mutex_leave(X)
#define sqlite3_mutex_held(X)     ((void)(X),1)
#define sqlite3_mutex_notheld(X)  ((void)(X),1)
#endif 



#define SQLITE_MULTIPLEX_VFS_NAME "multiplex"

#define SQLITE_MULTIPLEX_CHUNK_SIZE (MAX_PAGE_SIZE*16384)

#define SQLITE_MULTIPLEX_MAX_CHUNKS 32

/* If SQLITE_MULTIPLEX_EXT_OVWR is defined, the 
** last SQLITE_MULTIPLEX_EXT_SZ characters of the 
** filename will be overwritten, otherwise, the 
** multiplex extension is simply appended to the filename.
** Ex.  (undefined) test.db -> test.db01
**      (defined)   test.db -> test.01
** Chunk 0 does not have a modified extension.
*/
#define SQLITE_MULTIPLEX_EXT_FMT    "%02d"
#define SQLITE_MULTIPLEX_EXT_SZ     2


typedef struct multiplexGroup multiplexGroup;
typedef struct multiplexConn multiplexConn;

struct multiplexGroup {
  sqlite3_file **pReal;            
  char *bOpen;                     
  char *zName;                     
  int nName;                       
  int flags;                       
  int nChunkSize;                  
  int nMaxChunks;                  
  int bEnabled;                    
  multiplexGroup *pNext, *pPrev;   
};

struct multiplexConn {
  sqlite3_file base;              
  multiplexGroup *pGroup;         
};

static struct {
  sqlite3_vfs *pOrigVfs;

  sqlite3_vfs sThisVfs;

  sqlite3_io_methods sIoMethodsV1;
  sqlite3_io_methods sIoMethodsV2;

  int isInitialized;

  sqlite3_mutex *pMutex;

  multiplexGroup *pGroups;

  char *zName;

} gMultiplex;

static void multiplexEnter(void){ sqlite3_mutex_enter(gMultiplex.pMutex); }
static void multiplexLeave(void){ sqlite3_mutex_leave(gMultiplex.pMutex); }

int multiplexStrlen30(const char *z){
  const char *z2 = z;
  if( z==0 ) return 0;
  while( *z2 ){ z2++; }
  return 0x3fffffff & (int)(z2 - z);
}

static sqlite3_file *multiplexSubOpen(multiplexConn *pConn, int iChunk, int *rc, int *pOutFlags){
  multiplexGroup *pGroup = pConn->pGroup;
  sqlite3_vfs *pOrigVfs = gMultiplex.pOrigVfs;        
  if( iChunk<pGroup->nMaxChunks ){
    sqlite3_file *pSubOpen = pGroup->pReal[iChunk];    
    if( !pGroup->bOpen[iChunk] ){
      memcpy(gMultiplex.zName, pGroup->zName, pGroup->nName+1);
      if( iChunk ){
#ifdef SQLITE_MULTIPLEX_EXT_OVWR
        sqlite3_snprintf(SQLITE_MULTIPLEX_EXT_SZ+1, gMultiplex.zName+pGroup->nName-SQLITE_MULTIPLEX_EXT_SZ, SQLITE_MULTIPLEX_EXT_FMT, iChunk);
#else
        sqlite3_snprintf(SQLITE_MULTIPLEX_EXT_SZ+1, gMultiplex.zName+pGroup->nName, SQLITE_MULTIPLEX_EXT_FMT, iChunk);
#endif
      }
      *rc = pOrigVfs->xOpen(pOrigVfs, gMultiplex.zName, pSubOpen, pGroup->flags, pOutFlags);
      if( *rc==SQLITE_OK ){
        pGroup->bOpen[iChunk] = -1;
        return pSubOpen;
      }
      return NULL;
    }
    *rc = SQLITE_OK;
    return pSubOpen;
  }
  *rc = SQLITE_FULL;
  return NULL;
}

static void multiplexControlFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  int rc = SQLITE_OK;
  sqlite3 *db = sqlite3_context_db_handle(context);
  int op;
  int iVal;

  if( !db || argc!=2 ){ 
    rc = SQLITE_ERROR; 
  }else{
    
    op = sqlite3_value_int(argv[0]);
    iVal = sqlite3_value_int(argv[1]);
    
    switch( op ){
      case 1: 
        op = MULTIPLEX_CTRL_ENABLE; 
        break;
      case 2: 
        op = MULTIPLEX_CTRL_SET_CHUNK_SIZE; 
        break;
      case 3: 
        op = MULTIPLEX_CTRL_SET_MAX_CHUNKS; 
        break;
      default:
        rc = SQLITE_NOTFOUND;
        break;
    }
  }
  if( rc==SQLITE_OK ){
    rc = sqlite3_file_control(db, 0, op, &iVal);
  }
  sqlite3_result_error_code(context, rc);
}

static int multiplexFuncInit(
  sqlite3 *db, 
  char **pzErrMsg, 
  const sqlite3_api_routines *pApi
){
  int rc;
  rc = sqlite3_create_function(db, "multiplex_control", 2, SQLITE_ANY, 
      0, multiplexControlFunc, 0, 0);
  return rc;
}


static int multiplexOpen(
  sqlite3_vfs *pVfs,         
  const char *zName,         
  sqlite3_file *pConn,       
  int flags,                 
  int *pOutFlags             
){
  int rc;                                        
  multiplexConn *pMultiplexOpen;                 
  multiplexGroup *pGroup;                        
  sqlite3_file *pSubOpen;                        
  sqlite3_vfs *pOrigVfs = gMultiplex.pOrigVfs;   
  int nName = multiplexStrlen30(zName);
  int i;
  int sz;

  UNUSED_PARAMETER(pVfs);

  multiplexEnter();
  pMultiplexOpen = (multiplexConn*)pConn;
  
  sz = sizeof(multiplexGroup)                                
     + (sizeof(sqlite3_file *)*SQLITE_MULTIPLEX_MAX_CHUNKS)  
     + (pOrigVfs->szOsFile*SQLITE_MULTIPLEX_MAX_CHUNKS)      
     + SQLITE_MULTIPLEX_MAX_CHUNKS                           
     + nName + 1;                                            
#ifndef SQLITE_MULTIPLEX_EXT_OVWR
  sz += SQLITE_MULTIPLEX_EXT_SZ;
  assert(nName+SQLITE_MULTIPLEX_EXT_SZ < pOrigVfs->mxPathname);
#else
  assert(nName >= SQLITE_MULTIPLEX_EXT_SZ);
  assert(nName < pOrigVfs->mxPathname);
#endif
  pGroup = sqlite3_malloc( sz );
  if( pGroup==0 ){
    rc=SQLITE_NOMEM;
  }else{
    
    char *p = (char *)&pGroup[1];
    pMultiplexOpen->pGroup = pGroup;
    memset(pGroup, 0, sz);
    pGroup->bEnabled = -1;
    pGroup->nChunkSize = SQLITE_MULTIPLEX_CHUNK_SIZE;
    pGroup->nMaxChunks = SQLITE_MULTIPLEX_MAX_CHUNKS;
    pGroup->pReal = (sqlite3_file **)p;
    p += (sizeof(sqlite3_file *)*pGroup->nMaxChunks);
    for(i=0; i<pGroup->nMaxChunks; i++){
      pGroup->pReal[i] = (sqlite3_file *)p;
      p += pOrigVfs->szOsFile;
    }
    
    pGroup->bOpen = p;
    p += pGroup->nMaxChunks;
    pGroup->zName = p;
    
    memcpy(pGroup->zName, zName, nName+1);
    pGroup->nName = nName;
    pGroup->flags = flags;
    pSubOpen = multiplexSubOpen(pMultiplexOpen, 0, &rc, pOutFlags);
    if( pSubOpen ){
      sqlite3_int64 sz;
      int rc2 = pSubOpen->pMethods->xFileSize(pSubOpen, &sz);
      if( (rc2==SQLITE_OK) && (sz>pGroup->nChunkSize) ){
        pGroup->bEnabled = 0;
      }
      if( pSubOpen->pMethods->iVersion==1 ){
        pMultiplexOpen->base.pMethods = &gMultiplex.sIoMethodsV1;
      }else{
        pMultiplexOpen->base.pMethods = &gMultiplex.sIoMethodsV2;
      }
      
      pGroup->pNext = gMultiplex.pGroups;
      if( gMultiplex.pGroups ) gMultiplex.pGroups->pPrev = pGroup;
      gMultiplex.pGroups = pGroup;
    }else{
      sqlite3_free(pGroup);
    }
  }
  multiplexLeave();
  return rc;
}

static int multiplexDelete(
  sqlite3_vfs *pVfs,         
  const char *zName,         
  int syncDir
){
  sqlite3_vfs *pOrigVfs = gMultiplex.pOrigVfs;   
  int rc = SQLITE_OK;
  int nName = multiplexStrlen30(zName);
  int i;

  UNUSED_PARAMETER(pVfs);

  multiplexEnter();
  memcpy(gMultiplex.zName, zName, nName+1);
  for(i=0; i<SQLITE_MULTIPLEX_MAX_CHUNKS; i++){
    int rc2;
    int exists = 0;
    if( i ){
#ifdef SQLITE_MULTIPLEX_EXT_OVWR
        sqlite3_snprintf(SQLITE_MULTIPLEX_EXT_SZ+1, 
            gMultiplex.zName+nName-SQLITE_MULTIPLEX_EXT_SZ, 
            SQLITE_MULTIPLEX_EXT_FMT, i);
#else
        sqlite3_snprintf(SQLITE_MULTIPLEX_EXT_SZ+1, 
            gMultiplex.zName+nName, 
            SQLITE_MULTIPLEX_EXT_FMT, i);
#endif
    }
    rc2 = pOrigVfs->xAccess(pOrigVfs, gMultiplex.zName, 
        SQLITE_ACCESS_EXISTS, &exists);
    if( rc2==SQLITE_OK && exists){
      
      rc2 = pOrigVfs->xDelete(pOrigVfs, gMultiplex.zName, syncDir);
      if( rc2!=SQLITE_OK ) rc = rc2;
    }else{
      
      break;
    }
  }
  multiplexLeave();
  return rc;
}

static int multiplexAccess(sqlite3_vfs *a, const char *b, int c, int *d){
  return gMultiplex.pOrigVfs->xAccess(gMultiplex.pOrigVfs, b, c, d);
}
static int multiplexFullPathname(sqlite3_vfs *a, const char *b, int c, char *d){
  return gMultiplex.pOrigVfs->xFullPathname(gMultiplex.pOrigVfs, b, c, d);
}
static void *multiplexDlOpen(sqlite3_vfs *a, const char *b){
  return gMultiplex.pOrigVfs->xDlOpen(gMultiplex.pOrigVfs, b);
}
static void multiplexDlError(sqlite3_vfs *a, int b, char *c){
  gMultiplex.pOrigVfs->xDlError(gMultiplex.pOrigVfs, b, c);
}
static void (*multiplexDlSym(sqlite3_vfs *a, void *b, const char *c))(void){
  return gMultiplex.pOrigVfs->xDlSym(gMultiplex.pOrigVfs, b, c);
}
static void multiplexDlClose(sqlite3_vfs *a, void *b){
  gMultiplex.pOrigVfs->xDlClose(gMultiplex.pOrigVfs, b);
}
static int multiplexRandomness(sqlite3_vfs *a, int b, char *c){
  return gMultiplex.pOrigVfs->xRandomness(gMultiplex.pOrigVfs, b, c);
}
static int multiplexSleep(sqlite3_vfs *a, int b){
  return gMultiplex.pOrigVfs->xSleep(gMultiplex.pOrigVfs, b);
}
static int multiplexCurrentTime(sqlite3_vfs *a, double *b){
  return gMultiplex.pOrigVfs->xCurrentTime(gMultiplex.pOrigVfs, b);
}
static int multiplexGetLastError(sqlite3_vfs *a, int b, char *c){
  return gMultiplex.pOrigVfs->xGetLastError(gMultiplex.pOrigVfs, b, c);
}
static int multiplexCurrentTimeInt64(sqlite3_vfs *a, sqlite3_int64 *b){
  return gMultiplex.pOrigVfs->xCurrentTimeInt64(gMultiplex.pOrigVfs, b);
}


static int multiplexClose(sqlite3_file *pConn){
  multiplexConn *p = (multiplexConn*)pConn;
  multiplexGroup *pGroup = p->pGroup;
  int rc = SQLITE_OK;
  int i;
  multiplexEnter();
  
  for(i=0; i<pGroup->nMaxChunks; i++){
    if( pGroup->bOpen[i] ){
      sqlite3_file *pSubOpen = pGroup->pReal[i];
      int rc2 = pSubOpen->pMethods->xClose(pSubOpen);
      if( rc2!=SQLITE_OK ) rc = rc2;
      pGroup->bOpen[i] = 0;
    }
  }
  
  if( pGroup->pNext ) pGroup->pNext->pPrev = pGroup->pPrev;
  if( pGroup->pPrev ){
    pGroup->pPrev->pNext = pGroup->pNext;
  }else{
    gMultiplex.pGroups = pGroup->pNext;
  }
  sqlite3_free(pGroup);
  multiplexLeave();
  return rc;
}

static int multiplexRead(
  sqlite3_file *pConn,
  void *pBuf,
  int iAmt,
  sqlite3_int64 iOfst
){
  multiplexConn *p = (multiplexConn*)pConn;
  multiplexGroup *pGroup = p->pGroup;
  int rc = SQLITE_OK;
  multiplexEnter();
  if( !pGroup->bEnabled ){
    sqlite3_file *pSubOpen = multiplexSubOpen(p, 0, &rc, NULL);
    rc = ( !pSubOpen ) ? SQLITE_IOERR_READ : pSubOpen->pMethods->xRead(pSubOpen, pBuf, iAmt, iOfst);
  }else{
    while( iAmt > 0 ){
      int i = (int)(iOfst / pGroup->nChunkSize);
      sqlite3_file *pSubOpen = multiplexSubOpen(p, i, &rc, NULL);
      if( pSubOpen ){
        int extra = ((int)(iOfst % pGroup->nChunkSize) + iAmt) - pGroup->nChunkSize;
        if( extra<0 ) extra = 0;
        iAmt -= extra;
        rc = pSubOpen->pMethods->xRead(pSubOpen, pBuf, iAmt, iOfst % pGroup->nChunkSize);
        if( rc!=SQLITE_OK ) break;
        pBuf = (char *)pBuf + iAmt;
        iOfst += iAmt;
        iAmt = extra;
      }else{
        rc = SQLITE_IOERR_READ;
        break;
      }
    }
  }
  multiplexLeave();
  return rc;
}

static int multiplexWrite(
  sqlite3_file *pConn,
  const void *pBuf,
  int iAmt,
  sqlite3_int64 iOfst
){
  multiplexConn *p = (multiplexConn*)pConn;
  multiplexGroup *pGroup = p->pGroup;
  int rc = SQLITE_OK;
  multiplexEnter();
  if( !pGroup->bEnabled ){
    sqlite3_file *pSubOpen = multiplexSubOpen(p, 0, &rc, NULL);
    rc = ( !pSubOpen ) ? SQLITE_IOERR_WRITE : pSubOpen->pMethods->xWrite(pSubOpen, pBuf, iAmt, iOfst);
  }else{
    while( iAmt > 0 ){
      int i = (int)(iOfst / pGroup->nChunkSize);
      sqlite3_file *pSubOpen = multiplexSubOpen(p, i, &rc, NULL);
      if( pSubOpen ){
        int extra = ((int)(iOfst % pGroup->nChunkSize) + iAmt) - pGroup->nChunkSize;
        if( extra<0 ) extra = 0;
        iAmt -= extra;
        rc = pSubOpen->pMethods->xWrite(pSubOpen, pBuf, iAmt, iOfst % pGroup->nChunkSize);
        if( rc!=SQLITE_OK ) break;
        pBuf = (char *)pBuf + iAmt;
        iOfst += iAmt;
        iAmt = extra;
      }else{
        rc = SQLITE_IOERR_WRITE;
        break;
      }
    }
  }
  multiplexLeave();
  return rc;
}

static int multiplexTruncate(sqlite3_file *pConn, sqlite3_int64 size){
  multiplexConn *p = (multiplexConn*)pConn;
  multiplexGroup *pGroup = p->pGroup;
  int rc = SQLITE_OK;
  multiplexEnter();
  if( !pGroup->bEnabled ){
    sqlite3_file *pSubOpen = multiplexSubOpen(p, 0, &rc, NULL);
    rc = ( !pSubOpen ) ? SQLITE_IOERR_TRUNCATE : pSubOpen->pMethods->xTruncate(pSubOpen, size);
  }else{
    int rc2;
    int i;
    sqlite3_file *pSubOpen;
    sqlite3_vfs *pOrigVfs = gMultiplex.pOrigVfs;   
    memcpy(gMultiplex.zName, pGroup->zName, pGroup->nName+1);
    
    for(i=(int)(size / pGroup->nChunkSize)+1; i<pGroup->nMaxChunks; i++){
      
      if( pGroup->bOpen[i] ){
        pSubOpen = pGroup->pReal[i];
        rc2 = pSubOpen->pMethods->xClose(pSubOpen);
        if( rc2!=SQLITE_OK ) rc = SQLITE_IOERR_TRUNCATE;
        pGroup->bOpen[i] = 0;
      }
#ifdef SQLITE_MULTIPLEX_EXT_OVWR
      sqlite3_snprintf(SQLITE_MULTIPLEX_EXT_SZ+1, 
          gMultiplex.zName+pGroup->nName-SQLITE_MULTIPLEX_EXT_SZ, 
          SQLITE_MULTIPLEX_EXT_FMT, i);
#else
      sqlite3_snprintf(SQLITE_MULTIPLEX_EXT_SZ+1, 
          gMultiplex.zName+pGroup->nName, 
          SQLITE_MULTIPLEX_EXT_FMT, i);
#endif
      rc2 = pOrigVfs->xDelete(pOrigVfs, gMultiplex.zName, 0);
      if( rc2!=SQLITE_OK ) rc = SQLITE_IOERR_TRUNCATE;
    }
    pSubOpen = multiplexSubOpen(p, (int)(size / pGroup->nChunkSize), &rc2, NULL);
    if( pSubOpen ){
      rc2 = pSubOpen->pMethods->xTruncate(pSubOpen, size % pGroup->nChunkSize);
      if( rc2!=SQLITE_OK ) rc = rc2;
    }else{
      rc = SQLITE_IOERR_TRUNCATE;
    }
  }
  multiplexLeave();
  return rc;
}

static int multiplexSync(sqlite3_file *pConn, int flags){
  multiplexConn *p = (multiplexConn*)pConn;
  multiplexGroup *pGroup = p->pGroup;
  int rc = SQLITE_OK;
  int i;
  multiplexEnter();
  for(i=0; i<pGroup->nMaxChunks; i++){
    
    if( pGroup->bOpen[i] ){
      sqlite3_file *pSubOpen = pGroup->pReal[i];
      int rc2 = pSubOpen->pMethods->xSync(pSubOpen, flags);
      if( rc2!=SQLITE_OK ) rc = rc2;
    }
  }
  multiplexLeave();
  return rc;
}

static int multiplexFileSize(sqlite3_file *pConn, sqlite3_int64 *pSize){
  multiplexConn *p = (multiplexConn*)pConn;
  multiplexGroup *pGroup = p->pGroup;
  int rc = SQLITE_OK;
  int rc2;
  int i;
  multiplexEnter();
  if( !pGroup->bEnabled ){
    sqlite3_file *pSubOpen = multiplexSubOpen(p, 0, &rc, NULL);
    rc = ( !pSubOpen ) ? SQLITE_IOERR_FSTAT : pSubOpen->pMethods->xFileSize(pSubOpen, pSize);
  }else{
    *pSize = 0;
    for(i=0; i<pGroup->nMaxChunks; i++){
      sqlite3_file *pSubOpen = NULL;
      
      if( pGroup->bOpen[i] ){
        pSubOpen = pGroup->pReal[i];
      }else{
        sqlite3_vfs *pOrigVfs = gMultiplex.pOrigVfs;   
        int exists = 0;
        memcpy(gMultiplex.zName, pGroup->zName, pGroup->nName+1);
        if( i ){
#ifdef SQLITE_MULTIPLEX_EXT_OVWR
          sqlite3_snprintf(SQLITE_MULTIPLEX_EXT_SZ+1, 
              gMultiplex.zName+pGroup->nName-SQLITE_MULTIPLEX_EXT_SZ, 
              SQLITE_MULTIPLEX_EXT_FMT, i);
#else
          sqlite3_snprintf(SQLITE_MULTIPLEX_EXT_SZ+1, 
              gMultiplex.zName+pGroup->nName, 
              SQLITE_MULTIPLEX_EXT_FMT, i);
#endif
        }
        rc2 = pOrigVfs->xAccess(pOrigVfs, gMultiplex.zName, 
            SQLITE_ACCESS_EXISTS, &exists);
        if( rc2==SQLITE_OK && exists){
          
          pSubOpen = multiplexSubOpen(p, i, &rc, NULL);
        }else{
          
          break;
        }
      }
      if( pSubOpen ){
        sqlite3_int64 sz;
        rc2 = pSubOpen->pMethods->xFileSize(pSubOpen, &sz);
        if( rc2!=SQLITE_OK ){
          rc = rc2;
        }else{
          if( sz>pGroup->nChunkSize ){
            rc = SQLITE_IOERR_FSTAT;
          }
          *pSize += sz;
        }
      }else{
        break;
      }
    }
  }
  multiplexLeave();
  return rc;
}

static int multiplexLock(sqlite3_file *pConn, int lock){
  multiplexConn *p = (multiplexConn*)pConn;
  int rc;
  sqlite3_file *pSubOpen = multiplexSubOpen(p, 0, &rc, NULL);
  if( pSubOpen ){
    return pSubOpen->pMethods->xLock(pSubOpen, lock);
  }
  return SQLITE_BUSY;
}

static int multiplexUnlock(sqlite3_file *pConn, int lock){
  multiplexConn *p = (multiplexConn*)pConn;
  int rc;
  sqlite3_file *pSubOpen = multiplexSubOpen(p, 0, &rc, NULL);
  if( pSubOpen ){
    return pSubOpen->pMethods->xUnlock(pSubOpen, lock);
  }
  return SQLITE_IOERR_UNLOCK;
}

static int multiplexCheckReservedLock(sqlite3_file *pConn, int *pResOut){
  multiplexConn *p = (multiplexConn*)pConn;
  int rc;
  sqlite3_file *pSubOpen = multiplexSubOpen(p, 0, &rc, NULL);
  if( pSubOpen ){
    return pSubOpen->pMethods->xCheckReservedLock(pSubOpen, pResOut);
  }
  return SQLITE_IOERR_CHECKRESERVEDLOCK;
}

static int multiplexFileControl(sqlite3_file *pConn, int op, void *pArg){
  multiplexConn *p = (multiplexConn*)pConn;
  multiplexGroup *pGroup = p->pGroup;
  int rc = SQLITE_ERROR;
  sqlite3_file *pSubOpen;

  if( !gMultiplex.isInitialized ) return SQLITE_MISUSE;
  switch( op ){
    case MULTIPLEX_CTRL_ENABLE:
      if( pArg ) {
        int bEnabled = *(int *)pArg;
        pGroup->bEnabled = bEnabled;
        rc = SQLITE_OK;
      }
      break;
    case MULTIPLEX_CTRL_SET_CHUNK_SIZE:
      if( pArg ) {
        int nChunkSize = *(int *)pArg;
        if( nChunkSize<1 ){
          rc = SQLITE_MISUSE;
        }else{
          
          nChunkSize = (nChunkSize + (MAX_PAGE_SIZE-1));
          nChunkSize &= ~(MAX_PAGE_SIZE-1);
          pGroup->nChunkSize = nChunkSize;
          rc = SQLITE_OK;
        }
      }
      break;
    case MULTIPLEX_CTRL_SET_MAX_CHUNKS:
      if( pArg ) {
        int nMaxChunks = *(int *)pArg;
        if(( nMaxChunks<1 ) || ( nMaxChunks>SQLITE_MULTIPLEX_MAX_CHUNKS )){
          rc = SQLITE_MISUSE;
        }else{
          pGroup->nMaxChunks = nMaxChunks;
          rc = SQLITE_OK;
        }
      }
      break;
    case SQLITE_FCNTL_SIZE_HINT:
    case SQLITE_FCNTL_CHUNK_SIZE:
      
      rc = SQLITE_OK;
      break;
    default:
      pSubOpen = multiplexSubOpen(p, 0, &rc, NULL);
      if( pSubOpen ){
        rc = pSubOpen->pMethods->xFileControl(pSubOpen, op, pArg);
      }
      break;
  }
  return rc;
}

static int multiplexSectorSize(sqlite3_file *pConn){
  multiplexConn *p = (multiplexConn*)pConn;
  int rc;
  sqlite3_file *pSubOpen = multiplexSubOpen(p, 0, &rc, NULL);
  if( pSubOpen ){
    return pSubOpen->pMethods->xSectorSize(pSubOpen);
  }
  return DEFAULT_SECTOR_SIZE;
}

static int multiplexDeviceCharacteristics(sqlite3_file *pConn){
  multiplexConn *p = (multiplexConn*)pConn;
  int rc;
  sqlite3_file *pSubOpen = multiplexSubOpen(p, 0, &rc, NULL);
  if( pSubOpen ){
    return pSubOpen->pMethods->xDeviceCharacteristics(pSubOpen);
  }
  return 0;
}

static int multiplexShmMap(
  sqlite3_file *pConn,            
  int iRegion,                    
  int szRegion,                   
  int bExtend,                    
  void volatile **pp              
){
  multiplexConn *p = (multiplexConn*)pConn;
  int rc;
  sqlite3_file *pSubOpen = multiplexSubOpen(p, 0, &rc, NULL);
  if( pSubOpen ){
    return pSubOpen->pMethods->xShmMap(pSubOpen, iRegion, szRegion, bExtend, pp);
  }
  return SQLITE_IOERR;
}

static int multiplexShmLock(
  sqlite3_file *pConn,       
  int ofst,                  
  int n,                     
  int flags                  
){
  multiplexConn *p = (multiplexConn*)pConn;
  int rc;
  sqlite3_file *pSubOpen = multiplexSubOpen(p, 0, &rc, NULL);
  if( pSubOpen ){
    return pSubOpen->pMethods->xShmLock(pSubOpen, ofst, n, flags);
  }
  return SQLITE_BUSY;
}

static void multiplexShmBarrier(sqlite3_file *pConn){
  multiplexConn *p = (multiplexConn*)pConn;
  int rc;
  sqlite3_file *pSubOpen = multiplexSubOpen(p, 0, &rc, NULL);
  if( pSubOpen ){
    pSubOpen->pMethods->xShmBarrier(pSubOpen);
  }
}

static int multiplexShmUnmap(sqlite3_file *pConn, int deleteFlag){
  multiplexConn *p = (multiplexConn*)pConn;
  int rc;
  sqlite3_file *pSubOpen = multiplexSubOpen(p, 0, &rc, NULL);
  if( pSubOpen ){
    return pSubOpen->pMethods->xShmUnmap(pSubOpen, deleteFlag);
  }
  return SQLITE_OK;
}

int sqlite3_multiplex_initialize(const char *zOrigVfsName, int makeDefault){
  sqlite3_vfs *pOrigVfs;
  if( gMultiplex.isInitialized ) return SQLITE_MISUSE;
  pOrigVfs = sqlite3_vfs_find(zOrigVfsName);
  if( pOrigVfs==0 ) return SQLITE_ERROR;
  assert( pOrigVfs!=&gMultiplex.sThisVfs );
  gMultiplex.pMutex = sqlite3_mutex_alloc(SQLITE_MUTEX_FAST);
  if( !gMultiplex.pMutex ){
    return SQLITE_NOMEM;
  }
  gMultiplex.zName = sqlite3_malloc(pOrigVfs->mxPathname);
  if( !gMultiplex.zName ){
    sqlite3_mutex_free(gMultiplex.pMutex);
    return SQLITE_NOMEM;
  }
  gMultiplex.pGroups = NULL;
  gMultiplex.isInitialized = 1;
  gMultiplex.pOrigVfs = pOrigVfs;
  gMultiplex.sThisVfs = *pOrigVfs;
  gMultiplex.sThisVfs.szOsFile += sizeof(multiplexConn);
  gMultiplex.sThisVfs.zName = SQLITE_MULTIPLEX_VFS_NAME;
  gMultiplex.sThisVfs.xOpen = multiplexOpen;
  gMultiplex.sThisVfs.xDelete = multiplexDelete;
  gMultiplex.sThisVfs.xAccess = multiplexAccess;
  gMultiplex.sThisVfs.xFullPathname = multiplexFullPathname;
  gMultiplex.sThisVfs.xDlOpen = multiplexDlOpen;
  gMultiplex.sThisVfs.xDlError = multiplexDlError;
  gMultiplex.sThisVfs.xDlSym = multiplexDlSym;
  gMultiplex.sThisVfs.xDlClose = multiplexDlClose;
  gMultiplex.sThisVfs.xRandomness = multiplexRandomness;
  gMultiplex.sThisVfs.xSleep = multiplexSleep;
  gMultiplex.sThisVfs.xCurrentTime = multiplexCurrentTime;
  gMultiplex.sThisVfs.xGetLastError = multiplexGetLastError;
  gMultiplex.sThisVfs.xCurrentTimeInt64 = multiplexCurrentTimeInt64;

  gMultiplex.sIoMethodsV1.iVersion = 1;
  gMultiplex.sIoMethodsV1.xClose = multiplexClose;
  gMultiplex.sIoMethodsV1.xRead = multiplexRead;
  gMultiplex.sIoMethodsV1.xWrite = multiplexWrite;
  gMultiplex.sIoMethodsV1.xTruncate = multiplexTruncate;
  gMultiplex.sIoMethodsV1.xSync = multiplexSync;
  gMultiplex.sIoMethodsV1.xFileSize = multiplexFileSize;
  gMultiplex.sIoMethodsV1.xLock = multiplexLock;
  gMultiplex.sIoMethodsV1.xUnlock = multiplexUnlock;
  gMultiplex.sIoMethodsV1.xCheckReservedLock = multiplexCheckReservedLock;
  gMultiplex.sIoMethodsV1.xFileControl = multiplexFileControl;
  gMultiplex.sIoMethodsV1.xSectorSize = multiplexSectorSize;
  gMultiplex.sIoMethodsV1.xDeviceCharacteristics = multiplexDeviceCharacteristics;
  gMultiplex.sIoMethodsV2 = gMultiplex.sIoMethodsV1;
  gMultiplex.sIoMethodsV2.iVersion = 2;
  gMultiplex.sIoMethodsV2.xShmMap = multiplexShmMap;
  gMultiplex.sIoMethodsV2.xShmLock = multiplexShmLock;
  gMultiplex.sIoMethodsV2.xShmBarrier = multiplexShmBarrier;
  gMultiplex.sIoMethodsV2.xShmUnmap = multiplexShmUnmap;
  sqlite3_vfs_register(&gMultiplex.sThisVfs, makeDefault);

  sqlite3_auto_extension((void*)multiplexFuncInit);

  return SQLITE_OK;
}

int sqlite3_multiplex_shutdown(void){
  if( gMultiplex.isInitialized==0 ) return SQLITE_MISUSE;
  if( gMultiplex.pGroups ) return SQLITE_MISUSE;
  gMultiplex.isInitialized = 0;
  sqlite3_free(gMultiplex.zName);
  sqlite3_mutex_free(gMultiplex.pMutex);
  sqlite3_vfs_unregister(&gMultiplex.sThisVfs);
  memset(&gMultiplex, 0, sizeof(gMultiplex));
  return SQLITE_OK;
}

#ifdef SQLITE_TEST
#include <tcl.h>
extern const char *sqlite3TestErrorName(int);


static int test_multiplex_initialize(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  const char *zName;              
  int makeDefault;                
  int rc;                         

  UNUSED_PARAMETER(clientData);

  
  if( objc!=3 ){
    Tcl_WrongNumArgs(interp, 1, objv, "NAME MAKEDEFAULT");
    return TCL_ERROR;
  }
  zName = Tcl_GetString(objv[1]);
  if( Tcl_GetBooleanFromObj(interp, objv[2], &makeDefault) ) return TCL_ERROR;
  if( zName[0]=='\0' ) zName = 0;

  
  rc = sqlite3_multiplex_initialize(zName, makeDefault);
  Tcl_SetResult(interp, (char *)sqlite3TestErrorName(rc), TCL_STATIC);

  return TCL_OK;
}

static int test_multiplex_shutdown(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  int rc;                         

  UNUSED_PARAMETER(clientData);

  if( objc!=1 ){
    Tcl_WrongNumArgs(interp, 1, objv, "");
    return TCL_ERROR;
  }

  
  rc = sqlite3_multiplex_shutdown();
  Tcl_SetResult(interp, (char *)sqlite3TestErrorName(rc), TCL_STATIC);

  return TCL_OK;
}

static int test_multiplex_dump(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  Tcl_Obj *pResult;
  Tcl_Obj *pGroupTerm;
  multiplexGroup *pGroup;
  int i;
  int nChunks = 0;

  UNUSED_PARAMETER(clientData);
  UNUSED_PARAMETER(objc);
  UNUSED_PARAMETER(objv);

  pResult = Tcl_NewObj();
  multiplexEnter();
  for(pGroup=gMultiplex.pGroups; pGroup; pGroup=pGroup->pNext){
    pGroupTerm = Tcl_NewObj();

    pGroup->zName[pGroup->nName] = '\0';
    Tcl_ListObjAppendElement(interp, pGroupTerm,
          Tcl_NewStringObj(pGroup->zName, -1));
    Tcl_ListObjAppendElement(interp, pGroupTerm,
          Tcl_NewIntObj(pGroup->nName));
    Tcl_ListObjAppendElement(interp, pGroupTerm,
          Tcl_NewIntObj(pGroup->flags));

    
    for(i=0; i<pGroup->nMaxChunks; i++){
      if( pGroup->bOpen[i] ) nChunks++;
    }
    Tcl_ListObjAppendElement(interp, pGroupTerm,
          Tcl_NewIntObj(nChunks));

    Tcl_ListObjAppendElement(interp, pGroupTerm,
          Tcl_NewIntObj(pGroup->nChunkSize));
    Tcl_ListObjAppendElement(interp, pGroupTerm,
          Tcl_NewIntObj(pGroup->nMaxChunks));

    Tcl_ListObjAppendElement(interp, pResult, pGroupTerm);
  }
  multiplexLeave();
  Tcl_SetObjResult(interp, pResult);
  return TCL_OK;
}

static int test_multiplex_control(
  ClientData cd,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  int rc;                         
  int idx;                        
  Tcl_CmdInfo cmdInfo;            
  sqlite3 *db;                    
  int iValue = 0;
  void *pArg = 0;

  struct SubCommand {
    const char *zName;
    int op;
    int argtype;
  } aSub[] = {
    { "enable",       MULTIPLEX_CTRL_ENABLE,           1 },
    { "chunk_size",   MULTIPLEX_CTRL_SET_CHUNK_SIZE,   1 },
    { "max_chunks",   MULTIPLEX_CTRL_SET_MAX_CHUNKS,   1 },
    { 0, 0, 0 }
  };

  if( objc!=5 ){
    Tcl_WrongNumArgs(interp, 1, objv, "HANDLE DBNAME SUB-COMMAND INT-VALUE");
    return TCL_ERROR;
  }

  if( 0==Tcl_GetCommandInfo(interp, Tcl_GetString(objv[1]), &cmdInfo) ){
    Tcl_AppendResult(interp, "expected database handle, got \"", 0);
    Tcl_AppendResult(interp, Tcl_GetString(objv[1]), "\"", 0);
    return TCL_ERROR;
  }else{
    db = *(sqlite3 **)cmdInfo.objClientData;
  }

  rc = Tcl_GetIndexFromObjStruct(
      interp, objv[3], aSub, sizeof(aSub[0]), "sub-command", 0, &idx
  );
  if( rc!=TCL_OK ) return rc;

  switch( aSub[idx].argtype ){
    case 1:
      if( Tcl_GetIntFromObj(interp, objv[4], &iValue) ){
        return TCL_ERROR;
      }
      pArg = (void *)&iValue;
      break;
    default:
      Tcl_WrongNumArgs(interp, 4, objv, "SUB-COMMAND");
      return TCL_ERROR;
  }

  rc = sqlite3_file_control(db, Tcl_GetString(objv[2]), aSub[idx].op, pArg);
  Tcl_SetResult(interp, (char *)sqlite3TestErrorName(rc), TCL_STATIC);
  return (rc==SQLITE_OK) ? TCL_OK : TCL_ERROR;
}

int Sqlitemultiplex_Init(Tcl_Interp *interp){
  static struct {
     char *zName;
     Tcl_ObjCmdProc *xProc;
  } aCmd[] = {
    { "sqlite3_multiplex_initialize", test_multiplex_initialize },
    { "sqlite3_multiplex_shutdown", test_multiplex_shutdown },
    { "sqlite3_multiplex_dump", test_multiplex_dump },
    { "sqlite3_multiplex_control", test_multiplex_control },
  };
  int i;

  for(i=0; i<sizeof(aCmd)/sizeof(aCmd[0]); i++){
    Tcl_CreateObjCommand(interp, aCmd[i].zName, aCmd[i].xProc, 0, 0);
  }

  return TCL_OK;
}
#endif

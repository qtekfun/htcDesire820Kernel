/*
** 2007 August 22
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
** This file implements a special kind of sqlite3_file object used
** by SQLite to create journal files if the atomic-write optimization
** is enabled.
**
** The distinctive characteristic of this sqlite3_file is that the
** actual on disk file is created lazily. When the file is created,
** the caller specifies a buffer size for an in-memory buffer to
** be used to service read() and write() requests. The actual file
** on disk is not created or populated until either:
**
**   1) The in-memory representation grows too large for the allocated 
**      buffer, or
**   2) The sqlite3JournalCreate() function is called.
*/
#ifdef SQLITE_ENABLE_ATOMIC_WRITE
#include "sqliteInt.h"


struct JournalFile {
  sqlite3_io_methods *pMethod;    
  int nBuf;                       
  char *zBuf;                     
  int iSize;                      
  int flags;                      
  sqlite3_vfs *pVfs;              
  sqlite3_file *pReal;            
  const char *zJournal;           
};
typedef struct JournalFile JournalFile;

static int createFile(JournalFile *p){
  int rc = SQLITE_OK;
  if( !p->pReal ){
    sqlite3_file *pReal = (sqlite3_file *)&p[1];
    rc = sqlite3OsOpen(p->pVfs, p->zJournal, pReal, p->flags, 0);
    if( rc==SQLITE_OK ){
      p->pReal = pReal;
      if( p->iSize>0 ){
        assert(p->iSize<=p->nBuf);
        rc = sqlite3OsWrite(p->pReal, p->zBuf, p->iSize, 0);
      }
    }
  }
  return rc;
}

static int jrnlClose(sqlite3_file *pJfd){
  JournalFile *p = (JournalFile *)pJfd;
  if( p->pReal ){
    sqlite3OsClose(p->pReal);
  }
  sqlite3_free(p->zBuf);
  return SQLITE_OK;
}

static int jrnlRead(
  sqlite3_file *pJfd,    
  void *zBuf,            
  int iAmt,              
  sqlite_int64 iOfst     
){
  int rc = SQLITE_OK;
  JournalFile *p = (JournalFile *)pJfd;
  if( p->pReal ){
    rc = sqlite3OsRead(p->pReal, zBuf, iAmt, iOfst);
  }else if( (iAmt+iOfst)>p->iSize ){
    rc = SQLITE_IOERR_SHORT_READ;
  }else{
    memcpy(zBuf, &p->zBuf[iOfst], iAmt);
  }
  return rc;
}

static int jrnlWrite(
  sqlite3_file *pJfd,    
  const void *zBuf,      /* Take data to be written from here */
  int iAmt,              
  sqlite_int64 iOfst     
){
  int rc = SQLITE_OK;
  JournalFile *p = (JournalFile *)pJfd;
  if( !p->pReal && (iOfst+iAmt)>p->nBuf ){
    rc = createFile(p);
  }
  if( rc==SQLITE_OK ){
    if( p->pReal ){
      rc = sqlite3OsWrite(p->pReal, zBuf, iAmt, iOfst);
    }else{
      memcpy(&p->zBuf[iOfst], zBuf, iAmt);
      if( p->iSize<(iOfst+iAmt) ){
        p->iSize = (iOfst+iAmt);
      }
    }
  }
  return rc;
}

static int jrnlTruncate(sqlite3_file *pJfd, sqlite_int64 size){
  int rc = SQLITE_OK;
  JournalFile *p = (JournalFile *)pJfd;
  if( p->pReal ){
    rc = sqlite3OsTruncate(p->pReal, size);
  }else if( size<p->iSize ){
    p->iSize = size;
  }
  return rc;
}

static int jrnlSync(sqlite3_file *pJfd, int flags){
  int rc;
  JournalFile *p = (JournalFile *)pJfd;
  if( p->pReal ){
    rc = sqlite3OsSync(p->pReal, flags);
  }else{
    rc = SQLITE_OK;
  }
  return rc;
}

static int jrnlFileSize(sqlite3_file *pJfd, sqlite_int64 *pSize){
  int rc = SQLITE_OK;
  JournalFile *p = (JournalFile *)pJfd;
  if( p->pReal ){
    rc = sqlite3OsFileSize(p->pReal, pSize);
  }else{
    *pSize = (sqlite_int64) p->iSize;
  }
  return rc;
}

static struct sqlite3_io_methods JournalFileMethods = {
  1,             
  jrnlClose,     
  jrnlRead,      
  jrnlWrite,     
  jrnlTruncate,  
  jrnlSync,      
  jrnlFileSize,  
  0,             
  0,             
  0,             
  0,             
  0,             
  0,             
  0,             
  0,             
  0,             
  0              
};

int sqlite3JournalOpen(
  sqlite3_vfs *pVfs,         
  const char *zName,         
  sqlite3_file *pJfd,        
  int flags,                 
  int nBuf                   
){
  JournalFile *p = (JournalFile *)pJfd;
  memset(p, 0, sqlite3JournalSize(pVfs));
  if( nBuf>0 ){
    p->zBuf = sqlite3MallocZero(nBuf);
    if( !p->zBuf ){
      return SQLITE_NOMEM;
    }
  }else{
    return sqlite3OsOpen(pVfs, zName, pJfd, flags, 0);
  }
  p->pMethod = &JournalFileMethods;
  p->nBuf = nBuf;
  p->flags = flags;
  p->zJournal = zName;
  p->pVfs = pVfs;
  return SQLITE_OK;
}

int sqlite3JournalCreate(sqlite3_file *p){
  if( p->pMethods!=&JournalFileMethods ){
    return SQLITE_OK;
  }
  return createFile((JournalFile *)p);
}

int sqlite3JournalSize(sqlite3_vfs *pVfs){
  return (pVfs->szOsFile+sizeof(JournalFile));
}
#endif

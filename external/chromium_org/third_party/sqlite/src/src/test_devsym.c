/*
** 2008 Jan 22
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
** This file contains code that modified the OS layer in order to simulate
** different device types (by overriding the return values of the 
** xDeviceCharacteristics() and xSectorSize() methods).
*/
#if SQLITE_TEST          

#include "sqlite3.h"
#include "sqliteInt.h"

#define DEVSYM_MAX_PATHNAME 512

#define DEVSYM_VFS_NAME "devsym"

typedef struct devsym_file devsym_file;
struct devsym_file {
  sqlite3_file base;
  sqlite3_file *pReal;
};

static int devsymClose(sqlite3_file*);
static int devsymRead(sqlite3_file*, void*, int iAmt, sqlite3_int64 iOfst);
static int devsymWrite(sqlite3_file*,const void*,int iAmt, sqlite3_int64 iOfst);
static int devsymTruncate(sqlite3_file*, sqlite3_int64 size);
static int devsymSync(sqlite3_file*, int flags);
static int devsymFileSize(sqlite3_file*, sqlite3_int64 *pSize);
static int devsymLock(sqlite3_file*, int);
static int devsymUnlock(sqlite3_file*, int);
static int devsymCheckReservedLock(sqlite3_file*, int *);
static int devsymFileControl(sqlite3_file*, int op, void *pArg);
static int devsymSectorSize(sqlite3_file*);
static int devsymDeviceCharacteristics(sqlite3_file*);
static int devsymShmLock(sqlite3_file*,int,int,int);
static int devsymShmMap(sqlite3_file*,int,int,int, void volatile **);
static void devsymShmBarrier(sqlite3_file*);
static int devsymShmUnmap(sqlite3_file*,int);

static int devsymOpen(sqlite3_vfs*, const char *, sqlite3_file*, int , int *);
static int devsymDelete(sqlite3_vfs*, const char *zName, int syncDir);
static int devsymAccess(sqlite3_vfs*, const char *zName, int flags, int *);
static int devsymFullPathname(sqlite3_vfs*, const char *zName, int, char *zOut);
#ifndef SQLITE_OMIT_LOAD_EXTENSION
static void *devsymDlOpen(sqlite3_vfs*, const char *zFilename);
static void devsymDlError(sqlite3_vfs*, int nByte, char *zErrMsg);
static void (*devsymDlSym(sqlite3_vfs*,void*, const char *zSymbol))(void);
static void devsymDlClose(sqlite3_vfs*, void*);
#endif 
static int devsymRandomness(sqlite3_vfs*, int nByte, char *zOut);
static int devsymSleep(sqlite3_vfs*, int microseconds);
static int devsymCurrentTime(sqlite3_vfs*, double*);

static sqlite3_vfs devsym_vfs = {
  2,                     
  sizeof(devsym_file),      
  DEVSYM_MAX_PATHNAME,      
  0,                     
  DEVSYM_VFS_NAME,          
  0,                     
  devsymOpen,               
  devsymDelete,             
  devsymAccess,             
  devsymFullPathname,       
#ifndef SQLITE_OMIT_LOAD_EXTENSION
  devsymDlOpen,             
  devsymDlError,            
  devsymDlSym,              
  devsymDlClose,            
#else
  0,                        
  0,                        
  0,                        
  0,                        
#endif 
  devsymRandomness,         
  devsymSleep,              
  devsymCurrentTime,        
  0,                        
  0                         
};

static sqlite3_io_methods devsym_io_methods = {
  2,                                
  devsymClose,                      
  devsymRead,                       
  devsymWrite,                      
  devsymTruncate,                   
  devsymSync,                       
  devsymFileSize,                   
  devsymLock,                       
  devsymUnlock,                     
  devsymCheckReservedLock,          
  devsymFileControl,                
  devsymSectorSize,                 
  devsymDeviceCharacteristics,      
  devsymShmMap,                     
  devsymShmLock,                    
  devsymShmBarrier,                 
  devsymShmUnmap                    
};

struct DevsymGlobal {
  sqlite3_vfs *pVfs;
  int iDeviceChar;
  int iSectorSize;
};
struct DevsymGlobal g = {0, 0, 512};

static int devsymClose(sqlite3_file *pFile){
  devsym_file *p = (devsym_file *)pFile;
  return sqlite3OsClose(p->pReal);
}

static int devsymRead(
  sqlite3_file *pFile, 
  void *zBuf, 
  int iAmt, 
  sqlite_int64 iOfst
){
  devsym_file *p = (devsym_file *)pFile;
  return sqlite3OsRead(p->pReal, zBuf, iAmt, iOfst);
}

static int devsymWrite(
  sqlite3_file *pFile, 
  const void *zBuf, 
  int iAmt, 
  sqlite_int64 iOfst
){
  devsym_file *p = (devsym_file *)pFile;
  return sqlite3OsWrite(p->pReal, zBuf, iAmt, iOfst);
}

static int devsymTruncate(sqlite3_file *pFile, sqlite_int64 size){
  devsym_file *p = (devsym_file *)pFile;
  return sqlite3OsTruncate(p->pReal, size);
}

static int devsymSync(sqlite3_file *pFile, int flags){
  devsym_file *p = (devsym_file *)pFile;
  return sqlite3OsSync(p->pReal, flags);
}

static int devsymFileSize(sqlite3_file *pFile, sqlite_int64 *pSize){
  devsym_file *p = (devsym_file *)pFile;
  return sqlite3OsFileSize(p->pReal, pSize);
}

static int devsymLock(sqlite3_file *pFile, int eLock){
  devsym_file *p = (devsym_file *)pFile;
  return sqlite3OsLock(p->pReal, eLock);
}

static int devsymUnlock(sqlite3_file *pFile, int eLock){
  devsym_file *p = (devsym_file *)pFile;
  return sqlite3OsUnlock(p->pReal, eLock);
}

static int devsymCheckReservedLock(sqlite3_file *pFile, int *pResOut){
  devsym_file *p = (devsym_file *)pFile;
  return sqlite3OsCheckReservedLock(p->pReal, pResOut);
}

static int devsymFileControl(sqlite3_file *pFile, int op, void *pArg){
  devsym_file *p = (devsym_file *)pFile;
  return sqlite3OsFileControl(p->pReal, op, pArg);
}

static int devsymSectorSize(sqlite3_file *pFile){
  return g.iSectorSize;
}

static int devsymDeviceCharacteristics(sqlite3_file *pFile){
  return g.iDeviceChar;
}

static int devsymShmLock(sqlite3_file *pFile, int ofst, int n, int flags){
  devsym_file *p = (devsym_file *)pFile;
  return sqlite3OsShmLock(p->pReal, ofst, n, flags);
}
static int devsymShmMap(
  sqlite3_file *pFile, 
  int iRegion, 
  int szRegion, 
  int isWrite, 
  void volatile **pp
){
  devsym_file *p = (devsym_file *)pFile;
  return sqlite3OsShmMap(p->pReal, iRegion, szRegion, isWrite, pp);
}
static void devsymShmBarrier(sqlite3_file *pFile){
  devsym_file *p = (devsym_file *)pFile;
  sqlite3OsShmBarrier(p->pReal);
}
static int devsymShmUnmap(sqlite3_file *pFile, int delFlag){
  devsym_file *p = (devsym_file *)pFile;
  return sqlite3OsShmUnmap(p->pReal, delFlag);
}



static int devsymOpen(
  sqlite3_vfs *pVfs,
  const char *zName,
  sqlite3_file *pFile,
  int flags,
  int *pOutFlags
){
  int rc;
  devsym_file *p = (devsym_file *)pFile;
  p->pReal = (sqlite3_file *)&p[1];
  rc = sqlite3OsOpen(g.pVfs, zName, p->pReal, flags, pOutFlags);
  if( p->pReal->pMethods ){
    pFile->pMethods = &devsym_io_methods;
  }
  return rc;
}

static int devsymDelete(sqlite3_vfs *pVfs, const char *zPath, int dirSync){
  return sqlite3OsDelete(g.pVfs, zPath, dirSync);
}

static int devsymAccess(
  sqlite3_vfs *pVfs, 
  const char *zPath, 
  int flags, 
  int *pResOut
){
  return sqlite3OsAccess(g.pVfs, zPath, flags, pResOut);
}

static int devsymFullPathname(
  sqlite3_vfs *pVfs, 
  const char *zPath, 
  int nOut, 
  char *zOut
){
  return sqlite3OsFullPathname(g.pVfs, zPath, nOut, zOut);
}

#ifndef SQLITE_OMIT_LOAD_EXTENSION
static void *devsymDlOpen(sqlite3_vfs *pVfs, const char *zPath){
  return sqlite3OsDlOpen(g.pVfs, zPath);
}

static void devsymDlError(sqlite3_vfs *pVfs, int nByte, char *zErrMsg){
  sqlite3OsDlError(g.pVfs, nByte, zErrMsg);
}

static void (*devsymDlSym(sqlite3_vfs *pVfs, void *p, const char *zSym))(void){
  return sqlite3OsDlSym(g.pVfs, p, zSym);
}

static void devsymDlClose(sqlite3_vfs *pVfs, void *pHandle){
  sqlite3OsDlClose(g.pVfs, pHandle);
}
#endif 

static int devsymRandomness(sqlite3_vfs *pVfs, int nByte, char *zBufOut){
  return sqlite3OsRandomness(g.pVfs, nByte, zBufOut);
}

static int devsymSleep(sqlite3_vfs *pVfs, int nMicro){
  return sqlite3OsSleep(g.pVfs, nMicro);
}

static int devsymCurrentTime(sqlite3_vfs *pVfs, double *pTimeOut){
  return g.pVfs->xCurrentTime(g.pVfs, pTimeOut);
}


void devsym_register(int iDeviceChar, int iSectorSize){
  if( g.pVfs==0 ){
    g.pVfs = sqlite3_vfs_find(0);
    devsym_vfs.szOsFile += g.pVfs->szOsFile;
    sqlite3_vfs_register(&devsym_vfs, 0);
  }
  if( iDeviceChar>=0 ){
    g.iDeviceChar = iDeviceChar;
  }else{
    g.iDeviceChar = 0;
  }
  if( iSectorSize>=0 ){
    g.iSectorSize = iSectorSize;
  }else{
    g.iSectorSize = 512;
  }
}

#endif

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
** This file contains code for a VFS layer that acts as a wrapper around
** an existing VFS. The code in this file attempts to verify that SQLite
** correctly populates and syncs a journal file before writing to a
** corresponding database file.
*/
#if SQLITE_TEST          

#include "sqlite3.h"
#include "sqliteInt.h"

/*
** INTERFACE
**
**   The public interface to this wrapper VFS is two functions:
**
**     jt_register()
**     jt_unregister()
**
**   See header comments associated with those two functions below for 
**   details.
**
** LIMITATIONS
**
**   This wrapper will not work if "PRAGMA synchronous = off" is used.
**
** OPERATION
**
**  Starting a Transaction:
**
**   When a write-transaction is started, the contents of the database is
**   inspected and the following data stored as part of the database file 
**   handle (type struct jt_file):
**
**     a) The page-size of the database file.
**     b) The number of pages that are in the database file.
**     c) The set of page numbers corresponding to free-list leaf pages.
**     d) A check-sum for every page in the database file.
**
**   The start of a write-transaction is deemed to have occurred when a 
**   28-byte journal header is written to byte offset 0 of the journal 
**   file.
**
**  Syncing the Journal File:
**
**   Whenever the xSync method is invoked to sync a journal-file, the
**   contents of the journal file are read. For each page written to
**   the journal file, a check-sum is calculated and compared to the  
**   check-sum calculated for the corresponding database page when the
**   write-transaction was initialized. The success of the comparison
**   is assert()ed. So if SQLite has written something other than the
**   original content to the database file, an assert() will fail.
**
**   Additionally, the set of page numbers for which records exist in
**   the journal file is added to (unioned with) the set of page numbers
**   corresponding to free-list leaf pages collected when the 
**   write-transaction was initialized. This set comprises the page-numbers 
**   corresponding to those pages that SQLite may now safely modify.
**
**  Writing to the Database File:
**
**   When a block of data is written to a database file, the following
**   invariants are asserted:
**
**     a) That the block of data is an aligned block of page-size bytes.
**
**     b) That if the page being written did not exist when the 
**        transaction was started (i.e. the database file is growing), then
**        the journal-file must have been synced at least once since
**        the start of the transaction.
**
**     c) That if the page being written did exist when the transaction 
**        was started, then the page must have either been a free-list
**        leaf page at the start of the transaction, or else must have
**        been stored in the journal file prior to the most recent sync.
**
**  Closing a Transaction:
**
**   When a transaction is closed, all data collected at the start of
**   the transaction, or following an xSync of a journal-file, is 
**   discarded. The end of a transaction is recognized when any one 
**   of the following occur:
**
**     a) A block of zeroes (or anything else that is not a valid 
**        journal-header) is written to the start of the journal file.
**
**     b) A journal file is truncated to zero bytes in size using xTruncate.
**
**     c) The journal file is deleted using xDelete.
*/

#define JT_MAX_PATHNAME 512

#define JT_VFS_NAME "jt"

typedef struct jt_file jt_file;
struct jt_file {
  sqlite3_file base;
  const char *zName;       
  int flags;               

  
  int eLock;               
  u32 nPage;               
  u32 nPagesize;           
  Bitvec *pWritable;       /* Bitvec of pages that may be written to the file */
  u32 *aCksum;             
  int nSync;               

  
  sqlite3_int64 iMaxOff;   /* Maximum offset written to this transaction */

  jt_file *pNext;          
  sqlite3_file *pReal;     
};

static int jtClose(sqlite3_file*);
static int jtRead(sqlite3_file*, void*, int iAmt, sqlite3_int64 iOfst);
static int jtWrite(sqlite3_file*,const void*,int iAmt, sqlite3_int64 iOfst);
static int jtTruncate(sqlite3_file*, sqlite3_int64 size);
static int jtSync(sqlite3_file*, int flags);
static int jtFileSize(sqlite3_file*, sqlite3_int64 *pSize);
static int jtLock(sqlite3_file*, int);
static int jtUnlock(sqlite3_file*, int);
static int jtCheckReservedLock(sqlite3_file*, int *);
static int jtFileControl(sqlite3_file*, int op, void *pArg);
static int jtSectorSize(sqlite3_file*);
static int jtDeviceCharacteristics(sqlite3_file*);

static int jtOpen(sqlite3_vfs*, const char *, sqlite3_file*, int , int *);
static int jtDelete(sqlite3_vfs*, const char *zName, int syncDir);
static int jtAccess(sqlite3_vfs*, const char *zName, int flags, int *);
static int jtFullPathname(sqlite3_vfs*, const char *zName, int, char *zOut);
static void *jtDlOpen(sqlite3_vfs*, const char *zFilename);
static void jtDlError(sqlite3_vfs*, int nByte, char *zErrMsg);
static void (*jtDlSym(sqlite3_vfs*,void*, const char *zSymbol))(void);
static void jtDlClose(sqlite3_vfs*, void*);
static int jtRandomness(sqlite3_vfs*, int nByte, char *zOut);
static int jtSleep(sqlite3_vfs*, int microseconds);
static int jtCurrentTime(sqlite3_vfs*, double*);
static int jtCurrentTimeInt64(sqlite3_vfs*, sqlite3_int64*);

static sqlite3_vfs jt_vfs = {
  2,                             
  sizeof(jt_file),               
  JT_MAX_PATHNAME,               
  0,                             
  JT_VFS_NAME,                   
  0,                             
  jtOpen,                        
  jtDelete,                      
  jtAccess,                      
  jtFullPathname,                
  jtDlOpen,                      
  jtDlError,                     
  jtDlSym,                       
  jtDlClose,                     
  jtRandomness,                  
  jtSleep,                       
  jtCurrentTime,                 
  0,                             
  jtCurrentTimeInt64             
};

static sqlite3_io_methods jt_io_methods = {
  1,                             
  jtClose,                       
  jtRead,                        
  jtWrite,                       
  jtTruncate,                    
  jtSync,                        
  jtFileSize,                    
  jtLock,                        
  jtUnlock,                      
  jtCheckReservedLock,           
  jtFileControl,                 
  jtSectorSize,                  
  jtDeviceCharacteristics        
};

struct JtGlobal {
  sqlite3_vfs *pVfs;             
  jt_file *pList;                
};
static struct JtGlobal g = {0, 0};

static void enterJtMutex(void){
  sqlite3_mutex_enter(sqlite3_mutex_alloc(SQLITE_MUTEX_STATIC_PRNG));
}
static void leaveJtMutex(void){
  sqlite3_mutex_leave(sqlite3_mutex_alloc(SQLITE_MUTEX_STATIC_PRNG));
}

extern int sqlite3_io_error_pending;
extern int sqlite3_io_error_hit;
static void stop_ioerr_simulation(int *piSave, int *piSave2){
  *piSave = sqlite3_io_error_pending;
  *piSave2 = sqlite3_io_error_hit;
  sqlite3_io_error_pending = -1;
  sqlite3_io_error_hit = 0;
}
static void start_ioerr_simulation(int iSave, int iSave2){
  sqlite3_io_error_pending = iSave;
  sqlite3_io_error_hit = iSave2;
}

static void closeTransaction(jt_file *p){
  sqlite3BitvecDestroy(p->pWritable);
  sqlite3_free(p->aCksum);
  p->pWritable = 0;
  p->aCksum = 0;
  p->nSync = 0;
}

static int jtClose(sqlite3_file *pFile){
  jt_file **pp;
  jt_file *p = (jt_file *)pFile;

  closeTransaction(p);
  enterJtMutex();
  if( p->zName ){
    for(pp=&g.pList; *pp!=p; pp=&(*pp)->pNext);
    *pp = p->pNext;
  }
  leaveJtMutex();
  return sqlite3OsClose(p->pReal);
}

static int jtRead(
  sqlite3_file *pFile, 
  void *zBuf, 
  int iAmt, 
  sqlite_int64 iOfst
){
  jt_file *p = (jt_file *)pFile;
  return sqlite3OsRead(p->pReal, zBuf, iAmt, iOfst);
}

static jt_file *locateDatabaseHandle(const char *zJournal){
  jt_file *pMain = 0;
  enterJtMutex();
  for(pMain=g.pList; pMain; pMain=pMain->pNext){
    int nName = strlen(zJournal) - strlen("-journal");
    if( (pMain->flags&SQLITE_OPEN_MAIN_DB)
     && (strlen(pMain->zName)==nName)
     && 0==memcmp(pMain->zName, zJournal, nName)
     && (pMain->eLock>=SQLITE_LOCK_RESERVED)
    ){
      break;
    }
  }
  leaveJtMutex();
  return pMain;
}

static u32 decodeUint32(const unsigned char *z){
  return (z[0]<<24) + (z[1]<<16) + (z[2]<<8) + z[3];
}

static u32 genCksum(const unsigned char *z, int n){
  int i;
  u32 cksum = 0;
  for(i=0; i<n; i++){
    cksum = cksum + z[i] + (cksum<<3);
  }
  return cksum;
}

static int decodeJournalHdr(
  const unsigned char *zBuf,         
  u32 *pnRec,                        
  u32 *pnPage,                       
  u32 *pnSector,                     
  u32 *pnPagesize                    
){
  unsigned char aMagic[] = { 0xd9, 0xd5, 0x05, 0xf9, 0x20, 0xa1, 0x63, 0xd7 };
  if( memcmp(aMagic, zBuf, 8) ) return SQLITE_ERROR;
  if( pnRec ) *pnRec = decodeUint32(&zBuf[8]);
  if( pnPage ) *pnPage = decodeUint32(&zBuf[16]);
  if( pnSector ) *pnSector = decodeUint32(&zBuf[20]);
  if( pnPagesize ) *pnPagesize = decodeUint32(&zBuf[24]);
  return SQLITE_OK;
}

/*
** This function is called when a new transaction is opened, just after
** the first journal-header is written to the journal file.
*/
static int openTransaction(jt_file *pMain, jt_file *pJournal){
  unsigned char *aData;
  sqlite3_file *p = pMain->pReal;
  int rc = SQLITE_OK;

  closeTransaction(pMain);
  aData = sqlite3_malloc(pMain->nPagesize);
  pMain->pWritable = sqlite3BitvecCreate(pMain->nPage);
  pMain->aCksum = sqlite3_malloc(sizeof(u32) * (pMain->nPage + 1));
  pJournal->iMaxOff = 0;

  if( !pMain->pWritable || !pMain->aCksum || !aData ){
    rc = SQLITE_IOERR_NOMEM;
  }else if( pMain->nPage>0 ){
    u32 iTrunk;
    int iSave;
    int iSave2;

    stop_ioerr_simulation(&iSave, &iSave2);

    rc = sqlite3OsRead(p, aData, pMain->nPagesize, 0);
    if( rc==SQLITE_OK ){
      u32 nDbsize = decodeUint32(&aData[28]);
      if( nDbsize>0 && memcmp(&aData[24], &aData[92], 4)==0 ){
        u32 iPg;
        for(iPg=nDbsize+1; iPg<=pMain->nPage; iPg++){
          sqlite3BitvecSet(pMain->pWritable, iPg);
        }
      }
    }
    iTrunk = decodeUint32(&aData[32]);
    while( rc==SQLITE_OK && iTrunk>0 ){
      u32 nLeaf;
      u32 iLeaf;
      sqlite3_int64 iOff = (iTrunk-1)*pMain->nPagesize;
      rc = sqlite3OsRead(p, aData, pMain->nPagesize, iOff);
      nLeaf = decodeUint32(&aData[4]);
      for(iLeaf=0; rc==SQLITE_OK && iLeaf<nLeaf; iLeaf++){
        u32 pgno = decodeUint32(&aData[8+4*iLeaf]);
        sqlite3BitvecSet(pMain->pWritable, pgno);
      }
      iTrunk = decodeUint32(aData);
    }

    
    if( rc==SQLITE_OK ){
      int ii;
      for(ii=0; rc==SQLITE_OK && ii<pMain->nPage; ii++){
        i64 iOff = (i64)(pMain->nPagesize) * (i64)ii;
        if( iOff==PENDING_BYTE ) continue;
        rc = sqlite3OsRead(pMain->pReal, aData, pMain->nPagesize, iOff);
        pMain->aCksum[ii] = genCksum(aData, pMain->nPagesize);
      }
    }

    start_ioerr_simulation(iSave, iSave2);
  }

  sqlite3_free(aData);
  return rc;
}

static int readJournalFile(jt_file *p, jt_file *pMain){
  int rc = SQLITE_OK;
  unsigned char zBuf[28];
  sqlite3_file *pReal = p->pReal;
  sqlite3_int64 iOff = 0;
  sqlite3_int64 iSize = p->iMaxOff;
  unsigned char *aPage;
  int iSave;
  int iSave2;

  aPage = sqlite3_malloc(pMain->nPagesize);
  if( !aPage ){
    return SQLITE_IOERR_NOMEM;
  }

  stop_ioerr_simulation(&iSave, &iSave2);

  while( rc==SQLITE_OK && iOff<iSize ){
    u32 nRec, nPage, nSector, nPagesize;
    u32 ii;

    
    rc = sqlite3OsRead(pReal, zBuf, 28, iOff);
    if( rc!=SQLITE_OK 
     || decodeJournalHdr(zBuf, &nRec, &nPage, &nSector, &nPagesize) 
    ){
      goto finish_rjf;
    }
    iOff += nSector;

    if( nRec==0 ){
      if( iSize>=(iOff+nSector) ){
        rc = sqlite3OsRead(pReal, zBuf, 28, iOff);
        if( rc!=SQLITE_OK || 0==decodeJournalHdr(zBuf, 0, 0, 0, 0) ){
          continue;
        }
      }
      nRec = (iSize-iOff) / (pMain->nPagesize+8);
    }

    
    for(ii=0; rc==SQLITE_OK && ii<nRec && iOff<iSize; ii++){
      u32 pgno;
      rc = sqlite3OsRead(pReal, zBuf, 4, iOff);
      if( rc==SQLITE_OK ){
        pgno = decodeUint32(zBuf);
        if( pgno>0 && pgno<=pMain->nPage ){
          if( 0==sqlite3BitvecTest(pMain->pWritable, pgno) ){
            rc = sqlite3OsRead(pReal, aPage, pMain->nPagesize, iOff+4);
            if( rc==SQLITE_OK ){
              u32 cksum = genCksum(aPage, pMain->nPagesize);
              assert( cksum==pMain->aCksum[pgno-1] );
            }
          }
          sqlite3BitvecSet(pMain->pWritable, pgno);
        }
        iOff += (8 + pMain->nPagesize);
      }
    }

    iOff = ((iOff + (nSector-1)) / nSector) * nSector;
  }

finish_rjf:
  start_ioerr_simulation(iSave, iSave2);
  sqlite3_free(aPage);
  if( rc==SQLITE_IOERR_SHORT_READ ){
    rc = SQLITE_OK;
  }
  return rc;
}

static int jtWrite(
  sqlite3_file *pFile, 
  const void *zBuf, 
  int iAmt, 
  sqlite_int64 iOfst
){
  int rc;
  jt_file *p = (jt_file *)pFile;
  if( p->flags&SQLITE_OPEN_MAIN_JOURNAL ){
    if( iOfst==0 ){
      jt_file *pMain = locateDatabaseHandle(p->zName);
      assert( pMain );
  
      if( iAmt==28 ){
        closeTransaction(pMain);
      }else if( iAmt!=12 ){
        u8 *z = (u8 *)zBuf;
        pMain->nPage = decodeUint32(&z[16]);
        pMain->nPagesize = decodeUint32(&z[24]);
        if( SQLITE_OK!=(rc=openTransaction(pMain, p)) ){
          return rc;
        }
      }
    }
    if( p->iMaxOff<(iOfst + iAmt) ){
      p->iMaxOff = iOfst + iAmt;
    }
  }

  if( p->flags&SQLITE_OPEN_MAIN_DB && p->pWritable ){
    if( iAmt<p->nPagesize 
     && p->nPagesize%iAmt==0 
     && iOfst>=(PENDING_BYTE+512) 
     && iOfst+iAmt<=PENDING_BYTE+p->nPagesize
    ){
    }else{
      u32 pgno = iOfst/p->nPagesize + 1;
      assert( (iAmt==1||iAmt==p->nPagesize) && ((iOfst+iAmt)%p->nPagesize)==0 );
      assert( pgno<=p->nPage || p->nSync>0 );
      assert( pgno>p->nPage || sqlite3BitvecTest(p->pWritable, pgno) );
    }
  }

  rc = sqlite3OsWrite(p->pReal, zBuf, iAmt, iOfst);
  if( (p->flags&SQLITE_OPEN_MAIN_JOURNAL) && iAmt==12 ){
    jt_file *pMain = locateDatabaseHandle(p->zName);
    int rc2 = readJournalFile(p, pMain);
    if( rc==SQLITE_OK ) rc = rc2;
  }
  return rc;
}

static int jtTruncate(sqlite3_file *pFile, sqlite_int64 size){
  jt_file *p = (jt_file *)pFile;
  if( p->flags&SQLITE_OPEN_MAIN_JOURNAL && size==0 ){
    
    jt_file *pMain = locateDatabaseHandle(p->zName);
    closeTransaction(pMain);
  }
  if( p->flags&SQLITE_OPEN_MAIN_DB && p->pWritable ){
    u32 pgno;
    u32 locking_page = (u32)(PENDING_BYTE/p->nPagesize+1);
    for(pgno=size/p->nPagesize+1; pgno<=p->nPage; pgno++){
      assert( pgno==locking_page || sqlite3BitvecTest(p->pWritable, pgno) );
    }
  }
  return sqlite3OsTruncate(p->pReal, size);
}

static int jtSync(sqlite3_file *pFile, int flags){
  jt_file *p = (jt_file *)pFile;

  if( p->flags&SQLITE_OPEN_MAIN_JOURNAL ){
    int rc;
    jt_file *pMain;                   

    pMain = locateDatabaseHandle(p->zName);
    assert(pMain);

    
    if( pMain->pWritable ){
      pMain->nSync++;
      rc = readJournalFile(p, pMain);
      if( rc!=SQLITE_OK ){
        return rc;
      }
    }
  }

  return sqlite3OsSync(p->pReal, flags);
}

static int jtFileSize(sqlite3_file *pFile, sqlite_int64 *pSize){
  jt_file *p = (jt_file *)pFile;
  return sqlite3OsFileSize(p->pReal, pSize);
}

static int jtLock(sqlite3_file *pFile, int eLock){
  int rc;
  jt_file *p = (jt_file *)pFile;
  rc = sqlite3OsLock(p->pReal, eLock);
  if( rc==SQLITE_OK && eLock>p->eLock ){
    p->eLock = eLock;
  }
  return rc;
}

static int jtUnlock(sqlite3_file *pFile, int eLock){
  int rc;
  jt_file *p = (jt_file *)pFile;
  rc = sqlite3OsUnlock(p->pReal, eLock);
  if( rc==SQLITE_OK && eLock<p->eLock ){
    p->eLock = eLock;
  }
  return rc;
}

static int jtCheckReservedLock(sqlite3_file *pFile, int *pResOut){
  jt_file *p = (jt_file *)pFile;
  return sqlite3OsCheckReservedLock(p->pReal, pResOut);
}

static int jtFileControl(sqlite3_file *pFile, int op, void *pArg){
  jt_file *p = (jt_file *)pFile;
  return sqlite3OsFileControl(p->pReal, op, pArg);
}

static int jtSectorSize(sqlite3_file *pFile){
  jt_file *p = (jt_file *)pFile;
  return sqlite3OsSectorSize(p->pReal);
}

static int jtDeviceCharacteristics(sqlite3_file *pFile){
  jt_file *p = (jt_file *)pFile;
  return sqlite3OsDeviceCharacteristics(p->pReal);
}

static int jtOpen(
  sqlite3_vfs *pVfs,
  const char *zName,
  sqlite3_file *pFile,
  int flags,
  int *pOutFlags
){
  int rc;
  jt_file *p = (jt_file *)pFile;
  pFile->pMethods = 0;
  p->pReal = (sqlite3_file *)&p[1];
  p->pReal->pMethods = 0;
  rc = sqlite3OsOpen(g.pVfs, zName, p->pReal, flags, pOutFlags);
  assert( rc==SQLITE_OK || p->pReal->pMethods==0 );
  if( rc==SQLITE_OK ){
    pFile->pMethods = &jt_io_methods;
    p->eLock = 0;
    p->zName = zName;
    p->flags = flags;
    p->pNext = 0;
    p->pWritable = 0;
    p->aCksum = 0;
    enterJtMutex();
    if( zName ){
      p->pNext = g.pList;
      g.pList = p;
    }
    leaveJtMutex();
  }
  return rc;
}

static int jtDelete(sqlite3_vfs *pVfs, const char *zPath, int dirSync){
  int nPath = strlen(zPath);
  if( nPath>8 && 0==strcmp("-journal", &zPath[nPath-8]) ){
    
    jt_file *pMain = locateDatabaseHandle(zPath);
    if( pMain ){
      closeTransaction(pMain);
    }
  }

  return sqlite3OsDelete(g.pVfs, zPath, dirSync);
}

static int jtAccess(
  sqlite3_vfs *pVfs, 
  const char *zPath, 
  int flags, 
  int *pResOut
){
  return sqlite3OsAccess(g.pVfs, zPath, flags, pResOut);
}

static int jtFullPathname(
  sqlite3_vfs *pVfs, 
  const char *zPath, 
  int nOut, 
  char *zOut
){
  return sqlite3OsFullPathname(g.pVfs, zPath, nOut, zOut);
}

static void *jtDlOpen(sqlite3_vfs *pVfs, const char *zPath){
  return g.pVfs->xDlOpen(g.pVfs, zPath);
}

static void jtDlError(sqlite3_vfs *pVfs, int nByte, char *zErrMsg){
  g.pVfs->xDlError(g.pVfs, nByte, zErrMsg);
}

static void (*jtDlSym(sqlite3_vfs *pVfs, void *p, const char *zSym))(void){
  return g.pVfs->xDlSym(g.pVfs, p, zSym);
}

static void jtDlClose(sqlite3_vfs *pVfs, void *pHandle){
  g.pVfs->xDlClose(g.pVfs, pHandle);
}

static int jtRandomness(sqlite3_vfs *pVfs, int nByte, char *zBufOut){
  return sqlite3OsRandomness(g.pVfs, nByte, zBufOut);
}

static int jtSleep(sqlite3_vfs *pVfs, int nMicro){
  return sqlite3OsSleep(g.pVfs, nMicro);
}

static int jtCurrentTime(sqlite3_vfs *pVfs, double *pTimeOut){
  return g.pVfs->xCurrentTime(g.pVfs, pTimeOut);
}
static int jtCurrentTimeInt64(sqlite3_vfs *pVfs, sqlite3_int64 *pTimeOut){
  return g.pVfs->xCurrentTimeInt64(g.pVfs, pTimeOut);
}


int jt_register(char *zWrap, int isDefault){
  g.pVfs = sqlite3_vfs_find(zWrap);
  if( g.pVfs==0 ){
    return SQLITE_ERROR;
  }
  jt_vfs.szOsFile = sizeof(jt_file) + g.pVfs->szOsFile;
  if( g.pVfs->iVersion==1 ){
    jt_vfs.iVersion = 1;
  }else if( g.pVfs->xCurrentTimeInt64==0 ){
    jt_vfs.xCurrentTimeInt64 = 0;
  }
  sqlite3_vfs_register(&jt_vfs, isDefault);
  return SQLITE_OK;
}

void jt_unregister(void){
  sqlite3_vfs_unregister(&jt_vfs);
}

#endif

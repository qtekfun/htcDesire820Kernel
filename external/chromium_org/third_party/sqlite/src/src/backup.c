/*
** 2009 January 28
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This file contains the implementation of the sqlite3_backup_XXX() 
** API functions and the related features.
*/
#include "sqliteInt.h"
#include "btreeInt.h"

#ifndef MIN
# define MIN(x,y) ((x)<(y)?(x):(y))
#endif

struct sqlite3_backup {
  sqlite3* pDestDb;        
  Btree *pDest;            
  u32 iDestSchema;         
  int bDestLocked;         

  Pgno iNext;              
  sqlite3* pSrcDb;         
  Btree *pSrc;             

  int rc;                  

  Pgno nRemaining;         
  Pgno nPagecount;         

  int isAttached;          
  sqlite3_backup *pNext;   
};


static Btree *findBtree(sqlite3 *pErrorDb, sqlite3 *pDb, const char *zDb){
  int i = sqlite3FindDbName(pDb, zDb);

  if( i==1 ){
    Parse *pParse;
    int rc = 0;
    pParse = sqlite3StackAllocZero(pErrorDb, sizeof(*pParse));
    if( pParse==0 ){
      sqlite3Error(pErrorDb, SQLITE_NOMEM, "out of memory");
      rc = SQLITE_NOMEM;
    }else{
      pParse->db = pDb;
      if( sqlite3OpenTempDatabase(pParse) ){
        sqlite3Error(pErrorDb, pParse->rc, "%s", pParse->zErrMsg);
        rc = SQLITE_ERROR;
      }
      sqlite3DbFree(pErrorDb, pParse->zErrMsg);
      sqlite3StackFree(pErrorDb, pParse);
    }
    if( rc ){
      return 0;
    }
  }

  if( i<0 ){
    sqlite3Error(pErrorDb, SQLITE_ERROR, "unknown database %s", zDb);
    return 0;
  }

  return pDb->aDb[i].pBt;
}

static int setDestPgsz(sqlite3_backup *p){
  int rc;
  rc = sqlite3BtreeSetPageSize(p->pDest,sqlite3BtreeGetPageSize(p->pSrc),-1,0);
  return rc;
}

sqlite3_backup *sqlite3_backup_init(
  sqlite3* pDestDb,                     
  const char *zDestDb,                  
  sqlite3* pSrcDb,                      
  const char *zSrcDb                    
){
  sqlite3_backup *p;                    

  sqlite3_mutex_enter(pSrcDb->mutex);
  sqlite3_mutex_enter(pDestDb->mutex);

  if( pSrcDb==pDestDb ){
    sqlite3Error(
        pDestDb, SQLITE_ERROR, "source and destination must be distinct"
    );
    p = 0;
  }else {
    p = (sqlite3_backup *)sqlite3_malloc(sizeof(sqlite3_backup));
    if( !p ){
      sqlite3Error(pDestDb, SQLITE_NOMEM, 0);
    }
  }

  
  if( p ){
    memset(p, 0, sizeof(sqlite3_backup));
    p->pSrc = findBtree(pDestDb, pSrcDb, zSrcDb);
    p->pDest = findBtree(pDestDb, pDestDb, zDestDb);
    p->pDestDb = pDestDb;
    p->pSrcDb = pSrcDb;
    p->iNext = 1;
    p->isAttached = 0;

    if( 0==p->pSrc || 0==p->pDest || setDestPgsz(p)==SQLITE_NOMEM ){
      /* One (or both) of the named databases did not exist or an OOM
      ** error was hit.  The error has already been written into the
      ** pDestDb handle.  All that is left to do here is free the
      ** sqlite3_backup structure.
      */
      sqlite3_free(p);
      p = 0;
    }
  }
  if( p ){
    p->pSrc->nBackup++;
  }

  sqlite3_mutex_leave(pDestDb->mutex);
  sqlite3_mutex_leave(pSrcDb->mutex);
  return p;
}

static int isFatalError(int rc){
  return (rc!=SQLITE_OK && rc!=SQLITE_BUSY && ALWAYS(rc!=SQLITE_LOCKED));
}

static int backupOnePage(sqlite3_backup *p, Pgno iSrcPg, const u8 *zSrcData){
  Pager * const pDestPager = sqlite3BtreePager(p->pDest);
  const int nSrcPgsz = sqlite3BtreeGetPageSize(p->pSrc);
  int nDestPgsz = sqlite3BtreeGetPageSize(p->pDest);
  const int nCopy = MIN(nSrcPgsz, nDestPgsz);
  const i64 iEnd = (i64)iSrcPg*(i64)nSrcPgsz;
#ifdef SQLITE_HAS_CODEC
  int nSrcReserve = sqlite3BtreeGetReserve(p->pSrc);
  int nDestReserve = sqlite3BtreeGetReserve(p->pDest);
#endif

  int rc = SQLITE_OK;
  i64 iOff;

  assert( p->bDestLocked );
  assert( !isFatalError(p->rc) );
  assert( iSrcPg!=PENDING_BYTE_PAGE(p->pSrc->pBt) );
  assert( zSrcData );

  if( nSrcPgsz!=nDestPgsz && sqlite3PagerIsMemdb(pDestPager) ){
    rc = SQLITE_READONLY;
  }

#ifdef SQLITE_HAS_CODEC
  if( nSrcPgsz!=nDestPgsz && sqlite3PagerGetCodec(pDestPager)!=0 ){
    rc = SQLITE_READONLY;
  }

  if( nSrcReserve!=nDestReserve ){
    u32 newPgsz = nSrcPgsz;
    rc = sqlite3PagerSetPagesize(pDestPager, &newPgsz, nSrcReserve);
    if( rc==SQLITE_OK && newPgsz!=nSrcPgsz ) rc = SQLITE_READONLY;
  }
#endif

  for(iOff=iEnd-(i64)nSrcPgsz; rc==SQLITE_OK && iOff<iEnd; iOff+=nDestPgsz){
    DbPage *pDestPg = 0;
    Pgno iDest = (Pgno)(iOff/nDestPgsz)+1;
    if( iDest==PENDING_BYTE_PAGE(p->pDest->pBt) ) continue;
    if( SQLITE_OK==(rc = sqlite3PagerGet(pDestPager, iDest, &pDestPg))
     && SQLITE_OK==(rc = sqlite3PagerWrite(pDestPg))
    ){
      const u8 *zIn = &zSrcData[iOff%nSrcPgsz];
      u8 *zDestData = sqlite3PagerGetData(pDestPg);
      u8 *zOut = &zDestData[iOff%nDestPgsz];

      memcpy(zOut, zIn, nCopy);
      ((u8 *)sqlite3PagerGetExtra(pDestPg))[0] = 0;
    }
    sqlite3PagerUnref(pDestPg);
  }

  return rc;
}

static int backupTruncateFile(sqlite3_file *pFile, i64 iSize){
  i64 iCurrent;
  int rc = sqlite3OsFileSize(pFile, &iCurrent);
  if( rc==SQLITE_OK && iCurrent>iSize ){
    rc = sqlite3OsTruncate(pFile, iSize);
  }
  return rc;
}

static void attachBackupObject(sqlite3_backup *p){
  sqlite3_backup **pp;
  assert( sqlite3BtreeHoldsMutex(p->pSrc) );
  pp = sqlite3PagerBackupPtr(sqlite3BtreePager(p->pSrc));
  p->pNext = *pp;
  *pp = p;
  p->isAttached = 1;
}

int sqlite3_backup_step(sqlite3_backup *p, int nPage){
  int rc;
  int destMode;       
  int pgszSrc = 0;    
  int pgszDest = 0;   

  sqlite3_mutex_enter(p->pSrcDb->mutex);
  sqlite3BtreeEnter(p->pSrc);
  if( p->pDestDb ){
    sqlite3_mutex_enter(p->pDestDb->mutex);
  }

  rc = p->rc;
  if( !isFatalError(rc) ){
    Pager * const pSrcPager = sqlite3BtreePager(p->pSrc);     
    Pager * const pDestPager = sqlite3BtreePager(p->pDest);   
    int ii;                            
    int nSrcPage = -1;                 
    int bCloseTrans = 0;               

    if( p->pDestDb && p->pSrc->pBt->inTransaction==TRANS_WRITE ){
      rc = SQLITE_BUSY;
    }else{
      rc = SQLITE_OK;
    }

    
    if( SQLITE_OK==rc && p->bDestLocked==0
     && SQLITE_OK==(rc = sqlite3BtreeBeginTrans(p->pDest, 2)) 
    ){
      p->bDestLocked = 1;
      sqlite3BtreeGetMeta(p->pDest, BTREE_SCHEMA_VERSION, &p->iDestSchema);
    }

    if( rc==SQLITE_OK && 0==sqlite3BtreeIsInReadTrans(p->pSrc) ){
      rc = sqlite3BtreeBeginTrans(p->pSrc, 0);
      bCloseTrans = 1;
    }

    pgszSrc = sqlite3BtreeGetPageSize(p->pSrc);
    pgszDest = sqlite3BtreeGetPageSize(p->pDest);
    destMode = sqlite3PagerGetJournalMode(sqlite3BtreePager(p->pDest));
    if( SQLITE_OK==rc && destMode==PAGER_JOURNALMODE_WAL && pgszSrc!=pgszDest ){
      rc = SQLITE_READONLY;
    }
  
    nSrcPage = (int)sqlite3BtreeLastPage(p->pSrc);
    assert( nSrcPage>=0 );
    for(ii=0; (nPage<0 || ii<nPage) && p->iNext<=(Pgno)nSrcPage && !rc; ii++){
      const Pgno iSrcPg = p->iNext;                 
      if( iSrcPg!=PENDING_BYTE_PAGE(p->pSrc->pBt) ){
        DbPage *pSrcPg;                             
        rc = sqlite3PagerGet(pSrcPager, iSrcPg, &pSrcPg);
        if( rc==SQLITE_OK ){
          rc = backupOnePage(p, iSrcPg, sqlite3PagerGetData(pSrcPg));
          sqlite3PagerUnref(pSrcPg);
        }
      }
      p->iNext++;
    }
    if( rc==SQLITE_OK ){
      p->nPagecount = nSrcPage;
      p->nRemaining = nSrcPage+1-p->iNext;
      if( p->iNext>(Pgno)nSrcPage ){
        rc = SQLITE_DONE;
      }else if( !p->isAttached ){
        attachBackupObject(p);
      }
    }
  
    if( rc==SQLITE_DONE 
     && (rc = sqlite3BtreeUpdateMeta(p->pDest,1,p->iDestSchema+1))==SQLITE_OK
    ){
      int nDestTruncate;
  
      if( p->pDestDb ){
        sqlite3ResetInternalSchema(p->pDestDb, -1);
      }

      assert( pgszSrc==sqlite3BtreeGetPageSize(p->pSrc) );
      assert( pgszDest==sqlite3BtreeGetPageSize(p->pDest) );
      if( pgszSrc<pgszDest ){
        int ratio = pgszDest/pgszSrc;
        nDestTruncate = (nSrcPage+ratio-1)/ratio;
        if( nDestTruncate==(int)PENDING_BYTE_PAGE(p->pDest->pBt) ){
          nDestTruncate--;
        }
      }else{
        nDestTruncate = nSrcPage * (pgszSrc/pgszDest);
      }
      sqlite3PagerTruncateImage(pDestPager, nDestTruncate);

      if( pgszSrc<pgszDest ){
        const i64 iSize = (i64)pgszSrc * (i64)nSrcPage;
        sqlite3_file * const pFile = sqlite3PagerFile(pDestPager);
        i64 iOff;
        i64 iEnd;

        assert( pFile );
        assert( (i64)nDestTruncate*(i64)pgszDest >= iSize || (
              nDestTruncate==(int)(PENDING_BYTE_PAGE(p->pDest->pBt)-1)
           && iSize>=PENDING_BYTE && iSize<=PENDING_BYTE+pgszDest
        ));

        rc = sqlite3PagerCommitPhaseOne(pDestPager, 0, 1);

        
        iEnd = MIN(PENDING_BYTE + pgszDest, iSize);
        for(
          iOff=PENDING_BYTE+pgszSrc; 
          rc==SQLITE_OK && iOff<iEnd; 
          iOff+=pgszSrc
        ){
          PgHdr *pSrcPg = 0;
          const Pgno iSrcPg = (Pgno)((iOff/pgszSrc)+1);
          rc = sqlite3PagerGet(pSrcPager, iSrcPg, &pSrcPg);
          if( rc==SQLITE_OK ){
            u8 *zData = sqlite3PagerGetData(pSrcPg);
            rc = sqlite3OsWrite(pFile, zData, pgszSrc, iOff);
          }
          sqlite3PagerUnref(pSrcPg);
        }
        if( rc==SQLITE_OK ){
          rc = backupTruncateFile(pFile, iSize);
        }

        
        if( rc==SQLITE_OK ){
          rc = sqlite3PagerSync(pDestPager);
        }
      }else{
        rc = sqlite3PagerCommitPhaseOne(pDestPager, 0, 0);
      }
  
      
      if( SQLITE_OK==rc
       && SQLITE_OK==(rc = sqlite3BtreeCommitPhaseTwo(p->pDest, 0))
      ){
        rc = SQLITE_DONE;
      }
    }
  
    if( bCloseTrans ){
      TESTONLY( int rc2 );
      TESTONLY( rc2  = ) sqlite3BtreeCommitPhaseOne(p->pSrc, 0);
      TESTONLY( rc2 |= ) sqlite3BtreeCommitPhaseTwo(p->pSrc, 0);
      assert( rc2==SQLITE_OK );
    }
  
    if( rc==SQLITE_IOERR_NOMEM ){
      rc = SQLITE_NOMEM;
    }
    p->rc = rc;
  }
  if( p->pDestDb ){
    sqlite3_mutex_leave(p->pDestDb->mutex);
  }
  sqlite3BtreeLeave(p->pSrc);
  sqlite3_mutex_leave(p->pSrcDb->mutex);
  return rc;
}

int sqlite3_backup_finish(sqlite3_backup *p){
  sqlite3_backup **pp;                 
  sqlite3_mutex *mutex;                
  int rc;                              

  
  if( p==0 ) return SQLITE_OK;
  sqlite3_mutex_enter(p->pSrcDb->mutex);
  sqlite3BtreeEnter(p->pSrc);
  mutex = p->pSrcDb->mutex;
  if( p->pDestDb ){
    sqlite3_mutex_enter(p->pDestDb->mutex);
  }

  
  if( p->pDestDb ){
    p->pSrc->nBackup--;
  }
  if( p->isAttached ){
    pp = sqlite3PagerBackupPtr(sqlite3BtreePager(p->pSrc));
    while( *pp!=p ){
      pp = &(*pp)->pNext;
    }
    *pp = p->pNext;
  }

  
  sqlite3BtreeRollback(p->pDest);

  
  rc = (p->rc==SQLITE_DONE) ? SQLITE_OK : p->rc;
  sqlite3Error(p->pDestDb, rc, 0);

  
  if( p->pDestDb ){
    sqlite3_mutex_leave(p->pDestDb->mutex);
  }
  sqlite3BtreeLeave(p->pSrc);
  if( p->pDestDb ){
    sqlite3_free(p);
  }
  sqlite3_mutex_leave(mutex);
  return rc;
}

int sqlite3_backup_remaining(sqlite3_backup *p){
  return p->nRemaining;
}

int sqlite3_backup_pagecount(sqlite3_backup *p){
  return p->nPagecount;
}

/*
** This function is called after the contents of page iPage of the
** source database have been modified. If page iPage has already been 
** copied into the destination database, then the data written to the
** destination is now invalidated. The destination copy of iPage needs
** to be updated with the new data before the backup operation is
** complete.
**
** It is assumed that the mutex associated with the BtShared object
** corresponding to the source database is held when this function is
** called.
*/
void sqlite3BackupUpdate(sqlite3_backup *pBackup, Pgno iPage, const u8 *aData){
  sqlite3_backup *p;                   
  for(p=pBackup; p; p=p->pNext){
    assert( sqlite3_mutex_held(p->pSrc->pBt->mutex) );
    if( !isFatalError(p->rc) && iPage<p->iNext ){
      int rc;
      assert( p->pDestDb );
      sqlite3_mutex_enter(p->pDestDb->mutex);
      rc = backupOnePage(p, iPage, aData);
      sqlite3_mutex_leave(p->pDestDb->mutex);
      assert( rc!=SQLITE_BUSY && rc!=SQLITE_LOCKED );
      if( rc!=SQLITE_OK ){
        p->rc = rc;
      }
    }
  }
}

void sqlite3BackupRestart(sqlite3_backup *pBackup){
  sqlite3_backup *p;                   
  for(p=pBackup; p; p=p->pNext){
    assert( sqlite3_mutex_held(p->pSrc->pBt->mutex) );
    p->iNext = 1;
  }
}

#ifndef SQLITE_OMIT_VACUUM
int sqlite3BtreeCopyFile(Btree *pTo, Btree *pFrom){
  int rc;
  sqlite3_backup b;
  sqlite3BtreeEnter(pTo);
  sqlite3BtreeEnter(pFrom);

  memset(&b, 0, sizeof(b));
  b.pSrcDb = pFrom->db;
  b.pSrc = pFrom;
  b.pDest = pTo;
  b.iNext = 1;

  sqlite3_backup_step(&b, 0x7FFFFFFF);
  assert( b.rc!=SQLITE_OK );
  rc = sqlite3_backup_finish(&b);
  if( rc==SQLITE_OK ){
    pTo->pBt->pageSizeFixed = 0;
  }

  sqlite3BtreeLeave(pFrom);
  sqlite3BtreeLeave(pTo);
  return rc;
}
#endif 

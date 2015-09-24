/*
** 2004 April 6
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This file implements a external (disk-based) database using BTrees.
** See the header comment on "btreeInt.h" for additional information.
** Including a description of file format and an overview of operation.
*/
#include "btreeInt.h"

static const char zMagicHeader[] = SQLITE_FILE_HEADER;

#if 0
int sqlite3BtreeTrace=1;  
# define TRACE(X)  if(sqlite3BtreeTrace){printf X;fflush(stdout);}
#else
# define TRACE(X)
#endif

#define get2byteNotZero(X)  (((((int)get2byte(X))-1)&0xffff)+1)

#ifndef SQLITE_OMIT_SHARED_CACHE
#ifdef SQLITE_TEST
BtShared *SQLITE_WSD sqlite3SharedCacheList = 0;
#else
static BtShared *SQLITE_WSD sqlite3SharedCacheList = 0;
#endif
#endif 

#ifndef SQLITE_OMIT_SHARED_CACHE
int sqlite3_enable_shared_cache(int enable){
  sqlite3GlobalConfig.sharedCacheEnabled = enable;
  return SQLITE_OK;
}
#endif



#ifdef SQLITE_OMIT_SHARED_CACHE
  #define querySharedCacheTableLock(a,b,c) SQLITE_OK
  #define setSharedCacheTableLock(a,b,c) SQLITE_OK
  #define clearAllSharedCacheTableLocks(a)
  #define downgradeAllSharedCacheTableLocks(a)
  #define hasSharedCacheTableLock(a,b,c,d) 1
  #define hasReadConflicts(a, b) 0
#endif

#ifndef SQLITE_OMIT_SHARED_CACHE

#ifdef SQLITE_DEBUG
/*
**** This function is only used as part of an assert() statement. ***
**
** Check to see if pBtree holds the required locks to read or write to the 
** table with root page iRoot.   Return 1 if it does and 0 if not.
**
** For example, when writing to a table with root-page iRoot via 
** Btree connection pBtree:
**
**    assert( hasSharedCacheTableLock(pBtree, iRoot, 0, WRITE_LOCK) );
**
** When writing to an index that resides in a sharable database, the 
** caller should have first obtained a lock specifying the root page of
** the corresponding table. This makes things a bit more complicated,
** as this module treats each table as a separate structure. To determine
** the table corresponding to the index being written, this
** function has to search through the database schema.
**
** Instead of a lock on the table/index rooted at page iRoot, the caller may
** hold a write-lock on the schema table (root page 1). This is also
** acceptable.
*/
static int hasSharedCacheTableLock(
  Btree *pBtree,         
  Pgno iRoot,            
  int isIndex,           
  int eLockType          
){
  Schema *pSchema = (Schema *)pBtree->pBt->pSchema;
  Pgno iTab = 0;
  BtLock *pLock;

  if( (pBtree->sharable==0)
   || (eLockType==READ_LOCK && (pBtree->db->flags & SQLITE_ReadUncommitted))
  ){
    return 1;
  }

  if( isIndex && (!pSchema || (pSchema->flags&DB_SchemaLoaded)==0) ){
    return 1;
  }

  /* Figure out the root-page that the lock should be held on. For table
  ** b-trees, this is just the root page of the b-tree being read or
  ** written. For index b-trees, it is the root page of the associated
  ** table.  */
  if( isIndex ){
    HashElem *p;
    for(p=sqliteHashFirst(&pSchema->idxHash); p; p=sqliteHashNext(p)){
      Index *pIdx = (Index *)sqliteHashData(p);
      if( pIdx->tnum==(int)iRoot ){
        iTab = pIdx->pTable->tnum;
      }
    }
  }else{
    iTab = iRoot;
  }

  for(pLock=pBtree->pBt->pLock; pLock; pLock=pLock->pNext){
    if( pLock->pBtree==pBtree 
     && (pLock->iTable==iTab || (pLock->eLock==WRITE_LOCK && pLock->iTable==1))
     && pLock->eLock>=eLockType 
    ){
      return 1;
    }
  }

  
  return 0;
}
#endif 

#ifdef SQLITE_DEBUG
static int hasReadConflicts(Btree *pBtree, Pgno iRoot){
  BtCursor *p;
  for(p=pBtree->pBt->pCursor; p; p=p->pNext){
    if( p->pgnoRoot==iRoot 
     && p->pBtree!=pBtree
     && 0==(p->pBtree->db->flags & SQLITE_ReadUncommitted)
    ){
      return 1;
    }
  }
  return 0;
}
#endif    

static int querySharedCacheTableLock(Btree *p, Pgno iTab, u8 eLock){
  BtShared *pBt = p->pBt;
  BtLock *pIter;

  assert( sqlite3BtreeHoldsMutex(p) );
  assert( eLock==READ_LOCK || eLock==WRITE_LOCK );
  assert( p->db!=0 );
  assert( !(p->db->flags&SQLITE_ReadUncommitted)||eLock==WRITE_LOCK||iTab==1 );
  
  assert( eLock==READ_LOCK || (p==pBt->pWriter && p->inTrans==TRANS_WRITE) );
  assert( eLock==READ_LOCK || pBt->inTransaction==TRANS_WRITE );
  
  
  if( !p->sharable ){
    return SQLITE_OK;
  }

  if( pBt->pWriter!=p && pBt->isExclusive ){
    sqlite3ConnectionBlocked(p->db, pBt->pWriter->db);
    return SQLITE_LOCKED_SHAREDCACHE;
  }

  for(pIter=pBt->pLock; pIter; pIter=pIter->pNext){
    assert( pIter->eLock==READ_LOCK || pIter->eLock==WRITE_LOCK );
    assert( eLock==READ_LOCK || pIter->pBtree==p || pIter->eLock==READ_LOCK);
    if( pIter->pBtree!=p && pIter->iTable==iTab && pIter->eLock!=eLock ){
      sqlite3ConnectionBlocked(p->db, pIter->pBtree->db);
      if( eLock==WRITE_LOCK ){
        assert( p==pBt->pWriter );
        pBt->isPending = 1;
      }
      return SQLITE_LOCKED_SHAREDCACHE;
    }
  }
  return SQLITE_OK;
}
#endif 

#ifndef SQLITE_OMIT_SHARED_CACHE
static int setSharedCacheTableLock(Btree *p, Pgno iTable, u8 eLock){
  BtShared *pBt = p->pBt;
  BtLock *pLock = 0;
  BtLock *pIter;

  assert( sqlite3BtreeHoldsMutex(p) );
  assert( eLock==READ_LOCK || eLock==WRITE_LOCK );
  assert( p->db!=0 );

  assert( 0==(p->db->flags&SQLITE_ReadUncommitted) || eLock==WRITE_LOCK );

  assert( p->sharable );
  assert( SQLITE_OK==querySharedCacheTableLock(p, iTable, eLock) );

  
  for(pIter=pBt->pLock; pIter; pIter=pIter->pNext){
    if( pIter->iTable==iTable && pIter->pBtree==p ){
      pLock = pIter;
      break;
    }
  }

  if( !pLock ){
    pLock = (BtLock *)sqlite3MallocZero(sizeof(BtLock));
    if( !pLock ){
      return SQLITE_NOMEM;
    }
    pLock->iTable = iTable;
    pLock->pBtree = p;
    pLock->pNext = pBt->pLock;
    pBt->pLock = pLock;
  }

  assert( WRITE_LOCK>READ_LOCK );
  if( eLock>pLock->eLock ){
    pLock->eLock = eLock;
  }

  return SQLITE_OK;
}
#endif 

#ifndef SQLITE_OMIT_SHARED_CACHE
static void clearAllSharedCacheTableLocks(Btree *p){
  BtShared *pBt = p->pBt;
  BtLock **ppIter = &pBt->pLock;

  assert( sqlite3BtreeHoldsMutex(p) );
  assert( p->sharable || 0==*ppIter );
  assert( p->inTrans>0 );

  while( *ppIter ){
    BtLock *pLock = *ppIter;
    assert( pBt->isExclusive==0 || pBt->pWriter==pLock->pBtree );
    assert( pLock->pBtree->inTrans>=pLock->eLock );
    if( pLock->pBtree==p ){
      *ppIter = pLock->pNext;
      assert( pLock->iTable!=1 || pLock==&p->lock );
      if( pLock->iTable!=1 ){
        sqlite3_free(pLock);
      }
    }else{
      ppIter = &pLock->pNext;
    }
  }

  assert( pBt->isPending==0 || pBt->pWriter );
  if( pBt->pWriter==p ){
    pBt->pWriter = 0;
    pBt->isExclusive = 0;
    pBt->isPending = 0;
  }else if( pBt->nTransaction==2 ){
    pBt->isPending = 0;
  }
}

static void downgradeAllSharedCacheTableLocks(Btree *p){
  BtShared *pBt = p->pBt;
  if( pBt->pWriter==p ){
    BtLock *pLock;
    pBt->pWriter = 0;
    pBt->isExclusive = 0;
    pBt->isPending = 0;
    for(pLock=pBt->pLock; pLock; pLock=pLock->pNext){
      assert( pLock->eLock==READ_LOCK || pLock->pBtree==p );
      pLock->eLock = READ_LOCK;
    }
  }
}

#endif 

static void releasePage(MemPage *pPage);  

#ifdef SQLITE_DEBUG
static int cursorHoldsMutex(BtCursor *p){
  return sqlite3_mutex_held(p->pBt->mutex);
}
#endif


#ifndef SQLITE_OMIT_INCRBLOB
static void invalidateOverflowCache(BtCursor *pCur){
  assert( cursorHoldsMutex(pCur) );
  sqlite3_free(pCur->aOverflow);
  pCur->aOverflow = 0;
}

static void invalidateAllOverflowCache(BtShared *pBt){
  BtCursor *p;
  assert( sqlite3_mutex_held(pBt->mutex) );
  for(p=pBt->pCursor; p; p=p->pNext){
    invalidateOverflowCache(p);
  }
}

static void invalidateIncrblobCursors(
  Btree *pBtree,          
  i64 iRow,               
  int isClearTable        
){
  BtCursor *p;
  BtShared *pBt = pBtree->pBt;
  assert( sqlite3BtreeHoldsMutex(pBtree) );
  for(p=pBt->pCursor; p; p=p->pNext){
    if( p->isIncrblobHandle && (isClearTable || p->info.nKey==iRow) ){
      p->eState = CURSOR_INVALID;
    }
  }
}

#else
  
  #define invalidateOverflowCache(x)
  #define invalidateAllOverflowCache(x)
  #define invalidateIncrblobCursors(x,y,z)
#endif 

/*
** Set bit pgno of the BtShared.pHasContent bitvec. This is called 
** when a page that previously contained data becomes a free-list leaf 
** page.
**
** The BtShared.pHasContent bitvec exists to work around an obscure
** bug caused by the interaction of two useful IO optimizations surrounding
** free-list leaf pages:
**
**   1) When all data is deleted from a page and the page becomes
**      a free-list leaf page, the page is not written to the database
**      (as free-list leaf pages contain no meaningful data). Sometimes
**      such a page is not even journalled (as it will not be modified,
**      why bother journalling it?).
**
**   2) When a free-list leaf page is reused, its content is not read
**      from the database or written to the journal file (why should it
**      be, if it is not at all meaningful?).
**
** By themselves, these optimizations work fine and provide a handy
** performance boost to bulk delete or insert operations. However, if
** a page is moved to the free-list and then reused within the same
** transaction, a problem comes up. If the page is not journalled when
** it is moved to the free-list and it is also not journalled when it
** is extracted from the free-list and reused, then the original data
** may be lost. In the event of a rollback, it may not be possible
** to restore the database to its original configuration.
**
** The solution is the BtShared.pHasContent bitvec. Whenever a page is 
** moved to become a free-list leaf page, the corresponding bit is
** set in the bitvec. Whenever a leaf page is extracted from the free-list,
** optimization 2 above is omitted if the corresponding bit is already
** set in BtShared.pHasContent. The contents of the bitvec are cleared
** at the end of every transaction.
*/
static int btreeSetHasContent(BtShared *pBt, Pgno pgno){
  int rc = SQLITE_OK;
  if( !pBt->pHasContent ){
    assert( pgno<=pBt->nPage );
    pBt->pHasContent = sqlite3BitvecCreate(pBt->nPage);
    if( !pBt->pHasContent ){
      rc = SQLITE_NOMEM;
    }
  }
  if( rc==SQLITE_OK && pgno<=sqlite3BitvecSize(pBt->pHasContent) ){
    rc = sqlite3BitvecSet(pBt->pHasContent, pgno);
  }
  return rc;
}

static int btreeGetHasContent(BtShared *pBt, Pgno pgno){
  Bitvec *p = pBt->pHasContent;
  return (p && (pgno>sqlite3BitvecSize(p) || sqlite3BitvecTest(p, pgno)));
}

static void btreeClearHasContent(BtShared *pBt){
  sqlite3BitvecDestroy(pBt->pHasContent);
  pBt->pHasContent = 0;
}

static int saveCursorPosition(BtCursor *pCur){
  int rc;

  assert( CURSOR_VALID==pCur->eState );
  assert( 0==pCur->pKey );
  assert( cursorHoldsMutex(pCur) );

  rc = sqlite3BtreeKeySize(pCur, &pCur->nKey);
  assert( rc==SQLITE_OK );  

  if( 0==pCur->apPage[0]->intKey ){
    void *pKey = sqlite3Malloc( (int)pCur->nKey );
    if( pKey ){
      rc = sqlite3BtreeKey(pCur, 0, (int)pCur->nKey, pKey);
      if( rc==SQLITE_OK ){
        pCur->pKey = pKey;
      }else{
        sqlite3_free(pKey);
      }
    }else{
      rc = SQLITE_NOMEM;
    }
  }
  assert( !pCur->apPage[0]->intKey || !pCur->pKey );

  if( rc==SQLITE_OK ){
    int i;
    for(i=0; i<=pCur->iPage; i++){
      releasePage(pCur->apPage[i]);
      pCur->apPage[i] = 0;
    }
    pCur->iPage = -1;
    pCur->eState = CURSOR_REQUIRESEEK;
  }

  invalidateOverflowCache(pCur);
  return rc;
}

static int saveAllCursors(BtShared *pBt, Pgno iRoot, BtCursor *pExcept){
  BtCursor *p;
  assert( sqlite3_mutex_held(pBt->mutex) );
  assert( pExcept==0 || pExcept->pBt==pBt );
  for(p=pBt->pCursor; p; p=p->pNext){
    if( p!=pExcept && (0==iRoot || p->pgnoRoot==iRoot) && 
        p->eState==CURSOR_VALID ){
      int rc = saveCursorPosition(p);
      if( SQLITE_OK!=rc ){
        return rc;
      }
    }
  }
  return SQLITE_OK;
}

void sqlite3BtreeClearCursor(BtCursor *pCur){
  assert( cursorHoldsMutex(pCur) );
  sqlite3_free(pCur->pKey);
  pCur->pKey = 0;
  pCur->eState = CURSOR_INVALID;
}

static int btreeMoveto(
  BtCursor *pCur,     
  const void *pKey,   
  i64 nKey,           
  int bias,           
  int *pRes           
){
  int rc;                    
  UnpackedRecord *pIdxKey;   
  char aSpace[150];          

  if( pKey ){
    assert( nKey==(i64)(int)nKey );
    pIdxKey = sqlite3VdbeRecordUnpack(pCur->pKeyInfo, (int)nKey, pKey,
                                      aSpace, sizeof(aSpace));
    if( pIdxKey==0 ) return SQLITE_NOMEM;
  }else{
    pIdxKey = 0;
  }
  rc = sqlite3BtreeMovetoUnpacked(pCur, pIdxKey, nKey, bias, pRes);
  if( pKey ){
    sqlite3VdbeDeleteUnpackedRecord(pIdxKey);
  }
  return rc;
}

static int btreeRestoreCursorPosition(BtCursor *pCur){
  int rc;
  assert( cursorHoldsMutex(pCur) );
  assert( pCur->eState>=CURSOR_REQUIRESEEK );
  if( pCur->eState==CURSOR_FAULT ){
    return pCur->skipNext;
  }
  pCur->eState = CURSOR_INVALID;
  rc = btreeMoveto(pCur, pCur->pKey, pCur->nKey, 0, &pCur->skipNext);
  if( rc==SQLITE_OK ){
    sqlite3_free(pCur->pKey);
    pCur->pKey = 0;
    assert( pCur->eState==CURSOR_VALID || pCur->eState==CURSOR_INVALID );
  }
  return rc;
}

#define restoreCursorPosition(p) \
  (p->eState>=CURSOR_REQUIRESEEK ? \
         btreeRestoreCursorPosition(p) : \
         SQLITE_OK)

int sqlite3BtreeCursorHasMoved(BtCursor *pCur, int *pHasMoved){
  int rc;

  rc = restoreCursorPosition(pCur);
  if( rc ){
    *pHasMoved = 1;
    return rc;
  }
  if( pCur->eState!=CURSOR_VALID || pCur->skipNext!=0 ){
    *pHasMoved = 1;
  }else{
    *pHasMoved = 0;
  }
  return SQLITE_OK;
}

#ifndef SQLITE_OMIT_AUTOVACUUM
static Pgno ptrmapPageno(BtShared *pBt, Pgno pgno){
  int nPagesPerMapPage;
  Pgno iPtrMap, ret;
  assert( sqlite3_mutex_held(pBt->mutex) );
  if( pgno<2 ) return 0;
  nPagesPerMapPage = (pBt->usableSize/5)+1;
  iPtrMap = (pgno-2)/nPagesPerMapPage;
  ret = (iPtrMap*nPagesPerMapPage) + 2; 
  if( ret==PENDING_BYTE_PAGE(pBt) ){
    ret++;
  }
  return ret;
}

/*
** Write an entry into the pointer map.
**
** This routine updates the pointer map entry for page number 'key'
** so that it maps to type 'eType' and parent page number 'pgno'.
**
** If *pRC is initially non-zero (non-SQLITE_OK) then this routine is
** a no-op.  If an error occurs, the appropriate error code is written
** into *pRC.
*/
static void ptrmapPut(BtShared *pBt, Pgno key, u8 eType, Pgno parent, int *pRC){
  DbPage *pDbPage;  
  u8 *pPtrmap;      
  Pgno iPtrmap;     
  int offset;       
  int rc;           

  if( *pRC ) return;

  assert( sqlite3_mutex_held(pBt->mutex) );
  
  assert( 0==PTRMAP_ISPAGE(pBt, PENDING_BYTE_PAGE(pBt)) );

  assert( pBt->autoVacuum );
  if( key==0 ){
    *pRC = SQLITE_CORRUPT_BKPT;
    return;
  }
  iPtrmap = PTRMAP_PAGENO(pBt, key);
  rc = sqlite3PagerGet(pBt->pPager, iPtrmap, &pDbPage);
  if( rc!=SQLITE_OK ){
    *pRC = rc;
    return;
  }
  offset = PTRMAP_PTROFFSET(iPtrmap, key);
  if( offset<0 ){
    *pRC = SQLITE_CORRUPT_BKPT;
    goto ptrmap_exit;
  }
  pPtrmap = (u8 *)sqlite3PagerGetData(pDbPage);

  if( eType!=pPtrmap[offset] || get4byte(&pPtrmap[offset+1])!=parent ){
    TRACE(("PTRMAP_UPDATE: %d->(%d,%d)\n", key, eType, parent));
    *pRC= rc = sqlite3PagerWrite(pDbPage);
    if( rc==SQLITE_OK ){
      pPtrmap[offset] = eType;
      put4byte(&pPtrmap[offset+1], parent);
    }
  }

ptrmap_exit:
  sqlite3PagerUnref(pDbPage);
}

static int ptrmapGet(BtShared *pBt, Pgno key, u8 *pEType, Pgno *pPgno){
  DbPage *pDbPage;   
  int iPtrmap;       
  u8 *pPtrmap;       
  int offset;        
  int rc;

  assert( sqlite3_mutex_held(pBt->mutex) );

  iPtrmap = PTRMAP_PAGENO(pBt, key);
  rc = sqlite3PagerGet(pBt->pPager, iPtrmap, &pDbPage);
  if( rc!=0 ){
    return rc;
  }
  pPtrmap = (u8 *)sqlite3PagerGetData(pDbPage);

  offset = PTRMAP_PTROFFSET(iPtrmap, key);
  assert( pEType!=0 );
  *pEType = pPtrmap[offset];
  if( pPgno ) *pPgno = get4byte(&pPtrmap[offset+1]);

  sqlite3PagerUnref(pDbPage);
  if( *pEType<1 || *pEType>5 ) return SQLITE_CORRUPT_BKPT;
  return SQLITE_OK;
}

#else 
  #define ptrmapPut(w,x,y,z,rc)
  #define ptrmapGet(w,x,y,z) SQLITE_OK
  #define ptrmapPutOvflPtr(x, y, rc)
#endif

#define findCell(P,I) \
  ((P)->aData + ((P)->maskPage & get2byte(&(P)->aData[(P)->cellOffset+2*(I)])))

static u8 *findOverflowCell(MemPage *pPage, int iCell){
  int i;
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  for(i=pPage->nOverflow-1; i>=0; i--){
    int k;
    struct _OvflCell *pOvfl;
    pOvfl = &pPage->aOvfl[i];
    k = pOvfl->idx;
    if( k<=iCell ){
      if( k==iCell ){
        return pOvfl->pCell;
      }
      iCell--;
    }
  }
  return findCell(pPage, iCell);
}

static void btreeParseCellPtr(
  MemPage *pPage,         
  u8 *pCell,              
  CellInfo *pInfo         
){
  u16 n;                  
  u32 nPayload;           

  assert( sqlite3_mutex_held(pPage->pBt->mutex) );

  pInfo->pCell = pCell;
  assert( pPage->leaf==0 || pPage->leaf==1 );
  n = pPage->childPtrSize;
  assert( n==4-4*pPage->leaf );
  if( pPage->intKey ){
    if( pPage->hasData ){
      n += getVarint32(&pCell[n], nPayload);
    }else{
      nPayload = 0;
    }
    n += getVarint(&pCell[n], (u64*)&pInfo->nKey);
    pInfo->nData = nPayload;
  }else{
    pInfo->nData = 0;
    n += getVarint32(&pCell[n], nPayload);
    pInfo->nKey = nPayload;
  }
  pInfo->nPayload = nPayload;
  pInfo->nHeader = n;
  testcase( nPayload==pPage->maxLocal );
  testcase( nPayload==pPage->maxLocal+1 );
  if( likely(nPayload<=pPage->maxLocal) ){
    if( (pInfo->nSize = (u16)(n+nPayload))<4 ) pInfo->nSize = 4;
    pInfo->nLocal = (u16)nPayload;
    pInfo->iOverflow = 0;
  }else{
    int minLocal;  
    int maxLocal;  
    int surplus;   

    minLocal = pPage->minLocal;
    maxLocal = pPage->maxLocal;
    surplus = minLocal + (nPayload - minLocal)%(pPage->pBt->usableSize - 4);
    testcase( surplus==maxLocal );
    testcase( surplus==maxLocal+1 );
    if( surplus <= maxLocal ){
      pInfo->nLocal = (u16)surplus;
    }else{
      pInfo->nLocal = (u16)minLocal;
    }
    pInfo->iOverflow = (u16)(pInfo->nLocal + n);
    pInfo->nSize = pInfo->iOverflow + 4;
  }
}
#define parseCell(pPage, iCell, pInfo) \
  btreeParseCellPtr((pPage), findCell((pPage), (iCell)), (pInfo))
static void btreeParseCell(
  MemPage *pPage,         
  int iCell,              
  CellInfo *pInfo         
){
  parseCell(pPage, iCell, pInfo);
}

static u16 cellSizePtr(MemPage *pPage, u8 *pCell){
  u8 *pIter = &pCell[pPage->childPtrSize];
  u32 nSize;

#ifdef SQLITE_DEBUG
  CellInfo debuginfo;
  btreeParseCellPtr(pPage, pCell, &debuginfo);
#endif

  if( pPage->intKey ){
    u8 *pEnd;
    if( pPage->hasData ){
      pIter += getVarint32(pIter, nSize);
    }else{
      nSize = 0;
    }

    pEnd = &pIter[9];
    while( (*pIter++)&0x80 && pIter<pEnd );
  }else{
    pIter += getVarint32(pIter, nSize);
  }

  testcase( nSize==pPage->maxLocal );
  testcase( nSize==pPage->maxLocal+1 );
  if( nSize>pPage->maxLocal ){
    int minLocal = pPage->minLocal;
    nSize = minLocal + (nSize - minLocal) % (pPage->pBt->usableSize - 4);
    testcase( nSize==pPage->maxLocal );
    testcase( nSize==pPage->maxLocal+1 );
    if( nSize>pPage->maxLocal ){
      nSize = minLocal;
    }
    nSize += 4;
  }
  nSize += (u32)(pIter - pCell);

  
  if( nSize<4 ){
    nSize = 4;
  }

  assert( nSize==debuginfo.nSize );
  return (u16)nSize;
}

#ifdef SQLITE_DEBUG
static u16 cellSize(MemPage *pPage, int iCell){
  return cellSizePtr(pPage, findCell(pPage, iCell));
}
#endif

#ifndef SQLITE_OMIT_AUTOVACUUM
static void ptrmapPutOvflPtr(MemPage *pPage, u8 *pCell, int *pRC){
  CellInfo info;
  if( *pRC ) return;
  assert( pCell!=0 );
  btreeParseCellPtr(pPage, pCell, &info);
  assert( (info.nData+(pPage->intKey?0:info.nKey))==info.nPayload );
  if( info.iOverflow ){
    Pgno ovfl = get4byte(&pCell[info.iOverflow]);
    ptrmapPut(pPage->pBt, ovfl, PTRMAP_OVERFLOW1, pPage->pgno, pRC);
  }
}
#endif


static int defragmentPage(MemPage *pPage){
  int i;                     
  int pc;                    
  int hdr;                   
  int size;                  
  int usableSize;            
  int cellOffset;            
  int cbrk;                  
  int nCell;                 
  unsigned char *data;       
  unsigned char *temp;       
  int iCellFirst;            
  int iCellLast;             


  assert( sqlite3PagerIswriteable(pPage->pDbPage) );
  assert( pPage->pBt!=0 );
  assert( pPage->pBt->usableSize <= SQLITE_MAX_PAGE_SIZE );
  assert( pPage->nOverflow==0 );
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  temp = sqlite3PagerTempSpace(pPage->pBt->pPager);
  data = pPage->aData;
  hdr = pPage->hdrOffset;
  cellOffset = pPage->cellOffset;
  nCell = pPage->nCell;
  assert( nCell==get2byte(&data[hdr+3]) );
  usableSize = pPage->pBt->usableSize;
  cbrk = get2byte(&data[hdr+5]);
  memcpy(&temp[cbrk], &data[cbrk], usableSize - cbrk);
  cbrk = usableSize;
  iCellFirst = cellOffset + 2*nCell;
  iCellLast = usableSize - 4;
  for(i=0; i<nCell; i++){
    u8 *pAddr;     
    pAddr = &data[cellOffset + i*2];
    pc = get2byte(pAddr);
    testcase( pc==iCellFirst );
    testcase( pc==iCellLast );
#if !defined(SQLITE_ENABLE_OVERSIZE_CELL_CHECK)
    if( pc<iCellFirst || pc>iCellLast ){
      return SQLITE_CORRUPT_BKPT;
    }
#endif
    assert( pc>=iCellFirst && pc<=iCellLast );
    size = cellSizePtr(pPage, &temp[pc]);
    cbrk -= size;
#if defined(SQLITE_ENABLE_OVERSIZE_CELL_CHECK)
    if( cbrk<iCellFirst ){
      return SQLITE_CORRUPT_BKPT;
    }
#else
    if( cbrk<iCellFirst || pc+size>usableSize ){
      return SQLITE_CORRUPT_BKPT;
    }
#endif
    assert( cbrk+size<=usableSize && cbrk>=iCellFirst );
    testcase( cbrk+size==usableSize );
    testcase( pc+size==usableSize );
    memcpy(&data[cbrk], &temp[pc], size);
    put2byte(pAddr, cbrk);
  }
  assert( cbrk>=iCellFirst );
  put2byte(&data[hdr+5], cbrk);
  data[hdr+1] = 0;
  data[hdr+2] = 0;
  data[hdr+7] = 0;
  memset(&data[iCellFirst], 0, cbrk-iCellFirst);
  assert( sqlite3PagerIswriteable(pPage->pDbPage) );
  if( cbrk-iCellFirst!=pPage->nFree ){
    return SQLITE_CORRUPT_BKPT;
  }
  return SQLITE_OK;
}

static int allocateSpace(MemPage *pPage, int nByte, int *pIdx){
  const int hdr = pPage->hdrOffset;    
  u8 * const data = pPage->aData;      
  int nFrag;                           
  int top;                             
  int gap;        
  int rc;         
  int usableSize; 
  
  assert( sqlite3PagerIswriteable(pPage->pDbPage) );
  assert( pPage->pBt );
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  assert( nByte>=0 );  
  assert( pPage->nFree>=nByte );
  assert( pPage->nOverflow==0 );
  usableSize = pPage->pBt->usableSize;
  assert( nByte < usableSize-8 );

  nFrag = data[hdr+7];
  assert( pPage->cellOffset == hdr + 12 - 4*pPage->leaf );
  gap = pPage->cellOffset + 2*pPage->nCell;
  top = get2byteNotZero(&data[hdr+5]);
  if( gap>top ) return SQLITE_CORRUPT_BKPT;
  testcase( gap+2==top );
  testcase( gap+1==top );
  testcase( gap==top );

  if( nFrag>=60 ){
    
    rc = defragmentPage(pPage);
    if( rc ) return rc;
    top = get2byteNotZero(&data[hdr+5]);
  }else if( gap+2<=top ){
    int pc, addr;
    for(addr=hdr+1; (pc = get2byte(&data[addr]))>0; addr=pc){
      int size;            
      if( pc>usableSize-4 || pc<addr+4 ){
        return SQLITE_CORRUPT_BKPT;
      }
      size = get2byte(&data[pc+2]);
      if( size>=nByte ){
        int x = size - nByte;
        testcase( x==4 );
        testcase( x==3 );
        if( x<4 ){
          memcpy(&data[addr], &data[pc], 2);
          data[hdr+7] = (u8)(nFrag + x);
        }else if( size+pc > usableSize ){
          return SQLITE_CORRUPT_BKPT;
        }else{
          put2byte(&data[pc+2], x);
        }
        *pIdx = pc + x;
        return SQLITE_OK;
      }
    }
  }

  testcase( gap+2+nByte==top );
  if( gap+2+nByte>top ){
    rc = defragmentPage(pPage);
    if( rc ) return rc;
    top = get2byteNotZero(&data[hdr+5]);
    assert( gap+nByte<=top );
  }


  top -= nByte;
  put2byte(&data[hdr+5], top);
  assert( top+nByte <= (int)pPage->pBt->usableSize );
  *pIdx = top;
  return SQLITE_OK;
}

static int freeSpace(MemPage *pPage, int start, int size){
  int addr, pbegin, hdr;
  int iLast;                        
  unsigned char *data = pPage->aData;

  assert( pPage->pBt!=0 );
  assert( sqlite3PagerIswriteable(pPage->pDbPage) );
  assert( start>=pPage->hdrOffset+6+pPage->childPtrSize );
  assert( (start + size) <= (int)pPage->pBt->usableSize );
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  assert( size>=0 );   

  if( pPage->pBt->secureDelete ){
    memset(&data[start], 0, size);
  }

  hdr = pPage->hdrOffset;
  addr = hdr + 1;
  iLast = pPage->pBt->usableSize - 4;
  assert( start<=iLast );
  while( (pbegin = get2byte(&data[addr]))<start && pbegin>0 ){
    if( pbegin<addr+4 ){
      return SQLITE_CORRUPT_BKPT;
    }
    addr = pbegin;
  }
  if( pbegin>iLast ){
    return SQLITE_CORRUPT_BKPT;
  }
  assert( pbegin>addr || pbegin==0 );
  put2byte(&data[addr], start);
  put2byte(&data[start], pbegin);
  put2byte(&data[start+2], size);
  pPage->nFree = pPage->nFree + (u16)size;

  
  addr = hdr + 1;
  while( (pbegin = get2byte(&data[addr]))>0 ){
    int pnext, psize, x;
    assert( pbegin>addr );
    assert( pbegin <= (int)pPage->pBt->usableSize-4 );
    pnext = get2byte(&data[pbegin]);
    psize = get2byte(&data[pbegin+2]);
    if( pbegin + psize + 3 >= pnext && pnext>0 ){
      int frag = pnext - (pbegin+psize);
      if( (frag<0) || (frag>(int)data[hdr+7]) ){
        return SQLITE_CORRUPT_BKPT;
      }
      data[hdr+7] -= (u8)frag;
      x = get2byte(&data[pnext]);
      put2byte(&data[pbegin], x);
      x = pnext + get2byte(&data[pnext+2]) - pbegin;
      put2byte(&data[pbegin+2], x);
    }else{
      addr = pbegin;
    }
  }

  
  if( data[hdr+1]==data[hdr+5] && data[hdr+2]==data[hdr+6] ){
    int top;
    pbegin = get2byte(&data[hdr+1]);
    memcpy(&data[hdr+1], &data[pbegin], 2);
    top = get2byte(&data[hdr+5]) + get2byte(&data[pbegin+2]);
    put2byte(&data[hdr+5], top);
  }
  assert( sqlite3PagerIswriteable(pPage->pDbPage) );
  return SQLITE_OK;
}

static int decodeFlags(MemPage *pPage, int flagByte){
  BtShared *pBt;     

  assert( pPage->hdrOffset==(pPage->pgno==1 ? 100 : 0) );
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  pPage->leaf = (u8)(flagByte>>3);  assert( PTF_LEAF == 1<<3 );
  flagByte &= ~PTF_LEAF;
  pPage->childPtrSize = 4-4*pPage->leaf;
  pBt = pPage->pBt;
  if( flagByte==(PTF_LEAFDATA | PTF_INTKEY) ){
    pPage->intKey = 1;
    pPage->hasData = pPage->leaf;
    pPage->maxLocal = pBt->maxLeaf;
    pPage->minLocal = pBt->minLeaf;
  }else if( flagByte==PTF_ZERODATA ){
    pPage->intKey = 0;
    pPage->hasData = 0;
    pPage->maxLocal = pBt->maxLocal;
    pPage->minLocal = pBt->minLocal;
  }else{
    return SQLITE_CORRUPT_BKPT;
  }
  return SQLITE_OK;
}

static int btreeInitPage(MemPage *pPage){

  assert( pPage->pBt!=0 );
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  assert( pPage->pgno==sqlite3PagerPagenumber(pPage->pDbPage) );
  assert( pPage == sqlite3PagerGetExtra(pPage->pDbPage) );
  assert( pPage->aData == sqlite3PagerGetData(pPage->pDbPage) );

  if( !pPage->isInit ){
    u16 pc;            
    u8 hdr;            
    u8 *data;          
    BtShared *pBt;        
    int usableSize;    
    u16 cellOffset;    
    int nFree;         
    int top;           
    int iCellFirst;    
    int iCellLast;     

    pBt = pPage->pBt;

    hdr = pPage->hdrOffset;
    data = pPage->aData;
    if( decodeFlags(pPage, data[hdr]) ) return SQLITE_CORRUPT_BKPT;
    assert( pBt->pageSize>=512 && pBt->pageSize<=65536 );
    pPage->maskPage = (u16)(pBt->pageSize - 1);
    pPage->nOverflow = 0;
    usableSize = pBt->usableSize;
    pPage->cellOffset = cellOffset = hdr + 12 - 4*pPage->leaf;
    top = get2byteNotZero(&data[hdr+5]);
    pPage->nCell = get2byte(&data[hdr+3]);
    if( pPage->nCell>MX_CELL(pBt) ){
      
      return SQLITE_CORRUPT_BKPT;
    }
    testcase( pPage->nCell==MX_CELL(pBt) );

    iCellFirst = cellOffset + 2*pPage->nCell;
    iCellLast = usableSize - 4;
#if defined(SQLITE_ENABLE_OVERSIZE_CELL_CHECK)
    {
      int i;            
      int sz;           

      if( !pPage->leaf ) iCellLast--;
      for(i=0; i<pPage->nCell; i++){
        pc = get2byte(&data[cellOffset+i*2]);
        testcase( pc==iCellFirst );
        testcase( pc==iCellLast );
        if( pc<iCellFirst || pc>iCellLast ){
          return SQLITE_CORRUPT_BKPT;
        }
        sz = cellSizePtr(pPage, &data[pc]);
        testcase( pc+sz==usableSize );
        if( pc+sz>usableSize ){
          return SQLITE_CORRUPT_BKPT;
        }
      }
      if( !pPage->leaf ) iCellLast++;
    }  
#endif

    
    pc = get2byte(&data[hdr+1]);
    nFree = data[hdr+7] + top;
    while( pc>0 ){
      u16 next, size;
      if( pc<iCellFirst || pc>iCellLast ){
        
        return SQLITE_CORRUPT_BKPT; 
      }
      next = get2byte(&data[pc]);
      size = get2byte(&data[pc+2]);
      if( (next>0 && next<=pc+size+3) || pc+size>usableSize ){
        return SQLITE_CORRUPT_BKPT; 
      }
      nFree = nFree + size;
      pc = next;
    }

    if( nFree>usableSize ){
      return SQLITE_CORRUPT_BKPT; 
    }
    pPage->nFree = (u16)(nFree - iCellFirst);
    pPage->isInit = 1;
  }
  return SQLITE_OK;
}

static void zeroPage(MemPage *pPage, int flags){
  unsigned char *data = pPage->aData;
  BtShared *pBt = pPage->pBt;
  u8 hdr = pPage->hdrOffset;
  u16 first;

  assert( sqlite3PagerPagenumber(pPage->pDbPage)==pPage->pgno );
  assert( sqlite3PagerGetExtra(pPage->pDbPage) == (void*)pPage );
  assert( sqlite3PagerGetData(pPage->pDbPage) == data );
  assert( sqlite3PagerIswriteable(pPage->pDbPage) );
  assert( sqlite3_mutex_held(pBt->mutex) );
  if( pBt->secureDelete ){
    memset(&data[hdr], 0, pBt->usableSize - hdr);
  }
  data[hdr] = (char)flags;
  first = hdr + 8 + 4*((flags&PTF_LEAF)==0 ?1:0);
  memset(&data[hdr+1], 0, 4);
  data[hdr+7] = 0;
  put2byte(&data[hdr+5], pBt->usableSize);
  pPage->nFree = (u16)(pBt->usableSize - first);
  decodeFlags(pPage, flags);
  pPage->hdrOffset = hdr;
  pPage->cellOffset = first;
  pPage->nOverflow = 0;
  assert( pBt->pageSize>=512 && pBt->pageSize<=65536 );
  pPage->maskPage = (u16)(pBt->pageSize - 1);
  pPage->nCell = 0;
  pPage->isInit = 1;
}


static MemPage *btreePageFromDbPage(DbPage *pDbPage, Pgno pgno, BtShared *pBt){
  MemPage *pPage = (MemPage*)sqlite3PagerGetExtra(pDbPage);
  pPage->aData = sqlite3PagerGetData(pDbPage);
  pPage->pDbPage = pDbPage;
  pPage->pBt = pBt;
  pPage->pgno = pgno;
  pPage->hdrOffset = pPage->pgno==1 ? 100 : 0;
  return pPage; 
}

static int btreeGetPage(
  BtShared *pBt,       
  Pgno pgno,           
  MemPage **ppPage,    
  int noContent        
){
  int rc;
  DbPage *pDbPage;

  assert( sqlite3_mutex_held(pBt->mutex) );
  rc = sqlite3PagerAcquire(pBt->pPager, pgno, (DbPage**)&pDbPage, noContent);
  if( rc ) return rc;
  *ppPage = btreePageFromDbPage(pDbPage, pgno, pBt);
  return SQLITE_OK;
}

static MemPage *btreePageLookup(BtShared *pBt, Pgno pgno){
  DbPage *pDbPage;
  assert( sqlite3_mutex_held(pBt->mutex) );
  pDbPage = sqlite3PagerLookup(pBt->pPager, pgno);
  if( pDbPage ){
    return btreePageFromDbPage(pDbPage, pgno, pBt);
  }
  return 0;
}

static Pgno btreePagecount(BtShared *pBt){
  return pBt->nPage;
}
u32 sqlite3BtreeLastPage(Btree *p){
  assert( sqlite3BtreeHoldsMutex(p) );
  assert( ((p->pBt->nPage)&0x8000000)==0 );
  return (int)btreePagecount(p->pBt);
}

static int getAndInitPage(
  BtShared *pBt,          
  Pgno pgno,           
  MemPage **ppPage     
){
  int rc;
  assert( sqlite3_mutex_held(pBt->mutex) );

  if( pgno>btreePagecount(pBt) ){
    rc = SQLITE_CORRUPT_BKPT;
  }else{
    rc = btreeGetPage(pBt, pgno, ppPage, 0);
    if( rc==SQLITE_OK ){
      rc = btreeInitPage(*ppPage);
      if( rc!=SQLITE_OK ){
        releasePage(*ppPage);
      }
    }
  }

  testcase( pgno==0 );
  assert( pgno!=0 || rc==SQLITE_CORRUPT );
  return rc;
}

static void releasePage(MemPage *pPage){
  if( pPage ){
    assert( pPage->aData );
    assert( pPage->pBt );
    assert( sqlite3PagerGetExtra(pPage->pDbPage) == (void*)pPage );
    assert( sqlite3PagerGetData(pPage->pDbPage)==pPage->aData );
    assert( sqlite3_mutex_held(pPage->pBt->mutex) );
    sqlite3PagerUnref(pPage->pDbPage);
  }
}

static void pageReinit(DbPage *pData){
  MemPage *pPage;
  pPage = (MemPage *)sqlite3PagerGetExtra(pData);
  assert( sqlite3PagerPageRefcount(pData)>0 );
  if( pPage->isInit ){
    assert( sqlite3_mutex_held(pPage->pBt->mutex) );
    pPage->isInit = 0;
    if( sqlite3PagerPageRefcount(pData)>1 ){
      btreeInitPage(pPage);
    }
  }
}

static int btreeInvokeBusyHandler(void *pArg){
  BtShared *pBt = (BtShared*)pArg;
  assert( pBt->db );
  assert( sqlite3_mutex_held(pBt->db->mutex) );
  return sqlite3InvokeBusyHandler(&pBt->db->busyHandler);
}

int sqlite3BtreeOpen(
  const char *zFilename,  
  sqlite3 *db,            
  Btree **ppBtree,        /* Pointer to new Btree object written here */
  int flags,              
  int vfsFlags            
){
  sqlite3_vfs *pVfs;             
  BtShared *pBt = 0;             
  Btree *p;                      
  sqlite3_mutex *mutexOpen = 0;  
  int rc = SQLITE_OK;            
  u8 nReserve;                   
  unsigned char zDbHeader[100];  

  
  const int isTempDb = zFilename==0 || zFilename[0]==0;

#ifdef SQLITE_OMIT_MEMORYDB
  const int isMemdb = 0;
#else
  const int isMemdb = (zFilename && strcmp(zFilename, ":memory:")==0)
                       || (isTempDb && sqlite3TempInMemory(db));
#endif

  assert( db!=0 );
  assert( sqlite3_mutex_held(db->mutex) );
  assert( (flags&0xff)==flags );   

  
  assert( (flags & BTREE_UNORDERED)==0 || (flags & BTREE_SINGLE)!=0 );

  
  assert( (flags & BTREE_SINGLE)==0 || isTempDb );

  if( db->flags & SQLITE_NoReadlock ){
    flags |= BTREE_NO_READLOCK;
  }
  if( isMemdb ){
    flags |= BTREE_MEMORY;
  }
  if( (vfsFlags & SQLITE_OPEN_MAIN_DB)!=0 && (isMemdb || isTempDb) ){
    vfsFlags = (vfsFlags & ~SQLITE_OPEN_MAIN_DB) | SQLITE_OPEN_TEMP_DB;
  }
  pVfs = db->pVfs;
  p = sqlite3MallocZero(sizeof(Btree));
  if( !p ){
    return SQLITE_NOMEM;
  }
  p->inTrans = TRANS_NONE;
  p->db = db;
#ifndef SQLITE_OMIT_SHARED_CACHE
  p->lock.pBtree = p;
  p->lock.iTable = 1;
#endif

#if !defined(SQLITE_OMIT_SHARED_CACHE) && !defined(SQLITE_OMIT_DISKIO)
  if( isMemdb==0 && isTempDb==0 ){
    if( vfsFlags & SQLITE_OPEN_SHAREDCACHE ){
      int nFullPathname = pVfs->mxPathname+1;
      char *zFullPathname = sqlite3Malloc(nFullPathname);
      sqlite3_mutex *mutexShared;
      p->sharable = 1;
      if( !zFullPathname ){
        sqlite3_free(p);
        return SQLITE_NOMEM;
      }
      sqlite3OsFullPathname(pVfs, zFilename, nFullPathname, zFullPathname);
      mutexOpen = sqlite3MutexAlloc(SQLITE_MUTEX_STATIC_OPEN);
      sqlite3_mutex_enter(mutexOpen);
      mutexShared = sqlite3MutexAlloc(SQLITE_MUTEX_STATIC_MASTER);
      sqlite3_mutex_enter(mutexShared);
      for(pBt=GLOBAL(BtShared*,sqlite3SharedCacheList); pBt; pBt=pBt->pNext){
        assert( pBt->nRef>0 );
        if( 0==strcmp(zFullPathname, sqlite3PagerFilename(pBt->pPager))
                 && sqlite3PagerVfs(pBt->pPager)==pVfs ){
          int iDb;
          for(iDb=db->nDb-1; iDb>=0; iDb--){
            Btree *pExisting = db->aDb[iDb].pBt;
            if( pExisting && pExisting->pBt==pBt ){
              sqlite3_mutex_leave(mutexShared);
              sqlite3_mutex_leave(mutexOpen);
              sqlite3_free(zFullPathname);
              sqlite3_free(p);
              return SQLITE_CONSTRAINT;
            }
          }
          p->pBt = pBt;
          pBt->nRef++;
          break;
        }
      }
      sqlite3_mutex_leave(mutexShared);
      sqlite3_free(zFullPathname);
    }
#ifdef SQLITE_DEBUG
    else{
      p->sharable = 1;
    }
#endif
  }
#endif
  if( pBt==0 ){
    assert( sizeof(i64)==8 || sizeof(i64)==4 );
    assert( sizeof(u64)==8 || sizeof(u64)==4 );
    assert( sizeof(u32)==4 );
    assert( sizeof(u16)==2 );
    assert( sizeof(Pgno)==4 );
  
    pBt = sqlite3MallocZero( sizeof(*pBt) );
    if( pBt==0 ){
      rc = SQLITE_NOMEM;
      goto btree_open_out;
    }
    rc = sqlite3PagerOpen(pVfs, &pBt->pPager, zFilename,
                          EXTRA_SIZE, flags, vfsFlags, pageReinit);
    if( rc==SQLITE_OK ){
      rc = sqlite3PagerReadFileheader(pBt->pPager,sizeof(zDbHeader),zDbHeader);
    }
    if( rc!=SQLITE_OK ){
      goto btree_open_out;
    }
    pBt->openFlags = (u8)flags;
    pBt->db = db;
    sqlite3PagerSetBusyhandler(pBt->pPager, btreeInvokeBusyHandler, pBt);
    p->pBt = pBt;
  
    pBt->pCursor = 0;
    pBt->pPage1 = 0;
    pBt->readOnly = sqlite3PagerIsreadonly(pBt->pPager);
#ifdef SQLITE_SECURE_DELETE
    pBt->secureDelete = 1;
#endif
    pBt->pageSize = (zDbHeader[16]<<8) | (zDbHeader[17]<<16);
    if( pBt->pageSize<512 || pBt->pageSize>SQLITE_MAX_PAGE_SIZE
         || ((pBt->pageSize-1)&pBt->pageSize)!=0 ){
      pBt->pageSize = 0;
#ifndef SQLITE_OMIT_AUTOVACUUM
      if( zFilename && !isMemdb ){
        pBt->autoVacuum = (SQLITE_DEFAULT_AUTOVACUUM ? 1 : 0);
        pBt->incrVacuum = (SQLITE_DEFAULT_AUTOVACUUM==2 ? 1 : 0);
      }
#endif
      nReserve = 0;
    }else{
      nReserve = zDbHeader[20];
      pBt->pageSizeFixed = 1;
#ifndef SQLITE_OMIT_AUTOVACUUM
      pBt->autoVacuum = (get4byte(&zDbHeader[36 + 4*4])?1:0);
      pBt->incrVacuum = (get4byte(&zDbHeader[36 + 7*4])?1:0);
#endif
    }
    rc = sqlite3PagerSetPagesize(pBt->pPager, &pBt->pageSize, nReserve);
    if( rc ) goto btree_open_out;
    pBt->usableSize = pBt->pageSize - nReserve;
    assert( (pBt->pageSize & 7)==0 );  
   
#if !defined(SQLITE_OMIT_SHARED_CACHE) && !defined(SQLITE_OMIT_DISKIO)
    if( p->sharable ){
      sqlite3_mutex *mutexShared;
      pBt->nRef = 1;
      mutexShared = sqlite3MutexAlloc(SQLITE_MUTEX_STATIC_MASTER);
      if( SQLITE_THREADSAFE && sqlite3GlobalConfig.bCoreMutex ){
        pBt->mutex = sqlite3MutexAlloc(SQLITE_MUTEX_FAST);
        if( pBt->mutex==0 ){
          rc = SQLITE_NOMEM;
          db->mallocFailed = 0;
          goto btree_open_out;
        }
      }
      sqlite3_mutex_enter(mutexShared);
      pBt->pNext = GLOBAL(BtShared*,sqlite3SharedCacheList);
      GLOBAL(BtShared*,sqlite3SharedCacheList) = pBt;
      sqlite3_mutex_leave(mutexShared);
    }
#endif
  }

#if !defined(SQLITE_OMIT_SHARED_CACHE) && !defined(SQLITE_OMIT_DISKIO)
  if( p->sharable ){
    int i;
    Btree *pSib;
    for(i=0; i<db->nDb; i++){
      if( (pSib = db->aDb[i].pBt)!=0 && pSib->sharable ){
        while( pSib->pPrev ){ pSib = pSib->pPrev; }
        if( p->pBt<pSib->pBt ){
          p->pNext = pSib;
          p->pPrev = 0;
          pSib->pPrev = p;
        }else{
          while( pSib->pNext && pSib->pNext->pBt<p->pBt ){
            pSib = pSib->pNext;
          }
          p->pNext = pSib->pNext;
          p->pPrev = pSib;
          if( p->pNext ){
            p->pNext->pPrev = p;
          }
          pSib->pNext = p;
        }
        break;
      }
    }
  }
#endif
  *ppBtree = p;

btree_open_out:
  if( rc!=SQLITE_OK ){
    if( pBt && pBt->pPager ){
      sqlite3PagerClose(pBt->pPager);
    }
    sqlite3_free(pBt);
    sqlite3_free(p);
    *ppBtree = 0;
  }else{
    if( sqlite3BtreeSchema(p, 0, 0)==0 ){
      sqlite3PagerSetCachesize(p->pBt->pPager, SQLITE_DEFAULT_CACHE_SIZE);
    }
  }
  if( mutexOpen ){
    assert( sqlite3_mutex_held(mutexOpen) );
    sqlite3_mutex_leave(mutexOpen);
  }
  return rc;
}

static int removeFromSharingList(BtShared *pBt){
#ifndef SQLITE_OMIT_SHARED_CACHE
  sqlite3_mutex *pMaster;
  BtShared *pList;
  int removed = 0;

  assert( sqlite3_mutex_notheld(pBt->mutex) );
  pMaster = sqlite3MutexAlloc(SQLITE_MUTEX_STATIC_MASTER);
  sqlite3_mutex_enter(pMaster);
  pBt->nRef--;
  if( pBt->nRef<=0 ){
    if( GLOBAL(BtShared*,sqlite3SharedCacheList)==pBt ){
      GLOBAL(BtShared*,sqlite3SharedCacheList) = pBt->pNext;
    }else{
      pList = GLOBAL(BtShared*,sqlite3SharedCacheList);
      while( ALWAYS(pList) && pList->pNext!=pBt ){
        pList=pList->pNext;
      }
      if( ALWAYS(pList) ){
        pList->pNext = pBt->pNext;
      }
    }
    if( SQLITE_THREADSAFE ){
      sqlite3_mutex_free(pBt->mutex);
    }
    removed = 1;
  }
  sqlite3_mutex_leave(pMaster);
  return removed;
#else
  return 1;
#endif
}

static void allocateTempSpace(BtShared *pBt){
  if( !pBt->pTmpSpace ){
    pBt->pTmpSpace = sqlite3PageMalloc( pBt->pageSize );
  }
}

static void freeTempSpace(BtShared *pBt){
  sqlite3PageFree( pBt->pTmpSpace);
  pBt->pTmpSpace = 0;
}

int sqlite3BtreeClose(Btree *p){
  BtShared *pBt = p->pBt;
  BtCursor *pCur;

  
  assert( sqlite3_mutex_held(p->db->mutex) );
  sqlite3BtreeEnter(p);
  pCur = pBt->pCursor;
  while( pCur ){
    BtCursor *pTmp = pCur;
    pCur = pCur->pNext;
    if( pTmp->pBtree==p ){
      sqlite3BtreeCloseCursor(pTmp);
    }
  }

  sqlite3BtreeRollback(p);
  sqlite3BtreeLeave(p);

  assert( p->wantToLock==0 && p->locked==0 );
  if( !p->sharable || removeFromSharingList(pBt) ){
    assert( !pBt->pCursor );
    sqlite3PagerClose(pBt->pPager);
    if( pBt->xFreeSchema && pBt->pSchema ){
      pBt->xFreeSchema(pBt->pSchema);
    }
    sqlite3DbFree(0, pBt->pSchema);
    freeTempSpace(pBt);
    sqlite3_free(pBt);
  }

#ifndef SQLITE_OMIT_SHARED_CACHE
  assert( p->wantToLock==0 );
  assert( p->locked==0 );
  if( p->pPrev ) p->pPrev->pNext = p->pNext;
  if( p->pNext ) p->pNext->pPrev = p->pPrev;
#endif

  sqlite3_free(p);
  return SQLITE_OK;
}

/*
** Change the limit on the number of pages allowed in the cache.
**
** The maximum number of cache pages is set to the absolute
** value of mxPage.  If mxPage is negative, the pager will
** operate asynchronously - it will not stop to do fsync()s
** to insure data is written to the disk surface before
** continuing.  Transactions still work if synchronous is off,
** and the database cannot be corrupted if this program
** crashes.  But if the operating system crashes or there is
** an abrupt power failure when synchronous is off, the database
** could be left in an inconsistent and unrecoverable state.
** Synchronous is on by default so database corruption is not
** normally a worry.
*/
int sqlite3BtreeSetCacheSize(Btree *p, int mxPage){
  BtShared *pBt = p->pBt;
  assert( sqlite3_mutex_held(p->db->mutex) );
  sqlite3BtreeEnter(p);
  sqlite3PagerSetCachesize(pBt->pPager, mxPage);
  sqlite3BtreeLeave(p);
  return SQLITE_OK;
}

#ifndef SQLITE_OMIT_PAGER_PRAGMAS
int sqlite3BtreeSetSafetyLevel(
  Btree *p,              
  int level,             
  int fullSync,          
  int ckptFullSync       
){
  BtShared *pBt = p->pBt;
  assert( sqlite3_mutex_held(p->db->mutex) );
  assert( level>=1 && level<=3 );
  sqlite3BtreeEnter(p);
  sqlite3PagerSetSafetyLevel(pBt->pPager, level, fullSync, ckptFullSync);
  sqlite3BtreeLeave(p);
  return SQLITE_OK;
}
#endif

int sqlite3BtreeSyncDisabled(Btree *p){
  BtShared *pBt = p->pBt;
  int rc;
  assert( sqlite3_mutex_held(p->db->mutex) );  
  sqlite3BtreeEnter(p);
  assert( pBt && pBt->pPager );
  rc = sqlite3PagerNosync(pBt->pPager);
  sqlite3BtreeLeave(p);
  return rc;
}

int sqlite3BtreeSetPageSize(Btree *p, int pageSize, int nReserve, int iFix){
  int rc = SQLITE_OK;
  BtShared *pBt = p->pBt;
  assert( nReserve>=-1 && nReserve<=255 );
  sqlite3BtreeEnter(p);
  if( pBt->pageSizeFixed ){
    sqlite3BtreeLeave(p);
    return SQLITE_READONLY;
  }
  if( nReserve<0 ){
    nReserve = pBt->pageSize - pBt->usableSize;
  }
  assert( nReserve>=0 && nReserve<=255 );
  if( pageSize>=512 && pageSize<=SQLITE_MAX_PAGE_SIZE &&
        ((pageSize-1)&pageSize)==0 ){
    assert( (pageSize & 7)==0 );
    assert( !pBt->pPage1 && !pBt->pCursor );
    pBt->pageSize = (u32)pageSize;
    freeTempSpace(pBt);
  }
  rc = sqlite3PagerSetPagesize(pBt->pPager, &pBt->pageSize, nReserve);
  pBt->usableSize = pBt->pageSize - (u16)nReserve;
  if( iFix ) pBt->pageSizeFixed = 1;
  sqlite3BtreeLeave(p);
  return rc;
}

int sqlite3BtreeGetPageSize(Btree *p){
  return p->pBt->pageSize;
}

#if !defined(SQLITE_OMIT_PAGER_PRAGMAS) || !defined(SQLITE_OMIT_VACUUM)
int sqlite3BtreeGetReserve(Btree *p){
  int n;
  sqlite3BtreeEnter(p);
  n = p->pBt->pageSize - p->pBt->usableSize;
  sqlite3BtreeLeave(p);
  return n;
}

int sqlite3BtreeMaxPageCount(Btree *p, int mxPage){
  int n;
  sqlite3BtreeEnter(p);
  n = sqlite3PagerMaxPageCount(p->pBt->pPager, mxPage);
  sqlite3BtreeLeave(p);
  return n;
}

int sqlite3BtreeSecureDelete(Btree *p, int newFlag){
  int b;
  if( p==0 ) return 0;
  sqlite3BtreeEnter(p);
  if( newFlag>=0 ){
    p->pBt->secureDelete = (newFlag!=0) ? 1 : 0;
  } 
  b = p->pBt->secureDelete;
  sqlite3BtreeLeave(p);
  return b;
}
#endif 

int sqlite3BtreeSetAutoVacuum(Btree *p, int autoVacuum){
#ifdef SQLITE_OMIT_AUTOVACUUM
  return SQLITE_READONLY;
#else
  BtShared *pBt = p->pBt;
  int rc = SQLITE_OK;
  u8 av = (u8)autoVacuum;

  sqlite3BtreeEnter(p);
  if( pBt->pageSizeFixed && (av ?1:0)!=pBt->autoVacuum ){
    rc = SQLITE_READONLY;
  }else{
    pBt->autoVacuum = av ?1:0;
    pBt->incrVacuum = av==2 ?1:0;
  }
  sqlite3BtreeLeave(p);
  return rc;
#endif
}

int sqlite3BtreeGetAutoVacuum(Btree *p){
#ifdef SQLITE_OMIT_AUTOVACUUM
  return BTREE_AUTOVACUUM_NONE;
#else
  int rc;
  sqlite3BtreeEnter(p);
  rc = (
    (!p->pBt->autoVacuum)?BTREE_AUTOVACUUM_NONE:
    (!p->pBt->incrVacuum)?BTREE_AUTOVACUUM_FULL:
    BTREE_AUTOVACUUM_INCR
  );
  sqlite3BtreeLeave(p);
  return rc;
#endif
}


static int lockBtree(BtShared *pBt){
  int rc;              
  MemPage *pPage1;     
  int nPage;           
  int nPageFile = 0;   
  int nPageHeader;     

  assert( sqlite3_mutex_held(pBt->mutex) );
  assert( pBt->pPage1==0 );
  rc = sqlite3PagerSharedLock(pBt->pPager);
  if( rc!=SQLITE_OK ) return rc;
  rc = btreeGetPage(pBt, 1, &pPage1, 0);
  if( rc!=SQLITE_OK ) return rc;

  nPage = nPageHeader = get4byte(28+(u8*)pPage1->aData);
  sqlite3PagerPagecount(pBt->pPager, &nPageFile);
  if( nPage==0 || memcmp(24+(u8*)pPage1->aData, 92+(u8*)pPage1->aData,4)!=0 ){
    nPage = nPageFile;
  }
  if( nPage>0 ){
    u32 pageSize;
    u32 usableSize;
    u8 *page1 = pPage1->aData;
    rc = SQLITE_NOTADB;
    if( memcmp(page1, zMagicHeader, 16)!=0 ){
      goto page1_init_failed;
    }

#ifdef SQLITE_OMIT_WAL
    if( page1[18]>1 ){
      pBt->readOnly = 1;
    }
    if( page1[19]>1 ){
      goto page1_init_failed;
    }
#else
    if( page1[18]>2 ){
      pBt->readOnly = 1;
    }
    if( page1[19]>2 ){
      goto page1_init_failed;
    }

    if( page1[19]==2 && pBt->doNotUseWAL==0 ){
      int isOpen = 0;
      rc = sqlite3PagerOpenWal(pBt->pPager, &isOpen);
      if( rc!=SQLITE_OK ){
        goto page1_init_failed;
      }else if( isOpen==0 ){
        releasePage(pPage1);
        return SQLITE_OK;
      }
      rc = SQLITE_NOTADB;
    }
#endif

    if( memcmp(&page1[21], "\100\040\040",3)!=0 ){
      goto page1_init_failed;
    }
    pageSize = (page1[16]<<8) | (page1[17]<<16);
    if( ((pageSize-1)&pageSize)!=0
     || pageSize>SQLITE_MAX_PAGE_SIZE 
     || pageSize<=256 
    ){
      goto page1_init_failed;
    }
    assert( (pageSize & 7)==0 );
    usableSize = pageSize - page1[20];
    if( (u32)pageSize!=pBt->pageSize ){
      releasePage(pPage1);
      pBt->usableSize = usableSize;
      pBt->pageSize = pageSize;
      freeTempSpace(pBt);
      rc = sqlite3PagerSetPagesize(pBt->pPager, &pBt->pageSize,
                                   pageSize-usableSize);
      return rc;
    }
    if( (pBt->db->flags & SQLITE_RecoveryMode)==0 && nPage>nPageFile ){
      rc = SQLITE_CORRUPT_BKPT;
      goto page1_init_failed;
    }
    if( usableSize<480 ){
      goto page1_init_failed;
    }
    pBt->pageSize = pageSize;
    pBt->usableSize = usableSize;
#ifndef SQLITE_OMIT_AUTOVACUUM
    pBt->autoVacuum = (get4byte(&page1[36 + 4*4])?1:0);
    pBt->incrVacuum = (get4byte(&page1[36 + 7*4])?1:0);
#endif
  }

  pBt->maxLocal = (u16)((pBt->usableSize-12)*64/255 - 23);
  pBt->minLocal = (u16)((pBt->usableSize-12)*32/255 - 23);
  pBt->maxLeaf = (u16)(pBt->usableSize - 35);
  pBt->minLeaf = (u16)((pBt->usableSize-12)*32/255 - 23);
  assert( pBt->maxLeaf + 23 <= MX_CELL_SIZE(pBt) );
  pBt->pPage1 = pPage1;
  pBt->nPage = nPage;
  return SQLITE_OK;

page1_init_failed:
  releasePage(pPage1);
  pBt->pPage1 = 0;
  return rc;
}

static void unlockBtreeIfUnused(BtShared *pBt){
  assert( sqlite3_mutex_held(pBt->mutex) );
  assert( pBt->pCursor==0 || pBt->inTransaction>TRANS_NONE );
  if( pBt->inTransaction==TRANS_NONE && pBt->pPage1!=0 ){
    assert( pBt->pPage1->aData );
    assert( sqlite3PagerRefcount(pBt->pPager)==1 );
    assert( pBt->pPage1->aData );
    releasePage(pBt->pPage1);
    pBt->pPage1 = 0;
  }
}

static int newDatabase(BtShared *pBt){
  MemPage *pP1;
  unsigned char *data;
  int rc;

  assert( sqlite3_mutex_held(pBt->mutex) );
  if( pBt->nPage>0 ){
    return SQLITE_OK;
  }
  pP1 = pBt->pPage1;
  assert( pP1!=0 );
  data = pP1->aData;
  rc = sqlite3PagerWrite(pP1->pDbPage);
  if( rc ) return rc;
  memcpy(data, zMagicHeader, sizeof(zMagicHeader));
  assert( sizeof(zMagicHeader)==16 );
  data[16] = (u8)((pBt->pageSize>>8)&0xff);
  data[17] = (u8)((pBt->pageSize>>16)&0xff);
  data[18] = 1;
  data[19] = 1;
  assert( pBt->usableSize<=pBt->pageSize && pBt->usableSize+255>=pBt->pageSize);
  data[20] = (u8)(pBt->pageSize - pBt->usableSize);
  data[21] = 64;
  data[22] = 32;
  data[23] = 32;
  memset(&data[24], 0, 100-24);
  zeroPage(pP1, PTF_INTKEY|PTF_LEAF|PTF_LEAFDATA );
  pBt->pageSizeFixed = 1;
#ifndef SQLITE_OMIT_AUTOVACUUM
  assert( pBt->autoVacuum==1 || pBt->autoVacuum==0 );
  assert( pBt->incrVacuum==1 || pBt->incrVacuum==0 );
  put4byte(&data[36 + 4*4], pBt->autoVacuum);
  put4byte(&data[36 + 7*4], pBt->incrVacuum);
#endif
  pBt->nPage = 1;
  data[31] = 1;
  return SQLITE_OK;
}

int sqlite3BtreeBeginTrans(Btree *p, int wrflag){
  sqlite3 *pBlock = 0;
  BtShared *pBt = p->pBt;
  int rc = SQLITE_OK;

  sqlite3BtreeEnter(p);
  btreeIntegrity(p);

  if( p->inTrans==TRANS_WRITE || (p->inTrans==TRANS_READ && !wrflag) ){
    goto trans_begun;
  }

  
  if( pBt->readOnly && wrflag ){
    rc = SQLITE_READONLY;
    goto trans_begun;
  }

#ifndef SQLITE_OMIT_SHARED_CACHE
  if( (wrflag && pBt->inTransaction==TRANS_WRITE) || pBt->isPending ){
    pBlock = pBt->pWriter->db;
  }else if( wrflag>1 ){
    BtLock *pIter;
    for(pIter=pBt->pLock; pIter; pIter=pIter->pNext){
      if( pIter->pBtree!=p ){
        pBlock = pIter->pBtree->db;
        break;
      }
    }
  }
  if( pBlock ){
    sqlite3ConnectionBlocked(p->db, pBlock);
    rc = SQLITE_LOCKED_SHAREDCACHE;
    goto trans_begun;
  }
#endif

  rc = querySharedCacheTableLock(p, MASTER_ROOT, READ_LOCK);
  if( SQLITE_OK!=rc ) goto trans_begun;

  pBt->initiallyEmpty = (u8)(pBt->nPage==0);
  do {
    while( pBt->pPage1==0 && SQLITE_OK==(rc = lockBtree(pBt)) );

    if( rc==SQLITE_OK && wrflag ){
      if( pBt->readOnly ){
        rc = SQLITE_READONLY;
      }else{
        rc = sqlite3PagerBegin(pBt->pPager,wrflag>1,sqlite3TempInMemory(p->db));
        if( rc==SQLITE_OK ){
          rc = newDatabase(pBt);
        }
      }
    }
  
    if( rc!=SQLITE_OK ){
      unlockBtreeIfUnused(pBt);
    }
  }while( (rc&0xFF)==SQLITE_BUSY && pBt->inTransaction==TRANS_NONE &&
          btreeInvokeBusyHandler(pBt) );

  if( rc==SQLITE_OK ){
    if( p->inTrans==TRANS_NONE ){
      pBt->nTransaction++;
#ifndef SQLITE_OMIT_SHARED_CACHE
      if( p->sharable ){
	assert( p->lock.pBtree==p && p->lock.iTable==1 );
        p->lock.eLock = READ_LOCK;
        p->lock.pNext = pBt->pLock;
        pBt->pLock = &p->lock;
      }
#endif
    }
    p->inTrans = (wrflag?TRANS_WRITE:TRANS_READ);
    if( p->inTrans>pBt->inTransaction ){
      pBt->inTransaction = p->inTrans;
    }
    if( wrflag ){
      MemPage *pPage1 = pBt->pPage1;
#ifndef SQLITE_OMIT_SHARED_CACHE
      assert( !pBt->pWriter );
      pBt->pWriter = p;
      pBt->isExclusive = (u8)(wrflag>1);
#endif

      if( pBt->nPage!=get4byte(&pPage1->aData[28]) ){
        rc = sqlite3PagerWrite(pPage1->pDbPage);
        if( rc==SQLITE_OK ){
          put4byte(&pPage1->aData[28], pBt->nPage);
        }
      }
    }
  }


trans_begun:
  if( rc==SQLITE_OK && wrflag ){
    rc = sqlite3PagerOpenSavepoint(pBt->pPager, p->db->nSavepoint);
  }

  btreeIntegrity(p);
  sqlite3BtreeLeave(p);
  return rc;
}

#ifndef SQLITE_OMIT_AUTOVACUUM

static int setChildPtrmaps(MemPage *pPage){
  int i;                             
  int nCell;                         
  int rc;                            
  BtShared *pBt = pPage->pBt;
  u8 isInitOrig = pPage->isInit;
  Pgno pgno = pPage->pgno;

  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  rc = btreeInitPage(pPage);
  if( rc!=SQLITE_OK ){
    goto set_child_ptrmaps_out;
  }
  nCell = pPage->nCell;

  for(i=0; i<nCell; i++){
    u8 *pCell = findCell(pPage, i);

    ptrmapPutOvflPtr(pPage, pCell, &rc);

    if( !pPage->leaf ){
      Pgno childPgno = get4byte(pCell);
      ptrmapPut(pBt, childPgno, PTRMAP_BTREE, pgno, &rc);
    }
  }

  if( !pPage->leaf ){
    Pgno childPgno = get4byte(&pPage->aData[pPage->hdrOffset+8]);
    ptrmapPut(pBt, childPgno, PTRMAP_BTREE, pgno, &rc);
  }

set_child_ptrmaps_out:
  pPage->isInit = isInitOrig;
  return rc;
}

static int modifyPagePointer(MemPage *pPage, Pgno iFrom, Pgno iTo, u8 eType){
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  assert( sqlite3PagerIswriteable(pPage->pDbPage) );
  if( eType==PTRMAP_OVERFLOW2 ){
    
    if( get4byte(pPage->aData)!=iFrom ){
      return SQLITE_CORRUPT_BKPT;
    }
    put4byte(pPage->aData, iTo);
  }else{
    u8 isInitOrig = pPage->isInit;
    int i;
    int nCell;

    btreeInitPage(pPage);
    nCell = pPage->nCell;

    for(i=0; i<nCell; i++){
      u8 *pCell = findCell(pPage, i);
      if( eType==PTRMAP_OVERFLOW1 ){
        CellInfo info;
        btreeParseCellPtr(pPage, pCell, &info);
        if( info.iOverflow ){
          if( iFrom==get4byte(&pCell[info.iOverflow]) ){
            put4byte(&pCell[info.iOverflow], iTo);
            break;
          }
        }
      }else{
        if( get4byte(pCell)==iFrom ){
          put4byte(pCell, iTo);
          break;
        }
      }
    }
  
    if( i==nCell ){
      if( eType!=PTRMAP_BTREE || 
          get4byte(&pPage->aData[pPage->hdrOffset+8])!=iFrom ){
        return SQLITE_CORRUPT_BKPT;
      }
      put4byte(&pPage->aData[pPage->hdrOffset+8], iTo);
    }

    pPage->isInit = isInitOrig;
  }
  return SQLITE_OK;
}


/*
** Move the open database page pDbPage to location iFreePage in the 
** database. The pDbPage reference remains valid.
**
** The isCommit flag indicates that there is no need to remember that
** the journal needs to be sync()ed before database page pDbPage->pgno 
** can be written to. The caller has already promised not to write to that
** page.
*/
static int relocatePage(
  BtShared *pBt,           
  MemPage *pDbPage,        
  u8 eType,                
  Pgno iPtrPage,           
  Pgno iFreePage,          
  int isCommit             
){
  MemPage *pPtrPage;   
  Pgno iDbPage = pDbPage->pgno;
  Pager *pPager = pBt->pPager;
  int rc;

  assert( eType==PTRMAP_OVERFLOW2 || eType==PTRMAP_OVERFLOW1 || 
      eType==PTRMAP_BTREE || eType==PTRMAP_ROOTPAGE );
  assert( sqlite3_mutex_held(pBt->mutex) );
  assert( pDbPage->pBt==pBt );

  
  TRACE(("AUTOVACUUM: Moving %d to free page %d (ptr page %d type %d)\n", 
      iDbPage, iFreePage, iPtrPage, eType));
  rc = sqlite3PagerMovepage(pPager, pDbPage->pDbPage, iFreePage, isCommit);
  if( rc!=SQLITE_OK ){
    return rc;
  }
  pDbPage->pgno = iFreePage;

  if( eType==PTRMAP_BTREE || eType==PTRMAP_ROOTPAGE ){
    rc = setChildPtrmaps(pDbPage);
    if( rc!=SQLITE_OK ){
      return rc;
    }
  }else{
    Pgno nextOvfl = get4byte(pDbPage->aData);
    if( nextOvfl!=0 ){
      ptrmapPut(pBt, nextOvfl, PTRMAP_OVERFLOW2, iFreePage, &rc);
      if( rc!=SQLITE_OK ){
        return rc;
      }
    }
  }

  if( eType!=PTRMAP_ROOTPAGE ){
    rc = btreeGetPage(pBt, iPtrPage, &pPtrPage, 0);
    if( rc!=SQLITE_OK ){
      return rc;
    }
    rc = sqlite3PagerWrite(pPtrPage->pDbPage);
    if( rc!=SQLITE_OK ){
      releasePage(pPtrPage);
      return rc;
    }
    rc = modifyPagePointer(pPtrPage, iDbPage, iFreePage, eType);
    releasePage(pPtrPage);
    if( rc==SQLITE_OK ){
      ptrmapPut(pBt, iFreePage, eType, iPtrPage, &rc);
    }
  }
  return rc;
}

static int allocateBtreePage(BtShared *, MemPage **, Pgno *, Pgno, u8);

static int incrVacuumStep(BtShared *pBt, Pgno nFin, Pgno iLastPg){
  Pgno nFreeList;           
  int rc;

  assert( sqlite3_mutex_held(pBt->mutex) );
  assert( iLastPg>nFin );

  if( !PTRMAP_ISPAGE(pBt, iLastPg) && iLastPg!=PENDING_BYTE_PAGE(pBt) ){
    u8 eType;
    Pgno iPtrPage;

    nFreeList = get4byte(&pBt->pPage1->aData[36]);
    if( nFreeList==0 ){
      return SQLITE_DONE;
    }

    rc = ptrmapGet(pBt, iLastPg, &eType, &iPtrPage);
    if( rc!=SQLITE_OK ){
      return rc;
    }
    if( eType==PTRMAP_ROOTPAGE ){
      return SQLITE_CORRUPT_BKPT;
    }

    if( eType==PTRMAP_FREEPAGE ){
      if( nFin==0 ){
        Pgno iFreePg;
        MemPage *pFreePg;
        rc = allocateBtreePage(pBt, &pFreePg, &iFreePg, iLastPg, 1);
        if( rc!=SQLITE_OK ){
          return rc;
        }
        assert( iFreePg==iLastPg );
        releasePage(pFreePg);
      }
    } else {
      Pgno iFreePg;             
      MemPage *pLastPg;

      rc = btreeGetPage(pBt, iLastPg, &pLastPg, 0);
      if( rc!=SQLITE_OK ){
        return rc;
      }

      do {
        MemPage *pFreePg;
        rc = allocateBtreePage(pBt, &pFreePg, &iFreePg, 0, 0);
        if( rc!=SQLITE_OK ){
          releasePage(pLastPg);
          return rc;
        }
        releasePage(pFreePg);
      }while( nFin!=0 && iFreePg>nFin );
      assert( iFreePg<iLastPg );
      
      rc = sqlite3PagerWrite(pLastPg->pDbPage);
      if( rc==SQLITE_OK ){
        rc = relocatePage(pBt, pLastPg, eType, iPtrPage, iFreePg, nFin!=0);
      }
      releasePage(pLastPg);
      if( rc!=SQLITE_OK ){
        return rc;
      }
    }
  }

  if( nFin==0 ){
    iLastPg--;
    while( iLastPg==PENDING_BYTE_PAGE(pBt)||PTRMAP_ISPAGE(pBt, iLastPg) ){
      if( PTRMAP_ISPAGE(pBt, iLastPg) ){
        MemPage *pPg;
        rc = btreeGetPage(pBt, iLastPg, &pPg, 0);
        if( rc!=SQLITE_OK ){
          return rc;
        }
        rc = sqlite3PagerWrite(pPg->pDbPage);
        releasePage(pPg);
        if( rc!=SQLITE_OK ){
          return rc;
        }
      }
      iLastPg--;
    }
    sqlite3PagerTruncateImage(pBt->pPager, iLastPg);
    pBt->nPage = iLastPg;
  }
  return SQLITE_OK;
}

int sqlite3BtreeIncrVacuum(Btree *p){
  int rc;
  BtShared *pBt = p->pBt;

  sqlite3BtreeEnter(p);
  assert( pBt->inTransaction==TRANS_WRITE && p->inTrans==TRANS_WRITE );
  if( !pBt->autoVacuum ){
    rc = SQLITE_DONE;
  }else{
    invalidateAllOverflowCache(pBt);
    rc = incrVacuumStep(pBt, 0, btreePagecount(pBt));
    if( rc==SQLITE_OK ){
      rc = sqlite3PagerWrite(pBt->pPage1->pDbPage);
      put4byte(&pBt->pPage1->aData[28], pBt->nPage);
    }
  }
  sqlite3BtreeLeave(p);
  return rc;
}

static int autoVacuumCommit(BtShared *pBt){
  int rc = SQLITE_OK;
  Pager *pPager = pBt->pPager;
  VVA_ONLY( int nRef = sqlite3PagerRefcount(pPager) );

  assert( sqlite3_mutex_held(pBt->mutex) );
  invalidateAllOverflowCache(pBt);
  assert(pBt->autoVacuum);
  if( !pBt->incrVacuum ){
    Pgno nFin;         
    Pgno nFree;        
    Pgno nPtrmap;      
    Pgno iFree;        
    int nEntry;        
    Pgno nOrig;        

    nOrig = btreePagecount(pBt);
    if( PTRMAP_ISPAGE(pBt, nOrig) || nOrig==PENDING_BYTE_PAGE(pBt) ){
      return SQLITE_CORRUPT_BKPT;
    }

    nFree = get4byte(&pBt->pPage1->aData[36]);
    nEntry = pBt->usableSize/5;
    nPtrmap = (nFree-nOrig+PTRMAP_PAGENO(pBt, nOrig)+nEntry)/nEntry;
    nFin = nOrig - nFree - nPtrmap;
    if( nOrig>PENDING_BYTE_PAGE(pBt) && nFin<PENDING_BYTE_PAGE(pBt) ){
      nFin--;
    }
    while( PTRMAP_ISPAGE(pBt, nFin) || nFin==PENDING_BYTE_PAGE(pBt) ){
      nFin--;
    }
    if( nFin>nOrig ) return SQLITE_CORRUPT_BKPT;

    for(iFree=nOrig; iFree>nFin && rc==SQLITE_OK; iFree--){
      rc = incrVacuumStep(pBt, nFin, iFree);
    }
    if( (rc==SQLITE_DONE || rc==SQLITE_OK) && nFree>0 ){
      rc = sqlite3PagerWrite(pBt->pPage1->pDbPage);
      put4byte(&pBt->pPage1->aData[32], 0);
      put4byte(&pBt->pPage1->aData[36], 0);
      put4byte(&pBt->pPage1->aData[28], nFin);
      sqlite3PagerTruncateImage(pBt->pPager, nFin);
      pBt->nPage = nFin;
    }
    if( rc!=SQLITE_OK ){
      sqlite3PagerRollback(pPager);
    }
  }

  assert( nRef==sqlite3PagerRefcount(pPager) );
  return rc;
}

#else 
# define setChildPtrmaps(x) SQLITE_OK
#endif

/*
** This routine does the first phase of a two-phase commit.  This routine
** causes a rollback journal to be created (if it does not already exist)
** and populated with enough information so that if a power loss occurs
** the database can be restored to its original state by playing back
** the journal.  Then the contents of the journal are flushed out to
** the disk.  After the journal is safely on oxide, the changes to the
** database are written into the database file and flushed to oxide.
** At the end of this call, the rollback journal still exists on the
** disk and we are still holding all locks, so the transaction has not
** committed.  See sqlite3BtreeCommitPhaseTwo() for the second phase of the
** commit process.
**
** This call is a no-op if no write-transaction is currently active on pBt.
**
** Otherwise, sync the database file for the btree pBt. zMaster points to
** the name of a master journal file that should be written into the
** individual journal file, or is NULL, indicating no master journal file 
** (single database transaction).
**
** When this is called, the master journal should already have been
** created, populated with this journal pointer and synced to disk.
**
** Once this is routine has returned, the only thing required to commit
** the write-transaction for this database file is to delete the journal.
*/
int sqlite3BtreeCommitPhaseOne(Btree *p, const char *zMaster){
  int rc = SQLITE_OK;
  if( p->inTrans==TRANS_WRITE ){
    BtShared *pBt = p->pBt;
    sqlite3BtreeEnter(p);
#ifndef SQLITE_OMIT_AUTOVACUUM
    if( pBt->autoVacuum ){
      rc = autoVacuumCommit(pBt);
      if( rc!=SQLITE_OK ){
        sqlite3BtreeLeave(p);
        return rc;
      }
    }
#endif
    rc = sqlite3PagerCommitPhaseOne(pBt->pPager, zMaster, 0);
    sqlite3BtreeLeave(p);
  }
  return rc;
}

static void btreeEndTransaction(Btree *p){
  BtShared *pBt = p->pBt;
  assert( sqlite3BtreeHoldsMutex(p) );

  btreeClearHasContent(pBt);
  if( p->inTrans>TRANS_NONE && p->db->activeVdbeCnt>1 ){
    downgradeAllSharedCacheTableLocks(p);
    p->inTrans = TRANS_READ;
  }else{
    if( p->inTrans!=TRANS_NONE ){
      clearAllSharedCacheTableLocks(p);
      pBt->nTransaction--;
      if( 0==pBt->nTransaction ){
        pBt->inTransaction = TRANS_NONE;
      }
    }

    p->inTrans = TRANS_NONE;
    unlockBtreeIfUnused(pBt);
  }

  btreeIntegrity(p);
}

/*
** Commit the transaction currently in progress.
**
** This routine implements the second phase of a 2-phase commit.  The
** sqlite3BtreeCommitPhaseOne() routine does the first phase and should
** be invoked prior to calling this routine.  The sqlite3BtreeCommitPhaseOne()
** routine did all the work of writing information out to disk and flushing the
** contents so that they are written onto the disk platter.  All this
** routine has to do is delete or truncate or zero the header in the
** the rollback journal (which causes the transaction to commit) and
** drop locks.
**
** Normally, if an error occurs while the pager layer is attempting to 
** finalize the underlying journal file, this function returns an error and
** the upper layer will attempt a rollback. However, if the second argument
** is non-zero then this b-tree transaction is part of a multi-file 
** transaction. In this case, the transaction has already been committed 
** (by deleting a master journal file) and the caller will ignore this 
** functions return code. So, even if an error occurs in the pager layer,
** reset the b-tree objects internal state to indicate that the write
** transaction has been closed. This is quite safe, as the pager will have
** transitioned to the error state.
**
** This will release the write lock on the database file.  If there
** are no active cursors, it also releases the read lock.
*/
int sqlite3BtreeCommitPhaseTwo(Btree *p, int bCleanup){

  if( p->inTrans==TRANS_NONE ) return SQLITE_OK;
  sqlite3BtreeEnter(p);
  btreeIntegrity(p);

  if( p->inTrans==TRANS_WRITE ){
    int rc;
    BtShared *pBt = p->pBt;
    assert( pBt->inTransaction==TRANS_WRITE );
    assert( pBt->nTransaction>0 );
    rc = sqlite3PagerCommitPhaseTwo(pBt->pPager);
    if( rc!=SQLITE_OK && bCleanup==0 ){
      sqlite3BtreeLeave(p);
      return rc;
    }
    pBt->inTransaction = TRANS_READ;
  }

  btreeEndTransaction(p);
  sqlite3BtreeLeave(p);
  return SQLITE_OK;
}

int sqlite3BtreeCommit(Btree *p){
  int rc;
  sqlite3BtreeEnter(p);
  rc = sqlite3BtreeCommitPhaseOne(p, 0);
  if( rc==SQLITE_OK ){
    rc = sqlite3BtreeCommitPhaseTwo(p, 0);
  }
  sqlite3BtreeLeave(p);
  return rc;
}

#ifndef NDEBUG
static int countWriteCursors(BtShared *pBt){
  BtCursor *pCur;
  int r = 0;
  for(pCur=pBt->pCursor; pCur; pCur=pCur->pNext){
    if( pCur->wrFlag && pCur->eState!=CURSOR_FAULT ) r++; 
  }
  return r;
}
#endif

void sqlite3BtreeTripAllCursors(Btree *pBtree, int errCode){
  BtCursor *p;
  sqlite3BtreeEnter(pBtree);
  for(p=pBtree->pBt->pCursor; p; p=p->pNext){
    int i;
    sqlite3BtreeClearCursor(p);
    p->eState = CURSOR_FAULT;
    p->skipNext = errCode;
    for(i=0; i<=p->iPage; i++){
      releasePage(p->apPage[i]);
      p->apPage[i] = 0;
    }
  }
  sqlite3BtreeLeave(pBtree);
}

int sqlite3BtreeRollback(Btree *p){
  int rc;
  BtShared *pBt = p->pBt;
  MemPage *pPage1;

  sqlite3BtreeEnter(p);
  rc = saveAllCursors(pBt, 0, 0);
#ifndef SQLITE_OMIT_SHARED_CACHE
  if( rc!=SQLITE_OK ){
    sqlite3BtreeTripAllCursors(p, rc);
  }
#endif
  btreeIntegrity(p);

  if( p->inTrans==TRANS_WRITE ){
    int rc2;

    assert( TRANS_WRITE==pBt->inTransaction );
    rc2 = sqlite3PagerRollback(pBt->pPager);
    if( rc2!=SQLITE_OK ){
      rc = rc2;
    }

    if( btreeGetPage(pBt, 1, &pPage1, 0)==SQLITE_OK ){
      int nPage = get4byte(28+(u8*)pPage1->aData);
      testcase( nPage==0 );
      if( nPage==0 ) sqlite3PagerPagecount(pBt->pPager, &nPage);
      testcase( pBt->nPage!=nPage );
      pBt->nPage = nPage;
      releasePage(pPage1);
    }
    assert( countWriteCursors(pBt)==0 );
    pBt->inTransaction = TRANS_READ;
  }

  btreeEndTransaction(p);
  sqlite3BtreeLeave(p);
  return rc;
}

int sqlite3BtreeBeginStmt(Btree *p, int iStatement){
  int rc;
  BtShared *pBt = p->pBt;
  sqlite3BtreeEnter(p);
  assert( p->inTrans==TRANS_WRITE );
  assert( pBt->readOnly==0 );
  assert( iStatement>0 );
  assert( iStatement>p->db->nSavepoint );
  assert( pBt->inTransaction==TRANS_WRITE );
  rc = sqlite3PagerOpenSavepoint(pBt->pPager, iStatement);
  sqlite3BtreeLeave(p);
  return rc;
}

int sqlite3BtreeSavepoint(Btree *p, int op, int iSavepoint){
  int rc = SQLITE_OK;
  if( p && p->inTrans==TRANS_WRITE ){
    BtShared *pBt = p->pBt;
    assert( op==SAVEPOINT_RELEASE || op==SAVEPOINT_ROLLBACK );
    assert( iSavepoint>=0 || (iSavepoint==-1 && op==SAVEPOINT_ROLLBACK) );
    sqlite3BtreeEnter(p);
    rc = sqlite3PagerSavepoint(pBt->pPager, op, iSavepoint);
    if( rc==SQLITE_OK ){
      if( iSavepoint<0 && pBt->initiallyEmpty ) pBt->nPage = 0;
      rc = newDatabase(pBt);
      pBt->nPage = get4byte(28 + pBt->pPage1->aData);

      /* The database size was written into the offset 28 of the header
      ** when the transaction started, so we know that the value at offset
      ** 28 is nonzero. */
      assert( pBt->nPage>0 );
    }
    sqlite3BtreeLeave(p);
  }
  return rc;
}

static int btreeCursor(
  Btree *p,                              
  int iTable,                            
  int wrFlag,                            
  struct KeyInfo *pKeyInfo,              
  BtCursor *pCur                         
){
  BtShared *pBt = p->pBt;                

  assert( sqlite3BtreeHoldsMutex(p) );
  assert( wrFlag==0 || wrFlag==1 );

  assert( hasSharedCacheTableLock(p, iTable, pKeyInfo!=0, wrFlag+1) );
  assert( wrFlag==0 || !hasReadConflicts(p, iTable) );

  
  assert( p->inTrans>TRANS_NONE );
  assert( wrFlag==0 || p->inTrans==TRANS_WRITE );
  assert( pBt->pPage1 && pBt->pPage1->aData );

  if( NEVER(wrFlag && pBt->readOnly) ){
    return SQLITE_READONLY;
  }
  if( iTable==1 && btreePagecount(pBt)==0 ){
    return SQLITE_EMPTY;
  }

  pCur->pgnoRoot = (Pgno)iTable;
  pCur->iPage = -1;
  pCur->pKeyInfo = pKeyInfo;
  pCur->pBtree = p;
  pCur->pBt = pBt;
  pCur->wrFlag = (u8)wrFlag;
  pCur->pNext = pBt->pCursor;
  if( pCur->pNext ){
    pCur->pNext->pPrev = pCur;
  }
  pBt->pCursor = pCur;
  pCur->eState = CURSOR_INVALID;
  pCur->cachedRowid = 0;
  return SQLITE_OK;
}
int sqlite3BtreeCursor(
  Btree *p,                                   
  int iTable,                                 
  int wrFlag,                                 
  struct KeyInfo *pKeyInfo,                   
  BtCursor *pCur                              
){
  int rc;
  sqlite3BtreeEnter(p);
  rc = btreeCursor(p, iTable, wrFlag, pKeyInfo, pCur);
  sqlite3BtreeLeave(p);
  return rc;
}

int sqlite3BtreeCursorSize(void){
  return ROUND8(sizeof(BtCursor));
}

void sqlite3BtreeCursorZero(BtCursor *p){
  memset(p, 0, offsetof(BtCursor, iPage));
}

void sqlite3BtreeSetCachedRowid(BtCursor *pCur, sqlite3_int64 iRowid){
  BtCursor *p;
  for(p=pCur->pBt->pCursor; p; p=p->pNext){
    if( p->pgnoRoot==pCur->pgnoRoot ) p->cachedRowid = iRowid;
  }
  assert( pCur->cachedRowid==iRowid );
}

sqlite3_int64 sqlite3BtreeGetCachedRowid(BtCursor *pCur){
  return pCur->cachedRowid;
}

int sqlite3BtreeCloseCursor(BtCursor *pCur){
  Btree *pBtree = pCur->pBtree;
  if( pBtree ){
    int i;
    BtShared *pBt = pCur->pBt;
    sqlite3BtreeEnter(pBtree);
    sqlite3BtreeClearCursor(pCur);
    if( pCur->pPrev ){
      pCur->pPrev->pNext = pCur->pNext;
    }else{
      pBt->pCursor = pCur->pNext;
    }
    if( pCur->pNext ){
      pCur->pNext->pPrev = pCur->pPrev;
    }
    for(i=0; i<=pCur->iPage; i++){
      releasePage(pCur->apPage[i]);
    }
    unlockBtreeIfUnused(pBt);
    invalidateOverflowCache(pCur);
    
    sqlite3BtreeLeave(pBtree);
  }
  return SQLITE_OK;
}

#ifndef NDEBUG
  static void assertCellInfo(BtCursor *pCur){
    CellInfo info;
    int iPage = pCur->iPage;
    memset(&info, 0, sizeof(info));
    btreeParseCell(pCur->apPage[iPage], pCur->aiIdx[iPage], &info);
    assert( memcmp(&info, &pCur->info, sizeof(info))==0 );
  }
#else
  #define assertCellInfo(x)
#endif
#ifdef _MSC_VER
  
  static void getCellInfo(BtCursor *pCur){
    if( pCur->info.nSize==0 ){
      int iPage = pCur->iPage;
      btreeParseCell(pCur->apPage[iPage],pCur->aiIdx[iPage],&pCur->info);
      pCur->validNKey = 1;
    }else{
      assertCellInfo(pCur);
    }
  }
#else 
  
#define getCellInfo(pCur)                                                      \
  if( pCur->info.nSize==0 ){                                                   \
    int iPage = pCur->iPage;                                                   \
    btreeParseCell(pCur->apPage[iPage],pCur->aiIdx[iPage],&pCur->info); \
    pCur->validNKey = 1;                                                       \
  }else{                                                                       \
    assertCellInfo(pCur);                                                      \
  }
#endif 

#ifndef NDEBUG  
int sqlite3BtreeCursorIsValid(BtCursor *pCur){
  return pCur && pCur->eState==CURSOR_VALID;
}
#endif 

int sqlite3BtreeKeySize(BtCursor *pCur, i64 *pSize){
  assert( cursorHoldsMutex(pCur) );
  assert( pCur->eState==CURSOR_INVALID || pCur->eState==CURSOR_VALID );
  if( pCur->eState!=CURSOR_VALID ){
    *pSize = 0;
  }else{
    getCellInfo(pCur);
    *pSize = pCur->info.nKey;
  }
  return SQLITE_OK;
}

int sqlite3BtreeDataSize(BtCursor *pCur, u32 *pSize){
  assert( cursorHoldsMutex(pCur) );
  assert( pCur->eState==CURSOR_VALID );
  getCellInfo(pCur);
  *pSize = pCur->info.nData;
  return SQLITE_OK;
}

/*
** Given the page number of an overflow page in the database (parameter
** ovfl), this function finds the page number of the next page in the 
** linked list of overflow pages. If possible, it uses the auto-vacuum
** pointer-map data instead of reading the content of page ovfl to do so. 
**
** If an error occurs an SQLite error code is returned. Otherwise:
**
** The page number of the next overflow page in the linked list is 
** written to *pPgnoNext. If page ovfl is the last page in its linked 
** list, *pPgnoNext is set to zero. 
**
** If ppPage is not NULL, and a reference to the MemPage object corresponding
** to page number pOvfl was obtained, then *ppPage is set to point to that
** reference. It is the responsibility of the caller to call releasePage()
** on *ppPage to free the reference. In no reference was obtained (because
** the pointer-map was used to obtain the value for *pPgnoNext), then
** *ppPage is set to zero.
*/
static int getOverflowPage(
  BtShared *pBt,               
  Pgno ovfl,                   
  MemPage **ppPage,            
  Pgno *pPgnoNext              
){
  Pgno next = 0;
  MemPage *pPage = 0;
  int rc = SQLITE_OK;

  assert( sqlite3_mutex_held(pBt->mutex) );
  assert(pPgnoNext);

#ifndef SQLITE_OMIT_AUTOVACUUM
  if( pBt->autoVacuum ){
    Pgno pgno;
    Pgno iGuess = ovfl+1;
    u8 eType;

    while( PTRMAP_ISPAGE(pBt, iGuess) || iGuess==PENDING_BYTE_PAGE(pBt) ){
      iGuess++;
    }

    if( iGuess<=btreePagecount(pBt) ){
      rc = ptrmapGet(pBt, iGuess, &eType, &pgno);
      if( rc==SQLITE_OK && eType==PTRMAP_OVERFLOW2 && pgno==ovfl ){
        next = iGuess;
        rc = SQLITE_DONE;
      }
    }
  }
#endif

  assert( next==0 || rc==SQLITE_DONE );
  if( rc==SQLITE_OK ){
    rc = btreeGetPage(pBt, ovfl, &pPage, 0);
    assert( rc==SQLITE_OK || pPage==0 );
    if( rc==SQLITE_OK ){
      next = get4byte(pPage->aData);
    }
  }

  *pPgnoNext = next;
  if( ppPage ){
    *ppPage = pPage;
  }else{
    releasePage(pPage);
  }
  return (rc==SQLITE_DONE ? SQLITE_OK : rc);
}

static int copyPayload(
  void *pPayload,           
  void *pBuf,               
  int nByte,                
  int eOp,                  
  DbPage *pDbPage           
){
  if( eOp ){
    
    int rc = sqlite3PagerWrite(pDbPage);
    if( rc!=SQLITE_OK ){
      return rc;
    }
    memcpy(pPayload, pBuf, nByte);
  }else{
    
    memcpy(pBuf, pPayload, nByte);
  }
  return SQLITE_OK;
}

/*
** This function is used to read or overwrite payload information
** for the entry that the pCur cursor is pointing to. If the eOp
** parameter is 0, this is a read operation (data copied into
** buffer pBuf). If it is non-zero, a write (data copied from
** buffer pBuf).
**
** A total of "amt" bytes are read or written beginning at "offset".
** Data is read to or from the buffer pBuf.
**
** The content being read or written might appear on the main page
** or be scattered out on multiple overflow pages.
**
** If the BtCursor.isIncrblobHandle flag is set, and the current
** cursor entry uses one or more overflow pages, this function
** allocates space for and lazily popluates the overflow page-list 
** cache array (BtCursor.aOverflow). Subsequent calls use this
** cache to make seeking to the supplied offset more efficient.
**
** Once an overflow page-list cache has been allocated, it may be
** invalidated if some other cursor writes to the same table, or if
** the cursor is moved to a different row. Additionally, in auto-vacuum
** mode, the following events may invalidate an overflow page-list cache.
**
**   * An incremental vacuum,
**   * A commit in auto_vacuum="full" mode,
**   * Creating a table (may require moving an overflow page).
*/
static int accessPayload(
  BtCursor *pCur,      
  u32 offset,          
  u32 amt,             
  unsigned char *pBuf,  
  int eOp              
){
  unsigned char *aPayload;
  int rc = SQLITE_OK;
  u32 nKey;
  int iIdx = 0;
  MemPage *pPage = pCur->apPage[pCur->iPage]; 
  BtShared *pBt = pCur->pBt;                  

  assert( pPage );
  assert( pCur->eState==CURSOR_VALID );
  assert( pCur->aiIdx[pCur->iPage]<pPage->nCell );
  assert( cursorHoldsMutex(pCur) );

  getCellInfo(pCur);
  aPayload = pCur->info.pCell + pCur->info.nHeader;
  nKey = (pPage->intKey ? 0 : (int)pCur->info.nKey);

  if( NEVER(offset+amt > nKey+pCur->info.nData) 
   || &aPayload[pCur->info.nLocal] > &pPage->aData[pBt->usableSize]
  ){
    
    return SQLITE_CORRUPT_BKPT;
  }

  /* Check if data must be read/written to/from the btree page itself. */
  if( offset<pCur->info.nLocal ){
    int a = amt;
    if( a+offset>pCur->info.nLocal ){
      a = pCur->info.nLocal - offset;
    }
    rc = copyPayload(&aPayload[offset], pBuf, a, eOp, pPage->pDbPage);
    offset = 0;
    pBuf += a;
    amt -= a;
  }else{
    offset -= pCur->info.nLocal;
  }

  if( rc==SQLITE_OK && amt>0 ){
    const u32 ovflSize = pBt->usableSize - 4;  
    Pgno nextPage;

    nextPage = get4byte(&aPayload[pCur->info.nLocal]);

#ifndef SQLITE_OMIT_INCRBLOB
    if( pCur->isIncrblobHandle && !pCur->aOverflow ){
      int nOvfl = (pCur->info.nPayload-pCur->info.nLocal+ovflSize-1)/ovflSize;
      pCur->aOverflow = (Pgno *)sqlite3MallocZero(sizeof(Pgno)*nOvfl);
      if( ALWAYS(nOvfl) && !pCur->aOverflow ){
        rc = SQLITE_NOMEM;
      }
    }

    if( pCur->aOverflow && pCur->aOverflow[offset/ovflSize] ){
      iIdx = (offset/ovflSize);
      nextPage = pCur->aOverflow[iIdx];
      offset = (offset%ovflSize);
    }
#endif

    for( ; rc==SQLITE_OK && amt>0 && nextPage; iIdx++){

#ifndef SQLITE_OMIT_INCRBLOB
      
      if( pCur->aOverflow ){
        assert(!pCur->aOverflow[iIdx] || pCur->aOverflow[iIdx]==nextPage);
        pCur->aOverflow[iIdx] = nextPage;
      }
#endif

      if( offset>=ovflSize ){
#ifndef SQLITE_OMIT_INCRBLOB
        if( pCur->aOverflow && pCur->aOverflow[iIdx+1] ){
          nextPage = pCur->aOverflow[iIdx+1];
        } else 
#endif
          rc = getOverflowPage(pBt, nextPage, 0, &nextPage);
        offset -= ovflSize;
      }else{
        /* Need to read this page properly. It contains some of the
        ** range of data that is being read (eOp==0) or written (eOp!=0).
        */
        DbPage *pDbPage;
        int a = amt;
        rc = sqlite3PagerGet(pBt->pPager, nextPage, &pDbPage);
        if( rc==SQLITE_OK ){
          aPayload = sqlite3PagerGetData(pDbPage);
          nextPage = get4byte(aPayload);
          if( a + offset > ovflSize ){
            a = ovflSize - offset;
          }
          rc = copyPayload(&aPayload[offset+4], pBuf, a, eOp, pDbPage);
          sqlite3PagerUnref(pDbPage);
          offset = 0;
          amt -= a;
          pBuf += a;
        }
      }
    }
  }

  if( rc==SQLITE_OK && amt>0 ){
    return SQLITE_CORRUPT_BKPT;
  }
  return rc;
}

int sqlite3BtreeKey(BtCursor *pCur, u32 offset, u32 amt, void *pBuf){
  assert( cursorHoldsMutex(pCur) );
  assert( pCur->eState==CURSOR_VALID );
  assert( pCur->iPage>=0 && pCur->apPage[pCur->iPage] );
  assert( pCur->aiIdx[pCur->iPage]<pCur->apPage[pCur->iPage]->nCell );
  return accessPayload(pCur, offset, amt, (unsigned char*)pBuf, 0);
}

int sqlite3BtreeData(BtCursor *pCur, u32 offset, u32 amt, void *pBuf){
  int rc;

#ifndef SQLITE_OMIT_INCRBLOB
  if ( pCur->eState==CURSOR_INVALID ){
    return SQLITE_ABORT;
  }
#endif

  assert( cursorHoldsMutex(pCur) );
  rc = restoreCursorPosition(pCur);
  if( rc==SQLITE_OK ){
    assert( pCur->eState==CURSOR_VALID );
    assert( pCur->iPage>=0 && pCur->apPage[pCur->iPage] );
    assert( pCur->aiIdx[pCur->iPage]<pCur->apPage[pCur->iPage]->nCell );
    rc = accessPayload(pCur, offset, amt, pBuf, 0);
  }
  return rc;
}

/*
** Return a pointer to payload information from the entry that the 
** pCur cursor is pointing to.  The pointer is to the beginning of
** the key if skipKey==0 and it points to the beginning of data if
** skipKey==1.  The number of bytes of available key/data is written
** into *pAmt.  If *pAmt==0, then the value returned will not be
** a valid pointer.
**
** This routine is an optimization.  It is common for the entire key
** and data to fit on the local page and for there to be no overflow
** pages.  When that is so, this routine can be used to access the
** key and data without making a copy.  If the key and/or data spills
** onto overflow pages, then accessPayload() must be used to reassemble
** the key/data and copy it into a preallocated buffer.
**
** The pointer returned by this routine looks directly into the cached
** page of the database.  The data might change or move the next time
** any btree routine is called.
*/
static const unsigned char *fetchPayload(
  BtCursor *pCur,      
  int *pAmt,           
  int skipKey          
){
  unsigned char *aPayload;
  MemPage *pPage;
  u32 nKey;
  u32 nLocal;

  assert( pCur!=0 && pCur->iPage>=0 && pCur->apPage[pCur->iPage]);
  assert( pCur->eState==CURSOR_VALID );
  assert( cursorHoldsMutex(pCur) );
  pPage = pCur->apPage[pCur->iPage];
  assert( pCur->aiIdx[pCur->iPage]<pPage->nCell );
  if( NEVER(pCur->info.nSize==0) ){
    btreeParseCell(pCur->apPage[pCur->iPage], pCur->aiIdx[pCur->iPage],
                   &pCur->info);
  }
  aPayload = pCur->info.pCell;
  aPayload += pCur->info.nHeader;
  if( pPage->intKey ){
    nKey = 0;
  }else{
    nKey = (int)pCur->info.nKey;
  }
  if( skipKey ){
    aPayload += nKey;
    nLocal = pCur->info.nLocal - nKey;
  }else{
    nLocal = pCur->info.nLocal;
    assert( nLocal<=nKey );
  }
  *pAmt = nLocal;
  return aPayload;
}


const void *sqlite3BtreeKeyFetch(BtCursor *pCur, int *pAmt){
  const void *p = 0;
  assert( sqlite3_mutex_held(pCur->pBtree->db->mutex) );
  assert( cursorHoldsMutex(pCur) );
  if( ALWAYS(pCur->eState==CURSOR_VALID) ){
    p = (const void*)fetchPayload(pCur, pAmt, 0);
  }
  return p;
}
const void *sqlite3BtreeDataFetch(BtCursor *pCur, int *pAmt){
  const void *p = 0;
  assert( sqlite3_mutex_held(pCur->pBtree->db->mutex) );
  assert( cursorHoldsMutex(pCur) );
  if( ALWAYS(pCur->eState==CURSOR_VALID) ){
    p = (const void*)fetchPayload(pCur, pAmt, 1);
  }
  return p;
}


static int moveToChild(BtCursor *pCur, u32 newPgno){
  int rc;
  int i = pCur->iPage;
  MemPage *pNewPage;
  BtShared *pBt = pCur->pBt;

  assert( cursorHoldsMutex(pCur) );
  assert( pCur->eState==CURSOR_VALID );
  assert( pCur->iPage<BTCURSOR_MAX_DEPTH );
  if( pCur->iPage>=(BTCURSOR_MAX_DEPTH-1) ){
    return SQLITE_CORRUPT_BKPT;
  }
  rc = getAndInitPage(pBt, newPgno, &pNewPage);
  if( rc ) return rc;
  pCur->apPage[i+1] = pNewPage;
  pCur->aiIdx[i+1] = 0;
  pCur->iPage++;

  pCur->info.nSize = 0;
  pCur->validNKey = 0;
  if( pNewPage->nCell<1 || pNewPage->intKey!=pCur->apPage[i]->intKey ){
    return SQLITE_CORRUPT_BKPT;
  }
  return SQLITE_OK;
}

#ifndef NDEBUG
static void assertParentIndex(MemPage *pParent, int iIdx, Pgno iChild){
  assert( iIdx<=pParent->nCell );
  if( iIdx==pParent->nCell ){
    assert( get4byte(&pParent->aData[pParent->hdrOffset+8])==iChild );
  }else{
    assert( get4byte(findCell(pParent, iIdx))==iChild );
  }
}
#else
#  define assertParentIndex(x,y,z) 
#endif

static void moveToParent(BtCursor *pCur){
  assert( cursorHoldsMutex(pCur) );
  assert( pCur->eState==CURSOR_VALID );
  assert( pCur->iPage>0 );
  assert( pCur->apPage[pCur->iPage] );
  assertParentIndex(
    pCur->apPage[pCur->iPage-1], 
    pCur->aiIdx[pCur->iPage-1], 
    pCur->apPage[pCur->iPage]->pgno
  );
  releasePage(pCur->apPage[pCur->iPage]);
  pCur->iPage--;
  pCur->info.nSize = 0;
  pCur->validNKey = 0;
}

static int moveToRoot(BtCursor *pCur){
  MemPage *pRoot;
  int rc = SQLITE_OK;
  Btree *p = pCur->pBtree;
  BtShared *pBt = p->pBt;

  assert( cursorHoldsMutex(pCur) );
  assert( CURSOR_INVALID < CURSOR_REQUIRESEEK );
  assert( CURSOR_VALID   < CURSOR_REQUIRESEEK );
  assert( CURSOR_FAULT   > CURSOR_REQUIRESEEK );
  if( pCur->eState>=CURSOR_REQUIRESEEK ){
    if( pCur->eState==CURSOR_FAULT ){
      assert( pCur->skipNext!=SQLITE_OK );
      return pCur->skipNext;
    }
    sqlite3BtreeClearCursor(pCur);
  }

  if( pCur->iPage>=0 ){
    int i;
    for(i=1; i<=pCur->iPage; i++){
      releasePage(pCur->apPage[i]);
    }
    pCur->iPage = 0;
  }else{
    rc = getAndInitPage(pBt, pCur->pgnoRoot, &pCur->apPage[0]);
    if( rc!=SQLITE_OK ){
      pCur->eState = CURSOR_INVALID;
      return rc;
    }
    pCur->iPage = 0;

    assert( pCur->apPage[0]->intKey==1 || pCur->apPage[0]->intKey==0 );
    if( (pCur->pKeyInfo==0)!=pCur->apPage[0]->intKey ){
      return SQLITE_CORRUPT_BKPT;
    }
  }

  pRoot = pCur->apPage[0];
  assert( pRoot->pgno==pCur->pgnoRoot );
  assert( pRoot->isInit && (pCur->pKeyInfo==0)==pRoot->intKey );

  pCur->aiIdx[0] = 0;
  pCur->info.nSize = 0;
  pCur->atLast = 0;
  pCur->validNKey = 0;

  if( pRoot->nCell==0 && !pRoot->leaf ){
    Pgno subpage;
    if( pRoot->pgno!=1 ) return SQLITE_CORRUPT_BKPT;
    subpage = get4byte(&pRoot->aData[pRoot->hdrOffset+8]);
    pCur->eState = CURSOR_VALID;
    rc = moveToChild(pCur, subpage);
  }else{
    pCur->eState = ((pRoot->nCell>0)?CURSOR_VALID:CURSOR_INVALID);
  }
  return rc;
}

static int moveToLeftmost(BtCursor *pCur){
  Pgno pgno;
  int rc = SQLITE_OK;
  MemPage *pPage;

  assert( cursorHoldsMutex(pCur) );
  assert( pCur->eState==CURSOR_VALID );
  while( rc==SQLITE_OK && !(pPage = pCur->apPage[pCur->iPage])->leaf ){
    assert( pCur->aiIdx[pCur->iPage]<pPage->nCell );
    pgno = get4byte(findCell(pPage, pCur->aiIdx[pCur->iPage]));
    rc = moveToChild(pCur, pgno);
  }
  return rc;
}

static int moveToRightmost(BtCursor *pCur){
  Pgno pgno;
  int rc = SQLITE_OK;
  MemPage *pPage = 0;

  assert( cursorHoldsMutex(pCur) );
  assert( pCur->eState==CURSOR_VALID );
  while( rc==SQLITE_OK && !(pPage = pCur->apPage[pCur->iPage])->leaf ){
    pgno = get4byte(&pPage->aData[pPage->hdrOffset+8]);
    pCur->aiIdx[pCur->iPage] = pPage->nCell;
    rc = moveToChild(pCur, pgno);
  }
  if( rc==SQLITE_OK ){
    pCur->aiIdx[pCur->iPage] = pPage->nCell-1;
    pCur->info.nSize = 0;
    pCur->validNKey = 0;
  }
  return rc;
}

int sqlite3BtreeFirst(BtCursor *pCur, int *pRes){
  int rc;

  assert( cursorHoldsMutex(pCur) );
  assert( sqlite3_mutex_held(pCur->pBtree->db->mutex) );
  rc = moveToRoot(pCur);
  if( rc==SQLITE_OK ){
    if( pCur->eState==CURSOR_INVALID ){
      assert( pCur->apPage[pCur->iPage]->nCell==0 );
      *pRes = 1;
    }else{
      assert( pCur->apPage[pCur->iPage]->nCell>0 );
      *pRes = 0;
      rc = moveToLeftmost(pCur);
    }
  }
  return rc;
}

int sqlite3BtreeLast(BtCursor *pCur, int *pRes){
  int rc;
 
  assert( cursorHoldsMutex(pCur) );
  assert( sqlite3_mutex_held(pCur->pBtree->db->mutex) );

  
  if( CURSOR_VALID==pCur->eState && pCur->atLast ){
#ifdef SQLITE_DEBUG
    int ii;
    for(ii=0; ii<pCur->iPage; ii++){
      assert( pCur->aiIdx[ii]==pCur->apPage[ii]->nCell );
    }
    assert( pCur->aiIdx[pCur->iPage]==pCur->apPage[pCur->iPage]->nCell-1 );
    assert( pCur->apPage[pCur->iPage]->leaf );
#endif
    return SQLITE_OK;
  }

  rc = moveToRoot(pCur);
  if( rc==SQLITE_OK ){
    if( CURSOR_INVALID==pCur->eState ){
      assert( pCur->apPage[pCur->iPage]->nCell==0 );
      *pRes = 1;
    }else{
      assert( pCur->eState==CURSOR_VALID );
      *pRes = 0;
      rc = moveToRightmost(pCur);
      pCur->atLast = rc==SQLITE_OK ?1:0;
    }
  }
  return rc;
}

/* Move the cursor so that it points to an entry near the key 
** specified by pIdxKey or intKey.   Return a success code.
**
** For INTKEY tables, the intKey parameter is used.  pIdxKey 
** must be NULL.  For index tables, pIdxKey is used and intKey
** is ignored.
**
** If an exact match is not found, then the cursor is always
** left pointing at a leaf page which would hold the entry if it
** were present.  The cursor might point to an entry that comes
** before or after the key.
**
** An integer is written into *pRes which is the result of
** comparing the key with the entry to which the cursor is 
** pointing.  The meaning of the integer written into
** *pRes is as follows:
**
**     *pRes<0      The cursor is left pointing at an entry that
**                  is smaller than intKey/pIdxKey or if the table is empty
**                  and the cursor is therefore left point to nothing.
**
**     *pRes==0     The cursor is left pointing at an entry that
**                  exactly matches intKey/pIdxKey.
**
**     *pRes>0      The cursor is left pointing at an entry that
**                  is larger than intKey/pIdxKey.
**
*/
int sqlite3BtreeMovetoUnpacked(
  BtCursor *pCur,          
  UnpackedRecord *pIdxKey, 
  i64 intKey,              
  int biasRight,           
  int *pRes                
){
  int rc;

  assert( cursorHoldsMutex(pCur) );
  assert( sqlite3_mutex_held(pCur->pBtree->db->mutex) );
  assert( pRes );
  assert( (pIdxKey==0)==(pCur->pKeyInfo==0) );

  if( pCur->eState==CURSOR_VALID && pCur->validNKey 
   && pCur->apPage[0]->intKey 
  ){
    if( pCur->info.nKey==intKey ){
      *pRes = 0;
      return SQLITE_OK;
    }
    if( pCur->atLast && pCur->info.nKey<intKey ){
      *pRes = -1;
      return SQLITE_OK;
    }
  }

  rc = moveToRoot(pCur);
  if( rc ){
    return rc;
  }
  assert( pCur->apPage[pCur->iPage] );
  assert( pCur->apPage[pCur->iPage]->isInit );
  assert( pCur->apPage[pCur->iPage]->nCell>0 || pCur->eState==CURSOR_INVALID );
  if( pCur->eState==CURSOR_INVALID ){
    *pRes = -1;
    assert( pCur->apPage[pCur->iPage]->nCell==0 );
    return SQLITE_OK;
  }
  assert( pCur->apPage[0]->intKey || pIdxKey );
  for(;;){
    int lwr, upr;
    Pgno chldPg;
    MemPage *pPage = pCur->apPage[pCur->iPage];
    int c;

    assert( pPage->nCell>0 );
    assert( pPage->intKey==(pIdxKey==0) );
    lwr = 0;
    upr = pPage->nCell-1;
    if( biasRight ){
      pCur->aiIdx[pCur->iPage] = (u16)upr;
    }else{
      pCur->aiIdx[pCur->iPage] = (u16)((upr+lwr)/2);
    }
    for(;;){
      int idx = pCur->aiIdx[pCur->iPage]; 
      u8 *pCell;                          

      pCur->info.nSize = 0;
      pCell = findCell(pPage, idx) + pPage->childPtrSize;
      if( pPage->intKey ){
        i64 nCellKey;
        if( pPage->hasData ){
          u32 dummy;
          pCell += getVarint32(pCell, dummy);
        }
        getVarint(pCell, (u64*)&nCellKey);
        if( nCellKey==intKey ){
          c = 0;
        }else if( nCellKey<intKey ){
          c = -1;
        }else{
          assert( nCellKey>intKey );
          c = +1;
        }
        pCur->validNKey = 1;
        pCur->info.nKey = nCellKey;
      }else{
        int nCell = pCell[0];
        if( !(nCell & 0x80) && nCell<=pPage->maxLocal ){
          c = sqlite3VdbeRecordCompare(nCell, (void*)&pCell[1], pIdxKey);
        }else if( !(pCell[1] & 0x80) 
          && (nCell = ((nCell&0x7f)<<7) + pCell[1])<=pPage->maxLocal
        ){
          c = sqlite3VdbeRecordCompare(nCell, (void*)&pCell[2], pIdxKey);
        }else{
          void *pCellKey;
          u8 * const pCellBody = pCell - pPage->childPtrSize;
          btreeParseCellPtr(pPage, pCellBody, &pCur->info);
          nCell = (int)pCur->info.nKey;
          pCellKey = sqlite3Malloc( nCell );
          if( pCellKey==0 ){
            rc = SQLITE_NOMEM;
            goto moveto_finish;
          }
          rc = accessPayload(pCur, 0, nCell, (unsigned char*)pCellKey, 0);
          if( rc ){
            sqlite3_free(pCellKey);
            goto moveto_finish;
          }
          c = sqlite3VdbeRecordCompare(nCell, pCellKey, pIdxKey);
          sqlite3_free(pCellKey);
        }
      }
      if( c==0 ){
        if( pPage->intKey && !pPage->leaf ){
          lwr = idx;
          upr = lwr - 1;
          break;
        }else{
          *pRes = 0;
          rc = SQLITE_OK;
          goto moveto_finish;
        }
      }
      if( c<0 ){
        lwr = idx+1;
      }else{
        upr = idx-1;
      }
      if( lwr>upr ){
        break;
      }
      pCur->aiIdx[pCur->iPage] = (u16)((lwr+upr)/2);
    }
    assert( lwr==upr+1 );
    assert( pPage->isInit );
    if( pPage->leaf ){
      chldPg = 0;
    }else if( lwr>=pPage->nCell ){
      chldPg = get4byte(&pPage->aData[pPage->hdrOffset+8]);
    }else{
      chldPg = get4byte(findCell(pPage, lwr));
    }
    if( chldPg==0 ){
      assert( pCur->aiIdx[pCur->iPage]<pCur->apPage[pCur->iPage]->nCell );
      *pRes = c;
      rc = SQLITE_OK;
      goto moveto_finish;
    }
    pCur->aiIdx[pCur->iPage] = (u16)lwr;
    pCur->info.nSize = 0;
    pCur->validNKey = 0;
    rc = moveToChild(pCur, chldPg);
    if( rc ) goto moveto_finish;
  }
moveto_finish:
  return rc;
}


int sqlite3BtreeEof(BtCursor *pCur){
  return (CURSOR_VALID!=pCur->eState);
}

int sqlite3BtreeNext(BtCursor *pCur, int *pRes){
  int rc;
  int idx;
  MemPage *pPage;

  assert( cursorHoldsMutex(pCur) );
  rc = restoreCursorPosition(pCur);
  if( rc!=SQLITE_OK ){
    return rc;
  }
  assert( pRes!=0 );
  if( CURSOR_INVALID==pCur->eState ){
    *pRes = 1;
    return SQLITE_OK;
  }
  if( pCur->skipNext>0 ){
    pCur->skipNext = 0;
    *pRes = 0;
    return SQLITE_OK;
  }
  pCur->skipNext = 0;

  pPage = pCur->apPage[pCur->iPage];
  idx = ++pCur->aiIdx[pCur->iPage];
  assert( pPage->isInit );
  assert( idx<=pPage->nCell );

  pCur->info.nSize = 0;
  pCur->validNKey = 0;
  if( idx>=pPage->nCell ){
    if( !pPage->leaf ){
      rc = moveToChild(pCur, get4byte(&pPage->aData[pPage->hdrOffset+8]));
      if( rc ) return rc;
      rc = moveToLeftmost(pCur);
      *pRes = 0;
      return rc;
    }
    do{
      if( pCur->iPage==0 ){
        *pRes = 1;
        pCur->eState = CURSOR_INVALID;
        return SQLITE_OK;
      }
      moveToParent(pCur);
      pPage = pCur->apPage[pCur->iPage];
    }while( pCur->aiIdx[pCur->iPage]>=pPage->nCell );
    *pRes = 0;
    if( pPage->intKey ){
      rc = sqlite3BtreeNext(pCur, pRes);
    }else{
      rc = SQLITE_OK;
    }
    return rc;
  }
  *pRes = 0;
  if( pPage->leaf ){
    return SQLITE_OK;
  }
  rc = moveToLeftmost(pCur);
  return rc;
}


int sqlite3BtreePrevious(BtCursor *pCur, int *pRes){
  int rc;
  MemPage *pPage;

  assert( cursorHoldsMutex(pCur) );
  rc = restoreCursorPosition(pCur);
  if( rc!=SQLITE_OK ){
    return rc;
  }
  pCur->atLast = 0;
  if( CURSOR_INVALID==pCur->eState ){
    *pRes = 1;
    return SQLITE_OK;
  }
  if( pCur->skipNext<0 ){
    pCur->skipNext = 0;
    *pRes = 0;
    return SQLITE_OK;
  }
  pCur->skipNext = 0;

  pPage = pCur->apPage[pCur->iPage];
  assert( pPage->isInit );
  if( !pPage->leaf ){
    int idx = pCur->aiIdx[pCur->iPage];
    rc = moveToChild(pCur, get4byte(findCell(pPage, idx)));
    if( rc ){
      return rc;
    }
    rc = moveToRightmost(pCur);
  }else{
    while( pCur->aiIdx[pCur->iPage]==0 ){
      if( pCur->iPage==0 ){
        pCur->eState = CURSOR_INVALID;
        *pRes = 1;
        return SQLITE_OK;
      }
      moveToParent(pCur);
    }
    pCur->info.nSize = 0;
    pCur->validNKey = 0;

    pCur->aiIdx[pCur->iPage]--;
    pPage = pCur->apPage[pCur->iPage];
    if( pPage->intKey && !pPage->leaf ){
      rc = sqlite3BtreePrevious(pCur, pRes);
    }else{
      rc = SQLITE_OK;
    }
  }
  *pRes = 0;
  return rc;
}

static int allocateBtreePage(
  BtShared *pBt, 
  MemPage **ppPage, 
  Pgno *pPgno, 
  Pgno nearby,
  u8 exact
){
  MemPage *pPage1;
  int rc;
  u32 n;     
  u32 k;     
  MemPage *pTrunk = 0;
  MemPage *pPrevTrunk = 0;
  Pgno mxPage;     

  assert( sqlite3_mutex_held(pBt->mutex) );
  pPage1 = pBt->pPage1;
  mxPage = btreePagecount(pBt);
  n = get4byte(&pPage1->aData[36]);
  testcase( n==mxPage-1 );
  if( n>=mxPage ){
    return SQLITE_CORRUPT_BKPT;
  }
  if( n>0 ){
    
    Pgno iTrunk;
    u8 searchList = 0; 
    
#ifndef SQLITE_OMIT_AUTOVACUUM
    if( exact && nearby<=mxPage ){
      u8 eType;
      assert( nearby>0 );
      assert( pBt->autoVacuum );
      rc = ptrmapGet(pBt, nearby, &eType, 0);
      if( rc ) return rc;
      if( eType==PTRMAP_FREEPAGE ){
        searchList = 1;
      }
      *pPgno = nearby;
    }
#endif

    rc = sqlite3PagerWrite(pPage1->pDbPage);
    if( rc ) return rc;
    put4byte(&pPage1->aData[36], n-1);

    do {
      pPrevTrunk = pTrunk;
      if( pPrevTrunk ){
        iTrunk = get4byte(&pPrevTrunk->aData[0]);
      }else{
        iTrunk = get4byte(&pPage1->aData[32]);
      }
      testcase( iTrunk==mxPage );
      if( iTrunk>mxPage ){
        rc = SQLITE_CORRUPT_BKPT;
      }else{
        rc = btreeGetPage(pBt, iTrunk, &pTrunk, 0);
      }
      if( rc ){
        pTrunk = 0;
        goto end_allocate_page;
      }

      k = get4byte(&pTrunk->aData[4]); 
      if( k==0 && !searchList ){
        assert( pPrevTrunk==0 );
        rc = sqlite3PagerWrite(pTrunk->pDbPage);
        if( rc ){
          goto end_allocate_page;
        }
        *pPgno = iTrunk;
        memcpy(&pPage1->aData[32], &pTrunk->aData[0], 4);
        *ppPage = pTrunk;
        pTrunk = 0;
        TRACE(("ALLOCATE: %d trunk - %d free pages left\n", *pPgno, n-1));
      }else if( k>(u32)(pBt->usableSize/4 - 2) ){
        
        rc = SQLITE_CORRUPT_BKPT;
        goto end_allocate_page;
#ifndef SQLITE_OMIT_AUTOVACUUM
      }else if( searchList && nearby==iTrunk ){
        assert( *pPgno==iTrunk );
        *ppPage = pTrunk;
        searchList = 0;
        rc = sqlite3PagerWrite(pTrunk->pDbPage);
        if( rc ){
          goto end_allocate_page;
        }
        if( k==0 ){
          if( !pPrevTrunk ){
            memcpy(&pPage1->aData[32], &pTrunk->aData[0], 4);
          }else{
            rc = sqlite3PagerWrite(pPrevTrunk->pDbPage);
            if( rc!=SQLITE_OK ){
              goto end_allocate_page;
            }
            memcpy(&pPrevTrunk->aData[0], &pTrunk->aData[0], 4);
          }
        }else{
          MemPage *pNewTrunk;
          Pgno iNewTrunk = get4byte(&pTrunk->aData[8]);
          if( iNewTrunk>mxPage ){ 
            rc = SQLITE_CORRUPT_BKPT;
            goto end_allocate_page;
          }
          testcase( iNewTrunk==mxPage );
          rc = btreeGetPage(pBt, iNewTrunk, &pNewTrunk, 0);
          if( rc!=SQLITE_OK ){
            goto end_allocate_page;
          }
          rc = sqlite3PagerWrite(pNewTrunk->pDbPage);
          if( rc!=SQLITE_OK ){
            releasePage(pNewTrunk);
            goto end_allocate_page;
          }
          memcpy(&pNewTrunk->aData[0], &pTrunk->aData[0], 4);
          put4byte(&pNewTrunk->aData[4], k-1);
          memcpy(&pNewTrunk->aData[8], &pTrunk->aData[12], (k-1)*4);
          releasePage(pNewTrunk);
          if( !pPrevTrunk ){
            assert( sqlite3PagerIswriteable(pPage1->pDbPage) );
            put4byte(&pPage1->aData[32], iNewTrunk);
          }else{
            rc = sqlite3PagerWrite(pPrevTrunk->pDbPage);
            if( rc ){
              goto end_allocate_page;
            }
            put4byte(&pPrevTrunk->aData[0], iNewTrunk);
          }
        }
        pTrunk = 0;
        TRACE(("ALLOCATE: %d trunk - %d free pages left\n", *pPgno, n-1));
#endif
      }else if( k>0 ){
        
        u32 closest;
        Pgno iPage;
        unsigned char *aData = pTrunk->aData;
        if( nearby>0 ){
          u32 i;
          int dist;
          closest = 0;
          dist = sqlite3AbsInt32(get4byte(&aData[8]) - nearby);
          for(i=1; i<k; i++){
            int d2 = sqlite3AbsInt32(get4byte(&aData[8+i*4]) - nearby);
            if( d2<dist ){
              closest = i;
              dist = d2;
            }
          }
        }else{
          closest = 0;
        }

        iPage = get4byte(&aData[8+closest*4]);
        testcase( iPage==mxPage );
        if( iPage>mxPage ){
          rc = SQLITE_CORRUPT_BKPT;
          goto end_allocate_page;
        }
        testcase( iPage==mxPage );
        if( !searchList || iPage==nearby ){
          int noContent;
          *pPgno = iPage;
          TRACE(("ALLOCATE: %d was leaf %d of %d on trunk %d"
                 ": %d more free pages\n",
                 *pPgno, closest+1, k, pTrunk->pgno, n-1));
          rc = sqlite3PagerWrite(pTrunk->pDbPage);
          if( rc ) goto end_allocate_page;
          if( closest<k-1 ){
            memcpy(&aData[8+closest*4], &aData[4+k*4], 4);
          }
          put4byte(&aData[4], k-1);
          noContent = !btreeGetHasContent(pBt, *pPgno);
          rc = btreeGetPage(pBt, *pPgno, ppPage, noContent);
          if( rc==SQLITE_OK ){
            rc = sqlite3PagerWrite((*ppPage)->pDbPage);
            if( rc!=SQLITE_OK ){
              releasePage(*ppPage);
            }
          }
          searchList = 0;
        }
      }
      releasePage(pPrevTrunk);
      pPrevTrunk = 0;
    }while( searchList );
  }else{
    rc = sqlite3PagerWrite(pBt->pPage1->pDbPage);
    if( rc ) return rc;
    pBt->nPage++;
    if( pBt->nPage==PENDING_BYTE_PAGE(pBt) ) pBt->nPage++;

#ifndef SQLITE_OMIT_AUTOVACUUM
    if( pBt->autoVacuum && PTRMAP_ISPAGE(pBt, pBt->nPage) ){
      MemPage *pPg = 0;
      TRACE(("ALLOCATE: %d from end of file (pointer-map page)\n", pBt->nPage));
      assert( pBt->nPage!=PENDING_BYTE_PAGE(pBt) );
      rc = btreeGetPage(pBt, pBt->nPage, &pPg, 1);
      if( rc==SQLITE_OK ){
        rc = sqlite3PagerWrite(pPg->pDbPage);
        releasePage(pPg);
      }
      if( rc ) return rc;
      pBt->nPage++;
      if( pBt->nPage==PENDING_BYTE_PAGE(pBt) ){ pBt->nPage++; }
    }
#endif
    put4byte(28 + (u8*)pBt->pPage1->aData, pBt->nPage);
    *pPgno = pBt->nPage;

    assert( *pPgno!=PENDING_BYTE_PAGE(pBt) );
    rc = btreeGetPage(pBt, *pPgno, ppPage, 1);
    if( rc ) return rc;
    rc = sqlite3PagerWrite((*ppPage)->pDbPage);
    if( rc!=SQLITE_OK ){
      releasePage(*ppPage);
    }
    TRACE(("ALLOCATE: %d from end of file\n", *pPgno));
  }

  assert( *pPgno!=PENDING_BYTE_PAGE(pBt) );

end_allocate_page:
  releasePage(pTrunk);
  releasePage(pPrevTrunk);
  if( rc==SQLITE_OK ){
    if( sqlite3PagerPageRefcount((*ppPage)->pDbPage)>1 ){
      releasePage(*ppPage);
      return SQLITE_CORRUPT_BKPT;
    }
    (*ppPage)->isInit = 0;
  }else{
    *ppPage = 0;
  }
  assert( rc!=SQLITE_OK || sqlite3PagerIswriteable((*ppPage)->pDbPage) );
  return rc;
}

static int freePage2(BtShared *pBt, MemPage *pMemPage, Pgno iPage){
  MemPage *pTrunk = 0;                
  Pgno iTrunk = 0;                     
  MemPage *pPage1 = pBt->pPage1;      
  MemPage *pPage;                     
  int rc;                             
  int nFree;                          

  assert( sqlite3_mutex_held(pBt->mutex) );
  assert( iPage>1 );
  assert( !pMemPage || pMemPage->pgno==iPage );

  if( pMemPage ){
    pPage = pMemPage;
    sqlite3PagerRef(pPage->pDbPage);
  }else{
    pPage = btreePageLookup(pBt, iPage);
  }

  
  rc = sqlite3PagerWrite(pPage1->pDbPage);
  if( rc ) goto freepage_out;
  nFree = get4byte(&pPage1->aData[36]);
  put4byte(&pPage1->aData[36], nFree+1);

  if( pBt->secureDelete ){
    if( (!pPage && ((rc = btreeGetPage(pBt, iPage, &pPage, 0))!=0) )
     ||            ((rc = sqlite3PagerWrite(pPage->pDbPage))!=0)
    ){
      goto freepage_out;
    }
    memset(pPage->aData, 0, pPage->pBt->pageSize);
  }

  if( ISAUTOVACUUM ){
    ptrmapPut(pBt, iPage, PTRMAP_FREEPAGE, 0, &rc);
    if( rc ) goto freepage_out;
  }

  if( nFree!=0 ){
    u32 nLeaf;                

    iTrunk = get4byte(&pPage1->aData[32]);
    rc = btreeGetPage(pBt, iTrunk, &pTrunk, 0);
    if( rc!=SQLITE_OK ){
      goto freepage_out;
    }

    nLeaf = get4byte(&pTrunk->aData[4]);
    assert( pBt->usableSize>32 );
    if( nLeaf > (u32)pBt->usableSize/4 - 2 ){
      rc = SQLITE_CORRUPT_BKPT;
      goto freepage_out;
    }
    if( nLeaf < (u32)pBt->usableSize/4 - 8 ){
      rc = sqlite3PagerWrite(pTrunk->pDbPage);
      if( rc==SQLITE_OK ){
        put4byte(&pTrunk->aData[4], nLeaf+1);
        put4byte(&pTrunk->aData[8+nLeaf*4], iPage);
        if( pPage && !pBt->secureDelete ){
          sqlite3PagerDontWrite(pPage->pDbPage);
        }
        rc = btreeSetHasContent(pBt, iPage);
      }
      TRACE(("FREE-PAGE: %d leaf on trunk page %d\n",pPage->pgno,pTrunk->pgno));
      goto freepage_out;
    }
  }

  if( pPage==0 && SQLITE_OK!=(rc = btreeGetPage(pBt, iPage, &pPage, 0)) ){
    goto freepage_out;
  }
  rc = sqlite3PagerWrite(pPage->pDbPage);
  if( rc!=SQLITE_OK ){
    goto freepage_out;
  }
  put4byte(pPage->aData, iTrunk);
  put4byte(&pPage->aData[4], 0);
  put4byte(&pPage1->aData[32], iPage);
  TRACE(("FREE-PAGE: %d new trunk page replacing %d\n", pPage->pgno, iTrunk));

freepage_out:
  if( pPage ){
    pPage->isInit = 0;
  }
  releasePage(pPage);
  releasePage(pTrunk);
  return rc;
}
static void freePage(MemPage *pPage, int *pRC){
  if( (*pRC)==SQLITE_OK ){
    *pRC = freePage2(pPage->pBt, pPage, pPage->pgno);
  }
}

static int clearCell(MemPage *pPage, unsigned char *pCell){
  BtShared *pBt = pPage->pBt;
  CellInfo info;
  Pgno ovflPgno;
  int rc;
  int nOvfl;
  u32 ovflPageSize;

  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  btreeParseCellPtr(pPage, pCell, &info);
  if( info.iOverflow==0 ){
    return SQLITE_OK;  
  }
  ovflPgno = get4byte(&pCell[info.iOverflow]);
  assert( pBt->usableSize > 4 );
  ovflPageSize = pBt->usableSize - 4;
  nOvfl = (info.nPayload - info.nLocal + ovflPageSize - 1)/ovflPageSize;
  assert( ovflPgno==0 || nOvfl>0 );
  while( nOvfl-- ){
    Pgno iNext = 0;
    MemPage *pOvfl = 0;
    if( ovflPgno<2 || ovflPgno>btreePagecount(pBt) ){
      return SQLITE_CORRUPT_BKPT;
    }
    if( nOvfl ){
      rc = getOverflowPage(pBt, ovflPgno, &pOvfl, &iNext);
      if( rc ) return rc;
    }

    if( ( pOvfl || ((pOvfl = btreePageLookup(pBt, ovflPgno))!=0) )
     && sqlite3PagerPageRefcount(pOvfl->pDbPage)!=1
    ){
      rc = SQLITE_CORRUPT_BKPT;
    }else{
      rc = freePage2(pBt, pOvfl, ovflPgno);
    }

    if( pOvfl ){
      sqlite3PagerUnref(pOvfl->pDbPage);
    }
    if( rc ) return rc;
    ovflPgno = iNext;
  }
  return SQLITE_OK;
}

static int fillInCell(
  MemPage *pPage,                
  unsigned char *pCell,          
  const void *pKey, i64 nKey,    
  const void *pData,int nData,   
  int nZero,                     
  int *pnSize                    
){
  int nPayload;
  const u8 *pSrc;
  int nSrc, n, rc;
  int spaceLeft;
  MemPage *pOvfl = 0;
  MemPage *pToRelease = 0;
  unsigned char *pPrior;
  unsigned char *pPayload;
  BtShared *pBt = pPage->pBt;
  Pgno pgnoOvfl = 0;
  int nHeader;
  CellInfo info;

  assert( sqlite3_mutex_held(pPage->pBt->mutex) );

  assert( pCell<pPage->aData || pCell>=&pPage->aData[pBt->pageSize]
            || sqlite3PagerIswriteable(pPage->pDbPage) );

  
  nHeader = 0;
  if( !pPage->leaf ){
    nHeader += 4;
  }
  if( pPage->hasData ){
    nHeader += putVarint(&pCell[nHeader], nData+nZero);
  }else{
    nData = nZero = 0;
  }
  nHeader += putVarint(&pCell[nHeader], *(u64*)&nKey);
  btreeParseCellPtr(pPage, pCell, &info);
  assert( info.nHeader==nHeader );
  assert( info.nKey==nKey );
  assert( info.nData==(u32)(nData+nZero) );
  
  
  nPayload = nData + nZero;
  if( pPage->intKey ){
    pSrc = pData;
    nSrc = nData;
    nData = 0;
  }else{ 
    if( NEVER(nKey>0x7fffffff || pKey==0) ){
      return SQLITE_CORRUPT_BKPT;
    }
    nPayload += (int)nKey;
    pSrc = pKey;
    nSrc = (int)nKey;
  }
  *pnSize = info.nSize;
  spaceLeft = info.nLocal;
  pPayload = &pCell[nHeader];
  pPrior = &pCell[info.iOverflow];

  while( nPayload>0 ){
    if( spaceLeft==0 ){
#ifndef SQLITE_OMIT_AUTOVACUUM
      Pgno pgnoPtrmap = pgnoOvfl; 
      if( pBt->autoVacuum ){
        do{
          pgnoOvfl++;
        } while( 
          PTRMAP_ISPAGE(pBt, pgnoOvfl) || pgnoOvfl==PENDING_BYTE_PAGE(pBt) 
        );
      }
#endif
      rc = allocateBtreePage(pBt, &pOvfl, &pgnoOvfl, pgnoOvfl, 0);
#ifndef SQLITE_OMIT_AUTOVACUUM
      if( pBt->autoVacuum && rc==SQLITE_OK ){
        u8 eType = (pgnoPtrmap?PTRMAP_OVERFLOW2:PTRMAP_OVERFLOW1);
        ptrmapPut(pBt, pgnoOvfl, eType, pgnoPtrmap, &rc);
        if( rc ){
          releasePage(pOvfl);
        }
      }
#endif
      if( rc ){
        releasePage(pToRelease);
        return rc;
      }

      assert( pToRelease==0 || sqlite3PagerIswriteable(pToRelease->pDbPage) );

      assert( pPrior<pPage->aData || pPrior>=&pPage->aData[pBt->pageSize]
            || sqlite3PagerIswriteable(pPage->pDbPage) );

      put4byte(pPrior, pgnoOvfl);
      releasePage(pToRelease);
      pToRelease = pOvfl;
      pPrior = pOvfl->aData;
      put4byte(pPrior, 0);
      pPayload = &pOvfl->aData[4];
      spaceLeft = pBt->usableSize - 4;
    }
    n = nPayload;
    if( n>spaceLeft ) n = spaceLeft;

    assert( pToRelease==0 || sqlite3PagerIswriteable(pToRelease->pDbPage) );

    assert( pPayload<pPage->aData || pPayload>=&pPage->aData[pBt->pageSize]
            || sqlite3PagerIswriteable(pPage->pDbPage) );

    if( nSrc>0 ){
      if( n>nSrc ) n = nSrc;
      assert( pSrc );
      memcpy(pPayload, pSrc, n);
    }else{
      memset(pPayload, 0, n);
    }
    nPayload -= n;
    pPayload += n;
    pSrc += n;
    nSrc -= n;
    spaceLeft -= n;
    if( nSrc==0 ){
      nSrc = nData;
      pSrc = pData;
    }
  }
  releasePage(pToRelease);
  return SQLITE_OK;
}

static void dropCell(MemPage *pPage, int idx, int sz, int *pRC){
  int i;          
  u32 pc;         
  u8 *data;       
  u8 *ptr;        
  int rc;         
  int hdr;        

  if( *pRC ) return;

  assert( idx>=0 && idx<pPage->nCell );
  assert( sz==cellSize(pPage, idx) );
  assert( sqlite3PagerIswriteable(pPage->pDbPage) );
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  data = pPage->aData;
  ptr = &data[pPage->cellOffset + 2*idx];
  pc = get2byte(ptr);
  hdr = pPage->hdrOffset;
  testcase( pc==get2byte(&data[hdr+5]) );
  testcase( pc+sz==pPage->pBt->usableSize );
  if( pc < (u32)get2byte(&data[hdr+5]) || pc+sz > pPage->pBt->usableSize ){
    *pRC = SQLITE_CORRUPT_BKPT;
    return;
  }
  rc = freeSpace(pPage, pc, sz);
  if( rc ){
    *pRC = rc;
    return;
  }
  for(i=idx+1; i<pPage->nCell; i++, ptr+=2){
    ptr[0] = ptr[2];
    ptr[1] = ptr[3];
  }
  pPage->nCell--;
  put2byte(&data[hdr+3], pPage->nCell);
  pPage->nFree += 2;
}

static void insertCell(
  MemPage *pPage,   
  int i,            
  u8 *pCell,        
  int sz,           
  u8 *pTemp,        
  Pgno iChild,      
  int *pRC          
){
  int idx = 0;      
  int j;            
  int end;          
  int ins;          
  int cellOffset;   
  u8 *data;         
  u8 *ptr;          

  int nSkip = (iChild ? 4 : 0);

  if( *pRC ) return;

  assert( i>=0 && i<=pPage->nCell+pPage->nOverflow );
  assert( pPage->nCell<=MX_CELL(pPage->pBt) && MX_CELL(pPage->pBt)<=10921 );
  assert( pPage->nOverflow<=ArraySize(pPage->aOvfl) );
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  assert( sz==cellSizePtr(pPage, pCell) || (sz==8 && iChild>0) );
  if( pPage->nOverflow || sz+2>pPage->nFree ){
    if( pTemp ){
      memcpy(pTemp+nSkip, pCell+nSkip, sz-nSkip);
      pCell = pTemp;
    }
    if( iChild ){
      put4byte(pCell, iChild);
    }
    j = pPage->nOverflow++;
    assert( j<(int)(sizeof(pPage->aOvfl)/sizeof(pPage->aOvfl[0])) );
    pPage->aOvfl[j].pCell = pCell;
    pPage->aOvfl[j].idx = (u16)i;
  }else{
    int rc = sqlite3PagerWrite(pPage->pDbPage);
    if( rc!=SQLITE_OK ){
      *pRC = rc;
      return;
    }
    assert( sqlite3PagerIswriteable(pPage->pDbPage) );
    data = pPage->aData;
    cellOffset = pPage->cellOffset;
    end = cellOffset + 2*pPage->nCell;
    ins = cellOffset + 2*i;
    rc = allocateSpace(pPage, sz, &idx);
    if( rc ){ *pRC = rc; return; }
    assert( idx >= end+2 );
    assert( idx+sz <= (int)pPage->pBt->usableSize );
    pPage->nCell++;
    pPage->nFree -= (u16)(2 + sz);
    memcpy(&data[idx+nSkip], pCell+nSkip, sz-nSkip);
    if( iChild ){
      put4byte(&data[idx], iChild);
    }
    for(j=end, ptr=&data[j]; j>ins; j-=2, ptr-=2){
      ptr[0] = ptr[-2];
      ptr[1] = ptr[-1];
    }
    put2byte(&data[ins], idx);
    put2byte(&data[pPage->hdrOffset+3], pPage->nCell);
#ifndef SQLITE_OMIT_AUTOVACUUM
    if( pPage->pBt->autoVacuum ){
      ptrmapPutOvflPtr(pPage, pCell, pRC);
    }
#endif
  }
}

static void assemblePage(
  MemPage *pPage,   
  int nCell,        
  u8 **apCell,      
  u16 *aSize        
){
  int i;            
  u8 *pCellptr;     
  int cellbody;     
  u8 * const data = pPage->aData;             
  const int hdr = pPage->hdrOffset;           
  const int nUsable = pPage->pBt->usableSize; 

  assert( pPage->nOverflow==0 );
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  assert( nCell>=0 && nCell<=(int)MX_CELL(pPage->pBt)
            && (int)MX_CELL(pPage->pBt)<=10921);
  assert( sqlite3PagerIswriteable(pPage->pDbPage) );

  
  assert( pPage->nCell==0 );
  assert( get2byteNotZero(&data[hdr+5])==nUsable );

  pCellptr = &data[pPage->cellOffset + nCell*2];
  cellbody = nUsable;
  for(i=nCell-1; i>=0; i--){
    pCellptr -= 2;
    cellbody -= aSize[i];
    put2byte(pCellptr, cellbody);
    memcpy(&data[cellbody], apCell[i], aSize[i]);
  }
  put2byte(&data[hdr+3], nCell);
  put2byte(&data[hdr+5], cellbody);
  pPage->nFree -= (nCell*2 + nUsable - cellbody);
  pPage->nCell = (u16)nCell;
}

#define NN 1             
#define NB (NN*2+1)      


#ifndef SQLITE_OMIT_QUICKBALANCE
static int balance_quick(MemPage *pParent, MemPage *pPage, u8 *pSpace){
  BtShared *const pBt = pPage->pBt;    
  MemPage *pNew;                       
  int rc;                              
  Pgno pgnoNew;                        

  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  assert( sqlite3PagerIswriteable(pParent->pDbPage) );
  assert( pPage->nOverflow==1 );

  
  if( pPage->nCell<=0 ) return SQLITE_CORRUPT_BKPT;

  rc = allocateBtreePage(pBt, &pNew, &pgnoNew, 0, 0);

  if( rc==SQLITE_OK ){

    u8 *pOut = &pSpace[4];
    u8 *pCell = pPage->aOvfl[0].pCell;
    u16 szCell = cellSizePtr(pPage, pCell);
    u8 *pStop;

    assert( sqlite3PagerIswriteable(pNew->pDbPage) );
    assert( pPage->aData[0]==(PTF_INTKEY|PTF_LEAFDATA|PTF_LEAF) );
    zeroPage(pNew, PTF_INTKEY|PTF_LEAFDATA|PTF_LEAF);
    assemblePage(pNew, 1, &pCell, &szCell);

    if( ISAUTOVACUUM ){
      ptrmapPut(pBt, pgnoNew, PTRMAP_BTREE, pParent->pgno, &rc);
      if( szCell>pNew->minLocal ){
        ptrmapPutOvflPtr(pNew, pCell, &rc);
      }
    }
  
    pCell = findCell(pPage, pPage->nCell-1);
    pStop = &pCell[9];
    while( (*(pCell++)&0x80) && pCell<pStop );
    pStop = &pCell[9];
    while( ((*(pOut++) = *(pCell++))&0x80) && pCell<pStop );

    
    insertCell(pParent, pParent->nCell, pSpace, (int)(pOut-pSpace),
               0, pPage->pgno, &rc);

    
    put4byte(&pParent->aData[pParent->hdrOffset+8], pgnoNew);
  
    
    releasePage(pNew);
  }

  return rc;
}
#endif 

#if 0
static int ptrmapCheckPages(MemPage **apPage, int nPage){
  int i, j;
  for(i=0; i<nPage; i++){
    Pgno n;
    u8 e;
    MemPage *pPage = apPage[i];
    BtShared *pBt = pPage->pBt;
    assert( pPage->isInit );

    for(j=0; j<pPage->nCell; j++){
      CellInfo info;
      u8 *z;
     
      z = findCell(pPage, j);
      btreeParseCellPtr(pPage, z, &info);
      if( info.iOverflow ){
        Pgno ovfl = get4byte(&z[info.iOverflow]);
        ptrmapGet(pBt, ovfl, &e, &n);
        assert( n==pPage->pgno && e==PTRMAP_OVERFLOW1 );
      }
      if( !pPage->leaf ){
        Pgno child = get4byte(z);
        ptrmapGet(pBt, child, &e, &n);
        assert( n==pPage->pgno && e==PTRMAP_BTREE );
      }
    }
    if( !pPage->leaf ){
      Pgno child = get4byte(&pPage->aData[pPage->hdrOffset+8]);
      ptrmapGet(pBt, child, &e, &n);
      assert( n==pPage->pgno && e==PTRMAP_BTREE );
    }
  }
  return 1;
}
#endif

static void copyNodeContent(MemPage *pFrom, MemPage *pTo, int *pRC){
  if( (*pRC)==SQLITE_OK ){
    BtShared * const pBt = pFrom->pBt;
    u8 * const aFrom = pFrom->aData;
    u8 * const aTo = pTo->aData;
    int const iFromHdr = pFrom->hdrOffset;
    int const iToHdr = ((pTo->pgno==1) ? 100 : 0);
    int rc;
    int iData;
  
  
    assert( pFrom->isInit );
    assert( pFrom->nFree>=iToHdr );
    assert( get2byte(&aFrom[iFromHdr+5]) <= (int)pBt->usableSize );
  
    
    iData = get2byte(&aFrom[iFromHdr+5]);
    memcpy(&aTo[iData], &aFrom[iData], pBt->usableSize-iData);
    memcpy(&aTo[iToHdr], &aFrom[iFromHdr], pFrom->cellOffset + 2*pFrom->nCell);
  
    pTo->isInit = 0;
    rc = btreeInitPage(pTo);
    if( rc!=SQLITE_OK ){
      *pRC = rc;
      return;
    }
  
    if( ISAUTOVACUUM ){
      *pRC = setChildPtrmaps(pTo);
    }
  }
}

static int balance_nonroot(
  MemPage *pParent,               
  int iParentIdx,                 
  u8 *aOvflSpace,                 
  int isRoot                      
){
  BtShared *pBt;               
  int nCell = 0;               
  int nMaxCells = 0;           
  int nNew = 0;                
  int nOld;                    
  int i, j, k;                 
  int nxDiv;                   
  int rc = SQLITE_OK;          
  u16 leafCorrection;          
  int leafData;                
  int usableSpace;             
  int pageFlags;               
  int subtotal;                
  int iSpace1 = 0;             
  int iOvflSpace = 0;          
  int szScratch;               
  MemPage *apOld[NB];          
  MemPage *apCopy[NB];         
  MemPage *apNew[NB+2];        
  u8 *pRight;                  
  u8 *apDiv[NB-1];             
  int cntNew[NB+2];            
  int szNew[NB+2];             
  u8 **apCell = 0;             
  u16 *szCell;                 
  u8 *aSpace1;                 
  Pgno pgno;                   

  pBt = pParent->pBt;
  assert( sqlite3_mutex_held(pBt->mutex) );
  assert( sqlite3PagerIswriteable(pParent->pDbPage) );

#if 0
  TRACE(("BALANCE: begin page %d child of %d\n", pPage->pgno, pParent->pgno));
#endif

  assert( pParent->nOverflow==0 || pParent->nOverflow==1 );
  assert( pParent->nOverflow==0 || pParent->aOvfl[0].idx==iParentIdx );

  if( !aOvflSpace ){
    return SQLITE_NOMEM;
  }

  i = pParent->nOverflow + pParent->nCell;
  if( i<2 ){
    nxDiv = 0;
    nOld = i+1;
  }else{
    nOld = 3;
    if( iParentIdx==0 ){                 
      nxDiv = 0;
    }else if( iParentIdx==i ){
      nxDiv = i-2;
    }else{
      nxDiv = iParentIdx-1;
    }
    i = 2;
  }
  if( (i+nxDiv-pParent->nOverflow)==pParent->nCell ){
    pRight = &pParent->aData[pParent->hdrOffset+8];
  }else{
    pRight = findCell(pParent, i+nxDiv-pParent->nOverflow);
  }
  pgno = get4byte(pRight);
  while( 1 ){
    rc = getAndInitPage(pBt, pgno, &apOld[i]);
    if( rc ){
      memset(apOld, 0, (i+1)*sizeof(MemPage*));
      goto balance_cleanup;
    }
    nMaxCells += 1+apOld[i]->nCell+apOld[i]->nOverflow;
    if( (i--)==0 ) break;

    if( i+nxDiv==pParent->aOvfl[0].idx && pParent->nOverflow ){
      apDiv[i] = pParent->aOvfl[0].pCell;
      pgno = get4byte(apDiv[i]);
      szNew[i] = cellSizePtr(pParent, apDiv[i]);
      pParent->nOverflow = 0;
    }else{
      apDiv[i] = findCell(pParent, i+nxDiv-pParent->nOverflow);
      pgno = get4byte(apDiv[i]);
      szNew[i] = cellSizePtr(pParent, apDiv[i]);

      if( pBt->secureDelete ){
        int iOff = SQLITE_PTR_TO_INT(apDiv[i]) - SQLITE_PTR_TO_INT(pParent->aData);
        if( (iOff+szNew[i])>(int)pBt->usableSize ){
          rc = SQLITE_CORRUPT_BKPT;
          memset(apOld, 0, (i+1)*sizeof(MemPage*));
          goto balance_cleanup;
        }else{
          memcpy(&aOvflSpace[iOff], apDiv[i], szNew[i]);
          apDiv[i] = &aOvflSpace[apDiv[i]-pParent->aData];
        }
      }
      dropCell(pParent, i+nxDiv-pParent->nOverflow, szNew[i], &rc);
    }
  }

  nMaxCells = (nMaxCells + 3)&~3;

  k = pBt->pageSize + ROUND8(sizeof(MemPage));
  szScratch =
       nMaxCells*sizeof(u8*)                       
     + nMaxCells*sizeof(u16)                       
     + pBt->pageSize                               
     + k*nOld;                                     
  apCell = sqlite3ScratchMalloc( szScratch ); 
  if( apCell==0 ){
    rc = SQLITE_NOMEM;
    goto balance_cleanup;
  }
  szCell = (u16*)&apCell[nMaxCells];
  aSpace1 = (u8*)&szCell[nMaxCells];
  assert( EIGHT_BYTE_ALIGNMENT(aSpace1) );

  leafCorrection = apOld[0]->leaf*4;
  leafData = apOld[0]->hasData;
  for(i=0; i<nOld; i++){
    int limit;
    
    /* Before doing anything else, take a copy of the i'th original sibling
    ** The rest of this function will use data from the copies rather
    ** that the original pages since the original pages will be in the
    ** process of being overwritten.  */
    MemPage *pOld = apCopy[i] = (MemPage*)&aSpace1[pBt->pageSize + k*i];
    memcpy(pOld, apOld[i], sizeof(MemPage));
    pOld->aData = (void*)&pOld[1];
    memcpy(pOld->aData, apOld[i]->aData, pBt->pageSize);

    limit = pOld->nCell+pOld->nOverflow;
    for(j=0; j<limit; j++){
      assert( nCell<nMaxCells );
      apCell[nCell] = findOverflowCell(pOld, j);
      szCell[nCell] = cellSizePtr(pOld, apCell[nCell]);
      nCell++;
    }
    if( i<nOld-1 && !leafData){
      u16 sz = (u16)szNew[i];
      u8 *pTemp;
      assert( nCell<nMaxCells );
      szCell[nCell] = sz;
      pTemp = &aSpace1[iSpace1];
      iSpace1 += sz;
      assert( sz<=pBt->maxLocal+23 );
      assert( iSpace1 <= (int)pBt->pageSize );
      memcpy(pTemp, apDiv[i], sz);
      apCell[nCell] = pTemp+leafCorrection;
      assert( leafCorrection==0 || leafCorrection==4 );
      szCell[nCell] = szCell[nCell] - leafCorrection;
      if( !pOld->leaf ){
        assert( leafCorrection==0 );
        assert( pOld->hdrOffset==0 );
        memcpy(apCell[nCell], &pOld->aData[8], 4);
      }else{
        assert( leafCorrection==4 );
        if( szCell[nCell]<4 ){
          
          szCell[nCell] = 4;
        }
      }
      nCell++;
    }
  }

  usableSpace = pBt->usableSize - 12 + leafCorrection;
  for(subtotal=k=i=0; i<nCell; i++){
    assert( i<nMaxCells );
    subtotal += szCell[i] + 2;
    if( subtotal > usableSpace ){
      szNew[k] = subtotal - szCell[i];
      cntNew[k] = i;
      if( leafData ){ i--; }
      subtotal = 0;
      k++;
      if( k>NB+1 ){ rc = SQLITE_CORRUPT_BKPT; goto balance_cleanup; }
    }
  }
  szNew[k] = subtotal;
  cntNew[k] = nCell;
  k++;

  for(i=k-1; i>0; i--){
    int szRight = szNew[i];  
    int szLeft = szNew[i-1]; 
    int r;              
    int d;              

    r = cntNew[i-1] - 1;
    d = r + 1 - leafData;
    assert( d<nMaxCells );
    assert( r<nMaxCells );
    while( szRight==0 || szRight+szCell[d]+2<=szLeft-(szCell[r]+2) ){
      szRight += szCell[d] + 2;
      szLeft -= szCell[r] + 2;
      cntNew[i-1]--;
      r = cntNew[i-1] - 1;
      d = r + 1 - leafData;
    }
    szNew[i] = szRight;
    szNew[i-1] = szLeft;
  }

  assert( cntNew[0]>0 || (pParent->pgno==1 && pParent->nCell==0) );

  TRACE(("BALANCE: old: %d %d %d  ",
    apOld[0]->pgno, 
    nOld>=2 ? apOld[1]->pgno : 0,
    nOld>=3 ? apOld[2]->pgno : 0
  ));

  if( apOld[0]->pgno<=1 ){
    rc = SQLITE_CORRUPT_BKPT;
    goto balance_cleanup;
  }
  pageFlags = apOld[0]->aData[0];
  for(i=0; i<k; i++){
    MemPage *pNew;
    if( i<nOld ){
      pNew = apNew[i] = apOld[i];
      apOld[i] = 0;
      rc = sqlite3PagerWrite(pNew->pDbPage);
      nNew++;
      if( rc ) goto balance_cleanup;
    }else{
      assert( i>0 );
      rc = allocateBtreePage(pBt, &pNew, &pgno, pgno, 0);
      if( rc ) goto balance_cleanup;
      apNew[i] = pNew;
      nNew++;

      
      if( ISAUTOVACUUM ){
        ptrmapPut(pBt, pNew->pgno, PTRMAP_BTREE, pParent->pgno, &rc);
        if( rc!=SQLITE_OK ){
          goto balance_cleanup;
        }
      }
    }
  }

  while( i<nOld ){
    freePage(apOld[i], &rc);
    if( rc ) goto balance_cleanup;
    releasePage(apOld[i]);
    apOld[i] = 0;
    i++;
  }

  for(i=0; i<k-1; i++){
    int minV = apNew[i]->pgno;
    int minI = i;
    for(j=i+1; j<k; j++){
      if( apNew[j]->pgno<(unsigned)minV ){
        minI = j;
        minV = apNew[j]->pgno;
      }
    }
    if( minI>i ){
      MemPage *pT;
      pT = apNew[i];
      apNew[i] = apNew[minI];
      apNew[minI] = pT;
    }
  }
  TRACE(("new: %d(%d) %d(%d) %d(%d) %d(%d) %d(%d)\n",
    apNew[0]->pgno, szNew[0],
    nNew>=2 ? apNew[1]->pgno : 0, nNew>=2 ? szNew[1] : 0,
    nNew>=3 ? apNew[2]->pgno : 0, nNew>=3 ? szNew[2] : 0,
    nNew>=4 ? apNew[3]->pgno : 0, nNew>=4 ? szNew[3] : 0,
    nNew>=5 ? apNew[4]->pgno : 0, nNew>=5 ? szNew[4] : 0));

  assert( sqlite3PagerIswriteable(pParent->pDbPage) );
  put4byte(pRight, apNew[nNew-1]->pgno);

  j = 0;
  for(i=0; i<nNew; i++){
    
    MemPage *pNew = apNew[i];
    assert( j<nMaxCells );
    zeroPage(pNew, pageFlags);
    assemblePage(pNew, cntNew[i]-j, &apCell[j], &szCell[j]);
    assert( pNew->nCell>0 || (nNew==1 && cntNew[0]==0) );
    assert( pNew->nOverflow==0 );

    j = cntNew[i];

    assert( i<nNew-1 || j==nCell );
    if( j<nCell ){
      u8 *pCell;
      u8 *pTemp;
      int sz;

      assert( j<nMaxCells );
      pCell = apCell[j];
      sz = szCell[j] + leafCorrection;
      pTemp = &aOvflSpace[iOvflSpace];
      if( !pNew->leaf ){
        memcpy(&pNew->aData[8], pCell, 4);
      }else if( leafData ){
        CellInfo info;
        j--;
        btreeParseCellPtr(pNew, apCell[j], &info);
        pCell = pTemp;
        sz = 4 + putVarint(&pCell[4], info.nKey);
        pTemp = 0;
      }else{
        pCell -= 4;
        if( szCell[j]==4 ){
          assert(leafCorrection==4);
          sz = cellSizePtr(pParent, pCell);
        }
      }
      iOvflSpace += sz;
      assert( sz<=pBt->maxLocal+23 );
      assert( iOvflSpace <= (int)pBt->pageSize );
      insertCell(pParent, nxDiv, pCell, sz, pTemp, pNew->pgno, &rc);
      if( rc!=SQLITE_OK ) goto balance_cleanup;
      assert( sqlite3PagerIswriteable(pParent->pDbPage) );

      j++;
      nxDiv++;
    }
  }
  assert( j==nCell );
  assert( nOld>0 );
  assert( nNew>0 );
  if( (pageFlags & PTF_LEAF)==0 ){
    u8 *zChild = &apCopy[nOld-1]->aData[8];
    memcpy(&apNew[nNew-1]->aData[8], zChild, 4);
  }

  if( isRoot && pParent->nCell==0 && pParent->hdrOffset<=apNew[0]->nFree ){
    assert( nNew==1 );
    assert( apNew[0]->nFree == 
        (get2byte(&apNew[0]->aData[5])-apNew[0]->cellOffset-apNew[0]->nCell*2) 
    );
    copyNodeContent(apNew[0], pParent, &rc);
    freePage(apNew[0], &rc);
  }else if( ISAUTOVACUUM ){
    MemPage *pNew = apNew[0];
    MemPage *pOld = apCopy[0];
    int nOverflow = pOld->nOverflow;
    int iNextOld = pOld->nCell + nOverflow;
    int iOverflow = (nOverflow ? pOld->aOvfl[0].idx : -1);
    j = 0;                             
    k = 0;                             
    for(i=0; i<nCell; i++){
      int isDivider = 0;
      while( i==iNextOld ){
        pOld = apCopy[++j];
        iNextOld = i + !leafData + pOld->nCell + pOld->nOverflow;
        if( pOld->nOverflow ){
          nOverflow = pOld->nOverflow;
          iOverflow = i + !leafData + pOld->aOvfl[0].idx;
        }
        isDivider = !leafData;  
      }

      assert(nOverflow>0 || iOverflow<i );
      assert(nOverflow<2 || pOld->aOvfl[0].idx==pOld->aOvfl[1].idx-1);
      assert(nOverflow<3 || pOld->aOvfl[1].idx==pOld->aOvfl[2].idx-1);
      if( i==iOverflow ){
        isDivider = 1;
        if( (--nOverflow)>0 ){
          iOverflow++;
        }
      }

      if( i==cntNew[k] ){
        pNew = apNew[++k];
        if( !leafData ) continue;
      }
      assert( j<nOld );
      assert( k<nNew );

      if( isDivider || pOld->pgno!=pNew->pgno ){
        if( !leafCorrection ){
          ptrmapPut(pBt, get4byte(apCell[i]), PTRMAP_BTREE, pNew->pgno, &rc);
        }
        if( szCell[i]>pNew->minLocal ){
          ptrmapPutOvflPtr(pNew, apCell[i], &rc);
        }
      }
    }

    if( !leafCorrection ){
      for(i=0; i<nNew; i++){
        u32 key = get4byte(&apNew[i]->aData[8]);
        ptrmapPut(pBt, key, PTRMAP_BTREE, apNew[i]->pgno, &rc);
      }
    }

#if 0
    ptrmapCheckPages(apNew, nNew);
    ptrmapCheckPages(&pParent, 1);
#endif
  }

  assert( pParent->isInit );
  TRACE(("BALANCE: finished: old=%d new=%d cells=%d\n",
          nOld, nNew, nCell));

balance_cleanup:
  sqlite3ScratchFree(apCell);
  for(i=0; i<nOld; i++){
    releasePage(apOld[i]);
  }
  for(i=0; i<nNew; i++){
    releasePage(apNew[i]);
  }

  return rc;
}


/*
** This function is called when the root page of a b-tree structure is
** overfull (has one or more overflow pages).
**
** A new child page is allocated and the contents of the current root
** page, including overflow cells, are copied into the child. The root
** page is then overwritten to make it an empty page with the right-child 
** pointer pointing to the new page.
**
** Before returning, all pointer-map entries corresponding to pages 
** that the new child-page now contains pointers to are updated. The
** entry corresponding to the new right-child pointer of the root
** page is also updated.
**
** If successful, *ppChild is set to contain a reference to the child 
** page and SQLITE_OK is returned. In this case the caller is required
** to call releasePage() on *ppChild exactly once. If an error occurs,
** an error code is returned and *ppChild is set to 0.
*/
static int balance_deeper(MemPage *pRoot, MemPage **ppChild){
  int rc;                        
  MemPage *pChild = 0;           
  Pgno pgnoChild = 0;            
  BtShared *pBt = pRoot->pBt;    

  assert( pRoot->nOverflow>0 );
  assert( sqlite3_mutex_held(pBt->mutex) );

  rc = sqlite3PagerWrite(pRoot->pDbPage);
  if( rc==SQLITE_OK ){
    rc = allocateBtreePage(pBt,&pChild,&pgnoChild,pRoot->pgno,0);
    copyNodeContent(pRoot, pChild, &rc);
    if( ISAUTOVACUUM ){
      ptrmapPut(pBt, pgnoChild, PTRMAP_BTREE, pRoot->pgno, &rc);
    }
  }
  if( rc ){
    *ppChild = 0;
    releasePage(pChild);
    return rc;
  }
  assert( sqlite3PagerIswriteable(pChild->pDbPage) );
  assert( sqlite3PagerIswriteable(pRoot->pDbPage) );
  assert( pChild->nCell==pRoot->nCell );

  TRACE(("BALANCE: copy root %d into %d\n", pRoot->pgno, pChild->pgno));

  
  memcpy(pChild->aOvfl, pRoot->aOvfl, pRoot->nOverflow*sizeof(pRoot->aOvfl[0]));
  pChild->nOverflow = pRoot->nOverflow;

  
  zeroPage(pRoot, pChild->aData[0] & ~PTF_LEAF);
  put4byte(&pRoot->aData[pRoot->hdrOffset+8], pgnoChild);

  *ppChild = pChild;
  return SQLITE_OK;
}

static int balance(BtCursor *pCur){
  int rc = SQLITE_OK;
  const int nMin = pCur->pBt->usableSize * 2 / 3;
  u8 aBalanceQuickSpace[13];
  u8 *pFree = 0;

  TESTONLY( int balance_quick_called = 0 );
  TESTONLY( int balance_deeper_called = 0 );

  do {
    int iPage = pCur->iPage;
    MemPage *pPage = pCur->apPage[iPage];

    if( iPage==0 ){
      if( pPage->nOverflow ){
 
        assert( (balance_deeper_called++)==0 );
        rc = balance_deeper(pPage, &pCur->apPage[1]);
        if( rc==SQLITE_OK ){
          pCur->iPage = 1;
          pCur->aiIdx[0] = 0;
          pCur->aiIdx[1] = 0;
          assert( pCur->apPage[1]->nOverflow );
        }
      }else{
        break;
      }
    }else if( pPage->nOverflow==0 && pPage->nFree<=nMin ){
      break;
    }else{
      MemPage * const pParent = pCur->apPage[iPage-1];
      int const iIdx = pCur->aiIdx[iPage-1];

      rc = sqlite3PagerWrite(pParent->pDbPage);
      if( rc==SQLITE_OK ){
#ifndef SQLITE_OMIT_QUICKBALANCE
        if( pPage->hasData
         && pPage->nOverflow==1
         && pPage->aOvfl[0].idx==pPage->nCell
         && pParent->pgno!=1
         && pParent->nCell==iIdx
        ){
          assert( (balance_quick_called++)==0 );
          rc = balance_quick(pParent, pPage, aBalanceQuickSpace);
        }else
#endif
        {
          u8 *pSpace = sqlite3PageMalloc(pCur->pBt->pageSize);
          rc = balance_nonroot(pParent, iIdx, pSpace, iPage==1);
          if( pFree ){
            sqlite3PageFree(pFree);
          }

          pFree = pSpace;
        }
      }

      pPage->nOverflow = 0;

      
      releasePage(pPage);
      pCur->iPage--;
    }
  }while( rc==SQLITE_OK );

  if( pFree ){
    sqlite3PageFree(pFree);
  }
  return rc;
}


/*
** Insert a new record into the BTree.  The key is given by (pKey,nKey)
** and the data is given by (pData,nData).  The cursor is used only to
** define what table the record should be inserted into.  The cursor
** is left pointing at a random location.
**
** For an INTKEY table, only the nKey value of the key is used.  pKey is
** ignored.  For a ZERODATA table, the pData and nData are both ignored.
**
** If the seekResult parameter is non-zero, then a successful call to
** MovetoUnpacked() to seek cursor pCur to (pKey, nKey) has already
** been performed. seekResult is the search result returned (a negative
** number if pCur points at an entry that is smaller than (pKey, nKey), or
** a positive value if pCur points at an etry that is larger than 
** (pKey, nKey)). 
**
** If the seekResult parameter is non-zero, then the caller guarantees that
** cursor pCur is pointing at the existing copy of a row that is to be
** overwritten.  If the seekResult parameter is 0, then cursor pCur may
** point to any entry or to no entry at all and so this function has to seek
** the cursor before the new key can be inserted.
*/
int sqlite3BtreeInsert(
  BtCursor *pCur,                
  const void *pKey, i64 nKey,    
  const void *pData, int nData,  
  int nZero,                     
  int appendBias,                
  int seekResult                 
){
  int rc;
  int loc = seekResult;          
  int szNew = 0;
  int idx;
  MemPage *pPage;
  Btree *p = pCur->pBtree;
  BtShared *pBt = p->pBt;
  unsigned char *oldCell;
  unsigned char *newCell = 0;

  if( pCur->eState==CURSOR_FAULT ){
    assert( pCur->skipNext!=SQLITE_OK );
    return pCur->skipNext;
  }

  assert( cursorHoldsMutex(pCur) );
  assert( pCur->wrFlag && pBt->inTransaction==TRANS_WRITE && !pBt->readOnly );
  assert( hasSharedCacheTableLock(p, pCur->pgnoRoot, pCur->pKeyInfo!=0, 2) );

  assert( (pKey==0)==(pCur->pKeyInfo==0) );

  if( pCur->pKeyInfo==0 ){
    invalidateIncrblobCursors(p, nKey, 0);
  }

  rc = saveAllCursors(pBt, pCur->pgnoRoot, pCur);
  if( rc ) return rc;
  if( !loc ){
    rc = btreeMoveto(pCur, pKey, nKey, appendBias, &loc);
    if( rc ) return rc;
  }
  assert( pCur->eState==CURSOR_VALID || (pCur->eState==CURSOR_INVALID && loc) );

  pPage = pCur->apPage[pCur->iPage];
  assert( pPage->intKey || nKey>=0 );
  assert( pPage->leaf || !pPage->intKey );

  TRACE(("INSERT: table=%d nkey=%lld ndata=%d page=%d %s\n",
          pCur->pgnoRoot, nKey, nData, pPage->pgno,
          loc==0 ? "overwrite" : "new entry"));
  assert( pPage->isInit );
  allocateTempSpace(pBt);
  newCell = pBt->pTmpSpace;
  if( newCell==0 ) return SQLITE_NOMEM;
  rc = fillInCell(pPage, newCell, pKey, nKey, pData, nData, nZero, &szNew);
  if( rc ) goto end_insert;
  assert( szNew==cellSizePtr(pPage, newCell) );
  assert( szNew <= MX_CELL_SIZE(pBt) );
  idx = pCur->aiIdx[pCur->iPage];
  if( loc==0 ){
    u16 szOld;
    assert( idx<pPage->nCell );
    rc = sqlite3PagerWrite(pPage->pDbPage);
    if( rc ){
      goto end_insert;
    }
    oldCell = findCell(pPage, idx);
    if( !pPage->leaf ){
      memcpy(newCell, oldCell, 4);
    }
    szOld = cellSizePtr(pPage, oldCell);
    rc = clearCell(pPage, oldCell);
    dropCell(pPage, idx, szOld, &rc);
    if( rc ) goto end_insert;
  }else if( loc<0 && pPage->nCell>0 ){
    assert( pPage->leaf );
    idx = ++pCur->aiIdx[pCur->iPage];
  }else{
    assert( pPage->leaf );
  }
  insertCell(pPage, idx, newCell, szNew, 0, 0, &rc);
  assert( rc!=SQLITE_OK || pPage->nCell>0 || pPage->nOverflow>0 );

  pCur->info.nSize = 0;
  pCur->validNKey = 0;
  if( rc==SQLITE_OK && pPage->nOverflow ){
    rc = balance(pCur);

    pCur->apPage[pCur->iPage]->nOverflow = 0;
    pCur->eState = CURSOR_INVALID;
  }
  assert( pCur->apPage[pCur->iPage]->nOverflow==0 );

end_insert:
  return rc;
}

int sqlite3BtreeDelete(BtCursor *pCur){
  Btree *p = pCur->pBtree;
  BtShared *pBt = p->pBt;              
  int rc;                              
  MemPage *pPage;                      
  unsigned char *pCell;                
  int iCellIdx;                        
  int iCellDepth;                       

  assert( cursorHoldsMutex(pCur) );
  assert( pBt->inTransaction==TRANS_WRITE );
  assert( !pBt->readOnly );
  assert( pCur->wrFlag );
  assert( hasSharedCacheTableLock(p, pCur->pgnoRoot, pCur->pKeyInfo!=0, 2) );
  assert( !hasReadConflicts(p, pCur->pgnoRoot) );

  if( NEVER(pCur->aiIdx[pCur->iPage]>=pCur->apPage[pCur->iPage]->nCell) 
   || NEVER(pCur->eState!=CURSOR_VALID)
  ){
    return SQLITE_ERROR;  
  }

  if( pCur->pKeyInfo==0 ){
    invalidateIncrblobCursors(p, pCur->info.nKey, 0);
  }

  iCellDepth = pCur->iPage;
  iCellIdx = pCur->aiIdx[iCellDepth];
  pPage = pCur->apPage[iCellDepth];
  pCell = findCell(pPage, iCellIdx);

  if( !pPage->leaf ){
    int notUsed;
    rc = sqlite3BtreePrevious(pCur, &notUsed);
    if( rc ) return rc;
  }

  rc = saveAllCursors(pBt, pCur->pgnoRoot, pCur);
  if( rc ) return rc;
  rc = sqlite3PagerWrite(pPage->pDbPage);
  if( rc ) return rc;
  rc = clearCell(pPage, pCell);
  dropCell(pPage, iCellIdx, cellSizePtr(pPage, pCell), &rc);
  if( rc ) return rc;

  if( !pPage->leaf ){
    MemPage *pLeaf = pCur->apPage[pCur->iPage];
    int nCell;
    Pgno n = pCur->apPage[iCellDepth+1]->pgno;
    unsigned char *pTmp;

    pCell = findCell(pLeaf, pLeaf->nCell-1);
    nCell = cellSizePtr(pLeaf, pCell);
    assert( MX_CELL_SIZE(pBt) >= nCell );

    allocateTempSpace(pBt);
    pTmp = pBt->pTmpSpace;

    rc = sqlite3PagerWrite(pLeaf->pDbPage);
    insertCell(pPage, iCellIdx, pCell-4, nCell+4, pTmp, n, &rc);
    dropCell(pLeaf, pLeaf->nCell-1, nCell, &rc);
    if( rc ) return rc;
  }

  rc = balance(pCur);
  if( rc==SQLITE_OK && pCur->iPage>iCellDepth ){
    while( pCur->iPage>iCellDepth ){
      releasePage(pCur->apPage[pCur->iPage--]);
    }
    rc = balance(pCur);
  }

  if( rc==SQLITE_OK ){
    moveToRoot(pCur);
  }
  return rc;
}

static int btreeCreateTable(Btree *p, int *piTable, int createTabFlags){
  BtShared *pBt = p->pBt;
  MemPage *pRoot;
  Pgno pgnoRoot;
  int rc;
  int ptfFlags;          

  assert( sqlite3BtreeHoldsMutex(p) );
  assert( pBt->inTransaction==TRANS_WRITE );
  assert( !pBt->readOnly );

#ifdef SQLITE_OMIT_AUTOVACUUM
  rc = allocateBtreePage(pBt, &pRoot, &pgnoRoot, 1, 0);
  if( rc ){
    return rc;
  }
#else
  if( pBt->autoVacuum ){
    Pgno pgnoMove;      
    MemPage *pPageMove; 

    invalidateAllOverflowCache(pBt);

    sqlite3BtreeGetMeta(p, BTREE_LARGEST_ROOT_PAGE, &pgnoRoot);
    pgnoRoot++;

    while( pgnoRoot==PTRMAP_PAGENO(pBt, pgnoRoot) ||
        pgnoRoot==PENDING_BYTE_PAGE(pBt) ){
      pgnoRoot++;
    }
    assert( pgnoRoot>=3 );

    rc = allocateBtreePage(pBt, &pPageMove, &pgnoMove, pgnoRoot, 1);
    if( rc!=SQLITE_OK ){
      return rc;
    }

    if( pgnoMove!=pgnoRoot ){
      u8 eType = 0;
      Pgno iPtrPage = 0;

      releasePage(pPageMove);

      
      rc = btreeGetPage(pBt, pgnoRoot, &pRoot, 0);
      if( rc!=SQLITE_OK ){
        return rc;
      }
      rc = ptrmapGet(pBt, pgnoRoot, &eType, &iPtrPage);
      if( eType==PTRMAP_ROOTPAGE || eType==PTRMAP_FREEPAGE ){
        rc = SQLITE_CORRUPT_BKPT;
      }
      if( rc!=SQLITE_OK ){
        releasePage(pRoot);
        return rc;
      }
      assert( eType!=PTRMAP_ROOTPAGE );
      assert( eType!=PTRMAP_FREEPAGE );
      rc = relocatePage(pBt, pRoot, eType, iPtrPage, pgnoMove, 0);
      releasePage(pRoot);

      
      if( rc!=SQLITE_OK ){
        return rc;
      }
      rc = btreeGetPage(pBt, pgnoRoot, &pRoot, 0);
      if( rc!=SQLITE_OK ){
        return rc;
      }
      rc = sqlite3PagerWrite(pRoot->pDbPage);
      if( rc!=SQLITE_OK ){
        releasePage(pRoot);
        return rc;
      }
    }else{
      pRoot = pPageMove;
    } 

    
    ptrmapPut(pBt, pgnoRoot, PTRMAP_ROOTPAGE, 0, &rc);
    if( rc ){
      releasePage(pRoot);
      return rc;
    }

    assert( sqlite3PagerIswriteable(pBt->pPage1->pDbPage) );
    rc = sqlite3BtreeUpdateMeta(p, 4, pgnoRoot);
    if( NEVER(rc) ){
      releasePage(pRoot);
      return rc;
    }

  }else{
    rc = allocateBtreePage(pBt, &pRoot, &pgnoRoot, 1, 0);
    if( rc ) return rc;
  }
#endif
  assert( sqlite3PagerIswriteable(pRoot->pDbPage) );
  if( createTabFlags & BTREE_INTKEY ){
    ptfFlags = PTF_INTKEY | PTF_LEAFDATA | PTF_LEAF;
  }else{
    ptfFlags = PTF_ZERODATA | PTF_LEAF;
  }
  zeroPage(pRoot, ptfFlags);
  sqlite3PagerUnref(pRoot->pDbPage);
  assert( (pBt->openFlags & BTREE_SINGLE)==0 || pgnoRoot==2 );
  *piTable = (int)pgnoRoot;
  return SQLITE_OK;
}
int sqlite3BtreeCreateTable(Btree *p, int *piTable, int flags){
  int rc;
  sqlite3BtreeEnter(p);
  rc = btreeCreateTable(p, piTable, flags);
  sqlite3BtreeLeave(p);
  return rc;
}

static int clearDatabasePage(
  BtShared *pBt,           
  Pgno pgno,               
  int freePageFlag,        
  int *pnChange            
){
  MemPage *pPage;
  int rc;
  unsigned char *pCell;
  int i;

  assert( sqlite3_mutex_held(pBt->mutex) );
  if( pgno>btreePagecount(pBt) ){
    return SQLITE_CORRUPT_BKPT;
  }

  rc = getAndInitPage(pBt, pgno, &pPage);
  if( rc ) return rc;
  for(i=0; i<pPage->nCell; i++){
    pCell = findCell(pPage, i);
    if( !pPage->leaf ){
      rc = clearDatabasePage(pBt, get4byte(pCell), 1, pnChange);
      if( rc ) goto cleardatabasepage_out;
    }
    rc = clearCell(pPage, pCell);
    if( rc ) goto cleardatabasepage_out;
  }
  if( !pPage->leaf ){
    rc = clearDatabasePage(pBt, get4byte(&pPage->aData[8]), 1, pnChange);
    if( rc ) goto cleardatabasepage_out;
  }else if( pnChange ){
    assert( pPage->intKey );
    *pnChange += pPage->nCell;
  }
  if( freePageFlag ){
    freePage(pPage, &rc);
  }else if( (rc = sqlite3PagerWrite(pPage->pDbPage))==0 ){
    zeroPage(pPage, pPage->aData[0] | PTF_LEAF);
  }

cleardatabasepage_out:
  releasePage(pPage);
  return rc;
}

int sqlite3BtreeClearTable(Btree *p, int iTable, int *pnChange){
  int rc;
  BtShared *pBt = p->pBt;
  sqlite3BtreeEnter(p);
  assert( p->inTrans==TRANS_WRITE );

  invalidateIncrblobCursors(p, 0, 1);

  rc = saveAllCursors(pBt, (Pgno)iTable, 0);
  if( SQLITE_OK==rc ){
    rc = clearDatabasePage(pBt, (Pgno)iTable, 0, pnChange);
  }
  sqlite3BtreeLeave(p);
  return rc;
}

static int btreeDropTable(Btree *p, Pgno iTable, int *piMoved){
  int rc;
  MemPage *pPage = 0;
  BtShared *pBt = p->pBt;

  assert( sqlite3BtreeHoldsMutex(p) );
  assert( p->inTrans==TRANS_WRITE );

  if( NEVER(pBt->pCursor) ){
    sqlite3ConnectionBlocked(p->db, pBt->pCursor->pBtree->db);
    return SQLITE_LOCKED_SHAREDCACHE;
  }

  rc = btreeGetPage(pBt, (Pgno)iTable, &pPage, 0);
  if( rc ) return rc;
  rc = sqlite3BtreeClearTable(p, iTable, 0);
  if( rc ){
    releasePage(pPage);
    return rc;
  }

  *piMoved = 0;

  if( iTable>1 ){
#ifdef SQLITE_OMIT_AUTOVACUUM
    freePage(pPage, &rc);
    releasePage(pPage);
#else
    if( pBt->autoVacuum ){
      Pgno maxRootPgno;
      sqlite3BtreeGetMeta(p, BTREE_LARGEST_ROOT_PAGE, &maxRootPgno);

      if( iTable==maxRootPgno ){
        freePage(pPage, &rc);
        releasePage(pPage);
        if( rc!=SQLITE_OK ){
          return rc;
        }
      }else{
        MemPage *pMove;
        releasePage(pPage);
        rc = btreeGetPage(pBt, maxRootPgno, &pMove, 0);
        if( rc!=SQLITE_OK ){
          return rc;
        }
        rc = relocatePage(pBt, pMove, PTRMAP_ROOTPAGE, 0, iTable, 0);
        releasePage(pMove);
        if( rc!=SQLITE_OK ){
          return rc;
        }
        pMove = 0;
        rc = btreeGetPage(pBt, maxRootPgno, &pMove, 0);
        freePage(pMove, &rc);
        releasePage(pMove);
        if( rc!=SQLITE_OK ){
          return rc;
        }
        *piMoved = maxRootPgno;
      }

      maxRootPgno--;
      while( maxRootPgno==PENDING_BYTE_PAGE(pBt)
             || PTRMAP_ISPAGE(pBt, maxRootPgno) ){
        maxRootPgno--;
      }
      assert( maxRootPgno!=PENDING_BYTE_PAGE(pBt) );

      rc = sqlite3BtreeUpdateMeta(p, 4, maxRootPgno);
    }else{
      freePage(pPage, &rc);
      releasePage(pPage);
    }
#endif
  }else{
    zeroPage(pPage, PTF_INTKEY|PTF_LEAF );
    releasePage(pPage);
  }
  return rc;  
}
int sqlite3BtreeDropTable(Btree *p, int iTable, int *piMoved){
  int rc;
  sqlite3BtreeEnter(p);
  rc = btreeDropTable(p, iTable, piMoved);
  sqlite3BtreeLeave(p);
  return rc;
}


void sqlite3BtreeGetMeta(Btree *p, int idx, u32 *pMeta){
  BtShared *pBt = p->pBt;

  sqlite3BtreeEnter(p);
  assert( p->inTrans>TRANS_NONE );
  assert( SQLITE_OK==querySharedCacheTableLock(p, MASTER_ROOT, READ_LOCK) );
  assert( pBt->pPage1 );
  assert( idx>=0 && idx<=15 );

  *pMeta = get4byte(&pBt->pPage1->aData[36 + idx*4]);

#ifdef SQLITE_OMIT_AUTOVACUUM
  if( idx==BTREE_LARGEST_ROOT_PAGE && *pMeta>0 ) pBt->readOnly = 1;
#endif

  sqlite3BtreeLeave(p);
}

/*
** Write meta-information back into the database.  Meta[0] is
** read-only and may not be written.
*/
int sqlite3BtreeUpdateMeta(Btree *p, int idx, u32 iMeta){
  BtShared *pBt = p->pBt;
  unsigned char *pP1;
  int rc;
  assert( idx>=1 && idx<=15 );
  sqlite3BtreeEnter(p);
  assert( p->inTrans==TRANS_WRITE );
  assert( pBt->pPage1!=0 );
  pP1 = pBt->pPage1->aData;
  rc = sqlite3PagerWrite(pBt->pPage1->pDbPage);
  if( rc==SQLITE_OK ){
    put4byte(&pP1[36 + idx*4], iMeta);
#ifndef SQLITE_OMIT_AUTOVACUUM
    if( idx==BTREE_INCR_VACUUM ){
      assert( pBt->autoVacuum || iMeta==0 );
      assert( iMeta==0 || iMeta==1 );
      pBt->incrVacuum = (u8)iMeta;
    }
#endif
  }
  sqlite3BtreeLeave(p);
  return rc;
}

#ifndef SQLITE_OMIT_BTREECOUNT
int sqlite3BtreeCount(BtCursor *pCur, i64 *pnEntry){
  i64 nEntry = 0;                      
  int rc;                              
  rc = moveToRoot(pCur);

  while( rc==SQLITE_OK ){
    int iIdx;                          
    MemPage *pPage;                    

    pPage = pCur->apPage[pCur->iPage];
    if( pPage->leaf || !pPage->intKey ){
      nEntry += pPage->nCell;
    }

    if( pPage->leaf ){
      do {
        if( pCur->iPage==0 ){
          
          *pnEntry = nEntry;
          return SQLITE_OK;
        }
        moveToParent(pCur);
      }while ( pCur->aiIdx[pCur->iPage]>=pCur->apPage[pCur->iPage]->nCell );

      pCur->aiIdx[pCur->iPage]++;
      pPage = pCur->apPage[pCur->iPage];
    }

    iIdx = pCur->aiIdx[pCur->iPage];
    if( iIdx==pPage->nCell ){
      rc = moveToChild(pCur, get4byte(&pPage->aData[pPage->hdrOffset+8]));
    }else{
      rc = moveToChild(pCur, get4byte(findCell(pPage, iIdx)));
    }
  }

  
  return rc;
}
#endif

Pager *sqlite3BtreePager(Btree *p){
  return p->pBt->pPager;
}

#ifndef SQLITE_OMIT_INTEGRITY_CHECK
static void checkAppendMsg(
  IntegrityCk *pCheck,
  char *zMsg1,
  const char *zFormat,
  ...
){
  va_list ap;
  if( !pCheck->mxErr ) return;
  pCheck->mxErr--;
  pCheck->nErr++;
  va_start(ap, zFormat);
  if( pCheck->errMsg.nChar ){
    sqlite3StrAccumAppend(&pCheck->errMsg, "\n", 1);
  }
  if( zMsg1 ){
    sqlite3StrAccumAppend(&pCheck->errMsg, zMsg1, -1);
  }
  sqlite3VXPrintf(&pCheck->errMsg, 1, zFormat, ap);
  va_end(ap);
  if( pCheck->errMsg.mallocFailed ){
    pCheck->mallocFailed = 1;
  }
}
#endif 

#ifndef SQLITE_OMIT_INTEGRITY_CHECK
static int checkRef(IntegrityCk *pCheck, Pgno iPage, char *zContext){
  if( iPage==0 ) return 1;
  if( iPage>pCheck->nPage ){
    checkAppendMsg(pCheck, zContext, "invalid page number %d", iPage);
    return 1;
  }
  if( pCheck->anRef[iPage]==1 ){
    checkAppendMsg(pCheck, zContext, "2nd reference to page %d", iPage);
    return 1;
  }
  return  (pCheck->anRef[iPage]++)>1;
}

#ifndef SQLITE_OMIT_AUTOVACUUM
static void checkPtrmap(
  IntegrityCk *pCheck,   
  Pgno iChild,           
  u8 eType,              
  Pgno iParent,          
  char *zContext         
){
  int rc;
  u8 ePtrmapType;
  Pgno iPtrmapParent;

  rc = ptrmapGet(pCheck->pBt, iChild, &ePtrmapType, &iPtrmapParent);
  if( rc!=SQLITE_OK ){
    if( rc==SQLITE_NOMEM || rc==SQLITE_IOERR_NOMEM ) pCheck->mallocFailed = 1;
    checkAppendMsg(pCheck, zContext, "Failed to read ptrmap key=%d", iChild);
    return;
  }

  if( ePtrmapType!=eType || iPtrmapParent!=iParent ){
    checkAppendMsg(pCheck, zContext, 
      "Bad ptr map entry key=%d expected=(%d,%d) got=(%d,%d)", 
      iChild, eType, iParent, ePtrmapType, iPtrmapParent);
  }
}
#endif

static void checkList(
  IntegrityCk *pCheck,  
  int isFreeList,       
  int iPage,            
  int N,                
  char *zContext        
){
  int i;
  int expected = N;
  int iFirst = iPage;
  while( N-- > 0 && pCheck->mxErr ){
    DbPage *pOvflPage;
    unsigned char *pOvflData;
    if( iPage<1 ){
      checkAppendMsg(pCheck, zContext,
         "%d of %d pages missing from overflow list starting at %d",
          N+1, expected, iFirst);
      break;
    }
    if( checkRef(pCheck, iPage, zContext) ) break;
    if( sqlite3PagerGet(pCheck->pPager, (Pgno)iPage, &pOvflPage) ){
      checkAppendMsg(pCheck, zContext, "failed to get page %d", iPage);
      break;
    }
    pOvflData = (unsigned char *)sqlite3PagerGetData(pOvflPage);
    if( isFreeList ){
      int n = get4byte(&pOvflData[4]);
#ifndef SQLITE_OMIT_AUTOVACUUM
      if( pCheck->pBt->autoVacuum ){
        checkPtrmap(pCheck, iPage, PTRMAP_FREEPAGE, 0, zContext);
      }
#endif
      if( n>(int)pCheck->pBt->usableSize/4-2 ){
        checkAppendMsg(pCheck, zContext,
           "freelist leaf count too big on page %d", iPage);
        N--;
      }else{
        for(i=0; i<n; i++){
          Pgno iFreePage = get4byte(&pOvflData[8+i*4]);
#ifndef SQLITE_OMIT_AUTOVACUUM
          if( pCheck->pBt->autoVacuum ){
            checkPtrmap(pCheck, iFreePage, PTRMAP_FREEPAGE, 0, zContext);
          }
#endif
          checkRef(pCheck, iFreePage, zContext);
        }
        N -= n;
      }
    }
#ifndef SQLITE_OMIT_AUTOVACUUM
    else{
      if( pCheck->pBt->autoVacuum && N>0 ){
        i = get4byte(pOvflData);
        checkPtrmap(pCheck, i, PTRMAP_OVERFLOW2, iPage, zContext);
      }
    }
#endif
    iPage = get4byte(pOvflData);
    sqlite3PagerUnref(pOvflPage);
  }
}
#endif 

#ifndef SQLITE_OMIT_INTEGRITY_CHECK
static int checkTreePage(
  IntegrityCk *pCheck,  
  int iPage,            
  char *zParentContext, 
  i64 *pnParentMinKey, 
  i64 *pnParentMaxKey
){
  MemPage *pPage;
  int i, rc, depth, d2, pgno, cnt;
  int hdr, cellStart;
  int nCell;
  u8 *data;
  BtShared *pBt;
  int usableSize;
  char zContext[100];
  char *hit = 0;
  i64 nMinKey = 0;
  i64 nMaxKey = 0;

  sqlite3_snprintf(sizeof(zContext), zContext, "Page %d: ", iPage);

  pBt = pCheck->pBt;
  usableSize = pBt->usableSize;
  if( iPage==0 ) return 0;
  if( checkRef(pCheck, iPage, zParentContext) ) return 0;
  if( (rc = btreeGetPage(pBt, (Pgno)iPage, &pPage, 0))!=0 ){
    checkAppendMsg(pCheck, zContext,
       "unable to get the page. error code=%d", rc);
    return 0;
  }

  pPage->isInit = 0;
  if( (rc = btreeInitPage(pPage))!=0 ){
    assert( rc==SQLITE_CORRUPT );  
    checkAppendMsg(pCheck, zContext, 
                   "btreeInitPage() returns error code %d", rc);
    releasePage(pPage);
    return 0;
  }

  depth = 0;
  for(i=0; i<pPage->nCell && pCheck->mxErr; i++){
    u8 *pCell;
    u32 sz;
    CellInfo info;

    sqlite3_snprintf(sizeof(zContext), zContext,
             "On tree page %d cell %d: ", iPage, i);
    pCell = findCell(pPage,i);
    btreeParseCellPtr(pPage, pCell, &info);
    sz = info.nData;
    if( !pPage->intKey ) sz += (int)info.nKey;
    else if( i==0 ) nMinKey = nMaxKey = info.nKey;
    else{
      if( info.nKey <= nMaxKey ){
        checkAppendMsg(pCheck, zContext, 
            "Rowid %lld out of order (previous was %lld)", info.nKey, nMaxKey);
      }
      nMaxKey = info.nKey;
    }
    assert( sz==info.nPayload );
    if( (sz>info.nLocal) 
     && (&pCell[info.iOverflow]<=&pPage->aData[pBt->usableSize])
    ){
      int nPage = (sz - info.nLocal + usableSize - 5)/(usableSize - 4);
      Pgno pgnoOvfl = get4byte(&pCell[info.iOverflow]);
#ifndef SQLITE_OMIT_AUTOVACUUM
      if( pBt->autoVacuum ){
        checkPtrmap(pCheck, pgnoOvfl, PTRMAP_OVERFLOW1, iPage, zContext);
      }
#endif
      checkList(pCheck, 0, pgnoOvfl, nPage, zContext);
    }

    if( !pPage->leaf ){
      pgno = get4byte(pCell);
#ifndef SQLITE_OMIT_AUTOVACUUM
      if( pBt->autoVacuum ){
        checkPtrmap(pCheck, pgno, PTRMAP_BTREE, iPage, zContext);
      }
#endif
      d2 = checkTreePage(pCheck, pgno, zContext, &nMinKey, i==0 ? NULL : &nMaxKey);
      if( i>0 && d2!=depth ){
        checkAppendMsg(pCheck, zContext, "Child page depth differs");
      }
      depth = d2;
    }
  }

  if( !pPage->leaf ){
    pgno = get4byte(&pPage->aData[pPage->hdrOffset+8]);
    sqlite3_snprintf(sizeof(zContext), zContext, 
                     "On page %d at right child: ", iPage);
#ifndef SQLITE_OMIT_AUTOVACUUM
    if( pBt->autoVacuum ){
      checkPtrmap(pCheck, pgno, PTRMAP_BTREE, iPage, zContext);
    }
#endif
    checkTreePage(pCheck, pgno, zContext, NULL, !pPage->nCell ? NULL : &nMaxKey);
  }
 
  if( pPage->leaf && pPage->intKey ){
    
    if( pnParentMinKey ){
      
      if( !pnParentMaxKey ){
        if( nMaxKey > *pnParentMinKey ){
          checkAppendMsg(pCheck, zContext, 
              "Rowid %lld out of order (max larger than parent min of %lld)",
              nMaxKey, *pnParentMinKey);
        }
      }else{
        if( nMinKey <= *pnParentMinKey ){
          checkAppendMsg(pCheck, zContext, 
              "Rowid %lld out of order (min less than parent min of %lld)",
              nMinKey, *pnParentMinKey);
        }
        if( nMaxKey > *pnParentMaxKey ){
          checkAppendMsg(pCheck, zContext, 
              "Rowid %lld out of order (max larger than parent max of %lld)",
              nMaxKey, *pnParentMaxKey);
        }
        *pnParentMinKey = nMaxKey;
      }
    
    } else if( pnParentMaxKey ){
      if( nMinKey <= *pnParentMaxKey ){
        checkAppendMsg(pCheck, zContext, 
            "Rowid %lld out of order (min less than parent max of %lld)",
            nMinKey, *pnParentMaxKey);
      }
    }
  }

  data = pPage->aData;
  hdr = pPage->hdrOffset;
  hit = sqlite3PageMalloc( pBt->pageSize );
  if( hit==0 ){
    pCheck->mallocFailed = 1;
  }else{
    int contentOffset = get2byteNotZero(&data[hdr+5]);
    assert( contentOffset<=usableSize );  
    memset(hit+contentOffset, 0, usableSize-contentOffset);
    memset(hit, 1, contentOffset);
    nCell = get2byte(&data[hdr+3]);
    cellStart = hdr + 12 - 4*pPage->leaf;
    for(i=0; i<nCell; i++){
      int pc = get2byte(&data[cellStart+i*2]);
      u32 size = 65536;
      int j;
      if( pc<=usableSize-4 ){
        size = cellSizePtr(pPage, &data[pc]);
      }
      if( (int)(pc+size-1)>=usableSize ){
        checkAppendMsg(pCheck, 0, 
            "Corruption detected in cell %d on page %d",i,iPage);
      }else{
        for(j=pc+size-1; j>=pc; j--) hit[j]++;
      }
    }
    i = get2byte(&data[hdr+1]);
    while( i>0 ){
      int size, j;
      assert( i<=usableSize-4 );     
      size = get2byte(&data[i+2]);
      assert( i+size<=usableSize );  
      for(j=i+size-1; j>=i; j--) hit[j]++;
      j = get2byte(&data[i]);
      assert( j==0 || j>i+size );  
      assert( j<=usableSize-4 );   
      i = j;
    }
    for(i=cnt=0; i<usableSize; i++){
      if( hit[i]==0 ){
        cnt++;
      }else if( hit[i]>1 ){
        checkAppendMsg(pCheck, 0,
          "Multiple uses for byte %d of page %d", i, iPage);
        break;
      }
    }
    if( cnt!=data[hdr+7] ){
      checkAppendMsg(pCheck, 0, 
          "Fragmentation of %d bytes reported as %d on page %d",
          cnt, data[hdr+7], iPage);
    }
  }
  sqlite3PageFree(hit);
  releasePage(pPage);
  return depth+1;
}
#endif 

#ifndef SQLITE_OMIT_INTEGRITY_CHECK
char *sqlite3BtreeIntegrityCheck(
  Btree *p,     
  int *aRoot,   
  int nRoot,    
  int mxErr,    
  int *pnErr    
){
  Pgno i;
  int nRef;
  IntegrityCk sCheck;
  BtShared *pBt = p->pBt;
  char zErr[100];

  sqlite3BtreeEnter(p);
  assert( p->inTrans>TRANS_NONE && pBt->inTransaction>TRANS_NONE );
  nRef = sqlite3PagerRefcount(pBt->pPager);
  sCheck.pBt = pBt;
  sCheck.pPager = pBt->pPager;
  sCheck.nPage = btreePagecount(sCheck.pBt);
  sCheck.mxErr = mxErr;
  sCheck.nErr = 0;
  sCheck.mallocFailed = 0;
  *pnErr = 0;
  if( sCheck.nPage==0 ){
    sqlite3BtreeLeave(p);
    return 0;
  }
  sCheck.anRef = sqlite3Malloc( (sCheck.nPage+1)*sizeof(sCheck.anRef[0]) );
  if( !sCheck.anRef ){
    *pnErr = 1;
    sqlite3BtreeLeave(p);
    return 0;
  }
  for(i=0; i<=sCheck.nPage; i++){ sCheck.anRef[i] = 0; }
  i = PENDING_BYTE_PAGE(pBt);
  if( i<=sCheck.nPage ){
    sCheck.anRef[i] = 1;
  }
  sqlite3StrAccumInit(&sCheck.errMsg, zErr, sizeof(zErr), 20000);
  sCheck.errMsg.useMalloc = 2;

  checkList(&sCheck, 1, get4byte(&pBt->pPage1->aData[32]),
            get4byte(&pBt->pPage1->aData[36]), "Main freelist: ");

  for(i=0; (int)i<nRoot && sCheck.mxErr; i++){
    if( aRoot[i]==0 ) continue;
#ifndef SQLITE_OMIT_AUTOVACUUM
    if( pBt->autoVacuum && aRoot[i]>1 ){
      checkPtrmap(&sCheck, aRoot[i], PTRMAP_ROOTPAGE, 0, 0);
    }
#endif
    checkTreePage(&sCheck, aRoot[i], "List of tree roots: ", NULL, NULL);
  }

  for(i=1; i<=sCheck.nPage && sCheck.mxErr; i++){
#ifdef SQLITE_OMIT_AUTOVACUUM
    if( sCheck.anRef[i]==0 ){
      checkAppendMsg(&sCheck, 0, "Page %d is never used", i);
    }
#else
    if( sCheck.anRef[i]==0 && 
       (PTRMAP_PAGENO(pBt, i)!=i || !pBt->autoVacuum) ){
      checkAppendMsg(&sCheck, 0, "Page %d is never used", i);
    }
    if( sCheck.anRef[i]!=0 && 
       (PTRMAP_PAGENO(pBt, i)==i && pBt->autoVacuum) ){
      checkAppendMsg(&sCheck, 0, "Pointer map page %d is referenced", i);
    }
#endif
  }

  if( NEVER(nRef != sqlite3PagerRefcount(pBt->pPager)) ){
    checkAppendMsg(&sCheck, 0, 
      "Outstanding page count goes from %d to %d during this analysis",
      nRef, sqlite3PagerRefcount(pBt->pPager)
    );
  }

  sqlite3BtreeLeave(p);
  sqlite3_free(sCheck.anRef);
  if( sCheck.mallocFailed ){
    sqlite3StrAccumReset(&sCheck.errMsg);
    *pnErr = sCheck.nErr+1;
    return 0;
  }
  *pnErr = sCheck.nErr;
  if( sCheck.nErr==0 ) sqlite3StrAccumReset(&sCheck.errMsg);
  return sqlite3StrAccumFinish(&sCheck.errMsg);
}
#endif 

const char *sqlite3BtreeGetFilename(Btree *p){
  assert( p->pBt->pPager!=0 );
  return sqlite3PagerFilename(p->pBt->pPager);
}

const char *sqlite3BtreeGetJournalname(Btree *p){
  assert( p->pBt->pPager!=0 );
  return sqlite3PagerJournalname(p->pBt->pPager);
}

int sqlite3BtreeIsInTrans(Btree *p){
  assert( p==0 || sqlite3_mutex_held(p->db->mutex) );
  return (p && (p->inTrans==TRANS_WRITE));
}

#ifndef SQLITE_OMIT_WAL
int sqlite3BtreeCheckpoint(Btree *p, int eMode, int *pnLog, int *pnCkpt){
  int rc = SQLITE_OK;
  if( p ){
    BtShared *pBt = p->pBt;
    sqlite3BtreeEnter(p);
    if( pBt->inTransaction!=TRANS_NONE ){
      rc = SQLITE_LOCKED;
    }else{
      rc = sqlite3PagerCheckpoint(pBt->pPager, eMode, pnLog, pnCkpt);
    }
    sqlite3BtreeLeave(p);
  }
  return rc;
}
#endif

int sqlite3BtreeIsInReadTrans(Btree *p){
  assert( p );
  assert( sqlite3_mutex_held(p->db->mutex) );
  return p->inTrans!=TRANS_NONE;
}

int sqlite3BtreeIsInBackup(Btree *p){
  assert( p );
  assert( sqlite3_mutex_held(p->db->mutex) );
  return p->nBackup!=0;
}

void *sqlite3BtreeSchema(Btree *p, int nBytes, void(*xFree)(void *)){
  BtShared *pBt = p->pBt;
  sqlite3BtreeEnter(p);
  if( !pBt->pSchema && nBytes ){
    pBt->pSchema = sqlite3DbMallocZero(0, nBytes);
    pBt->xFreeSchema = xFree;
  }
  sqlite3BtreeLeave(p);
  return pBt->pSchema;
}

int sqlite3BtreeSchemaLocked(Btree *p){
  int rc;
  assert( sqlite3_mutex_held(p->db->mutex) );
  sqlite3BtreeEnter(p);
  rc = querySharedCacheTableLock(p, MASTER_ROOT, READ_LOCK);
  assert( rc==SQLITE_OK || rc==SQLITE_LOCKED_SHAREDCACHE );
  sqlite3BtreeLeave(p);
  return rc;
}


#ifndef SQLITE_OMIT_SHARED_CACHE
int sqlite3BtreeLockTable(Btree *p, int iTab, u8 isWriteLock){
  int rc = SQLITE_OK;
  assert( p->inTrans!=TRANS_NONE );
  if( p->sharable ){
    u8 lockType = READ_LOCK + isWriteLock;
    assert( READ_LOCK+1==WRITE_LOCK );
    assert( isWriteLock==0 || isWriteLock==1 );

    sqlite3BtreeEnter(p);
    rc = querySharedCacheTableLock(p, iTab, lockType);
    if( rc==SQLITE_OK ){
      rc = setSharedCacheTableLock(p, iTab, lockType);
    }
    sqlite3BtreeLeave(p);
  }
  return rc;
}
#endif

#ifndef SQLITE_OMIT_INCRBLOB
int sqlite3BtreePutData(BtCursor *pCsr, u32 offset, u32 amt, void *z){
  int rc;
  assert( cursorHoldsMutex(pCsr) );
  assert( sqlite3_mutex_held(pCsr->pBtree->db->mutex) );
  assert( pCsr->isIncrblobHandle );

  rc = restoreCursorPosition(pCsr);
  if( rc!=SQLITE_OK ){
    return rc;
  }
  assert( pCsr->eState!=CURSOR_REQUIRESEEK );
  if( pCsr->eState!=CURSOR_VALID ){
    return SQLITE_ABORT;
  }

  if( !pCsr->wrFlag ){
    return SQLITE_READONLY;
  }
  assert( !pCsr->pBt->readOnly && pCsr->pBt->inTransaction==TRANS_WRITE );
  assert( hasSharedCacheTableLock(pCsr->pBtree, pCsr->pgnoRoot, 0, 2) );
  assert( !hasReadConflicts(pCsr->pBtree, pCsr->pgnoRoot) );
  assert( pCsr->apPage[pCsr->iPage]->intKey );

  return accessPayload(pCsr, offset, amt, (unsigned char *)z, 1);
}

void sqlite3BtreeCacheOverflow(BtCursor *pCur){
  assert( cursorHoldsMutex(pCur) );
  assert( sqlite3_mutex_held(pCur->pBtree->db->mutex) );
  invalidateOverflowCache(pCur);
  pCur->isIncrblobHandle = 1;
}
#endif

int sqlite3BtreeSetVersion(Btree *pBtree, int iVersion){
  BtShared *pBt = pBtree->pBt;
  int rc;                         
 
  assert( pBtree->inTrans==TRANS_NONE );
  assert( iVersion==1 || iVersion==2 );

  pBt->doNotUseWAL = (u8)(iVersion==1);

  rc = sqlite3BtreeBeginTrans(pBtree, 0);
  if( rc==SQLITE_OK ){
    u8 *aData = pBt->pPage1->aData;
    if( aData[18]!=(u8)iVersion || aData[19]!=(u8)iVersion ){
      rc = sqlite3BtreeBeginTrans(pBtree, 2);
      if( rc==SQLITE_OK ){
        rc = sqlite3PagerWrite(pBt->pPage1->pDbPage);
        if( rc==SQLITE_OK ){
          aData[18] = (u8)iVersion;
          aData[19] = (u8)iVersion;
        }
      }
    }
  }

  pBt->doNotUseWAL = 0;
  return rc;
}

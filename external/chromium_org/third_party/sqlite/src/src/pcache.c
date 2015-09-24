/*
** 2008 August 05
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This file implements that page cache.
*/
#include "sqliteInt.h"

struct PCache {
  PgHdr *pDirty, *pDirtyTail;         
  PgHdr *pSynced;                     
  int nRef;                           
  int nMax;                           
  int szPage;                         
  int szExtra;                        
  int bPurgeable;                     
  int (*xStress)(void*,PgHdr*);       
  void *pStress;                      
  sqlite3_pcache *pCache;             
  PgHdr *pPage1;                      
};

#ifdef SQLITE_ENABLE_EXPENSIVE_ASSERT
# define expensive_assert(X)  assert(X)
#else
# define expensive_assert(X)
#endif


#if !defined(NDEBUG) && defined(SQLITE_ENABLE_EXPENSIVE_ASSERT)
static int pcacheCheckSynced(PCache *pCache){
  PgHdr *p;
  for(p=pCache->pDirtyTail; p!=pCache->pSynced; p=p->pDirtyPrev){
    assert( p->nRef || (p->flags&PGHDR_NEED_SYNC) );
  }
  return (p==0 || p->nRef || (p->flags&PGHDR_NEED_SYNC)==0);
}
#endif 

static void pcacheRemoveFromDirtyList(PgHdr *pPage){
  PCache *p = pPage->pCache;

  assert( pPage->pDirtyNext || pPage==p->pDirtyTail );
  assert( pPage->pDirtyPrev || pPage==p->pDirty );

  
  if( p->pSynced==pPage ){
    PgHdr *pSynced = pPage->pDirtyPrev;
    while( pSynced && (pSynced->flags&PGHDR_NEED_SYNC) ){
      pSynced = pSynced->pDirtyPrev;
    }
    p->pSynced = pSynced;
  }

  if( pPage->pDirtyNext ){
    pPage->pDirtyNext->pDirtyPrev = pPage->pDirtyPrev;
  }else{
    assert( pPage==p->pDirtyTail );
    p->pDirtyTail = pPage->pDirtyPrev;
  }
  if( pPage->pDirtyPrev ){
    pPage->pDirtyPrev->pDirtyNext = pPage->pDirtyNext;
  }else{
    assert( pPage==p->pDirty );
    p->pDirty = pPage->pDirtyNext;
  }
  pPage->pDirtyNext = 0;
  pPage->pDirtyPrev = 0;

  expensive_assert( pcacheCheckSynced(p) );
}

static void pcacheAddToDirtyList(PgHdr *pPage){
  PCache *p = pPage->pCache;

  assert( pPage->pDirtyNext==0 && pPage->pDirtyPrev==0 && p->pDirty!=pPage );

  pPage->pDirtyNext = p->pDirty;
  if( pPage->pDirtyNext ){
    assert( pPage->pDirtyNext->pDirtyPrev==0 );
    pPage->pDirtyNext->pDirtyPrev = pPage;
  }
  p->pDirty = pPage;
  if( !p->pDirtyTail ){
    p->pDirtyTail = pPage;
  }
  if( !p->pSynced && 0==(pPage->flags&PGHDR_NEED_SYNC) ){
    p->pSynced = pPage;
  }
  expensive_assert( pcacheCheckSynced(p) );
}

static void pcacheUnpin(PgHdr *p){
  PCache *pCache = p->pCache;
  if( pCache->bPurgeable ){
    if( p->pgno==1 ){
      pCache->pPage1 = 0;
    }
    sqlite3GlobalConfig.pcache.xUnpin(pCache->pCache, p, 0);
  }
}

int sqlite3PcacheInitialize(void){
  if( sqlite3GlobalConfig.pcache.xInit==0 ){
    sqlite3PCacheSetDefault();
  }
  return sqlite3GlobalConfig.pcache.xInit(sqlite3GlobalConfig.pcache.pArg);
}
void sqlite3PcacheShutdown(void){
  if( sqlite3GlobalConfig.pcache.xShutdown ){
    
    sqlite3GlobalConfig.pcache.xShutdown(sqlite3GlobalConfig.pcache.pArg);
  }
}

int sqlite3PcacheSize(void){ return sizeof(PCache); }

void sqlite3PcacheOpen(
  int szPage,                  
  int szExtra,                 
  int bPurgeable,              
  int (*xStress)(void*,PgHdr*),
  void *pStress,               
  PCache *p                    
){
  memset(p, 0, sizeof(PCache));
  p->szPage = szPage;
  p->szExtra = szExtra;
  p->bPurgeable = bPurgeable;
  p->xStress = xStress;
  p->pStress = pStress;
  p->nMax = 100;
}

void sqlite3PcacheSetPageSize(PCache *pCache, int szPage){
  assert( pCache->nRef==0 && pCache->pDirty==0 );
  if( pCache->pCache ){
    sqlite3GlobalConfig.pcache.xDestroy(pCache->pCache);
    pCache->pCache = 0;
    pCache->pPage1 = 0;
  }
  pCache->szPage = szPage;
}

int sqlite3PcacheFetch(
  PCache *pCache,       
  Pgno pgno,            
  int createFlag,       
  PgHdr **ppPage        
){
  PgHdr *pPage = 0;
  int eCreate;

  assert( pCache!=0 );
  assert( createFlag==1 || createFlag==0 );
  assert( pgno>0 );

  if( !pCache->pCache && createFlag ){
    sqlite3_pcache *p;
    int nByte;
    nByte = pCache->szPage + pCache->szExtra + sizeof(PgHdr);
    p = sqlite3GlobalConfig.pcache.xCreate(nByte, pCache->bPurgeable);
    if( !p ){
      return SQLITE_NOMEM;
    }
    sqlite3GlobalConfig.pcache.xCachesize(p, pCache->nMax);
    pCache->pCache = p;
  }

  eCreate = createFlag * (1 + (!pCache->bPurgeable || !pCache->pDirty));
  if( pCache->pCache ){
    pPage = sqlite3GlobalConfig.pcache.xFetch(pCache->pCache, pgno, eCreate);
  }

  if( !pPage && eCreate==1 ){
    PgHdr *pPg;

    expensive_assert( pcacheCheckSynced(pCache) );
    for(pPg=pCache->pSynced; 
        pPg && (pPg->nRef || (pPg->flags&PGHDR_NEED_SYNC)); 
        pPg=pPg->pDirtyPrev
    );
    pCache->pSynced = pPg;
    if( !pPg ){
      for(pPg=pCache->pDirtyTail; pPg && pPg->nRef; pPg=pPg->pDirtyPrev);
    }
    if( pPg ){
      int rc;
      rc = pCache->xStress(pCache->pStress, pPg);
      if( rc!=SQLITE_OK && rc!=SQLITE_BUSY ){
        return rc;
      }
    }

    pPage = sqlite3GlobalConfig.pcache.xFetch(pCache->pCache, pgno, 2);
  }

  if( pPage ){
    if( !pPage->pData ){
      memset(pPage, 0, sizeof(PgHdr));
      pPage->pData = (void *)&pPage[1];
      pPage->pExtra = (void*)&((char *)pPage->pData)[pCache->szPage];
      memset(pPage->pExtra, 0, pCache->szExtra);
      pPage->pCache = pCache;
      pPage->pgno = pgno;
    }
    assert( pPage->pCache==pCache );
    assert( pPage->pgno==pgno );
    assert( pPage->pData==(void *)&pPage[1] );
    assert( pPage->pExtra==(void *)&((char *)&pPage[1])[pCache->szPage] );

    if( 0==pPage->nRef ){
      pCache->nRef++;
    }
    pPage->nRef++;
    if( pgno==1 ){
      pCache->pPage1 = pPage;
    }
  }
  *ppPage = pPage;
  return (pPage==0 && eCreate) ? SQLITE_NOMEM : SQLITE_OK;
}

void sqlite3PcacheRelease(PgHdr *p){
  assert( p->nRef>0 );
  p->nRef--;
  if( p->nRef==0 ){
    PCache *pCache = p->pCache;
    pCache->nRef--;
    if( (p->flags&PGHDR_DIRTY)==0 ){
      pcacheUnpin(p);
    }else{
      
      pcacheRemoveFromDirtyList(p);
      pcacheAddToDirtyList(p);
    }
  }
}

void sqlite3PcacheRef(PgHdr *p){
  assert(p->nRef>0);
  p->nRef++;
}

void sqlite3PcacheDrop(PgHdr *p){
  PCache *pCache;
  assert( p->nRef==1 );
  if( p->flags&PGHDR_DIRTY ){
    pcacheRemoveFromDirtyList(p);
  }
  pCache = p->pCache;
  pCache->nRef--;
  if( p->pgno==1 ){
    pCache->pPage1 = 0;
  }
  sqlite3GlobalConfig.pcache.xUnpin(pCache->pCache, p, 1);
}

void sqlite3PcacheMakeDirty(PgHdr *p){
  p->flags &= ~PGHDR_DONT_WRITE;
  assert( p->nRef>0 );
  if( 0==(p->flags & PGHDR_DIRTY) ){
    p->flags |= PGHDR_DIRTY;
    pcacheAddToDirtyList( p);
  }
}

void sqlite3PcacheMakeClean(PgHdr *p){
  if( (p->flags & PGHDR_DIRTY) ){
    pcacheRemoveFromDirtyList(p);
    p->flags &= ~(PGHDR_DIRTY|PGHDR_NEED_SYNC);
    if( p->nRef==0 ){
      pcacheUnpin(p);
    }
  }
}

void sqlite3PcacheCleanAll(PCache *pCache){
  PgHdr *p;
  while( (p = pCache->pDirty)!=0 ){
    sqlite3PcacheMakeClean(p);
  }
}

void sqlite3PcacheClearSyncFlags(PCache *pCache){
  PgHdr *p;
  for(p=pCache->pDirty; p; p=p->pDirtyNext){
    p->flags &= ~PGHDR_NEED_SYNC;
  }
  pCache->pSynced = pCache->pDirtyTail;
}

void sqlite3PcacheMove(PgHdr *p, Pgno newPgno){
  PCache *pCache = p->pCache;
  assert( p->nRef>0 );
  assert( newPgno>0 );
  sqlite3GlobalConfig.pcache.xRekey(pCache->pCache, p, p->pgno, newPgno);
  p->pgno = newPgno;
  if( (p->flags&PGHDR_DIRTY) && (p->flags&PGHDR_NEED_SYNC) ){
    pcacheRemoveFromDirtyList(p);
    pcacheAddToDirtyList(p);
  }
}

void sqlite3PcacheTruncate(PCache *pCache, Pgno pgno){
  if( pCache->pCache ){
    PgHdr *p;
    PgHdr *pNext;
    for(p=pCache->pDirty; p; p=pNext){
      pNext = p->pDirtyNext;
      assert( p->pgno>0 );
      if( ALWAYS(p->pgno>pgno) ){
        assert( p->flags&PGHDR_DIRTY );
        sqlite3PcacheMakeClean(p);
      }
    }
    if( pgno==0 && pCache->pPage1 ){
      memset(pCache->pPage1->pData, 0, pCache->szPage);
      pgno = 1;
    }
    sqlite3GlobalConfig.pcache.xTruncate(pCache->pCache, pgno+1);
  }
}

void sqlite3PcacheClose(PCache *pCache){
  if( pCache->pCache ){
    sqlite3GlobalConfig.pcache.xDestroy(pCache->pCache);
  }
}

void sqlite3PcacheClear(PCache *pCache){
  sqlite3PcacheTruncate(pCache, 0);
}

static PgHdr *pcacheMergeDirtyList(PgHdr *pA, PgHdr *pB){
  PgHdr result, *pTail;
  pTail = &result;
  while( pA && pB ){
    if( pA->pgno<pB->pgno ){
      pTail->pDirty = pA;
      pTail = pA;
      pA = pA->pDirty;
    }else{
      pTail->pDirty = pB;
      pTail = pB;
      pB = pB->pDirty;
    }
  }
  if( pA ){
    pTail->pDirty = pA;
  }else if( pB ){
    pTail->pDirty = pB;
  }else{
    pTail->pDirty = 0;
  }
  return result.pDirty;
}

#define N_SORT_BUCKET  32
static PgHdr *pcacheSortDirtyList(PgHdr *pIn){
  PgHdr *a[N_SORT_BUCKET], *p;
  int i;
  memset(a, 0, sizeof(a));
  while( pIn ){
    p = pIn;
    pIn = p->pDirty;
    p->pDirty = 0;
    for(i=0; ALWAYS(i<N_SORT_BUCKET-1); i++){
      if( a[i]==0 ){
        a[i] = p;
        break;
      }else{
        p = pcacheMergeDirtyList(a[i], p);
        a[i] = 0;
      }
    }
    if( NEVER(i==N_SORT_BUCKET-1) ){
      a[i] = pcacheMergeDirtyList(a[i], p);
    }
  }
  p = a[0];
  for(i=1; i<N_SORT_BUCKET; i++){
    p = pcacheMergeDirtyList(p, a[i]);
  }
  return p;
}

PgHdr *sqlite3PcacheDirtyList(PCache *pCache){
  PgHdr *p;
  for(p=pCache->pDirty; p; p=p->pDirtyNext){
    p->pDirty = p->pDirtyNext;
  }
  return pcacheSortDirtyList(pCache->pDirty);
}

int sqlite3PcacheRefCount(PCache *pCache){
  return pCache->nRef;
}

int sqlite3PcachePageRefcount(PgHdr *p){
  return p->nRef;
}

int sqlite3PcachePagecount(PCache *pCache){
  int nPage = 0;
  if( pCache->pCache ){
    nPage = sqlite3GlobalConfig.pcache.xPagecount(pCache->pCache);
  }
  return nPage;
}

int sqlite3PcacheGetCachesize(PCache *pCache){
  return pCache->nMax;
}

void sqlite3PcacheSetCachesize(PCache *pCache, int mxPage){
  pCache->nMax = mxPage;
  if( pCache->pCache ){
    sqlite3GlobalConfig.pcache.xCachesize(pCache->pCache, mxPage);
  }
}

#if defined(SQLITE_CHECK_PAGES) || defined(SQLITE_DEBUG)
void sqlite3PcacheIterateDirty(PCache *pCache, void (*xIter)(PgHdr *)){
  PgHdr *pDirty;
  for(pDirty=pCache->pDirty; pDirty; pDirty=pDirty->pDirtyNext){
    xIter(pDirty);
  }
}
#endif

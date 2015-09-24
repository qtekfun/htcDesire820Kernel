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
** This header file defines the interface that the sqlite page cache
** subsystem. 
*/

#ifndef _PCACHE_H_

typedef struct PgHdr PgHdr;
typedef struct PCache PCache;

struct PgHdr {
  void *pData;                   
  void *pExtra;                  
  PgHdr *pDirty;                 
  Pgno pgno;                     
  Pager *pPager;                 
#ifdef SQLITE_CHECK_PAGES
  u32 pageHash;                  
#endif
  u16 flags;                     

  i16 nRef;                      
  PCache *pCache;                

  PgHdr *pDirtyNext;             
  PgHdr *pDirtyPrev;             
};

#define PGHDR_DIRTY             0x002  
#define PGHDR_NEED_SYNC         0x004  
#define PGHDR_NEED_READ         0x008  
#define PGHDR_REUSE_UNLIKELY    0x010  
#define PGHDR_DONT_WRITE        0x020  

int sqlite3PcacheInitialize(void);
void sqlite3PcacheShutdown(void);

void sqlite3PCacheBufferSetup(void *, int sz, int n);

void sqlite3PcacheOpen(
  int szPage,                    
  int szExtra,                   
  int bPurgeable,                
  int (*xStress)(void*, PgHdr*), 
  void *pStress,                 
  PCache *pToInit                
);

void sqlite3PcacheSetPageSize(PCache *, int);

int sqlite3PcacheSize(void);

int sqlite3PcacheFetch(PCache*, Pgno, int createFlag, PgHdr**);
void sqlite3PcacheRelease(PgHdr*);

void sqlite3PcacheDrop(PgHdr*);         
void sqlite3PcacheMakeDirty(PgHdr*);    
void sqlite3PcacheMakeClean(PgHdr*);    
void sqlite3PcacheCleanAll(PCache*);    

void sqlite3PcacheMove(PgHdr*, Pgno);

void sqlite3PcacheTruncate(PCache*, Pgno x);

PgHdr *sqlite3PcacheDirtyList(PCache*);

void sqlite3PcacheClose(PCache*);

void sqlite3PcacheClearSyncFlags(PCache *);

void sqlite3PcacheClear(PCache*);

int sqlite3PcacheRefCount(PCache*);

void sqlite3PcacheRef(PgHdr*);

int sqlite3PcachePageRefcount(PgHdr*);

int sqlite3PcachePagecount(PCache*);

#if defined(SQLITE_CHECK_PAGES) || defined(SQLITE_DEBUG)
void sqlite3PcacheIterateDirty(PCache *pCache, void (*xIter)(PgHdr *));
#endif

void sqlite3PcacheSetCachesize(PCache *, int);
int sqlite3PcacheGetCachesize(PCache *);

#ifdef SQLITE_ENABLE_MEMORY_MANAGEMENT
int sqlite3PcacheReleaseMemory(int);
#endif

#ifdef SQLITE_TEST
void sqlite3PcacheStats(int*,int*,int*,int*);
#endif

void sqlite3PCacheSetDefault(void);

#endif 

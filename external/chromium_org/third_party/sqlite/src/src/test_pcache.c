/*
** 2008 November 18
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
** This file contains code used for testing the SQLite system.
** None of the code in this file goes into a deliverable build.
** 
** This file contains an application-defined pager cache
** implementation that can be plugged in in place of the
** default pcache.  This alternative pager cache will throw
** some errors that the default cache does not.
**
** This pagecache implementation is designed for simplicity
** not speed.  
*/
#include "sqlite3.h"
#include <string.h>
#include <assert.h>

typedef struct testpcacheGlobalType testpcacheGlobalType;
struct testpcacheGlobalType {
  void *pDummy;             
  int nInstance;            
  unsigned discardChance;   
  unsigned prngSeed;        
  unsigned highStress;      
};
static testpcacheGlobalType testpcacheGlobal;

static int testpcacheInit(void *pArg){
  assert( pArg==(void*)&testpcacheGlobal );
  assert( testpcacheGlobal.pDummy==0 );
  assert( testpcacheGlobal.nInstance==0 );
  testpcacheGlobal.pDummy = sqlite3_malloc(10);
  return testpcacheGlobal.pDummy==0 ? SQLITE_NOMEM : SQLITE_OK;
}

static void testpcacheShutdown(void *pArg){
  assert( pArg==(void*)&testpcacheGlobal );
  assert( testpcacheGlobal.pDummy!=0 );
  assert( testpcacheGlobal.nInstance==0 );
  sqlite3_free( testpcacheGlobal.pDummy );
  testpcacheGlobal.pDummy = 0;
}

#if defined(SQLITE_TEMP_STORE) && SQLITE_TEMP_STORE>=2
# define TESTPCACHE_NPAGE    499
#else
# define TESTPCACHE_NPAGE    217
#endif
#define TESTPCACHE_RESERVE   17

#define TESTPCACHE_VALID  0x364585fd
#define TESTPCACHE_CLEAR  0xd42670d4

typedef struct testpcache testpcache;
struct testpcache {
  int szPage;               
  int bPurgeable;           
  int nFree;                
  int nPinned;              
  unsigned iRand;           
  unsigned iMagic;          
  struct testpcachePage {
    unsigned key;              
    int isPinned;              
    void *pData;               
  } a[TESTPCACHE_NPAGE];    
};

static unsigned testpcacheRandom(testpcache *p){
  unsigned x = 0;
  int i;
  for(i=0; i<4; i++){
    p->iRand = (p->iRand*69069 + 5);
    x = (x<<8) | ((p->iRand>>16)&0xff);
  }
  return x;
}


static sqlite3_pcache *testpcacheCreate(int szPage, int bPurgeable){
  int nMem;
  char *x;
  testpcache *p;
  int i;
  assert( testpcacheGlobal.pDummy!=0 );
  szPage = (szPage+7)&~7;
  nMem = sizeof(testpcache) + TESTPCACHE_NPAGE*szPage;
  p = sqlite3_malloc( nMem );
  if( p==0 ) return 0;
  x = (char*)&p[1];
  p->szPage = szPage;
  p->nFree = TESTPCACHE_NPAGE;
  p->nPinned = 0;
  p->iRand = testpcacheGlobal.prngSeed;
  p->bPurgeable = bPurgeable;
  p->iMagic = TESTPCACHE_VALID;
  for(i=0; i<TESTPCACHE_NPAGE; i++, x += szPage){
    p->a[i].key = 0;
    p->a[i].isPinned = 0;
    p->a[i].pData = (void*)x;
  }
  testpcacheGlobal.nInstance++;
  return (sqlite3_pcache*)p;
}

static void testpcacheCachesize(sqlite3_pcache *pCache, int newSize){
  testpcache *p = (testpcache*)pCache;
  assert( p->iMagic==TESTPCACHE_VALID );
  assert( newSize>=1 );
  assert( testpcacheGlobal.pDummy!=0 );
  assert( testpcacheGlobal.nInstance>0 );
}

static int testpcachePagecount(sqlite3_pcache *pCache){
  testpcache *p = (testpcache*)pCache;
  assert( p->iMagic==TESTPCACHE_VALID );
  assert( testpcacheGlobal.pDummy!=0 );
  assert( testpcacheGlobal.nInstance>0 );
  return TESTPCACHE_NPAGE - p->nFree;
}

static void *testpcacheFetch(
  sqlite3_pcache *pCache,
  unsigned key,
  int createFlag
){
  testpcache *p = (testpcache*)pCache;
  int i, j;
  assert( p->iMagic==TESTPCACHE_VALID );
  assert( testpcacheGlobal.pDummy!=0 );
  assert( testpcacheGlobal.nInstance>0 );

  
  for(i=0; i<TESTPCACHE_NPAGE; i++){
    if( p->a[i].key==key ){
      if( !p->a[i].isPinned ){
        p->nPinned++;
        assert( p->nPinned <= TESTPCACHE_NPAGE - p->nFree );
        p->a[i].isPinned = 1;
      }
      return p->a[i].pData;
    }
  }

  
  if( createFlag==0 ){
    return 0;
  }

  
  if( p->nPinned==TESTPCACHE_NPAGE ){
    return 0;
  }

  
  if( p->nPinned>=TESTPCACHE_NPAGE-TESTPCACHE_RESERVE && createFlag<2 ){
    return 0;
  }

  if( testpcacheGlobal.highStress && createFlag<2 ){
    return 0;
  }

  if( p->nFree>TESTPCACHE_RESERVE || (createFlag==2 && p->nFree>0) ){
    j = testpcacheRandom(p) % TESTPCACHE_NPAGE;
    for(i=0; i<TESTPCACHE_NPAGE; i++, j = (j+1)%TESTPCACHE_NPAGE){
      if( p->a[j].key==0 ){
        p->a[j].key = key;
        p->a[j].isPinned = 1;
        memset(p->a[j].pData, 0, p->szPage);
        p->nPinned++;
        p->nFree--;
        assert( p->nPinned <= TESTPCACHE_NPAGE - p->nFree );
        return p->a[j].pData;
      }
    }

    
    assert( 0 );
  }

  if( p->bPurgeable==0 ){
    return 0;
  }

  j = testpcacheRandom(p) % TESTPCACHE_NPAGE;
  for(i=0; i<TESTPCACHE_NPAGE; i++, j = (j+1)%TESTPCACHE_NPAGE){
    if( p->a[j].key>0 && p->a[j].isPinned==0 ){
      p->a[j].key = key;
      p->a[j].isPinned = 1;
      memset(p->a[j].pData, 0, p->szPage);
      p->nPinned++;
      assert( p->nPinned <= TESTPCACHE_NPAGE - p->nFree );
      return p->a[j].pData;
    }
  }

  
  assert(0);
  return 0;
}

static void testpcacheUnpin(
  sqlite3_pcache *pCache,
  void *pOldPage,
  int discard
){
  testpcache *p = (testpcache*)pCache;
  int i;
  assert( p->iMagic==TESTPCACHE_VALID );
  assert( testpcacheGlobal.pDummy!=0 );
  assert( testpcacheGlobal.nInstance>0 );

  if( p->bPurgeable
  && (100-testpcacheGlobal.discardChance) <= (testpcacheRandom(p)%100)
  ){
    discard = 1;
  }

  for(i=0; i<TESTPCACHE_NPAGE; i++){
    if( p->a[i].pData==pOldPage ){
      
      assert( p->a[i].isPinned );
      p->a[i].isPinned = 0;
      p->nPinned--;
      assert( p->nPinned>=0 );
      if( discard ){
        p->a[i].key = 0;
        p->nFree++;
        assert( p->nFree<=TESTPCACHE_NPAGE );
      }
      return;
    }
  }

  
  assert( 0 );
}


static void testpcacheRekey(
  sqlite3_pcache *pCache,
  void *pOldPage,
  unsigned oldKey,
  unsigned newKey
){
  testpcache *p = (testpcache*)pCache;
  int i;
  assert( p->iMagic==TESTPCACHE_VALID );
  assert( testpcacheGlobal.pDummy!=0 );
  assert( testpcacheGlobal.nInstance>0 );

  for(i=0; i<TESTPCACHE_NPAGE; i++){
    if( p->a[i].key==newKey ){
      
      assert( p->a[i].isPinned==0 );
      p->a[i].key = 0;
      p->nFree++;
      assert( p->nFree<=TESTPCACHE_NPAGE );
      break;
    }
  }

  for(i=0; i<TESTPCACHE_NPAGE; i++){
    if( p->a[i].key==oldKey ){
      
      assert( p->a[i].pData==pOldPage );
      
      assert( p->a[i].isPinned );
      p->a[i].key = newKey;
      return;
    }
  }

  
  assert( 0 );
}


static void testpcacheTruncate(sqlite3_pcache *pCache, unsigned iLimit){
  testpcache *p = (testpcache*)pCache;
  unsigned int i;
  assert( p->iMagic==TESTPCACHE_VALID );
  assert( testpcacheGlobal.pDummy!=0 );
  assert( testpcacheGlobal.nInstance>0 );
  for(i=0; i<TESTPCACHE_NPAGE; i++){
    if( p->a[i].key>=iLimit ){
      p->a[i].key = 0;
      if( p->a[i].isPinned ){
        p->nPinned--;
        assert( p->nPinned>=0 );
      }
      p->nFree++;
      assert( p->nFree<=TESTPCACHE_NPAGE );
    }
  }
}

static void testpcacheDestroy(sqlite3_pcache *pCache){
  testpcache *p = (testpcache*)pCache;
  assert( p->iMagic==TESTPCACHE_VALID );
  assert( testpcacheGlobal.pDummy!=0 );
  assert( testpcacheGlobal.nInstance>0 );
  p->iMagic = TESTPCACHE_CLEAR;
  sqlite3_free(p);
  testpcacheGlobal.nInstance--;
}


void installTestPCache(
  int installFlag,            
  unsigned discardChance,     
  unsigned prngSeed,          
  unsigned highStress         
){
  static const sqlite3_pcache_methods testPcache = {
    (void*)&testpcacheGlobal,
    testpcacheInit,
    testpcacheShutdown,
    testpcacheCreate,
    testpcacheCachesize,
    testpcachePagecount,
    testpcacheFetch,
    testpcacheUnpin,
    testpcacheRekey,
    testpcacheTruncate,
    testpcacheDestroy,
  };
  static sqlite3_pcache_methods defaultPcache;
  static int isInstalled = 0;

  assert( testpcacheGlobal.nInstance==0 );
  assert( testpcacheGlobal.pDummy==0 );
  assert( discardChance<=100 );
  testpcacheGlobal.discardChance = discardChance;
  testpcacheGlobal.prngSeed = prngSeed ^ (prngSeed<<16);
  testpcacheGlobal.highStress = highStress;
  if( installFlag!=isInstalled ){
    if( installFlag ){
      sqlite3_config(SQLITE_CONFIG_GETPCACHE, &defaultPcache);
      assert( defaultPcache.xCreate!=testpcacheCreate );
      sqlite3_config(SQLITE_CONFIG_PCACHE, &testPcache);
    }else{
      assert( defaultPcache.xCreate!=0 );
      sqlite3_config(SQLITE_CONFIG_PCACHE, &defaultPcache);
    }
    isInstalled = installFlag;
  }
}

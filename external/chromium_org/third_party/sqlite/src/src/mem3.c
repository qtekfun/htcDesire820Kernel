/*
** 2007 October 14
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This file contains the C functions that implement a memory
** allocation subsystem for use by SQLite. 
**
** This version of the memory allocation subsystem omits all
** use of malloc(). The SQLite user supplies a block of memory
** before calling sqlite3_initialize() from which allocations
** are made and returned by the xMalloc() and xRealloc() 
** implementations. Once sqlite3_initialize() has been called,
** the amount of memory available to SQLite is fixed and cannot
** be changed.
**
** This version of the memory allocation subsystem is included
** in the build only if SQLITE_ENABLE_MEMSYS3 is defined.
*/
#include "sqliteInt.h"

#ifdef SQLITE_ENABLE_MEMSYS3

#define MX_SMALL 10


#define N_HASH  61

/*
** A memory allocation (also called a "chunk") consists of two or 
** more blocks where each block is 8 bytes.  The first 8 bytes are 
** a header that is not returned to the user.
**
** A chunk is two or more blocks that is either checked out or
** free.  The first block has format u.hdr.  u.hdr.size4x is 4 times the
** size of the allocation in blocks if the allocation is free.
** The u.hdr.size4x&1 bit is true if the chunk is checked out and
** false if the chunk is on the freelist.  The u.hdr.size4x&2 bit
** is true if the previous chunk is checked out and false if the
** previous chunk is free.  The u.hdr.prevSize field is the size of
** the previous chunk in blocks if the previous chunk is on the
** freelist. If the previous chunk is checked out, then
** u.hdr.prevSize can be part of the data for that chunk and should
** not be read or written.
**
** We often identify a chunk by its index in mem3.aPool[].  When
** this is done, the chunk index refers to the second block of
** the chunk.  In this way, the first chunk has an index of 1.
** A chunk index of 0 means "no such chunk" and is the equivalent
** of a NULL pointer.
**
** The second block of free chunks is of the form u.list.  The
** two fields form a double-linked list of chunks of related sizes.
** Pointers to the head of the list are stored in mem3.aiSmall[] 
** for smaller chunks and mem3.aiHash[] for larger chunks.
**
** The second block of a chunk is user data if the chunk is checked 
** out.  If a chunk is checked out, the user data may extend into
** the u.hdr.prevSize value of the following chunk.
*/
typedef struct Mem3Block Mem3Block;
struct Mem3Block {
  union {
    struct {
      u32 prevSize;   
      u32 size4x;     
    } hdr;
    struct {
      u32 next;       
      u32 prev;       
    } list;
  } u;
};

static SQLITE_WSD struct Mem3Global {
  u32 nPool;
  Mem3Block *aPool;

  int alarmBusy;
  
  sqlite3_mutex *mutex;
  
  u32 mnMaster;

  u32 iMaster;
  u32 szMaster;

  u32 aiSmall[MX_SMALL-1];   
  u32 aiHash[N_HASH];        
} mem3 = { 97535575 };

#define mem3 GLOBAL(struct Mem3Global, mem3)

static void memsys3UnlinkFromList(u32 i, u32 *pRoot){
  u32 next = mem3.aPool[i].u.list.next;
  u32 prev = mem3.aPool[i].u.list.prev;
  assert( sqlite3_mutex_held(mem3.mutex) );
  if( prev==0 ){
    *pRoot = next;
  }else{
    mem3.aPool[prev].u.list.next = next;
  }
  if( next ){
    mem3.aPool[next].u.list.prev = prev;
  }
  mem3.aPool[i].u.list.next = 0;
  mem3.aPool[i].u.list.prev = 0;
}

static void memsys3Unlink(u32 i){
  u32 size, hash;
  assert( sqlite3_mutex_held(mem3.mutex) );
  assert( (mem3.aPool[i-1].u.hdr.size4x & 1)==0 );
  assert( i>=1 );
  size = mem3.aPool[i-1].u.hdr.size4x/4;
  assert( size==mem3.aPool[i+size-1].u.hdr.prevSize );
  assert( size>=2 );
  if( size <= MX_SMALL ){
    memsys3UnlinkFromList(i, &mem3.aiSmall[size-2]);
  }else{
    hash = size % N_HASH;
    memsys3UnlinkFromList(i, &mem3.aiHash[hash]);
  }
}

static void memsys3LinkIntoList(u32 i, u32 *pRoot){
  assert( sqlite3_mutex_held(mem3.mutex) );
  mem3.aPool[i].u.list.next = *pRoot;
  mem3.aPool[i].u.list.prev = 0;
  if( *pRoot ){
    mem3.aPool[*pRoot].u.list.prev = i;
  }
  *pRoot = i;
}

static void memsys3Link(u32 i){
  u32 size, hash;
  assert( sqlite3_mutex_held(mem3.mutex) );
  assert( i>=1 );
  assert( (mem3.aPool[i-1].u.hdr.size4x & 1)==0 );
  size = mem3.aPool[i-1].u.hdr.size4x/4;
  assert( size==mem3.aPool[i+size-1].u.hdr.prevSize );
  assert( size>=2 );
  if( size <= MX_SMALL ){
    memsys3LinkIntoList(i, &mem3.aiSmall[size-2]);
  }else{
    hash = size % N_HASH;
    memsys3LinkIntoList(i, &mem3.aiHash[hash]);
  }
}

static void memsys3Enter(void){
  if( sqlite3GlobalConfig.bMemstat==0 && mem3.mutex==0 ){
    mem3.mutex = sqlite3MutexAlloc(SQLITE_MUTEX_STATIC_MEM);
  }
  sqlite3_mutex_enter(mem3.mutex);
}
static void memsys3Leave(void){
  sqlite3_mutex_leave(mem3.mutex);
}

static void memsys3OutOfMemory(int nByte){
  if( !mem3.alarmBusy ){
    mem3.alarmBusy = 1;
    assert( sqlite3_mutex_held(mem3.mutex) );
    sqlite3_mutex_leave(mem3.mutex);
    sqlite3_release_memory(nByte);
    sqlite3_mutex_enter(mem3.mutex);
    mem3.alarmBusy = 0;
  }
}


static void *memsys3Checkout(u32 i, u32 nBlock){
  u32 x;
  assert( sqlite3_mutex_held(mem3.mutex) );
  assert( i>=1 );
  assert( mem3.aPool[i-1].u.hdr.size4x/4==nBlock );
  assert( mem3.aPool[i+nBlock-1].u.hdr.prevSize==nBlock );
  x = mem3.aPool[i-1].u.hdr.size4x;
  mem3.aPool[i-1].u.hdr.size4x = nBlock*4 | 1 | (x&2);
  mem3.aPool[i+nBlock-1].u.hdr.prevSize = nBlock;
  mem3.aPool[i+nBlock-1].u.hdr.size4x |= 2;
  return &mem3.aPool[i];
}

static void *memsys3FromMaster(u32 nBlock){
  assert( sqlite3_mutex_held(mem3.mutex) );
  assert( mem3.szMaster>=nBlock );
  if( nBlock>=mem3.szMaster-1 ){
    
    void *p = memsys3Checkout(mem3.iMaster, mem3.szMaster);
    mem3.iMaster = 0;
    mem3.szMaster = 0;
    mem3.mnMaster = 0;
    return p;
  }else{
    
    u32 newi, x;
    newi = mem3.iMaster + mem3.szMaster - nBlock;
    assert( newi > mem3.iMaster+1 );
    mem3.aPool[mem3.iMaster+mem3.szMaster-1].u.hdr.prevSize = nBlock;
    mem3.aPool[mem3.iMaster+mem3.szMaster-1].u.hdr.size4x |= 2;
    mem3.aPool[newi-1].u.hdr.size4x = nBlock*4 + 1;
    mem3.szMaster -= nBlock;
    mem3.aPool[newi-1].u.hdr.prevSize = mem3.szMaster;
    x = mem3.aPool[mem3.iMaster-1].u.hdr.size4x & 2;
    mem3.aPool[mem3.iMaster-1].u.hdr.size4x = mem3.szMaster*4 | x;
    if( mem3.szMaster < mem3.mnMaster ){
      mem3.mnMaster = mem3.szMaster;
    }
    return (void*)&mem3.aPool[newi];
  }
}

static void memsys3Merge(u32 *pRoot){
  u32 iNext, prev, size, i, x;

  assert( sqlite3_mutex_held(mem3.mutex) );
  for(i=*pRoot; i>0; i=iNext){
    iNext = mem3.aPool[i].u.list.next;
    size = mem3.aPool[i-1].u.hdr.size4x;
    assert( (size&1)==0 );
    if( (size&2)==0 ){
      memsys3UnlinkFromList(i, pRoot);
      assert( i > mem3.aPool[i-1].u.hdr.prevSize );
      prev = i - mem3.aPool[i-1].u.hdr.prevSize;
      if( prev==iNext ){
        iNext = mem3.aPool[prev].u.list.next;
      }
      memsys3Unlink(prev);
      size = i + size/4 - prev;
      x = mem3.aPool[prev-1].u.hdr.size4x & 2;
      mem3.aPool[prev-1].u.hdr.size4x = size*4 | x;
      mem3.aPool[prev+size-1].u.hdr.prevSize = size;
      memsys3Link(prev);
      i = prev;
    }else{
      size /= 4;
    }
    if( size>mem3.szMaster ){
      mem3.iMaster = i;
      mem3.szMaster = size;
    }
  }
}

static void *memsys3MallocUnsafe(int nByte){
  u32 i;
  u32 nBlock;
  u32 toFree;

  assert( sqlite3_mutex_held(mem3.mutex) );
  assert( sizeof(Mem3Block)==8 );
  if( nByte<=12 ){
    nBlock = 2;
  }else{
    nBlock = (nByte + 11)/8;
  }
  assert( nBlock>=2 );

  if( nBlock <= MX_SMALL ){
    i = mem3.aiSmall[nBlock-2];
    if( i>0 ){
      memsys3UnlinkFromList(i, &mem3.aiSmall[nBlock-2]);
      return memsys3Checkout(i, nBlock);
    }
  }else{
    int hash = nBlock % N_HASH;
    for(i=mem3.aiHash[hash]; i>0; i=mem3.aPool[i].u.list.next){
      if( mem3.aPool[i-1].u.hdr.size4x/4==nBlock ){
        memsys3UnlinkFromList(i, &mem3.aiHash[hash]);
        return memsys3Checkout(i, nBlock);
      }
    }
  }

  if( mem3.szMaster>=nBlock ){
    return memsys3FromMaster(nBlock);
  }


  for(toFree=nBlock*16; toFree<(mem3.nPool*16); toFree *= 2){
    memsys3OutOfMemory(toFree);
    if( mem3.iMaster ){
      memsys3Link(mem3.iMaster);
      mem3.iMaster = 0;
      mem3.szMaster = 0;
    }
    for(i=0; i<N_HASH; i++){
      memsys3Merge(&mem3.aiHash[i]);
    }
    for(i=0; i<MX_SMALL-1; i++){
      memsys3Merge(&mem3.aiSmall[i]);
    }
    if( mem3.szMaster ){
      memsys3Unlink(mem3.iMaster);
      if( mem3.szMaster>=nBlock ){
        return memsys3FromMaster(nBlock);
      }
    }
  }

  
  return 0;
}

void memsys3FreeUnsafe(void *pOld){
  Mem3Block *p = (Mem3Block*)pOld;
  int i;
  u32 size, x;
  assert( sqlite3_mutex_held(mem3.mutex) );
  assert( p>mem3.aPool && p<&mem3.aPool[mem3.nPool] );
  i = p - mem3.aPool;
  assert( (mem3.aPool[i-1].u.hdr.size4x&1)==1 );
  size = mem3.aPool[i-1].u.hdr.size4x/4;
  assert( i+size<=mem3.nPool+1 );
  mem3.aPool[i-1].u.hdr.size4x &= ~1;
  mem3.aPool[i+size-1].u.hdr.prevSize = size;
  mem3.aPool[i+size-1].u.hdr.size4x &= ~2;
  memsys3Link(i);

  
  if( mem3.iMaster ){
    while( (mem3.aPool[mem3.iMaster-1].u.hdr.size4x&2)==0 ){
      size = mem3.aPool[mem3.iMaster-1].u.hdr.prevSize;
      mem3.iMaster -= size;
      mem3.szMaster += size;
      memsys3Unlink(mem3.iMaster);
      x = mem3.aPool[mem3.iMaster-1].u.hdr.size4x & 2;
      mem3.aPool[mem3.iMaster-1].u.hdr.size4x = mem3.szMaster*4 | x;
      mem3.aPool[mem3.iMaster+mem3.szMaster-1].u.hdr.prevSize = mem3.szMaster;
    }
    x = mem3.aPool[mem3.iMaster-1].u.hdr.size4x & 2;
    while( (mem3.aPool[mem3.iMaster+mem3.szMaster-1].u.hdr.size4x&1)==0 ){
      memsys3Unlink(mem3.iMaster+mem3.szMaster);
      mem3.szMaster += mem3.aPool[mem3.iMaster+mem3.szMaster-1].u.hdr.size4x/4;
      mem3.aPool[mem3.iMaster-1].u.hdr.size4x = mem3.szMaster*4 | x;
      mem3.aPool[mem3.iMaster+mem3.szMaster-1].u.hdr.prevSize = mem3.szMaster;
    }
  }
}

static int memsys3Size(void *p){
  Mem3Block *pBlock;
  if( p==0 ) return 0;
  pBlock = (Mem3Block*)p;
  assert( (pBlock[-1].u.hdr.size4x&1)!=0 );
  return (pBlock[-1].u.hdr.size4x&~3)*2 - 4;
}

static int memsys3Roundup(int n){
  if( n<=12 ){
    return 12;
  }else{
    return ((n+11)&~7) - 4;
  }
}

static void *memsys3Malloc(int nBytes){
  sqlite3_int64 *p;
  assert( nBytes>0 );          
  memsys3Enter();
  p = memsys3MallocUnsafe(nBytes);
  memsys3Leave();
  return (void*)p; 
}

void memsys3Free(void *pPrior){
  assert( pPrior );
  memsys3Enter();
  memsys3FreeUnsafe(pPrior);
  memsys3Leave();
}

void *memsys3Realloc(void *pPrior, int nBytes){
  int nOld;
  void *p;
  if( pPrior==0 ){
    return sqlite3_malloc(nBytes);
  }
  if( nBytes<=0 ){
    sqlite3_free(pPrior);
    return 0;
  }
  nOld = memsys3Size(pPrior);
  if( nBytes<=nOld && nBytes>=nOld-128 ){
    return pPrior;
  }
  memsys3Enter();
  p = memsys3MallocUnsafe(nBytes);
  if( p ){
    if( nOld<nBytes ){
      memcpy(p, pPrior, nOld);
    }else{
      memcpy(p, pPrior, nBytes);
    }
    memsys3FreeUnsafe(pPrior);
  }
  memsys3Leave();
  return p;
}

static int memsys3Init(void *NotUsed){
  UNUSED_PARAMETER(NotUsed);
  if( !sqlite3GlobalConfig.pHeap ){
    return SQLITE_ERROR;
  }

  
  assert( sizeof(Mem3Block)==8 );
  mem3.aPool = (Mem3Block *)sqlite3GlobalConfig.pHeap;
  mem3.nPool = (sqlite3GlobalConfig.nHeap / sizeof(Mem3Block)) - 2;

  
  mem3.szMaster = mem3.nPool;
  mem3.mnMaster = mem3.szMaster;
  mem3.iMaster = 1;
  mem3.aPool[0].u.hdr.size4x = (mem3.szMaster<<2) + 2;
  mem3.aPool[mem3.nPool].u.hdr.prevSize = mem3.nPool;
  mem3.aPool[mem3.nPool].u.hdr.size4x = 1;

  return SQLITE_OK;
}

static void memsys3Shutdown(void *NotUsed){
  UNUSED_PARAMETER(NotUsed);
  mem3.mutex = 0;
  return;
}



void sqlite3Memsys3Dump(const char *zFilename){
#ifdef SQLITE_DEBUG
  FILE *out;
  u32 i, j;
  u32 size;
  if( zFilename==0 || zFilename[0]==0 ){
    out = stdout;
  }else{
    out = fopen(zFilename, "w");
    if( out==0 ){
      fprintf(stderr, "** Unable to output memory debug output log: %s **\n",
                      zFilename);
      return;
    }
  }
  memsys3Enter();
  fprintf(out, "CHUNKS:\n");
  for(i=1; i<=mem3.nPool; i+=size/4){
    size = mem3.aPool[i-1].u.hdr.size4x;
    if( size/4<=1 ){
      fprintf(out, "%p size error\n", &mem3.aPool[i]);
      assert( 0 );
      break;
    }
    if( (size&1)==0 && mem3.aPool[i+size/4-1].u.hdr.prevSize!=size/4 ){
      fprintf(out, "%p tail size does not match\n", &mem3.aPool[i]);
      assert( 0 );
      break;
    }
    if( ((mem3.aPool[i+size/4-1].u.hdr.size4x&2)>>1)!=(size&1) ){
      fprintf(out, "%p tail checkout bit is incorrect\n", &mem3.aPool[i]);
      assert( 0 );
      break;
    }
    if( size&1 ){
      fprintf(out, "%p %6d bytes checked out\n", &mem3.aPool[i], (size/4)*8-8);
    }else{
      fprintf(out, "%p %6d bytes free%s\n", &mem3.aPool[i], (size/4)*8-8,
                  i==mem3.iMaster ? " **master**" : "");
    }
  }
  for(i=0; i<MX_SMALL-1; i++){
    if( mem3.aiSmall[i]==0 ) continue;
    fprintf(out, "small(%2d):", i);
    for(j = mem3.aiSmall[i]; j>0; j=mem3.aPool[j].u.list.next){
      fprintf(out, " %p(%d)", &mem3.aPool[j],
              (mem3.aPool[j-1].u.hdr.size4x/4)*8-8);
    }
    fprintf(out, "\n"); 
  }
  for(i=0; i<N_HASH; i++){
    if( mem3.aiHash[i]==0 ) continue;
    fprintf(out, "hash(%2d):", i);
    for(j = mem3.aiHash[i]; j>0; j=mem3.aPool[j].u.list.next){
      fprintf(out, " %p(%d)", &mem3.aPool[j],
              (mem3.aPool[j-1].u.hdr.size4x/4)*8-8);
    }
    fprintf(out, "\n"); 
  }
  fprintf(out, "master=%d\n", mem3.iMaster);
  fprintf(out, "nowUsed=%d\n", mem3.nPool*8 - mem3.szMaster*8);
  fprintf(out, "mxUsed=%d\n", mem3.nPool*8 - mem3.mnMaster*8);
  sqlite3_mutex_leave(mem3.mutex);
  if( out==stdout ){
    fflush(stdout);
  }else{
    fclose(out);
  }
#else
  UNUSED_PARAMETER(zFilename);
#endif
}

const sqlite3_mem_methods *sqlite3MemGetMemsys3(void){
  static const sqlite3_mem_methods mempoolMethods = {
     memsys3Malloc,
     memsys3Free,
     memsys3Realloc,
     memsys3Size,
     memsys3Roundup,
     memsys3Init,
     memsys3Shutdown,
     0
  };
  return &mempoolMethods;
}

#endif 

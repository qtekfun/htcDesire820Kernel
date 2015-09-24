/*
** 2010 February 1
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
** This file contains the implementation of a write-ahead log (WAL) used in 
** "journal_mode=WAL" mode.
**
** WRITE-AHEAD LOG (WAL) FILE FORMAT
**
** A WAL file consists of a header followed by zero or more "frames".
** Each frame records the revised content of a single page from the
** database file.  All changes to the database are recorded by writing
** frames into the WAL.  Transactions commit when a frame is written that
** contains a commit marker.  A single WAL can and usually does record 
** multiple transactions.  Periodically, the content of the WAL is
** transferred back into the database file in an operation called a
** "checkpoint".
**
** A single WAL file can be used multiple times.  In other words, the
** WAL can fill up with frames and then be checkpointed and then new
** frames can overwrite the old ones.  A WAL always grows from beginning
** toward the end.  Checksums and counters attached to each frame are
** used to determine which frames within the WAL are valid and which
** are leftovers from prior checkpoints.
**
** The WAL header is 32 bytes in size and consists of the following eight
** big-endian 32-bit unsigned integer values:
**
**     0: Magic number.  0x377f0682 or 0x377f0683
**     4: File format version.  Currently 3007000
**     8: Database page size.  Example: 1024
**    12: Checkpoint sequence number
**    16: Salt-1, random integer incremented with each checkpoint
**    20: Salt-2, a different random integer changing with each ckpt
**    24: Checksum-1 (first part of checksum for first 24 bytes of header).
**    28: Checksum-2 (second part of checksum for first 24 bytes of header).
**
** Immediately following the wal-header are zero or more frames. Each
** frame consists of a 24-byte frame-header followed by a <page-size> bytes
** of page data. The frame-header is six big-endian 32-bit unsigned 
** integer values, as follows:
**
**     0: Page number.
**     4: For commit records, the size of the database image in pages 
**        after the commit. For all other records, zero.
**     8: Salt-1 (copied from the header)
**    12: Salt-2 (copied from the header)
**    16: Checksum-1.
**    20: Checksum-2.
**
** A frame is considered valid if and only if the following conditions are
** true:
**
**    (1) The salt-1 and salt-2 values in the frame-header match
**        salt values in the wal-header
**
**    (2) The checksum values in the final 8 bytes of the frame-header
**        exactly match the checksum computed consecutively on the
**        WAL header and the first 8 bytes and the content of all frames
**        up to and including the current frame.
**
** The checksum is computed using 32-bit big-endian integers if the
** magic number in the first 4 bytes of the WAL is 0x377f0683 and it
** is computed using little-endian if the magic number is 0x377f0682.
** The checksum values are always stored in the frame header in a
** big-endian format regardless of which byte order is used to compute
** the checksum.  The checksum is computed by interpreting the input as
** an even number of unsigned 32-bit integers: x[0] through x[N].  The
** algorithm used for the checksum is as follows:
** 
**   for i from 0 to n-1 step 2:
**     s0 += x[i] + s1;
**     s1 += x[i+1] + s0;
**   endfor
**
** Note that s0 and s1 are both weighted checksums using fibonacci weights
** in reverse order (the largest fibonacci weight occurs on the first element
** of the sequence being summed.)  The s1 value spans all 32-bit 
** terms of the sequence whereas s0 omits the final term.
**
** On a checkpoint, the WAL is first VFS.xSync-ed, then valid content of the
** WAL is transferred into the database, then the database is VFS.xSync-ed.
** The VFS.xSync operations serve as write barriers - all writes launched
** before the xSync must complete before any write that launches after the
** xSync begins.
**
** After each checkpoint, the salt-1 value is incremented and the salt-2
** value is randomized.  This prevents old and new frames in the WAL from
** being considered valid at the same time and being checkpointing together
** following a crash.
**
** READER ALGORITHM
**
** To read a page from the database (call it page number P), a reader
** first checks the WAL to see if it contains page P.  If so, then the
** last valid instance of page P that is a followed by a commit frame
** or is a commit frame itself becomes the value read.  If the WAL
** contains no copies of page P that are valid and which are a commit
** frame or are followed by a commit frame, then page P is read from
** the database file.
**
** To start a read transaction, the reader records the index of the last
** valid frame in the WAL.  The reader uses this recorded "mxFrame" value
** for all subsequent read operations.  New transactions can be appended
** to the WAL, but as long as the reader uses its original mxFrame value
** and ignores the newly appended content, it will see a consistent snapshot
** of the database from a single point in time.  This technique allows
** multiple concurrent readers to view different versions of the database
** content simultaneously.
**
** The reader algorithm in the previous paragraphs works correctly, but 
** because frames for page P can appear anywhere within the WAL, the
** reader has to scan the entire WAL looking for page P frames.  If the
** WAL is large (multiple megabytes is typical) that scan can be slow,
** and read performance suffers.  To overcome this problem, a separate
** data structure called the wal-index is maintained to expedite the
** search for frames of a particular page.
** 
** WAL-INDEX FORMAT
**
** Conceptually, the wal-index is shared memory, though VFS implementations
** might choose to implement the wal-index using a mmapped file.  Because
** the wal-index is shared memory, SQLite does not support journal_mode=WAL 
** on a network filesystem.  All users of the database must be able to
** share memory.
**
** The wal-index is transient.  After a crash, the wal-index can (and should
** be) reconstructed from the original WAL file.  In fact, the VFS is required
** to either truncate or zero the header of the wal-index when the last
** connection to it closes.  Because the wal-index is transient, it can
** use an architecture-specific format; it does not have to be cross-platform.
** Hence, unlike the database and WAL file formats which store all values
** as big endian, the wal-index can store multi-byte values in the native
** byte order of the host computer.
**
** The purpose of the wal-index is to answer this question quickly:  Given
** a page number P, return the index of the last frame for page P in the WAL,
** or return NULL if there are no frames for page P in the WAL.
**
** The wal-index consists of a header region, followed by an one or
** more index blocks.  
**
** The wal-index header contains the total number of frames within the WAL
** in the the mxFrame field.  
**
** Each index block except for the first contains information on 
** HASHTABLE_NPAGE frames. The first index block contains information on
** HASHTABLE_NPAGE_ONE frames. The values of HASHTABLE_NPAGE_ONE and 
** HASHTABLE_NPAGE are selected so that together the wal-index header and
** first index block are the same size as all other index blocks in the
** wal-index.
**
** Each index block contains two sections, a page-mapping that contains the
** database page number associated with each wal frame, and a hash-table 
** that allows readers to query an index block for a specific page number.
** The page-mapping is an array of HASHTABLE_NPAGE (or HASHTABLE_NPAGE_ONE
** for the first index block) 32-bit page numbers. The first entry in the 
** first index-block contains the database page number corresponding to the
** first frame in the WAL file. The first entry in the second index block
** in the WAL file corresponds to the (HASHTABLE_NPAGE_ONE+1)th frame in
** the log, and so on.
**
** The last index block in a wal-index usually contains less than the full
** complement of HASHTABLE_NPAGE (or HASHTABLE_NPAGE_ONE) page-numbers,
** depending on the contents of the WAL file. This does not change the
** allocated size of the page-mapping array - the page-mapping array merely
** contains unused entries.
**
** Even without using the hash table, the last frame for page P
** can be found by scanning the page-mapping sections of each index block
** starting with the last index block and moving toward the first, and
** within each index block, starting at the end and moving toward the
** beginning.  The first entry that equals P corresponds to the frame
** holding the content for that page.
**
** The hash table consists of HASHTABLE_NSLOT 16-bit unsigned integers.
** HASHTABLE_NSLOT = 2*HASHTABLE_NPAGE, and there is one entry in the
** hash table for each page number in the mapping section, so the hash 
** table is never more than half full.  The expected number of collisions 
** prior to finding a match is 1.  Each entry of the hash table is an
** 1-based index of an entry in the mapping section of the same
** index block.   Let K be the 1-based index of the largest entry in
** the mapping section.  (For index blocks other than the last, K will
** always be exactly HASHTABLE_NPAGE (4096) and for the last index block
** K will be (mxFrame%HASHTABLE_NPAGE).)  Unused slots of the hash table
** contain a value of 0.
**
** To look for page P in the hash table, first compute a hash iKey on
** P as follows:
**
**      iKey = (P * 383) % HASHTABLE_NSLOT
**
** Then start scanning entries of the hash table, starting with iKey
** (wrapping around to the beginning when the end of the hash table is
** reached) until an unused hash slot is found. Let the first unused slot
** be at index iUnused.  (iUnused might be less than iKey if there was
** wrap-around.) Because the hash table is never more than half full,
** the search is guaranteed to eventually hit an unused entry.  Let 
** iMax be the value between iKey and iUnused, closest to iUnused,
** where aHash[iMax]==P.  If there is no iMax entry (if there exists
** no hash slot such that aHash[i]==p) then page P is not in the
** current index block.  Otherwise the iMax-th mapping entry of the
** current index block corresponds to the last entry that references 
** page P.
**
** A hash search begins with the last index block and moves toward the
** first index block, looking for entries corresponding to page P.  On
** average, only two or three slots in each index block need to be
** examined in order to either find the last entry for page P, or to
** establish that no such entry exists in the block.  Each index block
** holds over 4000 entries.  So two or three index blocks are sufficient
** to cover a typical 10 megabyte WAL file, assuming 1K pages.  8 or 10
** comparisons (on average) suffice to either locate a frame in the
** WAL or to establish that the frame does not exist in the WAL.  This
** is much faster than scanning the entire 10MB WAL.
**
** Note that entries are added in order of increasing K.  Hence, one
** reader might be using some value K0 and a second reader that started
** at a later time (after additional transactions were added to the WAL
** and to the wal-index) might be using a different value K1, where K1>K0.
** Both readers can use the same hash table and mapping section to get
** the correct result.  There may be entries in the hash table with
** K>K0 but to the first reader, those entries will appear to be unused
** slots in the hash table and so the first reader will get an answer as
** if no values greater than K0 had ever been inserted into the hash table
** in the first place - which is what reader one wants.  Meanwhile, the
** second reader using K1 will see additional values that were inserted
** later, which is exactly what reader two wants.  
**
** When a rollback occurs, the value of K is decreased. Hash table entries
** that correspond to frames greater than the new K value are removed
** from the hash table at this point.
*/
#ifndef SQLITE_OMIT_WAL

#include "wal.h"

#if defined(SQLITE_TEST) && defined(SQLITE_DEBUG)
int sqlite3WalTrace = 0;
# define WALTRACE(X)  if(sqlite3WalTrace) sqlite3DebugPrintf X
#else
# define WALTRACE(X)
#endif

#define WAL_MAX_VERSION      3007000
#define WALINDEX_MAX_VERSION 3007000

#define WAL_WRITE_LOCK         0
#define WAL_ALL_BUT_WRITE      1
#define WAL_CKPT_LOCK          1
#define WAL_RECOVER_LOCK       2
#define WAL_READ_LOCK(I)       (3+(I))
#define WAL_NREADER            (SQLITE_SHM_NLOCK-3)


typedef struct WalIndexHdr WalIndexHdr;
typedef struct WalIterator WalIterator;
typedef struct WalCkptInfo WalCkptInfo;


struct WalIndexHdr {
  u32 iVersion;                   
  u32 unused;                     
  u32 iChange;                    
  u8 isInit;                      
  u8 bigEndCksum;                 
  u16 szPage;                     
  u32 mxFrame;                    
  u32 nPage;                      
  u32 aFrameCksum[2];             
  u32 aSalt[2];                   
  u32 aCksum[2];                  
};

/*
** A copy of the following object occurs in the wal-index immediately
** following the second copy of the WalIndexHdr.  This object stores
** information used by checkpoint.
**
** nBackfill is the number of frames in the WAL that have been written
** back into the database. (We call the act of moving content from WAL to
** database "backfilling".)  The nBackfill number is never greater than
** WalIndexHdr.mxFrame.  nBackfill can only be increased by threads
** holding the WAL_CKPT_LOCK lock (which includes a recovery thread).
** However, a WAL_WRITE_LOCK thread can move the value of nBackfill from
** mxFrame back to zero when the WAL is reset.
**
** There is one entry in aReadMark[] for each reader lock.  If a reader
** holds read-lock K, then the value in aReadMark[K] is no greater than
** the mxFrame for that reader.  The value READMARK_NOT_USED (0xffffffff)
** for any aReadMark[] means that entry is unused.  aReadMark[0] is 
** a special case; its value is never used and it exists as a place-holder
** to avoid having to offset aReadMark[] indexs by one.  Readers holding
** WAL_READ_LOCK(0) always ignore the entire WAL and read all content
** directly from the database.
**
** The value of aReadMark[K] may only be changed by a thread that
** is holding an exclusive lock on WAL_READ_LOCK(K).  Thus, the value of
** aReadMark[K] cannot changed while there is a reader is using that mark
** since the reader will be holding a shared lock on WAL_READ_LOCK(K).
**
** The checkpointer may only transfer frames from WAL to database where
** the frame numbers are less than or equal to every aReadMark[] that is
** in use (that is, every aReadMark[j] for which there is a corresponding
** WAL_READ_LOCK(j)).  New readers (usually) pick the aReadMark[] with the
** largest value and will increase an unused aReadMark[] to mxFrame if there
** is not already an aReadMark[] equal to mxFrame.  The exception to the
** previous sentence is when nBackfill equals mxFrame (meaning that everything
** in the WAL has been backfilled into the database) then new readers
** will choose aReadMark[0] which has value 0 and hence such reader will
** get all their all content directly from the database file and ignore 
** the WAL.
**
** Writers normally append new frames to the end of the WAL.  However,
** if nBackfill equals mxFrame (meaning that all WAL content has been
** written back into the database) and if no readers are using the WAL
** (in other words, if there are no WAL_READ_LOCK(i) where i>0) then
** the writer will first "reset" the WAL back to the beginning and start
** writing new content beginning at frame 1.
**
** We assume that 32-bit loads are atomic and so no locks are needed in
** order to read from any aReadMark[] entries.
*/
struct WalCkptInfo {
  u32 nBackfill;                  
  u32 aReadMark[WAL_NREADER];     
};
#define READMARK_NOT_USED  0xffffffff


#define WALINDEX_LOCK_OFFSET   (sizeof(WalIndexHdr)*2 + sizeof(WalCkptInfo))
#define WALINDEX_LOCK_RESERVED 16
#define WALINDEX_HDR_SIZE      (WALINDEX_LOCK_OFFSET+WALINDEX_LOCK_RESERVED)

#define WAL_FRAME_HDRSIZE 24

#define WAL_HDRSIZE 32

#define WAL_MAGIC 0x377f0682

#define walFrameOffset(iFrame, szPage) (                               \
  WAL_HDRSIZE + ((iFrame)-1)*(i64)((szPage)+WAL_FRAME_HDRSIZE)         \
)

struct Wal {
  sqlite3_vfs *pVfs;         
  sqlite3_file *pDbFd;       
  sqlite3_file *pWalFd;      
  u32 iCallback;             
  int nWiData;               
  volatile u32 **apWiData;   
  u32 szPage;                
  i16 readLock;              
  u8 exclusiveMode;          
  u8 writeLock;              
  u8 ckptLock;               
  u8 readOnly;               
  WalIndexHdr hdr;           
  const char *zWalName;      
  u32 nCkpt;                 
#ifdef SQLITE_DEBUG
  u8 lockError;              
#endif
};

#define WAL_NORMAL_MODE     0
#define WAL_EXCLUSIVE_MODE  1     
#define WAL_HEAPMEMORY_MODE 2

typedef u16 ht_slot;

/*
** This structure is used to implement an iterator that loops through
** all frames in the WAL in database page order. Where two or more frames
** correspond to the same database page, the iterator visits only the 
** frame most recently written to the WAL (in other words, the frame with
** the largest index).
**
** The internals of this structure are only accessed by:
**
**   walIteratorInit() - Create a new iterator,
**   walIteratorNext() - Step an iterator,
**   walIteratorFree() - Free an iterator.
**
** This functionality is used by the checkpoint code (see walCheckpoint()).
*/
struct WalIterator {
  int iPrior;                     
  int nSegment;                   
  struct WalSegment {
    int iNext;                    
    ht_slot *aIndex;              
    u32 *aPgno;                   
    int nEntry;                   
    int iZero;                    
  } aSegment[1];                  
};

#define HASHTABLE_NPAGE      4096                 
#define HASHTABLE_HASH_1     383                  
#define HASHTABLE_NSLOT      (HASHTABLE_NPAGE*2)  

#define HASHTABLE_NPAGE_ONE  (HASHTABLE_NPAGE - (WALINDEX_HDR_SIZE/sizeof(u32)))

#define WALINDEX_PGSZ   (                                         \
    sizeof(ht_slot)*HASHTABLE_NSLOT + HASHTABLE_NPAGE*sizeof(u32) \
)

static int walIndexPage(Wal *pWal, int iPage, volatile u32 **ppPage){
  int rc = SQLITE_OK;

  
  if( pWal->nWiData<=iPage ){
    int nByte = sizeof(u32*)*(iPage+1);
    volatile u32 **apNew;
    apNew = (volatile u32 **)sqlite3_realloc((void *)pWal->apWiData, nByte);
    if( !apNew ){
      *ppPage = 0;
      return SQLITE_NOMEM;
    }
    memset((void*)&apNew[pWal->nWiData], 0,
           sizeof(u32*)*(iPage+1-pWal->nWiData));
    pWal->apWiData = apNew;
    pWal->nWiData = iPage+1;
  }

  
  if( pWal->apWiData[iPage]==0 ){
    if( pWal->exclusiveMode==WAL_HEAPMEMORY_MODE ){
      pWal->apWiData[iPage] = (u32 volatile *)sqlite3MallocZero(WALINDEX_PGSZ);
      if( !pWal->apWiData[iPage] ) rc = SQLITE_NOMEM;
    }else{
      rc = sqlite3OsShmMap(pWal->pDbFd, iPage, WALINDEX_PGSZ, 
          pWal->writeLock, (void volatile **)&pWal->apWiData[iPage]
      );
    }
  }

  *ppPage = pWal->apWiData[iPage];
  assert( iPage==0 || *ppPage || rc!=SQLITE_OK );
  return rc;
}

static volatile WalCkptInfo *walCkptInfo(Wal *pWal){
  assert( pWal->nWiData>0 && pWal->apWiData[0] );
  return (volatile WalCkptInfo*)&(pWal->apWiData[0][sizeof(WalIndexHdr)/2]);
}

static volatile WalIndexHdr *walIndexHdr(Wal *pWal){
  assert( pWal->nWiData>0 && pWal->apWiData[0] );
  return (volatile WalIndexHdr*)pWal->apWiData[0];
}

#define BYTESWAP32(x) ( \
    (((x)&0x000000FF)<<24) + (((x)&0x0000FF00)<<8)  \
  + (((x)&0x00FF0000)>>8)  + (((x)&0xFF000000)>>24) \
)

/*
** Generate or extend an 8 byte checksum based on the data in 
** array aByte[] and the initial values of aIn[0] and aIn[1] (or
** initial values of 0 and 0 if aIn==NULL).
**
** The checksum is written back into aOut[] before returning.
**
** nByte must be a positive multiple of 8.
*/
static void walChecksumBytes(
  int nativeCksum, 
  u8 *a,           
  int nByte,       
  const u32 *aIn,  
  u32 *aOut        
){
  u32 s1, s2;
  u32 *aData = (u32 *)a;
  u32 *aEnd = (u32 *)&a[nByte];

  if( aIn ){
    s1 = aIn[0];
    s2 = aIn[1];
  }else{
    s1 = s2 = 0;
  }

  assert( nByte>=8 );
  assert( (nByte&0x00000007)==0 );

  if( nativeCksum ){
    do {
      s1 += *aData++ + s2;
      s2 += *aData++ + s1;
    }while( aData<aEnd );
  }else{
    do {
      s1 += BYTESWAP32(aData[0]) + s2;
      s2 += BYTESWAP32(aData[1]) + s1;
      aData += 2;
    }while( aData<aEnd );
  }

  aOut[0] = s1;
  aOut[1] = s2;
}

static void walShmBarrier(Wal *pWal){
  if( pWal->exclusiveMode!=WAL_HEAPMEMORY_MODE ){
    sqlite3OsShmBarrier(pWal->pDbFd);
  }
}

/*
** Write the header information in pWal->hdr into the wal-index.
**
** The checksum on pWal->hdr is updated before it is written.
*/
static void walIndexWriteHdr(Wal *pWal){
  volatile WalIndexHdr *aHdr = walIndexHdr(pWal);
  const int nCksum = offsetof(WalIndexHdr, aCksum);

  assert( pWal->writeLock );
  pWal->hdr.isInit = 1;
  pWal->hdr.iVersion = WALINDEX_MAX_VERSION;
  walChecksumBytes(1, (u8*)&pWal->hdr, nCksum, 0, pWal->hdr.aCksum);
  memcpy((void *)&aHdr[1], (void *)&pWal->hdr, sizeof(WalIndexHdr));
  walShmBarrier(pWal);
  memcpy((void *)&aHdr[0], (void *)&pWal->hdr, sizeof(WalIndexHdr));
}

static void walEncodeFrame(
  Wal *pWal,                      
  u32 iPage,                      
  u32 nTruncate,                  
  u8 *aData,                      
  u8 *aFrame                      
){
  int nativeCksum;                
  u32 *aCksum = pWal->hdr.aFrameCksum;
  assert( WAL_FRAME_HDRSIZE==24 );
  sqlite3Put4byte(&aFrame[0], iPage);
  sqlite3Put4byte(&aFrame[4], nTruncate);
  memcpy(&aFrame[8], pWal->hdr.aSalt, 8);

  nativeCksum = (pWal->hdr.bigEndCksum==SQLITE_BIGENDIAN);
  walChecksumBytes(nativeCksum, aFrame, 8, aCksum, aCksum);
  walChecksumBytes(nativeCksum, aData, pWal->szPage, aCksum, aCksum);

  sqlite3Put4byte(&aFrame[16], aCksum[0]);
  sqlite3Put4byte(&aFrame[20], aCksum[1]);
}

static int walDecodeFrame(
  Wal *pWal,                      
  u32 *piPage,                    
  u32 *pnTruncate,                
  u8 *aData,                      
  u8 *aFrame                      
){
  int nativeCksum;                
  u32 *aCksum = pWal->hdr.aFrameCksum;
  u32 pgno;                       
  assert( WAL_FRAME_HDRSIZE==24 );

  if( memcmp(&pWal->hdr.aSalt, &aFrame[8], 8)!=0 ){
    return 0;
  }

  pgno = sqlite3Get4byte(&aFrame[0]);
  if( pgno==0 ){
    return 0;
  }

  nativeCksum = (pWal->hdr.bigEndCksum==SQLITE_BIGENDIAN);
  walChecksumBytes(nativeCksum, aFrame, 8, aCksum, aCksum);
  walChecksumBytes(nativeCksum, aData, pWal->szPage, aCksum, aCksum);
  if( aCksum[0]!=sqlite3Get4byte(&aFrame[16]) 
   || aCksum[1]!=sqlite3Get4byte(&aFrame[20]) 
  ){
    
    return 0;
  }

  *piPage = pgno;
  *pnTruncate = sqlite3Get4byte(&aFrame[4]);
  return 1;
}


#if defined(SQLITE_TEST) && defined(SQLITE_DEBUG)
static const char *walLockName(int lockIdx){
  if( lockIdx==WAL_WRITE_LOCK ){
    return "WRITE-LOCK";
  }else if( lockIdx==WAL_CKPT_LOCK ){
    return "CKPT-LOCK";
  }else if( lockIdx==WAL_RECOVER_LOCK ){
    return "RECOVER-LOCK";
  }else{
    static char zName[15];
    sqlite3_snprintf(sizeof(zName), zName, "READ-LOCK[%d]",
                     lockIdx-WAL_READ_LOCK(0));
    return zName;
  }
}
#endif 
    

static int walLockShared(Wal *pWal, int lockIdx){
  int rc;
  if( pWal->exclusiveMode ) return SQLITE_OK;
  rc = sqlite3OsShmLock(pWal->pDbFd, lockIdx, 1,
                        SQLITE_SHM_LOCK | SQLITE_SHM_SHARED);
  WALTRACE(("WAL%p: acquire SHARED-%s %s\n", pWal,
            walLockName(lockIdx), rc ? "failed" : "ok"));
  VVA_ONLY( pWal->lockError = (u8)(rc!=SQLITE_OK && rc!=SQLITE_BUSY); )
  return rc;
}
static void walUnlockShared(Wal *pWal, int lockIdx){
  if( pWal->exclusiveMode ) return;
  (void)sqlite3OsShmLock(pWal->pDbFd, lockIdx, 1,
                         SQLITE_SHM_UNLOCK | SQLITE_SHM_SHARED);
  WALTRACE(("WAL%p: release SHARED-%s\n", pWal, walLockName(lockIdx)));
}
static int walLockExclusive(Wal *pWal, int lockIdx, int n){
  int rc;
  if( pWal->exclusiveMode ) return SQLITE_OK;
  rc = sqlite3OsShmLock(pWal->pDbFd, lockIdx, n,
                        SQLITE_SHM_LOCK | SQLITE_SHM_EXCLUSIVE);
  WALTRACE(("WAL%p: acquire EXCLUSIVE-%s cnt=%d %s\n", pWal,
            walLockName(lockIdx), n, rc ? "failed" : "ok"));
  VVA_ONLY( pWal->lockError = (u8)(rc!=SQLITE_OK && rc!=SQLITE_BUSY); )
  return rc;
}
static void walUnlockExclusive(Wal *pWal, int lockIdx, int n){
  if( pWal->exclusiveMode ) return;
  (void)sqlite3OsShmLock(pWal->pDbFd, lockIdx, n,
                         SQLITE_SHM_UNLOCK | SQLITE_SHM_EXCLUSIVE);
  WALTRACE(("WAL%p: release EXCLUSIVE-%s cnt=%d\n", pWal,
             walLockName(lockIdx), n));
}

static int walHash(u32 iPage){
  assert( iPage>0 );
  assert( (HASHTABLE_NSLOT & (HASHTABLE_NSLOT-1))==0 );
  return (iPage*HASHTABLE_HASH_1) & (HASHTABLE_NSLOT-1);
}
static int walNextHash(int iPriorHash){
  return (iPriorHash+1)&(HASHTABLE_NSLOT-1);
}

static int walHashGet(
  Wal *pWal,                      
  int iHash,                      
  volatile ht_slot **paHash,      
  volatile u32 **paPgno,          
  u32 *piZero                     
){
  int rc;                         
  volatile u32 *aPgno;

  rc = walIndexPage(pWal, iHash, &aPgno);
  assert( rc==SQLITE_OK || iHash>0 );

  if( rc==SQLITE_OK ){
    u32 iZero;
    volatile ht_slot *aHash;

    aHash = (volatile ht_slot *)&aPgno[HASHTABLE_NPAGE];
    if( iHash==0 ){
      aPgno = &aPgno[WALINDEX_HDR_SIZE/sizeof(u32)];
      iZero = 0;
    }else{
      iZero = HASHTABLE_NPAGE_ONE + (iHash-1)*HASHTABLE_NPAGE;
    }
  
    *paPgno = &aPgno[-1];
    *paHash = aHash;
    *piZero = iZero;
  }
  return rc;
}

static int walFramePage(u32 iFrame){
  int iHash = (iFrame+HASHTABLE_NPAGE-HASHTABLE_NPAGE_ONE-1) / HASHTABLE_NPAGE;
  assert( (iHash==0 || iFrame>HASHTABLE_NPAGE_ONE)
       && (iHash>=1 || iFrame<=HASHTABLE_NPAGE_ONE)
       && (iHash<=1 || iFrame>(HASHTABLE_NPAGE_ONE+HASHTABLE_NPAGE))
       && (iHash>=2 || iFrame<=HASHTABLE_NPAGE_ONE+HASHTABLE_NPAGE)
       && (iHash<=2 || iFrame>(HASHTABLE_NPAGE_ONE+2*HASHTABLE_NPAGE))
  );
  return iHash;
}

static u32 walFramePgno(Wal *pWal, u32 iFrame){
  int iHash = walFramePage(iFrame);
  if( iHash==0 ){
    return pWal->apWiData[0][WALINDEX_HDR_SIZE/sizeof(u32) + iFrame - 1];
  }
  return pWal->apWiData[iHash][(iFrame-1-HASHTABLE_NPAGE_ONE)%HASHTABLE_NPAGE];
}

static void walCleanupHash(Wal *pWal){
  volatile ht_slot *aHash = 0;    
  volatile u32 *aPgno = 0;        
  u32 iZero = 0;                  
  int iLimit = 0;                 
  int nByte;                      
  int i;                          

  assert( pWal->writeLock );
  testcase( pWal->hdr.mxFrame==HASHTABLE_NPAGE_ONE-1 );
  testcase( pWal->hdr.mxFrame==HASHTABLE_NPAGE_ONE );
  testcase( pWal->hdr.mxFrame==HASHTABLE_NPAGE_ONE+1 );

  if( pWal->hdr.mxFrame==0 ) return;

  assert( pWal->nWiData>walFramePage(pWal->hdr.mxFrame) );
  assert( pWal->apWiData[walFramePage(pWal->hdr.mxFrame)] );
  walHashGet(pWal, walFramePage(pWal->hdr.mxFrame), &aHash, &aPgno, &iZero);

  iLimit = pWal->hdr.mxFrame - iZero;
  assert( iLimit>0 );
  for(i=0; i<HASHTABLE_NSLOT; i++){
    if( aHash[i]>iLimit ){
      aHash[i] = 0;
    }
  }
  
  nByte = (int)((char *)aHash - (char *)&aPgno[iLimit+1]);
  memset((void *)&aPgno[iLimit+1], 0, nByte);

#ifdef SQLITE_ENABLE_EXPENSIVE_ASSERT
  if( iLimit ){
    int i;           
    int iKey;        
    for(i=1; i<=iLimit; i++){
      for(iKey=walHash(aPgno[i]); aHash[iKey]; iKey=walNextHash(iKey)){
        if( aHash[iKey]==i ) break;
      }
      assert( aHash[iKey]==i );
    }
  }
#endif 
}


static int walIndexAppend(Wal *pWal, u32 iFrame, u32 iPage){
  int rc;                         
  u32 iZero = 0;                  
  volatile u32 *aPgno = 0;        
  volatile ht_slot *aHash = 0;    

  rc = walHashGet(pWal, walFramePage(iFrame), &aHash, &aPgno, &iZero);

  if( rc==SQLITE_OK ){
    int iKey;                     
    int idx;                      
    int nCollide;                 

    idx = iFrame - iZero;
    assert( idx <= HASHTABLE_NSLOT/2 + 1 );
    
    if( idx==1 ){
      int nByte = (int)((u8 *)&aHash[HASHTABLE_NSLOT] - (u8 *)&aPgno[1]);
      memset((void*)&aPgno[1], 0, nByte);
    }

    if( aPgno[idx] ){
      walCleanupHash(pWal);
      assert( !aPgno[idx] );
    }

    
    nCollide = idx;
    for(iKey=walHash(iPage); aHash[iKey]; iKey=walNextHash(iKey)){
      if( (nCollide--)==0 ) return SQLITE_CORRUPT_BKPT;
    }
    aPgno[idx] = iPage;
    aHash[iKey] = (ht_slot)idx;

#ifdef SQLITE_ENABLE_EXPENSIVE_ASSERT
    {
      int i;           
      int nEntry = 0;  
      for(i=0; i<HASHTABLE_NSLOT; i++){ if( aHash[i] ) nEntry++; }
      assert( nEntry==idx );
    }

    if( (idx&0x3ff)==0 ){
      int i;           
      for(i=1; i<=idx; i++){
        for(iKey=walHash(aPgno[i]); aHash[iKey]; iKey=walNextHash(iKey)){
          if( aHash[iKey]==i ) break;
        }
        assert( aHash[iKey]==i );
      }
    }
#endif 
  }


  return rc;
}


static int walIndexRecover(Wal *pWal){
  int rc;                         
  i64 nSize;                      
  u32 aFrameCksum[2] = {0, 0};
  int iLock;                      
  int nLock;                      

  assert( pWal->ckptLock==1 || pWal->ckptLock==0 );
  assert( WAL_ALL_BUT_WRITE==WAL_WRITE_LOCK+1 );
  assert( WAL_CKPT_LOCK==WAL_ALL_BUT_WRITE );
  assert( pWal->writeLock );
  iLock = WAL_ALL_BUT_WRITE + pWal->ckptLock;
  nLock = SQLITE_SHM_NLOCK - iLock;
  rc = walLockExclusive(pWal, iLock, nLock);
  if( rc ){
    return rc;
  }
  WALTRACE(("WAL%p: recovery begin...\n", pWal));

  memset(&pWal->hdr, 0, sizeof(WalIndexHdr));

  rc = sqlite3OsFileSize(pWal->pWalFd, &nSize);
  if( rc!=SQLITE_OK ){
    goto recovery_error;
  }

  if( nSize>WAL_HDRSIZE ){
    u8 aBuf[WAL_HDRSIZE];         
    u8 *aFrame = 0;               
    int szFrame;                  
    u8 *aData;                    
    int iFrame;                   
    i64 iOffset;                  
    int szPage;                   
    u32 magic;                    
    u32 version;                  

    
    rc = sqlite3OsRead(pWal->pWalFd, aBuf, WAL_HDRSIZE, 0);
    if( rc!=SQLITE_OK ){
      goto recovery_error;
    }

    magic = sqlite3Get4byte(&aBuf[0]);
    szPage = sqlite3Get4byte(&aBuf[8]);
    if( (magic&0xFFFFFFFE)!=WAL_MAGIC 
     || szPage&(szPage-1) 
     || szPage>SQLITE_MAX_PAGE_SIZE 
     || szPage<512 
    ){
      goto finished;
    }
    pWal->hdr.bigEndCksum = (u8)(magic&0x00000001);
    pWal->szPage = szPage;
    pWal->nCkpt = sqlite3Get4byte(&aBuf[12]);
    memcpy(&pWal->hdr.aSalt, &aBuf[16], 8);

    
    walChecksumBytes(pWal->hdr.bigEndCksum==SQLITE_BIGENDIAN, 
        aBuf, WAL_HDRSIZE-2*4, 0, pWal->hdr.aFrameCksum
    );
    if( pWal->hdr.aFrameCksum[0]!=sqlite3Get4byte(&aBuf[24])
     || pWal->hdr.aFrameCksum[1]!=sqlite3Get4byte(&aBuf[28])
    ){
      goto finished;
    }

    version = sqlite3Get4byte(&aBuf[4]);
    if( version!=WAL_MAX_VERSION ){
      rc = SQLITE_CANTOPEN_BKPT;
      goto finished;
    }

    
    szFrame = szPage + WAL_FRAME_HDRSIZE;
    aFrame = (u8 *)sqlite3_malloc(szFrame);
    if( !aFrame ){
      rc = SQLITE_NOMEM;
      goto recovery_error;
    }
    aData = &aFrame[WAL_FRAME_HDRSIZE];

    
    iFrame = 0;
    for(iOffset=WAL_HDRSIZE; (iOffset+szFrame)<=nSize; iOffset+=szFrame){
      u32 pgno;                   
      u32 nTruncate;              
      int isValid;                

      
      rc = sqlite3OsRead(pWal->pWalFd, aFrame, szFrame, iOffset);
      if( rc!=SQLITE_OK ) break;
      isValid = walDecodeFrame(pWal, &pgno, &nTruncate, aData, aFrame);
      if( !isValid ) break;
      rc = walIndexAppend(pWal, ++iFrame, pgno);
      if( rc!=SQLITE_OK ) break;

      
      if( nTruncate ){
        pWal->hdr.mxFrame = iFrame;
        pWal->hdr.nPage = nTruncate;
        pWal->hdr.szPage = (u16)((szPage&0xff00) | (szPage>>16));
        testcase( szPage<=32768 );
        testcase( szPage>=65536 );
        aFrameCksum[0] = pWal->hdr.aFrameCksum[0];
        aFrameCksum[1] = pWal->hdr.aFrameCksum[1];
      }
    }

    sqlite3_free(aFrame);
  }

finished:
  if( rc==SQLITE_OK ){
    volatile WalCkptInfo *pInfo;
    int i;
    pWal->hdr.aFrameCksum[0] = aFrameCksum[0];
    pWal->hdr.aFrameCksum[1] = aFrameCksum[1];
    walIndexWriteHdr(pWal);

    pInfo = walCkptInfo(pWal);
    pInfo->nBackfill = 0;
    pInfo->aReadMark[0] = 0;
    for(i=1; i<WAL_NREADER; i++) pInfo->aReadMark[i] = READMARK_NOT_USED;

    if( pWal->hdr.nPage ){
      sqlite3_log(SQLITE_OK, "Recovered %d frames from WAL file %s",
          pWal->hdr.nPage, pWal->zWalName
      );
    }
  }

recovery_error:
  WALTRACE(("WAL%p: recovery %s\n", pWal, rc ? "failed" : "ok"));
  walUnlockExclusive(pWal, iLock, nLock);
  return rc;
}

static void walIndexClose(Wal *pWal, int isDelete){
  if( pWal->exclusiveMode==WAL_HEAPMEMORY_MODE ){
    int i;
    for(i=0; i<pWal->nWiData; i++){
      sqlite3_free((void *)pWal->apWiData[i]);
      pWal->apWiData[i] = 0;
    }
  }else{
    sqlite3OsShmUnmap(pWal->pDbFd, isDelete);
  }
}

int sqlite3WalOpen(
  sqlite3_vfs *pVfs,              
  sqlite3_file *pDbFd,            
  const char *zWalName,           
  int bNoShm,                     
  Wal **ppWal                     
){
  int rc;                         
  Wal *pRet;                      
  int flags;                      

  assert( zWalName && zWalName[0] );
  assert( pDbFd );

#ifdef WIN_SHM_BASE
  assert( WIN_SHM_BASE==WALINDEX_LOCK_OFFSET );
#endif
#ifdef UNIX_SHM_BASE
  assert( UNIX_SHM_BASE==WALINDEX_LOCK_OFFSET );
#endif


  
  *ppWal = 0;
  pRet = (Wal*)sqlite3MallocZero(sizeof(Wal) + pVfs->szOsFile);
  if( !pRet ){
    return SQLITE_NOMEM;
  }

  pRet->pVfs = pVfs;
  pRet->pWalFd = (sqlite3_file *)&pRet[1];
  pRet->pDbFd = pDbFd;
  pRet->readLock = -1;
  pRet->zWalName = zWalName;
  pRet->exclusiveMode = (bNoShm ? WAL_HEAPMEMORY_MODE: WAL_NORMAL_MODE);

  
  flags = (SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE|SQLITE_OPEN_WAL);
  rc = sqlite3OsOpen(pVfs, zWalName, pRet->pWalFd, flags, &flags);
  if( rc==SQLITE_OK && flags&SQLITE_OPEN_READONLY ){
    pRet->readOnly = 1;
  }

  if( rc!=SQLITE_OK ){
    walIndexClose(pRet, 0);
    sqlite3OsClose(pRet->pWalFd);
    sqlite3_free(pRet);
  }else{
    *ppWal = pRet;
    WALTRACE(("WAL%d: opened\n", pRet));
  }
  return rc;
}

/*
** Find the smallest page number out of all pages held in the WAL that
** has not been returned by any prior invocation of this method on the
** same WalIterator object.   Write into *piFrame the frame index where
** that page was last written into the WAL.  Write into *piPage the page
** number.
**
** Return 0 on success.  If there are no pages in the WAL with a page
** number larger than *piPage, then return 1.
*/
static int walIteratorNext(
  WalIterator *p,               
  u32 *piPage,                  
  u32 *piFrame                  
){
  u32 iMin;                     
  u32 iRet = 0xFFFFFFFF;        
  int i;                        

  iMin = p->iPrior;
  assert( iMin<0xffffffff );
  for(i=p->nSegment-1; i>=0; i--){
    struct WalSegment *pSegment = &p->aSegment[i];
    while( pSegment->iNext<pSegment->nEntry ){
      u32 iPg = pSegment->aPgno[pSegment->aIndex[pSegment->iNext]];
      if( iPg>iMin ){
        if( iPg<iRet ){
          iRet = iPg;
          *piFrame = pSegment->iZero + pSegment->aIndex[pSegment->iNext];
        }
        break;
      }
      pSegment->iNext++;
    }
  }

  *piPage = p->iPrior = iRet;
  return (iRet==0xFFFFFFFF);
}

static void walMerge(
  const u32 *aContent,            
  ht_slot *aLeft,                 
  int nLeft,                      
  ht_slot **paRight,              
  int *pnRight,                   
  ht_slot *aTmp                   
){
  int iLeft = 0;                  
  int iRight = 0;                 
  int iOut = 0;                   
  int nRight = *pnRight;
  ht_slot *aRight = *paRight;

  assert( nLeft>0 && nRight>0 );
  while( iRight<nRight || iLeft<nLeft ){
    ht_slot logpage;
    Pgno dbpage;

    if( (iLeft<nLeft) 
     && (iRight>=nRight || aContent[aLeft[iLeft]]<aContent[aRight[iRight]])
    ){
      logpage = aLeft[iLeft++];
    }else{
      logpage = aRight[iRight++];
    }
    dbpage = aContent[logpage];

    aTmp[iOut++] = logpage;
    if( iLeft<nLeft && aContent[aLeft[iLeft]]==dbpage ) iLeft++;

    assert( iLeft>=nLeft || aContent[aLeft[iLeft]]>dbpage );
    assert( iRight>=nRight || aContent[aRight[iRight]]>dbpage );
  }

  *paRight = aLeft;
  *pnRight = iOut;
  memcpy(aLeft, aTmp, sizeof(aTmp[0])*iOut);
}

static void walMergesort(
  const u32 *aContent,            
  ht_slot *aBuffer,               
  ht_slot *aList,                 
  int *pnList                     
){
  struct Sublist {
    int nList;                    
    ht_slot *aList;               
  };

  const int nList = *pnList;      
  int nMerge = 0;                 
  ht_slot *aMerge = 0;            
  int iList;                      
  int iSub = 0;                   
  struct Sublist aSub[13];        

  memset(aSub, 0, sizeof(aSub));
  assert( nList<=HASHTABLE_NPAGE && nList>0 );
  assert( HASHTABLE_NPAGE==(1<<(ArraySize(aSub)-1)) );

  for(iList=0; iList<nList; iList++){
    nMerge = 1;
    aMerge = &aList[iList];
    for(iSub=0; iList & (1<<iSub); iSub++){
      struct Sublist *p = &aSub[iSub];
      assert( p->aList && p->nList<=(1<<iSub) );
      assert( p->aList==&aList[iList&~((2<<iSub)-1)] );
      walMerge(aContent, p->aList, p->nList, &aMerge, &nMerge, aBuffer);
    }
    aSub[iSub].aList = aMerge;
    aSub[iSub].nList = nMerge;
  }

  for(iSub++; iSub<ArraySize(aSub); iSub++){
    if( nList & (1<<iSub) ){
      struct Sublist *p = &aSub[iSub];
      assert( p->nList<=(1<<iSub) );
      assert( p->aList==&aList[nList&~((2<<iSub)-1)] );
      walMerge(aContent, p->aList, p->nList, &aMerge, &nMerge, aBuffer);
    }
  }
  assert( aMerge==aList );
  *pnList = nMerge;

#ifdef SQLITE_DEBUG
  {
    int i;
    for(i=1; i<*pnList; i++){
      assert( aContent[aList[i]] > aContent[aList[i-1]] );
    }
  }
#endif
}

static void walIteratorFree(WalIterator *p){
  sqlite3ScratchFree(p);
}

static int walIteratorInit(Wal *pWal, WalIterator **pp){
  WalIterator *p;                 
  int nSegment;                   
  u32 iLast;                      
  int nByte;                      
  int i;                          
  ht_slot *aTmp;                  
  int rc = SQLITE_OK;             

  assert( pWal->ckptLock && pWal->hdr.mxFrame>0 );
  iLast = pWal->hdr.mxFrame;

  
  nSegment = walFramePage(iLast) + 1;
  nByte = sizeof(WalIterator) 
        + (nSegment-1)*sizeof(struct WalSegment)
        + iLast*sizeof(ht_slot);
  p = (WalIterator *)sqlite3ScratchMalloc(nByte);
  if( !p ){
    return SQLITE_NOMEM;
  }
  memset(p, 0, nByte);
  p->nSegment = nSegment;

  aTmp = (ht_slot *)sqlite3ScratchMalloc(
      sizeof(ht_slot) * (iLast>HASHTABLE_NPAGE?HASHTABLE_NPAGE:iLast)
  );
  if( !aTmp ){
    rc = SQLITE_NOMEM;
  }

  for(i=0; rc==SQLITE_OK && i<nSegment; i++){
    volatile ht_slot *aHash;
    u32 iZero;
    volatile u32 *aPgno;

    rc = walHashGet(pWal, i, &aHash, &aPgno, &iZero);
    if( rc==SQLITE_OK ){
      int j;                      
      int nEntry;                 
      ht_slot *aIndex;            

      aPgno++;
      if( (i+1)==nSegment ){
        nEntry = (int)(iLast - iZero);
      }else{
        nEntry = (int)((u32*)aHash - (u32*)aPgno);
      }
      aIndex = &((ht_slot *)&p->aSegment[p->nSegment])[iZero];
      iZero++;
  
      for(j=0; j<nEntry; j++){
        aIndex[j] = (ht_slot)j;
      }
      walMergesort((u32 *)aPgno, aTmp, aIndex, &nEntry);
      p->aSegment[i].iZero = iZero;
      p->aSegment[i].nEntry = nEntry;
      p->aSegment[i].aIndex = aIndex;
      p->aSegment[i].aPgno = (u32 *)aPgno;
    }
  }
  sqlite3ScratchFree(aTmp);

  if( rc!=SQLITE_OK ){
    walIteratorFree(p);
  }
  *pp = p;
  return rc;
}

static int walBusyLock(
  Wal *pWal,                      
  int (*xBusy)(void*),            
  void *pBusyArg,                 
  int lockIdx,                    
  int n                           
){
  int rc;
  do {
    rc = walLockExclusive(pWal, lockIdx, n);
  }while( xBusy && rc==SQLITE_BUSY && xBusy(pBusyArg) );
  return rc;
}

static int walPagesize(Wal *pWal){
  return (pWal->hdr.szPage&0xfe00) + ((pWal->hdr.szPage&0x0001)<<16);
}

static int walCheckpoint(
  Wal *pWal,                      
  int eMode,                      
  int (*xBusyCall)(void*),        
  void *pBusyArg,                 
  int sync_flags,                 
  u8 *zBuf                        
){
  int rc;                         
  int szPage;                     
  WalIterator *pIter = 0;         
  u32 iDbpage = 0;                
  u32 iFrame = 0;                 
  u32 mxSafeFrame;                
  u32 mxPage;                     
  int i;                          
  volatile WalCkptInfo *pInfo;    
  int (*xBusy)(void*) = 0;        

  szPage = walPagesize(pWal);
  testcase( szPage<=32768 );
  testcase( szPage>=65536 );
  pInfo = walCkptInfo(pWal);
  if( pInfo->nBackfill>=pWal->hdr.mxFrame ) return SQLITE_OK;

  
  rc = walIteratorInit(pWal, &pIter);
  if( rc!=SQLITE_OK ){
    return rc;
  }
  assert( pIter );

  if( eMode!=SQLITE_CHECKPOINT_PASSIVE ) xBusy = xBusyCall;

  mxSafeFrame = pWal->hdr.mxFrame;
  mxPage = pWal->hdr.nPage;
  for(i=1; i<WAL_NREADER; i++){
    u32 y = pInfo->aReadMark[i];
    if( mxSafeFrame>y ){
      assert( y<=pWal->hdr.mxFrame );
      rc = walBusyLock(pWal, xBusy, pBusyArg, WAL_READ_LOCK(i), 1);
      if( rc==SQLITE_OK ){
        pInfo->aReadMark[i] = READMARK_NOT_USED;
        walUnlockExclusive(pWal, WAL_READ_LOCK(i), 1);
      }else if( rc==SQLITE_BUSY ){
        mxSafeFrame = y;
        xBusy = 0;
      }else{
        goto walcheckpoint_out;
      }
    }
  }

  if( pInfo->nBackfill<mxSafeFrame
   && (rc = walBusyLock(pWal, xBusy, pBusyArg, WAL_READ_LOCK(0), 1))==SQLITE_OK
  ){
    i64 nSize;                    
    u32 nBackfill = pInfo->nBackfill;

    
    if( sync_flags ){
      rc = sqlite3OsSync(pWal->pWalFd, sync_flags);
    }

    if( rc==SQLITE_OK ){
      i64 nReq = ((i64)mxPage * szPage);
      rc = sqlite3OsFileSize(pWal->pDbFd, &nSize);
      if( rc==SQLITE_OK && nSize<nReq ){
        sqlite3OsFileControl(pWal->pDbFd, SQLITE_FCNTL_SIZE_HINT, &nReq);
      }
    }

    
    while( rc==SQLITE_OK && 0==walIteratorNext(pIter, &iDbpage, &iFrame) ){
      i64 iOffset;
      assert( walFramePgno(pWal, iFrame)==iDbpage );
      if( iFrame<=nBackfill || iFrame>mxSafeFrame || iDbpage>mxPage ) continue;
      iOffset = walFrameOffset(iFrame, szPage) + WAL_FRAME_HDRSIZE;
      
      rc = sqlite3OsRead(pWal->pWalFd, zBuf, szPage, iOffset);
      if( rc!=SQLITE_OK ) break;
      iOffset = (iDbpage-1)*(i64)szPage;
      testcase( IS_BIG_INT(iOffset) );
      rc = sqlite3OsWrite(pWal->pDbFd, zBuf, szPage, iOffset);
      if( rc!=SQLITE_OK ) break;
    }

    
    if( rc==SQLITE_OK ){
      if( mxSafeFrame==walIndexHdr(pWal)->mxFrame ){
        i64 szDb = pWal->hdr.nPage*(i64)szPage;
        testcase( IS_BIG_INT(szDb) );
        rc = sqlite3OsTruncate(pWal->pDbFd, szDb);
        if( rc==SQLITE_OK && sync_flags ){
          rc = sqlite3OsSync(pWal->pDbFd, sync_flags);
        }
      }
      if( rc==SQLITE_OK ){
        pInfo->nBackfill = mxSafeFrame;
      }
    }

    
    walUnlockExclusive(pWal, WAL_READ_LOCK(0), 1);
  }

  if( rc==SQLITE_BUSY ){
    rc = SQLITE_OK;
  }

  if( rc==SQLITE_OK && eMode!=SQLITE_CHECKPOINT_PASSIVE ){
    assert( pWal->writeLock );
    if( pInfo->nBackfill<pWal->hdr.mxFrame ){
      rc = SQLITE_BUSY;
    }else if( eMode==SQLITE_CHECKPOINT_RESTART ){
      assert( mxSafeFrame==pWal->hdr.mxFrame );
      rc = walBusyLock(pWal, xBusy, pBusyArg, WAL_READ_LOCK(1), WAL_NREADER-1);
      if( rc==SQLITE_OK ){
        walUnlockExclusive(pWal, WAL_READ_LOCK(1), WAL_NREADER-1);
      }
    }
  }

 walcheckpoint_out:
  walIteratorFree(pIter);
  return rc;
}

int sqlite3WalClose(
  Wal *pWal,                      
  int sync_flags,                 
  int nBuf,
  u8 *zBuf                        
){
  int rc = SQLITE_OK;
  if( pWal ){
    int isDelete = 0;             

    rc = sqlite3OsLock(pWal->pDbFd, SQLITE_LOCK_EXCLUSIVE);
    if( rc==SQLITE_OK ){
      if( pWal->exclusiveMode==WAL_NORMAL_MODE ){
        pWal->exclusiveMode = WAL_EXCLUSIVE_MODE;
      }
      rc = sqlite3WalCheckpoint(
          pWal, SQLITE_CHECKPOINT_PASSIVE, 0, 0, sync_flags, nBuf, zBuf, 0, 0
      );
      if( rc==SQLITE_OK ){
        isDelete = 1;
      }
    }

    walIndexClose(pWal, isDelete);
    sqlite3OsClose(pWal->pWalFd);
    if( isDelete ){
      sqlite3OsDelete(pWal->pVfs, pWal->zWalName, 0);
    }
    WALTRACE(("WAL%p: closed\n", pWal));
    sqlite3_free((void *)pWal->apWiData);
    sqlite3_free(pWal);
  }
  return rc;
}

static int walIndexTryHdr(Wal *pWal, int *pChanged){
  u32 aCksum[2];                  
  WalIndexHdr h1, h2;             
  WalIndexHdr volatile *aHdr;     

  
  assert( pWal->nWiData>0 && pWal->apWiData[0] );

  aHdr = walIndexHdr(pWal);
  memcpy(&h1, (void *)&aHdr[0], sizeof(h1));
  walShmBarrier(pWal);
  memcpy(&h2, (void *)&aHdr[1], sizeof(h2));

  if( memcmp(&h1, &h2, sizeof(h1))!=0 ){
    return 1;   
  }  
  if( h1.isInit==0 ){
    return 1;   
  }
  walChecksumBytes(1, (u8*)&h1, sizeof(h1)-sizeof(h1.aCksum), 0, aCksum);
  if( aCksum[0]!=h1.aCksum[0] || aCksum[1]!=h1.aCksum[1] ){
    return 1;   
  }

  if( memcmp(&pWal->hdr, &h1, sizeof(WalIndexHdr)) ){
    *pChanged = 1;
    memcpy(&pWal->hdr, &h1, sizeof(WalIndexHdr));
    pWal->szPage = (pWal->hdr.szPage&0xfe00) + ((pWal->hdr.szPage&0x0001)<<16);
    testcase( pWal->szPage<=32768 );
    testcase( pWal->szPage>=65536 );
  }

  
  return 0;
}

static int walIndexReadHdr(Wal *pWal, int *pChanged){
  int rc;                         
  int badHdr;                     
  volatile u32 *page0;            

  assert( pChanged );
  rc = walIndexPage(pWal, 0, &page0);
  if( rc!=SQLITE_OK ){
    return rc;
  };
  assert( page0 || pWal->writeLock==0 );

  badHdr = (page0 ? walIndexTryHdr(pWal, pChanged) : 1);

  assert( badHdr==0 || pWal->writeLock==0 );
  if( badHdr && SQLITE_OK==(rc = walLockExclusive(pWal, WAL_WRITE_LOCK, 1)) ){
    pWal->writeLock = 1;
    if( SQLITE_OK==(rc = walIndexPage(pWal, 0, &page0)) ){
      badHdr = walIndexTryHdr(pWal, pChanged);
      if( badHdr ){
        rc = walIndexRecover(pWal);
        *pChanged = 1;
      }
    }
    pWal->writeLock = 0;
    walUnlockExclusive(pWal, WAL_WRITE_LOCK, 1);
  }

  if( badHdr==0 && pWal->hdr.iVersion!=WALINDEX_MAX_VERSION ){
    rc = SQLITE_CANTOPEN_BKPT;
  }

  return rc;
}

#define WAL_RETRY  (-1)

static int walTryBeginRead(Wal *pWal, int *pChanged, int useWal, int cnt){
  volatile WalCkptInfo *pInfo;    
  u32 mxReadMark;                 
  int mxI;                        
  int i;                          
  int rc = SQLITE_OK;             

  assert( pWal->readLock<0 );     

  if( cnt>5 ){
    int nDelay = 1;                      
    if( cnt>100 ){
      VVA_ONLY( pWal->lockError = 1; )
      return SQLITE_PROTOCOL;
    }
    if( cnt>=10 ) nDelay = (cnt-9)*238;  
    sqlite3OsSleep(pWal->pVfs, nDelay);
  }

  if( !useWal ){
    rc = walIndexReadHdr(pWal, pChanged);
    if( rc==SQLITE_BUSY ){
      if( pWal->apWiData[0]==0 ){
        rc = WAL_RETRY;
      }else if( SQLITE_OK==(rc = walLockShared(pWal, WAL_RECOVER_LOCK)) ){
        walUnlockShared(pWal, WAL_RECOVER_LOCK);
        rc = WAL_RETRY;
      }else if( rc==SQLITE_BUSY ){
        rc = SQLITE_BUSY_RECOVERY;
      }
    }
    if( rc!=SQLITE_OK ){
      return rc;
    }
  }

  pInfo = walCkptInfo(pWal);
  if( !useWal && pInfo->nBackfill==pWal->hdr.mxFrame ){
    rc = walLockShared(pWal, WAL_READ_LOCK(0));
    walShmBarrier(pWal);
    if( rc==SQLITE_OK ){
      if( memcmp((void *)walIndexHdr(pWal), &pWal->hdr, sizeof(WalIndexHdr)) ){
        /* It is not safe to allow the reader to continue here if frames
        ** may have been appended to the log before READ_LOCK(0) was obtained.
        ** When holding READ_LOCK(0), the reader ignores the entire log file,
        ** which implies that the database file contains a trustworthy
        ** snapshoT. Since holding READ_LOCK(0) prevents a checkpoint from
        ** happening, this is usually correct.
        **
        ** However, if frames have been appended to the log (or if the log 
        ** is wrapped and written for that matter) before the READ_LOCK(0)
        ** is obtained, that is not necessarily true. A checkpointer may
        ** have started to backfill the appended frames but crashed before
        ** it finished. Leaving a corrupt image in the database file.
        */
        walUnlockShared(pWal, WAL_READ_LOCK(0));
        return WAL_RETRY;
      }
      pWal->readLock = 0;
      return SQLITE_OK;
    }else if( rc!=SQLITE_BUSY ){
      return rc;
    }
  }

  mxReadMark = 0;
  mxI = 0;
  for(i=1; i<WAL_NREADER; i++){
    u32 thisMark = pInfo->aReadMark[i];
    if( mxReadMark<=thisMark && thisMark<=pWal->hdr.mxFrame ){
      assert( thisMark!=READMARK_NOT_USED );
      mxReadMark = thisMark;
      mxI = i;
    }
  }
  
  {
    if( mxReadMark < pWal->hdr.mxFrame || mxI==0 ){
      for(i=1; i<WAL_NREADER; i++){
        rc = walLockExclusive(pWal, WAL_READ_LOCK(i), 1);
        if( rc==SQLITE_OK ){
          mxReadMark = pInfo->aReadMark[i] = pWal->hdr.mxFrame;
          mxI = i;
          walUnlockExclusive(pWal, WAL_READ_LOCK(i), 1);
          break;
        }else if( rc!=SQLITE_BUSY ){
          return rc;
        }
      }
    }
    if( mxI==0 ){
      assert( rc==SQLITE_BUSY );
      return WAL_RETRY;
    }

    rc = walLockShared(pWal, WAL_READ_LOCK(mxI));
    if( rc ){
      return rc==SQLITE_BUSY ? WAL_RETRY : rc;
    }
    walShmBarrier(pWal);
    if( pInfo->aReadMark[mxI]!=mxReadMark
     || memcmp((void *)walIndexHdr(pWal), &pWal->hdr, sizeof(WalIndexHdr))
    ){
      walUnlockShared(pWal, WAL_READ_LOCK(mxI));
      return WAL_RETRY;
    }else{
      assert( mxReadMark<=pWal->hdr.mxFrame );
      pWal->readLock = (i16)mxI;
    }
  }
  return rc;
}

int sqlite3WalBeginReadTransaction(Wal *pWal, int *pChanged){
  int rc;                         
  int cnt = 0;                    

  do{
    rc = walTryBeginRead(pWal, pChanged, 0, ++cnt);
  }while( rc==WAL_RETRY );
  testcase( (rc&0xff)==SQLITE_BUSY );
  testcase( (rc&0xff)==SQLITE_IOERR );
  testcase( rc==SQLITE_PROTOCOL );
  testcase( rc==SQLITE_OK );
  return rc;
}

void sqlite3WalEndReadTransaction(Wal *pWal){
  sqlite3WalEndWriteTransaction(pWal);
  if( pWal->readLock>=0 ){
    walUnlockShared(pWal, WAL_READ_LOCK(pWal->readLock));
    pWal->readLock = -1;
  }
}

int sqlite3WalRead(
  Wal *pWal,                      
  Pgno pgno,                      
  int *pInWal,                    
  int nOut,                       
  u8 *pOut                        
){
  u32 iRead = 0;                  
  u32 iLast = pWal->hdr.mxFrame;  
  int iHash;                      

  
  assert( pWal->readLock>=0 || pWal->lockError );

  if( iLast==0 || pWal->readLock==0 ){
    *pInWal = 0;
    return SQLITE_OK;
  }

  /* Search the hash table or tables for an entry matching page number
  ** pgno. Each iteration of the following for() loop searches one
  ** hash table (each hash table indexes up to HASHTABLE_NPAGE frames).
  **
  ** This code might run concurrently to the code in walIndexAppend()
  ** that adds entries to the wal-index (and possibly to this hash 
  ** table). This means the value just read from the hash 
  ** slot (aHash[iKey]) may have been added before or after the 
  ** current read transaction was opened. Values added after the
  ** read transaction was opened may have been written incorrectly -
  ** i.e. these slots may contain garbage data. However, we assume
  ** that any slots written before the current read transaction was
  ** opened remain unmodified.
  **
  ** For the reasons above, the if(...) condition featured in the inner
  ** loop of the following block is more stringent that would be required 
  ** if we had exclusive access to the hash-table:
  **
  **   (aPgno[iFrame]==pgno): 
  **     This condition filters out normal hash-table collisions.
  **
  **   (iFrame<=iLast): 
  **     This condition filters out entries that were added to the hash
  **     table after the current read-transaction had started.
  */
  for(iHash=walFramePage(iLast); iHash>=0 && iRead==0; iHash--){
    volatile ht_slot *aHash;      
    volatile u32 *aPgno;          
    u32 iZero;                    
    int iKey;                     
    int nCollide;                 
    int rc;                       

    rc = walHashGet(pWal, iHash, &aHash, &aPgno, &iZero);
    if( rc!=SQLITE_OK ){
      return rc;
    }
    nCollide = HASHTABLE_NSLOT;
    for(iKey=walHash(pgno); aHash[iKey]; iKey=walNextHash(iKey)){
      u32 iFrame = aHash[iKey] + iZero;
      if( iFrame<=iLast && aPgno[aHash[iKey]]==pgno ){
        assert( iFrame>iRead );
        iRead = iFrame;
      }
      if( (nCollide--)==0 ){
        return SQLITE_CORRUPT_BKPT;
      }
    }
  }

#ifdef SQLITE_ENABLE_EXPENSIVE_ASSERT
  {
    u32 iRead2 = 0;
    u32 iTest;
    for(iTest=iLast; iTest>0; iTest--){
      if( walFramePgno(pWal, iTest)==pgno ){
        iRead2 = iTest;
        break;
      }
    }
    assert( iRead==iRead2 );
  }
#endif

  if( iRead ){
    int sz;
    i64 iOffset;
    sz = pWal->hdr.szPage;
    sz = (pWal->hdr.szPage&0xfe00) + ((pWal->hdr.szPage&0x0001)<<16);
    testcase( sz<=32768 );
    testcase( sz>=65536 );
    iOffset = walFrameOffset(iRead, sz) + WAL_FRAME_HDRSIZE;
    *pInWal = 1;
    
    return sqlite3OsRead(pWal->pWalFd, pOut, nOut, iOffset);
  }

  *pInWal = 0;
  return SQLITE_OK;
}


Pgno sqlite3WalDbsize(Wal *pWal){
  if( pWal && ALWAYS(pWal->readLock>=0) ){
    return pWal->hdr.nPage;
  }
  return 0;
}


/* 
** This function starts a write transaction on the WAL.
**
** A read transaction must have already been started by a prior call
** to sqlite3WalBeginReadTransaction().
**
** If another thread or process has written into the database since
** the read transaction was started, then it is not possible for this
** thread to write as doing so would cause a fork.  So this routine
** returns SQLITE_BUSY in that case and no write transaction is started.
**
** There can only be a single writer active at a time.
*/
int sqlite3WalBeginWriteTransaction(Wal *pWal){
  int rc;

  assert( pWal->readLock>=0 );

  if( pWal->readOnly ){
    return SQLITE_READONLY;
  }

  rc = walLockExclusive(pWal, WAL_WRITE_LOCK, 1);
  if( rc ){
    return rc;
  }
  pWal->writeLock = 1;

  /* If another connection has written to the database file since the
  ** time the read transaction on this connection was started, then
  ** the write is disallowed.
  */
  if( memcmp(&pWal->hdr, (void *)walIndexHdr(pWal), sizeof(WalIndexHdr))!=0 ){
    walUnlockExclusive(pWal, WAL_WRITE_LOCK, 1);
    pWal->writeLock = 0;
    rc = SQLITE_BUSY;
  }

  return rc;
}

int sqlite3WalEndWriteTransaction(Wal *pWal){
  if( pWal->writeLock ){
    walUnlockExclusive(pWal, WAL_WRITE_LOCK, 1);
    pWal->writeLock = 0;
  }
  return SQLITE_OK;
}

/*
** If any data has been written (but not committed) to the log file, this
** function moves the write-pointer back to the start of the transaction.
**
** Additionally, the callback function is invoked for each frame written
** to the WAL since the start of the transaction. If the callback returns
** other than SQLITE_OK, it is not invoked again and the error code is
** returned to the caller.
**
** Otherwise, if the callback function does not return an error, this
** function returns SQLITE_OK.
*/
int sqlite3WalUndo(Wal *pWal, int (*xUndo)(void *, Pgno), void *pUndoCtx){
  int rc = SQLITE_OK;
  if( ALWAYS(pWal->writeLock) ){
    Pgno iMax = pWal->hdr.mxFrame;
    Pgno iFrame;
  
    memcpy(&pWal->hdr, (void *)walIndexHdr(pWal), sizeof(WalIndexHdr));

    for(iFrame=pWal->hdr.mxFrame+1; 
        ALWAYS(rc==SQLITE_OK) && iFrame<=iMax; 
        iFrame++
    ){
      /* This call cannot fail. Unless the page for which the page number
      ** is passed as the second argument is (a) in the cache and 
      ** (b) has an outstanding reference, then xUndo is either a no-op
      ** (if (a) is false) or simply expels the page from the cache (if (b)
      ** is false).
      **
      ** If the upper layer is doing a rollback, it is guaranteed that there
      ** are no outstanding references to any page other than page 1. And
      ** page 1 is never written to the log until the transaction is
      ** committed. As a result, the call to xUndo may not fail.
      */
      assert( walFramePgno(pWal, iFrame)!=1 );
      rc = xUndo(pUndoCtx, walFramePgno(pWal, iFrame));
    }
    walCleanupHash(pWal);
  }
  assert( rc==SQLITE_OK );
  return rc;
}

void sqlite3WalSavepoint(Wal *pWal, u32 *aWalData){
  assert( pWal->writeLock );
  aWalData[0] = pWal->hdr.mxFrame;
  aWalData[1] = pWal->hdr.aFrameCksum[0];
  aWalData[2] = pWal->hdr.aFrameCksum[1];
  aWalData[3] = pWal->nCkpt;
}

int sqlite3WalSavepointUndo(Wal *pWal, u32 *aWalData){
  int rc = SQLITE_OK;

  assert( pWal->writeLock );
  assert( aWalData[3]!=pWal->nCkpt || aWalData[0]<=pWal->hdr.mxFrame );

  if( aWalData[3]!=pWal->nCkpt ){
    aWalData[0] = 0;
    aWalData[3] = pWal->nCkpt;
  }

  if( aWalData[0]<pWal->hdr.mxFrame ){
    pWal->hdr.mxFrame = aWalData[0];
    pWal->hdr.aFrameCksum[0] = aWalData[1];
    pWal->hdr.aFrameCksum[1] = aWalData[2];
    walCleanupHash(pWal);
  }

  return rc;
}

static int walRestartLog(Wal *pWal){
  int rc = SQLITE_OK;
  int cnt;

  if( pWal->readLock==0 ){
    volatile WalCkptInfo *pInfo = walCkptInfo(pWal);
    assert( pInfo->nBackfill==pWal->hdr.mxFrame );
    if( pInfo->nBackfill>0 ){
      u32 salt1;
      sqlite3_randomness(4, &salt1);
      rc = walLockExclusive(pWal, WAL_READ_LOCK(1), WAL_NREADER-1);
      if( rc==SQLITE_OK ){
        int i;                    
        u32 *aSalt = pWal->hdr.aSalt;       
        pWal->nCkpt++;
        pWal->hdr.mxFrame = 0;
        sqlite3Put4byte((u8*)&aSalt[0], 1 + sqlite3Get4byte((u8*)&aSalt[0]));
        aSalt[1] = salt1;
        walIndexWriteHdr(pWal);
        pInfo->nBackfill = 0;
        for(i=1; i<WAL_NREADER; i++) pInfo->aReadMark[i] = READMARK_NOT_USED;
        assert( pInfo->aReadMark[0]==0 );
        walUnlockExclusive(pWal, WAL_READ_LOCK(1), WAL_NREADER-1);
      }else if( rc!=SQLITE_BUSY ){
        return rc;
      }
    }
    walUnlockShared(pWal, WAL_READ_LOCK(0));
    pWal->readLock = -1;
    cnt = 0;
    do{
      int notUsed;
      rc = walTryBeginRead(pWal, &notUsed, 1, ++cnt);
    }while( rc==WAL_RETRY );
    assert( (rc&0xff)!=SQLITE_BUSY ); 
    testcase( (rc&0xff)==SQLITE_IOERR );
    testcase( rc==SQLITE_PROTOCOL );
    testcase( rc==SQLITE_OK );
  }
  return rc;
}

int sqlite3WalFrames(
  Wal *pWal,                      
  int szPage,                     
  PgHdr *pList,                   
  Pgno nTruncate,                 
  int isCommit,                   
  int sync_flags                  
){
  int rc;                         
  u32 iFrame;                     
  u8 aFrame[WAL_FRAME_HDRSIZE];   
  PgHdr *p;                       
  PgHdr *pLast = 0;               
  int nLast = 0;                  

  assert( pList );
  assert( pWal->writeLock );

#if defined(SQLITE_TEST) && defined(SQLITE_DEBUG)
  { int cnt; for(cnt=0, p=pList; p; p=p->pDirty, cnt++){}
    WALTRACE(("WAL%p: frame write begin. %d frames. mxFrame=%d. %s\n",
              pWal, cnt, pWal->hdr.mxFrame, isCommit ? "Commit" : "Spill"));
  }
#endif

  if( SQLITE_OK!=(rc = walRestartLog(pWal)) ){
    return rc;
  }

  /* If this is the first frame written into the log, write the WAL
  ** header to the start of the WAL file. See comments at the top of
  ** this source file for a description of the WAL header format.
  */
  iFrame = pWal->hdr.mxFrame;
  if( iFrame==0 ){
    u8 aWalHdr[WAL_HDRSIZE];      
    u32 aCksum[2];                

    sqlite3Put4byte(&aWalHdr[0], (WAL_MAGIC | SQLITE_BIGENDIAN));
    sqlite3Put4byte(&aWalHdr[4], WAL_MAX_VERSION);
    sqlite3Put4byte(&aWalHdr[8], szPage);
    sqlite3Put4byte(&aWalHdr[12], pWal->nCkpt);
    sqlite3_randomness(8, pWal->hdr.aSalt);
    memcpy(&aWalHdr[16], pWal->hdr.aSalt, 8);
    walChecksumBytes(1, aWalHdr, WAL_HDRSIZE-2*4, 0, aCksum);
    sqlite3Put4byte(&aWalHdr[24], aCksum[0]);
    sqlite3Put4byte(&aWalHdr[28], aCksum[1]);
    
    pWal->szPage = szPage;
    pWal->hdr.bigEndCksum = SQLITE_BIGENDIAN;
    pWal->hdr.aFrameCksum[0] = aCksum[0];
    pWal->hdr.aFrameCksum[1] = aCksum[1];

    rc = sqlite3OsWrite(pWal->pWalFd, aWalHdr, sizeof(aWalHdr), 0);
    WALTRACE(("WAL%p: wal-header write %s\n", pWal, rc ? "failed" : "ok"));
    if( rc!=SQLITE_OK ){
      return rc;
    }
  }
  assert( (int)pWal->szPage==szPage );

  
  for(p=pList; p; p=p->pDirty){
    u32 nDbsize;                  
    i64 iOffset;                  
    void *pData;
   
    iOffset = walFrameOffset(++iFrame, szPage);
    
    
    
    nDbsize = (isCommit && p->pDirty==0) ? nTruncate : 0;
#if defined(SQLITE_HAS_CODEC)
    if( (pData = sqlite3PagerCodec(p))==0 ) return SQLITE_NOMEM;
#else
    pData = p->pData;
#endif
    walEncodeFrame(pWal, p->pgno, nDbsize, pData, aFrame);
    rc = sqlite3OsWrite(pWal->pWalFd, aFrame, sizeof(aFrame), iOffset);
    if( rc!=SQLITE_OK ){
      return rc;
    }

    
    rc = sqlite3OsWrite(pWal->pWalFd, pData, szPage, iOffset+sizeof(aFrame));
    if( rc!=SQLITE_OK ){
      return rc;
    }
    pLast = p;
  }

  
  if( sync_flags ){
    i64 iSegment = sqlite3OsSectorSize(pWal->pWalFd);
    i64 iOffset = walFrameOffset(iFrame+1, szPage);

    assert( isCommit );
    assert( iSegment>0 );

    iSegment = (((iOffset+iSegment-1)/iSegment) * iSegment);
    while( iOffset<iSegment ){
      void *pData;
#if defined(SQLITE_HAS_CODEC)
      if( (pData = sqlite3PagerCodec(pLast))==0 ) return SQLITE_NOMEM;
#else
      pData = pLast->pData;
#endif
      walEncodeFrame(pWal, pLast->pgno, nTruncate, pData, aFrame);
      
      rc = sqlite3OsWrite(pWal->pWalFd, aFrame, sizeof(aFrame), iOffset);
      if( rc!=SQLITE_OK ){
        return rc;
      }
      iOffset += WAL_FRAME_HDRSIZE;
      rc = sqlite3OsWrite(pWal->pWalFd, pData, szPage, iOffset); 
      if( rc!=SQLITE_OK ){
        return rc;
      }
      nLast++;
      iOffset += szPage;
    }

    rc = sqlite3OsSync(pWal->pWalFd, sync_flags);
  }

  /* Append data to the wal-index. It is not necessary to lock the 
  ** wal-index to do this as the SQLITE_SHM_WRITE lock held on the wal-index
  ** guarantees that there are no other writers, and no data that may
  ** be in use by existing readers is being overwritten.
  */
  iFrame = pWal->hdr.mxFrame;
  for(p=pList; p && rc==SQLITE_OK; p=p->pDirty){
    iFrame++;
    rc = walIndexAppend(pWal, iFrame, p->pgno);
  }
  while( nLast>0 && rc==SQLITE_OK ){
    iFrame++;
    nLast--;
    rc = walIndexAppend(pWal, iFrame, pLast->pgno);
  }

  if( rc==SQLITE_OK ){
    
    pWal->hdr.szPage = (u16)((szPage&0xff00) | (szPage>>16));
    testcase( szPage<=32768 );
    testcase( szPage>=65536 );
    pWal->hdr.mxFrame = iFrame;
    if( isCommit ){
      pWal->hdr.iChange++;
      pWal->hdr.nPage = nTruncate;
    }
    
    if( isCommit ){
      walIndexWriteHdr(pWal);
      pWal->iCallback = iFrame;
    }
  }

  WALTRACE(("WAL%p: frame write %s\n", pWal, rc ? "failed" : "ok"));
  return rc;
}

int sqlite3WalCheckpoint(
  Wal *pWal,                      
  int eMode,                      
  int (*xBusy)(void*),            
  void *pBusyArg,                 
  int sync_flags,                 
  int nBuf,                       
  u8 *zBuf,                       
  int *pnLog,                     
  int *pnCkpt                     
){
  int rc;                         
  int isChanged = 0;              
  int eMode2 = eMode;             

  assert( pWal->ckptLock==0 );
  assert( pWal->writeLock==0 );

  WALTRACE(("WAL%p: checkpoint begins\n", pWal));
  rc = walLockExclusive(pWal, WAL_CKPT_LOCK, 1);
  if( rc ){
    return rc;
  }
  pWal->ckptLock = 1;

  if( eMode!=SQLITE_CHECKPOINT_PASSIVE ){
    rc = walBusyLock(pWal, xBusy, pBusyArg, WAL_WRITE_LOCK, 1);
    if( rc==SQLITE_OK ){
      pWal->writeLock = 1;
    }else if( rc==SQLITE_BUSY ){
      eMode2 = SQLITE_CHECKPOINT_PASSIVE;
      rc = SQLITE_OK;
    }
  }

  
  if( rc==SQLITE_OK ){
    rc = walIndexReadHdr(pWal, &isChanged);
  }

  
  if( rc==SQLITE_OK ){
    if( pWal->hdr.mxFrame && walPagesize(pWal)!=nBuf ){
      rc = SQLITE_CORRUPT_BKPT;
    }else{
      rc = walCheckpoint(pWal, eMode2, xBusy, pBusyArg, sync_flags, zBuf);
    }

    
    if( rc==SQLITE_OK || rc==SQLITE_BUSY ){
      if( pnLog ) *pnLog = (int)pWal->hdr.mxFrame;
      if( pnCkpt ) *pnCkpt = (int)(walCkptInfo(pWal)->nBackfill);
    }
  }

  if( isChanged ){
    memset(&pWal->hdr, 0, sizeof(WalIndexHdr));
  }

  
  sqlite3WalEndWriteTransaction(pWal);
  walUnlockExclusive(pWal, WAL_CKPT_LOCK, 1);
  pWal->ckptLock = 0;
  WALTRACE(("WAL%p: checkpoint %s\n", pWal, rc ? "failed" : "ok"));
  return (rc==SQLITE_OK && eMode!=eMode2 ? SQLITE_BUSY : rc);
}

int sqlite3WalCallback(Wal *pWal){
  u32 ret = 0;
  if( pWal ){
    ret = pWal->iCallback;
    pWal->iCallback = 0;
  }
  return (int)ret;
}

int sqlite3WalExclusiveMode(Wal *pWal, int op){
  int rc;
  assert( pWal->writeLock==0 );
  assert( pWal->exclusiveMode!=WAL_HEAPMEMORY_MODE || op==-1 );

  assert( pWal->readLock>=0 || pWal->lockError );
  assert( pWal->readLock>=0 || (op<=0 && pWal->exclusiveMode==0) );

  if( op==0 ){
    if( pWal->exclusiveMode ){
      pWal->exclusiveMode = 0;
      if( walLockShared(pWal, WAL_READ_LOCK(pWal->readLock))!=SQLITE_OK ){
        pWal->exclusiveMode = 1;
      }
      rc = pWal->exclusiveMode==0;
    }else{
      
      rc = 0;
    }
  }else if( op>0 ){
    assert( pWal->exclusiveMode==0 );
    assert( pWal->readLock>=0 );
    walUnlockShared(pWal, WAL_READ_LOCK(pWal->readLock));
    pWal->exclusiveMode = 1;
    rc = 1;
  }else{
    rc = pWal->exclusiveMode==0;
  }
  return rc;
}

int sqlite3WalHeapMemory(Wal *pWal){
  return (pWal && pWal->exclusiveMode==WAL_HEAPMEMORY_MODE );
}

#endif 

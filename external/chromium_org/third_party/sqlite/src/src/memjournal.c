/*
** 2008 October 7
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
** This file contains code use to implement an in-memory rollback journal.
** The in-memory rollback journal is used to journal transactions for
** ":memory:" databases and when the journal_mode=MEMORY pragma is used.
*/
#include "sqliteInt.h"

typedef struct MemJournal MemJournal;
typedef struct FilePoint FilePoint;
typedef struct FileChunk FileChunk;

#define JOURNAL_CHUNKSIZE ((int)(1024-sizeof(FileChunk*)))

#ifndef MIN
# define MIN(x,y) ((x)<(y)?(x):(y))
#endif

struct FileChunk {
  FileChunk *pNext;               
  u8 zChunk[JOURNAL_CHUNKSIZE];   
};

struct FilePoint {
  sqlite3_int64 iOffset;          
  FileChunk *pChunk;              
};

struct MemJournal {
  sqlite3_io_methods *pMethod;    
  FileChunk *pFirst;              
  FilePoint endpoint;             
  FilePoint readpoint;            
};

static int memjrnlRead(
  sqlite3_file *pJfd,    
  void *zBuf,            
  int iAmt,              
  sqlite_int64 iOfst     
){
  MemJournal *p = (MemJournal *)pJfd;
  u8 *zOut = zBuf;
  int nRead = iAmt;
  int iChunkOffset;
  FileChunk *pChunk;

  
  assert( iOfst+iAmt<=p->endpoint.iOffset );

  if( p->readpoint.iOffset!=iOfst || iOfst==0 ){
    sqlite3_int64 iOff = 0;
    for(pChunk=p->pFirst; 
        ALWAYS(pChunk) && (iOff+JOURNAL_CHUNKSIZE)<=iOfst;
        pChunk=pChunk->pNext
    ){
      iOff += JOURNAL_CHUNKSIZE;
    }
  }else{
    pChunk = p->readpoint.pChunk;
  }

  iChunkOffset = (int)(iOfst%JOURNAL_CHUNKSIZE);
  do {
    int iSpace = JOURNAL_CHUNKSIZE - iChunkOffset;
    int nCopy = MIN(nRead, (JOURNAL_CHUNKSIZE - iChunkOffset));
    memcpy(zOut, &pChunk->zChunk[iChunkOffset], nCopy);
    zOut += nCopy;
    nRead -= iSpace;
    iChunkOffset = 0;
  } while( nRead>=0 && (pChunk=pChunk->pNext)!=0 && nRead>0 );
  p->readpoint.iOffset = iOfst+iAmt;
  p->readpoint.pChunk = pChunk;

  return SQLITE_OK;
}

static int memjrnlWrite(
  sqlite3_file *pJfd,    
  const void *zBuf,      /* Take data to be written from here */
  int iAmt,              
  sqlite_int64 iOfst     
){
  MemJournal *p = (MemJournal *)pJfd;
  int nWrite = iAmt;
  u8 *zWrite = (u8 *)zBuf;

  assert( iOfst==p->endpoint.iOffset );
  UNUSED_PARAMETER(iOfst);

  while( nWrite>0 ){
    FileChunk *pChunk = p->endpoint.pChunk;
    int iChunkOffset = (int)(p->endpoint.iOffset%JOURNAL_CHUNKSIZE);
    int iSpace = MIN(nWrite, JOURNAL_CHUNKSIZE - iChunkOffset);

    if( iChunkOffset==0 ){
      
      FileChunk *pNew = sqlite3_malloc(sizeof(FileChunk));
      if( !pNew ){
        return SQLITE_IOERR_NOMEM;
      }
      pNew->pNext = 0;
      if( pChunk ){
        assert( p->pFirst );
        pChunk->pNext = pNew;
      }else{
        assert( !p->pFirst );
        p->pFirst = pNew;
      }
      p->endpoint.pChunk = pNew;
    }

    memcpy(&p->endpoint.pChunk->zChunk[iChunkOffset], zWrite, iSpace);
    zWrite += iSpace;
    nWrite -= iSpace;
    p->endpoint.iOffset += iSpace;
  }

  return SQLITE_OK;
}

static int memjrnlTruncate(sqlite3_file *pJfd, sqlite_int64 size){
  MemJournal *p = (MemJournal *)pJfd;
  FileChunk *pChunk;
  assert(size==0);
  UNUSED_PARAMETER(size);
  pChunk = p->pFirst;
  while( pChunk ){
    FileChunk *pTmp = pChunk;
    pChunk = pChunk->pNext;
    sqlite3_free(pTmp);
  }
  sqlite3MemJournalOpen(pJfd);
  return SQLITE_OK;
}

static int memjrnlClose(sqlite3_file *pJfd){
  memjrnlTruncate(pJfd, 0);
  return SQLITE_OK;
}


static int memjrnlSync(sqlite3_file *NotUsed, int NotUsed2){
  UNUSED_PARAMETER2(NotUsed, NotUsed2);
  return SQLITE_OK;
}

static int memjrnlFileSize(sqlite3_file *pJfd, sqlite_int64 *pSize){
  MemJournal *p = (MemJournal *)pJfd;
  *pSize = (sqlite_int64) p->endpoint.iOffset;
  return SQLITE_OK;
}

static const struct sqlite3_io_methods MemJournalMethods = {
  1,                
  memjrnlClose,     
  memjrnlRead,      
  memjrnlWrite,     
  memjrnlTruncate,  
  memjrnlSync,      
  memjrnlFileSize,  
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

void sqlite3MemJournalOpen(sqlite3_file *pJfd){
  MemJournal *p = (MemJournal *)pJfd;
  assert( EIGHT_BYTE_ALIGNMENT(p) );
  memset(p, 0, sqlite3MemJournalSize());
  p->pMethod = (sqlite3_io_methods*)&MemJournalMethods;
}

int sqlite3IsMemJournal(sqlite3_file *pJfd){
  return pJfd->pMethods==&MemJournalMethods;
}

int sqlite3MemJournalSize(void){
  return sizeof(MemJournal);
}

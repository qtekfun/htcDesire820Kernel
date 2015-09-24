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
** This header file defines the interface to the write-ahead logging 
** system. Refer to the comments below and the header comment attached to 
** the implementation of each function in log.c for further details.
*/

#ifndef _WAL_H_
#define _WAL_H_

#include "sqliteInt.h"

#ifdef SQLITE_OMIT_WAL
# define sqlite3WalOpen(x,y,z)                   0
# define sqlite3WalClose(w,x,y,z)                0
# define sqlite3WalBeginReadTransaction(y,z)     0
# define sqlite3WalEndReadTransaction(z)
# define sqlite3WalRead(v,w,x,y,z)               0
# define sqlite3WalDbsize(y)                     0
# define sqlite3WalBeginWriteTransaction(y)      0
# define sqlite3WalEndWriteTransaction(x)        0
# define sqlite3WalUndo(x,y,z)                   0
# define sqlite3WalSavepoint(y,z)
# define sqlite3WalSavepointUndo(y,z)            0
# define sqlite3WalFrames(u,v,w,x,y,z)           0
# define sqlite3WalCheckpoint(r,s,t,u,v,w,x,y,z) 0
# define sqlite3WalCallback(z)                   0
# define sqlite3WalExclusiveMode(y,z)            0
# define sqlite3WalHeapMemory(z)                 0
#else

#define WAL_SAVEPOINT_NDATA 4

typedef struct Wal Wal;

int sqlite3WalOpen(sqlite3_vfs*, sqlite3_file*, const char *zName, int, Wal**);
int sqlite3WalClose(Wal *pWal, int sync_flags, int, u8 *);

int sqlite3WalBeginReadTransaction(Wal *pWal, int *);
void sqlite3WalEndReadTransaction(Wal *pWal);

int sqlite3WalRead(Wal *pWal, Pgno pgno, int *pInWal, int nOut, u8 *pOut);

Pgno sqlite3WalDbsize(Wal *pWal);

int sqlite3WalBeginWriteTransaction(Wal *pWal);
int sqlite3WalEndWriteTransaction(Wal *pWal);

/* Undo any frames written (but not committed) to the log */
int sqlite3WalUndo(Wal *pWal, int (*xUndo)(void *, Pgno), void *pUndoCtx);

void sqlite3WalSavepoint(Wal *pWal, u32 *aWalData);

int sqlite3WalSavepointUndo(Wal *pWal, u32 *aWalData);

int sqlite3WalFrames(Wal *pWal, int, PgHdr *, Pgno, int, int);

 
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
);

int sqlite3WalCallback(Wal *pWal);

int sqlite3WalExclusiveMode(Wal *pWal, int op);

int sqlite3WalHeapMemory(Wal *pWal);

#endif 
#endif 

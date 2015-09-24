/*
** 2001 September 15
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
** subsystem.  The page cache subsystem reads and writes a file a page
** at a time and provides a journal for rollback.
*/

#ifndef _PAGER_H_
#define _PAGER_H_

#ifndef SQLITE_DEFAULT_JOURNAL_SIZE_LIMIT
  #define SQLITE_DEFAULT_JOURNAL_SIZE_LIMIT -1
#endif

typedef u32 Pgno;

typedef struct Pager Pager;

typedef struct PgHdr DbPage;

#define PAGER_MJ_PGNO(x) ((Pgno)((PENDING_BYTE/((x)->pageSize))+1))

#define PAGER_OMIT_JOURNAL  0x0001    
#define PAGER_NO_READLOCK   0x0002    
#define PAGER_MEMORY        0x0004    

#define PAGER_LOCKINGMODE_QUERY      -1
#define PAGER_LOCKINGMODE_NORMAL      0
#define PAGER_LOCKINGMODE_EXCLUSIVE   1

#define PAGER_JOURNALMODE_QUERY     (-1)  
#define PAGER_JOURNALMODE_DELETE      0   
#define PAGER_JOURNALMODE_PERSIST     1   
#define PAGER_JOURNALMODE_OFF         2   
#define PAGER_JOURNALMODE_TRUNCATE    3   
#define PAGER_JOURNALMODE_MEMORY      4   
#define PAGER_JOURNALMODE_WAL         5   


 
int sqlite3PagerOpen(
  sqlite3_vfs*,
  Pager **ppPager,
  const char*,
  int,
  int,
  int,
  void(*)(DbPage*)
);
int sqlite3PagerClose(Pager *pPager);
int sqlite3PagerReadFileheader(Pager*, int, unsigned char*);

void sqlite3PagerSetBusyhandler(Pager*, int(*)(void *), void *);
int sqlite3PagerSetPagesize(Pager*, u32*, int);
int sqlite3PagerMaxPageCount(Pager*, int);
void sqlite3PagerSetCachesize(Pager*, int);
void sqlite3PagerSetSafetyLevel(Pager*,int,int,int);
int sqlite3PagerLockingMode(Pager *, int);
int sqlite3PagerSetJournalMode(Pager *, int);
int sqlite3PagerGetJournalMode(Pager*);
int sqlite3PagerOkToChangeJournalMode(Pager*);
i64 sqlite3PagerJournalSizeLimit(Pager *, i64);
sqlite3_backup **sqlite3PagerBackupPtr(Pager*);

 
int sqlite3PagerAcquire(Pager *pPager, Pgno pgno, DbPage **ppPage, int clrFlag);
#define sqlite3PagerGet(A,B,C) sqlite3PagerAcquire(A,B,C,0)
DbPage *sqlite3PagerLookup(Pager *pPager, Pgno pgno);
void sqlite3PagerRef(DbPage*);
void sqlite3PagerUnref(DbPage*);

int sqlite3PagerWrite(DbPage*);
void sqlite3PagerDontWrite(DbPage*);
int sqlite3PagerMovepage(Pager*,DbPage*,Pgno,int);
int sqlite3PagerPageRefcount(DbPage*);
void *sqlite3PagerGetData(DbPage *); 
void *sqlite3PagerGetExtra(DbPage *); 

void sqlite3PagerPagecount(Pager*, int*);
int sqlite3PagerBegin(Pager*, int exFlag, int);
int sqlite3PagerCommitPhaseOne(Pager*,const char *zMaster, int);
int sqlite3PagerExclusiveLock(Pager*);
int sqlite3PagerSync(Pager *pPager);
int sqlite3PagerCommitPhaseTwo(Pager*);
int sqlite3PagerRollback(Pager*);
int sqlite3PagerOpenSavepoint(Pager *pPager, int n);
int sqlite3PagerSavepoint(Pager *pPager, int op, int iSavepoint);
int sqlite3PagerSharedLock(Pager *pPager);

int sqlite3PagerCheckpoint(Pager *pPager, int, int*, int*);
int sqlite3PagerWalSupported(Pager *pPager);
int sqlite3PagerWalCallback(Pager *pPager);
int sqlite3PagerOpenWal(Pager *pPager, int *pisOpen);
int sqlite3PagerCloseWal(Pager *pPager);

u8 sqlite3PagerIsreadonly(Pager*);
int sqlite3PagerRefcount(Pager*);
int sqlite3PagerMemUsed(Pager*);
const char *sqlite3PagerFilename(Pager*);
const sqlite3_vfs *sqlite3PagerVfs(Pager*);
sqlite3_file *sqlite3PagerFile(Pager*);
const char *sqlite3PagerJournalname(Pager*);
int sqlite3PagerNosync(Pager*);
int sqlite3PagerLoadall(Pager*);
void *sqlite3PagerTempSpace(Pager*);
int sqlite3PagerIsMemdb(Pager*);

void sqlite3PagerTruncateImage(Pager*,Pgno);

#if defined(SQLITE_HAS_CODEC) && !defined(SQLITE_OMIT_WAL)
void *sqlite3PagerCodec(DbPage *);
#endif

#if !defined(NDEBUG) || defined(SQLITE_TEST)
  Pgno sqlite3PagerPagenumber(DbPage*);
  int sqlite3PagerIswriteable(DbPage*);
#endif
#ifdef SQLITE_TEST
  int *sqlite3PagerStats(Pager*);
  void sqlite3PagerRefdump(Pager*);
  void disable_simulated_io_errors(void);
  void enable_simulated_io_errors(void);
#else
# define disable_simulated_io_errors()
# define enable_simulated_io_errors()
#endif

#endif 

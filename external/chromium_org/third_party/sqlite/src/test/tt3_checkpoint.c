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
** This file is part of the test program "threadtest3". Despite being a C
** file it is not compiled separately, but included by threadtest3.c using
** the #include directive normally used with header files.
**
** This file contains the implementation of test cases:
**
**     checkpoint_starvation_1
**     checkpoint_starvation_2
*/


#define CHECKPOINT_STARVATION_FRAMELIMIT 50

#define CHECKPOINT_STARVATION_READMS    100

struct CheckpointStarvationCtx {
  int eMode;
  int nMaxFrame;
};
typedef struct CheckpointStarvationCtx CheckpointStarvationCtx;

static int checkpoint_starvation_walhook(
  void *pCtx, 
  sqlite3 *db, 
  const char *zDb, 
  int nFrame
){
  CheckpointStarvationCtx *p = (CheckpointStarvationCtx *)pCtx;
  if( nFrame>p->nMaxFrame ){
    p->nMaxFrame = nFrame;
  }
  if( nFrame>=CHECKPOINT_STARVATION_FRAMELIMIT ){
    sqlite3_wal_checkpoint_v2(db, zDb, p->eMode, 0, 0);
  }
  return SQLITE_OK;
}

static char *checkpoint_starvation_reader(int iTid, int iArg){
  Error err = {0};
  Sqlite db = {0};

  opendb(&err, &db, "test.db", 0);
  while( !timetostop(&err) ){
    i64 iCount1, iCount2;
    sql_script(&err, &db, "BEGIN");
    iCount1 = execsql_i64(&err, &db, "SELECT count(x) FROM t1");
    usleep(CHECKPOINT_STARVATION_READMS*1000);
    iCount2 = execsql_i64(&err, &db, "SELECT count(x) FROM t1");
    sql_script(&err, &db, "COMMIT");

    if( iCount1!=iCount2 ){
      test_error(&err, "Isolation failure - %lld %lld", iCount1, iCount2);
    }
  }
  closedb(&err, &db);

  print_and_free_err(&err);
  return 0;
}

static void checkpoint_starvation_main(int nMs, CheckpointStarvationCtx *p){
  Error err = {0};
  Sqlite db = {0};
  Threadset threads = {0};
  int nInsert = 0;
  int i;

  opendb(&err, &db, "test.db", 1);
  sql_script(&err, &db, 
      "PRAGMA page_size = 1024;"
      "PRAGMA journal_mode = WAL;"
      "CREATE TABLE t1(x);"
  );

  setstoptime(&err, nMs);

  for(i=0; i<4; i++){
    launch_thread(&err, &threads, checkpoint_starvation_reader, 0);
    usleep(CHECKPOINT_STARVATION_READMS*1000/4);
  }

  sqlite3_wal_hook(db.db, checkpoint_starvation_walhook, (void *)p);
  while( !timetostop(&err) ){
    sql_script(&err, &db, "INSERT INTO t1 VALUES(randomblob(1200))");
    nInsert++;
  }

  printf(" Checkpoint mode  : %s\n",
      p->eMode==SQLITE_CHECKPOINT_PASSIVE ? "PASSIVE" : "RESTART"
  );
  printf(" Peak WAL         : %d frames\n", p->nMaxFrame);
  printf(" Transaction count: %d transactions\n", nInsert);

  join_all_threads(&err, &threads);
  closedb(&err, &db);
  print_and_free_err(&err);
}

static void checkpoint_starvation_1(int nMs){
  Error err = {0};
  CheckpointStarvationCtx ctx = { SQLITE_CHECKPOINT_PASSIVE, 0 };
  checkpoint_starvation_main(nMs, &ctx);
  if( ctx.nMaxFrame<(CHECKPOINT_STARVATION_FRAMELIMIT*10) ){
    test_error(&err, "WAL failed to grow - %d frames", ctx.nMaxFrame);
  }
  print_and_free_err(&err);
}

static void checkpoint_starvation_2(int nMs){
  Error err = {0};
  CheckpointStarvationCtx ctx = { SQLITE_CHECKPOINT_RESTART, 0 };
  checkpoint_starvation_main(nMs, &ctx);
  if( ctx.nMaxFrame>CHECKPOINT_STARVATION_FRAMELIMIT+10 ){
    test_error(&err, "WAL grew too large - %d frames", ctx.nMaxFrame);
  }
  print_and_free_err(&err);
}



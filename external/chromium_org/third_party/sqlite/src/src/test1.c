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
** Code for testing all sorts of SQLite interfaces.  This code
** is not included in the SQLite library.  It is used for automated
** testing of the SQLite library.
*/
#include "sqliteInt.h"
#include "tcl.h"
#include <stdlib.h>
#include <string.h>

struct SqliteDb {
  sqlite3 *db;
};

static int testHexToInt(int h){
  if( h>='0' && h<='9' ){
    return h - '0';
  }else if( h>='a' && h<='f' ){
    return h - 'a' + 10;
  }else{
    assert( h>='A' && h<='F' );
    return h - 'A' + 10;
  }
}
void *sqlite3TestTextToPtr(const char *z){
  void *p;
  u64 v;
  u32 v2;
  if( z[0]=='0' && z[1]=='x' ){
    z += 2;
  }
  v = 0;
  while( *z ){
    v = (v<<4) + testHexToInt(*z);
    z++;
  }
  if( sizeof(p)==sizeof(v) ){
    memcpy(&p, &v, sizeof(p));
  }else{
    assert( sizeof(p)==sizeof(v2) );
    v2 = (u32)v;
    memcpy(&p, &v2, sizeof(p));
  }
  return p;
}


static int get_sqlite_pointer(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  struct SqliteDb *p;
  Tcl_CmdInfo cmdInfo;
  char zBuf[100];
  if( objc!=2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "SQLITE-CONNECTION");
    return TCL_ERROR;
  }
  if( !Tcl_GetCommandInfo(interp, Tcl_GetString(objv[1]), &cmdInfo) ){
    Tcl_AppendResult(interp, "command not found: ",
           Tcl_GetString(objv[1]), (char*)0);
    return TCL_ERROR;
  }
  p = (struct SqliteDb*)cmdInfo.objClientData;
  sprintf(zBuf, "%p", p->db);
  if( strncmp(zBuf,"0x",2) ){
    sprintf(zBuf, "0x%p", p->db);
  }
  Tcl_AppendResult(interp, zBuf, 0);
  return TCL_OK;
}

int getDbPointer(Tcl_Interp *interp, const char *zA, sqlite3 **ppDb){
  struct SqliteDb *p;
  Tcl_CmdInfo cmdInfo;
  if( Tcl_GetCommandInfo(interp, zA, &cmdInfo) ){
    p = (struct SqliteDb*)cmdInfo.objClientData;
    *ppDb = p->db;
  }else{
    *ppDb = (sqlite3*)sqlite3TestTextToPtr(zA);
  }
  return TCL_OK;
}


const char *sqlite3TestErrorName(int rc){
  const char *zName = 0;
  switch( rc ){
    case SQLITE_OK:                  zName = "SQLITE_OK";                break;
    case SQLITE_ERROR:               zName = "SQLITE_ERROR";             break;
    case SQLITE_INTERNAL:            zName = "SQLITE_INTERNAL";          break;
    case SQLITE_PERM:                zName = "SQLITE_PERM";              break;
    case SQLITE_ABORT:               zName = "SQLITE_ABORT";             break;
    case SQLITE_BUSY:                zName = "SQLITE_BUSY";              break;
    case SQLITE_LOCKED:              zName = "SQLITE_LOCKED";            break;
    case SQLITE_LOCKED_SHAREDCACHE:  zName = "SQLITE_LOCKED_SHAREDCACHE";break;
    case SQLITE_NOMEM:               zName = "SQLITE_NOMEM";             break;
    case SQLITE_READONLY:            zName = "SQLITE_READONLY";          break;
    case SQLITE_INTERRUPT:           zName = "SQLITE_INTERRUPT";         break;
    case SQLITE_IOERR:               zName = "SQLITE_IOERR";             break;
    case SQLITE_CORRUPT:             zName = "SQLITE_CORRUPT";           break;
    case SQLITE_NOTFOUND:            zName = "SQLITE_NOTFOUND";          break;
    case SQLITE_FULL:                zName = "SQLITE_FULL";              break;
    case SQLITE_CANTOPEN:            zName = "SQLITE_CANTOPEN";          break;
    case SQLITE_PROTOCOL:            zName = "SQLITE_PROTOCOL";          break;
    case SQLITE_EMPTY:               zName = "SQLITE_EMPTY";             break;
    case SQLITE_SCHEMA:              zName = "SQLITE_SCHEMA";            break;
    case SQLITE_TOOBIG:              zName = "SQLITE_TOOBIG";            break;
    case SQLITE_CONSTRAINT:          zName = "SQLITE_CONSTRAINT";        break;
    case SQLITE_MISMATCH:            zName = "SQLITE_MISMATCH";          break;
    case SQLITE_MISUSE:              zName = "SQLITE_MISUSE";            break;
    case SQLITE_NOLFS:               zName = "SQLITE_NOLFS";             break;
    case SQLITE_AUTH:                zName = "SQLITE_AUTH";              break;
    case SQLITE_FORMAT:              zName = "SQLITE_FORMAT";            break;
    case SQLITE_RANGE:               zName = "SQLITE_RANGE";             break;
    case SQLITE_NOTADB:              zName = "SQLITE_NOTADB";            break;
    case SQLITE_ROW:                 zName = "SQLITE_ROW";               break;
    case SQLITE_DONE:                zName = "SQLITE_DONE";              break;
    case SQLITE_IOERR_READ:          zName = "SQLITE_IOERR_READ";        break;
    case SQLITE_IOERR_SHORT_READ:    zName = "SQLITE_IOERR_SHORT_READ";  break;
    case SQLITE_IOERR_WRITE:         zName = "SQLITE_IOERR_WRITE";       break;
    case SQLITE_IOERR_FSYNC:         zName = "SQLITE_IOERR_FSYNC";       break;
    case SQLITE_IOERR_DIR_FSYNC:     zName = "SQLITE_IOERR_DIR_FSYNC";   break;
    case SQLITE_IOERR_TRUNCATE:      zName = "SQLITE_IOERR_TRUNCATE";    break;
    case SQLITE_IOERR_FSTAT:         zName = "SQLITE_IOERR_FSTAT";       break;
    case SQLITE_IOERR_UNLOCK:        zName = "SQLITE_IOERR_UNLOCK";      break;
    case SQLITE_IOERR_RDLOCK:        zName = "SQLITE_IOERR_RDLOCK";      break;
    case SQLITE_IOERR_DELETE:        zName = "SQLITE_IOERR_DELETE";      break;
    case SQLITE_IOERR_BLOCKED:       zName = "SQLITE_IOERR_BLOCKED";     break;
    case SQLITE_IOERR_NOMEM:         zName = "SQLITE_IOERR_NOMEM";       break;
    case SQLITE_IOERR_ACCESS:        zName = "SQLITE_IOERR_ACCESS";      break;
    case SQLITE_IOERR_CHECKRESERVEDLOCK:
                               zName = "SQLITE_IOERR_CHECKRESERVEDLOCK"; break;
    case SQLITE_IOERR_LOCK:          zName = "SQLITE_IOERR_LOCK";        break;
    default:                         zName = "SQLITE_Unknown";           break;
  }
  return zName;
}
#define t1ErrorName sqlite3TestErrorName

#define StmtToDb(X)   sqlite3_db_handle(X)

int sqlite3TestErrCode(Tcl_Interp *interp, sqlite3 *db, int rc){
  if( sqlite3_threadsafe()==0 && rc!=SQLITE_MISUSE && rc!=SQLITE_OK
   && sqlite3_errcode(db)!=rc ){
    char zBuf[200];
    int r2 = sqlite3_errcode(db);
    sprintf(zBuf, "error code %s (%d) does not match sqlite3_errcode %s (%d)",
       t1ErrorName(rc), rc, t1ErrorName(r2), r2);
    Tcl_ResetResult(interp);
    Tcl_AppendResult(interp, zBuf, 0);
    return 1;
  }
  return 0;
}

static int getStmtPointer(
  Tcl_Interp *interp, 
  const char *zArg,  
  sqlite3_stmt **ppStmt
){
  *ppStmt = (sqlite3_stmt*)sqlite3TestTextToPtr(zArg);
  return TCL_OK;
}

int sqlite3TestMakePointerStr(Tcl_Interp *interp, char *zPtr, void *p){
  sqlite3_snprintf(100, zPtr, "%p", p);
  return TCL_OK;
}

static int exec_printf_cb(void *pArg, int argc, char **argv, char **name){
  Tcl_DString *str = (Tcl_DString*)pArg;
  int i;

  if( Tcl_DStringLength(str)==0 ){
    for(i=0; i<argc; i++){
      Tcl_DStringAppendElement(str, name[i] ? name[i] : "NULL");
    }
  }
  for(i=0; i<argc; i++){
    Tcl_DStringAppendElement(str, argv[i] ? argv[i] : "NULL");
  }
  return 0;
}

#if !defined(SQLITE_OMIT_TRACE) && defined(SQLITE_ENABLE_IOTRACE)
static FILE *iotrace_file = 0;
static void io_trace_callback(const char *zFormat, ...){
  va_list ap;
  va_start(ap, zFormat);
  vfprintf(iotrace_file, zFormat, ap);
  va_end(ap);
  fflush(iotrace_file);
}
#endif

static int test_io_trace(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
#if !defined(SQLITE_OMIT_TRACE) && defined(SQLITE_ENABLE_IOTRACE)
  if( argc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
          " FILENAME\"", 0);
    return TCL_ERROR;
  }
  if( iotrace_file ){
    if( iotrace_file!=stdout && iotrace_file!=stderr ){
      fclose(iotrace_file);
    }
    iotrace_file = 0;
    sqlite3IoTrace = 0;
  }
  if( argv[1][0] ){
    if( strcmp(argv[1],"stdout")==0 ){
      iotrace_file = stdout;
    }else if( strcmp(argv[1],"stderr")==0 ){
      iotrace_file = stderr;
    }else{
      iotrace_file = fopen(argv[1], "w");
    }
    sqlite3IoTrace = io_trace_callback;
  }
#endif
  return TCL_OK;
}


static int test_exec_printf(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  sqlite3 *db;
  Tcl_DString str;
  int rc;
  char *zErr = 0;
  char *zSql;
  char zBuf[30];
  if( argc!=4 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0], 
       " DB FORMAT STRING", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, argv[1], &db) ) return TCL_ERROR;
  Tcl_DStringInit(&str);
  zSql = sqlite3_mprintf(argv[2], argv[3]);
  rc = sqlite3_exec(db, zSql, exec_printf_cb, &str, &zErr);
  sqlite3_free(zSql);
  sprintf(zBuf, "%d", rc);
  Tcl_AppendElement(interp, zBuf);
  Tcl_AppendElement(interp, rc==SQLITE_OK ? Tcl_DStringValue(&str) : zErr);
  Tcl_DStringFree(&str);
  if( zErr ) sqlite3_free(zErr);
  if( sqlite3TestErrCode(interp, db, rc) ) return TCL_ERROR;
  return TCL_OK;
}

static int test_exec_hex(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  sqlite3 *db;
  Tcl_DString str;
  int rc, i, j;
  char *zErr = 0;
  char *zHex;
  char zSql[500];
  char zBuf[30];
  if( argc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0], 
       " DB HEX", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, argv[1], &db) ) return TCL_ERROR;
  zHex = argv[2];
  for(i=j=0; i<sizeof(zSql) && zHex[j]; i++, j++){
    if( zHex[j]=='%' && zHex[j+2] && zHex[j+2] ){
      zSql[i] = (testHexToInt(zHex[j+1])<<4) + testHexToInt(zHex[j+2]);
      j += 2;
    }else{
      zSql[i] = zHex[j];
    }
  }
  zSql[i] = 0;
  Tcl_DStringInit(&str);
  rc = sqlite3_exec(db, zSql, exec_printf_cb, &str, &zErr);
  sprintf(zBuf, "%d", rc);
  Tcl_AppendElement(interp, zBuf);
  Tcl_AppendElement(interp, rc==SQLITE_OK ? Tcl_DStringValue(&str) : zErr);
  Tcl_DStringFree(&str);
  if( zErr ) sqlite3_free(zErr);
  if( sqlite3TestErrCode(interp, db, rc) ) return TCL_ERROR;
  return TCL_OK;
}

static int db_enter(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  sqlite3 *db;
  if( argc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0], 
       " DB", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, argv[1], &db) ) return TCL_ERROR;
  sqlite3_mutex_enter(db->mutex);
  return TCL_OK;
}
static int db_leave(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  sqlite3 *db;
  if( argc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0], 
       " DB", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, argv[1], &db) ) return TCL_ERROR;
  sqlite3_mutex_leave(db->mutex);
  return TCL_OK;
}

static int test_exec(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  sqlite3 *db;
  Tcl_DString str;
  int rc;
  char *zErr = 0;
  char *zSql;
  int i, j;
  char zBuf[30];
  if( argc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0], 
       " DB SQL", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, argv[1], &db) ) return TCL_ERROR;
  Tcl_DStringInit(&str);
  zSql = sqlite3_mprintf("%s", argv[2]);
  for(i=j=0; zSql[i];){
    if( zSql[i]=='%' ){
      zSql[j++] = (testHexToInt(zSql[i+1])<<4) + testHexToInt(zSql[i+2]);
      i += 3;
    }else{
      zSql[j++] = zSql[i++];
    }
  }
  zSql[j] = 0;
  rc = sqlite3_exec(db, zSql, exec_printf_cb, &str, &zErr);
  sqlite3_free(zSql);
  sprintf(zBuf, "%d", rc);
  Tcl_AppendElement(interp, zBuf);
  Tcl_AppendElement(interp, rc==SQLITE_OK ? Tcl_DStringValue(&str) : zErr);
  Tcl_DStringFree(&str);
  if( zErr ) sqlite3_free(zErr);
  if( sqlite3TestErrCode(interp, db, rc) ) return TCL_ERROR;
  return TCL_OK;
}

static int test_exec_nr(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  sqlite3 *db;
  int rc;
  char *zErr = 0;
  if( argc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0], 
       " DB SQL", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, argv[1], &db) ) return TCL_ERROR;
  rc = sqlite3_exec(db, argv[2], 0, 0, &zErr);
  if( sqlite3TestErrCode(interp, db, rc) ) return TCL_ERROR;
  return TCL_OK;
}

static int test_mprintf_z(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  char *zResult = 0;
  int i;

  for(i=2; i<argc && (i==2 || zResult); i++){
    zResult = sqlite3_mprintf("%z%s%s", zResult, argv[1], argv[i]);
  }
  Tcl_AppendResult(interp, zResult, 0);
  sqlite3_free(zResult);
  return TCL_OK;
}

static int test_mprintf_n(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  char *zStr;
  int n = 0;
  zStr = sqlite3_mprintf("%s%n", argv[1], &n);
  sqlite3_free(zStr);
  Tcl_SetObjResult(interp, Tcl_NewIntObj(n));
  return TCL_OK;
}

static int test_snprintf_int(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  char zStr[100];
  int n = atoi(argv[1]);
  const char *zFormat = argv[2];
  int a1 = atoi(argv[3]);
  if( n>sizeof(zStr) ) n = sizeof(zStr);
  sqlite3_snprintf(sizeof(zStr), zStr, "abcdefghijklmnopqrstuvwxyz");
  sqlite3_snprintf(n, zStr, zFormat, a1);
  Tcl_AppendResult(interp, zStr, 0);
  return TCL_OK;
}

#ifndef SQLITE_OMIT_GET_TABLE

static int test_get_table_printf(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  sqlite3 *db;
  Tcl_DString str;
  int rc;
  char *zErr = 0;
  int nRow, nCol;
  char **aResult;
  int i;
  char zBuf[30];
  char *zSql;
  int resCount = -1;
  if( argc==5 ){
    if( Tcl_GetInt(interp, argv[4], &resCount) ) return TCL_ERROR;
  }
  if( argc!=4 && argc!=5 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0], 
       " DB FORMAT STRING ?COUNT?", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, argv[1], &db) ) return TCL_ERROR;
  Tcl_DStringInit(&str);
  zSql = sqlite3_mprintf(argv[2],argv[3]);
  if( argc==5 ){
    rc = sqlite3_get_table(db, zSql, &aResult, 0, 0, &zErr);
  }else{
    rc = sqlite3_get_table(db, zSql, &aResult, &nRow, &nCol, &zErr);
    resCount = (nRow+1)*nCol;
  }
  sqlite3_free(zSql);
  sprintf(zBuf, "%d", rc);
  Tcl_AppendElement(interp, zBuf);
  if( rc==SQLITE_OK ){
    if( argc==4 ){
      sprintf(zBuf, "%d", nRow);
      Tcl_AppendElement(interp, zBuf);
      sprintf(zBuf, "%d", nCol);
      Tcl_AppendElement(interp, zBuf);
    }
    for(i=0; i<resCount; i++){
      Tcl_AppendElement(interp, aResult[i] ? aResult[i] : "NULL");
    }
  }else{
    Tcl_AppendElement(interp, zErr);
  }
  sqlite3_free_table(aResult);
  if( zErr ) sqlite3_free(zErr);
  if( sqlite3TestErrCode(interp, db, rc) ) return TCL_ERROR;
  return TCL_OK;
}

#endif 


static int test_last_rowid(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  sqlite3 *db;
  char zBuf[30];

  if( argc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0], " DB\"", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, argv[1], &db) ) return TCL_ERROR;
  sprintf(zBuf, "%lld", sqlite3_last_insert_rowid(db));
  Tcl_AppendResult(interp, zBuf, 0);
  return SQLITE_OK;
}

static int test_key(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  sqlite3 *db;
  const char *zKey;
  int nKey;
  if( argc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " FILENAME\"", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, argv[1], &db) ) return TCL_ERROR;
  zKey = argv[2];
  nKey = strlen(zKey);
#ifdef SQLITE_HAS_CODEC
  sqlite3_key(db, zKey, nKey);
#endif
  return TCL_OK;
}

static int test_rekey(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  sqlite3 *db;
  const char *zKey;
  int nKey;
  if( argc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " FILENAME\"", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, argv[1], &db) ) return TCL_ERROR;
  zKey = argv[2];
  nKey = strlen(zKey);
#ifdef SQLITE_HAS_CODEC
  sqlite3_rekey(db, zKey, nKey);
#endif
  return TCL_OK;
}

static int sqlite_test_close(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  sqlite3 *db;
  int rc;
  if( argc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " FILENAME\"", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, argv[1], &db) ) return TCL_ERROR;
  rc = sqlite3_close(db);
  Tcl_SetResult(interp, (char *)t1ErrorName(rc), TCL_STATIC);
  return TCL_OK;
}

static void t1_ifnullFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  int i;
  for(i=0; i<argc; i++){
    if( SQLITE_NULL!=sqlite3_value_type(argv[i]) ){
      int n = sqlite3_value_bytes(argv[i]);
      sqlite3_result_text(context, (char*)sqlite3_value_text(argv[i]),
          n, SQLITE_TRANSIENT);
      break;
    }
  }
}

static void hex8Func(sqlite3_context *p, int argc, sqlite3_value **argv){
  const unsigned char *z;
  int i;
  char zBuf[200];
  z = sqlite3_value_text(argv[0]);
  for(i=0; i<sizeof(zBuf)/2 - 2 && z[i]; i++){
    sprintf(&zBuf[i*2], "%02x", z[i]&0xff);
  }
  zBuf[i*2] = 0;
  sqlite3_result_text(p, (char*)zBuf, -1, SQLITE_TRANSIENT);
}
#ifndef SQLITE_OMIT_UTF16
static void hex16Func(sqlite3_context *p, int argc, sqlite3_value **argv){
  const unsigned short int *z;
  int i;
  char zBuf[400];
  z = sqlite3_value_text16(argv[0]);
  for(i=0; i<sizeof(zBuf)/4 - 4 && z[i]; i++){
    sprintf(&zBuf[i*4], "%04x", z[i]&0xff);
  }
  zBuf[i*4] = 0;
  sqlite3_result_text(p, (char*)zBuf, -1, SQLITE_TRANSIENT);
}
#endif

struct dstr {
  int nAlloc;  
  int nUsed;   
  char *z;     
};

static void dstrAppend(struct dstr *p, const char *z, int divider){
  int n = strlen(z);
  if( p->nUsed + n + 2 > p->nAlloc ){
    char *zNew;
    p->nAlloc = p->nAlloc*2 + n + 200;
    zNew = sqlite3_realloc(p->z, p->nAlloc);
    if( zNew==0 ){
      sqlite3_free(p->z);
      memset(p, 0, sizeof(*p));
      return;
    }
    p->z = zNew;
  }
  if( divider && p->nUsed>0 ){
    p->z[p->nUsed++] = divider;
  }
  memcpy(&p->z[p->nUsed], z, n+1);
  p->nUsed += n;
}

static int execFuncCallback(void *pData, int argc, char **argv, char **NotUsed){
  struct dstr *p = (struct dstr*)pData;
  int i;
  for(i=0; i<argc; i++){
    if( argv[i]==0 ){
      dstrAppend(p, "NULL", ' ');
    }else{
      dstrAppend(p, argv[i], ' ');
    }
  }
  return 0;
}

static void sqlite3ExecFunc(
  sqlite3_context *context, 
  int argc,  
  sqlite3_value **argv
){
  struct dstr x;
  memset(&x, 0, sizeof(x));
  (void)sqlite3_exec((sqlite3*)sqlite3_user_data(context),
      (char*)sqlite3_value_text(argv[0]),
      execFuncCallback, &x, 0);
  sqlite3_result_text(context, x.z, x.nUsed, SQLITE_TRANSIENT);
  sqlite3_free(x.z);
}

static void tkt2213Function(
  sqlite3_context *context, 
  int argc,  
  sqlite3_value **argv
){
  int nText;
  unsigned char const *zText1;
  unsigned char const *zText2;
  unsigned char const *zText3;

  nText = sqlite3_value_bytes(argv[0]);
  zText1 = sqlite3_value_text(argv[0]);
  zText2 = sqlite3_value_text(argv[0]);
  zText3 = sqlite3_value_text(argv[0]);

  if( zText1!=zText2 || zText2!=zText3 ){
    sqlite3_result_error(context, "tkt2213 is not fixed", -1);
  }else{
    char *zCopy = (char *)sqlite3_malloc(nText);
    memcpy(zCopy, zText1, nText);
    sqlite3_result_text(context, zCopy, nText, sqlite3_free);
  }
}

static void ptrChngFunction(
  sqlite3_context *context, 
  int argc,  
  sqlite3_value **argv
){
  const void *p1, *p2;
  const char *zCmd;
  if( argc!=4 ) return;
  zCmd = (const char*)sqlite3_value_text(argv[1]);
  if( zCmd==0 ) return;
  if( strcmp(zCmd,"text")==0 ){
    p1 = (const void*)sqlite3_value_text(argv[0]);
#ifndef SQLITE_OMIT_UTF16
  }else if( strcmp(zCmd, "text16")==0 ){
    p1 = (const void*)sqlite3_value_text16(argv[0]);
#endif
  }else if( strcmp(zCmd, "blob")==0 ){
    p1 = (const void*)sqlite3_value_blob(argv[0]);
  }else{
    return;
  }
  zCmd = (const char*)sqlite3_value_text(argv[2]);
  if( zCmd==0 ) return;
  if( strcmp(zCmd,"bytes")==0 ){
    sqlite3_value_bytes(argv[0]);
#ifndef SQLITE_OMIT_UTF16
  }else if( strcmp(zCmd, "bytes16")==0 ){
    sqlite3_value_bytes16(argv[0]);
#endif
  }else if( strcmp(zCmd, "noop")==0 ){
    
  }else{
    return;
  }
  zCmd = (const char*)sqlite3_value_text(argv[3]);
  if( zCmd==0 ) return;
  if( strcmp(zCmd,"text")==0 ){
    p2 = (const void*)sqlite3_value_text(argv[0]);
#ifndef SQLITE_OMIT_UTF16
  }else if( strcmp(zCmd, "text16")==0 ){
    p2 = (const void*)sqlite3_value_text16(argv[0]);
#endif
  }else if( strcmp(zCmd, "blob")==0 ){
    p2 = (const void*)sqlite3_value_blob(argv[0]);
  }else{
    return;
  }
  sqlite3_result_int(context, p1!=p2);
}


static int test_create_function(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  int rc;
  sqlite3 *db;

  if( argc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " DB\"", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, argv[1], &db) ) return TCL_ERROR;
  rc = sqlite3_create_function(db, "x_coalesce", -1, SQLITE_ANY, 0, 
        t1_ifnullFunc, 0, 0);
  if( rc==SQLITE_OK ){
    rc = sqlite3_create_function(db, "hex8", 1, SQLITE_ANY, 0, 
          hex8Func, 0, 0);
  }
#ifndef SQLITE_OMIT_UTF16
  if( rc==SQLITE_OK ){
    rc = sqlite3_create_function(db, "hex16", 1, SQLITE_ANY, 0, 
          hex16Func, 0, 0);
  }
#endif
  if( rc==SQLITE_OK ){
    rc = sqlite3_create_function(db, "tkt2213func", 1, SQLITE_ANY, 0, 
          tkt2213Function, 0, 0);
  }
  if( rc==SQLITE_OK ){
    rc = sqlite3_create_function(db, "pointer_change", 4, SQLITE_ANY, 0, 
          ptrChngFunction, 0, 0);
  }

#ifndef SQLITE_OMIT_UTF16
  if( rc==SQLITE_OK ){
    const void *zUtf16;
    sqlite3_value *pVal;
    sqlite3_mutex_enter(db->mutex);
    pVal = sqlite3ValueNew(db);
    sqlite3ValueSetStr(pVal, -1, "x_sqlite_exec", SQLITE_UTF8, SQLITE_STATIC);
    zUtf16 = sqlite3ValueText(pVal, SQLITE_UTF16NATIVE);
    if( db->mallocFailed ){
      rc = SQLITE_NOMEM;
    }else{
      rc = sqlite3_create_function16(db, zUtf16, 
                1, SQLITE_UTF16, db, sqlite3ExecFunc, 0, 0);
    }
    sqlite3ValueFree(pVal);
    sqlite3_mutex_leave(db->mutex);
  }
#endif

  if( sqlite3TestErrCode(interp, db, rc) ) return TCL_ERROR;
  Tcl_SetResult(interp, (char *)t1ErrorName(rc), 0);
  return TCL_OK;
}

typedef struct t1CountCtx t1CountCtx;
struct t1CountCtx {
  int n;
};
static void t1CountStep(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  t1CountCtx *p;
  p = sqlite3_aggregate_context(context, sizeof(*p));
  if( (argc==0 || SQLITE_NULL!=sqlite3_value_type(argv[0]) ) && p ){
    p->n++;
  }
  if( argc>0 ){
    int v = sqlite3_value_int(argv[0]);
    if( v==40 ){
      sqlite3_result_error(context, "value of 40 handed to x_count", -1);
#ifndef SQLITE_OMIT_UTF16
    }else if( v==41 ){
      const char zUtf16ErrMsg[] = { 0, 0x61, 0, 0x62, 0, 0x63, 0, 0, 0};
      sqlite3_result_error16(context, &zUtf16ErrMsg[1-SQLITE_BIGENDIAN], -1);
#endif
    }
  }
}   
static void t1CountFinalize(sqlite3_context *context){
  t1CountCtx *p;
  p = sqlite3_aggregate_context(context, sizeof(*p));
  if( p ){
    if( p->n==42 ){
      sqlite3_result_error(context, "x_count totals to 42", -1);
    }else{
      sqlite3_result_int(context, p ? p->n : 0);
    }
  }
}

#ifndef SQLITE_OMIT_DEPRECATED
static void legacyCountStep(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  
}

static void legacyCountFinalize(sqlite3_context *context){
  sqlite3_result_int(context, sqlite3_aggregate_count(context));
}
#endif

static int test_create_aggregate(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  sqlite3 *db;
  int rc;
  if( argc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " FILENAME\"", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, argv[1], &db) ) return TCL_ERROR;
  rc = sqlite3_create_function(db, "x_count", 0, SQLITE_UTF8, 0, 0,
      t1CountStep,t1CountFinalize);
  if( rc==SQLITE_OK ){
    rc = sqlite3_create_function(db, "x_count", 1, SQLITE_UTF8, 0, 0,
        t1CountStep,t1CountFinalize);
  }
#ifndef SQLITE_OMIT_DEPRECATED
  if( rc==SQLITE_OK ){
    rc = sqlite3_create_function(db, "legacy_count", 0, SQLITE_ANY, 0, 0,
        legacyCountStep, legacyCountFinalize
    );
  }
#endif
  if( sqlite3TestErrCode(interp, db, rc) ) return TCL_ERROR;
  Tcl_SetResult(interp, (char *)t1ErrorName(rc), 0);
  return TCL_OK;
}


static int test_printf(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  if( argc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " TEXT\"", 0);
    return TCL_ERROR;
  }
  printf("%s\n", argv[1]);
  return TCL_OK;
}



static int sqlite3_mprintf_int(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  int a[3], i;
  char *z;
  if( argc!=5 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " FORMAT INT INT INT\"", 0);
    return TCL_ERROR;
  }
  for(i=2; i<5; i++){
    if( Tcl_GetInt(interp, argv[i], &a[i-2]) ) return TCL_ERROR;
  }
  z = sqlite3_mprintf(argv[1], a[0], a[1], a[2]);
  Tcl_AppendResult(interp, z, 0);
  sqlite3_free(z);
  return TCL_OK;
}

static int sqlite3_mprintf_int64(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  int i;
  sqlite_int64 a[3];
  char *z;
  if( argc!=5 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " FORMAT INT INT INT\"", 0);
    return TCL_ERROR;
  }
  for(i=2; i<5; i++){
    if( sqlite3Atoi64(argv[i], &a[i-2], 1000000, SQLITE_UTF8) ){
      Tcl_AppendResult(interp, "argument is not a valid 64-bit integer", 0);
      return TCL_ERROR;
    }
  }
  z = sqlite3_mprintf(argv[1], a[0], a[1], a[2]);
  Tcl_AppendResult(interp, z, 0);
  sqlite3_free(z);
  return TCL_OK;
}

static int sqlite3_mprintf_long(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  int i;
  long int a[3];
  int b[3];
  char *z;
  if( argc!=5 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " FORMAT INT INT INT\"", 0);
    return TCL_ERROR;
  }
  for(i=2; i<5; i++){
    if( Tcl_GetInt(interp, argv[i], &b[i-2]) ) return TCL_ERROR;
    a[i-2] = (long int)b[i-2];
    a[i-2] &= (((u64)1)<<(sizeof(int)*8))-1;
  }
  z = sqlite3_mprintf(argv[1], a[0], a[1], a[2]);
  Tcl_AppendResult(interp, z, 0);
  sqlite3_free(z);
  return TCL_OK;
}

static int sqlite3_mprintf_str(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  int a[3], i;
  char *z;
  if( argc<4 || argc>5 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " FORMAT INT INT ?STRING?\"", 0);
    return TCL_ERROR;
  }
  for(i=2; i<4; i++){
    if( Tcl_GetInt(interp, argv[i], &a[i-2]) ) return TCL_ERROR;
  }
  z = sqlite3_mprintf(argv[1], a[0], a[1], argc>4 ? argv[4] : NULL);
  Tcl_AppendResult(interp, z, 0);
  sqlite3_free(z);
  return TCL_OK;
}

static int sqlite3_snprintf_str(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  int a[3], i;
  int n;
  char *z;
  if( argc<5 || argc>6 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " INT FORMAT INT INT ?STRING?\"", 0);
    return TCL_ERROR;
  }
  if( Tcl_GetInt(interp, argv[1], &n) ) return TCL_ERROR;
  if( n<0 ){
    Tcl_AppendResult(interp, "N must be non-negative", 0);
    return TCL_ERROR;
  }
  for(i=3; i<5; i++){
    if( Tcl_GetInt(interp, argv[i], &a[i-3]) ) return TCL_ERROR;
  }
  z = sqlite3_malloc( n+1 );
  sqlite3_snprintf(n, z, argv[2], a[0], a[1], argc>4 ? argv[5] : NULL);
  Tcl_AppendResult(interp, z, 0);
  sqlite3_free(z);
  return TCL_OK;
}

static int sqlite3_mprintf_double(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  int a[3], i;
  double r;
  char *z;
  if( argc!=5 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " FORMAT INT INT DOUBLE\"", 0);
    return TCL_ERROR;
  }
  for(i=2; i<4; i++){
    if( Tcl_GetInt(interp, argv[i], &a[i-2]) ) return TCL_ERROR;
  }
  if( Tcl_GetDouble(interp, argv[4], &r) ) return TCL_ERROR;
  z = sqlite3_mprintf(argv[1], a[0], a[1], r);
  Tcl_AppendResult(interp, z, 0);
  sqlite3_free(z);
  return TCL_OK;
}

static int sqlite3_mprintf_scaled(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  int i;
  double r[2];
  char *z;
  if( argc!=4 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " FORMAT DOUBLE DOUBLE\"", 0);
    return TCL_ERROR;
  }
  for(i=2; i<4; i++){
    if( Tcl_GetDouble(interp, argv[i], &r[i-2]) ) return TCL_ERROR;
  }
  z = sqlite3_mprintf(argv[1], r[0]*r[1]);
  Tcl_AppendResult(interp, z, 0);
  sqlite3_free(z);
  return TCL_OK;
}

static int sqlite3_mprintf_stronly(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  char *z;
  if( argc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " FORMAT STRING\"", 0);
    return TCL_ERROR;
  }
  z = sqlite3_mprintf(argv[1], argv[2]);
  Tcl_AppendResult(interp, z, 0);
  sqlite3_free(z);
  return TCL_OK;
}

static int sqlite3_mprintf_hexdouble(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  char *z;
  double r;
  unsigned int x1, x2;
  sqlite_uint64 d;
  if( argc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
       " FORMAT STRING\"", 0);
    return TCL_ERROR;
  }
  if( sscanf(argv[2], "%08x%08x", &x2, &x1)!=2 ){
    Tcl_AppendResult(interp, "2nd argument should be 16-characters of hex", 0);
    return TCL_ERROR;
  }
  d = x2;
  d = (d<<32) + x1;
  memcpy(&r, &d, sizeof(r));
  z = sqlite3_mprintf(argv[1], r);
  Tcl_AppendResult(interp, z, 0);
  sqlite3_free(z);
  return TCL_OK;
}

#if !defined(SQLITE_OMIT_SHARED_CACHE)
static int test_enable_shared(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  int rc;
  int enable;
  int ret = 0;

  if( objc!=2 && objc!=1 ){
    Tcl_WrongNumArgs(interp, 1, objv, "?BOOLEAN?");
    return TCL_ERROR;
  }
  ret = sqlite3GlobalConfig.sharedCacheEnabled;

  if( objc==2 ){
    if( Tcl_GetBooleanFromObj(interp, objv[1], &enable) ){
      return TCL_ERROR;
    }
    rc = sqlite3_enable_shared_cache(enable);
    if( rc!=SQLITE_OK ){
      Tcl_SetResult(interp, (char *)sqlite3ErrStr(rc), TCL_STATIC);
      return TCL_ERROR;
    }
  }
  Tcl_SetObjResult(interp, Tcl_NewBooleanObj(ret));
  return TCL_OK;
}
#endif



static int test_extended_result_codes(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  int enable;
  sqlite3 *db;

  if( objc!=3 ){
    Tcl_WrongNumArgs(interp, 1, objv, "DB BOOLEAN");
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;
  if( Tcl_GetBooleanFromObj(interp, objv[2], &enable) ) return TCL_ERROR;
  sqlite3_extended_result_codes(db, enable);
  return TCL_OK;
}

static int test_libversion_number(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  Tcl_SetObjResult(interp, Tcl_NewIntObj(sqlite3_libversion_number()));
  return TCL_OK;
}

#ifdef SQLITE_ENABLE_COLUMN_METADATA
static int test_table_column_metadata(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  sqlite3 *db;
  const char *zDb;
  const char *zTbl;
  const char *zCol;
  int rc;
  Tcl_Obj *pRet;

  const char *zDatatype;
  const char *zCollseq;
  int notnull;
  int primarykey;
  int autoincrement;

  if( objc!=5 ){
    Tcl_WrongNumArgs(interp, 1, objv, "DB dbname tblname colname");
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;
  zDb = Tcl_GetString(objv[2]);
  zTbl = Tcl_GetString(objv[3]);
  zCol = Tcl_GetString(objv[4]);

  if( strlen(zDb)==0 ) zDb = 0;

  rc = sqlite3_table_column_metadata(db, zDb, zTbl, zCol, 
      &zDatatype, &zCollseq, &notnull, &primarykey, &autoincrement);

  if( rc!=SQLITE_OK ){
    Tcl_AppendResult(interp, sqlite3_errmsg(db), 0);
    return TCL_ERROR;
  }

  pRet = Tcl_NewObj();
  Tcl_ListObjAppendElement(0, pRet, Tcl_NewStringObj(zDatatype, -1));
  Tcl_ListObjAppendElement(0, pRet, Tcl_NewStringObj(zCollseq, -1));
  Tcl_ListObjAppendElement(0, pRet, Tcl_NewIntObj(notnull));
  Tcl_ListObjAppendElement(0, pRet, Tcl_NewIntObj(primarykey));
  Tcl_ListObjAppendElement(0, pRet, Tcl_NewIntObj(autoincrement));
  Tcl_SetObjResult(interp, pRet);

  return TCL_OK;
}
#endif

#ifndef SQLITE_OMIT_INCRBLOB

static int blobHandleFromObj(
  Tcl_Interp *interp, 
  Tcl_Obj *pObj,
  sqlite3_blob **ppBlob
){
  char *z;
  int n;

  z = Tcl_GetStringFromObj(pObj, &n);
  if( n==0 ){
    *ppBlob = 0;
  }else{
    int notUsed;
    Tcl_Channel channel;
    ClientData instanceData;
    
    channel = Tcl_GetChannel(interp, z, &notUsed);
    if( !channel ) return TCL_ERROR;

    Tcl_Flush(channel);
    Tcl_Seek(channel, 0, SEEK_SET);

    instanceData = Tcl_GetChannelInstanceData(channel);
    *ppBlob = *((sqlite3_blob **)instanceData);
  }

  return TCL_OK;
}

static int test_blob_bytes(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  sqlite3_blob *pBlob;
  int nByte;
  
  if( objc!=2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "CHANNEL");
    return TCL_ERROR;
  }

  if( blobHandleFromObj(interp, objv[1], &pBlob) ) return TCL_ERROR;
  nByte = sqlite3_blob_bytes(pBlob);
  Tcl_SetObjResult(interp, Tcl_NewIntObj(nByte));

  return TCL_OK;
}

static int test_blob_close(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  sqlite3_blob *pBlob;
  
  if( objc!=2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "CHANNEL");
    return TCL_ERROR;
  }

  if( blobHandleFromObj(interp, objv[1], &pBlob) ) return TCL_ERROR;
  sqlite3_blob_close(pBlob);

  return TCL_OK;
}

static int test_blob_read(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  sqlite3_blob *pBlob;
  int nByte;
  int iOffset;
  unsigned char *zBuf = 0;
  int rc;
  
  if( objc!=4 ){
    Tcl_WrongNumArgs(interp, 1, objv, "CHANNEL OFFSET N");
    return TCL_ERROR;
  }

  if( blobHandleFromObj(interp, objv[1], &pBlob) ) return TCL_ERROR;
  if( TCL_OK!=Tcl_GetIntFromObj(interp, objv[2], &iOffset)
   || TCL_OK!=Tcl_GetIntFromObj(interp, objv[3], &nByte)
  ){ 
    return TCL_ERROR;
  }

  if( nByte>0 ){
    zBuf = (unsigned char *)Tcl_Alloc(nByte);
  }
  rc = sqlite3_blob_read(pBlob, zBuf, nByte, iOffset);
  if( rc==SQLITE_OK ){
    Tcl_SetObjResult(interp, Tcl_NewByteArrayObj(zBuf, nByte));
  }else{
    Tcl_SetResult(interp, (char *)sqlite3TestErrorName(rc), TCL_VOLATILE);
  }
  Tcl_Free((char *)zBuf);

  return (rc==SQLITE_OK ? TCL_OK : TCL_ERROR);
}

static int test_blob_write(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  sqlite3_blob *pBlob;
  int iOffset;
  int rc;

  unsigned char *zBuf;
  int nBuf;
  
  if( objc!=4 && objc!=5 ){
    Tcl_WrongNumArgs(interp, 1, objv, "CHANNEL OFFSET DATA ?NDATA?");
    return TCL_ERROR;
  }

  if( blobHandleFromObj(interp, objv[1], &pBlob) ) return TCL_ERROR;
  if( TCL_OK!=Tcl_GetIntFromObj(interp, objv[2], &iOffset) ){ 
    return TCL_ERROR;
  }

  zBuf = Tcl_GetByteArrayFromObj(objv[3], &nBuf);
  if( objc==5 && Tcl_GetIntFromObj(interp, objv[4], &nBuf) ){
    return TCL_ERROR;
  }
  rc = sqlite3_blob_write(pBlob, zBuf, nBuf, iOffset);
  if( rc!=SQLITE_OK ){
    Tcl_SetResult(interp, (char *)sqlite3TestErrorName(rc), TCL_VOLATILE);
  }

  return (rc==SQLITE_OK ? TCL_OK : TCL_ERROR);
}

static int test_blob_reopen(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  Tcl_WideInt iRowid;
  sqlite3_blob *pBlob;
  int rc;

  if( objc!=3 ){
    Tcl_WrongNumArgs(interp, 1, objv, "CHANNEL ROWID");
    return TCL_ERROR;
  }

  if( blobHandleFromObj(interp, objv[1], &pBlob) ) return TCL_ERROR;
  if( Tcl_GetWideIntFromObj(interp, objv[2], &iRowid) ) return TCL_ERROR;

  rc = sqlite3_blob_reopen(pBlob, iRowid);
  if( rc!=SQLITE_OK ){
    Tcl_SetResult(interp, (char *)sqlite3TestErrorName(rc), TCL_VOLATILE);
  }

  return (rc==SQLITE_OK ? TCL_OK : TCL_ERROR);
}

#endif

struct TestCollationX {
  Tcl_Interp *interp;
  Tcl_Obj *pCmp;
  Tcl_Obj *pDel;
};
typedef struct TestCollationX TestCollationX;
static void testCreateCollationDel(void *pCtx){
  TestCollationX *p = (TestCollationX *)pCtx;

  int rc = Tcl_EvalObjEx(p->interp, p->pDel, TCL_EVAL_DIRECT|TCL_EVAL_GLOBAL);
  if( rc!=TCL_OK ){
    Tcl_BackgroundError(p->interp);
  }

  Tcl_DecrRefCount(p->pCmp);
  Tcl_DecrRefCount(p->pDel);
  sqlite3_free((void *)p);
}
static int testCreateCollationCmp(
  void *pCtx,
  int nLeft,
  const void *zLeft,
  int nRight,
  const void *zRight
){
  TestCollationX *p = (TestCollationX *)pCtx;
  Tcl_Obj *pScript = Tcl_DuplicateObj(p->pCmp);
  int iRes = 0;

  Tcl_IncrRefCount(pScript);
  Tcl_ListObjAppendElement(0, pScript, Tcl_NewStringObj((char *)zLeft, nLeft));
  Tcl_ListObjAppendElement(0, pScript, Tcl_NewStringObj((char *)zRight,nRight));

  if( TCL_OK!=Tcl_EvalObjEx(p->interp, pScript, TCL_EVAL_DIRECT|TCL_EVAL_GLOBAL)
   || TCL_OK!=Tcl_GetIntFromObj(p->interp, Tcl_GetObjResult(p->interp), &iRes)
  ){
    Tcl_BackgroundError(p->interp);
  }
  Tcl_DecrRefCount(pScript);

  return iRes;
}
static int test_create_collation_v2(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  TestCollationX *p;
  sqlite3 *db;
  int rc;

  if( objc!=5 ){
    Tcl_WrongNumArgs(interp, 1, objv, "DB-HANDLE NAME CMP-PROC DEL-PROC");
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;

  p = (TestCollationX *)sqlite3_malloc(sizeof(TestCollationX));
  p->pCmp = objv[3];
  p->pDel = objv[4];
  p->interp = interp;
  Tcl_IncrRefCount(p->pCmp);
  Tcl_IncrRefCount(p->pDel);

  rc = sqlite3_create_collation_v2(db, Tcl_GetString(objv[2]), 16, 
      (void *)p, testCreateCollationCmp, testCreateCollationDel
  );
  if( rc!=SQLITE_MISUSE ){
    Tcl_AppendResult(interp, "sqlite3_create_collate_v2() failed to detect "
      "an invalid encoding", (char*)0);
    return TCL_ERROR;
  }
  rc = sqlite3_create_collation_v2(db, Tcl_GetString(objv[2]), SQLITE_UTF8, 
      (void *)p, testCreateCollationCmp, testCreateCollationDel
  );
  return TCL_OK;
}

typedef struct CreateFunctionV2 CreateFunctionV2;
struct CreateFunctionV2 {
  Tcl_Interp *interp;
  Tcl_Obj *pFunc;                 
  Tcl_Obj *pStep;                 
  Tcl_Obj *pFinal;                
  Tcl_Obj *pDestroy;              
};
static void cf2Func(sqlite3_context *ctx, int nArg, sqlite3_value **aArg){
}
static void cf2Step(sqlite3_context *ctx, int nArg, sqlite3_value **aArg){
}
static void cf2Final(sqlite3_context *ctx){
}
static void cf2Destroy(void *pUser){
  CreateFunctionV2 *p = (CreateFunctionV2 *)pUser;

  if( p->interp && p->pDestroy ){
    int rc = Tcl_EvalObjEx(p->interp, p->pDestroy, 0);
    if( rc!=TCL_OK ) Tcl_BackgroundError(p->interp);
  }

  if( p->pFunc ) Tcl_DecrRefCount(p->pFunc); 
  if( p->pStep ) Tcl_DecrRefCount(p->pStep); 
  if( p->pFinal ) Tcl_DecrRefCount(p->pFinal); 
  if( p->pDestroy ) Tcl_DecrRefCount(p->pDestroy); 
  sqlite3_free(p);
}
static int test_create_function_v2(
  ClientData clientData,          
  Tcl_Interp *interp,             
  int objc,                       
  Tcl_Obj *CONST objv[]           
){
  sqlite3 *db;
  const char *zFunc;
  int nArg;
  int enc;
  CreateFunctionV2 *p;
  int i;
  int rc;

  struct EncTable {
    const char *zEnc;
    int enc;
  } aEnc[] = {
    {"utf8",    SQLITE_UTF8 },
    {"utf16",   SQLITE_UTF16 },
    {"utf16le", SQLITE_UTF16LE },
    {"utf16be", SQLITE_UTF16BE },
    {"any",     SQLITE_ANY },
    {"0", 0 }
  };

  if( objc<5 || (objc%2)==0 ){
    Tcl_WrongNumArgs(interp, 1, objv, "DB NAME NARG ENC SWITCHES...");
    return TCL_ERROR;
  }

  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;
  zFunc = Tcl_GetString(objv[2]);
  if( Tcl_GetIntFromObj(interp, objv[3], &nArg) ) return TCL_ERROR;
  if( Tcl_GetIndexFromObjStruct(interp, objv[4], aEnc, sizeof(aEnc[0]), 
          "encoding", 0, &enc)
  ){
    return TCL_ERROR;
  }
  enc = aEnc[enc].enc;

  p = sqlite3_malloc(sizeof(CreateFunctionV2));
  assert( p );
  memset(p, 0, sizeof(CreateFunctionV2));
  p->interp = interp;

  for(i=5; i<objc; i+=2){
    int iSwitch;
    const char *azSwitch[] = {"-func", "-step", "-final", "-destroy", 0};
    if( Tcl_GetIndexFromObj(interp, objv[i], azSwitch, "switch", 0, &iSwitch) ){
      sqlite3_free(p);
      return TCL_ERROR;
    }

    switch( iSwitch ){
      case 0: p->pFunc = objv[i+1];      break;
      case 1: p->pStep = objv[i+1];      break;
      case 2: p->pFinal = objv[i+1];     break;
      case 3: p->pDestroy = objv[i+1];   break;
    }
  }
  if( p->pFunc ) p->pFunc = Tcl_DuplicateObj(p->pFunc); 
  if( p->pStep ) p->pStep = Tcl_DuplicateObj(p->pStep); 
  if( p->pFinal ) p->pFinal = Tcl_DuplicateObj(p->pFinal); 
  if( p->pDestroy ) p->pDestroy = Tcl_DuplicateObj(p->pDestroy); 

  if( p->pFunc ) Tcl_IncrRefCount(p->pFunc); 
  if( p->pStep ) Tcl_IncrRefCount(p->pStep); 
  if( p->pFinal ) Tcl_IncrRefCount(p->pFinal); 
  if( p->pDestroy ) Tcl_IncrRefCount(p->pDestroy); 

  rc = sqlite3_create_function_v2(db, zFunc, nArg, enc, (void *)p, 
      (p->pFunc ? cf2Func : 0),
      (p->pStep ? cf2Step : 0),
      (p->pFinal ? cf2Final : 0),
      cf2Destroy
  );
  if( rc!=SQLITE_OK ){
    Tcl_ResetResult(interp);
    Tcl_AppendResult(interp, sqlite3TestErrorName(rc), 0);
    return TCL_ERROR;
  }
  return TCL_OK;
}

static int test_load_extension(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  Tcl_CmdInfo cmdInfo;
  sqlite3 *db;
  int rc;
  char *zDb;
  char *zFile;
  char *zProc = 0;
  char *zErr = 0;

  if( objc!=4 && objc!=3 ){
    Tcl_WrongNumArgs(interp, 1, objv, "DB-HANDLE FILE ?PROC?");
    return TCL_ERROR;
  }
  zDb = Tcl_GetString(objv[1]);
  zFile = Tcl_GetString(objv[2]);
  if( objc==4 ){
    zProc = Tcl_GetString(objv[3]);
  }

  
  if( !Tcl_GetCommandInfo(interp, zDb, &cmdInfo) ){
    Tcl_AppendResult(interp, "command not found: ", zDb, (char*)0);
    return TCL_ERROR;
  }
  db = ((struct SqliteDb*)cmdInfo.objClientData)->db;
  assert(db);

#ifdef SQLITE_OMIT_LOAD_EXTENSION
  rc = SQLITE_ERROR;
  zErr = sqlite3_mprintf("this build omits sqlite3_load_extension()");
#else
  rc = sqlite3_load_extension(db, zFile, zProc, &zErr);
#endif
  if( rc!=SQLITE_OK ){
    Tcl_SetResult(interp, zErr ? zErr : "", TCL_VOLATILE);
    rc = TCL_ERROR;
  }else{
    rc = TCL_OK;
  }
  sqlite3_free(zErr);

  return rc;
}

static int test_enable_load(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  Tcl_CmdInfo cmdInfo;
  sqlite3 *db;
  char *zDb;
  int onoff;

  if( objc!=3 ){
    Tcl_WrongNumArgs(interp, 1, objv, "DB-HANDLE ONOFF");
    return TCL_ERROR;
  }
  zDb = Tcl_GetString(objv[1]);

  
  if( !Tcl_GetCommandInfo(interp, zDb, &cmdInfo) ){
    Tcl_AppendResult(interp, "command not found: ", zDb, (char*)0);
    return TCL_ERROR;
  }
  db = ((struct SqliteDb*)cmdInfo.objClientData)->db;
  assert(db);

  
  if( Tcl_GetBooleanFromObj(interp, objv[2], &onoff) ){
    return TCL_ERROR;
  }

#ifdef SQLITE_OMIT_LOAD_EXTENSION
  Tcl_AppendResult(interp, "this build omits sqlite3_load_extension()");
  return TCL_ERROR;
#else
  sqlite3_enable_load_extension(db, onoff);
  return TCL_OK;
#endif
}

static int sqlite_abort(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
#if defined(_MSC_VER)
  _set_abort_behavior( 0, _CALL_REPORTFAULT );
#endif
  exit(255);
  assert( interp==0 );   
  return TCL_OK;
}

static void testFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
  while( argc>=2 ){
    const char *zArg0 = (char*)sqlite3_value_text(argv[0]);
    if( zArg0 ){
      if( 0==sqlite3StrICmp(zArg0, "int") ){
        sqlite3_result_int(context, sqlite3_value_int(argv[1]));
      }else if( sqlite3StrICmp(zArg0,"int64")==0 ){
        sqlite3_result_int64(context, sqlite3_value_int64(argv[1]));
      }else if( sqlite3StrICmp(zArg0,"string")==0 ){
        sqlite3_result_text(context, (char*)sqlite3_value_text(argv[1]), -1,
            SQLITE_TRANSIENT);
      }else if( sqlite3StrICmp(zArg0,"double")==0 ){
        sqlite3_result_double(context, sqlite3_value_double(argv[1]));
      }else if( sqlite3StrICmp(zArg0,"null")==0 ){
        sqlite3_result_null(context);
      }else if( sqlite3StrICmp(zArg0,"value")==0 ){
        sqlite3_result_value(context, argv[sqlite3_value_int(argv[1])]);
      }else{
        goto error_out;
      }
    }else{
      goto error_out;
    }
    argc -= 2;
    argv += 2;
  }
  return;

error_out:
  sqlite3_result_error(context,"first argument should be one of: "
      "int int64 string double null value", -1);
}

static int test_register_func(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  sqlite3 *db;
  int rc;
  if( argc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0], 
       " DB FUNCTION-NAME", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, argv[1], &db) ) return TCL_ERROR;
  rc = sqlite3_create_function(db, argv[2], -1, SQLITE_UTF8, 0, 
      testFunc, 0, 0);
  if( rc!=0 ){
    Tcl_AppendResult(interp, sqlite3ErrStr(rc), 0);
    return TCL_ERROR;
  }
  if( sqlite3TestErrCode(interp, db, rc) ) return TCL_ERROR;
  return TCL_OK;
}

static int test_finalize(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;
  int rc;
  sqlite3 *db = 0;

  if( objc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"",
        Tcl_GetStringFromObj(objv[0], 0), " <STMT>", 0);
    return TCL_ERROR;
  }

  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;

  if( pStmt ){
    db = StmtToDb(pStmt);
  }
  rc = sqlite3_finalize(pStmt);
  Tcl_SetResult(interp, (char *)t1ErrorName(rc), TCL_STATIC);
  if( db && sqlite3TestErrCode(interp, db, rc) ) return TCL_ERROR;
  return TCL_OK;
}

static int test_stmt_status(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  int iValue;
  int i, op, resetFlag;
  const char *zOpName;
  sqlite3_stmt *pStmt;

  static const struct {
    const char *zName;
    int op;
  } aOp[] = {
    { "SQLITE_STMTSTATUS_FULLSCAN_STEP",   SQLITE_STMTSTATUS_FULLSCAN_STEP   },
    { "SQLITE_STMTSTATUS_SORT",            SQLITE_STMTSTATUS_SORT            },
    { "SQLITE_STMTSTATUS_AUTOINDEX",       SQLITE_STMTSTATUS_AUTOINDEX       },
  };
  if( objc!=4 ){
    Tcl_WrongNumArgs(interp, 1, objv, "STMT PARAMETER RESETFLAG");
    return TCL_ERROR;
  }
  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  zOpName = Tcl_GetString(objv[2]);
  for(i=0; i<ArraySize(aOp); i++){
    if( strcmp(aOp[i].zName, zOpName)==0 ){
      op = aOp[i].op;
      break;
    }
  }
  if( i>=ArraySize(aOp) ){
    if( Tcl_GetIntFromObj(interp, objv[2], &op) ) return TCL_ERROR;
  }
  if( Tcl_GetBooleanFromObj(interp, objv[3], &resetFlag) ) return TCL_ERROR;
  iValue = sqlite3_stmt_status(pStmt, op, resetFlag);
  Tcl_SetObjResult(interp, Tcl_NewIntObj(iValue));
  return TCL_OK;
}

static int test_next_stmt(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;
  sqlite3 *db = 0;
  char zBuf[50];

  if( objc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"",
        Tcl_GetStringFromObj(objv[0], 0), " DB STMT", 0);
    return TCL_ERROR;
  }

  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;
  if( getStmtPointer(interp, Tcl_GetString(objv[2]), &pStmt) ) return TCL_ERROR;
  pStmt = sqlite3_next_stmt(db, pStmt);
  if( pStmt ){
    if( sqlite3TestMakePointerStr(interp, zBuf, pStmt) ) return TCL_ERROR;
    Tcl_AppendResult(interp, zBuf, 0);
  }
  return TCL_OK;
}

static int test_stmt_readonly(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;
  int rc;

  if( objc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"",
        Tcl_GetStringFromObj(objv[0], 0), " STMT", 0);
    return TCL_ERROR;
  }

  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  rc = sqlite3_stmt_readonly(pStmt);
  Tcl_SetObjResult(interp, Tcl_NewBooleanObj(rc));
  return TCL_OK;
}


static int test_reset(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;
  int rc;

  if( objc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"",
        Tcl_GetStringFromObj(objv[0], 0), " <STMT>", 0);
    return TCL_ERROR;
  }

  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;

  rc = sqlite3_reset(pStmt);
  if( pStmt && sqlite3TestErrCode(interp, StmtToDb(pStmt), rc) ){
    return TCL_ERROR;
  }
  Tcl_SetResult(interp, (char *)t1ErrorName(rc), TCL_STATIC);
  return TCL_OK;
}

static int test_expired(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
#ifndef SQLITE_OMIT_DEPRECATED
  sqlite3_stmt *pStmt;
  if( objc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"",
        Tcl_GetStringFromObj(objv[0], 0), " <STMT>", 0);
    return TCL_ERROR;
  }
  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  Tcl_SetObjResult(interp, Tcl_NewBooleanObj(sqlite3_expired(pStmt)));
#endif
  return TCL_OK;
}

static int test_transfer_bind(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
#ifndef SQLITE_OMIT_DEPRECATED
  sqlite3_stmt *pStmt1, *pStmt2;
  if( objc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"",
        Tcl_GetStringFromObj(objv[0], 0), " FROM-STMT TO-STMT", 0);
    return TCL_ERROR;
  }
  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt1)) return TCL_ERROR;
  if( getStmtPointer(interp, Tcl_GetString(objv[2]), &pStmt2)) return TCL_ERROR;
  Tcl_SetObjResult(interp, 
     Tcl_NewIntObj(sqlite3_transfer_bindings(pStmt1,pStmt2)));
#endif
  return TCL_OK;
}

static int test_changes(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3 *db;
  if( objc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"",
       Tcl_GetString(objv[0]), " DB", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;
  Tcl_SetObjResult(interp, Tcl_NewIntObj(sqlite3_changes(db)));
  return TCL_OK;
}

static char *sqlite_static_bind_value = 0;
static int sqlite_static_bind_nbyte = 0;

static int test_bind(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  sqlite3_stmt *pStmt;
  int rc;
  int idx;
  if( argc!=5 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0], 
       " VM IDX VALUE (null|static|normal)\"", 0);
    return TCL_ERROR;
  }
  if( getStmtPointer(interp, argv[1], &pStmt) ) return TCL_ERROR;
  if( Tcl_GetInt(interp, argv[2], &idx) ) return TCL_ERROR;
  if( strcmp(argv[4],"null")==0 ){
    rc = sqlite3_bind_null(pStmt, idx);
  }else if( strcmp(argv[4],"static")==0 ){
    rc = sqlite3_bind_text(pStmt, idx, sqlite_static_bind_value, -1, 0);
  }else if( strcmp(argv[4],"static-nbytes")==0 ){
    rc = sqlite3_bind_text(pStmt, idx, sqlite_static_bind_value,
                                       sqlite_static_bind_nbyte, 0);
  }else if( strcmp(argv[4],"normal")==0 ){
    rc = sqlite3_bind_text(pStmt, idx, argv[3], -1, SQLITE_TRANSIENT);
  }else if( strcmp(argv[4],"blob10")==0 ){
    rc = sqlite3_bind_text(pStmt, idx, "abc\000xyz\000pq", 10, SQLITE_STATIC);
  }else{
    Tcl_AppendResult(interp, "4th argument should be "
        "\"null\" or \"static\" or \"normal\"", 0);
    return TCL_ERROR;
  }
  if( sqlite3TestErrCode(interp, StmtToDb(pStmt), rc) ) return TCL_ERROR;
  if( rc ){
    char zBuf[50];
    sprintf(zBuf, "(%d) ", rc);
    Tcl_AppendResult(interp, zBuf, sqlite3ErrStr(rc), 0);
    return TCL_ERROR;
  }
  return TCL_OK;
}

#ifndef SQLITE_OMIT_UTF16
static Tcl_Interp* pTestCollateInterp;
static int test_collate_func(
  void *pCtx, 
  int nA, const void *zA,
  int nB, const void *zB
){
  Tcl_Interp *i = pTestCollateInterp;
  int encin = SQLITE_PTR_TO_INT(pCtx);
  int res;
  int n;

  sqlite3_value *pVal;
  Tcl_Obj *pX;

  pX = Tcl_NewStringObj("test_collate", -1);
  Tcl_IncrRefCount(pX);

  switch( encin ){
    case SQLITE_UTF8:
      Tcl_ListObjAppendElement(i,pX,Tcl_NewStringObj("UTF-8",-1));
      break;
    case SQLITE_UTF16LE:
      Tcl_ListObjAppendElement(i,pX,Tcl_NewStringObj("UTF-16LE",-1));
      break;
    case SQLITE_UTF16BE:
      Tcl_ListObjAppendElement(i,pX,Tcl_NewStringObj("UTF-16BE",-1));
      break;
    default:
      assert(0);
  }

  sqlite3BeginBenignMalloc();
  pVal = sqlite3ValueNew(0);
  if( pVal ){
    sqlite3ValueSetStr(pVal, nA, zA, encin, SQLITE_STATIC);
    n = sqlite3_value_bytes(pVal);
    Tcl_ListObjAppendElement(i,pX,
        Tcl_NewStringObj((char*)sqlite3_value_text(pVal),n));
    sqlite3ValueSetStr(pVal, nB, zB, encin, SQLITE_STATIC);
    n = sqlite3_value_bytes(pVal);
    Tcl_ListObjAppendElement(i,pX,
        Tcl_NewStringObj((char*)sqlite3_value_text(pVal),n));
    sqlite3ValueFree(pVal);
  }
  sqlite3EndBenignMalloc();

  Tcl_EvalObjEx(i, pX, 0);
  Tcl_DecrRefCount(pX);
  Tcl_GetIntFromObj(i, Tcl_GetObjResult(i), &res);
  return res;
}
static int test_collate(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3 *db;
  int val;
  sqlite3_value *pVal;
  int rc;

  if( objc!=5 ) goto bad_args;
  pTestCollateInterp = interp;
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;

  if( TCL_OK!=Tcl_GetBooleanFromObj(interp, objv[2], &val) ) return TCL_ERROR;
  rc = sqlite3_create_collation(db, "test_collate", SQLITE_UTF8, 
          (void *)SQLITE_UTF8, val?test_collate_func:0);
  if( rc==SQLITE_OK ){
    const void *zUtf16;
    if( TCL_OK!=Tcl_GetBooleanFromObj(interp, objv[3], &val) ) return TCL_ERROR;
    rc = sqlite3_create_collation(db, "test_collate", SQLITE_UTF16LE, 
            (void *)SQLITE_UTF16LE, val?test_collate_func:0);
    if( TCL_OK!=Tcl_GetBooleanFromObj(interp, objv[4], &val) ) return TCL_ERROR;

#if 0
    if( sqlite3_iMallocFail>0 ){
      sqlite3_iMallocFail++;
    }
#endif
    sqlite3_mutex_enter(db->mutex);
    pVal = sqlite3ValueNew(db);
    sqlite3ValueSetStr(pVal, -1, "test_collate", SQLITE_UTF8, SQLITE_STATIC);
    zUtf16 = sqlite3ValueText(pVal, SQLITE_UTF16NATIVE);
    if( db->mallocFailed ){
      rc = SQLITE_NOMEM;
    }else{
      rc = sqlite3_create_collation16(db, zUtf16, SQLITE_UTF16BE, 
          (void *)SQLITE_UTF16BE, val?test_collate_func:0);
    }
    sqlite3ValueFree(pVal);
    sqlite3_mutex_leave(db->mutex);
  }
  if( sqlite3TestErrCode(interp, db, rc) ) return TCL_ERROR;
  
  if( rc!=SQLITE_OK ){
    Tcl_AppendResult(interp, sqlite3TestErrorName(rc), 0);
    return TCL_ERROR;
  }
  return TCL_OK;

bad_args:
  Tcl_AppendResult(interp, "wrong # args: should be \"",
      Tcl_GetStringFromObj(objv[0], 0), " <DB> <utf8> <utf16le> <utf16be>", 0);
  return TCL_ERROR;
}

static char zNeededCollation[200];
static char *pzNeededCollation = zNeededCollation;


static void test_collate_needed_cb(
  void *pCtx, 
  sqlite3 *db,
  int eTextRep,
  const void *pName
){
  int enc = ENC(db);
  int i;
  char *z;
  for(z = (char*)pName, i=0; *z || z[1]; z++){
    if( *z ) zNeededCollation[i++] = *z;
  }
  zNeededCollation[i] = 0;
  sqlite3_create_collation(
      db, "test_collate", ENC(db), SQLITE_INT_TO_PTR(enc), test_collate_func);
}

static int test_collate_needed(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3 *db;
  int rc;

  if( objc!=2 ) goto bad_args;
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;
  rc = sqlite3_collation_needed16(db, 0, test_collate_needed_cb);
  zNeededCollation[0] = 0;
  if( sqlite3TestErrCode(interp, db, rc) ) return TCL_ERROR;
  return TCL_OK;

bad_args:
  Tcl_WrongNumArgs(interp, 1, objv, "DB");
  return TCL_ERROR;
}

static int unaligned_string_counter = 0;
static int alignmentCollFunc(
  void *NotUsed,
  int nKey1, const void *pKey1,
  int nKey2, const void *pKey2
){
  int rc, n;
  n = nKey1<nKey2 ? nKey1 : nKey2;
  if( nKey1>0 && 1==(1&(SQLITE_PTR_TO_INT(pKey1))) ) unaligned_string_counter++;
  if( nKey2>0 && 1==(1&(SQLITE_PTR_TO_INT(pKey2))) ) unaligned_string_counter++;
  rc = memcmp(pKey1, pKey2, n);
  if( rc==0 ){
    rc = nKey1 - nKey2;
  }
  return rc;
}
static int add_alignment_test_collations(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3 *db;
  if( objc>=2 ){
    if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;
    sqlite3_create_collation(db, "utf16_unaligned", SQLITE_UTF16, 
        0, alignmentCollFunc);
    sqlite3_create_collation(db, "utf16_aligned", SQLITE_UTF16_ALIGNED, 
        0, alignmentCollFunc);
  }
  return SQLITE_OK;
}
#endif 

#ifndef SQLITE_OMIT_UTF16
static void test_function_utf8(
  sqlite3_context *pCtx, 
  int nArg,
  sqlite3_value **argv
){
  Tcl_Interp *interp;
  Tcl_Obj *pX;
  sqlite3_value *pVal;
  interp = (Tcl_Interp *)sqlite3_user_data(pCtx);
  pX = Tcl_NewStringObj("test_function", -1);
  Tcl_IncrRefCount(pX);
  Tcl_ListObjAppendElement(interp, pX, Tcl_NewStringObj("UTF-8", -1));
  Tcl_ListObjAppendElement(interp, pX, 
      Tcl_NewStringObj((char*)sqlite3_value_text(argv[0]), -1));
  Tcl_EvalObjEx(interp, pX, 0);
  Tcl_DecrRefCount(pX);
  sqlite3_result_text(pCtx, Tcl_GetStringResult(interp), -1, SQLITE_TRANSIENT);
  pVal = sqlite3ValueNew(0);
  sqlite3ValueSetStr(pVal, -1, Tcl_GetStringResult(interp), 
      SQLITE_UTF8, SQLITE_STATIC);
  sqlite3_result_text16be(pCtx, sqlite3_value_text16be(pVal),
      -1, SQLITE_TRANSIENT);
  sqlite3ValueFree(pVal);
}
static void test_function_utf16le(
  sqlite3_context *pCtx, 
  int nArg,
  sqlite3_value **argv
){
  Tcl_Interp *interp;
  Tcl_Obj *pX;
  sqlite3_value *pVal;
  interp = (Tcl_Interp *)sqlite3_user_data(pCtx);
  pX = Tcl_NewStringObj("test_function", -1);
  Tcl_IncrRefCount(pX);
  Tcl_ListObjAppendElement(interp, pX, Tcl_NewStringObj("UTF-16LE", -1));
  Tcl_ListObjAppendElement(interp, pX, 
      Tcl_NewStringObj((char*)sqlite3_value_text(argv[0]), -1));
  Tcl_EvalObjEx(interp, pX, 0);
  Tcl_DecrRefCount(pX);
  pVal = sqlite3ValueNew(0);
  sqlite3ValueSetStr(pVal, -1, Tcl_GetStringResult(interp), 
      SQLITE_UTF8, SQLITE_STATIC);
  sqlite3_result_text(pCtx,(char*)sqlite3_value_text(pVal),-1,SQLITE_TRANSIENT);
  sqlite3ValueFree(pVal);
}
static void test_function_utf16be(
  sqlite3_context *pCtx, 
  int nArg,
  sqlite3_value **argv
){
  Tcl_Interp *interp;
  Tcl_Obj *pX;
  sqlite3_value *pVal;
  interp = (Tcl_Interp *)sqlite3_user_data(pCtx);
  pX = Tcl_NewStringObj("test_function", -1);
  Tcl_IncrRefCount(pX);
  Tcl_ListObjAppendElement(interp, pX, Tcl_NewStringObj("UTF-16BE", -1));
  Tcl_ListObjAppendElement(interp, pX, 
      Tcl_NewStringObj((char*)sqlite3_value_text(argv[0]), -1));
  Tcl_EvalObjEx(interp, pX, 0);
  Tcl_DecrRefCount(pX);
  pVal = sqlite3ValueNew(0);
  sqlite3ValueSetStr(pVal, -1, Tcl_GetStringResult(interp), 
      SQLITE_UTF8, SQLITE_STATIC);
  sqlite3_result_text16(pCtx, sqlite3_value_text16le(pVal),
      -1, SQLITE_TRANSIENT);
  sqlite3_result_text16be(pCtx, sqlite3_value_text16le(pVal),
      -1, SQLITE_TRANSIENT);
  sqlite3_result_text16le(pCtx, sqlite3_value_text16le(pVal),
      -1, SQLITE_TRANSIENT);
  sqlite3ValueFree(pVal);
}
#endif 
static int test_function(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
#ifndef SQLITE_OMIT_UTF16
  sqlite3 *db;
  int val;

  if( objc!=5 ) goto bad_args;
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;

  if( TCL_OK!=Tcl_GetBooleanFromObj(interp, objv[2], &val) ) return TCL_ERROR;
  if( val ){
    sqlite3_create_function(db, "test_function", 1, SQLITE_UTF8, 
        interp, test_function_utf8, 0, 0);
  }
  if( TCL_OK!=Tcl_GetBooleanFromObj(interp, objv[3], &val) ) return TCL_ERROR;
  if( val ){
    sqlite3_create_function(db, "test_function", 1, SQLITE_UTF16LE, 
        interp, test_function_utf16le, 0, 0);
  }
  if( TCL_OK!=Tcl_GetBooleanFromObj(interp, objv[4], &val) ) return TCL_ERROR;
  if( val ){
    sqlite3_create_function(db, "test_function", 1, SQLITE_UTF16BE, 
        interp, test_function_utf16be, 0, 0);
  }

  return TCL_OK;
bad_args:
  Tcl_AppendResult(interp, "wrong # args: should be \"",
      Tcl_GetStringFromObj(objv[0], 0), " <DB> <utf8> <utf16le> <utf16be>", 0);
#endif 
  return TCL_ERROR;
}

static int test_errstr(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  char *zCode;
  int i;
  if( objc!=1 ){
    Tcl_WrongNumArgs(interp, 1, objv, "<error code>");
  }

  zCode = Tcl_GetString(objv[1]);
  for(i=0; i<200; i++){
    if( 0==strcmp(t1ErrorName(i), zCode) ) break;
  }
  Tcl_SetResult(interp, (char *)sqlite3ErrStr(i), 0);
  return TCL_OK;
}

static int test_breakpoint(
  void *NotUsed,
  Tcl_Interp *interp,    
  int argc,              
  char **argv            
){
  return TCL_OK;         
}

static int test_bind_zeroblob(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;
  int idx;
  int n;
  int rc;

  if( objc!=4 ){
    Tcl_WrongNumArgs(interp, 1, objv, "STMT IDX N");
    return TCL_ERROR;
  }

  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  if( Tcl_GetIntFromObj(interp, objv[2], &idx) ) return TCL_ERROR;
  if( Tcl_GetIntFromObj(interp, objv[3], &n) ) return TCL_ERROR;

  rc = sqlite3_bind_zeroblob(pStmt, idx, n);
  if( sqlite3TestErrCode(interp, StmtToDb(pStmt), rc) ) return TCL_ERROR;
  if( rc!=SQLITE_OK ){
    return TCL_ERROR;
  }

  return TCL_OK;
}

static int test_bind_int(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;
  int idx;
  int value;
  int rc;

  if( objc!=4 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"",
        Tcl_GetStringFromObj(objv[0], 0), " STMT N VALUE", 0);
    return TCL_ERROR;
  }

  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  if( Tcl_GetIntFromObj(interp, objv[2], &idx) ) return TCL_ERROR;
  if( Tcl_GetIntFromObj(interp, objv[3], &value) ) return TCL_ERROR;

  rc = sqlite3_bind_int(pStmt, idx, value);
  if( sqlite3TestErrCode(interp, StmtToDb(pStmt), rc) ) return TCL_ERROR;
  if( rc!=SQLITE_OK ){
    return TCL_ERROR;
  }

  return TCL_OK;
}


static int test_bind_int64(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;
  int idx;
  i64 value;
  int rc;

  if( objc!=4 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"",
        Tcl_GetStringFromObj(objv[0], 0), " STMT N VALUE", 0);
    return TCL_ERROR;
  }

  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  if( Tcl_GetIntFromObj(interp, objv[2], &idx) ) return TCL_ERROR;
  if( Tcl_GetWideIntFromObj(interp, objv[3], &value) ) return TCL_ERROR;

  rc = sqlite3_bind_int64(pStmt, idx, value);
  if( sqlite3TestErrCode(interp, StmtToDb(pStmt), rc) ) return TCL_ERROR;
  if( rc!=SQLITE_OK ){
    return TCL_ERROR;
  }

  return TCL_OK;
}


static int test_bind_double(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;
  int idx;
  double value;
  int rc;
  const char *zVal;
  int i;
  static const struct {
    const char *zName;     
    unsigned int iUpper;   
    unsigned int iLower;   
  } aSpecialFp[] = {
    {  "NaN",      0x7fffffff, 0xffffffff },
    {  "SNaN",     0x7ff7ffff, 0xffffffff },
    {  "-NaN",     0xffffffff, 0xffffffff },
    {  "-SNaN",    0xfff7ffff, 0xffffffff },
    {  "+Inf",     0x7ff00000, 0x00000000 },
    {  "-Inf",     0xfff00000, 0x00000000 },
    {  "Epsilon",  0x00000000, 0x00000001 },
    {  "-Epsilon", 0x80000000, 0x00000001 },
    {  "NaN0",     0x7ff80000, 0x00000000 },
    {  "-NaN0",    0xfff80000, 0x00000000 },
  };

  if( objc!=4 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"",
        Tcl_GetStringFromObj(objv[0], 0), " STMT N VALUE", 0);
    return TCL_ERROR;
  }

  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  if( Tcl_GetIntFromObj(interp, objv[2], &idx) ) return TCL_ERROR;

  zVal = Tcl_GetString(objv[3]);
  for(i=0; i<sizeof(aSpecialFp)/sizeof(aSpecialFp[0]); i++){
    if( strcmp(aSpecialFp[i].zName, zVal)==0 ){
      sqlite3_uint64 x;
      x = aSpecialFp[i].iUpper;
      x <<= 32;
      x |= aSpecialFp[i].iLower;
      assert( sizeof(value)==8 );
      assert( sizeof(x)==8 );
      memcpy(&value, &x, 8);
      break;
    }
  }
  if( i>=sizeof(aSpecialFp)/sizeof(aSpecialFp[0]) &&
         Tcl_GetDoubleFromObj(interp, objv[3], &value) ){
    return TCL_ERROR;
  }
  rc = sqlite3_bind_double(pStmt, idx, value);
  if( sqlite3TestErrCode(interp, StmtToDb(pStmt), rc) ) return TCL_ERROR;
  if( rc!=SQLITE_OK ){
    return TCL_ERROR;
  }

  return TCL_OK;
}

static int test_bind_null(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;
  int idx;
  int rc;

  if( objc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"",
        Tcl_GetStringFromObj(objv[0], 0), " STMT N", 0);
    return TCL_ERROR;
  }

  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  if( Tcl_GetIntFromObj(interp, objv[2], &idx) ) return TCL_ERROR;

  rc = sqlite3_bind_null(pStmt, idx);
  if( sqlite3TestErrCode(interp, StmtToDb(pStmt), rc) ) return TCL_ERROR;
  if( rc!=SQLITE_OK ){
    return TCL_ERROR;
  }

  return TCL_OK;
}

static int test_bind_text(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;
  int idx;
  int bytes;
  char *value;
  int rc;

  if( objc!=5 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"",
        Tcl_GetStringFromObj(objv[0], 0), " STMT N VALUE BYTES", 0);
    return TCL_ERROR;
  }

  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  if( Tcl_GetIntFromObj(interp, objv[2], &idx) ) return TCL_ERROR;
  value = (char*)Tcl_GetByteArrayFromObj(objv[3], &bytes);
  if( Tcl_GetIntFromObj(interp, objv[4], &bytes) ) return TCL_ERROR;

  rc = sqlite3_bind_text(pStmt, idx, value, bytes, SQLITE_TRANSIENT);
  if( sqlite3TestErrCode(interp, StmtToDb(pStmt), rc) ) return TCL_ERROR;
  if( rc!=SQLITE_OK ){
    Tcl_AppendResult(interp, sqlite3TestErrorName(rc), 0);
    return TCL_ERROR;
  }

  return TCL_OK;
}

static int test_bind_text16(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
#ifndef SQLITE_OMIT_UTF16
  sqlite3_stmt *pStmt;
  int idx;
  int bytes;
  char *value;
  int rc;

  void (*xDel)() = (objc==6?SQLITE_STATIC:SQLITE_TRANSIENT);
  Tcl_Obj *oStmt    = objv[objc-4];
  Tcl_Obj *oN       = objv[objc-3];
  Tcl_Obj *oString  = objv[objc-2];
  Tcl_Obj *oBytes   = objv[objc-1];

  if( objc!=5 && objc!=6){
    Tcl_AppendResult(interp, "wrong # args: should be \"",
        Tcl_GetStringFromObj(objv[0], 0), " STMT N VALUE BYTES", 0);
    return TCL_ERROR;
  }

  if( getStmtPointer(interp, Tcl_GetString(oStmt), &pStmt) ) return TCL_ERROR;
  if( Tcl_GetIntFromObj(interp, oN, &idx) ) return TCL_ERROR;
  value = (char*)Tcl_GetByteArrayFromObj(oString, 0);
  if( Tcl_GetIntFromObj(interp, oBytes, &bytes) ) return TCL_ERROR;

  rc = sqlite3_bind_text16(pStmt, idx, (void *)value, bytes, xDel);
  if( sqlite3TestErrCode(interp, StmtToDb(pStmt), rc) ) return TCL_ERROR;
  if( rc!=SQLITE_OK ){
    Tcl_AppendResult(interp, sqlite3TestErrorName(rc), 0);
    return TCL_ERROR;
  }

#endif 
  return TCL_OK;
}

static int test_bind_blob(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;
  int idx;
  int bytes;
  char *value;
  int rc;
  sqlite3_destructor_type xDestructor = SQLITE_TRANSIENT;

  if( objc!=5 && objc!=6 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"",
        Tcl_GetStringFromObj(objv[0], 0), " STMT N DATA BYTES", 0);
    return TCL_ERROR;
  }

  if( objc==6 ){
    xDestructor = SQLITE_STATIC;
    objv++;
  }

  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  if( Tcl_GetIntFromObj(interp, objv[2], &idx) ) return TCL_ERROR;
  value = Tcl_GetString(objv[3]);
  if( Tcl_GetIntFromObj(interp, objv[4], &bytes) ) return TCL_ERROR;

  rc = sqlite3_bind_blob(pStmt, idx, value, bytes, xDestructor);
  if( sqlite3TestErrCode(interp, StmtToDb(pStmt), rc) ) return TCL_ERROR;
  if( rc!=SQLITE_OK ){
    return TCL_ERROR;
  }

  return TCL_OK;
}

static int test_bind_parameter_count(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;

  if( objc!=2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "STMT");
    return TCL_ERROR;
  }
  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  Tcl_SetObjResult(interp, Tcl_NewIntObj(sqlite3_bind_parameter_count(pStmt)));
  return TCL_OK;
}

static int test_bind_parameter_name(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;
  int i;

  if( objc!=3 ){
    Tcl_WrongNumArgs(interp, 1, objv, "STMT N");
    return TCL_ERROR;
  }
  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  if( Tcl_GetIntFromObj(interp, objv[2], &i) ) return TCL_ERROR;
  Tcl_SetObjResult(interp, 
     Tcl_NewStringObj(sqlite3_bind_parameter_name(pStmt,i),-1)
  );
  return TCL_OK;
}

static int test_bind_parameter_index(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;

  if( objc!=3 ){
    Tcl_WrongNumArgs(interp, 1, objv, "STMT NAME");
    return TCL_ERROR;
  }
  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  Tcl_SetObjResult(interp, 
     Tcl_NewIntObj(
       sqlite3_bind_parameter_index(pStmt,Tcl_GetString(objv[2]))
     )
  );
  return TCL_OK;
}

static int test_clear_bindings(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;

  if( objc!=2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "STMT");
    return TCL_ERROR;
  }
  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  Tcl_SetObjResult(interp, Tcl_NewIntObj(sqlite3_clear_bindings(pStmt)));
  return TCL_OK;
}

static int test_sleep(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  int ms;

  if( objc!=2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "MILLISECONDS");
    return TCL_ERROR;
  }
  if( Tcl_GetIntFromObj(interp, objv[1], &ms) ){
    return TCL_ERROR;
  }
  Tcl_SetObjResult(interp, Tcl_NewIntObj(sqlite3_sleep(ms)));
  return TCL_OK;
}

static int test_ex_errcode(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3 *db;
  int rc;

  if( objc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", 
       Tcl_GetString(objv[0]), " DB", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;
  rc = sqlite3_extended_errcode(db);
  Tcl_AppendResult(interp, (char *)t1ErrorName(rc), 0);
  return TCL_OK;
}


static int test_errcode(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3 *db;
  int rc;

  if( objc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", 
       Tcl_GetString(objv[0]), " DB", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;
  rc = sqlite3_errcode(db);
  Tcl_AppendResult(interp, (char *)t1ErrorName(rc), 0);
  return TCL_OK;
}

static int test_errmsg(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3 *db;
  const char *zErr;

  if( objc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", 
       Tcl_GetString(objv[0]), " DB", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;

  zErr = sqlite3_errmsg(db);
  Tcl_SetObjResult(interp, Tcl_NewStringObj(zErr, -1));
  return TCL_OK;
}

static int test_errmsg16(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
#ifndef SQLITE_OMIT_UTF16
  sqlite3 *db;
  const void *zErr;
  const char *z;
  int bytes = 0;

  if( objc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", 
       Tcl_GetString(objv[0]), " DB", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;

  zErr = sqlite3_errmsg16(db);
  if( zErr ){
    z = zErr;
    for(bytes=0; z[bytes] || z[bytes+1]; bytes+=2){}
  }
  Tcl_SetObjResult(interp, Tcl_NewByteArrayObj(zErr, bytes));
#endif 
  return TCL_OK;
}

static int test_prepare(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3 *db;
  const char *zSql;
  int bytes;
  const char *zTail = 0;
  sqlite3_stmt *pStmt = 0;
  char zBuf[50];
  int rc;

  if( objc!=5 && objc!=4 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", 
       Tcl_GetString(objv[0]), " DB sql bytes ?tailvar?", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;
  zSql = Tcl_GetString(objv[2]);
  if( Tcl_GetIntFromObj(interp, objv[3], &bytes) ) return TCL_ERROR;

  rc = sqlite3_prepare(db, zSql, bytes, &pStmt, objc>=5 ? &zTail : 0);
  Tcl_ResetResult(interp);
  if( sqlite3TestErrCode(interp, db, rc) ) return TCL_ERROR;
  if( zTail && objc>=5 ){
    if( bytes>=0 ){
      bytes = bytes - (zTail-zSql);
    }
    if( strlen(zTail)<bytes ){
      bytes = strlen(zTail);
    }
    Tcl_ObjSetVar2(interp, objv[4], 0, Tcl_NewStringObj(zTail, bytes), 0);
  }
  if( rc!=SQLITE_OK ){
    assert( pStmt==0 );
    sprintf(zBuf, "(%d) ", rc);
    Tcl_AppendResult(interp, zBuf, sqlite3_errmsg(db), 0);
    return TCL_ERROR;
  }

  if( pStmt ){
    if( sqlite3TestMakePointerStr(interp, zBuf, pStmt) ) return TCL_ERROR;
    Tcl_AppendResult(interp, zBuf, 0);
  }
  return TCL_OK;
}

static int test_prepare_v2(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3 *db;
  const char *zSql;
  int bytes;
  const char *zTail = 0;
  sqlite3_stmt *pStmt = 0;
  char zBuf[50];
  int rc;

  if( objc!=5 && objc!=4 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", 
       Tcl_GetString(objv[0]), " DB sql bytes tailvar", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;
  zSql = Tcl_GetString(objv[2]);
  if( Tcl_GetIntFromObj(interp, objv[3], &bytes) ) return TCL_ERROR;

  rc = sqlite3_prepare_v2(db, zSql, bytes, &pStmt, objc>=5 ? &zTail : 0);
  assert(rc==SQLITE_OK || pStmt==0);
  Tcl_ResetResult(interp);
  if( sqlite3TestErrCode(interp, db, rc) ) return TCL_ERROR;
  if( zTail && objc>=5 ){
    if( bytes>=0 ){
      bytes = bytes - (zTail-zSql);
    }
    Tcl_ObjSetVar2(interp, objv[4], 0, Tcl_NewStringObj(zTail, bytes), 0);
  }
  if( rc!=SQLITE_OK ){
    assert( pStmt==0 );
    sprintf(zBuf, "(%d) ", rc);
    Tcl_AppendResult(interp, zBuf, sqlite3_errmsg(db), 0);
    return TCL_ERROR;
  }

  if( pStmt ){
    if( sqlite3TestMakePointerStr(interp, zBuf, pStmt) ) return TCL_ERROR;
    Tcl_AppendResult(interp, zBuf, 0);
  }
  return TCL_OK;
}

static int test_prepare_tkt3134(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3 *db;
  static const char zSql[] = "\000SELECT 1";
  sqlite3_stmt *pStmt = 0;
  char zBuf[50];
  int rc;

  if( objc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", 
       Tcl_GetString(objv[0]), " DB sql bytes tailvar", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;
  rc = sqlite3_prepare_v2(db, &zSql[1], 0, &pStmt, 0);
  assert(rc==SQLITE_OK || pStmt==0);
  if( sqlite3TestErrCode(interp, db, rc) ) return TCL_ERROR;
  if( rc!=SQLITE_OK ){
    assert( pStmt==0 );
    sprintf(zBuf, "(%d) ", rc);
    Tcl_AppendResult(interp, zBuf, sqlite3_errmsg(db), 0);
    return TCL_ERROR;
  }

  if( pStmt ){
    if( sqlite3TestMakePointerStr(interp, zBuf, pStmt) ) return TCL_ERROR;
    Tcl_AppendResult(interp, zBuf, 0);
  }
  return TCL_OK;
}

static int test_prepare16(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
#ifndef SQLITE_OMIT_UTF16
  sqlite3 *db;
  const void *zSql;
  const void *zTail = 0;
  Tcl_Obj *pTail = 0;
  sqlite3_stmt *pStmt = 0;
  char zBuf[50]; 
  int rc;
  int bytes;                
  int objlen;               

  if( objc!=5 && objc!=4 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", 
       Tcl_GetString(objv[0]), " DB sql bytes ?tailvar?", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;
  zSql = Tcl_GetByteArrayFromObj(objv[2], &objlen);
  if( Tcl_GetIntFromObj(interp, objv[3], &bytes) ) return TCL_ERROR;

  rc = sqlite3_prepare16(db, zSql, bytes, &pStmt, objc>=5 ? &zTail : 0);
  if( sqlite3TestErrCode(interp, db, rc) ) return TCL_ERROR;
  if( rc ){
    return TCL_ERROR;
  }

  if( objc>=5 ){
    if( zTail ){
      objlen = objlen - ((u8 *)zTail-(u8 *)zSql);
    }else{
      objlen = 0;
    }
    pTail = Tcl_NewByteArrayObj((u8 *)zTail, objlen);
    Tcl_IncrRefCount(pTail);
    Tcl_ObjSetVar2(interp, objv[4], 0, pTail, 0);
    Tcl_DecrRefCount(pTail);
  }

  if( pStmt ){
    if( sqlite3TestMakePointerStr(interp, zBuf, pStmt) ) return TCL_ERROR;
  }
  Tcl_AppendResult(interp, zBuf, 0);
#endif 
  return TCL_OK;
}

static int test_prepare16_v2(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
#ifndef SQLITE_OMIT_UTF16
  sqlite3 *db;
  const void *zSql;
  const void *zTail = 0;
  Tcl_Obj *pTail = 0;
  sqlite3_stmt *pStmt = 0;
  char zBuf[50]; 
  int rc;
  int bytes;                
  int objlen;               

  if( objc!=5 && objc!=4 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", 
       Tcl_GetString(objv[0]), " DB sql bytes ?tailvar?", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;
  zSql = Tcl_GetByteArrayFromObj(objv[2], &objlen);
  if( Tcl_GetIntFromObj(interp, objv[3], &bytes) ) return TCL_ERROR;

  rc = sqlite3_prepare16_v2(db, zSql, bytes, &pStmt, objc>=5 ? &zTail : 0);
  if( sqlite3TestErrCode(interp, db, rc) ) return TCL_ERROR;
  if( rc ){
    return TCL_ERROR;
  }

  if( objc>=5 ){
    if( zTail ){
      objlen = objlen - ((u8 *)zTail-(u8 *)zSql);
    }else{
      objlen = 0;
    }
    pTail = Tcl_NewByteArrayObj((u8 *)zTail, objlen);
    Tcl_IncrRefCount(pTail);
    Tcl_ObjSetVar2(interp, objv[4], 0, pTail, 0);
    Tcl_DecrRefCount(pTail);
  }

  if( pStmt ){
    if( sqlite3TestMakePointerStr(interp, zBuf, pStmt) ) return TCL_ERROR;
  }
  Tcl_AppendResult(interp, zBuf, 0);
#endif 
  return TCL_OK;
}

static int test_open(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  const char *zFilename;
  sqlite3 *db;
  int rc;
  char zBuf[100];

  if( objc!=3 && objc!=2 && objc!=1 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", 
       Tcl_GetString(objv[0]), " filename options-list", 0);
    return TCL_ERROR;
  }

  zFilename = objc>1 ? Tcl_GetString(objv[1]) : 0;
  rc = sqlite3_open(zFilename, &db);
  
  if( sqlite3TestMakePointerStr(interp, zBuf, db) ) return TCL_ERROR;
  Tcl_AppendResult(interp, zBuf, 0);
  return TCL_OK;
}

static int test_open16(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
#ifndef SQLITE_OMIT_UTF16
  const void *zFilename;
  sqlite3 *db;
  int rc;
  char zBuf[100];

  if( objc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", 
       Tcl_GetString(objv[0]), " filename options-list", 0);
    return TCL_ERROR;
  }

  zFilename = Tcl_GetByteArrayFromObj(objv[1], 0);
  rc = sqlite3_open16(zFilename, &db);
  
  if( sqlite3TestMakePointerStr(interp, zBuf, db) ) return TCL_ERROR;
  Tcl_AppendResult(interp, zBuf, 0);
#endif 
  return TCL_OK;
}

static int test_complete16(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
#if !defined(SQLITE_OMIT_COMPLETE) && !defined(SQLITE_OMIT_UTF16)
  char *zBuf;

  if( objc!=2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "<utf-16 sql>");
    return TCL_ERROR;
  }

  zBuf = (char*)Tcl_GetByteArrayFromObj(objv[1], 0);
  Tcl_SetObjResult(interp, Tcl_NewIntObj(sqlite3_complete16(zBuf)));
#endif 
  return TCL_OK;
}

static int test_step(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;
  int rc;

  if( objc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", 
       Tcl_GetString(objv[0]), " STMT", 0);
    return TCL_ERROR;
  }

  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  rc = sqlite3_step(pStmt);

  
  Tcl_SetResult(interp, (char *)t1ErrorName(rc), 0);
  return TCL_OK;
}

static int test_sql(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;

  if( objc!=2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "STMT");
    return TCL_ERROR;
  }

  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  Tcl_SetResult(interp, (char *)sqlite3_sql(pStmt), TCL_VOLATILE);
  return TCL_OK;
}

static int test_column_count(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;

  if( objc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", 
       Tcl_GetString(objv[0]), " STMT column", 0);
    return TCL_ERROR;
  }

  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;

  Tcl_SetObjResult(interp, Tcl_NewIntObj(sqlite3_column_count(pStmt)));
  return TCL_OK;
}

static int test_column_type(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;
  int col;
  int tp;

  if( objc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", 
       Tcl_GetString(objv[0]), " STMT column", 0);
    return TCL_ERROR;
  }

  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  if( Tcl_GetIntFromObj(interp, objv[2], &col) ) return TCL_ERROR;

  tp = sqlite3_column_type(pStmt, col);
  switch( tp ){
    case SQLITE_INTEGER: 
      Tcl_SetResult(interp, "INTEGER", TCL_STATIC); 
      break;
    case SQLITE_NULL:
      Tcl_SetResult(interp, "NULL", TCL_STATIC); 
      break;
    case SQLITE_FLOAT:
      Tcl_SetResult(interp, "FLOAT", TCL_STATIC); 
      break;
    case SQLITE_TEXT:
      Tcl_SetResult(interp, "TEXT", TCL_STATIC); 
      break;
    case SQLITE_BLOB:
      Tcl_SetResult(interp, "BLOB", TCL_STATIC); 
      break;
    default:
      assert(0);
  }

  return TCL_OK;
}

static int test_column_int64(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;
  int col;
  i64 iVal;

  if( objc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", 
       Tcl_GetString(objv[0]), " STMT column", 0);
    return TCL_ERROR;
  }

  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  if( Tcl_GetIntFromObj(interp, objv[2], &col) ) return TCL_ERROR;

  iVal = sqlite3_column_int64(pStmt, col);
  Tcl_SetObjResult(interp, Tcl_NewWideIntObj(iVal));
  return TCL_OK;
}

static int test_column_blob(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;
  int col;

  int len;
  const void *pBlob;

  if( objc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", 
       Tcl_GetString(objv[0]), " STMT column", 0);
    return TCL_ERROR;
  }

  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  if( Tcl_GetIntFromObj(interp, objv[2], &col) ) return TCL_ERROR;

  len = sqlite3_column_bytes(pStmt, col);
  pBlob = sqlite3_column_blob(pStmt, col);
  Tcl_SetObjResult(interp, Tcl_NewByteArrayObj(pBlob, len));
  return TCL_OK;
}

static int test_column_double(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;
  int col;
  double rVal;

  if( objc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", 
       Tcl_GetString(objv[0]), " STMT column", 0);
    return TCL_ERROR;
  }

  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  if( Tcl_GetIntFromObj(interp, objv[2], &col) ) return TCL_ERROR;

  rVal = sqlite3_column_double(pStmt, col);
  Tcl_SetObjResult(interp, Tcl_NewDoubleObj(rVal));
  return TCL_OK;
}

static int test_data_count(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;

  if( objc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", 
       Tcl_GetString(objv[0]), " STMT column", 0);
    return TCL_ERROR;
  }

  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;

  Tcl_SetObjResult(interp, Tcl_NewIntObj(sqlite3_data_count(pStmt)));
  return TCL_OK;
}

static int test_stmt_utf8(
  void * clientData,        
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;
  int col;
  const char *(*xFunc)(sqlite3_stmt*, int);
  const char *zRet;

  xFunc = (const char *(*)(sqlite3_stmt*, int))clientData;
  if( objc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", 
       Tcl_GetString(objv[0]), " STMT column", 0);
    return TCL_ERROR;
  }

  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  if( Tcl_GetIntFromObj(interp, objv[2], &col) ) return TCL_ERROR;
  zRet = xFunc(pStmt, col);
  if( zRet ){
    Tcl_SetResult(interp, (char *)zRet, 0);
  }
  return TCL_OK;
}

static int test_global_recover(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
#ifndef SQLITE_OMIT_DEPRECATED
  int rc;
  if( objc!=1 ){
    Tcl_WrongNumArgs(interp, 1, objv, "");
    return TCL_ERROR;
  }
  rc = sqlite3_global_recover();
  Tcl_SetResult(interp, (char *)t1ErrorName(rc), TCL_STATIC);
#endif
  return TCL_OK;
}

static int test_stmt_utf16(
  void * clientData,     
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
#ifndef SQLITE_OMIT_UTF16
  sqlite3_stmt *pStmt;
  int col;
  Tcl_Obj *pRet;
  const void *zName16;
  const void *(*xFunc)(sqlite3_stmt*, int);

  xFunc = (const void *(*)(sqlite3_stmt*, int))clientData;
  if( objc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", 
       Tcl_GetString(objv[0]), " STMT column", 0);
    return TCL_ERROR;
  }

  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  if( Tcl_GetIntFromObj(interp, objv[2], &col) ) return TCL_ERROR;

  zName16 = xFunc(pStmt, col);
  if( zName16 ){
    int n;
    const char *z = zName16;
    for(n=0; z[n] || z[n+1]; n+=2){}
    pRet = Tcl_NewByteArrayObj(zName16, n+2);
    Tcl_SetObjResult(interp, pRet);
  }
#endif 

  return TCL_OK;
}

static int test_stmt_int(
  void * clientData,    
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_stmt *pStmt;
  int col;
  int (*xFunc)(sqlite3_stmt*, int);

  xFunc = (int (*)(sqlite3_stmt*, int))clientData;
  if( objc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", 
       Tcl_GetString(objv[0]), " STMT column", 0);
    return TCL_ERROR;
  }

  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  if( Tcl_GetIntFromObj(interp, objv[2], &col) ) return TCL_ERROR;

  Tcl_SetObjResult(interp, Tcl_NewIntObj(xFunc(pStmt, col)));
  return TCL_OK;
}

static int sqlite_set_magic(
  void * clientData,
  Tcl_Interp *interp,
  int argc,
  char **argv
){
  sqlite3 *db;
  if( argc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
         " DB MAGIC", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, argv[1], &db) ) return TCL_ERROR;
  if( strcmp(argv[2], "SQLITE_MAGIC_OPEN")==0 ){
    db->magic = SQLITE_MAGIC_OPEN;
  }else if( strcmp(argv[2], "SQLITE_MAGIC_CLOSED")==0 ){
    db->magic = SQLITE_MAGIC_CLOSED;
  }else if( strcmp(argv[2], "SQLITE_MAGIC_BUSY")==0 ){
    db->magic = SQLITE_MAGIC_BUSY;
  }else if( strcmp(argv[2], "SQLITE_MAGIC_ERROR")==0 ){
    db->magic = SQLITE_MAGIC_ERROR;
  }else if( Tcl_GetInt(interp, argv[2], (int*)&db->magic) ){
    return TCL_ERROR;
  }
  return TCL_OK;
}

static int test_interrupt(
  void * clientData,
  Tcl_Interp *interp,
  int argc,
  char **argv
){
  sqlite3 *db;
  if( argc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0], " DB", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, argv[1], &db) ) return TCL_ERROR;
  sqlite3_interrupt(db);
  return TCL_OK;
}

static u8 *sqlite3_stack_baseline = 0;

static void prepStack(void){
  int i;
  u32 bigBuf[65536];
  for(i=0; i<sizeof(bigBuf); i++) bigBuf[i] = 0xdeadbeef;
  sqlite3_stack_baseline = (u8*)&bigBuf[65536];
}

u64 sqlite3StackDepth(void){
  u8 x;
  return (u64)(sqlite3_stack_baseline - &x);
}

static int test_stack_used(
  void * clientData,
  Tcl_Interp *interp,
  int argc,
  char **argv
){
  sqlite3 *db;
  int i;
  if( argc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0], 
        " DB SQL", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, argv[1], &db) ) return TCL_ERROR;
  prepStack();
  (void)sqlite3_exec(db, argv[2], 0, 0, 0);
  for(i=65535; i>=0 && ((u32*)sqlite3_stack_baseline)[-i]==0xdeadbeef; i--){}
  Tcl_SetObjResult(interp, Tcl_NewIntObj(i*4));
  return TCL_OK;
}

static int delete_function(
  void * clientData,
  Tcl_Interp *interp,
  int argc,
  char **argv
){
  int rc;
  sqlite3 *db;
  if( argc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0], 
        " DB function-name", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, argv[1], &db) ) return TCL_ERROR;
  rc = sqlite3_create_function(db, argv[2], -1, SQLITE_UTF8, 0, 0, 0, 0);
  Tcl_SetResult(interp, (char *)t1ErrorName(rc), TCL_STATIC);
  return TCL_OK;
}

static int delete_collation(
  void * clientData,
  Tcl_Interp *interp,
  int argc,
  char **argv
){
  int rc;
  sqlite3 *db;
  if( argc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0], 
        " DB function-name", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, argv[1], &db) ) return TCL_ERROR;
  rc = sqlite3_create_collation(db, argv[2], SQLITE_UTF8, 0, 0);
  Tcl_SetResult(interp, (char *)t1ErrorName(rc), TCL_STATIC);
  return TCL_OK;
}

static int get_autocommit(
  void * clientData,
  Tcl_Interp *interp,
  int argc,
  char **argv
){
  char zBuf[30];
  sqlite3 *db;
  if( argc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0], 
        " DB", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, argv[1], &db) ) return TCL_ERROR;
  sprintf(zBuf, "%d", sqlite3_get_autocommit(db));
  Tcl_AppendResult(interp, zBuf, 0);
  return TCL_OK;
}

static int test_busy_timeout(
  void * clientData,
  Tcl_Interp *interp,
  int argc,
  char **argv
){
  int rc, ms;
  sqlite3 *db;
  if( argc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0], 
        " DB", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, argv[1], &db) ) return TCL_ERROR;
  if( Tcl_GetInt(interp, argv[2], &ms) ) return TCL_ERROR;
  rc = sqlite3_busy_timeout(db, ms);
  Tcl_AppendResult(interp, sqlite3TestErrorName(rc), 0);
  return TCL_OK;
}

static int tcl_variable_type(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  Tcl_Obj *pVar;
  if( objc!=2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "VARIABLE");
    return TCL_ERROR;
  }
  pVar = Tcl_GetVar2Ex(interp, Tcl_GetString(objv[1]), 0, TCL_LEAVE_ERR_MSG);
  if( pVar==0 ) return TCL_ERROR;
  if( pVar->typePtr ){
    Tcl_SetObjResult(interp, Tcl_NewStringObj(pVar->typePtr->name, -1));
  }
  return TCL_OK;
}

static int test_release_memory(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
#if defined(SQLITE_ENABLE_MEMORY_MANAGEMENT) && !defined(SQLITE_OMIT_DISKIO)
  int N;
  int amt;
  if( objc!=1 && objc!=2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "?N?");
    return TCL_ERROR;
  }
  if( objc==2 ){
    if( Tcl_GetIntFromObj(interp, objv[1], &N) ) return TCL_ERROR;
  }else{
    N = -1;
  }
  amt = sqlite3_release_memory(N);
  Tcl_SetObjResult(interp, Tcl_NewIntObj(amt));
#endif
  return TCL_OK;
}

static int test_soft_heap_limit(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_int64 amt;
  sqlite3_int64 N = -1;
  if( objc!=1 && objc!=2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "?N?");
    return TCL_ERROR;
  }
  if( objc==2 ){
    if( Tcl_GetWideIntFromObj(interp, objv[1], &N) ) return TCL_ERROR;
  }
  amt = sqlite3_soft_heap_limit64(N);
  Tcl_SetObjResult(interp, Tcl_NewWideIntObj(amt));
  return TCL_OK;
}

static int test_thread_cleanup(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
#ifndef SQLITE_OMIT_DEPRECATED
  sqlite3_thread_cleanup();
#endif
  return TCL_OK;
}

static int test_pager_refcounts(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3 *db;
  int i;
  int v, *a;
  Tcl_Obj *pResult;

  if( objc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"",
        Tcl_GetStringFromObj(objv[0], 0), " DB", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;
  pResult = Tcl_NewObj();
  for(i=0; i<db->nDb; i++){
    if( db->aDb[i].pBt==0 ){
      v = -1;
    }else{
      sqlite3_mutex_enter(db->mutex);
      a = sqlite3PagerStats(sqlite3BtreePager(db->aDb[i].pBt));
      v = a[0];
      sqlite3_mutex_leave(db->mutex);
    }
    Tcl_ListObjAppendElement(0, pResult, Tcl_NewIntObj(v));
  }
  Tcl_SetObjResult(interp, pResult);
  return TCL_OK;
}


static int working_64bit_int(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  Tcl_Obj *pTestObj;
  int working = 0;

  pTestObj = Tcl_NewWideIntObj(1000000*(i64)1234567890);
  working = strcmp(Tcl_GetString(pTestObj), "1234567890000000")==0;
  Tcl_DecrRefCount(pTestObj);
  Tcl_SetObjResult(interp, Tcl_NewBooleanObj(working));
  return TCL_OK;
}


static int vfs_unlink_test(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  int i;
  sqlite3_vfs *pMain;
  sqlite3_vfs *apVfs[20];
  sqlite3_vfs one, two;

  sqlite3_vfs_unregister(0);   
  one.zName = "__one";
  two.zName = "__two";

  pMain = sqlite3_vfs_find(0);
  sqlite3_vfs_register(&one, 0);
  assert( pMain==0 || pMain==sqlite3_vfs_find(0) );
  sqlite3_vfs_register(&two, 0);
  assert( pMain==0 || pMain==sqlite3_vfs_find(0) );

  
  assert( sqlite3_vfs_find("__one")==&one );
  assert( sqlite3_vfs_find("__two")==&two );

  sqlite3_vfs_register(&one, 1);
  assert( sqlite3_vfs_find("__one")==&one );
  assert( sqlite3_vfs_find("__two")==&two );
  assert( sqlite3_vfs_find(0)==&one );
  sqlite3_vfs_register(&two, 1);
  assert( sqlite3_vfs_find("__one")==&one );
  assert( sqlite3_vfs_find("__two")==&two );
  assert( sqlite3_vfs_find(0)==&two );
  if( pMain ){
    sqlite3_vfs_register(pMain, 1);
    assert( sqlite3_vfs_find("__one")==&one );
    assert( sqlite3_vfs_find("__two")==&two );
    assert( sqlite3_vfs_find(0)==pMain );
  }
  
  for(i=0; i<sizeof(apVfs)/sizeof(apVfs[0]); i++){
    apVfs[i] = sqlite3_vfs_find(0);
    if( apVfs[i] ){
      assert( apVfs[i]==sqlite3_vfs_find(apVfs[i]->zName) );
      sqlite3_vfs_unregister(apVfs[i]);
      assert( 0==sqlite3_vfs_find(apVfs[i]->zName) );
    }
  }
  assert( 0==sqlite3_vfs_find(0) );
  
  sqlite3_vfs_register(pMain, 0);
  assert( sqlite3_vfs_find(0)==pMain );
  
  
  sqlite3_vfs_unregister(pMain);
  assert( 0==sqlite3_vfs_find(0) );

    
  for(i=sizeof(apVfs)/sizeof(apVfs[0])-1; i>=0; i--){
    if( apVfs[i] ){
      sqlite3_vfs_register(apVfs[i], 1);
      assert( apVfs[i]==sqlite3_vfs_find(0) );
      assert( apVfs[i]==sqlite3_vfs_find(apVfs[i]->zName) );
    }
  }

  
  sqlite3_vfs_unregister(&one);
  sqlite3_vfs_unregister(&two);

  
  sqlite3_vfs_unregister(&one);
  sqlite3_vfs_unregister(&two);
  assert( sqlite3_vfs_find("__one")==0 );
  assert( sqlite3_vfs_find("__two")==0 );

  assert( sqlite3_vfs_find(0)==pMain );

  return TCL_OK;
}

static int vfs_initfail_test(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  sqlite3_vfs one;
  one.zName = "__one";

  if( sqlite3_vfs_find(0) ) return TCL_ERROR;
  sqlite3_vfs_register(&one, 0);
  if( sqlite3_vfs_find(0) ) return TCL_ERROR;
  sqlite3_vfs_register(&one, 1);
  if( sqlite3_vfs_find(0) ) return TCL_ERROR;
  return TCL_OK;
}

static sqlite3_vfs *apVfs[20];
static int nVfs = 0;

static int vfs_unregister_all(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  int i;
  for(i=0; i<ArraySize(apVfs); i++){
    apVfs[i] = sqlite3_vfs_find(0);
    if( apVfs[i]==0 ) break;
    sqlite3_vfs_unregister(apVfs[i]);
  }
  nVfs = i;
  return TCL_OK;
}
static int vfs_reregister_all(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  int i;
  for(i=0; i<nVfs; i++){
    sqlite3_vfs_register(apVfs[i], i==0);
  }
  return TCL_OK;
}


static int file_control_test(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  int iArg = 0;
  sqlite3 *db;
  int rc;

  if( objc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"",
        Tcl_GetStringFromObj(objv[0], 0), " DB", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;
  rc = sqlite3_file_control(db, 0, 0, &iArg);
  assert( rc==SQLITE_NOTFOUND );
  rc = sqlite3_file_control(db, "notadatabase", SQLITE_FCNTL_LOCKSTATE, &iArg);
  assert( rc==SQLITE_ERROR );
  rc = sqlite3_file_control(db, "main", -1, &iArg);
  assert( rc==SQLITE_NOTFOUND );
  rc = sqlite3_file_control(db, "temp", -1, &iArg);
  assert( rc==SQLITE_NOTFOUND || rc==SQLITE_ERROR );

  return TCL_OK;
}


static int file_control_lasterrno_test(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  int iArg = 0;
  sqlite3 *db;
  int rc;

  if( objc!=2 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"",
        Tcl_GetStringFromObj(objv[0], 0), " DB", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ){
    return TCL_ERROR;
  }
  rc = sqlite3_file_control(db, NULL, SQLITE_LAST_ERRNO, &iArg);
  if( rc ){ 
    Tcl_SetObjResult(interp, Tcl_NewIntObj(rc)); 
    return TCL_ERROR; 
  }
  if( iArg!=0 ) {
    Tcl_AppendResult(interp, "Unexpected non-zero errno: ",
                     Tcl_GetStringFromObj(Tcl_NewIntObj(iArg), 0), " ", 0);
    return TCL_ERROR;
  }
  return TCL_OK;  
}

static int file_control_chunksize_test(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  int nSize;                      
  char *zDb;                      
  sqlite3 *db;                    
  int rc;                         

  if( objc!=4 ){
    Tcl_WrongNumArgs(interp, 1, objv, "DB DBNAME SIZE");
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) 
   || Tcl_GetIntFromObj(interp, objv[3], &nSize)
  ){
   return TCL_ERROR;
  }
  zDb = Tcl_GetString(objv[2]);
  if( zDb[0]=='\0' ) zDb = NULL;

  rc = sqlite3_file_control(db, zDb, SQLITE_FCNTL_CHUNK_SIZE, (void *)&nSize);
  if( rc ){
    Tcl_SetResult(interp, (char *)sqlite3TestErrorName(rc), TCL_STATIC);
    return TCL_ERROR;
  }
  return TCL_OK;
}

static int file_control_sizehint_test(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  sqlite3_int64 nSize;            
  char *zDb;                      
  sqlite3 *db;                    
  int rc;                         

  if( objc!=4 ){
    Tcl_WrongNumArgs(interp, 1, objv, "DB DBNAME SIZE");
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) 
   || Tcl_GetWideIntFromObj(interp, objv[3], &nSize)
  ){
   return TCL_ERROR;
  }
  zDb = Tcl_GetString(objv[2]);
  if( zDb[0]=='\0' ) zDb = NULL;

  rc = sqlite3_file_control(db, zDb, SQLITE_FCNTL_SIZE_HINT, (void *)&nSize);
  if( rc ){
    Tcl_SetResult(interp, (char *)sqlite3TestErrorName(rc), TCL_STATIC);
    return TCL_ERROR;
  }
  return TCL_OK;
}

static int file_control_lockproxy_test(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  sqlite3 *db;
  const char *zPwd;
  int nPwd;
  
  if( objc!=3 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"",
                     Tcl_GetStringFromObj(objv[0], 0), " DB PWD", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ){
   return TCL_ERROR;
  }
  zPwd = Tcl_GetStringFromObj(objv[2], &nPwd);
  
#if !defined(SQLITE_ENABLE_LOCKING_STYLE)
#  if defined(__APPLE__)
#    define SQLITE_ENABLE_LOCKING_STYLE 1
#  else
#    define SQLITE_ENABLE_LOCKING_STYLE 0
#  endif
#endif
#if SQLITE_ENABLE_LOCKING_STYLE && defined(__APPLE__)
  {
    char *testPath;
    int rc;
    char proxyPath[400];
    
    if( sizeof(proxyPath)<nPwd+20 ){
      Tcl_AppendResult(interp, "PWD too big", (void*)0);
      return TCL_ERROR;
    }
    sprintf(proxyPath, "%s/test.proxy", zPwd);
    rc = sqlite3_file_control(db, NULL, SQLITE_SET_LOCKPROXYFILE, proxyPath);
    if( rc ){
      Tcl_SetObjResult(interp, Tcl_NewIntObj(rc)); 
      return TCL_ERROR;
    }
    rc = sqlite3_file_control(db, NULL, SQLITE_GET_LOCKPROXYFILE, &testPath);
    if( strncmp(proxyPath,testPath,11) ){
      Tcl_AppendResult(interp, "Lock proxy file did not match the "
                               "previously assigned value", 0);
      return TCL_ERROR;
    }
    if( rc ){
      Tcl_SetObjResult(interp, Tcl_NewIntObj(rc));
      return TCL_ERROR;
    }
    rc = sqlite3_file_control(db, NULL, SQLITE_SET_LOCKPROXYFILE, proxyPath);
    if( rc ){
      Tcl_SetObjResult(interp, Tcl_NewIntObj(rc));
      return TCL_ERROR;
    }
  }
#endif
  return TCL_OK;  
}


static int vfs_list(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  sqlite3_vfs *pVfs;
  Tcl_Obj *pRet = Tcl_NewObj();
  if( objc!=1 ){
    Tcl_WrongNumArgs(interp, 1, objv, "");
    return TCL_ERROR;
  }
  for(pVfs=sqlite3_vfs_find(0); pVfs; pVfs=pVfs->pNext){
    Tcl_ListObjAppendElement(interp, pRet, Tcl_NewStringObj(pVfs->zName, -1));
  }
  Tcl_SetObjResult(interp, pRet);
  return TCL_OK;  
}

static int test_limit(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  sqlite3 *db;
  int rc;
  static const struct {
     char *zName;
     int id;
  } aId[] = {
    { "SQLITE_LIMIT_LENGTH",              SQLITE_LIMIT_LENGTH               },
    { "SQLITE_LIMIT_SQL_LENGTH",          SQLITE_LIMIT_SQL_LENGTH           },
    { "SQLITE_LIMIT_COLUMN",              SQLITE_LIMIT_COLUMN               },
    { "SQLITE_LIMIT_EXPR_DEPTH",          SQLITE_LIMIT_EXPR_DEPTH           },
    { "SQLITE_LIMIT_COMPOUND_SELECT",     SQLITE_LIMIT_COMPOUND_SELECT      },
    { "SQLITE_LIMIT_VDBE_OP",             SQLITE_LIMIT_VDBE_OP              },
    { "SQLITE_LIMIT_FUNCTION_ARG",        SQLITE_LIMIT_FUNCTION_ARG         },
    { "SQLITE_LIMIT_ATTACHED",            SQLITE_LIMIT_ATTACHED             },
    { "SQLITE_LIMIT_LIKE_PATTERN_LENGTH", SQLITE_LIMIT_LIKE_PATTERN_LENGTH  },
    { "SQLITE_LIMIT_VARIABLE_NUMBER",     SQLITE_LIMIT_VARIABLE_NUMBER      },
    { "SQLITE_LIMIT_TRIGGER_DEPTH",       SQLITE_LIMIT_TRIGGER_DEPTH        },
    
    
    { "SQLITE_LIMIT_TOOSMALL",            -1,                               },
    { "SQLITE_LIMIT_TOOBIG",              SQLITE_LIMIT_TRIGGER_DEPTH+1      },
  };
  int i, id;
  int val;
  const char *zId;

  if( objc!=4 ){
    Tcl_AppendResult(interp, "wrong # args: should be \"",
        Tcl_GetStringFromObj(objv[0], 0), " DB ID VALUE", 0);
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;
  zId = Tcl_GetString(objv[2]);
  for(i=0; i<sizeof(aId)/sizeof(aId[0]); i++){
    if( strcmp(zId, aId[i].zName)==0 ){
      id = aId[i].id;
      break;
    }
  }
  if( i>=sizeof(aId)/sizeof(aId[0]) ){
    Tcl_AppendResult(interp, "unknown limit type: ", zId, (char*)0);
    return TCL_ERROR;
  }
  if( Tcl_GetIntFromObj(interp, objv[3], &val) ) return TCL_ERROR;
  rc = sqlite3_limit(db, id, val);
  Tcl_SetObjResult(interp, Tcl_NewIntObj(rc));
  return TCL_OK;  
}

static int save_prng_state(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  int rc = sqlite3_test_control(9999);
  assert( rc==0 );
  rc = sqlite3_test_control(-1);
  assert( rc==0 );
  sqlite3_test_control(SQLITE_TESTCTRL_PRNG_SAVE);
  return TCL_OK;
}
static int restore_prng_state(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  sqlite3_test_control(SQLITE_TESTCTRL_PRNG_RESTORE);
  return TCL_OK;
}
static int reset_prng_state(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  sqlite3_test_control(SQLITE_TESTCTRL_PRNG_RESET);
  return TCL_OK;
}

static int test_pcache_stats(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  int nMin;
  int nMax;
  int nCurrent;
  int nRecyclable;
  Tcl_Obj *pRet;

  sqlite3PcacheStats(&nCurrent, &nMax, &nMin, &nRecyclable);

  pRet = Tcl_NewObj();
  Tcl_ListObjAppendElement(interp, pRet, Tcl_NewStringObj("current", -1));
  Tcl_ListObjAppendElement(interp, pRet, Tcl_NewIntObj(nCurrent));
  Tcl_ListObjAppendElement(interp, pRet, Tcl_NewStringObj("max", -1));
  Tcl_ListObjAppendElement(interp, pRet, Tcl_NewIntObj(nMax));
  Tcl_ListObjAppendElement(interp, pRet, Tcl_NewStringObj("min", -1));
  Tcl_ListObjAppendElement(interp, pRet, Tcl_NewIntObj(nMin));
  Tcl_ListObjAppendElement(interp, pRet, Tcl_NewStringObj("recyclable", -1));
  Tcl_ListObjAppendElement(interp, pRet, Tcl_NewIntObj(nRecyclable));

  Tcl_SetObjResult(interp, pRet);

  return TCL_OK;
}

#ifdef SQLITE_ENABLE_UNLOCK_NOTIFY
static void test_unlock_notify_cb(void **aArg, int nArg){
  int ii;
  for(ii=0; ii<nArg; ii++){
    Tcl_EvalEx((Tcl_Interp *)aArg[ii], "unlock_notify", -1, TCL_EVAL_GLOBAL);
  }
}
#endif 

#ifdef SQLITE_ENABLE_UNLOCK_NOTIFY
static int test_unlock_notify(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  sqlite3 *db;
  int rc;

  if( objc!=2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "DB");
    return TCL_ERROR;
  }

  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ){
    return TCL_ERROR;
  }
  rc = sqlite3_unlock_notify(db, test_unlock_notify_cb, (void *)interp);
  Tcl_SetResult(interp, (char *)t1ErrorName(rc), TCL_STATIC);
  return TCL_OK;
}
#endif

static int test_wal_checkpoint(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  char *zDb = 0;
  sqlite3 *db;
  int rc;

  if( objc!=3 && objc!=2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "DB ?NAME?");
    return TCL_ERROR;
  }

  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ){
    return TCL_ERROR;
  }
  if( objc==3 ){
    zDb = Tcl_GetString(objv[2]);
  }
  rc = sqlite3_wal_checkpoint(db, zDb);
  Tcl_SetResult(interp, (char *)t1ErrorName(rc), TCL_STATIC);
  return TCL_OK;
}

static int test_wal_checkpoint_v2(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  char *zDb = 0;
  sqlite3 *db;
  int rc;

  int eMode;
  int nLog = -555;
  int nCkpt = -555;
  Tcl_Obj *pRet;

  const char * aMode[] = { "passive", "full", "restart", 0 };
  assert( SQLITE_CHECKPOINT_PASSIVE==0 );
  assert( SQLITE_CHECKPOINT_FULL==1 );
  assert( SQLITE_CHECKPOINT_RESTART==2 );

  if( objc!=3 && objc!=4 ){
    Tcl_WrongNumArgs(interp, 1, objv, "DB MODE ?NAME?");
    return TCL_ERROR;
  }

  if( objc==4 ){
    zDb = Tcl_GetString(objv[3]);
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db)
   || Tcl_GetIndexFromObj(interp, objv[2], aMode, "mode", 0, &eMode) 
  ){
    return TCL_ERROR;
  }

  rc = sqlite3_wal_checkpoint_v2(db, zDb, eMode, &nLog, &nCkpt);
  if( rc!=SQLITE_OK && rc!=SQLITE_BUSY ){
    Tcl_SetResult(interp, (char *)sqlite3_errmsg(db), TCL_VOLATILE);
    return TCL_ERROR;
  }

  pRet = Tcl_NewObj();
  Tcl_ListObjAppendElement(interp, pRet, Tcl_NewIntObj(rc==SQLITE_BUSY?1:0));
  Tcl_ListObjAppendElement(interp, pRet, Tcl_NewIntObj(nLog));
  Tcl_ListObjAppendElement(interp, pRet, Tcl_NewIntObj(nCkpt));
  Tcl_SetObjResult(interp, pRet);

  return TCL_OK;
}

static struct LogCallback {
  Tcl_Interp *pInterp;
  Tcl_Obj *pObj;
} logcallback = {0, 0};
static void xLogcallback(void *unused, int err, char *zMsg){
  Tcl_Obj *pNew = Tcl_DuplicateObj(logcallback.pObj);
  Tcl_IncrRefCount(pNew);
  Tcl_ListObjAppendElement(
      0, pNew, Tcl_NewStringObj(sqlite3TestErrorName(err), -1)
  );
  Tcl_ListObjAppendElement(0, pNew, Tcl_NewStringObj(zMsg, -1));
  Tcl_EvalObjEx(logcallback.pInterp, pNew, TCL_EVAL_GLOBAL|TCL_EVAL_DIRECT);
  Tcl_DecrRefCount(pNew);
}
static int test_sqlite3_log(
  ClientData clientData,
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  if( objc>2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "SCRIPT");
    return TCL_ERROR;
  }
  if( logcallback.pObj ){
    Tcl_DecrRefCount(logcallback.pObj);
    logcallback.pObj = 0;
    logcallback.pInterp = 0;
    sqlite3_config(SQLITE_CONFIG_LOG, 0, 0);
  }
  if( objc>1 ){
    logcallback.pObj = objv[1];
    Tcl_IncrRefCount(logcallback.pObj);
    logcallback.pInterp = interp;
    sqlite3_config(SQLITE_CONFIG_LOG, xLogcallback, 0);
  }
  return TCL_OK;
}

static int runAsObjProc(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  Tcl_CmdInfo cmdInfo;
  if( objc<2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "COMMAND ...");
    return TCL_ERROR;
  }
  if( !Tcl_GetCommandInfo(interp, Tcl_GetString(objv[1]), &cmdInfo) ){
    Tcl_AppendResult(interp, "command not found: ",
           Tcl_GetString(objv[1]), (char*)0);
    return TCL_ERROR;
  }
  if( cmdInfo.objProc==0 ){
    Tcl_AppendResult(interp, "command has no objProc: ",
           Tcl_GetString(objv[1]), (char*)0);
    return TCL_ERROR;
  }
  return cmdInfo.objProc(cmdInfo.objClientData, interp, objc-1, objv+1);
}

#ifndef SQLITE_OMIT_EXPLAIN
int printExplainQueryPlan(sqlite3_stmt *pStmt){
  const char *zSql;               
  char *zExplain;                 
  sqlite3_stmt *pExplain;         
  int rc;                         

  zSql = sqlite3_sql(pStmt);
  if( zSql==0 ) return SQLITE_ERROR;

  zExplain = sqlite3_mprintf("EXPLAIN QUERY PLAN %s", zSql);
  if( zExplain==0 ) return SQLITE_NOMEM;

  rc = sqlite3_prepare_v2(sqlite3_db_handle(pStmt), zExplain, -1, &pExplain, 0);
  sqlite3_free(zExplain);
  if( rc!=SQLITE_OK ) return rc;

  while( SQLITE_ROW==sqlite3_step(pExplain) ){
    int iSelectid = sqlite3_column_int(pExplain, 0);
    int iOrder = sqlite3_column_int(pExplain, 1);
    int iFrom = sqlite3_column_int(pExplain, 2);
    const char *zDetail = (const char *)sqlite3_column_text(pExplain, 3);

    printf("%d %d %d %s\n", iSelectid, iOrder, iFrom, zDetail);
  }

  return sqlite3_finalize(pExplain);
}

static int test_print_eqp(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  int rc;
  sqlite3_stmt *pStmt;

  if( objc!=2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "STMT");
    return TCL_ERROR;
  }
  if( getStmtPointer(interp, Tcl_GetString(objv[1]), &pStmt) ) return TCL_ERROR;
  rc = printExplainQueryPlan(pStmt);
  Tcl_SetResult(interp, (char *)t1ErrorName(rc), 0);
  return TCL_OK;
}
#endif 

static int optimization_control(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  int i;
  sqlite3 *db;
  const char *zOpt;
  int onoff;
  int mask;
  static const struct {
    const char *zOptName;
    int mask;
  } aOpt[] = {
    { "all",              SQLITE_OptMask        },
    { "query-flattener",  SQLITE_QueryFlattener },
    { "column-cache",     SQLITE_ColumnCache    },
    { "index-sort",       SQLITE_IndexSort      },
    { "index-search",     SQLITE_IndexSearch    },
    { "index-cover",      SQLITE_IndexCover     },
    { "groupby-order",    SQLITE_GroupByOrder   },
    { "factor-constants", SQLITE_FactorOutConst },
  };

  if( objc!=4 ){
    Tcl_WrongNumArgs(interp, 1, objv, "DB OPT BOOLEAN");
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;
  if( Tcl_GetBooleanFromObj(interp, objv[3], &onoff) ) return TCL_ERROR;
  zOpt = Tcl_GetString(objv[2]);
  for(i=0; i<sizeof(aOpt)/sizeof(aOpt[0]); i++){
    if( strcmp(zOpt, aOpt[i].zOptName)==0 ){
      mask = aOpt[i].mask;
      break;
    }
  }
  if( onoff ) mask = ~mask;
  if( i>=sizeof(aOpt)/sizeof(aOpt[0]) ){
    Tcl_AppendResult(interp, "unknown optimization - should be one of:",
                     (char*)0);
    for(i=0; i<sizeof(aOpt)/sizeof(aOpt[0]); i++){
      Tcl_AppendResult(interp, " ", aOpt[i].zOptName);
    }
    return TCL_ERROR;
  }
  sqlite3_test_control(SQLITE_TESTCTRL_OPTIMIZATIONS, db, mask);
  return TCL_OK;
}

int Sqlitetest1_Init(Tcl_Interp *interp){
  extern int sqlite3_search_count;
  extern int sqlite3_found_count;
  extern int sqlite3_interrupt_count;
  extern int sqlite3_open_file_count;
  extern int sqlite3_sort_count;
  extern int sqlite3_current_time;
#if SQLITE_OS_UNIX && defined(__APPLE__) && SQLITE_ENABLE_LOCKING_STYLE
  extern int sqlite3_hostid_num;
#endif
  extern int sqlite3_max_blobsize;
  extern int sqlite3BtreeSharedCacheReport(void*,
                                          Tcl_Interp*,int,Tcl_Obj*CONST*);
  static struct {
     char *zName;
     Tcl_CmdProc *xProc;
  } aCmd[] = {
     { "db_enter",                      (Tcl_CmdProc*)db_enter               },
     { "db_leave",                      (Tcl_CmdProc*)db_leave               },
     { "sqlite3_mprintf_int",           (Tcl_CmdProc*)sqlite3_mprintf_int    },
     { "sqlite3_mprintf_int64",         (Tcl_CmdProc*)sqlite3_mprintf_int64  },
     { "sqlite3_mprintf_long",          (Tcl_CmdProc*)sqlite3_mprintf_long   },
     { "sqlite3_mprintf_str",           (Tcl_CmdProc*)sqlite3_mprintf_str    },
     { "sqlite3_snprintf_str",          (Tcl_CmdProc*)sqlite3_snprintf_str   },
     { "sqlite3_mprintf_stronly",       (Tcl_CmdProc*)sqlite3_mprintf_stronly},
     { "sqlite3_mprintf_double",        (Tcl_CmdProc*)sqlite3_mprintf_double },
     { "sqlite3_mprintf_scaled",        (Tcl_CmdProc*)sqlite3_mprintf_scaled },
     { "sqlite3_mprintf_hexdouble",   (Tcl_CmdProc*)sqlite3_mprintf_hexdouble},
     { "sqlite3_mprintf_z_test",        (Tcl_CmdProc*)test_mprintf_z        },
     { "sqlite3_mprintf_n_test",        (Tcl_CmdProc*)test_mprintf_n        },
     { "sqlite3_snprintf_int",          (Tcl_CmdProc*)test_snprintf_int     },
     { "sqlite3_last_insert_rowid",     (Tcl_CmdProc*)test_last_rowid       },
     { "sqlite3_exec_printf",           (Tcl_CmdProc*)test_exec_printf      },
     { "sqlite3_exec_hex",              (Tcl_CmdProc*)test_exec_hex         },
     { "sqlite3_exec",                  (Tcl_CmdProc*)test_exec             },
     { "sqlite3_exec_nr",               (Tcl_CmdProc*)test_exec_nr          },
#ifndef SQLITE_OMIT_GET_TABLE
     { "sqlite3_get_table_printf",      (Tcl_CmdProc*)test_get_table_printf },
#endif
     { "sqlite3_close",                 (Tcl_CmdProc*)sqlite_test_close     },
     { "sqlite3_create_function",       (Tcl_CmdProc*)test_create_function  },
     { "sqlite3_create_aggregate",      (Tcl_CmdProc*)test_create_aggregate },
     { "sqlite_register_test_function", (Tcl_CmdProc*)test_register_func    },
     { "sqlite_abort",                  (Tcl_CmdProc*)sqlite_abort          },
     { "sqlite_bind",                   (Tcl_CmdProc*)test_bind             },
     { "breakpoint",                    (Tcl_CmdProc*)test_breakpoint       },
     { "sqlite3_key",                   (Tcl_CmdProc*)test_key              },
     { "sqlite3_rekey",                 (Tcl_CmdProc*)test_rekey            },
     { "sqlite_set_magic",              (Tcl_CmdProc*)sqlite_set_magic      },
     { "sqlite3_interrupt",             (Tcl_CmdProc*)test_interrupt        },
     { "sqlite_delete_function",        (Tcl_CmdProc*)delete_function       },
     { "sqlite_delete_collation",       (Tcl_CmdProc*)delete_collation      },
     { "sqlite3_get_autocommit",        (Tcl_CmdProc*)get_autocommit        },
     { "sqlite3_stack_used",            (Tcl_CmdProc*)test_stack_used       },
     { "sqlite3_busy_timeout",          (Tcl_CmdProc*)test_busy_timeout     },
     { "printf",                        (Tcl_CmdProc*)test_printf           },
     { "sqlite3IoTrace",              (Tcl_CmdProc*)test_io_trace         },
  };
  static struct {
     char *zName;
     Tcl_ObjCmdProc *xProc;
     void *clientData;
  } aObjCmd[] = {
     { "sqlite3_connection_pointer",    get_sqlite_pointer, 0 },
     { "sqlite3_bind_int",              test_bind_int,      0 },
     { "sqlite3_bind_zeroblob",         test_bind_zeroblob, 0 },
     { "sqlite3_bind_int64",            test_bind_int64,    0 },
     { "sqlite3_bind_double",           test_bind_double,   0 },
     { "sqlite3_bind_null",             test_bind_null     ,0 },
     { "sqlite3_bind_text",             test_bind_text     ,0 },
     { "sqlite3_bind_text16",           test_bind_text16   ,0 },
     { "sqlite3_bind_blob",             test_bind_blob     ,0 },
     { "sqlite3_bind_parameter_count",  test_bind_parameter_count, 0},
     { "sqlite3_bind_parameter_name",   test_bind_parameter_name,  0},
     { "sqlite3_bind_parameter_index",  test_bind_parameter_index, 0},
     { "sqlite3_clear_bindings",        test_clear_bindings, 0},
     { "sqlite3_sleep",                 test_sleep,          0},
     { "sqlite3_errcode",               test_errcode       ,0 },
     { "sqlite3_extended_errcode",      test_ex_errcode    ,0 },
     { "sqlite3_errmsg",                test_errmsg        ,0 },
     { "sqlite3_errmsg16",              test_errmsg16      ,0 },
     { "sqlite3_open",                  test_open          ,0 },
     { "sqlite3_open16",                test_open16        ,0 },
     { "sqlite3_complete16",            test_complete16    ,0 },

     { "sqlite3_prepare",               test_prepare       ,0 },
     { "sqlite3_prepare16",             test_prepare16     ,0 },
     { "sqlite3_prepare_v2",            test_prepare_v2    ,0 },
     { "sqlite3_prepare_tkt3134",       test_prepare_tkt3134, 0},
     { "sqlite3_prepare16_v2",          test_prepare16_v2  ,0 },
     { "sqlite3_finalize",              test_finalize      ,0 },
     { "sqlite3_stmt_status",           test_stmt_status   ,0 },
     { "sqlite3_reset",                 test_reset         ,0 },
     { "sqlite3_expired",               test_expired       ,0 },
     { "sqlite3_transfer_bindings",     test_transfer_bind ,0 },
     { "sqlite3_changes",               test_changes       ,0 },
     { "sqlite3_step",                  test_step          ,0 },
     { "sqlite3_sql",                   test_sql           ,0 },
     { "sqlite3_next_stmt",             test_next_stmt     ,0 },
     { "sqlite3_stmt_readonly",         test_stmt_readonly ,0 },

     { "sqlite3_release_memory",        test_release_memory,     0},
     { "sqlite3_soft_heap_limit",       test_soft_heap_limit,    0},
     { "sqlite3_thread_cleanup",        test_thread_cleanup,     0},
     { "sqlite3_pager_refcounts",       test_pager_refcounts,    0},

     { "sqlite3_load_extension",        test_load_extension,     0},
     { "sqlite3_enable_load_extension", test_enable_load,        0},
     { "sqlite3_extended_result_codes", test_extended_result_codes, 0},
     { "sqlite3_limit",                 test_limit,                 0},

     { "save_prng_state",               save_prng_state,    0 },
     { "restore_prng_state",            restore_prng_state, 0 },
     { "reset_prng_state",              reset_prng_state,   0 },
     { "optimization_control",          optimization_control,0},
     { "tcl_objproc",                   runAsObjProc,       0 },

     
     { "sqlite3_column_count",          test_column_count  ,0 },
     { "sqlite3_data_count",            test_data_count    ,0 },
     { "sqlite3_column_type",           test_column_type   ,0 },
     { "sqlite3_column_blob",           test_column_blob   ,0 },
     { "sqlite3_column_double",         test_column_double ,0 },
     { "sqlite3_column_int64",          test_column_int64  ,0 },
     { "sqlite3_column_text",   test_stmt_utf8,  (void*)sqlite3_column_text },
     { "sqlite3_column_name",   test_stmt_utf8,  (void*)sqlite3_column_name },
     { "sqlite3_column_int",    test_stmt_int,   (void*)sqlite3_column_int  },
     { "sqlite3_column_bytes",  test_stmt_int,   (void*)sqlite3_column_bytes},
#ifndef SQLITE_OMIT_DECLTYPE
     { "sqlite3_column_decltype",test_stmt_utf8,(void*)sqlite3_column_decltype},
#endif
#ifdef SQLITE_ENABLE_COLUMN_METADATA
{ "sqlite3_column_database_name",test_stmt_utf8,(void*)sqlite3_column_database_name},
{ "sqlite3_column_table_name",test_stmt_utf8,(void*)sqlite3_column_table_name},
{ "sqlite3_column_origin_name",test_stmt_utf8,(void*)sqlite3_column_origin_name},
#endif

#ifndef SQLITE_OMIT_UTF16
     { "sqlite3_column_bytes16", test_stmt_int, (void*)sqlite3_column_bytes16 },
     { "sqlite3_column_text16",  test_stmt_utf16, (void*)sqlite3_column_text16},
     { "sqlite3_column_name16",  test_stmt_utf16, (void*)sqlite3_column_name16},
     { "add_alignment_test_collations", add_alignment_test_collations, 0      },
#ifndef SQLITE_OMIT_DECLTYPE
     { "sqlite3_column_decltype16",test_stmt_utf16,(void*)sqlite3_column_decltype16},
#endif
#ifdef SQLITE_ENABLE_COLUMN_METADATA
{"sqlite3_column_database_name16",
  test_stmt_utf16, sqlite3_column_database_name16},
{"sqlite3_column_table_name16", test_stmt_utf16, (void*)sqlite3_column_table_name16},
{"sqlite3_column_origin_name16", test_stmt_utf16, (void*)sqlite3_column_origin_name16},
#endif
#endif
     { "sqlite3_create_collation_v2", test_create_collation_v2, 0 },
     { "sqlite3_global_recover",     test_global_recover, 0   },
     { "working_64bit_int",          working_64bit_int,   0   },
     { "vfs_unlink_test",            vfs_unlink_test,     0   },
     { "vfs_initfail_test",          vfs_initfail_test,   0   },
     { "vfs_unregister_all",         vfs_unregister_all,  0   },
     { "vfs_reregister_all",         vfs_reregister_all,  0   },
     { "file_control_test",          file_control_test,   0   },
     { "file_control_lasterrno_test", file_control_lasterrno_test,  0   },
     { "file_control_lockproxy_test", file_control_lockproxy_test,  0   },
     { "file_control_chunksize_test", file_control_chunksize_test,  0   },
     { "file_control_sizehint_test", file_control_sizehint_test,  0   },
     { "sqlite3_vfs_list",           vfs_list,     0   },
     { "sqlite3_create_function_v2", test_create_function_v2, 0 },

     
#ifndef SQLITE_OMIT_UTF16
     { "add_test_collate",        test_collate, 0            },
     { "add_test_collate_needed", test_collate_needed, 0     },
     { "add_test_function",       test_function, 0           },
#endif
     { "sqlite3_test_errstr",     test_errstr, 0             },
     { "tcl_variable_type",       tcl_variable_type, 0       },
#ifndef SQLITE_OMIT_SHARED_CACHE
     { "sqlite3_enable_shared_cache", test_enable_shared, 0  },
     { "sqlite3_shared_cache_report", sqlite3BtreeSharedCacheReport, 0},
#endif
     { "sqlite3_libversion_number", test_libversion_number, 0  },
#ifdef SQLITE_ENABLE_COLUMN_METADATA
     { "sqlite3_table_column_metadata", test_table_column_metadata, 0  },
#endif
#ifndef SQLITE_OMIT_INCRBLOB
     { "sqlite3_blob_read",   test_blob_read, 0  },
     { "sqlite3_blob_write",  test_blob_write, 0  },
     { "sqlite3_blob_reopen", test_blob_reopen, 0  },
     { "sqlite3_blob_bytes",  test_blob_bytes, 0  },
     { "sqlite3_blob_close",  test_blob_close, 0  },
#endif
     { "pcache_stats",       test_pcache_stats, 0  },
#ifdef SQLITE_ENABLE_UNLOCK_NOTIFY
     { "sqlite3_unlock_notify", test_unlock_notify, 0  },
#endif
     { "sqlite3_wal_checkpoint",   test_wal_checkpoint, 0  },
     { "sqlite3_wal_checkpoint_v2",test_wal_checkpoint_v2, 0  },
     { "test_sqlite3_log",         test_sqlite3_log, 0  },
#ifndef SQLITE_OMIT_EXPLAIN
     { "print_explain_query_plan", test_print_eqp, 0  },
#endif
  };
  static int bitmask_size = sizeof(Bitmask)*8;
  int i;
  extern int sqlite3_sync_count, sqlite3_fullsync_count;
  extern int sqlite3_opentemp_count;
  extern int sqlite3_like_count;
  extern int sqlite3_xferopt_count;
  extern int sqlite3_pager_readdb_count;
  extern int sqlite3_pager_writedb_count;
  extern int sqlite3_pager_writej_count;
#if SQLITE_OS_WIN
  extern int sqlite3_os_type;
#endif
#ifdef SQLITE_DEBUG
  extern int sqlite3WhereTrace;
  extern int sqlite3OSTrace;
  extern int sqlite3VdbeAddopTrace;
  extern int sqlite3WalTrace;
#endif
#ifdef SQLITE_TEST
  extern char sqlite3_query_plan[];
  static char *query_plan = sqlite3_query_plan;
#ifdef SQLITE_ENABLE_FTS3
  extern int sqlite3_fts3_enable_parentheses;
#endif
#endif

  for(i=0; i<sizeof(aCmd)/sizeof(aCmd[0]); i++){
    Tcl_CreateCommand(interp, aCmd[i].zName, aCmd[i].xProc, 0, 0);
  }
  for(i=0; i<sizeof(aObjCmd)/sizeof(aObjCmd[0]); i++){
    Tcl_CreateObjCommand(interp, aObjCmd[i].zName, 
        aObjCmd[i].xProc, aObjCmd[i].clientData, 0);
  }
  Tcl_LinkVar(interp, "sqlite_search_count", 
      (char*)&sqlite3_search_count, TCL_LINK_INT);
  Tcl_LinkVar(interp, "sqlite_found_count", 
      (char*)&sqlite3_found_count, TCL_LINK_INT);
  Tcl_LinkVar(interp, "sqlite_sort_count", 
      (char*)&sqlite3_sort_count, TCL_LINK_INT);
  Tcl_LinkVar(interp, "sqlite3_max_blobsize", 
      (char*)&sqlite3_max_blobsize, TCL_LINK_INT);
  Tcl_LinkVar(interp, "sqlite_like_count", 
      (char*)&sqlite3_like_count, TCL_LINK_INT);
  Tcl_LinkVar(interp, "sqlite_interrupt_count", 
      (char*)&sqlite3_interrupt_count, TCL_LINK_INT);
  Tcl_LinkVar(interp, "sqlite_open_file_count", 
      (char*)&sqlite3_open_file_count, TCL_LINK_INT);
  Tcl_LinkVar(interp, "sqlite_current_time", 
      (char*)&sqlite3_current_time, TCL_LINK_INT);
#if SQLITE_OS_UNIX && defined(__APPLE__) && SQLITE_ENABLE_LOCKING_STYLE
  Tcl_LinkVar(interp, "sqlite_hostid_num", 
      (char*)&sqlite3_hostid_num, TCL_LINK_INT);
#endif
  Tcl_LinkVar(interp, "sqlite3_xferopt_count",
      (char*)&sqlite3_xferopt_count, TCL_LINK_INT);
  Tcl_LinkVar(interp, "sqlite3_pager_readdb_count",
      (char*)&sqlite3_pager_readdb_count, TCL_LINK_INT);
  Tcl_LinkVar(interp, "sqlite3_pager_writedb_count",
      (char*)&sqlite3_pager_writedb_count, TCL_LINK_INT);
  Tcl_LinkVar(interp, "sqlite3_pager_writej_count",
      (char*)&sqlite3_pager_writej_count, TCL_LINK_INT);
#ifndef SQLITE_OMIT_UTF16
  Tcl_LinkVar(interp, "unaligned_string_counter",
      (char*)&unaligned_string_counter, TCL_LINK_INT);
#endif
#ifndef SQLITE_OMIT_UTF16
  Tcl_LinkVar(interp, "sqlite_last_needed_collation",
      (char*)&pzNeededCollation, TCL_LINK_STRING|TCL_LINK_READ_ONLY);
#endif
#if SQLITE_OS_WIN
  Tcl_LinkVar(interp, "sqlite_os_type",
      (char*)&sqlite3_os_type, TCL_LINK_INT);
#endif
#ifdef SQLITE_TEST
  Tcl_LinkVar(interp, "sqlite_query_plan",
      (char*)&query_plan, TCL_LINK_STRING|TCL_LINK_READ_ONLY);
#endif
#ifdef SQLITE_DEBUG
  Tcl_LinkVar(interp, "sqlite_addop_trace",
      (char*)&sqlite3VdbeAddopTrace, TCL_LINK_INT);
  Tcl_LinkVar(interp, "sqlite_where_trace",
      (char*)&sqlite3WhereTrace, TCL_LINK_INT);
  Tcl_LinkVar(interp, "sqlite_os_trace",
      (char*)&sqlite3OSTrace, TCL_LINK_INT);
#ifndef SQLITE_OMIT_WAL
  Tcl_LinkVar(interp, "sqlite_wal_trace",
      (char*)&sqlite3WalTrace, TCL_LINK_INT);
#endif
#endif
#ifndef SQLITE_OMIT_DISKIO
  Tcl_LinkVar(interp, "sqlite_opentemp_count",
      (char*)&sqlite3_opentemp_count, TCL_LINK_INT);
#endif
  Tcl_LinkVar(interp, "sqlite_static_bind_value",
      (char*)&sqlite_static_bind_value, TCL_LINK_STRING);
  Tcl_LinkVar(interp, "sqlite_static_bind_nbyte",
      (char*)&sqlite_static_bind_nbyte, TCL_LINK_INT);
  Tcl_LinkVar(interp, "sqlite_temp_directory",
      (char*)&sqlite3_temp_directory, TCL_LINK_STRING);
  Tcl_LinkVar(interp, "bitmask_size",
      (char*)&bitmask_size, TCL_LINK_INT|TCL_LINK_READ_ONLY);
  Tcl_LinkVar(interp, "sqlite_sync_count",
      (char*)&sqlite3_sync_count, TCL_LINK_INT);
  Tcl_LinkVar(interp, "sqlite_fullsync_count",
      (char*)&sqlite3_fullsync_count, TCL_LINK_INT);
#if defined(SQLITE_ENABLE_FTS3) && defined(SQLITE_TEST)
  Tcl_LinkVar(interp, "sqlite_fts3_enable_parentheses",
      (char*)&sqlite3_fts3_enable_parentheses, TCL_LINK_INT);
#endif
  return TCL_OK;
}

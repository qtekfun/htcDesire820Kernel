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
** Internal interface definitions for SQLite.
**
*/
#ifndef _SQLITEINT_H_
#define _SQLITEINT_H_

#ifndef SQLITE_DISABLE_LFS
# define _LARGE_FILE       1
# ifndef _FILE_OFFSET_BITS
#   define _FILE_OFFSET_BITS 64
# endif
# define _LARGEFILE_SOURCE 1
#endif

#ifdef _HAVE_SQLITE_CONFIG_H
#include "config.h"
#endif

#include "sqliteLimit.h"

#if defined(__BORLANDC__)
#pragma warn -rch 
#pragma warn -ccc 
#pragma warn -aus 
#pragma warn -csu 
#pragma warn -spa 
#endif

#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#define SQLITE_INDEX_SAMPLES 10

#if defined(__PTRDIFF_TYPE__)  
# define SQLITE_INT_TO_PTR(X)  ((void*)(__PTRDIFF_TYPE__)(X))
# define SQLITE_PTR_TO_INT(X)  ((int)(__PTRDIFF_TYPE__)(X))
#elif !defined(__GNUC__)       
# define SQLITE_INT_TO_PTR(X)  ((void*)&((char*)0)[X])
# define SQLITE_PTR_TO_INT(X)  ((int)(((char*)X)-(char*)0))
#elif defined(HAVE_STDINT_H)   
# define SQLITE_INT_TO_PTR(X)  ((void*)(intptr_t)(X))
# define SQLITE_PTR_TO_INT(X)  ((int)(intptr_t)(X))
#else                          
# define SQLITE_INT_TO_PTR(X)  ((void*)(X))
# define SQLITE_PTR_TO_INT(X)  ((int)(X))
#endif

#if !defined(SQLITE_THREADSAFE)
#if defined(THREADSAFE)
# define SQLITE_THREADSAFE THREADSAFE
#else
# define SQLITE_THREADSAFE 1 
#endif
#endif

#if !defined(SQLITE_DEFAULT_MEMSTATUS)
# define SQLITE_DEFAULT_MEMSTATUS 1
#endif

#if defined(SQLITE_SYSTEM_MALLOC)+defined(SQLITE_MEMDEBUG)>1
# error "At most one of the following compile-time configuration options\
 is allows: SQLITE_SYSTEM_MALLOC, SQLITE_MEMDEBUG"
#endif
#if defined(SQLITE_SYSTEM_MALLOC)+defined(SQLITE_MEMDEBUG)==0
# define SQLITE_SYSTEM_MALLOC 1
#endif

#if !defined(SQLITE_MALLOC_SOFT_LIMIT)
# define SQLITE_MALLOC_SOFT_LIMIT 1024
#endif

#if !defined(_XOPEN_SOURCE) && !defined(__DARWIN__) && !defined(__APPLE__) && SQLITE_THREADSAFE
#  define _XOPEN_SOURCE 500  
#endif

#if defined(SQLITE_TCL) || defined(TCLSH)
# include <tcl.h>
#endif

#if !defined(NDEBUG) && !defined(SQLITE_DEBUG) 
# define NDEBUG 1
#endif

#ifdef SQLITE_COVERAGE_TEST
  void sqlite3Coverage(int);
# define testcase(X)  if( X ){ sqlite3Coverage(__LINE__); }
#else
# define testcase(X)
#endif

#if !defined(NDEBUG) || defined(SQLITE_COVERAGE_TEST)
# define TESTONLY(X)  X
#else
# define TESTONLY(X)
#endif

#ifndef NDEBUG
# define VVA_ONLY(X)  X
#else
# define VVA_ONLY(X)
#endif

#if defined(SQLITE_COVERAGE_TEST)
# define ALWAYS(X)      (1)
# define NEVER(X)       (0)
#elif !defined(NDEBUG)
# define ALWAYS(X)      ((X)?1:(assert(0),0))
# define NEVER(X)       ((X)?(assert(0),1):0)
#else
# define ALWAYS(X)      (X)
# define NEVER(X)       (X)
#endif

#define IS_BIG_INT(X)  (((X)&~(i64)0xffffffff)!=0)

#if defined(__GNUC__) && 0
# define likely(X)    __builtin_expect((X),1)
# define unlikely(X)  __builtin_expect((X),0)
#else
# define likely(X)    !!(X)
# define unlikely(X)  !!(X)
#endif

#include "sqlite3.h"
#include "hash.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

#ifdef SQLITE_OMIT_FLOATING_POINT
# define double sqlite_int64
# define float sqlite_int64
# define LONGDOUBLE_TYPE sqlite_int64
# ifndef SQLITE_BIG_DBL
#   define SQLITE_BIG_DBL (((sqlite3_int64)1)<<50)
# endif
# define SQLITE_OMIT_DATETIME_FUNCS 1
# define SQLITE_OMIT_TRACE 1
# undef SQLITE_MIXED_ENDIAN_64BIT_FLOAT
# undef SQLITE_HAVE_ISNAN
#endif
#ifndef SQLITE_BIG_DBL
# define SQLITE_BIG_DBL (1e99)
#endif

#ifdef SQLITE_OMIT_TEMPDB
#define OMIT_TEMPDB 1
#else
#define OMIT_TEMPDB 0
#endif

#define SQLITE_MAX_FILE_FORMAT 4
#ifndef SQLITE_DEFAULT_FILE_FORMAT
# define SQLITE_DEFAULT_FILE_FORMAT 1
#endif

#ifndef SQLITE_DEFAULT_RECURSIVE_TRIGGERS
# define SQLITE_DEFAULT_RECURSIVE_TRIGGERS 0
#endif

#ifndef SQLITE_TEMP_STORE
# define SQLITE_TEMP_STORE 1
#endif

#ifndef offsetof
#define offsetof(STRUCTURE,FIELD) ((int)((char*)&((STRUCTURE*)0)->FIELD))
#endif

#if 'A' == '\301'
# define SQLITE_EBCDIC 1
#else
# define SQLITE_ASCII 1
#endif

#ifndef UINT32_TYPE
# ifdef HAVE_UINT32_T
#  define UINT32_TYPE uint32_t
# else
#  define UINT32_TYPE unsigned int
# endif
#endif
#ifndef UINT16_TYPE
# ifdef HAVE_UINT16_T
#  define UINT16_TYPE uint16_t
# else
#  define UINT16_TYPE unsigned short int
# endif
#endif
#ifndef INT16_TYPE
# ifdef HAVE_INT16_T
#  define INT16_TYPE int16_t
# else
#  define INT16_TYPE short int
# endif
#endif
#ifndef UINT8_TYPE
# ifdef HAVE_UINT8_T
#  define UINT8_TYPE uint8_t
# else
#  define UINT8_TYPE unsigned char
# endif
#endif
#ifndef INT8_TYPE
# ifdef HAVE_INT8_T
#  define INT8_TYPE int8_t
# else
#  define INT8_TYPE signed char
# endif
#endif
#ifndef LONGDOUBLE_TYPE
# define LONGDOUBLE_TYPE long double
#endif
typedef sqlite_int64 i64;          
typedef sqlite_uint64 u64;         
typedef UINT32_TYPE u32;           
typedef UINT16_TYPE u16;           
typedef INT16_TYPE i16;            
typedef UINT8_TYPE u8;             
typedef INT8_TYPE i8;              

#define SQLITE_MAX_U32  ((((u64)1)<<32)-1)

#ifdef SQLITE_AMALGAMATION
const int sqlite3one = 1;
#else
extern const int sqlite3one;
#endif
#if defined(i386) || defined(__i386__) || defined(_M_IX86)\
                             || defined(__x86_64) || defined(__x86_64__)
# define SQLITE_BIGENDIAN    0
# define SQLITE_LITTLEENDIAN 1
# define SQLITE_UTF16NATIVE  SQLITE_UTF16LE
#else
# define SQLITE_BIGENDIAN    (*(char *)(&sqlite3one)==0)
# define SQLITE_LITTLEENDIAN (*(char *)(&sqlite3one)==1)
# define SQLITE_UTF16NATIVE (SQLITE_BIGENDIAN?SQLITE_UTF16BE:SQLITE_UTF16LE)
#endif

#define LARGEST_INT64  (0xffffffff|(((i64)0x7fffffff)<<32))
#define SMALLEST_INT64 (((i64)-1) - LARGEST_INT64)

#define ROUND8(x)     (((x)+7)&~7)

#define ROUNDDOWN8(x) ((x)&~7)

#ifdef SQLITE_4_BYTE_ALIGNED_MALLOC
# define EIGHT_BYTE_ALIGNMENT(X)   ((((char*)(X) - (char*)0)&3)==0)
#else
# define EIGHT_BYTE_ALIGNMENT(X)   ((((char*)(X) - (char*)0)&7)==0)
#endif


typedef struct BusyHandler BusyHandler;
struct BusyHandler {
  int (*xFunc)(void *,int);  
  void *pArg;                
  int nBusy;                 
};

#define MASTER_NAME       "sqlite_master"
#define TEMP_MASTER_NAME  "sqlite_temp_master"

#define MASTER_ROOT       1

#define SCHEMA_TABLE(x)  ((!OMIT_TEMPDB)&&(x==1)?TEMP_MASTER_NAME:MASTER_NAME)

#define ArraySize(X)    ((int)(sizeof(X)/sizeof(X[0])))

#define SQLITE_DYNAMIC   ((sqlite3_destructor_type)sqlite3DbFree)

#ifdef SQLITE_OMIT_WSD
  #define SQLITE_WSD const
  #define GLOBAL(t,v) (*(t*)sqlite3_wsd_find((void*)&(v), sizeof(v)))
  #define sqlite3GlobalConfig GLOBAL(struct Sqlite3Config, sqlite3Config)
  int sqlite3_wsd_init(int N, int J);
  void *sqlite3_wsd_find(void *K, int L);
#else
  #define SQLITE_WSD 
  #define GLOBAL(t,v) v
  #define sqlite3GlobalConfig sqlite3Config
#endif

#define UNUSED_PARAMETER(x) (void)(x)
#define UNUSED_PARAMETER2(x,y) UNUSED_PARAMETER(x),UNUSED_PARAMETER(y)

typedef struct AggInfo AggInfo;
typedef struct AuthContext AuthContext;
typedef struct AutoincInfo AutoincInfo;
typedef struct Bitvec Bitvec;
typedef struct CollSeq CollSeq;
typedef struct Column Column;
typedef struct Db Db;
typedef struct Schema Schema;
typedef struct Expr Expr;
typedef struct ExprList ExprList;
typedef struct ExprSpan ExprSpan;
typedef struct FKey FKey;
typedef struct FuncDestructor FuncDestructor;
typedef struct FuncDef FuncDef;
typedef struct FuncDefHash FuncDefHash;
typedef struct IdList IdList;
typedef struct Index Index;
typedef struct IndexSample IndexSample;
typedef struct KeyClass KeyClass;
typedef struct KeyInfo KeyInfo;
typedef struct Lookaside Lookaside;
typedef struct LookasideSlot LookasideSlot;
typedef struct Module Module;
typedef struct NameContext NameContext;
typedef struct Parse Parse;
typedef struct RowSet RowSet;
typedef struct Savepoint Savepoint;
typedef struct Select Select;
typedef struct SrcList SrcList;
typedef struct StrAccum StrAccum;
typedef struct Table Table;
typedef struct TableLock TableLock;
typedef struct Token Token;
typedef struct Trigger Trigger;
typedef struct TriggerPrg TriggerPrg;
typedef struct TriggerStep TriggerStep;
typedef struct UnpackedRecord UnpackedRecord;
typedef struct VTable VTable;
typedef struct Walker Walker;
typedef struct WherePlan WherePlan;
typedef struct WhereInfo WhereInfo;
typedef struct WhereLevel WhereLevel;

#include "btree.h"
#include "vdbe.h"
#include "pager.h"
#include "pcache.h"

#include "os.h"
#include "mutex.h"


struct Db {
  char *zName;         
  Btree *pBt;          
  u8 inTrans;          
  u8 safety_level;     
  Schema *pSchema;     
};

struct Schema {
  int schema_cookie;   
  int iGeneration;     
  Hash tblHash;        
  Hash idxHash;        
  Hash trigHash;       
  Hash fkeyHash;       
  Table *pSeqTab;      
  u8 file_format;      
  u8 enc;              
  u16 flags;           
  int cache_size;      
};

#define DbHasProperty(D,I,P)     (((D)->aDb[I].pSchema->flags&(P))==(P))
#define DbHasAnyProperty(D,I,P)  (((D)->aDb[I].pSchema->flags&(P))!=0)
#define DbSetProperty(D,I,P)     (D)->aDb[I].pSchema->flags|=(P)
#define DbClearProperty(D,I,P)   (D)->aDb[I].pSchema->flags&=~(P)

#define DB_SchemaLoaded    0x0001  
#define DB_UnresetViews    0x0002  
#define DB_Empty           0x0004  

#define SQLITE_N_LIMIT (SQLITE_LIMIT_TRIGGER_DEPTH+1)

struct Lookaside {
  u16 sz;                 
  u8 bEnabled;            
  u8 bMalloced;           
  int nOut;               
  int mxOut;              
  int anStat[3];          
  LookasideSlot *pFree;   
  void *pStart;           
  void *pEnd;             
};
struct LookasideSlot {
  LookasideSlot *pNext;    
};

struct FuncDefHash {
  FuncDef *a[23];       
};

struct sqlite3 {
  sqlite3_vfs *pVfs;            
  int nDb;                      
  Db *aDb;                      
  int flags;                    
  int openFlags;                
  int errCode;                  
  int errMask;                  
  u8 autoCommit;                
  u8 temp_store;                
  u8 mallocFailed;              
  u8 dfltLockMode;              
  signed char nextAutovac;      
  u8 suppressErr;               
  int nextPagesize;             
  int nTable;                   
  CollSeq *pDfltColl;           
  i64 lastRowid;                
  u32 magic;                    
  int nChange;                  
  int nTotalChange;             
  sqlite3_mutex *mutex;         
  int aLimit[SQLITE_N_LIMIT];   
  struct sqlite3InitInfo {      
    int iDb;                    
    int newTnum;                
    u8 busy;                    
    u8 orphanTrigger;           
  } init;
  int nExtension;               
  void **aExtension;            
  struct Vdbe *pVdbe;           
  int activeVdbeCnt;            
  int writeVdbeCnt;             
  int vdbeExecCnt;              
  void (*xTrace)(void*,const char*);        
  void *pTraceArg;                          
  void (*xProfile)(void*,const char*,u64);  
  void *pProfileArg;                        
  void *pCommitArg;                    
  int (*xCommitCallback)(void*);    
  void *pRollbackArg;                  
  void (*xRollbackCallback)(void*); 
  void *pUpdateArg;
  void (*xUpdateCallback)(void*,int, const char*,const char*,sqlite_int64);
#ifndef SQLITE_OMIT_WAL
  int (*xWalCallback)(void *, sqlite3 *, const char *, int);
  void *pWalArg;
#endif
  void(*xCollNeeded)(void*,sqlite3*,int eTextRep,const char*);
  void(*xCollNeeded16)(void*,sqlite3*,int eTextRep,const void*);
  void *pCollNeededArg;
  sqlite3_value *pErr;          
  char *zErrMsg;                
  char *zErrMsg16;              
  union {
    volatile int isInterrupted; 
    double notUsed1;            
  } u1;
  Lookaside lookaside;          
#ifndef SQLITE_OMIT_AUTHORIZATION
  int (*xAuth)(void*,int,const char*,const char*,const char*,const char*);
                                
  void *pAuthArg;               
#endif
#ifndef SQLITE_OMIT_PROGRESS_CALLBACK
  int (*xProgress)(void *);     
  void *pProgressArg;           
  int nProgressOps;             
#endif
#ifndef SQLITE_OMIT_VIRTUALTABLE
  Hash aModule;                 
  Table *pVTab;                 
  VTable **aVTrans;             
  int nVTrans;                  
  VTable *pDisconnect;    
#endif
  FuncDefHash aFunc;            
  Hash aCollSeq;                
  BusyHandler busyHandler;      
  int busyTimeout;              
  Db aDbStatic[2];              
  Savepoint *pSavepoint;        
  int nSavepoint;               
  int nStatement;               
  u8 isTransactionSavepoint;    
  i64 nDeferredCons;            
  int *pnBytesFreed;            

#ifdef SQLITE_ENABLE_UNLOCK_NOTIFY
  sqlite3 *pBlockingConnection; 
  sqlite3 *pUnlockConnection;           
  void *pUnlockArg;                     
  void (*xUnlockNotify)(void **, int);  
  sqlite3 *pNextBlocked;        
#endif
};

#define ENC(db) ((db)->aDb[0].pSchema->enc)

#define SQLITE_VdbeTrace      0x00000100  
#define SQLITE_InternChanges  0x00000200  
#define SQLITE_FullColNames   0x00000400  
#define SQLITE_ShortColNames  0x00000800  
#define SQLITE_CountRows      0x00001000  
                                          
                                          
#define SQLITE_NullCallback   0x00002000  
                                          
#define SQLITE_SqlTrace       0x00004000  
#define SQLITE_VdbeListing    0x00008000  
#define SQLITE_WriteSchema    0x00010000  
#define SQLITE_NoReadlock     0x00020000  
#define SQLITE_IgnoreChecks   0x00040000  
#define SQLITE_ReadUncommitted 0x0080000  
#define SQLITE_LegacyFileFmt  0x00100000  
#define SQLITE_FullFSync      0x00200000  
#define SQLITE_CkptFullFSync  0x00400000  
#define SQLITE_RecoveryMode   0x00800000  
#define SQLITE_ReverseOrder   0x01000000  
#define SQLITE_RecTriggers    0x02000000  
#define SQLITE_ForeignKeys    0x04000000  
#define SQLITE_AutoIndex      0x08000000  
#define SQLITE_PreferBuiltin  0x10000000  
#define SQLITE_LoadExtension  0x20000000  
#define SQLITE_EnableTrigger  0x40000000  

#define SQLITE_QueryFlattener 0x01        
#define SQLITE_ColumnCache    0x02        
#define SQLITE_IndexSort      0x04        
#define SQLITE_IndexSearch    0x08        
#define SQLITE_IndexCover     0x10        
#define SQLITE_GroupByOrder   0x20        
#define SQLITE_FactorOutConst 0x40        
#define SQLITE_OptMask        0xff        

#define SQLITE_MAGIC_OPEN     0xa029a697  
#define SQLITE_MAGIC_CLOSED   0x9f3c2d33  
#define SQLITE_MAGIC_SICK     0x4b771290  
#define SQLITE_MAGIC_BUSY     0xf03b7906  
#define SQLITE_MAGIC_ERROR    0xb5357930  

struct FuncDef {
  i16 nArg;            
  u8 iPrefEnc;         
  u8 flags;            
  void *pUserData;     
  FuncDef *pNext;      
  void (*xFunc)(sqlite3_context*,int,sqlite3_value**); 
  void (*xStep)(sqlite3_context*,int,sqlite3_value**); 
  void (*xFinalize)(sqlite3_context*);                
  char *zName;         
  FuncDef *pHash;      
  FuncDestructor *pDestructor;   
};

struct FuncDestructor {
  int nRef;
  void (*xDestroy)(void *);
  void *pUserData;
};

#define SQLITE_FUNC_LIKE     0x01 
#define SQLITE_FUNC_CASE     0x02 
#define SQLITE_FUNC_EPHEM    0x04 
#define SQLITE_FUNC_NEEDCOLL 0x08 
#define SQLITE_FUNC_PRIVATE  0x10 
#define SQLITE_FUNC_COUNT    0x20 
#define SQLITE_FUNC_COALESCE 0x40 

#define FUNCTION(zName, nArg, iArg, bNC, xFunc) \
  {nArg, SQLITE_UTF8, bNC*SQLITE_FUNC_NEEDCOLL, \
   SQLITE_INT_TO_PTR(iArg), 0, xFunc, 0, 0, #zName, 0, 0}
#define STR_FUNCTION(zName, nArg, pArg, bNC, xFunc) \
  {nArg, SQLITE_UTF8, bNC*SQLITE_FUNC_NEEDCOLL, \
   pArg, 0, xFunc, 0, 0, #zName, 0, 0}
#define LIKEFUNC(zName, nArg, arg, flags) \
  {nArg, SQLITE_UTF8, flags, (void *)arg, 0, likeFunc, 0, 0, #zName, 0, 0}
#define AGGREGATE(zName, nArg, arg, nc, xStep, xFinal) \
  {nArg, SQLITE_UTF8, nc*SQLITE_FUNC_NEEDCOLL, \
   SQLITE_INT_TO_PTR(arg), 0, 0, xStep,xFinal,#zName,0,0}

struct Savepoint {
  char *zName;                        
  i64 nDeferredCons;                  
  Savepoint *pNext;                   
};

#define SAVEPOINT_BEGIN      0
#define SAVEPOINT_RELEASE    1
#define SAVEPOINT_ROLLBACK   2


struct Module {
  const sqlite3_module *pModule;       
  const char *zName;                   
  void *pAux;                          
  void (*xDestroy)(void *);            
};

struct Column {
  char *zName;     
  Expr *pDflt;     
  char *zDflt;     
  char *zType;     
  char *zColl;     
  u8 notNull;      
  u8 isPrimKey;    
  char affinity;   
#ifndef SQLITE_OMIT_VIRTUALTABLE
  u8 isHidden;     
#endif
};

/*
** A "Collating Sequence" is defined by an instance of the following
** structure. Conceptually, a collating sequence consists of a name and
** a comparison routine that defines the order of that sequence.
**
** There may two separate implementations of the collation function, one
** that processes text in UTF-8 encoding (CollSeq.xCmp) and another that
** processes text encoded in UTF-16 (CollSeq.xCmp16), using the machine
** native byte order. When a collation sequence is invoked, SQLite selects
** the version that will require the least expensive encoding
** translations, if any.
**
** The CollSeq.pUser member variable is an extra parameter that passed in
** as the first argument to the UTF-8 comparison function, xCmp.
** CollSeq.pUser16 is the equivalent for the UTF-16 comparison function,
** xCmp16.
**
** If both CollSeq.xCmp and CollSeq.xCmp16 are NULL, it means that the
** collating sequence is undefined.  Indices built on an undefined
** collating sequence may not be read or written.
*/
struct CollSeq {
  char *zName;          
  u8 enc;               
  u8 type;              
  void *pUser;          
  int (*xCmp)(void*,int, const void*, int, const void*);
  void (*xDel)(void*);  
};

#define SQLITE_COLL_BINARY  1  
#define SQLITE_COLL_NOCASE  2  
#define SQLITE_COLL_REVERSE 3  
#define SQLITE_COLL_USER    0  

#define SQLITE_SO_ASC       0  
#define SQLITE_SO_DESC      1  

#define SQLITE_AFF_TEXT     'a'
#define SQLITE_AFF_NONE     'b'
#define SQLITE_AFF_NUMERIC  'c'
#define SQLITE_AFF_INTEGER  'd'
#define SQLITE_AFF_REAL     'e'

#define sqlite3IsNumericAffinity(X)  ((X)>=SQLITE_AFF_NUMERIC)

#define SQLITE_AFF_MASK     0x67

#define SQLITE_JUMPIFNULL   0x08  
#define SQLITE_STOREP2      0x10  
#define SQLITE_NULLEQ       0x80  

struct VTable {
  sqlite3 *db;              
  Module *pMod;             
  sqlite3_vtab *pVtab;      
  int nRef;                 
  VTable *pNext;            
};

struct Table {
  char *zName;         
  int iPKey;           
  int nCol;            
  Column *aCol;        
  Index *pIndex;       
  int tnum;            
  unsigned nRowEst;    
  Select *pSelect;     
  u16 nRef;            
  u8 tabFlags;         
  u8 keyConf;          
  FKey *pFKey;         
  char *zColAff;       
#ifndef SQLITE_OMIT_CHECK
  Expr *pCheck;        
#endif
#ifndef SQLITE_OMIT_ALTERTABLE
  int addColOffset;    
#endif
#ifndef SQLITE_OMIT_VIRTUALTABLE
  VTable *pVTable;     
  int nModuleArg;      
  char **azModuleArg;  
#endif
  Trigger *pTrigger;   
  Schema *pSchema;     
  Table *pNextZombie;  
};

#define TF_Readonly        0x01    
#define TF_Ephemeral       0x02    
#define TF_HasPrimaryKey   0x04    
#define TF_Autoincrement   0x08    
#define TF_Virtual         0x10    
#define TF_NeedMetadata    0x20    



#ifndef SQLITE_OMIT_VIRTUALTABLE
#  define IsVirtual(X)      (((X)->tabFlags & TF_Virtual)!=0)
#  define IsHiddenColumn(X) ((X)->isHidden)
#else
#  define IsVirtual(X)      0
#  define IsHiddenColumn(X) 0
#endif

struct FKey {
  Table *pFrom;     
  FKey *pNextFrom;  
  char *zTo;        
  FKey *pNextTo;    
  FKey *pPrevTo;    
  int nCol;         
  
  u8 isDeferred;    
  u8 aAction[2];          
  Trigger *apTrigger[2];  
  struct sColMap {  
    int iFrom;         
    char *zCol;        
  } aCol[1];        
};

#define OE_None     0   
#define OE_Rollback 1   
#define OE_Abort    2   
#define OE_Fail     3   
#define OE_Ignore   4   
#define OE_Replace  5   

#define OE_Restrict 6   
#define OE_SetNull  7   
#define OE_SetDflt  8   
#define OE_Cascade  9   

#define OE_Default  99  


struct KeyInfo {
  sqlite3 *db;        
  u8 enc;             
  u16 nField;         
  u8 *aSortOrder;     
  CollSeq *aColl[1];  
};

struct UnpackedRecord {
  KeyInfo *pKeyInfo;  
  u16 nField;         
  u16 flags;          
  i64 rowid;          
  Mem *aMem;          
};

#define UNPACKED_NEED_FREE     0x0001  
#define UNPACKED_NEED_DESTROY  0x0002  
#define UNPACKED_IGNORE_ROWID  0x0004  
#define UNPACKED_INCRKEY       0x0008  
#define UNPACKED_PREFIX_MATCH  0x0010  
#define UNPACKED_PREFIX_SEARCH 0x0020  

struct Index {
  char *zName;     
  int nColumn;     
  int *aiColumn;   
  unsigned *aiRowEst; 
  Table *pTable;   
  int tnum;        
  u8 onError;      
  u8 autoIndex;    
  u8 bUnordered;   
  char *zColAff;   
  Index *pNext;    
  Schema *pSchema; 
  u8 *aSortOrder;  
  char **azColl;   
  IndexSample *aSample;    
};

struct IndexSample {
  union {
    char *z;        
    double r;       
  } u;
  u8 eType;         
  u8 nByte;         
};

struct Token {
  const char *z;     
  unsigned int n;    
};

struct AggInfo {
  u8 directMode;          
  u8 useSortingIdx;       
  int sortingIdx;         
  ExprList *pGroupBy;     
  int nSortingColumn;     
  struct AggInfo_col {    
    Table *pTab;             
    int iTable;              
    int iColumn;             
    int iSorterColumn;       
    int iMem;                
    Expr *pExpr;             
  } *aCol;
  int nColumn;            
  int nColumnAlloc;       
  int nAccumulator;       
  struct AggInfo_func {   
    Expr *pExpr;             
    FuncDef *pFunc;          
    int iMem;                
    int iDistinct;           
  } *aFunc;
  int nFunc;              
  int nFuncAlloc;         
};

#if SQLITE_MAX_VARIABLE_NUMBER<=32767
typedef i16 ynVar;
#else
typedef int ynVar;
#endif

struct Expr {
  u8 op;                 
  char affinity;         
  u16 flags;             
  union {
    char *zToken;          
    int iValue;            
  } u;


  Expr *pLeft;           
  Expr *pRight;          
  union {
    ExprList *pList;     
    Select *pSelect;     
  } x;
  CollSeq *pColl;        


  int iTable;            
  ynVar iColumn;         
  i16 iAgg;              
  i16 iRightJoinTable;   
  u8 flags2;             
  u8 op2;                
  AggInfo *pAggInfo;     
  Table *pTab;           
#if SQLITE_MAX_EXPR_DEPTH>0
  int nHeight;           
#endif
};

#define EP_FromJoin   0x0001  
#define EP_Agg        0x0002  
#define EP_Resolved   0x0004  
#define EP_Error      0x0008  
#define EP_Distinct   0x0010  
#define EP_VarSelect  0x0020  
#define EP_DblQuoted  0x0040  
#define EP_InfixFunc  0x0080  
#define EP_ExpCollate 0x0100  
#define EP_FixedDest  0x0200  
#define EP_IntValue   0x0400  
#define EP_xIsSelect  0x0800  

#define EP_Reduced    0x1000  
#define EP_TokenOnly  0x2000  
#define EP_Static     0x4000  

#define EP2_MallocedToken  0x0001  
#define EP2_Irreducible    0x0002  

#ifdef SQLITE_DEBUG
# define ExprSetIrreducible(X)  (X)->flags2 |= EP2_Irreducible
#else
# define ExprSetIrreducible(X)
#endif

#define ExprHasProperty(E,P)     (((E)->flags&(P))==(P))
#define ExprHasAnyProperty(E,P)  (((E)->flags&(P))!=0)
#define ExprSetProperty(E,P)     (E)->flags|=(P)
#define ExprClearProperty(E,P)   (E)->flags&=~(P)

#define EXPR_FULLSIZE           sizeof(Expr)           
#define EXPR_REDUCEDSIZE        offsetof(Expr,iTable)  
#define EXPR_TOKENONLYSIZE      offsetof(Expr,pLeft)   

#define EXPRDUP_REDUCE         0x0001  

struct ExprList {
  int nExpr;             
  int nAlloc;            
  int iECursor;          
  struct ExprList_item {
    Expr *pExpr;           
    char *zName;           
    char *zSpan;           
    u8 sortOrder;          
    u8 done;               
    u16 iCol;              
    u16 iAlias;            
  } *a;                  
};

struct ExprSpan {
  Expr *pExpr;          
  const char *zStart;   
  const char *zEnd;     
};

struct IdList {
  struct IdList_item {
    char *zName;      
    int idx;          
  } *a;
  int nId;         
  int nAlloc;      
};

typedef u64 Bitmask;

#define BMS  ((int)(sizeof(Bitmask)*8))

struct SrcList {
  i16 nSrc;        
  i16 nAlloc;      
  struct SrcList_item {
    char *zDatabase;  
    char *zName;      
    char *zAlias;     
    Table *pTab;      
    Select *pSelect;  
    u8 isPopulated;   
    u8 jointype;      
    u8 notIndexed;    
#ifndef SQLITE_OMIT_EXPLAIN
    u8 iSelectId;     
#endif
    int iCursor;      
    Expr *pOn;        
    IdList *pUsing;   
    Bitmask colUsed;  
    char *zIndex;     
    Index *pIndex;    
  } a[1];             
};

#define JT_INNER     0x0001    
#define JT_CROSS     0x0002    
#define JT_NATURAL   0x0004    
#define JT_LEFT      0x0008    
#define JT_RIGHT     0x0010    
#define JT_OUTER     0x0020    
#define JT_ERROR     0x0040    


struct WherePlan {
  u32 wsFlags;                   
  u32 nEq;                       
  double nRow;                   
  union {
    Index *pIdx;                   
    struct WhereTerm *pTerm;       
    sqlite3_index_info *pVtabIdx;  
  } u;
};

struct WhereLevel {
  WherePlan plan;       
  int iLeftJoin;        
  int iTabCur;          
  int iIdxCur;          
  int addrBrk;          
  int addrNxt;          
  int addrCont;         
  int addrFirst;        
  u8 iFrom;             
  u8 op, p5;            
  int p1, p2;           
  union {               
    struct {
      int nIn;              
      struct InLoop {
        int iCur;              
        int addrInTop;         
      } *aInLoop;           
    } in;                 
  } u;

  sqlite3_index_info *pIdxInfo;  
};

#define WHERE_ORDERBY_NORMAL   0x0000 
#define WHERE_ORDERBY_MIN      0x0001 
#define WHERE_ORDERBY_MAX      0x0002 
#define WHERE_ONEPASS_DESIRED  0x0004 
#define WHERE_DUPLICATES_OK    0x0008 
#define WHERE_OMIT_OPEN        0x0010 
#define WHERE_OMIT_CLOSE       0x0020 
#define WHERE_FORCE_TABLE      0x0040 
#define WHERE_ONETABLE_ONLY    0x0080 

struct WhereInfo {
  Parse *pParse;       
  u16 wctrlFlags;      
  u8 okOnePass;        
  u8 untestedTerms;    
  SrcList *pTabList;             
  int iTop;                      
  int iContinue;                 
  int iBreak;                    
  int nLevel;                    
  struct WhereClause *pWC;       
  double savedNQueryLoop;        
  double nRowOut;                
  WhereLevel a[1];               
};

struct NameContext {
  Parse *pParse;       
  SrcList *pSrcList;   
  ExprList *pEList;    
  int nRef;            
  int nErr;            
  u8 allowAgg;         
  u8 hasAgg;           
  u8 isCheck;          
  int nDepth;          
  AggInfo *pAggInfo;   
  NameContext *pNext;  
};

struct Select {
  ExprList *pEList;      
  u8 op;                 
  char affinity;         
  u16 selFlags;          
  SrcList *pSrc;         
  Expr *pWhere;          
  ExprList *pGroupBy;    
  Expr *pHaving;         
  ExprList *pOrderBy;    
  Select *pPrior;        
  Select *pNext;         
  Select *pRightmost;    
  Expr *pLimit;          
  Expr *pOffset;         
  int iLimit, iOffset;   
  int addrOpenEphm[3];   
  double nSelectRow;     
};

#define SF_Distinct        0x0001  
#define SF_Resolved        0x0002  
#define SF_Aggregate       0x0004  
#define SF_UsesEphemeral   0x0008  
#define SF_Expanded        0x0010  
#define SF_HasTypeInfo     0x0020  


#define SRT_Union        1  
#define SRT_Except       2  
#define SRT_Exists       3  
#define SRT_Discard      4  

#define IgnorableOrderby(X) ((X->eDest)<=SRT_Discard)

#define SRT_Output       5  
#define SRT_Mem          6  
#define SRT_Set          7  
#define SRT_Table        8  
#define SRT_EphemTab     9  
#define SRT_Coroutine   10  

typedef struct SelectDest SelectDest;
struct SelectDest {
  u8 eDest;         
  u8 affinity;      
  int iParm;        
  int iMem;         /* Base register where results are written */
  int nMem;         
};

struct AutoincInfo {
  AutoincInfo *pNext;   
  Table *pTab;          
  int iDb;              
  int regCtr;           
};

#ifndef SQLITE_N_COLCACHE
# define SQLITE_N_COLCACHE 10
#endif

struct TriggerPrg {
  Trigger *pTrigger;      
  int orconf;             
  SubProgram *pProgram;   
  u32 aColmask[2];        
  TriggerPrg *pNext;      
};

#if SQLITE_MAX_ATTACHED>30
  typedef sqlite3_uint64 yDbMask;
#else
  typedef unsigned int yDbMask;
#endif

struct Parse {
  sqlite3 *db;         
  int rc;              
  char *zErrMsg;       
  Vdbe *pVdbe;         
  u8 colNamesSet;      
  u8 nameClash;        
  u8 checkSchema;      
  u8 nested;           
  u8 parseError;       
  u8 nTempReg;         
  u8 nTempInUse;       
  int aTempReg[8];     
  int nRangeReg;       
  int iRangeReg;       
  int nErr;            
  int nTab;            
  int nMem;            
  int nSet;            
  int ckBase;          
  int iCacheLevel;     
  int iCacheCnt;       
  u8 nColCache;        
  u8 iColCache;        
  struct yColCache {
    int iTable;           
    int iColumn;          
    u8 tempReg;           
    int iLevel;           
    int iReg;             
    int lru;              
  } aColCache[SQLITE_N_COLCACHE];  
  yDbMask writeMask;   
  yDbMask cookieMask;  
  u8 isMultiWrite;     
  u8 mayAbort;         
  int cookieGoto;      
  int cookieValue[SQLITE_MAX_ATTACHED+2];  
#ifndef SQLITE_OMIT_SHARED_CACHE
  int nTableLock;        
  TableLock *aTableLock; 
#endif
  int regRowid;        
  int regRoot;         
  AutoincInfo *pAinc;  
  int nMaxArg;         

  
  Parse *pToplevel;    
  Table *pTriggerTab;  
  u32 oldmask;         
  u32 newmask;         
  u8 eTriggerOp;       
  u8 eOrconf;          
  u8 disableTriggers;  
  double nQueryLoop;   


  int nVar;            
  int nVarExpr;        
  int nVarExprAlloc;   
  Expr **apVarExpr;    
  Vdbe *pReprepare;    
  int nAlias;          
  int nAliasAlloc;     
  int *aAlias;         
  u8 explain;          
  Token sNameToken;    
  Token sLastToken;    
  const char *zTail;   
  Table *pNewTable;    
  Trigger *pNewTrigger;     
  const char *zAuthContext; 
#ifndef SQLITE_OMIT_VIRTUALTABLE
  Token sArg;                
  u8 declareVtab;            
  int nVtabLock;             
  Table **apVtabLock;        
#endif
  int nHeight;            
  Table *pZombieTab;      
  TriggerPrg *pTriggerPrg;    

#ifndef SQLITE_OMIT_EXPLAIN
  int iSelectId;
  int iNextSelectId;
#endif
};

#ifdef SQLITE_OMIT_VIRTUALTABLE
  #define IN_DECLARE_VTAB 0
#else
  #define IN_DECLARE_VTAB (pParse->declareVtab)
#endif

struct AuthContext {
  const char *zAuthContext;   
  Parse *pParse;              
};

#define OPFLAG_NCHANGE       0x01    
#define OPFLAG_LASTROWID     0x02    
#define OPFLAG_ISUPDATE      0x04    
#define OPFLAG_APPEND        0x08    
#define OPFLAG_USESEEKRESULT 0x10    
#define OPFLAG_CLEARCACHE    0x20    

struct Trigger {
  char *zName;            
  char *table;            
  u8 op;                  
  u8 tr_tm;               
  Expr *pWhen;            
  IdList *pColumns;       
  Schema *pSchema;        
  Schema *pTabSchema;     
  TriggerStep *step_list; 
  Trigger *pNext;         
};

#define TRIGGER_BEFORE  1
#define TRIGGER_AFTER   2

struct TriggerStep {
  u8 op;               
  u8 orconf;           
  Trigger *pTrig;      
  Select *pSelect;     
  Token target;        
  Expr *pWhere;        
  ExprList *pExprList; 
  IdList *pIdList;     
  TriggerStep *pNext;  
  TriggerStep *pLast;  
};

typedef struct DbFixer DbFixer;
struct DbFixer {
  Parse *pParse;      /* The parsing context.  Error messages written here */
  const char *zDb;    
  const char *zType;  
  const Token *pName; 
};

struct StrAccum {
  sqlite3 *db;         
  char *zBase;         
  char *zText;         
  int  nChar;          
  int  nAlloc;         
  int  mxAlloc;        
  u8   mallocFailed;   
  u8   useMalloc;      
  u8   tooBig;         
};

typedef struct {
  sqlite3 *db;        
  int iDb;            
  char **pzErrMsg;    
  int rc;             
} InitData;

struct Sqlite3Config {
  int bMemstat;                     
  int bCoreMutex;                   
  int bFullMutex;                   
  int mxStrlen;                     
  int szLookaside;                  
  int nLookaside;                   
  sqlite3_mem_methods m;            
  sqlite3_mutex_methods mutex;      
  sqlite3_pcache_methods pcache;    
  void *pHeap;                      
  int nHeap;                        
  int mnReq, mxReq;                 
  void *pScratch;                   
  int szScratch;                    
  int nScratch;                     
  void *pPage;                      
  int szPage;                       
  int nPage;                        
  int mxParserStack;                
  int sharedCacheEnabled;           
  int isInit;                       
  int inProgress;                   
  int isMutexInit;                  
  int isMallocInit;                 
  int isPCacheInit;                 
  sqlite3_mutex *pInitMutex;        
  int nRefInitMutex;                
  void (*xLog)(void*,int,const char*); 
  void *pLogArg;                       
};

struct Walker {
  int (*xExprCallback)(Walker*, Expr*);     
  int (*xSelectCallback)(Walker*,Select*);  
  Parse *pParse;                            
  union {                                   
    NameContext *pNC;                          
    int i;                                     
  } u;
};

int sqlite3WalkExpr(Walker*, Expr*);
int sqlite3WalkExprList(Walker*, ExprList*);
int sqlite3WalkSelect(Walker*, Select*);
int sqlite3WalkSelectExpr(Walker*, Select*);
int sqlite3WalkSelectFrom(Walker*, Select*);

#define WRC_Continue    0   
#define WRC_Prune       1   
#define WRC_Abort       2   

#define SQLITE_SKIP_UTF8(zIn) {                        \
  if( (*(zIn++))>=0xc0 ){                              \
    while( (*zIn & 0xc0)==0x80 ){ zIn++; }             \
  }                                                    \
}

int sqlite3CorruptError(int);
int sqlite3MisuseError(int);
int sqlite3CantopenError(int);
#define SQLITE_CORRUPT_BKPT sqlite3CorruptError(__LINE__)
#define SQLITE_MISUSE_BKPT sqlite3MisuseError(__LINE__)
#define SQLITE_CANTOPEN_BKPT sqlite3CantopenError(__LINE__)


#if defined(SQLITE_ENABLE_FTS4) && !defined(SQLITE_ENABLE_FTS3)
# define SQLITE_ENABLE_FTS3
#endif

#if !defined(SQLITE_ASCII) || \
    (defined(SQLITE_ENABLE_FTS3) && defined(SQLITE_AMALGAMATION))
# include <ctype.h>
#endif

#if defined(__APPLE__)
#include <CoreServices/CoreServices.h>
#include <CoreFoundation/CoreFoundation.h>
#endif

#ifdef SQLITE_ASCII
# define sqlite3Toupper(x)  ((x)&~(sqlite3CtypeMap[(unsigned char)(x)]&0x20))
# define sqlite3Isspace(x)   (sqlite3CtypeMap[(unsigned char)(x)]&0x01)
# define sqlite3Isalnum(x)   (sqlite3CtypeMap[(unsigned char)(x)]&0x06)
# define sqlite3Isalpha(x)   (sqlite3CtypeMap[(unsigned char)(x)]&0x02)
# define sqlite3Isdigit(x)   (sqlite3CtypeMap[(unsigned char)(x)]&0x04)
# define sqlite3Isxdigit(x)  (sqlite3CtypeMap[(unsigned char)(x)]&0x08)
# define sqlite3Tolower(x)   (sqlite3UpperToLower[(unsigned char)(x)])
#else
# define sqlite3Toupper(x)   toupper((unsigned char)(x))
# define sqlite3Isspace(x)   isspace((unsigned char)(x))
# define sqlite3Isalnum(x)   isalnum((unsigned char)(x))
# define sqlite3Isalpha(x)   isalpha((unsigned char)(x))
# define sqlite3Isdigit(x)   isdigit((unsigned char)(x))
# define sqlite3Isxdigit(x)  isxdigit((unsigned char)(x))
# define sqlite3Tolower(x)   tolower((unsigned char)(x))
#endif

int sqlite3StrICmp(const char *, const char *);
int sqlite3Strlen30(const char*);
#define sqlite3StrNICmp sqlite3_strnicmp

int sqlite3MallocInit(void);
void sqlite3MallocEnd(void);
void *sqlite3Malloc(int);
void *sqlite3MallocZero(int);
void *sqlite3DbMallocZero(sqlite3*, int);
void *sqlite3DbMallocRaw(sqlite3*, int);
char *sqlite3DbStrDup(sqlite3*,const char*);
char *sqlite3DbStrNDup(sqlite3*,const char*, int);
void *sqlite3Realloc(void*, int);
void *sqlite3DbReallocOrFree(sqlite3 *, void *, int);
void *sqlite3DbRealloc(sqlite3 *, void *, int);
void sqlite3DbFree(sqlite3*, void*);
int sqlite3MallocSize(void*);
int sqlite3DbMallocSize(sqlite3*, void*);
void *sqlite3ScratchMalloc(int);
void sqlite3ScratchFree(void*);
void *sqlite3PageMalloc(int);
void sqlite3PageFree(void*);
void sqlite3MemSetDefault(void);
void sqlite3BenignMallocHooks(void (*)(void), void (*)(void));
int sqlite3HeapNearlyFull(void);

#ifdef SQLITE_USE_ALLOCA
# define sqlite3StackAllocRaw(D,N)   alloca(N)
# define sqlite3StackAllocZero(D,N)  memset(alloca(N), 0, N)
# define sqlite3StackFree(D,P)       
#else
# define sqlite3StackAllocRaw(D,N)   sqlite3DbMallocRaw(D,N)
# define sqlite3StackAllocZero(D,N)  sqlite3DbMallocZero(D,N)
# define sqlite3StackFree(D,P)       sqlite3DbFree(D,P)
#endif

#ifdef SQLITE_ENABLE_MEMSYS3
const sqlite3_mem_methods *sqlite3MemGetMemsys3(void);
#endif
#ifdef SQLITE_ENABLE_MEMSYS5
const sqlite3_mem_methods *sqlite3MemGetMemsys5(void);
#endif


#ifndef SQLITE_MUTEX_OMIT
  sqlite3_mutex_methods const *sqlite3DefaultMutex(void);
  sqlite3_mutex_methods const *sqlite3NoopMutex(void);
  sqlite3_mutex *sqlite3MutexAlloc(int);
  int sqlite3MutexInit(void);
  int sqlite3MutexEnd(void);
#endif

int sqlite3StatusValue(int);
void sqlite3StatusAdd(int, int);
void sqlite3StatusSet(int, int);

#ifndef SQLITE_OMIT_FLOATING_POINT
  int sqlite3IsNaN(double);
#else
# define sqlite3IsNaN(X)  0
#endif

void sqlite3VXPrintf(StrAccum*, int, const char*, va_list);
#ifndef SQLITE_OMIT_TRACE
void sqlite3XPrintf(StrAccum*, const char*, ...);
#endif
char *sqlite3MPrintf(sqlite3*,const char*, ...);
char *sqlite3VMPrintf(sqlite3*,const char*, va_list);
char *sqlite3MAppendf(sqlite3*,char*,const char*,...);
#if defined(SQLITE_TEST) || defined(SQLITE_DEBUG)
  void sqlite3DebugPrintf(const char*, ...);
#endif
#if defined(SQLITE_TEST)
  void *sqlite3TestTextToPtr(const char*);
#endif
void sqlite3SetString(char **, sqlite3*, const char*, ...);
void sqlite3ErrorMsg(Parse*, const char*, ...);
int sqlite3Dequote(char*);
int sqlite3KeywordCode(const unsigned char*, int);
int sqlite3RunParser(Parse*, const char*, char **);
void sqlite3FinishCoding(Parse*);
int sqlite3GetTempReg(Parse*);
void sqlite3ReleaseTempReg(Parse*,int);
int sqlite3GetTempRange(Parse*,int);
void sqlite3ReleaseTempRange(Parse*,int,int);
Expr *sqlite3ExprAlloc(sqlite3*,int,const Token*,int);
Expr *sqlite3Expr(sqlite3*,int,const char*);
void sqlite3ExprAttachSubtrees(sqlite3*,Expr*,Expr*,Expr*);
Expr *sqlite3PExpr(Parse*, int, Expr*, Expr*, const Token*);
Expr *sqlite3ExprAnd(sqlite3*,Expr*, Expr*);
Expr *sqlite3ExprFunction(Parse*,ExprList*, Token*);
void sqlite3ExprAssignVarNumber(Parse*, Expr*);
void sqlite3ExprDelete(sqlite3*, Expr*);
ExprList *sqlite3ExprListAppend(Parse*,ExprList*,Expr*);
void sqlite3ExprListSetName(Parse*,ExprList*,Token*,int);
void sqlite3ExprListSetSpan(Parse*,ExprList*,ExprSpan*);
void sqlite3ExprListDelete(sqlite3*, ExprList*);
int sqlite3Init(sqlite3*, char**);
int sqlite3InitCallback(void*, int, char**, char**);
void sqlite3Pragma(Parse*,Token*,Token*,Token*,int);
void sqlite3ResetInternalSchema(sqlite3*, int);
void sqlite3BeginParse(Parse*,int);
void sqlite3CommitInternalChanges(sqlite3*);
Table *sqlite3ResultSetOfSelect(Parse*,Select*);
void sqlite3OpenMasterTable(Parse *, int);
void sqlite3StartTable(Parse*,Token*,Token*,int,int,int,int);
void sqlite3AddColumn(Parse*,Token*);
void sqlite3AddNotNull(Parse*, int);
void sqlite3AddPrimaryKey(Parse*, ExprList*, int, int, int);
void sqlite3AddCheckConstraint(Parse*, Expr*);
void sqlite3AddColumnType(Parse*,Token*);
void sqlite3AddDefaultValue(Parse*,ExprSpan*);
void sqlite3AddCollateType(Parse*, Token*);
void sqlite3EndTable(Parse*,Token*,Token*,Select*);

Bitvec *sqlite3BitvecCreate(u32);
int sqlite3BitvecTest(Bitvec*, u32);
int sqlite3BitvecSet(Bitvec*, u32);
void sqlite3BitvecClear(Bitvec*, u32, void*);
void sqlite3BitvecDestroy(Bitvec*);
u32 sqlite3BitvecSize(Bitvec*);
int sqlite3BitvecBuiltinTest(int,int*);

RowSet *sqlite3RowSetInit(sqlite3*, void*, unsigned int);
void sqlite3RowSetClear(RowSet*);
void sqlite3RowSetInsert(RowSet*, i64);
int sqlite3RowSetTest(RowSet*, u8 iBatch, i64);
int sqlite3RowSetNext(RowSet*, i64*);

void sqlite3CreateView(Parse*,Token*,Token*,Token*,Select*,int,int);

#if !defined(SQLITE_OMIT_VIEW) || !defined(SQLITE_OMIT_VIRTUALTABLE)
  int sqlite3ViewGetColumnNames(Parse*,Table*);
#else
# define sqlite3ViewGetColumnNames(A,B) 0
#endif

void sqlite3DropTable(Parse*, SrcList*, int, int);
void sqlite3DeleteTable(sqlite3*, Table*);
#ifndef SQLITE_OMIT_AUTOINCREMENT
  void sqlite3AutoincrementBegin(Parse *pParse);
  void sqlite3AutoincrementEnd(Parse *pParse);
#else
# define sqlite3AutoincrementBegin(X)
# define sqlite3AutoincrementEnd(X)
#endif
void sqlite3Insert(Parse*, SrcList*, ExprList*, Select*, IdList*, int);
void *sqlite3ArrayAllocate(sqlite3*,void*,int,int,int*,int*,int*);
IdList *sqlite3IdListAppend(sqlite3*, IdList*, Token*);
int sqlite3IdListIndex(IdList*,const char*);
SrcList *sqlite3SrcListEnlarge(sqlite3*, SrcList*, int, int);
SrcList *sqlite3SrcListAppend(sqlite3*, SrcList*, Token*, Token*);
SrcList *sqlite3SrcListAppendFromTerm(Parse*, SrcList*, Token*, Token*,
                                      Token*, Select*, Expr*, IdList*);
void sqlite3SrcListIndexedBy(Parse *, SrcList *, Token *);
int sqlite3IndexedByLookup(Parse *, struct SrcList_item *);
void sqlite3SrcListShiftJoinType(SrcList*);
void sqlite3SrcListAssignCursors(Parse*, SrcList*);
void sqlite3IdListDelete(sqlite3*, IdList*);
void sqlite3SrcListDelete(sqlite3*, SrcList*);
Index *sqlite3CreateIndex(Parse*,Token*,Token*,SrcList*,ExprList*,int,Token*,
                        Token*, int, int);
void sqlite3DropIndex(Parse*, SrcList*, int);
int sqlite3Select(Parse*, Select*, SelectDest*);
Select *sqlite3SelectNew(Parse*,ExprList*,SrcList*,Expr*,ExprList*,
                         Expr*,ExprList*,int,Expr*,Expr*);
void sqlite3SelectDelete(sqlite3*, Select*);
Table *sqlite3SrcListLookup(Parse*, SrcList*);
int sqlite3IsReadOnly(Parse*, Table*, int);
void sqlite3OpenTable(Parse*, int iCur, int iDb, Table*, int);
#if defined(SQLITE_ENABLE_UPDATE_DELETE_LIMIT) && !defined(SQLITE_OMIT_SUBQUERY)
Expr *sqlite3LimitWhere(Parse *, SrcList *, Expr *, ExprList *, Expr *, Expr *, char *);
#endif
void sqlite3DeleteFrom(Parse*, SrcList*, Expr*);
void sqlite3Update(Parse*, SrcList*, ExprList*, Expr*, int);
WhereInfo *sqlite3WhereBegin(Parse*, SrcList*, Expr*, ExprList**, u16);
void sqlite3WhereEnd(WhereInfo*);
int sqlite3ExprCodeGetColumn(Parse*, Table*, int, int, int);
void sqlite3ExprCodeGetColumnOfTable(Vdbe*, Table*, int, int, int);
void sqlite3ExprCodeMove(Parse*, int, int, int);
void sqlite3ExprCodeCopy(Parse*, int, int, int);
void sqlite3ExprCacheStore(Parse*, int, int, int);
void sqlite3ExprCachePush(Parse*);
void sqlite3ExprCachePop(Parse*, int);
void sqlite3ExprCacheRemove(Parse*, int, int);
void sqlite3ExprCacheClear(Parse*);
void sqlite3ExprCacheAffinityChange(Parse*, int, int);
int sqlite3ExprCode(Parse*, Expr*, int);
int sqlite3ExprCodeTemp(Parse*, Expr*, int*);
int sqlite3ExprCodeTarget(Parse*, Expr*, int);
int sqlite3ExprCodeAndCache(Parse*, Expr*, int);
void sqlite3ExprCodeConstants(Parse*, Expr*);
int sqlite3ExprCodeExprList(Parse*, ExprList*, int, int);
void sqlite3ExprIfTrue(Parse*, Expr*, int, int);
void sqlite3ExprIfFalse(Parse*, Expr*, int, int);
Table *sqlite3FindTable(sqlite3*,const char*, const char*);
Table *sqlite3LocateTable(Parse*,int isView,const char*, const char*);
Index *sqlite3FindIndex(sqlite3*,const char*, const char*);
void sqlite3UnlinkAndDeleteTable(sqlite3*,int,const char*);
void sqlite3UnlinkAndDeleteIndex(sqlite3*,int,const char*);
void sqlite3Vacuum(Parse*);
int sqlite3RunVacuum(char**, sqlite3*);
char *sqlite3NameFromToken(sqlite3*, Token*);
int sqlite3ExprCompare(Expr*, Expr*);
int sqlite3ExprListCompare(ExprList*, ExprList*);
void sqlite3ExprAnalyzeAggregates(NameContext*, Expr*);
void sqlite3ExprAnalyzeAggList(NameContext*,ExprList*);
Vdbe *sqlite3GetVdbe(Parse*);
void sqlite3PrngSaveState(void);
void sqlite3PrngRestoreState(void);
void sqlite3PrngResetState(void);
void sqlite3RollbackAll(sqlite3*);
void sqlite3CodeVerifySchema(Parse*, int);
void sqlite3CodeVerifyNamedSchema(Parse*, const char *zDb);
void sqlite3BeginTransaction(Parse*, int);
void sqlite3CommitTransaction(Parse*);
void sqlite3RollbackTransaction(Parse*);
void sqlite3Savepoint(Parse*, int, Token*);
void sqlite3CloseSavepoints(sqlite3 *);
int sqlite3ExprIsConstant(Expr*);
int sqlite3ExprIsConstantNotJoin(Expr*);
int sqlite3ExprIsConstantOrFunction(Expr*);
int sqlite3ExprIsInteger(Expr*, int*);
int sqlite3ExprCanBeNull(const Expr*);
void sqlite3ExprCodeIsNullJump(Vdbe*, const Expr*, int, int);
int sqlite3ExprNeedsNoAffinityChange(const Expr*, char);
int sqlite3IsRowid(const char*);
void sqlite3GenerateRowDelete(Parse*, Table*, int, int, int, Trigger *, int);
void sqlite3GenerateRowIndexDelete(Parse*, Table*, int, int*);
int sqlite3GenerateIndexKey(Parse*, Index*, int, int, int);
void sqlite3GenerateConstraintChecks(Parse*,Table*,int,int,
                                     int*,int,int,int,int,int*);
void sqlite3CompleteInsertion(Parse*, Table*, int, int, int*, int, int, int);
int sqlite3OpenTableAndIndices(Parse*, Table*, int, int);
void sqlite3BeginWriteOperation(Parse*, int, int);
void sqlite3MultiWrite(Parse*);
void sqlite3MayAbort(Parse*);
void sqlite3HaltConstraint(Parse*, int, char*, int);
Expr *sqlite3ExprDup(sqlite3*,Expr*,int);
ExprList *sqlite3ExprListDup(sqlite3*,ExprList*,int);
SrcList *sqlite3SrcListDup(sqlite3*,SrcList*,int);
IdList *sqlite3IdListDup(sqlite3*,IdList*);
Select *sqlite3SelectDup(sqlite3*,Select*,int);
void sqlite3FuncDefInsert(FuncDefHash*, FuncDef*);
FuncDef *sqlite3FindFunction(sqlite3*,const char*,int,int,u8,int);
void sqlite3RegisterBuiltinFunctions(sqlite3*);
void sqlite3RegisterDateTimeFunctions(void);
void sqlite3RegisterGlobalFunctions(void);
int sqlite3SafetyCheckOk(sqlite3*);
int sqlite3SafetyCheckSickOrOk(sqlite3*);
void sqlite3ChangeCookie(Parse*, int);

#if !defined(SQLITE_OMIT_VIEW) && !defined(SQLITE_OMIT_TRIGGER)
void sqlite3MaterializeView(Parse*, Table*, Expr*, int);
#endif

#ifndef SQLITE_OMIT_TRIGGER
  void sqlite3BeginTrigger(Parse*, Token*,Token*,int,int,IdList*,SrcList*,
                           Expr*,int, int);
  void sqlite3FinishTrigger(Parse*, TriggerStep*, Token*);
  void sqlite3DropTrigger(Parse*, SrcList*, int);
  void sqlite3DropTriggerPtr(Parse*, Trigger*);
  Trigger *sqlite3TriggersExist(Parse *, Table*, int, ExprList*, int *pMask);
  Trigger *sqlite3TriggerList(Parse *, Table *);
  void sqlite3CodeRowTrigger(Parse*, Trigger *, int, ExprList*, int, Table *,
                            int, int, int);
  void sqlite3CodeRowTriggerDirect(Parse *, Trigger *, Table *, int, int, int);
  void sqliteViewTriggers(Parse*, Table*, Expr*, int, ExprList*);
  void sqlite3DeleteTriggerStep(sqlite3*, TriggerStep*);
  TriggerStep *sqlite3TriggerSelectStep(sqlite3*,Select*);
  TriggerStep *sqlite3TriggerInsertStep(sqlite3*,Token*, IdList*,
                                        ExprList*,Select*,u8);
  TriggerStep *sqlite3TriggerUpdateStep(sqlite3*,Token*,ExprList*, Expr*, u8);
  TriggerStep *sqlite3TriggerDeleteStep(sqlite3*,Token*, Expr*);
  void sqlite3DeleteTrigger(sqlite3*, Trigger*);
  void sqlite3UnlinkAndDeleteTrigger(sqlite3*,int,const char*);
  u32 sqlite3TriggerColmask(Parse*,Trigger*,ExprList*,int,int,Table*,int);
# define sqlite3ParseToplevel(p) ((p)->pToplevel ? (p)->pToplevel : (p))
#else
# define sqlite3TriggersExist(B,C,D,E,F) 0
# define sqlite3DeleteTrigger(A,B)
# define sqlite3DropTriggerPtr(A,B)
# define sqlite3UnlinkAndDeleteTrigger(A,B,C)
# define sqlite3CodeRowTrigger(A,B,C,D,E,F,G,H,I)
# define sqlite3CodeRowTriggerDirect(A,B,C,D,E,F)
# define sqlite3TriggerList(X, Y) 0
# define sqlite3ParseToplevel(p) p
# define sqlite3TriggerColmask(A,B,C,D,E,F,G) 0
#endif

int sqlite3JoinType(Parse*, Token*, Token*, Token*);
void sqlite3CreateForeignKey(Parse*, ExprList*, Token*, ExprList*, int);
void sqlite3DeferForeignKey(Parse*, int);
#ifndef SQLITE_OMIT_AUTHORIZATION
  void sqlite3AuthRead(Parse*,Expr*,Schema*,SrcList*);
  int sqlite3AuthCheck(Parse*,int, const char*, const char*, const char*);
  void sqlite3AuthContextPush(Parse*, AuthContext*, const char*);
  void sqlite3AuthContextPop(AuthContext*);
  int sqlite3AuthReadCol(Parse*, const char *, const char *, int);
#else
# define sqlite3AuthRead(a,b,c,d)
# define sqlite3AuthCheck(a,b,c,d,e)    SQLITE_OK
# define sqlite3AuthContextPush(a,b,c)
# define sqlite3AuthContextPop(a)  ((void)(a))
#endif
void sqlite3Attach(Parse*, Expr*, Expr*, Expr*);
void sqlite3Detach(Parse*, Expr*);
int sqlite3FixInit(DbFixer*, Parse*, int, const char*, const Token*);
int sqlite3FixSrcList(DbFixer*, SrcList*);
int sqlite3FixSelect(DbFixer*, Select*);
int sqlite3FixExpr(DbFixer*, Expr*);
int sqlite3FixExprList(DbFixer*, ExprList*);
int sqlite3FixTriggerStep(DbFixer*, TriggerStep*);
int sqlite3AtoF(const char *z, double*, int, u8);
int sqlite3GetInt32(const char *, int*);
int sqlite3Atoi(const char*);
int sqlite3Utf16ByteLen(const void *pData, int nChar);
int sqlite3Utf8CharLen(const char *pData, int nByte);
int sqlite3Utf8Read(const u8*, const u8**);

int sqlite3PutVarint(unsigned char*, u64);
int sqlite3PutVarint32(unsigned char*, u32);
u8 sqlite3GetVarint(const unsigned char *, u64 *);
u8 sqlite3GetVarint32(const unsigned char *, u32 *);
int sqlite3VarintLen(u64 v);

#define getVarint32(A,B)  (u8)((*(A)<(u8)0x80) ? ((B) = (u32)*(A)),1 : sqlite3GetVarint32((A), (u32 *)&(B)))
#define putVarint32(A,B)  (u8)(((u32)(B)<(u32)0x80) ? (*(A) = (unsigned char)(B)),1 : sqlite3PutVarint32((A), (B)))
#define getVarint    sqlite3GetVarint
#define putVarint    sqlite3PutVarint


const char *sqlite3IndexAffinityStr(Vdbe *, Index *);
void sqlite3TableAffinityStr(Vdbe *, Table *);
char sqlite3CompareAffinity(Expr *pExpr, char aff2);
int sqlite3IndexAffinityOk(Expr *pExpr, char idx_affinity);
char sqlite3ExprAffinity(Expr *pExpr);
int sqlite3Atoi64(const char*, i64*, int, u8);
void sqlite3Error(sqlite3*, int, const char*,...);
void *sqlite3HexToBlob(sqlite3*, const char *z, int n);
int sqlite3TwoPartName(Parse *, Token *, Token *, Token **);
const char *sqlite3ErrStr(int);
int sqlite3ReadSchema(Parse *pParse);
CollSeq *sqlite3FindCollSeq(sqlite3*,u8 enc, const char*,int);
CollSeq *sqlite3LocateCollSeq(Parse *pParse, const char*zName);
CollSeq *sqlite3ExprCollSeq(Parse *pParse, Expr *pExpr);
Expr *sqlite3ExprSetColl(Expr*, CollSeq*);
Expr *sqlite3ExprSetCollByToken(Parse *pParse, Expr*, Token*);
int sqlite3CheckCollSeq(Parse *, CollSeq *);
int sqlite3CheckObjectName(Parse *, const char *);
void sqlite3VdbeSetChanges(sqlite3 *, int);
int sqlite3AddInt64(i64*,i64);
int sqlite3SubInt64(i64*,i64);
int sqlite3MulInt64(i64*,i64);
int sqlite3AbsInt32(int);

const void *sqlite3ValueText(sqlite3_value*, u8);
int sqlite3ValueBytes(sqlite3_value*, u8);
void sqlite3ValueSetStr(sqlite3_value*, int, const void *,u8, 
                        void(*)(void*));
void sqlite3ValueFree(sqlite3_value*);
sqlite3_value *sqlite3ValueNew(sqlite3 *);
char *sqlite3Utf16to8(sqlite3 *, const void*, int, u8);
#ifdef SQLITE_ENABLE_STAT2
char *sqlite3Utf8to16(sqlite3 *, u8, char *, int, int *);
#endif
int sqlite3ValueFromExpr(sqlite3 *, Expr *, u8, u8, sqlite3_value **);
void sqlite3ValueApplyAffinity(sqlite3_value *, u8, u8);
#ifndef SQLITE_AMALGAMATION
extern const unsigned char sqlite3OpcodeProperty[];
extern const unsigned char sqlite3UpperToLower[];
extern const unsigned char sqlite3CtypeMap[];
extern const Token sqlite3IntTokens[];
extern SQLITE_WSD struct Sqlite3Config sqlite3Config;
extern SQLITE_WSD FuncDefHash sqlite3GlobalFunctions;
#ifndef SQLITE_OMIT_WSD
extern int sqlite3PendingByte;
#endif
#endif
void sqlite3RootPageMoved(sqlite3*, int, int, int);
void sqlite3Reindex(Parse*, Token*, Token*);
void sqlite3AlterFunctions(void);
void sqlite3AlterRenameTable(Parse*, SrcList*, Token*);
int sqlite3GetToken(const unsigned char *, int *);
void sqlite3NestedParse(Parse*, const char*, ...);
void sqlite3ExpirePreparedStatements(sqlite3*);
int sqlite3CodeSubselect(Parse *, Expr *, int, int);
void sqlite3SelectPrep(Parse*, Select*, NameContext*);
int sqlite3ResolveExprNames(NameContext*, Expr*);
void sqlite3ResolveSelectNames(Parse*, Select*, NameContext*);
int sqlite3ResolveOrderGroupBy(Parse*, Select*, ExprList*, const char*);
void sqlite3ColumnDefault(Vdbe *, Table *, int, int);
void sqlite3AlterFinishAddColumn(Parse *, Token *);
void sqlite3AlterBeginAddColumn(Parse *, SrcList *);
CollSeq *sqlite3GetCollSeq(sqlite3*, u8, CollSeq *, const char*);
char sqlite3AffinityType(const char*);
void sqlite3Analyze(Parse*, Token*, Token*);
int sqlite3InvokeBusyHandler(BusyHandler*);
int sqlite3FindDb(sqlite3*, Token*);
int sqlite3FindDbName(sqlite3 *, const char *);
int sqlite3AnalysisLoad(sqlite3*,int iDB);
void sqlite3DeleteIndexSamples(sqlite3*,Index*);
void sqlite3DefaultRowEst(Index*);
void sqlite3RegisterLikeFunctions(sqlite3*, int);
int sqlite3IsLikeFunction(sqlite3*,Expr*,int*,char*);
void sqlite3MinimumFileFormat(Parse*, int, int);
void sqlite3SchemaClear(void *);
Schema *sqlite3SchemaGet(sqlite3 *, Btree *);
int sqlite3SchemaToIndex(sqlite3 *db, Schema *);
KeyInfo *sqlite3IndexKeyinfo(Parse *, Index *);
int sqlite3CreateFunc(sqlite3 *, const char *, int, int, void *, 
  void (*)(sqlite3_context*,int,sqlite3_value **),
  void (*)(sqlite3_context*,int,sqlite3_value **), void (*)(sqlite3_context*),
  FuncDestructor *pDestructor
);
int sqlite3ApiExit(sqlite3 *db, int);
int sqlite3OpenTempDatabase(Parse *);

void sqlite3StrAccumInit(StrAccum*, char*, int, int);
void sqlite3StrAccumAppend(StrAccum*,const char*,int);
char *sqlite3StrAccumFinish(StrAccum*);
void sqlite3StrAccumReset(StrAccum*);
void sqlite3SelectDestInit(SelectDest*,int,int);
Expr *sqlite3CreateColumnExpr(sqlite3 *, SrcList *, int, int);

void sqlite3BackupRestart(sqlite3_backup *);
void sqlite3BackupUpdate(sqlite3_backup *, Pgno, const u8 *);

void *sqlite3ParserAlloc(void*(*)(size_t));
void sqlite3ParserFree(void*, void(*)(void*));
void sqlite3Parser(void*, int, Token, Parse*);
#ifdef YYTRACKMAXSTACKDEPTH
  int sqlite3ParserStackPeak(void*);
#endif

void sqlite3AutoLoadExtensions(sqlite3*);
#ifndef SQLITE_OMIT_LOAD_EXTENSION
  void sqlite3CloseExtensions(sqlite3*);
#else
# define sqlite3CloseExtensions(X)
#endif

#ifndef SQLITE_OMIT_SHARED_CACHE
  void sqlite3TableLock(Parse *, int, int, u8, const char *);
#else
  #define sqlite3TableLock(v,w,x,y,z)
#endif

#ifdef SQLITE_TEST
  int sqlite3Utf8To8(unsigned char*);
#endif

#ifdef SQLITE_OMIT_VIRTUALTABLE
#  define sqlite3VtabClear(Y)
#  define sqlite3VtabSync(X,Y) SQLITE_OK
#  define sqlite3VtabRollback(X)
#  define sqlite3VtabCommit(X)
#  define sqlite3VtabInSync(db) 0
#  define sqlite3VtabLock(X) 
#  define sqlite3VtabUnlock(X)
#  define sqlite3VtabUnlockList(X)
#else
   void sqlite3VtabClear(sqlite3 *db, Table*);
   int sqlite3VtabSync(sqlite3 *db, char **);
   int sqlite3VtabRollback(sqlite3 *db);
   int sqlite3VtabCommit(sqlite3 *db);
   void sqlite3VtabLock(VTable *);
   void sqlite3VtabUnlock(VTable *);
   void sqlite3VtabUnlockList(sqlite3*);
#  define sqlite3VtabInSync(db) ((db)->nVTrans>0 && (db)->aVTrans==0)
#endif
void sqlite3VtabMakeWritable(Parse*,Table*);
void sqlite3VtabBeginParse(Parse*, Token*, Token*, Token*);
void sqlite3VtabFinishParse(Parse*, Token*);
void sqlite3VtabArgInit(Parse*);
void sqlite3VtabArgExtend(Parse*, Token*);
int sqlite3VtabCallCreate(sqlite3*, int, const char *, char **);
int sqlite3VtabCallConnect(Parse*, Table*);
int sqlite3VtabCallDestroy(sqlite3*, int, const char *);
int sqlite3VtabBegin(sqlite3 *, VTable *);
FuncDef *sqlite3VtabOverloadFunction(sqlite3 *,FuncDef*, int nArg, Expr*);
void sqlite3InvalidFunction(sqlite3_context*,int,sqlite3_value**);
int sqlite3VdbeParameterIndex(Vdbe*, const char*, int);
int sqlite3TransferBindings(sqlite3_stmt *, sqlite3_stmt *);
int sqlite3Reprepare(Vdbe*);
void sqlite3ExprListCheckLength(Parse*, ExprList*, const char*);
CollSeq *sqlite3BinaryCompareCollSeq(Parse *, Expr *, Expr *);
int sqlite3TempInMemory(const sqlite3*);
VTable *sqlite3GetVTable(sqlite3*, Table*);
const char *sqlite3JournalModename(int);
int sqlite3Checkpoint(sqlite3*, int, int, int*, int*);
int sqlite3WalDefaultHook(void*,sqlite3*,const char*,int);

#if !defined(SQLITE_OMIT_FOREIGN_KEY) && !defined(SQLITE_OMIT_TRIGGER)
  void sqlite3FkCheck(Parse*, Table*, int, int);
  void sqlite3FkDropTable(Parse*, SrcList *, Table*);
  void sqlite3FkActions(Parse*, Table*, ExprList*, int);
  int sqlite3FkRequired(Parse*, Table*, int*, int);
  u32 sqlite3FkOldmask(Parse*, Table*);
  FKey *sqlite3FkReferences(Table *);
#else
  #define sqlite3FkActions(a,b,c,d)
  #define sqlite3FkCheck(a,b,c,d)
  #define sqlite3FkDropTable(a,b,c)
  #define sqlite3FkOldmask(a,b)      0
  #define sqlite3FkRequired(a,b,c,d) 0
#endif
#ifndef SQLITE_OMIT_FOREIGN_KEY
  void sqlite3FkDelete(sqlite3 *, Table*);
#else
  #define sqlite3FkDelete(a,b)
#endif


#define SQLITE_FAULTINJECTOR_MALLOC     0
#define SQLITE_FAULTINJECTOR_COUNT      1

#ifndef SQLITE_OMIT_BUILTIN_TEST
  void sqlite3BeginBenignMalloc(void);
  void sqlite3EndBenignMalloc(void);
#else
  #define sqlite3BeginBenignMalloc()
  #define sqlite3EndBenignMalloc()
#endif

#define IN_INDEX_ROWID           1
#define IN_INDEX_EPH             2
#define IN_INDEX_INDEX           3
int sqlite3FindInIndex(Parse *, Expr *, int*);

#ifdef SQLITE_ENABLE_ATOMIC_WRITE
  int sqlite3JournalOpen(sqlite3_vfs *, const char *, sqlite3_file *, int, int);
  int sqlite3JournalSize(sqlite3_vfs *);
  int sqlite3JournalCreate(sqlite3_file *);
#else
  #define sqlite3JournalSize(pVfs) ((pVfs)->szOsFile)
#endif

void sqlite3MemJournalOpen(sqlite3_file *);
int sqlite3MemJournalSize(void);
int sqlite3IsMemJournal(sqlite3_file *);

#if SQLITE_MAX_EXPR_DEPTH>0
  void sqlite3ExprSetHeight(Parse *pParse, Expr *p);
  int sqlite3SelectExprHeight(Select *);
  int sqlite3ExprCheckHeight(Parse*, int);
#else
  #define sqlite3ExprSetHeight(x,y)
  #define sqlite3SelectExprHeight(x) 0
  #define sqlite3ExprCheckHeight(x,y)
#endif

u32 sqlite3Get4byte(const u8*);
void sqlite3Put4byte(u8*, u32);

#ifdef SQLITE_ENABLE_UNLOCK_NOTIFY
  void sqlite3ConnectionBlocked(sqlite3 *, sqlite3 *);
  void sqlite3ConnectionUnlocked(sqlite3 *db);
  void sqlite3ConnectionClosed(sqlite3 *db);
#else
  #define sqlite3ConnectionBlocked(x,y)
  #define sqlite3ConnectionUnlocked(x)
  #define sqlite3ConnectionClosed(x)
#endif

#ifdef SQLITE_DEBUG
  void sqlite3ParserTrace(FILE*, char *);
#endif

#ifdef SQLITE_ENABLE_IOTRACE
# define IOTRACE(A)  if( sqlite3IoTrace ){ sqlite3IoTrace A; }
  void sqlite3VdbeIOTraceSql(Vdbe*);
SQLITE_EXTERN void (*sqlite3IoTrace)(const char*,...);
#else
# define IOTRACE(A)
# define sqlite3VdbeIOTraceSql(X)
#endif

#ifdef SQLITE_MEMDEBUG
  void sqlite3MemdebugSetType(void*,u8);
  int sqlite3MemdebugHasType(void*,u8);
  int sqlite3MemdebugNoType(void*,u8);
#else
# define sqlite3MemdebugSetType(X,Y)  
# define sqlite3MemdebugHasType(X,Y)  1
# define sqlite3MemdebugNoType(X,Y)   1
#endif
#define MEMTYPE_HEAP       0x01  
#define MEMTYPE_LOOKASIDE  0x02  
#define MEMTYPE_SCRATCH    0x04  
#define MEMTYPE_PCACHE     0x08  
#define MEMTYPE_DB         0x10  

#endif 

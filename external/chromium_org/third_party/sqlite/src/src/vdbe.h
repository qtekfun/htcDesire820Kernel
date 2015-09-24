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
** Header file for the Virtual DataBase Engine (VDBE)
**
** This header defines the interface to the virtual database engine
** or VDBE.  The VDBE implements an abstract machine that runs a
** simple program to access and modify the underlying database.
*/
#ifndef _SQLITE_VDBE_H_
#define _SQLITE_VDBE_H_
#include <stdio.h>

typedef struct Vdbe Vdbe;

typedef struct VdbeFunc VdbeFunc;
typedef struct Mem Mem;
typedef struct SubProgram SubProgram;

struct VdbeOp {
  u8 opcode;          
  signed char p4type; 
  u8 opflags;         
  u8 p5;              
  int p1;             
  int p2;             
  int p3;             
  union {             
    int i;                 
    void *p;               
    char *z;               
    i64 *pI64;             
    double *pReal;         
    FuncDef *pFunc;        
    VdbeFunc *pVdbeFunc;   
    CollSeq *pColl;        
    Mem *pMem;             
    VTable *pVtab;         
    KeyInfo *pKeyInfo;     
    int *ai;               
    SubProgram *pProgram;  
  } p4;
#ifdef SQLITE_DEBUG
  char *zComment;          
#endif
#ifdef VDBE_PROFILE
  int cnt;                 
  u64 cycles;              
#endif
};
typedef struct VdbeOp VdbeOp;


struct SubProgram {
  VdbeOp *aOp;                  
  int nOp;                      
  int nMem;                     
  int nCsr;                     
  void *token;                  
  SubProgram *pNext;            
};

struct VdbeOpList {
  u8 opcode;          
  signed char p1;     
  signed char p2;     
  signed char p3;     
};
typedef struct VdbeOpList VdbeOpList;

#define P4_NOTUSED    0   
#define P4_DYNAMIC  (-1)  
#define P4_STATIC   (-2)  
#define P4_COLLSEQ  (-4)  
#define P4_FUNCDEF  (-5)  
#define P4_KEYINFO  (-6)  
#define P4_VDBEFUNC (-7)  
#define P4_MEM      (-8)  
#define P4_TRANSIENT  0   
#define P4_VTAB     (-10) 
#define P4_MPRINTF  (-11) 
#define P4_REAL     (-12) 
#define P4_INT64    (-13) 
#define P4_INT32    (-14) 
#define P4_INTARRAY (-15) 
#define P4_SUBPROGRAM  (-18) 

#define P4_KEYINFO_HANDOFF (-16)
#define P4_KEYINFO_STATIC  (-17)

#define COLNAME_NAME     0
#define COLNAME_DECLTYPE 1
#define COLNAME_DATABASE 2
#define COLNAME_TABLE    3
#define COLNAME_COLUMN   4
#ifdef SQLITE_ENABLE_COLUMN_METADATA
# define COLNAME_N        5      
#else
# ifdef SQLITE_OMIT_DECLTYPE
#   define COLNAME_N      1      
# else
#   define COLNAME_N      2      
# endif
#endif

#define ADDR(X)  (-1-(X))

#include "opcodes.h"

Vdbe *sqlite3VdbeCreate(sqlite3*);
int sqlite3VdbeAddOp0(Vdbe*,int);
int sqlite3VdbeAddOp1(Vdbe*,int,int);
int sqlite3VdbeAddOp2(Vdbe*,int,int,int);
int sqlite3VdbeAddOp3(Vdbe*,int,int,int,int);
int sqlite3VdbeAddOp4(Vdbe*,int,int,int,int,const char *zP4,int);
int sqlite3VdbeAddOp4Int(Vdbe*,int,int,int,int,int);
int sqlite3VdbeAddOpList(Vdbe*, int nOp, VdbeOpList const *aOp);
void sqlite3VdbeChangeP1(Vdbe*, int addr, int P1);
void sqlite3VdbeChangeP2(Vdbe*, int addr, int P2);
void sqlite3VdbeChangeP3(Vdbe*, int addr, int P3);
void sqlite3VdbeChangeP5(Vdbe*, u8 P5);
void sqlite3VdbeJumpHere(Vdbe*, int addr);
void sqlite3VdbeChangeToNoop(Vdbe*, int addr, int N);
void sqlite3VdbeChangeP4(Vdbe*, int addr, const char *zP4, int N);
void sqlite3VdbeUsesBtree(Vdbe*, int);
VdbeOp *sqlite3VdbeGetOp(Vdbe*, int);
int sqlite3VdbeMakeLabel(Vdbe*);
void sqlite3VdbeRunOnlyOnce(Vdbe*);
void sqlite3VdbeDelete(Vdbe*);
void sqlite3VdbeDeleteObject(sqlite3*,Vdbe*);
void sqlite3VdbeMakeReady(Vdbe*,int,int,int,int,int,int);
int sqlite3VdbeFinalize(Vdbe*);
void sqlite3VdbeResolveLabel(Vdbe*, int);
int sqlite3VdbeCurrentAddr(Vdbe*);
#ifdef SQLITE_DEBUG
  int sqlite3VdbeAssertMayAbort(Vdbe *, int);
  void sqlite3VdbeTrace(Vdbe*,FILE*);
#endif
void sqlite3VdbeResetStepResult(Vdbe*);
int sqlite3VdbeReset(Vdbe*);
void sqlite3VdbeSetNumCols(Vdbe*,int);
int sqlite3VdbeSetColName(Vdbe*, int, int, const char *, void(*)(void*));
void sqlite3VdbeCountChanges(Vdbe*);
sqlite3 *sqlite3VdbeDb(Vdbe*);
void sqlite3VdbeSetSql(Vdbe*, const char *z, int n, int);
void sqlite3VdbeSwap(Vdbe*,Vdbe*);
VdbeOp *sqlite3VdbeTakeOpArray(Vdbe*, int*, int*);
sqlite3_value *sqlite3VdbeGetValue(Vdbe*, int, u8);
void sqlite3VdbeSetVarmask(Vdbe*, int);
#ifndef SQLITE_OMIT_TRACE
  char *sqlite3VdbeExpandSql(Vdbe*, const char*);
#endif

UnpackedRecord *sqlite3VdbeRecordUnpack(KeyInfo*,int,const void*,char*,int);
void sqlite3VdbeDeleteUnpackedRecord(UnpackedRecord*);
int sqlite3VdbeRecordCompare(int,const void*,UnpackedRecord*);

#ifndef SQLITE_OMIT_TRIGGER
void sqlite3VdbeLinkSubProgram(Vdbe *, SubProgram *);
#endif


#ifndef NDEBUG
  void sqlite3VdbeComment(Vdbe*, const char*, ...);
# define VdbeComment(X)  sqlite3VdbeComment X
  void sqlite3VdbeNoopComment(Vdbe*, const char*, ...);
# define VdbeNoopComment(X)  sqlite3VdbeNoopComment X
#else
# define VdbeComment(X)
# define VdbeNoopComment(X)
#endif

#endif

/*
** 2003 September 6
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This is the header file for information that is private to the
** VDBE.  This information used to all be at the top of the single
** source code file "vdbe.c".  When that file became too big (over
** 6000 lines long) it was split up into several smaller files and
** this header information was factored out.
*/
#ifndef _VDBEINT_H_
#define _VDBEINT_H_

typedef struct VdbeOp Op;

typedef unsigned char Bool;

struct VdbeCursor {
  BtCursor *pCursor;    
  Btree *pBt;           
  KeyInfo *pKeyInfo;    
  int iDb;              
  int pseudoTableReg;   
  int nField;           
  Bool zeroed;          
  Bool rowidIsValid;    
  Bool atFirst;         
  Bool useRandomRowid;  
  Bool nullRow;         
  Bool deferredMoveto;  
  Bool isTable;         
  Bool isIndex;         
  Bool isOrdered;       
  sqlite3_vtab_cursor *pVtabCursor;  
  const sqlite3_module *pModule;     
  i64 seqCount;         
  i64 movetoTarget;     
  i64 lastRowid;        

  int seekResult;

  u32 cacheStatus;      
  int payloadSize;      
  u32 *aType;           
  u32 *aOffset;         
  u8 *aRow;             
};
typedef struct VdbeCursor VdbeCursor;

/*
** When a sub-program is executed (OP_Program), a structure of this type
** is allocated to store the current value of the program counter, as
** well as the current memory cell array and various other frame specific
** values stored in the Vdbe struct. When the sub-program is finished, 
** these values are copied back to the Vdbe from the VdbeFrame structure,
** restoring the state of the VM to as it was before the sub-program
** began executing.
**
** The memory for a VdbeFrame object is allocated and managed by a memory
** cell in the parent (calling) frame. When the memory cell is deleted or
** overwritten, the VdbeFrame object is not freed immediately. Instead, it
** is linked into the Vdbe.pDelFrame list. The contents of the Vdbe.pDelFrame
** list is deleted when the VM is reset in VdbeHalt(). The reason for doing
** this instead of deleting the VdbeFrame immediately is to avoid recursive
** calls to sqlite3VdbeMemRelease() when the memory cells belonging to the
** child frame are released.
**
** The currently executing frame is stored in Vdbe.pFrame. Vdbe.pFrame is
** set to NULL if the currently executing frame is the main program.
*/
typedef struct VdbeFrame VdbeFrame;
struct VdbeFrame {
  Vdbe *v;                
  int pc;                 
  Op *aOp;                
  int nOp;                
  Mem *aMem;              
  int nMem;               
  VdbeCursor **apCsr;     
  u16 nCursor;            
  void *token;            
  int nChildMem;          
  int nChildCsr;          
  i64 lastRowid;          
  int nChange;            
  VdbeFrame *pParent;     
};

#define VdbeFrameMem(p) ((Mem *)&((u8 *)p)[ROUND8(sizeof(VdbeFrame))])

#define CACHE_STALE 0

struct Mem {
  sqlite3 *db;        
  char *z;            
  double r;           
  union {
    i64 i;              
    int nZero;          
    FuncDef *pDef;      
    RowSet *pRowSet;    
    VdbeFrame *pFrame;  
  } u;
  int n;              
  u16 flags;          
  u8  type;           
  u8  enc;            
#ifdef SQLITE_DEBUG
  Mem *pScopyFrom;    
  void *pFiller;      
#endif
  void (*xDel)(void *);  
  char *zMalloc;      
};

#define MEM_Null      0x0001   
#define MEM_Str       0x0002   
#define MEM_Int       0x0004   
#define MEM_Real      0x0008   
#define MEM_Blob      0x0010   
#define MEM_RowSet    0x0020   
#define MEM_Frame     0x0040   
#define MEM_Invalid   0x0080   
#define MEM_TypeMask  0x00ff   

#define MEM_Term      0x0200   
#define MEM_Dyn       0x0400   
#define MEM_Static    0x0800   
#define MEM_Ephem     0x1000   
#define MEM_Agg       0x2000   
#define MEM_Zero      0x4000   
#ifdef SQLITE_OMIT_INCRBLOB
  #undef MEM_Zero
  #define MEM_Zero 0x0000
#endif

#define MemSetTypeFlag(p, f) \
   ((p)->flags = ((p)->flags&~(MEM_TypeMask|MEM_Zero))|f)

#ifdef SQLITE_DEBUG
#define memIsValid(M)  ((M)->flags & MEM_Invalid)==0
#endif


struct VdbeFunc {
  FuncDef *pFunc;               
  int nAux;                     
  struct AuxData {
    void *pAux;                   
    void (*xDelete)(void *);      
  } apAux[1];                   
};

struct sqlite3_context {
  FuncDef *pFunc;       
  VdbeFunc *pVdbeFunc;  
  Mem s;                
  Mem *pMem;            
  int isError;          
  CollSeq *pColl;       
};

struct Vdbe {
  sqlite3 *db;            
  Op *aOp;                
  Mem *aMem;              
  Mem **apArg;            
  Mem *aColName;          
  Mem *pResultSet;        
  int nMem;               
  int nOp;                
  int nOpAlloc;           
  int nLabel;             
  int nLabelAlloc;        
  int *aLabel;            
  u16 nResColumn;         
  u16 nCursor;            
  u32 magic;              
  char *zErrMsg;          /* Error message written here */
  Vdbe *pPrev,*pNext;     
  VdbeCursor **apCsr;     
  Mem *aVar;              
  char **azVar;           
  ynVar nVar;             
  u32 cacheCtr;           
  int pc;                 
  int rc;                 
  u8 errorAction;         
  u8 okVar;               
  u8 explain;             
  u8 changeCntOn;         
  u8 expired;             
  u8 runOnlyOnce;         
  u8 minWriteFileFormat;  
  u8 inVtabMethod;        
  u8 usesStmtJournal;     
  u8 readOnly;            
  u8 isPrepareV2;         
  int nChange;            
  yDbMask btreeMask;      
  yDbMask lockMask;       
  int iStatement;         
  int aCounter[3];        
#ifndef SQLITE_OMIT_TRACE
  i64 startTime;          
#endif
  i64 nFkConstraint;      
  i64 nStmtDefCons;       
  char *zSql;             
  void *pFree;            
#ifdef SQLITE_DEBUG
  FILE *trace;            
#endif
  VdbeFrame *pFrame;      
  VdbeFrame *pDelFrame;   
  int nFrame;             
  u32 expmask;            
  SubProgram *pProgram;   
};

#define VDBE_MAGIC_INIT     0x26bceaa5    
#define VDBE_MAGIC_RUN      0xbdf20da3    
#define VDBE_MAGIC_HALT     0x519c2973    
#define VDBE_MAGIC_DEAD     0xb606c3c8    

void sqlite3VdbeFreeCursor(Vdbe *, VdbeCursor*);
void sqliteVdbePopStack(Vdbe*,int);
int sqlite3VdbeCursorMoveto(VdbeCursor*);
#if defined(SQLITE_DEBUG) || defined(VDBE_PROFILE)
void sqlite3VdbePrintOp(FILE*, int, Op*);
#endif
u32 sqlite3VdbeSerialTypeLen(u32);
u32 sqlite3VdbeSerialType(Mem*, int);
u32 sqlite3VdbeSerialPut(unsigned char*, int, Mem*, int);
u32 sqlite3VdbeSerialGet(const unsigned char*, u32, Mem*);
void sqlite3VdbeDeleteAuxData(VdbeFunc*, int);

int sqlite2BtreeKeyCompare(BtCursor *, const void *, int, int, int *);
int sqlite3VdbeIdxKeyCompare(VdbeCursor*,UnpackedRecord*,int*);
int sqlite3VdbeIdxRowid(sqlite3*, BtCursor *, i64 *);
int sqlite3MemCompare(const Mem*, const Mem*, const CollSeq*);
int sqlite3VdbeExec(Vdbe*);
int sqlite3VdbeList(Vdbe*);
int sqlite3VdbeHalt(Vdbe*);
int sqlite3VdbeChangeEncoding(Mem *, int);
int sqlite3VdbeMemTooBig(Mem*);
int sqlite3VdbeMemCopy(Mem*, const Mem*);
void sqlite3VdbeMemShallowCopy(Mem*, const Mem*, int);
void sqlite3VdbeMemMove(Mem*, Mem*);
int sqlite3VdbeMemNulTerminate(Mem*);
int sqlite3VdbeMemSetStr(Mem*, const char*, int, u8, void(*)(void*));
void sqlite3VdbeMemSetInt64(Mem*, i64);
#ifdef SQLITE_OMIT_FLOATING_POINT
# define sqlite3VdbeMemSetDouble sqlite3VdbeMemSetInt64
#else
  void sqlite3VdbeMemSetDouble(Mem*, double);
#endif
void sqlite3VdbeMemSetNull(Mem*);
void sqlite3VdbeMemSetZeroBlob(Mem*,int);
void sqlite3VdbeMemSetRowSet(Mem*);
int sqlite3VdbeMemMakeWriteable(Mem*);
int sqlite3VdbeMemStringify(Mem*, int);
i64 sqlite3VdbeIntValue(Mem*);
int sqlite3VdbeMemIntegerify(Mem*);
double sqlite3VdbeRealValue(Mem*);
void sqlite3VdbeIntegerAffinity(Mem*);
int sqlite3VdbeMemRealify(Mem*);
int sqlite3VdbeMemNumerify(Mem*);
int sqlite3VdbeMemFromBtree(BtCursor*,int,int,int,Mem*);
void sqlite3VdbeMemRelease(Mem *p);
void sqlite3VdbeMemReleaseExternal(Mem *p);
int sqlite3VdbeMemFinalize(Mem*, FuncDef*);
const char *sqlite3OpcodeName(int);
int sqlite3VdbeMemGrow(Mem *pMem, int n, int preserve);
int sqlite3VdbeCloseStatement(Vdbe *, int);
void sqlite3VdbeFrameDelete(VdbeFrame*);
int sqlite3VdbeFrameRestore(VdbeFrame *);
void sqlite3VdbeMemStoreType(Mem *pMem);

#if !defined(SQLITE_OMIT_SHARED_CACHE) && SQLITE_THREADSAFE>0
  void sqlite3VdbeEnter(Vdbe*);
  void sqlite3VdbeLeave(Vdbe*);
#else
# define sqlite3VdbeEnter(X)
# define sqlite3VdbeLeave(X)
#endif

#ifdef SQLITE_DEBUG
void sqlite3VdbeMemPrepareToChange(Vdbe*,Mem*);
#endif

#ifndef SQLITE_OMIT_FOREIGN_KEY
int sqlite3VdbeCheckFk(Vdbe *, int);
#else
# define sqlite3VdbeCheckFk(p,i) 0
#endif

int sqlite3VdbeMemTranslate(Mem*, u8);
#ifdef SQLITE_DEBUG
  void sqlite3VdbePrintSql(Vdbe*);
  void sqlite3VdbeMemPrettyPrint(Mem *pMem, char *zBuf);
#endif
int sqlite3VdbeMemHandleBom(Mem *pMem);

#ifndef SQLITE_OMIT_INCRBLOB
  int sqlite3VdbeMemExpandBlob(Mem *);
#else
  #define sqlite3VdbeMemExpandBlob(x) SQLITE_OK
#endif

#endif 

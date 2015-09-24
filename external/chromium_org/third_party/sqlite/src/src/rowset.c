/*
** 2008 December 3
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
** This module implements an object we call a "RowSet".
**
** The RowSet object is a collection of rowids.  Rowids
** are inserted into the RowSet in an arbitrary order.  Inserts
** can be intermixed with tests to see if a given rowid has been
** previously inserted into the RowSet.
**
** After all inserts are finished, it is possible to extract the
** elements of the RowSet in sorted order.  Once this extraction
** process has started, no new elements may be inserted.
**
** Hence, the primitive operations for a RowSet are:
**
**    CREATE
**    INSERT
**    TEST
**    SMALLEST
**    DESTROY
**
** The CREATE and DESTROY primitives are the constructor and destructor,
** obviously.  The INSERT primitive adds a new element to the RowSet.
** TEST checks to see if an element is already in the RowSet.  SMALLEST
** extracts the least value from the RowSet.
**
** The INSERT primitive might allocate additional memory.  Memory is
** allocated in chunks so most INSERTs do no allocation.  There is an 
** upper bound on the size of allocated memory.  No memory is freed
** until DESTROY.
**
** The TEST primitive includes a "batch" number.  The TEST primitive
** will only see elements that were inserted before the last change
** in the batch number.  In other words, if an INSERT occurs between
** two TESTs where the TESTs have the same batch nubmer, then the
** value added by the INSERT will not be visible to the second TEST.
** The initial batch number is zero, so if the very first TEST contains
** a non-zero batch number, it will see all prior INSERTs.
**
** No INSERTs may occurs after a SMALLEST.  An assertion will fail if
** that is attempted.
**
** The cost of an INSERT is roughly constant.  (Sometime new memory
** has to be allocated on an INSERT.)  The cost of a TEST with a new
** batch number is O(NlogN) where N is the number of elements in the RowSet.
** The cost of a TEST using the same batch number is O(logN).  The cost
** of the first SMALLEST is O(NlogN).  Second and subsequent SMALLEST
** primitives are constant time.  The cost of DESTROY is O(N).
**
** There is an added cost of O(N) when switching between TEST and
** SMALLEST primitives.
*/
#include "sqliteInt.h"


#define ROWSET_ALLOCATION_SIZE 1024

#define ROWSET_ENTRY_PER_CHUNK  \
                       ((ROWSET_ALLOCATION_SIZE-8)/sizeof(struct RowSetEntry))

struct RowSetEntry {            
  i64 v;                        
  struct RowSetEntry *pRight;   
  struct RowSetEntry *pLeft;    
};

struct RowSetChunk {
  struct RowSetChunk *pNextChunk;        
  struct RowSetEntry aEntry[ROWSET_ENTRY_PER_CHUNK]; 
};

struct RowSet {
  struct RowSetChunk *pChunk;    
  sqlite3 *db;                   
  struct RowSetEntry *pEntry;    
  struct RowSetEntry *pLast;     
  struct RowSetEntry *pFresh;    
  struct RowSetEntry *pTree;     
  u16 nFresh;                    
  u8 isSorted;                   
  u8 iBatch;                     
};

RowSet *sqlite3RowSetInit(sqlite3 *db, void *pSpace, unsigned int N){
  RowSet *p;
  assert( N >= ROUND8(sizeof(*p)) );
  p = pSpace;
  p->pChunk = 0;
  p->db = db;
  p->pEntry = 0;
  p->pLast = 0;
  p->pTree = 0;
  p->pFresh = (struct RowSetEntry*)(ROUND8(sizeof(*p)) + (char*)p);
  p->nFresh = (u16)((N - ROUND8(sizeof(*p)))/sizeof(struct RowSetEntry));
  p->isSorted = 1;
  p->iBatch = 0;
  return p;
}

void sqlite3RowSetClear(RowSet *p){
  struct RowSetChunk *pChunk, *pNextChunk;
  for(pChunk=p->pChunk; pChunk; pChunk = pNextChunk){
    pNextChunk = pChunk->pNextChunk;
    sqlite3DbFree(p->db, pChunk);
  }
  p->pChunk = 0;
  p->nFresh = 0;
  p->pEntry = 0;
  p->pLast = 0;
  p->pTree = 0;
  p->isSorted = 1;
}

void sqlite3RowSetInsert(RowSet *p, i64 rowid){
  struct RowSetEntry *pEntry;  
  struct RowSetEntry *pLast;   
  assert( p!=0 );
  if( p->nFresh==0 ){
    struct RowSetChunk *pNew;
    pNew = sqlite3DbMallocRaw(p->db, sizeof(*pNew));
    if( pNew==0 ){
      return;
    }
    pNew->pNextChunk = p->pChunk;
    p->pChunk = pNew;
    p->pFresh = pNew->aEntry;
    p->nFresh = ROWSET_ENTRY_PER_CHUNK;
  }
  pEntry = p->pFresh++;
  p->nFresh--;
  pEntry->v = rowid;
  pEntry->pRight = 0;
  pLast = p->pLast;
  if( pLast ){
    if( p->isSorted && rowid<=pLast->v ){
      p->isSorted = 0;
    }
    pLast->pRight = pEntry;
  }else{
    assert( p->pEntry==0 ); 
    p->pEntry = pEntry;
  }
  p->pLast = pEntry;
}

static struct RowSetEntry *rowSetMerge(
  struct RowSetEntry *pA,    
  struct RowSetEntry *pB     
){
  struct RowSetEntry head;
  struct RowSetEntry *pTail;

  pTail = &head;
  while( pA && pB ){
    assert( pA->pRight==0 || pA->v<=pA->pRight->v );
    assert( pB->pRight==0 || pB->v<=pB->pRight->v );
    if( pA->v<pB->v ){
      pTail->pRight = pA;
      pA = pA->pRight;
      pTail = pTail->pRight;
    }else if( pB->v<pA->v ){
      pTail->pRight = pB;
      pB = pB->pRight;
      pTail = pTail->pRight;
    }else{
      pA = pA->pRight;
    }
  }
  if( pA ){
    assert( pA->pRight==0 || pA->v<=pA->pRight->v );
    pTail->pRight = pA;
  }else{
    assert( pB==0 || pB->pRight==0 || pB->v<=pB->pRight->v );
    pTail->pRight = pB;
  }
  return head.pRight;
}

 
static void rowSetSort(RowSet *p){
  unsigned int i;
  struct RowSetEntry *pEntry;
  struct RowSetEntry *aBucket[40];

  assert( p->isSorted==0 );
  memset(aBucket, 0, sizeof(aBucket));
  while( p->pEntry ){
    pEntry = p->pEntry;
    p->pEntry = pEntry->pRight;
    pEntry->pRight = 0;
    for(i=0; aBucket[i]; i++){
      pEntry = rowSetMerge(aBucket[i], pEntry);
      aBucket[i] = 0;
    }
    aBucket[i] = pEntry;
  }
  pEntry = 0;
  for(i=0; i<sizeof(aBucket)/sizeof(aBucket[0]); i++){
    pEntry = rowSetMerge(pEntry, aBucket[i]);
  }
  p->pEntry = pEntry;
  p->pLast = 0;
  p->isSorted = 1;
}


static void rowSetTreeToList(
  struct RowSetEntry *pIn,         
  struct RowSetEntry **ppFirst,    
  struct RowSetEntry **ppLast      
){
  assert( pIn!=0 );
  if( pIn->pLeft ){
    struct RowSetEntry *p;
    rowSetTreeToList(pIn->pLeft, ppFirst, &p);
    p->pRight = pIn;
  }else{
    *ppFirst = pIn;
  }
  if( pIn->pRight ){
    rowSetTreeToList(pIn->pRight, &pIn->pRight, ppLast);
  }else{
    *ppLast = pIn;
  }
  assert( (*ppLast)->pRight==0 );
}


static struct RowSetEntry *rowSetNDeepTree(
  struct RowSetEntry **ppList,
  int iDepth
){
  struct RowSetEntry *p;         
  struct RowSetEntry *pLeft;     
  if( *ppList==0 ){
    return 0;
  }
  if( iDepth==1 ){
    p = *ppList;
    *ppList = p->pRight;
    p->pLeft = p->pRight = 0;
    return p;
  }
  pLeft = rowSetNDeepTree(ppList, iDepth-1);
  p = *ppList;
  if( p==0 ){
    return pLeft;
  }
  p->pLeft = pLeft;
  *ppList = p->pRight;
  p->pRight = rowSetNDeepTree(ppList, iDepth-1);
  return p;
}

static struct RowSetEntry *rowSetListToTree(struct RowSetEntry *pList){
  int iDepth;           
  struct RowSetEntry *p;       
  struct RowSetEntry *pLeft;   

  assert( pList!=0 );
  p = pList;
  pList = p->pRight;
  p->pLeft = p->pRight = 0;
  for(iDepth=1; pList; iDepth++){
    pLeft = p;
    p = pList;
    pList = p->pRight;
    p->pLeft = pLeft;
    p->pRight = rowSetNDeepTree(&pList, iDepth);
  }
  return p;
}

static void rowSetToList(RowSet *p){
  if( !p->isSorted ){
    rowSetSort(p);
  }
  if( p->pTree ){
    struct RowSetEntry *pHead, *pTail;
    rowSetTreeToList(p->pTree, &pHead, &pTail);
    p->pTree = 0;
    p->pEntry = rowSetMerge(p->pEntry, pHead);
  }
}

int sqlite3RowSetNext(RowSet *p, i64 *pRowid){
  rowSetToList(p);
  if( p->pEntry ){
    *pRowid = p->pEntry->v;
    p->pEntry = p->pEntry->pRight;
    if( p->pEntry==0 ){
      sqlite3RowSetClear(p);
    }
    return 1;
  }else{
    return 0;
  }
}

int sqlite3RowSetTest(RowSet *pRowSet, u8 iBatch, sqlite3_int64 iRowid){
  struct RowSetEntry *p;
  if( iBatch!=pRowSet->iBatch ){
    if( pRowSet->pEntry ){
      rowSetToList(pRowSet);
      pRowSet->pTree = rowSetListToTree(pRowSet->pEntry);
      pRowSet->pEntry = 0;
      pRowSet->pLast = 0;
    }
    pRowSet->iBatch = iBatch;
  }
  p = pRowSet->pTree;
  while( p ){
    if( p->v<iRowid ){
      p = p->pRight;
    }else if( p->v>iRowid ){
      p = p->pLeft;
    }else{
      return 1;
    }
  }
  return 0;
}

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
** This file contains code for implementations of the r-tree and r*-tree
** algorithms packaged as an SQLite virtual table module.
*/


#if !defined(SQLITE_CORE) || defined(SQLITE_ENABLE_RTREE)


#define VARIANT_RSTARTREE_CHOOSESUBTREE 0
#define VARIANT_RSTARTREE_REINSERT      1

#define VARIANT_GUTTMAN_QUADRATIC_SPLIT 0
#define VARIANT_GUTTMAN_LINEAR_SPLIT    0
#define VARIANT_RSTARTREE_SPLIT         1

#define VARIANT_GUTTMAN_SPLIT \
        (VARIANT_GUTTMAN_LINEAR_SPLIT||VARIANT_GUTTMAN_QUADRATIC_SPLIT)

#if VARIANT_GUTTMAN_QUADRATIC_SPLIT
  #define PickNext QuadraticPickNext
  #define PickSeeds QuadraticPickSeeds
  #define AssignCells splitNodeGuttman
#endif
#if VARIANT_GUTTMAN_LINEAR_SPLIT
  #define PickNext LinearPickNext
  #define PickSeeds LinearPickSeeds
  #define AssignCells splitNodeGuttman
#endif
#if VARIANT_RSTARTREE_SPLIT
  #define AssignCells splitNodeStartree
#endif

#if !defined(NDEBUG) && !defined(SQLITE_DEBUG) 
# define NDEBUG 1
#endif

#ifndef SQLITE_CORE
  #include "sqlite3ext.h"
  SQLITE_EXTENSION_INIT1
#else
  #include "sqlite3.h"
#endif

#include <string.h>
#include <assert.h>

#ifndef SQLITE_AMALGAMATION
#include "sqlite3rtree.h"
typedef sqlite3_int64 i64;
typedef unsigned char u8;
typedef unsigned int u32;
#endif

#ifndef UNUSED_PARAMETER
# define UNUSED_PARAMETER(x) (void)(x)
#endif

typedef struct Rtree Rtree;
typedef struct RtreeCursor RtreeCursor;
typedef struct RtreeNode RtreeNode;
typedef struct RtreeCell RtreeCell;
typedef struct RtreeConstraint RtreeConstraint;
typedef struct RtreeMatchArg RtreeMatchArg;
typedef struct RtreeGeomCallback RtreeGeomCallback;
typedef union RtreeCoord RtreeCoord;

#define RTREE_MAX_DIMENSIONS 5

#define HASHSIZE 128

struct Rtree {
  sqlite3_vtab base;
  sqlite3 *db;                
  int iNodeSize;              
  int nDim;                   
  int nBytesPerCell;          
  int iDepth;                 
  char *zDb;                  
  char *zName;                 
  RtreeNode *aHash[HASHSIZE];  
  int nBusy;                  

  RtreeNode *pDeleted;
  int iReinsertHeight;        

  
  sqlite3_stmt *pReadNode;
  sqlite3_stmt *pWriteNode;
  sqlite3_stmt *pDeleteNode;

  
  sqlite3_stmt *pReadRowid;
  sqlite3_stmt *pWriteRowid;
  sqlite3_stmt *pDeleteRowid;

  
  sqlite3_stmt *pReadParent;
  sqlite3_stmt *pWriteParent;
  sqlite3_stmt *pDeleteParent;

  int eCoordType;
};

#define RTREE_COORD_REAL32 0
#define RTREE_COORD_INT32  1

#define RTREE_MINCELLS(p) ((((p)->iNodeSize-4)/(p)->nBytesPerCell)/3)
#define RTREE_REINSERT(p) RTREE_MINCELLS(p)
#define RTREE_MAXCELLS 51

#define RTREE_MAX_DEPTH 40

struct RtreeCursor {
  sqlite3_vtab_cursor base;
  RtreeNode *pNode;                 
  int iCell;                        
  int iStrategy;                    
  int nConstraint;                  
  RtreeConstraint *aConstraint;     
};

union RtreeCoord {
  float f;
  int i;
};

#define DCOORD(coord) (                           \
  (pRtree->eCoordType==RTREE_COORD_REAL32) ?      \
    ((double)coord.f) :                           \
    ((double)coord.i)                             \
)

struct RtreeConstraint {
  int iCoord;                     
  int op;                         
  double rValue;                  
  int (*xGeom)(sqlite3_rtree_geometry *, int, double *, int *);
  sqlite3_rtree_geometry *pGeom;  
};

#define RTREE_EQ    0x41
#define RTREE_LE    0x42
#define RTREE_LT    0x43
#define RTREE_GE    0x44
#define RTREE_GT    0x45
#define RTREE_MATCH 0x46

struct RtreeNode {
  RtreeNode *pParent;               
  i64 iNode;
  int nRef;
  int isDirty;
  u8 *zData;
  RtreeNode *pNext;                 
};
#define NCELL(pNode) readInt16(&(pNode)->zData[2])

struct RtreeCell {
  i64 iRowid;
  RtreeCoord aCoord[RTREE_MAX_DIMENSIONS*2];
};


#define RTREE_GEOMETRY_MAGIC 0x891245AB

struct RtreeMatchArg {
  u32 magic;                      
  int (*xGeom)(sqlite3_rtree_geometry *, int, double *, int *);
  void *pContext;
  int nParam;
  double aParam[1];
};

struct RtreeGeomCallback {
  int (*xGeom)(sqlite3_rtree_geometry *, int, double *, int *);
  void *pContext;
};

#ifndef MAX
# define MAX(x,y) ((x) < (y) ? (y) : (x))
#endif
#ifndef MIN
# define MIN(x,y) ((x) > (y) ? (y) : (x))
#endif

static int readInt16(u8 *p){
  return (p[0]<<8) + p[1];
}
static void readCoord(u8 *p, RtreeCoord *pCoord){
  u32 i = (
    (((u32)p[0]) << 24) + 
    (((u32)p[1]) << 16) + 
    (((u32)p[2]) <<  8) + 
    (((u32)p[3]) <<  0)
  );
  *(u32 *)pCoord = i;
}
static i64 readInt64(u8 *p){
  return (
    (((i64)p[0]) << 56) + 
    (((i64)p[1]) << 48) + 
    (((i64)p[2]) << 40) + 
    (((i64)p[3]) << 32) + 
    (((i64)p[4]) << 24) + 
    (((i64)p[5]) << 16) + 
    (((i64)p[6]) <<  8) + 
    (((i64)p[7]) <<  0)
  );
}

/*
** Functions to serialize a 16 bit integer, 32 bit real number and
** 64 bit integer. The value returned is the number of bytes written
** to the argument buffer (always 2, 4 and 8 respectively).
*/
static int writeInt16(u8 *p, int i){
  p[0] = (i>> 8)&0xFF;
  p[1] = (i>> 0)&0xFF;
  return 2;
}
static int writeCoord(u8 *p, RtreeCoord *pCoord){
  u32 i;
  assert( sizeof(RtreeCoord)==4 );
  assert( sizeof(u32)==4 );
  i = *(u32 *)pCoord;
  p[0] = (i>>24)&0xFF;
  p[1] = (i>>16)&0xFF;
  p[2] = (i>> 8)&0xFF;
  p[3] = (i>> 0)&0xFF;
  return 4;
}
static int writeInt64(u8 *p, i64 i){
  p[0] = (i>>56)&0xFF;
  p[1] = (i>>48)&0xFF;
  p[2] = (i>>40)&0xFF;
  p[3] = (i>>32)&0xFF;
  p[4] = (i>>24)&0xFF;
  p[5] = (i>>16)&0xFF;
  p[6] = (i>> 8)&0xFF;
  p[7] = (i>> 0)&0xFF;
  return 8;
}

static void nodeReference(RtreeNode *p){
  if( p ){
    p->nRef++;
  }
}

static void nodeZero(Rtree *pRtree, RtreeNode *p){
  memset(&p->zData[2], 0, pRtree->iNodeSize-2);
  p->isDirty = 1;
}

static int nodeHash(i64 iNode){
  return (
    (iNode>>56) ^ (iNode>>48) ^ (iNode>>40) ^ (iNode>>32) ^ 
    (iNode>>24) ^ (iNode>>16) ^ (iNode>> 8) ^ (iNode>> 0)
  ) % HASHSIZE;
}

static RtreeNode *nodeHashLookup(Rtree *pRtree, i64 iNode){
  RtreeNode *p;
  for(p=pRtree->aHash[nodeHash(iNode)]; p && p->iNode!=iNode; p=p->pNext);
  return p;
}

static void nodeHashInsert(Rtree *pRtree, RtreeNode *pNode){
  int iHash;
  assert( pNode->pNext==0 );
  iHash = nodeHash(pNode->iNode);
  pNode->pNext = pRtree->aHash[iHash];
  pRtree->aHash[iHash] = pNode;
}

static void nodeHashDelete(Rtree *pRtree, RtreeNode *pNode){
  RtreeNode **pp;
  if( pNode->iNode!=0 ){
    pp = &pRtree->aHash[nodeHash(pNode->iNode)];
    for( ; (*pp)!=pNode; pp = &(*pp)->pNext){ assert(*pp); }
    *pp = pNode->pNext;
    pNode->pNext = 0;
  }
}

static RtreeNode *nodeNew(Rtree *pRtree, RtreeNode *pParent){
  RtreeNode *pNode;
  pNode = (RtreeNode *)sqlite3_malloc(sizeof(RtreeNode) + pRtree->iNodeSize);
  if( pNode ){
    memset(pNode, 0, sizeof(RtreeNode) + pRtree->iNodeSize);
    pNode->zData = (u8 *)&pNode[1];
    pNode->nRef = 1;
    pNode->pParent = pParent;
    pNode->isDirty = 1;
    nodeReference(pParent);
  }
  return pNode;
}

static int
nodeAcquire(
  Rtree *pRtree,             
  i64 iNode,                 
  RtreeNode *pParent,        
  RtreeNode **ppNode         
){
  int rc;
  int rc2 = SQLITE_OK;
  RtreeNode *pNode;

  if( (pNode = nodeHashLookup(pRtree, iNode)) ){
    assert( !pParent || !pNode->pParent || pNode->pParent==pParent );
    if( pParent && !pNode->pParent ){
      nodeReference(pParent);
      pNode->pParent = pParent;
    }
    pNode->nRef++;
    *ppNode = pNode;
    return SQLITE_OK;
  }

  sqlite3_bind_int64(pRtree->pReadNode, 1, iNode);
  rc = sqlite3_step(pRtree->pReadNode);
  if( rc==SQLITE_ROW ){
    const u8 *zBlob = sqlite3_column_blob(pRtree->pReadNode, 0);
    if( pRtree->iNodeSize==sqlite3_column_bytes(pRtree->pReadNode, 0) ){
      pNode = (RtreeNode *)sqlite3_malloc(sizeof(RtreeNode)+pRtree->iNodeSize);
      if( !pNode ){
        rc2 = SQLITE_NOMEM;
      }else{
        pNode->pParent = pParent;
        pNode->zData = (u8 *)&pNode[1];
        pNode->nRef = 1;
        pNode->iNode = iNode;
        pNode->isDirty = 0;
        pNode->pNext = 0;
        memcpy(pNode->zData, zBlob, pRtree->iNodeSize);
        nodeReference(pParent);
      }
    }
  }
  rc = sqlite3_reset(pRtree->pReadNode);
  if( rc==SQLITE_OK ) rc = rc2;

  if( pNode && iNode==1 ){
    pRtree->iDepth = readInt16(pNode->zData);
    if( pRtree->iDepth>RTREE_MAX_DEPTH ){
      rc = SQLITE_CORRUPT;
    }
  }

  if( pNode && rc==SQLITE_OK ){
    if( NCELL(pNode)>((pRtree->iNodeSize-4)/pRtree->nBytesPerCell) ){
      rc = SQLITE_CORRUPT;
    }
  }

  if( rc==SQLITE_OK ){
    if( pNode!=0 ){
      nodeHashInsert(pRtree, pNode);
    }else{
      rc = SQLITE_CORRUPT;
    }
    *ppNode = pNode;
  }else{
    sqlite3_free(pNode);
    *ppNode = 0;
  }

  return rc;
}

static void nodeOverwriteCell(
  Rtree *pRtree, 
  RtreeNode *pNode,  
  RtreeCell *pCell, 
  int iCell
){
  int ii;
  u8 *p = &pNode->zData[4 + pRtree->nBytesPerCell*iCell];
  p += writeInt64(p, pCell->iRowid);
  for(ii=0; ii<(pRtree->nDim*2); ii++){
    p += writeCoord(p, &pCell->aCoord[ii]);
  }
  pNode->isDirty = 1;
}

static void nodeDeleteCell(Rtree *pRtree, RtreeNode *pNode, int iCell){
  u8 *pDst = &pNode->zData[4 + pRtree->nBytesPerCell*iCell];
  u8 *pSrc = &pDst[pRtree->nBytesPerCell];
  int nByte = (NCELL(pNode) - iCell - 1) * pRtree->nBytesPerCell;
  memmove(pDst, pSrc, nByte);
  writeInt16(&pNode->zData[2], NCELL(pNode)-1);
  pNode->isDirty = 1;
}

static int
nodeInsertCell(
  Rtree *pRtree, 
  RtreeNode *pNode, 
  RtreeCell *pCell 
){
  int nCell;                    
  int nMaxCell;                 

  nMaxCell = (pRtree->iNodeSize-4)/pRtree->nBytesPerCell;
  nCell = NCELL(pNode);

  assert( nCell<=nMaxCell );
  if( nCell<nMaxCell ){
    nodeOverwriteCell(pRtree, pNode, pCell, nCell);
    writeInt16(&pNode->zData[2], nCell+1);
    pNode->isDirty = 1;
  }

  return (nCell==nMaxCell);
}

static int
nodeWrite(Rtree *pRtree, RtreeNode *pNode){
  int rc = SQLITE_OK;
  if( pNode->isDirty ){
    sqlite3_stmt *p = pRtree->pWriteNode;
    if( pNode->iNode ){
      sqlite3_bind_int64(p, 1, pNode->iNode);
    }else{
      sqlite3_bind_null(p, 1);
    }
    sqlite3_bind_blob(p, 2, pNode->zData, pRtree->iNodeSize, SQLITE_STATIC);
    sqlite3_step(p);
    pNode->isDirty = 0;
    rc = sqlite3_reset(p);
    if( pNode->iNode==0 && rc==SQLITE_OK ){
      pNode->iNode = sqlite3_last_insert_rowid(pRtree->db);
      nodeHashInsert(pRtree, pNode);
    }
  }
  return rc;
}

/*
** Release a reference to a node. If the node is dirty and the reference
** count drops to zero, the node data is written to the database.
*/
static int
nodeRelease(Rtree *pRtree, RtreeNode *pNode){
  int rc = SQLITE_OK;
  if( pNode ){
    assert( pNode->nRef>0 );
    pNode->nRef--;
    if( pNode->nRef==0 ){
      if( pNode->iNode==1 ){
        pRtree->iDepth = -1;
      }
      if( pNode->pParent ){
        rc = nodeRelease(pRtree, pNode->pParent);
      }
      if( rc==SQLITE_OK ){
        rc = nodeWrite(pRtree, pNode);
      }
      nodeHashDelete(pRtree, pNode);
      sqlite3_free(pNode);
    }
  }
  return rc;
}

static i64 nodeGetRowid(
  Rtree *pRtree, 
  RtreeNode *pNode, 
  int iCell
){
  assert( iCell<NCELL(pNode) );
  return readInt64(&pNode->zData[4 + pRtree->nBytesPerCell*iCell]);
}

static void nodeGetCoord(
  Rtree *pRtree, 
  RtreeNode *pNode, 
  int iCell,
  int iCoord,
  RtreeCoord *pCoord           
){
  readCoord(&pNode->zData[12 + pRtree->nBytesPerCell*iCell + 4*iCoord], pCoord);
}

static void nodeGetCell(
  Rtree *pRtree, 
  RtreeNode *pNode, 
  int iCell,
  RtreeCell *pCell
){
  int ii;
  pCell->iRowid = nodeGetRowid(pRtree, pNode, iCell);
  for(ii=0; ii<pRtree->nDim*2; ii++){
    nodeGetCoord(pRtree, pNode, iCell, ii, &pCell->aCoord[ii]);
  }
}


static int rtreeInit(
  sqlite3 *, void *, int, const char *const*, sqlite3_vtab **, char **, int
);

static int rtreeCreate(
  sqlite3 *db,
  void *pAux,
  int argc, const char *const*argv,
  sqlite3_vtab **ppVtab,
  char **pzErr
){
  return rtreeInit(db, pAux, argc, argv, ppVtab, pzErr, 1);
}

static int rtreeConnect(
  sqlite3 *db,
  void *pAux,
  int argc, const char *const*argv,
  sqlite3_vtab **ppVtab,
  char **pzErr
){
  return rtreeInit(db, pAux, argc, argv, ppVtab, pzErr, 0);
}

static void rtreeReference(Rtree *pRtree){
  pRtree->nBusy++;
}

static void rtreeRelease(Rtree *pRtree){
  pRtree->nBusy--;
  if( pRtree->nBusy==0 ){
    sqlite3_finalize(pRtree->pReadNode);
    sqlite3_finalize(pRtree->pWriteNode);
    sqlite3_finalize(pRtree->pDeleteNode);
    sqlite3_finalize(pRtree->pReadRowid);
    sqlite3_finalize(pRtree->pWriteRowid);
    sqlite3_finalize(pRtree->pDeleteRowid);
    sqlite3_finalize(pRtree->pReadParent);
    sqlite3_finalize(pRtree->pWriteParent);
    sqlite3_finalize(pRtree->pDeleteParent);
    sqlite3_free(pRtree);
  }
}

static int rtreeDisconnect(sqlite3_vtab *pVtab){
  rtreeRelease((Rtree *)pVtab);
  return SQLITE_OK;
}

static int rtreeDestroy(sqlite3_vtab *pVtab){
  Rtree *pRtree = (Rtree *)pVtab;
  int rc;
  char *zCreate = sqlite3_mprintf(
    "DROP TABLE '%q'.'%q_node';"
    "DROP TABLE '%q'.'%q_rowid';"
    "DROP TABLE '%q'.'%q_parent';",
    pRtree->zDb, pRtree->zName, 
    pRtree->zDb, pRtree->zName,
    pRtree->zDb, pRtree->zName
  );
  if( !zCreate ){
    rc = SQLITE_NOMEM;
  }else{
    rc = sqlite3_exec(pRtree->db, zCreate, 0, 0, 0);
    sqlite3_free(zCreate);
  }
  if( rc==SQLITE_OK ){
    rtreeRelease(pRtree);
  }

  return rc;
}

static int rtreeOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor){
  int rc = SQLITE_NOMEM;
  RtreeCursor *pCsr;

  pCsr = (RtreeCursor *)sqlite3_malloc(sizeof(RtreeCursor));
  if( pCsr ){
    memset(pCsr, 0, sizeof(RtreeCursor));
    pCsr->base.pVtab = pVTab;
    rc = SQLITE_OK;
  }
  *ppCursor = (sqlite3_vtab_cursor *)pCsr;

  return rc;
}


static void freeCursorConstraints(RtreeCursor *pCsr){
  if( pCsr->aConstraint ){
    int i;                        
    for(i=0; i<pCsr->nConstraint; i++){
      sqlite3_rtree_geometry *pGeom = pCsr->aConstraint[i].pGeom;
      if( pGeom ){
        if( pGeom->xDelUser ) pGeom->xDelUser(pGeom->pUser);
        sqlite3_free(pGeom);
      }
    }
    sqlite3_free(pCsr->aConstraint);
    pCsr->aConstraint = 0;
  }
}

static int rtreeClose(sqlite3_vtab_cursor *cur){
  Rtree *pRtree = (Rtree *)(cur->pVtab);
  int rc;
  RtreeCursor *pCsr = (RtreeCursor *)cur;
  freeCursorConstraints(pCsr);
  rc = nodeRelease(pRtree, pCsr->pNode);
  sqlite3_free(pCsr);
  return rc;
}

static int rtreeEof(sqlite3_vtab_cursor *cur){
  RtreeCursor *pCsr = (RtreeCursor *)cur;
  return (pCsr->pNode==0);
}

static int testRtreeGeom(
  Rtree *pRtree,                  
  RtreeConstraint *pConstraint,   
  RtreeCell *pCell,               
  int *pbRes                      
){
  int i;
  double aCoord[RTREE_MAX_DIMENSIONS*2];
  int nCoord = pRtree->nDim*2;

  assert( pConstraint->op==RTREE_MATCH );
  assert( pConstraint->pGeom );

  for(i=0; i<nCoord; i++){
    aCoord[i] = DCOORD(pCell->aCoord[i]);
  }
  return pConstraint->xGeom(pConstraint->pGeom, nCoord, aCoord, pbRes);
}

static int testRtreeCell(Rtree *pRtree, RtreeCursor *pCursor, int *pbEof){
  RtreeCell cell;
  int ii;
  int bRes = 0;
  int rc = SQLITE_OK;

  nodeGetCell(pRtree, pCursor->pNode, pCursor->iCell, &cell);
  for(ii=0; bRes==0 && ii<pCursor->nConstraint; ii++){
    RtreeConstraint *p = &pCursor->aConstraint[ii];
    double cell_min = DCOORD(cell.aCoord[(p->iCoord>>1)*2]);
    double cell_max = DCOORD(cell.aCoord[(p->iCoord>>1)*2+1]);

    assert(p->op==RTREE_LE || p->op==RTREE_LT || p->op==RTREE_GE 
        || p->op==RTREE_GT || p->op==RTREE_EQ || p->op==RTREE_MATCH
    );

    switch( p->op ){
      case RTREE_LE: case RTREE_LT: 
        bRes = p->rValue<cell_min; 
        break;

      case RTREE_GE: case RTREE_GT: 
        bRes = p->rValue>cell_max; 
        break;

      case RTREE_EQ:
        bRes = (p->rValue>cell_max || p->rValue<cell_min);
        break;

      default: {
        assert( p->op==RTREE_MATCH );
        rc = testRtreeGeom(pRtree, p, &cell, &bRes);
        bRes = !bRes;
        break;
      }
    }
  }

  *pbEof = bRes;
  return rc;
}

static int testRtreeEntry(Rtree *pRtree, RtreeCursor *pCursor, int *pbEof){
  RtreeCell cell;
  int ii;
  *pbEof = 0;

  nodeGetCell(pRtree, pCursor->pNode, pCursor->iCell, &cell);
  for(ii=0; ii<pCursor->nConstraint; ii++){
    RtreeConstraint *p = &pCursor->aConstraint[ii];
    double coord = DCOORD(cell.aCoord[p->iCoord]);
    int res;
    assert(p->op==RTREE_LE || p->op==RTREE_LT || p->op==RTREE_GE 
        || p->op==RTREE_GT || p->op==RTREE_EQ || p->op==RTREE_MATCH
    );
    switch( p->op ){
      case RTREE_LE: res = (coord<=p->rValue); break;
      case RTREE_LT: res = (coord<p->rValue);  break;
      case RTREE_GE: res = (coord>=p->rValue); break;
      case RTREE_GT: res = (coord>p->rValue);  break;
      case RTREE_EQ: res = (coord==p->rValue); break;
      default: {
        int rc;
        assert( p->op==RTREE_MATCH );
        rc = testRtreeGeom(pRtree, p, &cell, &res);
        if( rc!=SQLITE_OK ){
          return rc;
        }
        break;
      }
    }

    if( !res ){
      *pbEof = 1;
      return SQLITE_OK;
    }
  }

  return SQLITE_OK;
}

static int descendToCell(
  Rtree *pRtree, 
  RtreeCursor *pCursor, 
  int iHeight,
  int *pEof                 
){
  int isEof;
  int rc;
  int ii;
  RtreeNode *pChild;
  sqlite3_int64 iRowid;

  RtreeNode *pSavedNode = pCursor->pNode;
  int iSavedCell = pCursor->iCell;

  assert( iHeight>=0 );

  if( iHeight==0 ){
    rc = testRtreeEntry(pRtree, pCursor, &isEof);
  }else{
    rc = testRtreeCell(pRtree, pCursor, &isEof);
  }
  if( rc!=SQLITE_OK || isEof || iHeight==0 ){
    goto descend_to_cell_out;
  }

  iRowid = nodeGetRowid(pRtree, pCursor->pNode, pCursor->iCell);
  rc = nodeAcquire(pRtree, iRowid, pCursor->pNode, &pChild);
  if( rc!=SQLITE_OK ){
    goto descend_to_cell_out;
  }

  nodeRelease(pRtree, pCursor->pNode);
  pCursor->pNode = pChild;
  isEof = 1;
  for(ii=0; isEof && ii<NCELL(pChild); ii++){
    pCursor->iCell = ii;
    rc = descendToCell(pRtree, pCursor, iHeight-1, &isEof);
    if( rc!=SQLITE_OK ){
      goto descend_to_cell_out;
    }
  }

  if( isEof ){
    assert( pCursor->pNode==pChild );
    nodeReference(pSavedNode);
    nodeRelease(pRtree, pChild);
    pCursor->pNode = pSavedNode;
    pCursor->iCell = iSavedCell;
  }

descend_to_cell_out:
  *pEof = isEof;
  return rc;
}

static int nodeRowidIndex(
  Rtree *pRtree, 
  RtreeNode *pNode, 
  i64 iRowid,
  int *piIndex
){
  int ii;
  int nCell = NCELL(pNode);
  for(ii=0; ii<nCell; ii++){
    if( nodeGetRowid(pRtree, pNode, ii)==iRowid ){
      *piIndex = ii;
      return SQLITE_OK;
    }
  }
  return SQLITE_CORRUPT;
}

static int nodeParentIndex(Rtree *pRtree, RtreeNode *pNode, int *piIndex){
  RtreeNode *pParent = pNode->pParent;
  if( pParent ){
    return nodeRowidIndex(pRtree, pParent, pNode->iNode, piIndex);
  }
  *piIndex = -1;
  return SQLITE_OK;
}

static int rtreeNext(sqlite3_vtab_cursor *pVtabCursor){
  Rtree *pRtree = (Rtree *)(pVtabCursor->pVtab);
  RtreeCursor *pCsr = (RtreeCursor *)pVtabCursor;
  int rc = SQLITE_OK;

  assert( pCsr->pNode );

  if( pCsr->iStrategy==1 ){
    
    nodeRelease(pRtree, pCsr->pNode);
    pCsr->pNode = 0;
  }else{
    
    int iHeight = 0;
    while( pCsr->pNode ){
      RtreeNode *pNode = pCsr->pNode;
      int nCell = NCELL(pNode);
      for(pCsr->iCell++; pCsr->iCell<nCell; pCsr->iCell++){
        int isEof;
        rc = descendToCell(pRtree, pCsr, iHeight, &isEof);
        if( rc!=SQLITE_OK || !isEof ){
          return rc;
        }
      }
      pCsr->pNode = pNode->pParent;
      rc = nodeParentIndex(pRtree, pNode, &pCsr->iCell);
      if( rc!=SQLITE_OK ){
        return rc;
      }
      nodeReference(pCsr->pNode);
      nodeRelease(pRtree, pNode);
      iHeight++;
    }
  }

  return rc;
}

static int rtreeRowid(sqlite3_vtab_cursor *pVtabCursor, sqlite_int64 *pRowid){
  Rtree *pRtree = (Rtree *)pVtabCursor->pVtab;
  RtreeCursor *pCsr = (RtreeCursor *)pVtabCursor;

  assert(pCsr->pNode);
  *pRowid = nodeGetRowid(pRtree, pCsr->pNode, pCsr->iCell);

  return SQLITE_OK;
}

static int rtreeColumn(sqlite3_vtab_cursor *cur, sqlite3_context *ctx, int i){
  Rtree *pRtree = (Rtree *)cur->pVtab;
  RtreeCursor *pCsr = (RtreeCursor *)cur;

  if( i==0 ){
    i64 iRowid = nodeGetRowid(pRtree, pCsr->pNode, pCsr->iCell);
    sqlite3_result_int64(ctx, iRowid);
  }else{
    RtreeCoord c;
    nodeGetCoord(pRtree, pCsr->pNode, pCsr->iCell, i-1, &c);
    if( pRtree->eCoordType==RTREE_COORD_REAL32 ){
      sqlite3_result_double(ctx, c.f);
    }else{
      assert( pRtree->eCoordType==RTREE_COORD_INT32 );
      sqlite3_result_int(ctx, c.i);
    }
  }

  return SQLITE_OK;
}

static int findLeafNode(Rtree *pRtree, i64 iRowid, RtreeNode **ppLeaf){
  int rc;
  *ppLeaf = 0;
  sqlite3_bind_int64(pRtree->pReadRowid, 1, iRowid);
  if( sqlite3_step(pRtree->pReadRowid)==SQLITE_ROW ){
    i64 iNode = sqlite3_column_int64(pRtree->pReadRowid, 0);
    rc = nodeAcquire(pRtree, iNode, 0, ppLeaf);
    sqlite3_reset(pRtree->pReadRowid);
  }else{
    rc = sqlite3_reset(pRtree->pReadRowid);
  }
  return rc;
}

static int deserializeGeometry(sqlite3_value *pValue, RtreeConstraint *pCons){
  RtreeMatchArg *p;
  sqlite3_rtree_geometry *pGeom;
  int nBlob;

  
  if( !sqlite3_value_type(pValue)==SQLITE_BLOB ) return SQLITE_ERROR;

  
  nBlob = sqlite3_value_bytes(pValue);
  if( nBlob<(int)sizeof(RtreeMatchArg) 
   || ((nBlob-sizeof(RtreeMatchArg))%sizeof(double))!=0
  ){
    return SQLITE_ERROR;
  }

  pGeom = (sqlite3_rtree_geometry *)sqlite3_malloc(
      sizeof(sqlite3_rtree_geometry) + nBlob
  );
  if( !pGeom ) return SQLITE_NOMEM;
  memset(pGeom, 0, sizeof(sqlite3_rtree_geometry));
  p = (RtreeMatchArg *)&pGeom[1];

  memcpy(p, sqlite3_value_blob(pValue), nBlob);
  if( p->magic!=RTREE_GEOMETRY_MAGIC 
   || nBlob!=(int)(sizeof(RtreeMatchArg) + (p->nParam-1)*sizeof(double))
  ){
    sqlite3_free(pGeom);
    return SQLITE_ERROR;
  }

  pGeom->pContext = p->pContext;
  pGeom->nParam = p->nParam;
  pGeom->aParam = p->aParam;

  pCons->xGeom = p->xGeom;
  pCons->pGeom = pGeom;
  return SQLITE_OK;
}

static int rtreeFilter(
  sqlite3_vtab_cursor *pVtabCursor, 
  int idxNum, const char *idxStr,
  int argc, sqlite3_value **argv
){
  Rtree *pRtree = (Rtree *)pVtabCursor->pVtab;
  RtreeCursor *pCsr = (RtreeCursor *)pVtabCursor;

  RtreeNode *pRoot = 0;
  int ii;
  int rc = SQLITE_OK;

  rtreeReference(pRtree);

  freeCursorConstraints(pCsr);
  pCsr->iStrategy = idxNum;

  if( idxNum==1 ){
    
    RtreeNode *pLeaf;        
    i64 iRowid = sqlite3_value_int64(argv[0]);
    rc = findLeafNode(pRtree, iRowid, &pLeaf);
    pCsr->pNode = pLeaf; 
    if( pLeaf ){
      assert( rc==SQLITE_OK );
      rc = nodeRowidIndex(pRtree, pLeaf, iRowid, &pCsr->iCell);
    }
  }else{
    if( argc>0 ){
      pCsr->aConstraint = sqlite3_malloc(sizeof(RtreeConstraint)*argc);
      pCsr->nConstraint = argc;
      if( !pCsr->aConstraint ){
        rc = SQLITE_NOMEM;
      }else{
        memset(pCsr->aConstraint, 0, sizeof(RtreeConstraint)*argc);
        assert( (idxStr==0 && argc==0) || (int)strlen(idxStr)==argc*2 );
        for(ii=0; ii<argc; ii++){
          RtreeConstraint *p = &pCsr->aConstraint[ii];
          p->op = idxStr[ii*2];
          p->iCoord = idxStr[ii*2+1]-'a';
          if( p->op==RTREE_MATCH ){
            rc = deserializeGeometry(argv[ii], p);
            if( rc!=SQLITE_OK ){
              break;
            }
          }else{
            p->rValue = sqlite3_value_double(argv[ii]);
          }
        }
      }
    }
  
    if( rc==SQLITE_OK ){
      pCsr->pNode = 0;
      rc = nodeAcquire(pRtree, 1, 0, &pRoot);
    }
    if( rc==SQLITE_OK ){
      int isEof = 1;
      int nCell = NCELL(pRoot);
      pCsr->pNode = pRoot;
      for(pCsr->iCell=0; rc==SQLITE_OK && pCsr->iCell<nCell; pCsr->iCell++){
        assert( pCsr->pNode==pRoot );
        rc = descendToCell(pRtree, pCsr, pRtree->iDepth, &isEof);
        if( !isEof ){
          break;
        }
      }
      if( rc==SQLITE_OK && isEof ){
        assert( pCsr->pNode==pRoot );
        nodeRelease(pRtree, pRoot);
        pCsr->pNode = 0;
      }
      assert( rc!=SQLITE_OK || !pCsr->pNode || pCsr->iCell<NCELL(pCsr->pNode) );
    }
  }

  rtreeRelease(pRtree);
  return rc;
}

static int rtreeBestIndex(sqlite3_vtab *tab, sqlite3_index_info *pIdxInfo){
  int rc = SQLITE_OK;
  int ii;

  int iIdx = 0;
  char zIdxStr[RTREE_MAX_DIMENSIONS*8+1];
  memset(zIdxStr, 0, sizeof(zIdxStr));
  UNUSED_PARAMETER(tab);

  assert( pIdxInfo->idxStr==0 );
  for(ii=0; ii<pIdxInfo->nConstraint && iIdx<(int)(sizeof(zIdxStr)-1); ii++){
    struct sqlite3_index_constraint *p = &pIdxInfo->aConstraint[ii];

    if( p->usable && p->iColumn==0 && p->op==SQLITE_INDEX_CONSTRAINT_EQ ){
      
      int jj;
      for(jj=0; jj<ii; jj++){
        pIdxInfo->aConstraintUsage[jj].argvIndex = 0;
        pIdxInfo->aConstraintUsage[jj].omit = 0;
      }
      pIdxInfo->idxNum = 1;
      pIdxInfo->aConstraintUsage[ii].argvIndex = 1;
      pIdxInfo->aConstraintUsage[jj].omit = 1;

 
      pIdxInfo->estimatedCost = 10.0;
      return SQLITE_OK;
    }

    if( p->usable && (p->iColumn>0 || p->op==SQLITE_INDEX_CONSTRAINT_MATCH) ){
      u8 op;
      switch( p->op ){
        case SQLITE_INDEX_CONSTRAINT_EQ: op = RTREE_EQ; break;
        case SQLITE_INDEX_CONSTRAINT_GT: op = RTREE_GT; break;
        case SQLITE_INDEX_CONSTRAINT_LE: op = RTREE_LE; break;
        case SQLITE_INDEX_CONSTRAINT_LT: op = RTREE_LT; break;
        case SQLITE_INDEX_CONSTRAINT_GE: op = RTREE_GE; break;
        default:
          assert( p->op==SQLITE_INDEX_CONSTRAINT_MATCH );
          op = RTREE_MATCH; 
          break;
      }
      zIdxStr[iIdx++] = op;
      zIdxStr[iIdx++] = p->iColumn - 1 + 'a';
      pIdxInfo->aConstraintUsage[ii].argvIndex = (iIdx/2);
      pIdxInfo->aConstraintUsage[ii].omit = 1;
    }
  }

  pIdxInfo->idxNum = 2;
  pIdxInfo->needToFreeIdxStr = 1;
  if( iIdx>0 && 0==(pIdxInfo->idxStr = sqlite3_mprintf("%s", zIdxStr)) ){
    return SQLITE_NOMEM;
  }
  assert( iIdx>=0 );
  pIdxInfo->estimatedCost = (2000000.0 / (double)(iIdx + 1));
  return rc;
}

static float cellArea(Rtree *pRtree, RtreeCell *p){
  float area = 1.0;
  int ii;
  for(ii=0; ii<(pRtree->nDim*2); ii+=2){
    area = area * (DCOORD(p->aCoord[ii+1]) - DCOORD(p->aCoord[ii]));
  }
  return area;
}

static float cellMargin(Rtree *pRtree, RtreeCell *p){
  float margin = 0.0;
  int ii;
  for(ii=0; ii<(pRtree->nDim*2); ii+=2){
    margin += (DCOORD(p->aCoord[ii+1]) - DCOORD(p->aCoord[ii]));
  }
  return margin;
}

static void cellUnion(Rtree *pRtree, RtreeCell *p1, RtreeCell *p2){
  int ii;
  if( pRtree->eCoordType==RTREE_COORD_REAL32 ){
    for(ii=0; ii<(pRtree->nDim*2); ii+=2){
      p1->aCoord[ii].f = MIN(p1->aCoord[ii].f, p2->aCoord[ii].f);
      p1->aCoord[ii+1].f = MAX(p1->aCoord[ii+1].f, p2->aCoord[ii+1].f);
    }
  }else{
    for(ii=0; ii<(pRtree->nDim*2); ii+=2){
      p1->aCoord[ii].i = MIN(p1->aCoord[ii].i, p2->aCoord[ii].i);
      p1->aCoord[ii+1].i = MAX(p1->aCoord[ii+1].i, p2->aCoord[ii+1].i);
    }
  }
}

static int cellContains(Rtree *pRtree, RtreeCell *p1, RtreeCell *p2){
  int ii;
  int isInt = (pRtree->eCoordType==RTREE_COORD_INT32);
  for(ii=0; ii<(pRtree->nDim*2); ii+=2){
    RtreeCoord *a1 = &p1->aCoord[ii];
    RtreeCoord *a2 = &p2->aCoord[ii];
    if( (!isInt && (a2[0].f<a1[0].f || a2[1].f>a1[1].f)) 
     || ( isInt && (a2[0].i<a1[0].i || a2[1].i>a1[1].i)) 
    ){
      return 0;
    }
  }
  return 1;
}

static float cellGrowth(Rtree *pRtree, RtreeCell *p, RtreeCell *pCell){
  float area;
  RtreeCell cell;
  memcpy(&cell, p, sizeof(RtreeCell));
  area = cellArea(pRtree, &cell);
  cellUnion(pRtree, &cell, pCell);
  return (cellArea(pRtree, &cell)-area);
}

#if VARIANT_RSTARTREE_CHOOSESUBTREE || VARIANT_RSTARTREE_SPLIT
static float cellOverlap(
  Rtree *pRtree, 
  RtreeCell *p, 
  RtreeCell *aCell, 
  int nCell, 
  int iExclude
){
  int ii;
  float overlap = 0.0;
  for(ii=0; ii<nCell; ii++){
#if VARIANT_RSTARTREE_CHOOSESUBTREE
    if( ii!=iExclude )
#else
    assert( iExclude==-1 );
    UNUSED_PARAMETER(iExclude);
#endif
    {
      int jj;
      float o = 1.0;
      for(jj=0; jj<(pRtree->nDim*2); jj+=2){
        double x1;
        double x2;

        x1 = MAX(DCOORD(p->aCoord[jj]), DCOORD(aCell[ii].aCoord[jj]));
        x2 = MIN(DCOORD(p->aCoord[jj+1]), DCOORD(aCell[ii].aCoord[jj+1]));

        if( x2<x1 ){
          o = 0.0;
          break;
        }else{
          o = o * (x2-x1);
        }
      }
      overlap += o;
    }
  }
  return overlap;
}
#endif

#if VARIANT_RSTARTREE_CHOOSESUBTREE
static float cellOverlapEnlargement(
  Rtree *pRtree, 
  RtreeCell *p, 
  RtreeCell *pInsert, 
  RtreeCell *aCell, 
  int nCell, 
  int iExclude
){
  float before;
  float after;
  before = cellOverlap(pRtree, p, aCell, nCell, iExclude);
  cellUnion(pRtree, p, pInsert);
  after = cellOverlap(pRtree, p, aCell, nCell, iExclude);
  return after-before;
}
#endif


static int ChooseLeaf(
  Rtree *pRtree,               
  RtreeCell *pCell,            
  int iHeight,                 
  RtreeNode **ppLeaf           
){
  int rc;
  int ii;
  RtreeNode *pNode;
  rc = nodeAcquire(pRtree, 1, 0, &pNode);

  for(ii=0; rc==SQLITE_OK && ii<(pRtree->iDepth-iHeight); ii++){
    int iCell;
    sqlite3_int64 iBest;

    float fMinGrowth;
    float fMinArea;
    float fMinOverlap;

    int nCell = NCELL(pNode);
    RtreeCell cell;
    RtreeNode *pChild;

    RtreeCell *aCell = 0;

#if VARIANT_RSTARTREE_CHOOSESUBTREE
    if( ii==(pRtree->iDepth-1) ){
      int jj;
      aCell = sqlite3_malloc(sizeof(RtreeCell)*nCell);
      if( !aCell ){
        rc = SQLITE_NOMEM;
        nodeRelease(pRtree, pNode);
        pNode = 0;
        continue;
      }
      for(jj=0; jj<nCell; jj++){
        nodeGetCell(pRtree, pNode, jj, &aCell[jj]);
      }
    }
#endif

    for(iCell=0; iCell<nCell; iCell++){
      int bBest = 0;
      float growth;
      float area;
      float overlap = 0.0;
      nodeGetCell(pRtree, pNode, iCell, &cell);
      growth = cellGrowth(pRtree, &cell, pCell);
      area = cellArea(pRtree, &cell);

#if VARIANT_RSTARTREE_CHOOSESUBTREE
      if( ii==(pRtree->iDepth-1) ){
        overlap = cellOverlapEnlargement(pRtree,&cell,pCell,aCell,nCell,iCell);
      }
      if( (iCell==0) 
       || (overlap<fMinOverlap) 
       || (overlap==fMinOverlap && growth<fMinGrowth)
       || (overlap==fMinOverlap && growth==fMinGrowth && area<fMinArea)
      ){
        bBest = 1;
      }
#else
      if( iCell==0||growth<fMinGrowth||(growth==fMinGrowth && area<fMinArea) ){
        bBest = 1;
      }
#endif
      if( bBest ){
        fMinOverlap = overlap;
        fMinGrowth = growth;
        fMinArea = area;
        iBest = cell.iRowid;
      }
    }

    sqlite3_free(aCell);
    rc = nodeAcquire(pRtree, iBest, pNode, &pChild);
    nodeRelease(pRtree, pNode);
    pNode = pChild;
  }

  *ppLeaf = pNode;
  return rc;
}

static int AdjustTree(
  Rtree *pRtree,                    
  RtreeNode *pNode,                 
  RtreeCell *pCell                  
){
  RtreeNode *p = pNode;
  while( p->pParent ){
    RtreeNode *pParent = p->pParent;
    RtreeCell cell;
    int iCell;

    if( nodeParentIndex(pRtree, p, &iCell) ){
      return SQLITE_CORRUPT;
    }

    nodeGetCell(pRtree, pParent, iCell, &cell);
    if( !cellContains(pRtree, &cell, pCell) ){
      cellUnion(pRtree, &cell, pCell);
      nodeOverwriteCell(pRtree, pParent, &cell, iCell);
    }
 
    p = pParent;
  }
  return SQLITE_OK;
}

static int rowidWrite(Rtree *pRtree, sqlite3_int64 iRowid, sqlite3_int64 iNode){
  sqlite3_bind_int64(pRtree->pWriteRowid, 1, iRowid);
  sqlite3_bind_int64(pRtree->pWriteRowid, 2, iNode);
  sqlite3_step(pRtree->pWriteRowid);
  return sqlite3_reset(pRtree->pWriteRowid);
}

static int parentWrite(Rtree *pRtree, sqlite3_int64 iNode, sqlite3_int64 iPar){
  sqlite3_bind_int64(pRtree->pWriteParent, 1, iNode);
  sqlite3_bind_int64(pRtree->pWriteParent, 2, iPar);
  sqlite3_step(pRtree->pWriteParent);
  return sqlite3_reset(pRtree->pWriteParent);
}

static int rtreeInsertCell(Rtree *, RtreeNode *, RtreeCell *, int);

#if VARIANT_GUTTMAN_LINEAR_SPLIT
static RtreeCell *LinearPickNext(
  Rtree *pRtree,
  RtreeCell *aCell, 
  int nCell, 
  RtreeCell *pLeftBox, 
  RtreeCell *pRightBox,
  int *aiUsed
){
  int ii;
  for(ii=0; aiUsed[ii]; ii++);
  aiUsed[ii] = 1;
  return &aCell[ii];
}

static void LinearPickSeeds(
  Rtree *pRtree,
  RtreeCell *aCell, 
  int nCell, 
  int *piLeftSeed, 
  int *piRightSeed
){
  int i;
  int iLeftSeed = 0;
  int iRightSeed = 1;
  float maxNormalInnerWidth = 0.0;

  for(i=0; i<pRtree->nDim; i++){
    float x1 = DCOORD(aCell[0].aCoord[i*2]);
    float x2 = DCOORD(aCell[0].aCoord[i*2+1]);
    float x3 = x1;
    float x4 = x2;
    int jj;

    int iCellLeft = 0;
    int iCellRight = 0;

    for(jj=1; jj<nCell; jj++){
      float left = DCOORD(aCell[jj].aCoord[i*2]);
      float right = DCOORD(aCell[jj].aCoord[i*2+1]);

      if( left<x1 ) x1 = left;
      if( right>x4 ) x4 = right;
      if( left>x3 ){
        x3 = left;
        iCellRight = jj;
      }
      if( right<x2 ){
        x2 = right;
        iCellLeft = jj;
      }
    }

    if( x4!=x1 ){
      float normalwidth = (x3 - x2) / (x4 - x1);
      if( normalwidth>maxNormalInnerWidth ){
        iLeftSeed = iCellLeft;
        iRightSeed = iCellRight;
      }
    }
  }

  *piLeftSeed = iLeftSeed;
  *piRightSeed = iRightSeed;
}
#endif 

#if VARIANT_GUTTMAN_QUADRATIC_SPLIT
static RtreeCell *QuadraticPickNext(
  Rtree *pRtree,
  RtreeCell *aCell, 
  int nCell, 
  RtreeCell *pLeftBox, 
  RtreeCell *pRightBox,
  int *aiUsed
){
  #define FABS(a) ((a)<0.0?-1.0*(a):(a))

  int iSelect = -1;
  float fDiff;
  int ii;
  for(ii=0; ii<nCell; ii++){
    if( aiUsed[ii]==0 ){
      float left = cellGrowth(pRtree, pLeftBox, &aCell[ii]);
      float right = cellGrowth(pRtree, pLeftBox, &aCell[ii]);
      float diff = FABS(right-left);
      if( iSelect<0 || diff>fDiff ){
        fDiff = diff;
        iSelect = ii;
      }
    }
  }
  aiUsed[iSelect] = 1;
  return &aCell[iSelect];
}

static void QuadraticPickSeeds(
  Rtree *pRtree,
  RtreeCell *aCell, 
  int nCell, 
  int *piLeftSeed, 
  int *piRightSeed
){
  int ii;
  int jj;

  int iLeftSeed = 0;
  int iRightSeed = 1;
  float fWaste = 0.0;

  for(ii=0; ii<nCell; ii++){
    for(jj=ii+1; jj<nCell; jj++){
      float right = cellArea(pRtree, &aCell[jj]);
      float growth = cellGrowth(pRtree, &aCell[ii], &aCell[jj]);
      float waste = growth - right;

      if( waste>fWaste ){
        iLeftSeed = ii;
        iRightSeed = jj;
        fWaste = waste;
      }
    }
  }

  *piLeftSeed = iLeftSeed;
  *piRightSeed = iRightSeed;
}
#endif 

static void SortByDistance(
  int *aIdx, 
  int nIdx, 
  float *aDistance, 
  int *aSpare
){
  if( nIdx>1 ){
    int iLeft = 0;
    int iRight = 0;

    int nLeft = nIdx/2;
    int nRight = nIdx-nLeft;
    int *aLeft = aIdx;
    int *aRight = &aIdx[nLeft];

    SortByDistance(aLeft, nLeft, aDistance, aSpare);
    SortByDistance(aRight, nRight, aDistance, aSpare);

    memcpy(aSpare, aLeft, sizeof(int)*nLeft);
    aLeft = aSpare;

    while( iLeft<nLeft || iRight<nRight ){
      if( iLeft==nLeft ){
        aIdx[iLeft+iRight] = aRight[iRight];
        iRight++;
      }else if( iRight==nRight ){
        aIdx[iLeft+iRight] = aLeft[iLeft];
        iLeft++;
      }else{
        float fLeft = aDistance[aLeft[iLeft]];
        float fRight = aDistance[aRight[iRight]];
        if( fLeft<fRight ){
          aIdx[iLeft+iRight] = aLeft[iLeft];
          iLeft++;
        }else{
          aIdx[iLeft+iRight] = aRight[iRight];
          iRight++;
        }
      }
    }

#if 0
    
    {
      int jj;
      for(jj=1; jj<nIdx; jj++){
        float left = aDistance[aIdx[jj-1]];
        float right = aDistance[aIdx[jj]];
        assert( left<=right );
      }
    }
#endif
  }
}

static void SortByDimension(
  Rtree *pRtree,
  int *aIdx, 
  int nIdx, 
  int iDim, 
  RtreeCell *aCell, 
  int *aSpare
){
  if( nIdx>1 ){

    int iLeft = 0;
    int iRight = 0;

    int nLeft = nIdx/2;
    int nRight = nIdx-nLeft;
    int *aLeft = aIdx;
    int *aRight = &aIdx[nLeft];

    SortByDimension(pRtree, aLeft, nLeft, iDim, aCell, aSpare);
    SortByDimension(pRtree, aRight, nRight, iDim, aCell, aSpare);

    memcpy(aSpare, aLeft, sizeof(int)*nLeft);
    aLeft = aSpare;
    while( iLeft<nLeft || iRight<nRight ){
      double xleft1 = DCOORD(aCell[aLeft[iLeft]].aCoord[iDim*2]);
      double xleft2 = DCOORD(aCell[aLeft[iLeft]].aCoord[iDim*2+1]);
      double xright1 = DCOORD(aCell[aRight[iRight]].aCoord[iDim*2]);
      double xright2 = DCOORD(aCell[aRight[iRight]].aCoord[iDim*2+1]);
      if( (iLeft!=nLeft) && ((iRight==nRight)
       || (xleft1<xright1)
       || (xleft1==xright1 && xleft2<xright2)
      )){
        aIdx[iLeft+iRight] = aLeft[iLeft];
        iLeft++;
      }else{
        aIdx[iLeft+iRight] = aRight[iRight];
        iRight++;
      }
    }

#if 0
    
    {
      int jj;
      for(jj=1; jj<nIdx; jj++){
        float xleft1 = aCell[aIdx[jj-1]].aCoord[iDim*2];
        float xleft2 = aCell[aIdx[jj-1]].aCoord[iDim*2+1];
        float xright1 = aCell[aIdx[jj]].aCoord[iDim*2];
        float xright2 = aCell[aIdx[jj]].aCoord[iDim*2+1];
        assert( xleft1<=xright1 && (xleft1<xright1 || xleft2<=xright2) );
      }
    }
#endif
  }
}

#if VARIANT_RSTARTREE_SPLIT
static int splitNodeStartree(
  Rtree *pRtree,
  RtreeCell *aCell,
  int nCell,
  RtreeNode *pLeft,
  RtreeNode *pRight,
  RtreeCell *pBboxLeft,
  RtreeCell *pBboxRight
){
  int **aaSorted;
  int *aSpare;
  int ii;

  int iBestDim;
  int iBestSplit;
  float fBestMargin;

  int nByte = (pRtree->nDim+1)*(sizeof(int*)+nCell*sizeof(int));

  aaSorted = (int **)sqlite3_malloc(nByte);
  if( !aaSorted ){
    return SQLITE_NOMEM;
  }

  aSpare = &((int *)&aaSorted[pRtree->nDim])[pRtree->nDim*nCell];
  memset(aaSorted, 0, nByte);
  for(ii=0; ii<pRtree->nDim; ii++){
    int jj;
    aaSorted[ii] = &((int *)&aaSorted[pRtree->nDim])[ii*nCell];
    for(jj=0; jj<nCell; jj++){
      aaSorted[ii][jj] = jj;
    }
    SortByDimension(pRtree, aaSorted[ii], nCell, ii, aCell, aSpare);
  }

  for(ii=0; ii<pRtree->nDim; ii++){
    float margin = 0.0;
    float fBestOverlap;
    float fBestArea;
    int iBestLeft;
    int nLeft;

    for(
      nLeft=RTREE_MINCELLS(pRtree); 
      nLeft<=(nCell-RTREE_MINCELLS(pRtree)); 
      nLeft++
    ){
      RtreeCell left;
      RtreeCell right;
      int kk;
      float overlap;
      float area;

      memcpy(&left, &aCell[aaSorted[ii][0]], sizeof(RtreeCell));
      memcpy(&right, &aCell[aaSorted[ii][nCell-1]], sizeof(RtreeCell));
      for(kk=1; kk<(nCell-1); kk++){
        if( kk<nLeft ){
          cellUnion(pRtree, &left, &aCell[aaSorted[ii][kk]]);
        }else{
          cellUnion(pRtree, &right, &aCell[aaSorted[ii][kk]]);
        }
      }
      margin += cellMargin(pRtree, &left);
      margin += cellMargin(pRtree, &right);
      overlap = cellOverlap(pRtree, &left, &right, 1, -1);
      area = cellArea(pRtree, &left) + cellArea(pRtree, &right);
      if( (nLeft==RTREE_MINCELLS(pRtree))
       || (overlap<fBestOverlap)
       || (overlap==fBestOverlap && area<fBestArea)
      ){
        iBestLeft = nLeft;
        fBestOverlap = overlap;
        fBestArea = area;
      }
    }

    if( ii==0 || margin<fBestMargin ){
      iBestDim = ii;
      fBestMargin = margin;
      iBestSplit = iBestLeft;
    }
  }

  memcpy(pBboxLeft, &aCell[aaSorted[iBestDim][0]], sizeof(RtreeCell));
  memcpy(pBboxRight, &aCell[aaSorted[iBestDim][iBestSplit]], sizeof(RtreeCell));
  for(ii=0; ii<nCell; ii++){
    RtreeNode *pTarget = (ii<iBestSplit)?pLeft:pRight;
    RtreeCell *pBbox = (ii<iBestSplit)?pBboxLeft:pBboxRight;
    RtreeCell *pCell = &aCell[aaSorted[iBestDim][ii]];
    nodeInsertCell(pRtree, pTarget, pCell);
    cellUnion(pRtree, pBbox, pCell);
  }

  sqlite3_free(aaSorted);
  return SQLITE_OK;
}
#endif

#if VARIANT_GUTTMAN_SPLIT
static int splitNodeGuttman(
  Rtree *pRtree,
  RtreeCell *aCell,
  int nCell,
  RtreeNode *pLeft,
  RtreeNode *pRight,
  RtreeCell *pBboxLeft,
  RtreeCell *pBboxRight
){
  int iLeftSeed = 0;
  int iRightSeed = 1;
  int *aiUsed;
  int i;

  aiUsed = sqlite3_malloc(sizeof(int)*nCell);
  if( !aiUsed ){
    return SQLITE_NOMEM;
  }
  memset(aiUsed, 0, sizeof(int)*nCell);

  PickSeeds(pRtree, aCell, nCell, &iLeftSeed, &iRightSeed);

  memcpy(pBboxLeft, &aCell[iLeftSeed], sizeof(RtreeCell));
  memcpy(pBboxRight, &aCell[iRightSeed], sizeof(RtreeCell));
  nodeInsertCell(pRtree, pLeft, &aCell[iLeftSeed]);
  nodeInsertCell(pRtree, pRight, &aCell[iRightSeed]);
  aiUsed[iLeftSeed] = 1;
  aiUsed[iRightSeed] = 1;

  for(i=nCell-2; i>0; i--){
    RtreeCell *pNext;
    pNext = PickNext(pRtree, aCell, nCell, pBboxLeft, pBboxRight, aiUsed);
    float diff =  
      cellGrowth(pRtree, pBboxLeft, pNext) - 
      cellGrowth(pRtree, pBboxRight, pNext)
    ;
    if( (RTREE_MINCELLS(pRtree)-NCELL(pRight)==i)
     || (diff>0.0 && (RTREE_MINCELLS(pRtree)-NCELL(pLeft)!=i))
    ){
      nodeInsertCell(pRtree, pRight, pNext);
      cellUnion(pRtree, pBboxRight, pNext);
    }else{
      nodeInsertCell(pRtree, pLeft, pNext);
      cellUnion(pRtree, pBboxLeft, pNext);
    }
  }

  sqlite3_free(aiUsed);
  return SQLITE_OK;
}
#endif

static int updateMapping(
  Rtree *pRtree, 
  i64 iRowid, 
  RtreeNode *pNode, 
  int iHeight
){
  int (*xSetMapping)(Rtree *, sqlite3_int64, sqlite3_int64);
  xSetMapping = ((iHeight==0)?rowidWrite:parentWrite);
  if( iHeight>0 ){
    RtreeNode *pChild = nodeHashLookup(pRtree, iRowid);
    if( pChild ){
      nodeRelease(pRtree, pChild->pParent);
      nodeReference(pNode);
      pChild->pParent = pNode;
    }
  }
  return xSetMapping(pRtree, iRowid, pNode->iNode);
}

static int SplitNode(
  Rtree *pRtree,
  RtreeNode *pNode,
  RtreeCell *pCell,
  int iHeight
){
  int i;
  int newCellIsRight = 0;

  int rc = SQLITE_OK;
  int nCell = NCELL(pNode);
  RtreeCell *aCell;
  int *aiUsed;

  RtreeNode *pLeft = 0;
  RtreeNode *pRight = 0;

  RtreeCell leftbbox;
  RtreeCell rightbbox;

  aCell = sqlite3_malloc((sizeof(RtreeCell)+sizeof(int))*(nCell+1));
  if( !aCell ){
    rc = SQLITE_NOMEM;
    goto splitnode_out;
  }
  aiUsed = (int *)&aCell[nCell+1];
  memset(aiUsed, 0, sizeof(int)*(nCell+1));
  for(i=0; i<nCell; i++){
    nodeGetCell(pRtree, pNode, i, &aCell[i]);
  }
  nodeZero(pRtree, pNode);
  memcpy(&aCell[nCell], pCell, sizeof(RtreeCell));
  nCell++;

  if( pNode->iNode==1 ){
    pRight = nodeNew(pRtree, pNode);
    pLeft = nodeNew(pRtree, pNode);
    pRtree->iDepth++;
    pNode->isDirty = 1;
    writeInt16(pNode->zData, pRtree->iDepth);
  }else{
    pLeft = pNode;
    pRight = nodeNew(pRtree, pLeft->pParent);
    nodeReference(pLeft);
  }

  if( !pLeft || !pRight ){
    rc = SQLITE_NOMEM;
    goto splitnode_out;
  }

  memset(pLeft->zData, 0, pRtree->iNodeSize);
  memset(pRight->zData, 0, pRtree->iNodeSize);

  rc = AssignCells(pRtree, aCell, nCell, pLeft, pRight, &leftbbox, &rightbbox);
  if( rc!=SQLITE_OK ){
    goto splitnode_out;
  }

  if( SQLITE_OK!=(rc = nodeWrite(pRtree, pRight))
   || (0==pLeft->iNode && SQLITE_OK!=(rc = nodeWrite(pRtree, pLeft)))
  ){
    goto splitnode_out;
  }

  rightbbox.iRowid = pRight->iNode;
  leftbbox.iRowid = pLeft->iNode;

  if( pNode->iNode==1 ){
    rc = rtreeInsertCell(pRtree, pLeft->pParent, &leftbbox, iHeight+1);
    if( rc!=SQLITE_OK ){
      goto splitnode_out;
    }
  }else{
    RtreeNode *pParent = pLeft->pParent;
    int iCell;
    rc = nodeParentIndex(pRtree, pLeft, &iCell);
    if( rc==SQLITE_OK ){
      nodeOverwriteCell(pRtree, pParent, &leftbbox, iCell);
      rc = AdjustTree(pRtree, pParent, &leftbbox);
    }
    if( rc!=SQLITE_OK ){
      goto splitnode_out;
    }
  }
  if( (rc = rtreeInsertCell(pRtree, pRight->pParent, &rightbbox, iHeight+1)) ){
    goto splitnode_out;
  }

  for(i=0; i<NCELL(pRight); i++){
    i64 iRowid = nodeGetRowid(pRtree, pRight, i);
    rc = updateMapping(pRtree, iRowid, pRight, iHeight);
    if( iRowid==pCell->iRowid ){
      newCellIsRight = 1;
    }
    if( rc!=SQLITE_OK ){
      goto splitnode_out;
    }
  }
  if( pNode->iNode==1 ){
    for(i=0; i<NCELL(pLeft); i++){
      i64 iRowid = nodeGetRowid(pRtree, pLeft, i);
      rc = updateMapping(pRtree, iRowid, pLeft, iHeight);
      if( rc!=SQLITE_OK ){
        goto splitnode_out;
      }
    }
  }else if( newCellIsRight==0 ){
    rc = updateMapping(pRtree, pCell->iRowid, pLeft, iHeight);
  }

  if( rc==SQLITE_OK ){
    rc = nodeRelease(pRtree, pRight);
    pRight = 0;
  }
  if( rc==SQLITE_OK ){
    rc = nodeRelease(pRtree, pLeft);
    pLeft = 0;
  }

splitnode_out:
  nodeRelease(pRtree, pRight);
  nodeRelease(pRtree, pLeft);
  sqlite3_free(aCell);
  return rc;
}

static int fixLeafParent(Rtree *pRtree, RtreeNode *pLeaf){
  int rc = SQLITE_OK;
  RtreeNode *pChild = pLeaf;
  while( rc==SQLITE_OK && pChild->iNode!=1 && pChild->pParent==0 ){
    int rc2 = SQLITE_OK;          
    sqlite3_bind_int64(pRtree->pReadParent, 1, pChild->iNode);
    rc = sqlite3_step(pRtree->pReadParent);
    if( rc==SQLITE_ROW ){
      RtreeNode *pTest;           
      i64 iNode;                  

      iNode = sqlite3_column_int64(pRtree->pReadParent, 0);
      for(pTest=pLeaf; pTest && pTest->iNode!=iNode; pTest=pTest->pParent);
      if( !pTest ){
        rc2 = nodeAcquire(pRtree, iNode, 0, &pChild->pParent);
      }
    }
    rc = sqlite3_reset(pRtree->pReadParent);
    if( rc==SQLITE_OK ) rc = rc2;
    if( rc==SQLITE_OK && !pChild->pParent ) rc = SQLITE_CORRUPT;
    pChild = pChild->pParent;
  }
  return rc;
}

static int deleteCell(Rtree *, RtreeNode *, int, int);

static int removeNode(Rtree *pRtree, RtreeNode *pNode, int iHeight){
  int rc;
  int rc2;
  RtreeNode *pParent;
  int iCell;

  assert( pNode->nRef==1 );

  
  rc = nodeParentIndex(pRtree, pNode, &iCell);
  if( rc==SQLITE_OK ){
    pParent = pNode->pParent;
    pNode->pParent = 0;
    rc = deleteCell(pRtree, pParent, iCell, iHeight+1);
  }
  rc2 = nodeRelease(pRtree, pParent);
  if( rc==SQLITE_OK ){
    rc = rc2;
  }
  if( rc!=SQLITE_OK ){
    return rc;
  }

  
  sqlite3_bind_int64(pRtree->pDeleteNode, 1, pNode->iNode);
  sqlite3_step(pRtree->pDeleteNode);
  if( SQLITE_OK!=(rc = sqlite3_reset(pRtree->pDeleteNode)) ){
    return rc;
  }

  
  sqlite3_bind_int64(pRtree->pDeleteParent, 1, pNode->iNode);
  sqlite3_step(pRtree->pDeleteParent);
  if( SQLITE_OK!=(rc = sqlite3_reset(pRtree->pDeleteParent)) ){
    return rc;
  }
  
  nodeHashDelete(pRtree, pNode);
  pNode->iNode = iHeight;
  pNode->pNext = pRtree->pDeleted;
  pNode->nRef++;
  pRtree->pDeleted = pNode;

  return SQLITE_OK;
}

static int fixBoundingBox(Rtree *pRtree, RtreeNode *pNode){
  RtreeNode *pParent = pNode->pParent;
  int rc = SQLITE_OK; 
  if( pParent ){
    int ii; 
    int nCell = NCELL(pNode);
    RtreeCell box;                            
    nodeGetCell(pRtree, pNode, 0, &box);
    for(ii=1; ii<nCell; ii++){
      RtreeCell cell;
      nodeGetCell(pRtree, pNode, ii, &cell);
      cellUnion(pRtree, &box, &cell);
    }
    box.iRowid = pNode->iNode;
    rc = nodeParentIndex(pRtree, pNode, &ii);
    if( rc==SQLITE_OK ){
      nodeOverwriteCell(pRtree, pParent, &box, ii);
      rc = fixBoundingBox(pRtree, pParent);
    }
  }
  return rc;
}

static int deleteCell(Rtree *pRtree, RtreeNode *pNode, int iCell, int iHeight){
  RtreeNode *pParent;
  int rc;

  if( SQLITE_OK!=(rc = fixLeafParent(pRtree, pNode)) ){
    return rc;
  }

  nodeDeleteCell(pRtree, pNode, iCell);

  pParent = pNode->pParent;
  assert( pParent || pNode->iNode==1 );
  if( pParent ){
    if( NCELL(pNode)<RTREE_MINCELLS(pRtree) ){
      rc = removeNode(pRtree, pNode, iHeight);
    }else{
      rc = fixBoundingBox(pRtree, pNode);
    }
  }

  return rc;
}

static int Reinsert(
  Rtree *pRtree, 
  RtreeNode *pNode, 
  RtreeCell *pCell, 
  int iHeight
){
  int *aOrder;
  int *aSpare;
  RtreeCell *aCell;
  float *aDistance;
  int nCell;
  float aCenterCoord[RTREE_MAX_DIMENSIONS];
  int iDim;
  int ii;
  int rc = SQLITE_OK;

  memset(aCenterCoord, 0, sizeof(float)*RTREE_MAX_DIMENSIONS);

  nCell = NCELL(pNode)+1;

  aCell = (RtreeCell *)sqlite3_malloc(nCell * (
    sizeof(RtreeCell) +         
    sizeof(int)       +         
    sizeof(int)       +         
    sizeof(float)               
  ));
  if( !aCell ){
    return SQLITE_NOMEM;
  }
  aOrder    = (int *)&aCell[nCell];
  aSpare    = (int *)&aOrder[nCell];
  aDistance = (float *)&aSpare[nCell];

  for(ii=0; ii<nCell; ii++){
    if( ii==(nCell-1) ){
      memcpy(&aCell[ii], pCell, sizeof(RtreeCell));
    }else{
      nodeGetCell(pRtree, pNode, ii, &aCell[ii]);
    }
    aOrder[ii] = ii;
    for(iDim=0; iDim<pRtree->nDim; iDim++){
      aCenterCoord[iDim] += DCOORD(aCell[ii].aCoord[iDim*2]);
      aCenterCoord[iDim] += DCOORD(aCell[ii].aCoord[iDim*2+1]);
    }
  }
  for(iDim=0; iDim<pRtree->nDim; iDim++){
    aCenterCoord[iDim] = aCenterCoord[iDim]/((float)nCell*2.0);
  }

  for(ii=0; ii<nCell; ii++){
    aDistance[ii] = 0.0;
    for(iDim=0; iDim<pRtree->nDim; iDim++){
      float coord = DCOORD(aCell[ii].aCoord[iDim*2+1]) - 
          DCOORD(aCell[ii].aCoord[iDim*2]);
      aDistance[ii] += (coord-aCenterCoord[iDim])*(coord-aCenterCoord[iDim]);
    }
  }

  SortByDistance(aOrder, nCell, aDistance, aSpare);
  nodeZero(pRtree, pNode);

  for(ii=0; rc==SQLITE_OK && ii<(nCell-(RTREE_MINCELLS(pRtree)+1)); ii++){
    RtreeCell *p = &aCell[aOrder[ii]];
    nodeInsertCell(pRtree, pNode, p);
    if( p->iRowid==pCell->iRowid ){
      if( iHeight==0 ){
        rc = rowidWrite(pRtree, p->iRowid, pNode->iNode);
      }else{
        rc = parentWrite(pRtree, p->iRowid, pNode->iNode);
      }
    }
  }
  if( rc==SQLITE_OK ){
    rc = fixBoundingBox(pRtree, pNode);
  }
  for(; rc==SQLITE_OK && ii<nCell; ii++){
    RtreeNode *pInsert;
    RtreeCell *p = &aCell[aOrder[ii]];
    rc = ChooseLeaf(pRtree, p, iHeight, &pInsert);
    if( rc==SQLITE_OK ){
      int rc2;
      rc = rtreeInsertCell(pRtree, pInsert, p, iHeight);
      rc2 = nodeRelease(pRtree, pInsert);
      if( rc==SQLITE_OK ){
        rc = rc2;
      }
    }
  }

  sqlite3_free(aCell);
  return rc;
}

static int rtreeInsertCell(
  Rtree *pRtree,
  RtreeNode *pNode,
  RtreeCell *pCell,
  int iHeight
){
  int rc = SQLITE_OK;
  if( iHeight>0 ){
    RtreeNode *pChild = nodeHashLookup(pRtree, pCell->iRowid);
    if( pChild ){
      nodeRelease(pRtree, pChild->pParent);
      nodeReference(pNode);
      pChild->pParent = pNode;
    }
  }
  if( nodeInsertCell(pRtree, pNode, pCell) ){
#if VARIANT_RSTARTREE_REINSERT
    if( iHeight<=pRtree->iReinsertHeight || pNode->iNode==1){
      rc = SplitNode(pRtree, pNode, pCell, iHeight);
    }else{
      pRtree->iReinsertHeight = iHeight;
      rc = Reinsert(pRtree, pNode, pCell, iHeight);
    }
#else
    rc = SplitNode(pRtree, pNode, pCell, iHeight);
#endif
  }else{
    rc = AdjustTree(pRtree, pNode, pCell);
    if( rc==SQLITE_OK ){
      if( iHeight==0 ){
        rc = rowidWrite(pRtree, pCell->iRowid, pNode->iNode);
      }else{
        rc = parentWrite(pRtree, pCell->iRowid, pNode->iNode);
      }
    }
  }
  return rc;
}

static int reinsertNodeContent(Rtree *pRtree, RtreeNode *pNode){
  int ii;
  int rc = SQLITE_OK;
  int nCell = NCELL(pNode);

  for(ii=0; rc==SQLITE_OK && ii<nCell; ii++){
    RtreeNode *pInsert;
    RtreeCell cell;
    nodeGetCell(pRtree, pNode, ii, &cell);

    rc = ChooseLeaf(pRtree, &cell, pNode->iNode, &pInsert);
    if( rc==SQLITE_OK ){
      int rc2;
      rc = rtreeInsertCell(pRtree, pInsert, &cell, pNode->iNode);
      rc2 = nodeRelease(pRtree, pInsert);
      if( rc==SQLITE_OK ){
        rc = rc2;
      }
    }
  }
  return rc;
}

static int newRowid(Rtree *pRtree, i64 *piRowid){
  int rc;
  sqlite3_bind_null(pRtree->pWriteRowid, 1);
  sqlite3_bind_null(pRtree->pWriteRowid, 2);
  sqlite3_step(pRtree->pWriteRowid);
  rc = sqlite3_reset(pRtree->pWriteRowid);
  *piRowid = sqlite3_last_insert_rowid(pRtree->db);
  return rc;
}

static int rtreeUpdate(
  sqlite3_vtab *pVtab, 
  int nData, 
  sqlite3_value **azData, 
  sqlite_int64 *pRowid
){
  Rtree *pRtree = (Rtree *)pVtab;
  int rc = SQLITE_OK;

  rtreeReference(pRtree);

  assert(nData>=1);

  if( sqlite3_value_type(azData[0])!=SQLITE_NULL ){
    i64 iDelete;                
    RtreeNode *pLeaf;           
    int iCell;                  
    RtreeNode *pRoot;

    
    rc = nodeAcquire(pRtree, 1, 0, &pRoot);

    if( rc==SQLITE_OK ){
      iDelete = sqlite3_value_int64(azData[0]);
      rc = findLeafNode(pRtree, iDelete, &pLeaf);
    }

    
    if( rc==SQLITE_OK ){
      int rc2;
      rc = nodeRowidIndex(pRtree, pLeaf, iDelete, &iCell);
      if( rc==SQLITE_OK ){
        rc = deleteCell(pRtree, pLeaf, iCell, 0);
      }
      rc2 = nodeRelease(pRtree, pLeaf);
      if( rc==SQLITE_OK ){
        rc = rc2;
      }
    }

    
    if( rc==SQLITE_OK ){
      sqlite3_bind_int64(pRtree->pDeleteRowid, 1, iDelete);
      sqlite3_step(pRtree->pDeleteRowid);
      rc = sqlite3_reset(pRtree->pDeleteRowid);
    }

    if( rc==SQLITE_OK && pRtree->iDepth>0 && NCELL(pRoot)==1 ){
      int rc2;
      RtreeNode *pChild;
      i64 iChild = nodeGetRowid(pRtree, pRoot, 0);
      rc = nodeAcquire(pRtree, iChild, pRoot, &pChild);
      if( rc==SQLITE_OK ){
        rc = removeNode(pRtree, pChild, pRtree->iDepth-1);
      }
      rc2 = nodeRelease(pRtree, pChild);
      if( rc==SQLITE_OK ) rc = rc2;
      if( rc==SQLITE_OK ){
        pRtree->iDepth--;
        writeInt16(pRoot->zData, pRtree->iDepth);
        pRoot->isDirty = 1;
      }
    }

    
    for(pLeaf=pRtree->pDeleted; pLeaf; pLeaf=pRtree->pDeleted){
      if( rc==SQLITE_OK ){
        rc = reinsertNodeContent(pRtree, pLeaf);
      }
      pRtree->pDeleted = pLeaf->pNext;
      sqlite3_free(pLeaf);
    }

    
    if( rc==SQLITE_OK ){
      rc = nodeRelease(pRtree, pRoot);
    }else{
      nodeRelease(pRtree, pRoot);
    }
  }

  if( rc==SQLITE_OK && nData>1 ){
    
    RtreeCell cell;
    int ii;
    RtreeNode *pLeaf;

    
    assert( nData==(pRtree->nDim*2 + 3) );
    if( pRtree->eCoordType==RTREE_COORD_REAL32 ){
      for(ii=0; ii<(pRtree->nDim*2); ii+=2){
        cell.aCoord[ii].f = (float)sqlite3_value_double(azData[ii+3]);
        cell.aCoord[ii+1].f = (float)sqlite3_value_double(azData[ii+4]);
        if( cell.aCoord[ii].f>cell.aCoord[ii+1].f ){
          rc = SQLITE_CONSTRAINT;
          goto constraint;
        }
      }
    }else{
      for(ii=0; ii<(pRtree->nDim*2); ii+=2){
        cell.aCoord[ii].i = sqlite3_value_int(azData[ii+3]);
        cell.aCoord[ii+1].i = sqlite3_value_int(azData[ii+4]);
        if( cell.aCoord[ii].i>cell.aCoord[ii+1].i ){
          rc = SQLITE_CONSTRAINT;
          goto constraint;
        }
      }
    }

    
    if( sqlite3_value_type(azData[2])==SQLITE_NULL ){
      rc = newRowid(pRtree, &cell.iRowid);
    }else{
      cell.iRowid = sqlite3_value_int64(azData[2]);
      sqlite3_bind_int64(pRtree->pReadRowid, 1, cell.iRowid);
      if( SQLITE_ROW==sqlite3_step(pRtree->pReadRowid) ){
        sqlite3_reset(pRtree->pReadRowid);
        rc = SQLITE_CONSTRAINT;
        goto constraint;
      }
      rc = sqlite3_reset(pRtree->pReadRowid);
    }
    *pRowid = cell.iRowid;

    if( rc==SQLITE_OK ){
      rc = ChooseLeaf(pRtree, &cell, 0, &pLeaf);
    }
    if( rc==SQLITE_OK ){
      int rc2;
      pRtree->iReinsertHeight = -1;
      rc = rtreeInsertCell(pRtree, pLeaf, &cell, 0);
      rc2 = nodeRelease(pRtree, pLeaf);
      if( rc==SQLITE_OK ){
        rc = rc2;
      }
    }
  }

constraint:
  rtreeRelease(pRtree);
  return rc;
}

static int rtreeRename(sqlite3_vtab *pVtab, const char *zNewName){
  Rtree *pRtree = (Rtree *)pVtab;
  int rc = SQLITE_NOMEM;
  char *zSql = sqlite3_mprintf(
    "ALTER TABLE %Q.'%q_node'   RENAME TO \"%w_node\";"
    "ALTER TABLE %Q.'%q_parent' RENAME TO \"%w_parent\";"
    "ALTER TABLE %Q.'%q_rowid'  RENAME TO \"%w_rowid\";"
    , pRtree->zDb, pRtree->zName, zNewName 
    , pRtree->zDb, pRtree->zName, zNewName 
    , pRtree->zDb, pRtree->zName, zNewName
  );
  if( zSql ){
    rc = sqlite3_exec(pRtree->db, zSql, 0, 0, 0);
    sqlite3_free(zSql);
  }
  return rc;
}

static sqlite3_module rtreeModule = {
  0,                         
  rtreeCreate,                
  rtreeConnect,               
  rtreeBestIndex,             
  rtreeDisconnect,            
  rtreeDestroy,               
  rtreeOpen,                  
  rtreeClose,                 
  rtreeFilter,                
  rtreeNext,                  
  rtreeEof,                   
  rtreeColumn,                
  rtreeRowid,                 
  rtreeUpdate,                
  0,                          
  0,                          
  0,                          
  0,                          
  0,                          
  rtreeRename                 
};

static int rtreeSqlInit(
  Rtree *pRtree, 
  sqlite3 *db, 
  const char *zDb, 
  const char *zPrefix, 
  int isCreate
){
  int rc = SQLITE_OK;

  #define N_STATEMENT 9
  static const char *azSql[N_STATEMENT] = {
    
    "SELECT data FROM '%q'.'%q_node' WHERE nodeno = :1",
    "INSERT OR REPLACE INTO '%q'.'%q_node' VALUES(:1, :2)",
    "DELETE FROM '%q'.'%q_node' WHERE nodeno = :1",

    
    "SELECT nodeno FROM '%q'.'%q_rowid' WHERE rowid = :1",
    "INSERT OR REPLACE INTO '%q'.'%q_rowid' VALUES(:1, :2)",
    "DELETE FROM '%q'.'%q_rowid' WHERE rowid = :1",

    
    "SELECT parentnode FROM '%q'.'%q_parent' WHERE nodeno = :1",
    "INSERT OR REPLACE INTO '%q'.'%q_parent' VALUES(:1, :2)",
    "DELETE FROM '%q'.'%q_parent' WHERE nodeno = :1"
  };
  sqlite3_stmt **appStmt[N_STATEMENT];
  int i;

  pRtree->db = db;

  if( isCreate ){
    char *zCreate = sqlite3_mprintf(
"CREATE TABLE \"%w\".\"%w_node\"(nodeno INTEGER PRIMARY KEY, data BLOB);"
"CREATE TABLE \"%w\".\"%w_rowid\"(rowid INTEGER PRIMARY KEY, nodeno INTEGER);"
"CREATE TABLE \"%w\".\"%w_parent\"(nodeno INTEGER PRIMARY KEY, parentnode INTEGER);"
"INSERT INTO '%q'.'%q_node' VALUES(1, zeroblob(%d))",
      zDb, zPrefix, zDb, zPrefix, zDb, zPrefix, zDb, zPrefix, pRtree->iNodeSize
    );
    if( !zCreate ){
      return SQLITE_NOMEM;
    }
    rc = sqlite3_exec(db, zCreate, 0, 0, 0);
    sqlite3_free(zCreate);
    if( rc!=SQLITE_OK ){
      return rc;
    }
  }

  appStmt[0] = &pRtree->pReadNode;
  appStmt[1] = &pRtree->pWriteNode;
  appStmt[2] = &pRtree->pDeleteNode;
  appStmt[3] = &pRtree->pReadRowid;
  appStmt[4] = &pRtree->pWriteRowid;
  appStmt[5] = &pRtree->pDeleteRowid;
  appStmt[6] = &pRtree->pReadParent;
  appStmt[7] = &pRtree->pWriteParent;
  appStmt[8] = &pRtree->pDeleteParent;

  for(i=0; i<N_STATEMENT && rc==SQLITE_OK; i++){
    char *zSql = sqlite3_mprintf(azSql[i], zDb, zPrefix);
    if( zSql ){
      rc = sqlite3_prepare_v2(db, zSql, -1, appStmt[i], 0); 
    }else{
      rc = SQLITE_NOMEM;
    }
    sqlite3_free(zSql);
  }

  return rc;
}

/*
** The second argument to this function contains the text of an SQL statement
** that returns a single integer value. The statement is compiled and executed
** using database connection db. If successful, the integer value returned
** is written to *piVal and SQLITE_OK returned. Otherwise, an SQLite error
** code is returned and the value of *piVal after returning is not defined.
*/
static int getIntFromStmt(sqlite3 *db, const char *zSql, int *piVal){
  int rc = SQLITE_NOMEM;
  if( zSql ){
    sqlite3_stmt *pStmt = 0;
    rc = sqlite3_prepare_v2(db, zSql, -1, &pStmt, 0);
    if( rc==SQLITE_OK ){
      if( SQLITE_ROW==sqlite3_step(pStmt) ){
        *piVal = sqlite3_column_int(pStmt, 0);
      }
      rc = sqlite3_finalize(pStmt);
    }
  }
  return rc;
}

static int getNodeSize(
  sqlite3 *db,                    
  Rtree *pRtree,                  
  int isCreate                    
){
  int rc;
  char *zSql;
  if( isCreate ){
    int iPageSize;
    zSql = sqlite3_mprintf("PRAGMA %Q.page_size", pRtree->zDb);
    rc = getIntFromStmt(db, zSql, &iPageSize);
    if( rc==SQLITE_OK ){
      pRtree->iNodeSize = iPageSize-64;
      if( (4+pRtree->nBytesPerCell*RTREE_MAXCELLS)<pRtree->iNodeSize ){
        pRtree->iNodeSize = 4+pRtree->nBytesPerCell*RTREE_MAXCELLS;
      }
    }
  }else{
    zSql = sqlite3_mprintf(
        "SELECT length(data) FROM '%q'.'%q_node' WHERE nodeno = 1",
        pRtree->zDb, pRtree->zName
    );
    rc = getIntFromStmt(db, zSql, &pRtree->iNodeSize);
  }

  sqlite3_free(zSql);
  return rc;
}

static int rtreeInit(
  sqlite3 *db,                        
  void *pAux,                         
  int argc, const char *const*argv,   
  sqlite3_vtab **ppVtab,              
  char **pzErr,                       
  int isCreate                        
){
  int rc = SQLITE_OK;
  Rtree *pRtree;
  int nDb;              
  int nName;            
  int eCoordType = (pAux ? RTREE_COORD_INT32 : RTREE_COORD_REAL32);

  const char *aErrMsg[] = {
    0,                                                    
    "Wrong number of columns for an rtree table",         
    "Too few columns for an rtree table",                 
    "Too many columns for an rtree table"                 
  };

  int iErr = (argc<6) ? 2 : argc>(RTREE_MAX_DIMENSIONS*2+4) ? 3 : argc%2;
  if( aErrMsg[iErr] ){
    *pzErr = sqlite3_mprintf("%s", aErrMsg[iErr]);
    return SQLITE_ERROR;
  }

  
  nDb = strlen(argv[1]);
  nName = strlen(argv[2]);
  pRtree = (Rtree *)sqlite3_malloc(sizeof(Rtree)+nDb+nName+2);
  if( !pRtree ){
    return SQLITE_NOMEM;
  }
  memset(pRtree, 0, sizeof(Rtree)+nDb+nName+2);
  pRtree->nBusy = 1;
  pRtree->base.pModule = &rtreeModule;
  pRtree->zDb = (char *)&pRtree[1];
  pRtree->zName = &pRtree->zDb[nDb+1];
  pRtree->nDim = (argc-4)/2;
  pRtree->nBytesPerCell = 8 + pRtree->nDim*4*2;
  pRtree->eCoordType = eCoordType;
  memcpy(pRtree->zDb, argv[1], nDb);
  memcpy(pRtree->zName, argv[2], nName);

  
  rc = getNodeSize(db, pRtree, isCreate);

  if( rc==SQLITE_OK ){
    if( (rc = rtreeSqlInit(pRtree, db, argv[1], argv[2], isCreate)) ){
      *pzErr = sqlite3_mprintf("%s", sqlite3_errmsg(db));
    }else{
      char *zSql = sqlite3_mprintf("CREATE TABLE x(%s", argv[3]);
      char *zTmp;
      int ii;
      for(ii=4; zSql && ii<argc; ii++){
        zTmp = zSql;
        zSql = sqlite3_mprintf("%s, %s", zTmp, argv[ii]);
        sqlite3_free(zTmp);
      }
      if( zSql ){
        zTmp = zSql;
        zSql = sqlite3_mprintf("%s);", zTmp);
        sqlite3_free(zTmp);
      }
      if( !zSql ){
        rc = SQLITE_NOMEM;
      }else if( SQLITE_OK!=(rc = sqlite3_declare_vtab(db, zSql)) ){
        *pzErr = sqlite3_mprintf("%s", sqlite3_errmsg(db));
      }
      sqlite3_free(zSql);
    }
  }

  if( rc==SQLITE_OK ){
    *ppVtab = (sqlite3_vtab *)pRtree;
  }else{
    rtreeRelease(pRtree);
  }
  return rc;
}


static void rtreenode(sqlite3_context *ctx, int nArg, sqlite3_value **apArg){
  char *zText = 0;
  RtreeNode node;
  Rtree tree;
  int ii;

  UNUSED_PARAMETER(nArg);
  memset(&node, 0, sizeof(RtreeNode));
  memset(&tree, 0, sizeof(Rtree));
  tree.nDim = sqlite3_value_int(apArg[0]);
  tree.nBytesPerCell = 8 + 8 * tree.nDim;
  node.zData = (u8 *)sqlite3_value_blob(apArg[1]);

  for(ii=0; ii<NCELL(&node); ii++){
    char zCell[512];
    int nCell = 0;
    RtreeCell cell;
    int jj;

    nodeGetCell(&tree, &node, ii, &cell);
    sqlite3_snprintf(512-nCell,&zCell[nCell],"%lld", cell.iRowid);
    nCell = strlen(zCell);
    for(jj=0; jj<tree.nDim*2; jj++){
      sqlite3_snprintf(512-nCell,&zCell[nCell]," %f",(double)cell.aCoord[jj].f);
      nCell = strlen(zCell);
    }

    if( zText ){
      char *zTextNew = sqlite3_mprintf("%s {%s}", zText, zCell);
      sqlite3_free(zText);
      zText = zTextNew;
    }else{
      zText = sqlite3_mprintf("{%s}", zCell);
    }
  }
  
  sqlite3_result_text(ctx, zText, -1, sqlite3_free);
}

static void rtreedepth(sqlite3_context *ctx, int nArg, sqlite3_value **apArg){
  UNUSED_PARAMETER(nArg);
  if( sqlite3_value_type(apArg[0])!=SQLITE_BLOB 
   || sqlite3_value_bytes(apArg[0])<2
  ){
    sqlite3_result_error(ctx, "Invalid argument to rtreedepth()", -1); 
  }else{
    u8 *zBlob = (u8 *)sqlite3_value_blob(apArg[0]);
    sqlite3_result_int(ctx, readInt16(zBlob));
  }
}

int sqlite3RtreeInit(sqlite3 *db){
  const int utf8 = SQLITE_UTF8;
  int rc;

  rc = sqlite3_create_function(db, "rtreenode", 2, utf8, 0, rtreenode, 0, 0);
  if( rc==SQLITE_OK ){
    rc = sqlite3_create_function(db, "rtreedepth", 1, utf8, 0,rtreedepth, 0, 0);
  }
  if( rc==SQLITE_OK ){
    void *c = (void *)RTREE_COORD_REAL32;
    rc = sqlite3_create_module_v2(db, "rtree", &rtreeModule, c, 0);
  }
  if( rc==SQLITE_OK ){
    void *c = (void *)RTREE_COORD_INT32;
    rc = sqlite3_create_module_v2(db, "rtree_i32", &rtreeModule, c, 0);
  }

  return rc;
}

static void doSqlite3Free(void *p){
  sqlite3_free(p);
}

static void geomCallback(sqlite3_context *ctx, int nArg, sqlite3_value **aArg){
  RtreeGeomCallback *pGeomCtx = (RtreeGeomCallback *)sqlite3_user_data(ctx);
  RtreeMatchArg *pBlob;
  int nBlob;

  nBlob = sizeof(RtreeMatchArg) + (nArg-1)*sizeof(double);
  pBlob = (RtreeMatchArg *)sqlite3_malloc(nBlob);
  if( !pBlob ){
    sqlite3_result_error_nomem(ctx);
  }else{
    int i;
    pBlob->magic = RTREE_GEOMETRY_MAGIC;
    pBlob->xGeom = pGeomCtx->xGeom;
    pBlob->pContext = pGeomCtx->pContext;
    pBlob->nParam = nArg;
    for(i=0; i<nArg; i++){
      pBlob->aParam[i] = sqlite3_value_double(aArg[i]);
    }
    sqlite3_result_blob(ctx, pBlob, nBlob, doSqlite3Free);
  }
}

int sqlite3_rtree_geometry_callback(
  sqlite3 *db,
  const char *zGeom,
  int (*xGeom)(sqlite3_rtree_geometry *, int, double *, int *),
  void *pContext
){
  RtreeGeomCallback *pGeomCtx;      

  
  pGeomCtx = (RtreeGeomCallback *)sqlite3_malloc(sizeof(RtreeGeomCallback));
  if( !pGeomCtx ) return SQLITE_NOMEM;
  pGeomCtx->xGeom = xGeom;
  pGeomCtx->pContext = pContext;

  return sqlite3_create_function_v2(db, zGeom, -1, SQLITE_ANY, 
      (void *)pGeomCtx, geomCallback, 0, 0, doSqlite3Free
  );
}

#if !SQLITE_CORE
int sqlite3_extension_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
  SQLITE_EXTENSION_INIT2(pApi)
  return sqlite3RtreeInit(db);
}
#endif

#endif

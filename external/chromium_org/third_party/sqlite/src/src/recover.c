/*
** 2012 Jan 11
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
*/

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "sqliteInt.h"

#if 0
#define FNENTRY() fprintf(stderr, "In %s\n", __FUNCTION__)
#else
#define FNENTRY()
#endif


static const unsigned char kTableLeafPage = 0x0D;
static const unsigned char kTableInteriorPage = 0x05;

static const unsigned kiPageTypeOffset = 0;
static const unsigned kiPageFreeBlockOffset = 1;
static const unsigned kiPageCellCountOffset = 3;
static const unsigned kiPageCellContentOffset = 5;
static const unsigned kiPageFragmentedBytesOffset = 7;
static const unsigned knPageLeafHeaderBytes = 8;
static const unsigned kiPageRightChildOffset = 8;
static const unsigned kiPageInteriorHeaderBytes = 12;

#define MASK_ROWID (1<<0)
#define MASK_INTEGER (1<<1)
#define MASK_FLOAT (1<<2)
#define MASK_TEXT (1<<3)
#define MASK_BLOB (1<<4)
#define MASK_NULL (1<<5)

static u32 decodeUnsigned16(const unsigned char *pData){
  return (pData[0]<<8) + pData[1];
}
static u32 decodeUnsigned32(const unsigned char *pData){
  return (decodeUnsigned16(pData)<<16) + decodeUnsigned16(pData+2);
}
static i64 decodeSigned(const unsigned char *pData, unsigned nBytes){
  i64 r = (char)(*pData);
  while( --nBytes ){
    r <<= 8;
    r += *(++pData);
  }
  return r;
}
static double decodeFloat64(const unsigned char *pData){
#if !defined(NDEBUG)
  static const u64 t1 = ((u64)0x3ff00000)<<32;
  static const double r1 = 1.0;
  u64 t2 = t1;
  assert( sizeof(r1)==sizeof(t2) && memcmp(&r1, &t2, sizeof(r1))==0 );
#endif
  i64 x = decodeSigned(pData, 8);
  double d;
  memcpy(&d, &x, sizeof(x));
  return d;
}

static int checkVarint(const unsigned char *pData, unsigned nData){
  unsigned i;

  
  if( nData>=9 ){
    return 1;
  }

  
  for( i=0; i<nData; ++i ){
    if( !(pData[i]&0x80) ){
      return 1;
    }
  }

  
  return 0;
}

static int checkVarints(const unsigned char *pData, unsigned nData,
                        unsigned n){
  unsigned nCur = 0;   
  unsigned nFound = 0; 
  unsigned i;

  
  if( nData>=9*n ){
    return 1;
  }

  for( i=0; nFound<n && i<nData; ++i ){
    nCur++;
    if( nCur==9 || !(pData[i]&0x80) ){
      nFound++;
      nCur = 0;
    }
  }

  return nFound==n;
}

static int ascii_isspace(char c){
  
  return c==' ' || c=='\t' || c=='\n' || c=='\r' || c=='\v' || c=='\f';
}
static int ascii_isalnum(int x){
  
  return (x>='0' && x<='9') || (x>='A' && x<='Z') || (x>='a' && x<='z');
}
static int ascii_tolower(int x){
  
  return (x>='A' && x<='Z') ? x-'A'+'a' : x;
}
static int ascii_strncasecmp(const char *s1, const char *s2, size_t n){
  const unsigned char *us1 = (const unsigned char *)s1;
  const unsigned char *us2 = (const unsigned char *)s2;
  while( *us1 && *us2 && n && ascii_tolower(*us1)==ascii_tolower(*us2) ){
    us1++, us2++, n--;
  }
  return n ? ascii_tolower(*us1)-ascii_tolower(*us2) : 0;
}
static int ascii_strcasecmp(const char *s1, const char *s2){
  return ascii_strncasecmp(s1, s2, strlen(s1)+1);
}

static const unsigned char *PageData(DbPage *pPage, unsigned iOffset){
  assert( iOffset<=pPage->nPageSize );
  return (unsigned char *)pPage->pData + iOffset;
}

static const unsigned char *PageHeader(DbPage *pPage){
  if( pPage->pgno==1 ){
    const unsigned nDatabaseHeader = 100;
    return PageData(pPage, nDatabaseHeader);
  }else{
    return PageData(pPage, 0);
  }
}

static int GetPager(sqlite3 *db, const char *zName,
                    Pager **pPager, unsigned *pnPageSize){
  Btree *pBt = NULL;
  int i;
  for( i=0; i<db->nDb; ++i ){
    if( ascii_strcasecmp(db->aDb[i].zName, zName)==0 ){
      pBt = db->aDb[i].pBt;
      break;
    }
  }
  if( !pBt ){
    return SQLITE_ERROR;
  }

  *pPager = sqlite3BtreePager(pBt);
  *pnPageSize = sqlite3BtreeGetPageSize(pBt) - sqlite3BtreeGetReserve(pBt);
  return SQLITE_OK;
}


static u32 SerialTypeLength(u64 iSerialType){
  switch( iSerialType ){
    case 0 : return 0;  
    case 1 : return 1;  
    case 2 : return 2;
    case 3 : return 3;
    case 4 : return 4;
    case 5 : return 6;
    case 6 : return 8;
    case 7 : return 8;  
    case 8 : return 0;  
    case 9 : return 0;  
    case 10 : case 11 : assert( !"RESERVED TYPE"); return 0;
  }
  return (u32)((iSerialType>>1) - 6);
}

static int SerialTypeIsBlob(u64 iSerialType){
  assert( iSerialType>=12 );
  return (iSerialType%2)==0;
}

static int SerialTypeIsCompatible(u64 iSerialType, unsigned char mask){
  switch( iSerialType ){
    case 0  : return (mask&MASK_NULL)!=0;
    case 1  : return (mask&MASK_INTEGER)!=0;
    case 2  : return (mask&MASK_INTEGER)!=0;
    case 3  : return (mask&MASK_INTEGER)!=0;
    case 4  : return (mask&MASK_INTEGER)!=0;
    case 5  : return (mask&MASK_INTEGER)!=0;
    case 6  : return (mask&MASK_INTEGER)!=0;
    case 7  : return (mask&MASK_FLOAT)!=0;
    case 8  : return (mask&MASK_INTEGER)!=0;
    case 9  : return (mask&MASK_INTEGER)!=0;
    case 10 : assert( !"RESERVED TYPE"); return 0;
    case 11 : assert( !"RESERVED TYPE"); return 0;
  }
  return (mask&(SerialTypeIsBlob(iSerialType) ? MASK_BLOB : MASK_TEXT));
}

static char *sqlite3_strndup(const char *z, unsigned n){
  char *zNew;

  if( z==NULL ){
    return NULL;
  }

  zNew = sqlite3_malloc(n+1);
  if( zNew!=NULL ){
    memcpy(zNew, z, n);
    zNew[n] = '\0';
  }
  return zNew;
}
static char *sqlite3_strdup(const char *z){
  if( z==NULL ){
    return NULL;
  }
  return sqlite3_strndup(z, strlen(z));
}

static int getRootPage(sqlite3 *db, const char *zDb, const char *zTable,
                       u32 *piRootPage){
  char *zSql;  
  sqlite3_stmt *pStmt;
  int rc;

  if( strcmp(zTable, "sqlite_master")==0 ){
    *piRootPage = 1;
    return SQLITE_OK;
  }

  zSql = sqlite3_mprintf("SELECT rootpage FROM %s.sqlite_master "
                         "WHERE type = 'table' AND tbl_name = %Q",
                         zDb, zTable);
  if( !zSql ){
    return SQLITE_NOMEM;
  }

  rc = sqlite3_prepare_v2(db, zSql, -1, &pStmt, 0);
  sqlite3_free(zSql);
  if( rc!=SQLITE_OK ){
    return rc;
  }

  
  rc = sqlite3_step(pStmt);
  if( rc==SQLITE_DONE ){
    rc = SQLITE_CORRUPT;
  }else if( rc==SQLITE_ROW ){
    *piRootPage = sqlite3_column_int(pStmt, 0);

    
    rc = sqlite3_step(pStmt);
    if( rc==SQLITE_DONE ){
      rc = SQLITE_OK;
    }else if( rc==SQLITE_ROW ){
      rc = SQLITE_CORRUPT;
    }
  }
  sqlite3_finalize(pStmt);
  return rc;
}

static int getEncoding(sqlite3 *db, const char *zDb, int* piEncoding){
  sqlite3_stmt *pStmt;
  int rc;
  char *zSql = sqlite3_mprintf("PRAGMA %s.encoding", zDb);
  if( !zSql ){
    return SQLITE_NOMEM;
  }

  rc = sqlite3_prepare_v2(db, zSql, -1, &pStmt, 0);
  sqlite3_free(zSql);
  if( rc!=SQLITE_OK ){
    return rc;
  }

  
  rc = sqlite3_step(pStmt);
  if( rc==SQLITE_DONE ){
    
    rc = SQLITE_CORRUPT;
  }else if( rc==SQLITE_ROW ){
    if( sqlite3_column_type(pStmt, 0)==SQLITE_TEXT ){
      const char* z = (const char *)sqlite3_column_text(pStmt, 0);
      
      if( !strcmp(z, "UTF-16le") ){
        *piEncoding = SQLITE_UTF16LE;
      }else if( !strcmp(z, "UTF-16be") ){
        *piEncoding = SQLITE_UTF16BE;
      }else if( !strcmp(z, "UTF-8") ){
        *piEncoding = SQLITE_UTF8;
      }else{
        
        *piEncoding = SQLITE_UTF8;
      }
    }else{
      
      *piEncoding = SQLITE_UTF8;
    }

    
    rc = sqlite3_step(pStmt);
    if( rc==SQLITE_DONE ){
      rc = SQLITE_OK;
    }else if( rc==SQLITE_ROW ){
      
      rc = SQLITE_CORRUPT;
    }
  }
  sqlite3_finalize(pStmt);
  return rc;
}

typedef struct RecoverInteriorCursor RecoverInteriorCursor;
struct RecoverInteriorCursor {
  RecoverInteriorCursor *pParent; 
  DbPage *pPage;                  
  unsigned nPageSize;             
  unsigned nChildren;             
  unsigned iChild;                
};

static void interiorCursorDestroy(RecoverInteriorCursor *pCursor){
  
  while( pCursor ){
    RecoverInteriorCursor *p = pCursor;
    pCursor = pCursor->pParent;

    if( p->pPage ){
      sqlite3PagerUnref(p->pPage);
      p->pPage = NULL;
    }

    memset(p, 0xA5, sizeof(*p));
    sqlite3_free(p);
  }
}

static void interiorCursorSetPage(RecoverInteriorCursor *pCursor,
                                  DbPage *pPage){
  assert( PageHeader(pPage)[kiPageTypeOffset]==kTableInteriorPage );

  if( pCursor->pPage ){
    sqlite3PagerUnref(pCursor->pPage);
    pCursor->pPage = NULL;
  }
  pCursor->pPage = pPage;
  pCursor->iChild = 0;

  
  pCursor->nChildren = decodeUnsigned16(PageHeader(pPage) +
                                        kiPageCellCountOffset) + 1;
}

static int interiorCursorCreate(RecoverInteriorCursor *pParent,
                                DbPage *pPage, int nPageSize,
                                RecoverInteriorCursor **ppCursor){
  RecoverInteriorCursor *pCursor =
    sqlite3_malloc(sizeof(RecoverInteriorCursor));
  if( !pCursor ){
    return SQLITE_NOMEM;
  }

  memset(pCursor, 0, sizeof(*pCursor));
  pCursor->pParent = pParent;
  pCursor->nPageSize = nPageSize;
  interiorCursorSetPage(pCursor, pPage);
  *ppCursor = pCursor;
  return SQLITE_OK;
}

static unsigned interiorCursorChildPage(RecoverInteriorCursor *pCursor){
  const unsigned char *pPageHeader;  
  const unsigned char *pCellOffsets; 
  unsigned iCellOffset;              

  assert( pCursor->iChild<pCursor->nChildren );

  
  pPageHeader = PageHeader(pCursor->pPage);
  if( pCursor->iChild==pCursor->nChildren-1 ){
    return decodeUnsigned32(pPageHeader + kiPageRightChildOffset);
  }

  pCellOffsets = pPageHeader + kiPageInteriorHeaderBytes;
  iCellOffset = decodeUnsigned16(pCellOffsets + pCursor->iChild*2);
  if( iCellOffset<=pCursor->nPageSize-4 ){
    return decodeUnsigned32(PageData(pCursor->pPage, iCellOffset));
  }


  
  return 0;
}

static int interiorCursorEOF(RecoverInteriorCursor *pCursor){
  
  while( pCursor && pCursor->iChild>=pCursor->nChildren ){
    pCursor = pCursor->pParent;
  }
  return pCursor==NULL;
}

static int interiorCursorPageInUse(RecoverInteriorCursor *pCursor,
                                   unsigned iPage){
  
  while( pCursor && pCursor->pPage->pgno!=iPage ){
    pCursor = pCursor->pParent;
  }
  return pCursor!=NULL;
}

static int interiorCursorNextPage(RecoverInteriorCursor **ppCursor,
                                  DbPage **ppPage){
  RecoverInteriorCursor *pCursor = *ppCursor;
  while( 1 ){
    int rc;
    const unsigned char *pPageHeader;  

    
    while( pCursor->iChild<pCursor->nChildren ){
      const unsigned iPage = interiorCursorChildPage(pCursor);
      pCursor->iChild++;
      if( interiorCursorPageInUse(pCursor, iPage) ){
        fprintf(stderr, "Loop detected at %d\n", iPage);
      }else{
        int rc = sqlite3PagerAcquire(pCursor->pPage->pPager, iPage, ppPage, 0);
        if( rc==SQLITE_OK ){
          return SQLITE_ROW;
        }
      }
    }

    
    if( !pCursor->pParent ){
      return SQLITE_DONE;
    }
    rc = interiorCursorNextPage(&pCursor->pParent, ppPage);
    if( rc!=SQLITE_ROW ){
      return rc;
    }

    pPageHeader = PageHeader(*ppPage);
    if( pPageHeader[kiPageTypeOffset]!=kTableInteriorPage ){
      *ppCursor = pCursor->pParent;
      pCursor->pParent = NULL;
      interiorCursorDestroy(pCursor);
      return SQLITE_ROW;
    }

    
    interiorCursorSetPage(pCursor, *ppPage);
    *ppPage = NULL;
  }

  assert(NULL);  
  return SQLITE_CORRUPT;
}

typedef struct RecoverOverflow RecoverOverflow;
struct RecoverOverflow {
  RecoverOverflow *pNextOverflow;
  DbPage *pPage;
  unsigned nPageSize;
};

static void overflowDestroy(RecoverOverflow *pOverflow){
  while( pOverflow ){
    RecoverOverflow *p = pOverflow;
    pOverflow = p->pNextOverflow;

    if( p->pPage ){
      sqlite3PagerUnref(p->pPage);
      p->pPage = NULL;
    }

    memset(p, 0xA5, sizeof(*p));
    sqlite3_free(p);
  }
}

static int overflowPageInUse(RecoverOverflow *pOverflow, unsigned iPage){
  while( pOverflow && pOverflow->pPage->pgno!=iPage ){
    pOverflow = pOverflow->pNextOverflow;
  }
  return pOverflow!=NULL;
}

static int overflowMaybeCreate(DbPage *pPage, unsigned nPageSize,
                               unsigned iRecordOffset, unsigned nRecordBytes,
                               unsigned *pnLocalRecordBytes,
                               RecoverOverflow **ppOverflow){
  unsigned nLocalRecordBytes;  
  unsigned iNextPage;          
  unsigned nBytes;             
  int rc;
  RecoverOverflow *pFirstOverflow;  
  RecoverOverflow *pLastOverflow;   

  const unsigned maxLocal = nPageSize - 35;
  const unsigned minLocal = ((nPageSize-12)*32/255)-23;  

  
  if( nRecordBytes<=maxLocal ){
    *pnLocalRecordBytes = nRecordBytes;
    *ppOverflow = NULL;
    return SQLITE_OK;
  }

  nLocalRecordBytes = minLocal+((nRecordBytes-minLocal)%(nPageSize-4));
  if( maxLocal<nLocalRecordBytes ){
    nLocalRecordBytes = minLocal;
  }

  
  if( iRecordOffset+nLocalRecordBytes+4>nPageSize ){
    return SQLITE_CORRUPT;
  }

  
  iNextPage =
      decodeUnsigned32(PageData(pPage, iRecordOffset + nLocalRecordBytes));
  nBytes = nLocalRecordBytes;

  pFirstOverflow = pLastOverflow = NULL;
  rc = SQLITE_OK;
  while( iNextPage && nBytes<nRecordBytes ){
    RecoverOverflow *pOverflow;  

    rc = sqlite3PagerAcquire(pPage->pPager, iNextPage, &pPage, 0);
    if( rc!=SQLITE_OK ){
      break;
    }

    pOverflow = sqlite3_malloc(sizeof(RecoverOverflow));
    if( !pOverflow ){
      sqlite3PagerUnref(pPage);
      rc = SQLITE_NOMEM;
      break;
    }
    memset(pOverflow, 0, sizeof(*pOverflow));
    pOverflow->pPage = pPage;
    pOverflow->nPageSize = nPageSize;

    if( !pFirstOverflow ){
      pFirstOverflow = pOverflow;
    }else{
      pLastOverflow->pNextOverflow = pOverflow;
    }
    pLastOverflow = pOverflow;

    iNextPage = decodeUnsigned32(pPage->pData);
    nBytes += nPageSize-4;

    
    if( overflowPageInUse(pFirstOverflow, iNextPage) ){
      fprintf(stderr, "Overflow loop detected at %d\n", iNextPage);
      rc = SQLITE_CORRUPT;
      break;
    }
  }

  if( rc==SQLITE_OK && (nBytes<nRecordBytes || iNextPage) ){
    rc = SQLITE_CORRUPT;
  }

  if( rc==SQLITE_OK ){
    *ppOverflow = pFirstOverflow;
    *pnLocalRecordBytes = nLocalRecordBytes;
  }else if( pFirstOverflow ){
    overflowDestroy(pFirstOverflow);
  }
  return rc;
}

static int overflowGetSegment(DbPage *pPage, unsigned iRecordOffset,
                              unsigned nLocalRecordBytes,
                              RecoverOverflow *pOverflow,
                              unsigned iRequestOffset, unsigned nRequestBytes,
                              unsigned char **ppBase, int *pbFree){
  unsigned nBase;         
  unsigned char *pBase;   

  
  while( iRequestOffset>=nLocalRecordBytes && pOverflow ){
    
    iRequestOffset -= nLocalRecordBytes;

    
    pPage = pOverflow->pPage;
    iRecordOffset = 4;
    nLocalRecordBytes = pOverflow->nPageSize - iRecordOffset;
    pOverflow = pOverflow->pNextOverflow;
  }

  if( iRequestOffset+nRequestBytes<=nLocalRecordBytes ){
    *ppBase = (unsigned char *)PageData(pPage, iRecordOffset + iRequestOffset);
    *pbFree = 0;
    return SQLITE_OK;
  }

  
  if( !pOverflow ){
    assert(NULL);  
    return SQLITE_ERROR;
  }

  
  nBase = 0;
  pBase = sqlite3_malloc(nRequestBytes);
  if( !pBase ){
    return SQLITE_NOMEM;
  }
  while( nBase<nRequestBytes ){
    
    unsigned nCopyBytes = nRequestBytes - nBase;
    if( nLocalRecordBytes-iRequestOffset<nCopyBytes ){
      nCopyBytes = nLocalRecordBytes - iRequestOffset;
    }
    memcpy(pBase + nBase, PageData(pPage, iRecordOffset + iRequestOffset),
           nCopyBytes);
    nBase += nCopyBytes;

    if( pOverflow ){
      
      iRequestOffset = 0;

      pPage = pOverflow->pPage;
      iRecordOffset = 4;
      nLocalRecordBytes = pOverflow->nPageSize - iRecordOffset;
      pOverflow = pOverflow->pNextOverflow;
    }else if( nBase<nRequestBytes ){
      assert(NULL);  
      sqlite3_free(pBase);
      return SQLITE_ERROR;
    }
  }
  assert( nBase==nRequestBytes );
  *ppBase = pBase;
  *pbFree = 1;
  return SQLITE_OK;
}

typedef struct RecoverLeafCursor RecoverLeafCursor;
struct RecoverLeafCursor {
  RecoverInteriorCursor *pParent;  
  DbPage *pPage;                   
  unsigned nPageSize;              
  unsigned nCells;                 
  unsigned iCell;                  

  
  i64 iRowid;                      
  unsigned nRecordCols;            
  u64 iRecordOffset;               
  u64 nRecordBytes;                
  unsigned nLocalRecordBytes;      
  unsigned nRecordHeaderBytes;     
  unsigned char *pRecordHeader;    
  int bFreeRecordHeader;           
  RecoverOverflow *pOverflow;      
};

static int leafCursorLoadPage(RecoverLeafCursor *pCursor, DbPage *pPage){
  const unsigned char *pPageHeader;  

  
  if( pCursor->pPage ){
    sqlite3PagerUnref(pCursor->pPage);
    pCursor->pPage = NULL;
    pCursor->iCell = pCursor->nCells = 0;
  }

  pPageHeader = PageHeader(pPage);
  if( pPageHeader[kiPageTypeOffset]==kTableInteriorPage ){
    RecoverInteriorCursor *pParent;
    int rc = interiorCursorCreate(pCursor->pParent, pPage, pCursor->nPageSize,
                                  &pParent);
    if( rc!=SQLITE_OK ){
      return rc;
    }
    pCursor->pParent = pParent;
    return SQLITE_OK;
  }

  
  if( pPageHeader[kiPageTypeOffset]!=kTableLeafPage ){
    sqlite3PagerUnref(pPage);
    return SQLITE_OK;
  }

  
  pCursor->pPage = pPage;
  pCursor->iCell = 0;
  pCursor->nCells = decodeUnsigned16(pPageHeader + kiPageCellCountOffset);
  return SQLITE_OK;
}

static int leafCursorNextPage(RecoverLeafCursor *pCursor){
  if( !pCursor->pParent ){
    return SQLITE_DONE;
  }

  
  do {
    DbPage *pNextPage;
    int rc = interiorCursorNextPage(&pCursor->pParent, &pNextPage);
    if( rc!=SQLITE_ROW ){
      assert( rc==SQLITE_DONE );
      return rc;
    }

    rc = leafCursorLoadPage(pCursor, pNextPage);
    if( rc!=SQLITE_OK ){
      sqlite3PagerUnref(pNextPage);
      return rc;
    }
  } while( !pCursor->pPage );

  return SQLITE_ROW;
}

static void leafCursorDestroyCellData(RecoverLeafCursor *pCursor){
  if( pCursor->bFreeRecordHeader ){
    sqlite3_free(pCursor->pRecordHeader);
  }
  pCursor->bFreeRecordHeader = 0;
  pCursor->pRecordHeader = NULL;

  if( pCursor->pOverflow ){
    overflowDestroy(pCursor->pOverflow);
    pCursor->pOverflow = NULL;
  }
}

static void leafCursorDestroy(RecoverLeafCursor *pCursor){
  leafCursorDestroyCellData(pCursor);

  if( pCursor->pParent ){
    interiorCursorDestroy(pCursor->pParent);
    pCursor->pParent = NULL;
  }

  if( pCursor->pPage ){
    sqlite3PagerUnref(pCursor->pPage);
    pCursor->pPage = NULL;
  }

  memset(pCursor, 0xA5, sizeof(*pCursor));
  sqlite3_free(pCursor);
}

static int leafCursorCreate(Pager *pPager, unsigned nPageSize,
                            u32 iRootPage, RecoverLeafCursor **ppCursor){
  DbPage *pPage;               
  RecoverLeafCursor *pCursor;  
  int rc;

  
  rc = sqlite3PagerAcquire(pPager, iRootPage, &pPage, 0);
  if( rc!=SQLITE_OK ){
    return rc;
  }

  pCursor = sqlite3_malloc(sizeof(RecoverLeafCursor));
  if( !pCursor ){
    sqlite3PagerUnref(pPage);
    return SQLITE_NOMEM;
  }
  memset(pCursor, 0, sizeof(*pCursor));

  pCursor->nPageSize = nPageSize;

  rc = leafCursorLoadPage(pCursor, pPage);
  if( rc!=SQLITE_OK ){
    sqlite3PagerUnref(pPage);
    leafCursorDestroy(pCursor);
    return rc;
  }

  
  if( !pCursor->pPage ){
    rc = leafCursorNextPage(pCursor);
    if( rc!=SQLITE_DONE && rc!=SQLITE_ROW ){
      leafCursorDestroy(pCursor);
      return rc;
    }
  }

  *ppCursor = pCursor;
  return SQLITE_OK;
}

static int ValidateError(){
  return SQLITE_ERROR;
}

static int leafCursorCellDecode(RecoverLeafCursor *pCursor){
  const unsigned char *pPageHeader;  
  const unsigned char *pCellOffsets; 
  unsigned iCellOffset;              
  const unsigned char *pCell;        
  unsigned nCellMaxBytes;            
  unsigned iEndOffset;               
  u64 nRecordBytes;                  
  u64 iRowid;                        
  unsigned nRead;                    
  unsigned nRecordHeaderRead;        
  u64 nRecordHeaderBytes;            
  unsigned nRecordCols;              
  u64 nRecordColBytes;               
  unsigned i;
  int rc;

  assert( pCursor->iCell<pCursor->nCells );

  leafCursorDestroyCellData(pCursor);

  
  pPageHeader = PageHeader(pCursor->pPage);
  pCellOffsets = pPageHeader + knPageLeafHeaderBytes;
  iCellOffset = decodeUnsigned16(pCellOffsets + pCursor->iCell*2);
  if( iCellOffset>=pCursor->nPageSize ){
    return ValidateError();
  }

  pCell = PageData(pCursor->pPage, iCellOffset);
  nCellMaxBytes = pCursor->nPageSize - iCellOffset;

  if( !checkVarints(pCell, nCellMaxBytes, 3) ){
    return ValidateError();
  }

  nRead = getVarint(pCell, &nRecordBytes);
  assert( iCellOffset+nRead<=pCursor->nPageSize );
  pCursor->nRecordBytes = nRecordBytes;

  nRead += getVarint(pCell + nRead, &iRowid);
  assert( iCellOffset+nRead<=pCursor->nPageSize );
  pCursor->iRowid = (i64)iRowid;

  pCursor->iRecordOffset = iCellOffset + nRead;

  rc = overflowMaybeCreate(pCursor->pPage, pCursor->nPageSize,
                           pCursor->iRecordOffset, pCursor->nRecordBytes,
                           &pCursor->nLocalRecordBytes,
                           &pCursor->pOverflow);
  if( rc!=SQLITE_OK ){
    return ValidateError();
  }

  
  iEndOffset = pCursor->iRecordOffset + pCursor->nLocalRecordBytes;
  for( i=0; i<pCursor->nCells; ++i ){
    const unsigned iOtherOffset = decodeUnsigned16(pCellOffsets + i*2);
    if( iOtherOffset>iCellOffset && iOtherOffset<iEndOffset ){
      return ValidateError();
    }
  }

  nRecordHeaderRead = getVarint(pCell + nRead, &nRecordHeaderBytes);
  assert( nRecordHeaderBytes<=nRecordBytes );
  pCursor->nRecordHeaderBytes = nRecordHeaderBytes;

  
  rc = overflowGetSegment(pCursor->pPage,
                          pCursor->iRecordOffset, pCursor->nLocalRecordBytes,
                          pCursor->pOverflow, 0, nRecordHeaderBytes,
                          &pCursor->pRecordHeader, &pCursor->bFreeRecordHeader);
  if( rc!=SQLITE_OK ){
    return ValidateError();
  }

  
  nRecordCols = 0;
  nRecordColBytes = 0;
  while( nRecordHeaderRead<nRecordHeaderBytes ){
    u64 iSerialType;  
    if( !checkVarint(pCursor->pRecordHeader + nRecordHeaderRead,
                     nRecordHeaderBytes - nRecordHeaderRead) ){
      return ValidateError();
    }
    nRecordHeaderRead += getVarint(pCursor->pRecordHeader + nRecordHeaderRead,
                                   &iSerialType);
    if( iSerialType==10 || iSerialType==11 ){
      return ValidateError();
    }
    nRecordColBytes += SerialTypeLength(iSerialType);
    nRecordCols++;
  }
  pCursor->nRecordCols = nRecordCols;

  
  if( nRecordHeaderRead!=nRecordHeaderBytes ){
    return ValidateError();
  }

  
  if( nRecordHeaderBytes+nRecordColBytes!=nRecordBytes ){
    return ValidateError();
  }

  return SQLITE_OK;
}

static i64 leafCursorCellRowid(RecoverLeafCursor *pCursor){
  return pCursor->iRowid;
}

static unsigned leafCursorCellColumns(RecoverLeafCursor *pCursor){
  return pCursor->nRecordCols;
}

static int leafCursorCellColInfo(RecoverLeafCursor *pCursor,
                                 unsigned iCol, u64 *piColType,
                                 unsigned char **ppBase, int *pbFree){
  const unsigned char *pRecordHeader;  
  u64 nRecordHeaderBytes;              
  unsigned nRead;                      
  u64 iColEndOffset;                   
  unsigned nColsSkipped;               
  u64 iSerialType;                     

  if( iCol>=pCursor->nRecordCols ){
    *piColType = 0;
    if( ppBase ){
      *ppBase = 0;
      *pbFree = 0;
    }
    return SQLITE_OK;
  }

  
  pRecordHeader = pCursor->pRecordHeader;
  if( !checkVarint(pRecordHeader, pCursor->nRecordHeaderBytes) ){
    return SQLITE_CORRUPT;
  }

  nRead = getVarint(pRecordHeader, &nRecordHeaderBytes);
  assert( nRecordHeaderBytes==pCursor->nRecordHeaderBytes );

  iColEndOffset = 0;
  nColsSkipped = 0;
  while( nColsSkipped<=iCol && nRead<nRecordHeaderBytes ){
    if( !checkVarint(pRecordHeader + nRead, nRecordHeaderBytes - nRead) ){
      return SQLITE_CORRUPT;
    }
    nRead += getVarint(pRecordHeader + nRead, &iSerialType);
    iColEndOffset += SerialTypeLength(iSerialType);
    nColsSkipped++;
  }

  
  if( nRecordHeaderBytes+iColEndOffset>pCursor->nRecordBytes ){
    return SQLITE_CORRUPT;
  }

  *piColType = iSerialType;
  if( ppBase ){
    const u32 nColBytes = SerialTypeLength(iSerialType);

    
    const unsigned iColOffset = nRecordHeaderBytes+iColEndOffset-nColBytes;

    return overflowGetSegment(pCursor->pPage, pCursor->iRecordOffset,
                              pCursor->nLocalRecordBytes, pCursor->pOverflow,
                              iColOffset, nColBytes, ppBase, pbFree);
  }
  return SQLITE_OK;
}

static int leafCursorNextValidCell(RecoverLeafCursor *pCursor){
  while( 1 ){
    int rc;

    
    pCursor->iCell++;

    
    if( pCursor->iCell>=pCursor->nCells ){
      rc = leafCursorNextPage(pCursor);
      if( rc!=SQLITE_ROW ){
        return rc;
      }
      assert( pCursor->iCell==0 );
    }

    
    rc = leafCursorCellDecode(pCursor);
    if( rc==SQLITE_OK ){
      return SQLITE_ROW;
    }

    
    
    fprintf(stderr, "Skipping invalid cell\n");
  }
  return SQLITE_ERROR;
}

typedef struct Recover Recover;
struct Recover {
  sqlite3_vtab base;
  sqlite3 *db;                
  char *zDb;                  
  char *zTable;               
  unsigned nCols;             
  unsigned char *pTypes;      
};

static void recoverRelease(Recover *pRecover){
  sqlite3_free(pRecover->zDb);
  sqlite3_free(pRecover->zTable);
  sqlite3_free(pRecover->pTypes);
  memset(pRecover, 0xA5, sizeof(*pRecover));
  sqlite3_free(pRecover);
}

static int recoverInit(
  sqlite3 *, void *, int, const char *const*, sqlite3_vtab **, char **
);

static int recoverCreate(
  sqlite3 *db,
  void *pAux,
  int argc, const char *const*argv,
  sqlite3_vtab **ppVtab,
  char **pzErr
){
  FNENTRY();
  return recoverInit(db, pAux, argc, argv, ppVtab, pzErr);
}

static int recoverConnect(
  sqlite3 *db,
  void *pAux,
  int argc, const char *const*argv,
  sqlite3_vtab **ppVtab,
  char **pzErr
){
  FNENTRY();
  return recoverInit(db, pAux, argc, argv, ppVtab, pzErr);
}

static int recoverBestIndex(sqlite3_vtab *tab, sqlite3_index_info *pIdxInfo){
  FNENTRY();
  return SQLITE_OK;
}

static int recoverDisconnect(sqlite3_vtab *pVtab){
  FNENTRY();
  recoverRelease((Recover*)pVtab);
  return SQLITE_OK;
}

static int recoverDestroy(sqlite3_vtab *pVtab){
  FNENTRY();
  recoverRelease((Recover*)pVtab);
  return SQLITE_OK;
}

typedef struct RecoverCursor RecoverCursor;
struct RecoverCursor {
  sqlite3_vtab_cursor base;
  RecoverLeafCursor *pLeafCursor;
  int iEncoding;
  int bEOF;
};

static int recoverOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor){
  Recover *pRecover = (Recover*)pVTab;
  u32 iRootPage;                   
  int iEncoding;                   
  unsigned nPageSize;              
  Pager *pPager;                   
  RecoverLeafCursor *pLeafCursor;  
  RecoverCursor *pCursor;          
  int rc;

  FNENTRY();

  iRootPage = 0;
  rc = getRootPage(pRecover->db, pRecover->zDb, pRecover->zTable,
                   &iRootPage);
  if( rc!=SQLITE_OK ){
    return rc;
  }

  iEncoding = 0;
  rc = getEncoding(pRecover->db, pRecover->zDb, &iEncoding);
  if( rc!=SQLITE_OK ){
    return rc;
  }

  rc = GetPager(pRecover->db, pRecover->zDb, &pPager, &nPageSize);
  if( rc!=SQLITE_OK ){
    return rc;
  }

  rc = leafCursorCreate(pPager, nPageSize, iRootPage, &pLeafCursor);
  if( rc!=SQLITE_OK ){
    return rc;
  }

  pCursor = sqlite3_malloc(sizeof(RecoverCursor));
  if( !pCursor ){
    leafCursorDestroy(pLeafCursor);
    return SQLITE_NOMEM;
  }
  memset(pCursor, 0, sizeof(*pCursor));
  pCursor->base.pVtab = pVTab;
  pCursor->pLeafCursor = pLeafCursor;
  pCursor->iEncoding = iEncoding;

  
  pCursor->bEOF = (pLeafCursor->pPage==NULL);

  *ppCursor = (sqlite3_vtab_cursor*)pCursor;
  return SQLITE_OK;
}

static int recoverClose(sqlite3_vtab_cursor *cur){
  RecoverCursor *pCursor = (RecoverCursor*)cur;
  FNENTRY();
  if( pCursor->pLeafCursor ){
    leafCursorDestroy(pCursor->pLeafCursor);
    pCursor->pLeafCursor = NULL;
  }
  memset(pCursor, 0xA5, sizeof(*pCursor));
  sqlite3_free(cur);
  return SQLITE_OK;
}

static int RecoverInvalidCell(){
  return SQLITE_ERROR;
}

static int recoverValidateLeafCell(Recover *pRecover, RecoverCursor *pCursor){
  unsigned i;

  
  if( leafCursorCellColumns(pCursor->pLeafCursor)>pRecover->nCols ){
    return RecoverInvalidCell();
  }

  
  for( i=0; i<pRecover->nCols; ++i ){
    u64 iType;  
    int rc;

    
    if( (pRecover->pTypes[i]&MASK_ROWID) ){
      continue;
    }

    rc = leafCursorCellColInfo(pCursor->pLeafCursor, i, &iType, NULL, NULL);
    assert( rc==SQLITE_OK );
    if( rc!=SQLITE_OK || !SerialTypeIsCompatible(iType, pRecover->pTypes[i]) ){
      return RecoverInvalidCell();
    }
  }

  return SQLITE_OK;
}

static int recoverNext(sqlite3_vtab_cursor *pVtabCursor){
  RecoverCursor *pCursor = (RecoverCursor*)pVtabCursor;
  Recover *pRecover = (Recover*)pCursor->base.pVtab;
  int rc;

  FNENTRY();

  while( (rc = leafCursorNextValidCell(pCursor->pLeafCursor))==SQLITE_ROW ){
    if( recoverValidateLeafCell(pRecover, pCursor)==SQLITE_OK ){
      return SQLITE_OK;
    }
  }

  if( rc==SQLITE_DONE ){
    pCursor->bEOF = 1;
    return SQLITE_OK;
  }

  assert( rc!=SQLITE_OK );
  return rc;
}

static int recoverFilter(
  sqlite3_vtab_cursor *pVtabCursor,
  int idxNum, const char *idxStr,
  int argc, sqlite3_value **argv
){
  RecoverCursor *pCursor = (RecoverCursor*)pVtabCursor;
  Recover *pRecover = (Recover*)pCursor->base.pVtab;
  int rc;

  FNENTRY();

  
  if( pCursor->bEOF ){
    return SQLITE_OK;
  }

  rc = leafCursorCellDecode(pCursor->pLeafCursor);
  if( rc!=SQLITE_OK || recoverValidateLeafCell(pRecover, pCursor)!=SQLITE_OK ){
    return recoverNext(pVtabCursor);
  }

  return SQLITE_OK;
}

static int recoverEof(sqlite3_vtab_cursor *pVtabCursor){
  RecoverCursor *pCursor = (RecoverCursor*)pVtabCursor;
  FNENTRY();
  return pCursor->bEOF;
}

static int recoverColumn(sqlite3_vtab_cursor *cur, sqlite3_context *ctx, int i){
  RecoverCursor *pCursor = (RecoverCursor*)cur;
  Recover *pRecover = (Recover*)pCursor->base.pVtab;
  u64 iColType;             
  unsigned char *pColData;  
  int shouldFree;           
  int rc;

  FNENTRY();

  if( i>=pRecover->nCols ){
    return SQLITE_ERROR;
  }

  
  if( (pRecover->pTypes[i]&MASK_ROWID) ){
    sqlite3_result_int64(ctx, leafCursorCellRowid(pCursor->pLeafCursor));
    return SQLITE_OK;
  }

  pColData = NULL;
  shouldFree = 0;
  rc = leafCursorCellColInfo(pCursor->pLeafCursor, i, &iColType,
                             &pColData, &shouldFree);
  if( rc!=SQLITE_OK ){
    return rc;
  }
  if( !SerialTypeIsCompatible(iColType, pRecover->pTypes[i]) ){
    if( shouldFree ){
      sqlite3_free(pColData);
    }
    return SQLITE_ERROR;
  }

  switch( iColType ){
    case 0 : sqlite3_result_null(ctx); break;
    case 1 : sqlite3_result_int64(ctx, decodeSigned(pColData, 1)); break;
    case 2 : sqlite3_result_int64(ctx, decodeSigned(pColData, 2)); break;
    case 3 : sqlite3_result_int64(ctx, decodeSigned(pColData, 3)); break;
    case 4 : sqlite3_result_int64(ctx, decodeSigned(pColData, 4)); break;
    case 5 : sqlite3_result_int64(ctx, decodeSigned(pColData, 6)); break;
    case 6 : sqlite3_result_int64(ctx, decodeSigned(pColData, 8)); break;
    case 7 : sqlite3_result_double(ctx, decodeFloat64(pColData)); break;
    case 8 : sqlite3_result_int(ctx, 0); break;
    case 9 : sqlite3_result_int(ctx, 1); break;
    case 10 : assert( iColType!=10 ); break;
    case 11 : assert( iColType!=11 ); break;

    default : {
      u32 l = SerialTypeLength(iColType);

      
      sqlite3_destructor_type pFn = SQLITE_TRANSIENT;
      if( shouldFree ){
        pFn = sqlite3_free;
        shouldFree = 0;
      }

      if( SerialTypeIsBlob(iColType) ){
        sqlite3_result_blob(ctx, pColData, l, pFn);
      }else{
        if( pCursor->iEncoding==SQLITE_UTF16LE ){
          sqlite3_result_text16le(ctx, (const void*)pColData, l, pFn);
        }else if( pCursor->iEncoding==SQLITE_UTF16BE ){
          sqlite3_result_text16be(ctx, (const void*)pColData, l, pFn);
        }else{
          sqlite3_result_text(ctx, (const char*)pColData, l, pFn);
        }
      }
    } break;
  }
  if( shouldFree ){
    sqlite3_free(pColData);
  }
  return SQLITE_OK;
}

static int recoverRowid(sqlite3_vtab_cursor *pVtabCursor, sqlite_int64 *pRowid){
  RecoverCursor *pCursor = (RecoverCursor*)pVtabCursor;
  FNENTRY();
  *pRowid = leafCursorCellRowid(pCursor->pLeafCursor);
  return SQLITE_OK;
}

static sqlite3_module recoverModule = {
  0,                         
  recoverCreate,             
  recoverConnect,            
  recoverBestIndex,          
  recoverDisconnect,         
  recoverDestroy,            
  recoverOpen,               
  recoverClose,              
  recoverFilter,             
  recoverNext,               
  recoverEof,                
  recoverColumn,             
  recoverRowid,              
  0,                         
  0,                         
  0,                         
  0,                         
  0,                         
  0,                         
  0,                         
};

int recoverVtableInit(sqlite3 *db){
  return sqlite3_create_module_v2(db, "recover", &recoverModule, NULL, 0);
}


static int findWord(const char *zText,
                    const char **pzWordStart, const char **pzWordEnd){
  int r;
  while( ascii_isspace(*zText) ){
    zText++;
  }
  *pzWordStart = zText;
  while( ascii_isalnum(*zText) || *zText=='_' ){
    zText++;
  }
  r = zText>*pzWordStart;  
  *pzWordEnd = zText;
  return r;
}

static int expectWord(const char *zText, const char *zWord,
                      const char **pzContinue){
  const char *zWordStart, *zWordEnd;
  if( findWord(zText, &zWordStart, &zWordEnd) &&
      ascii_strncasecmp(zWord, zWordStart, zWordEnd - zWordStart)==0 ){
    *pzContinue = zWordEnd;
    return 1;
  }
  return 0;
}

static int findNameAndType(const char *parameter,
                           const char **pzNameStart, const char **pzNameEnd,
                           const char **pzTypeStart, const char **pzTypeEnd,
                           unsigned char *pTypeMask){
  unsigned nNameLen;   
  const char *zEnd;    
  int bNotNull;        
  int bStrict;         
  const char *zDummy;  
  unsigned i;

  static struct {
    const char *zName;
    unsigned char strictMask;
    unsigned char otherMask;
    const char *zReplace;
  } kTypeInfo[] = {
    { "ANY",
      MASK_INTEGER | MASK_FLOAT | MASK_BLOB | MASK_TEXT | MASK_NULL,
      0, "",
    },
    { "ROWID",   MASK_INTEGER | MASK_ROWID,             0, "INTEGER", },
    { "INTEGER", MASK_INTEGER | MASK_NULL,              0, NULL, },
    { "FLOAT",   MASK_FLOAT | MASK_NULL,                MASK_INTEGER, NULL, },
    { "NUMERIC", MASK_INTEGER | MASK_FLOAT | MASK_NULL, MASK_TEXT, NULL, },
    { "TEXT",    MASK_TEXT | MASK_NULL,                 MASK_BLOB, NULL, },
    { "BLOB",    MASK_BLOB | MASK_NULL,                 0, NULL, },
  };

  if( !findWord(parameter, pzNameStart, pzNameEnd) ){
    return SQLITE_MISUSE;
  }

  
  if( !findWord(*pzNameEnd, pzTypeStart, pzTypeEnd) ){
    *pzTypeEnd = *pzTypeStart = "";
    *pTypeMask = MASK_INTEGER | MASK_FLOAT | MASK_BLOB | MASK_TEXT | MASK_NULL;
    return SQLITE_OK;
  }

  nNameLen = *pzTypeEnd - *pzTypeStart;
  for( i=0; i<ArraySize(kTypeInfo); ++i ){
    if( ascii_strncasecmp(kTypeInfo[i].zName, *pzTypeStart, nNameLen)==0 ){
      break;
    }
  }
  if( i==ArraySize(kTypeInfo) ){
    return SQLITE_MISUSE;
  }

  zEnd = *pzTypeEnd;
  bStrict = 0;
  if( expectWord(zEnd, "STRICT", &zEnd) ){
    if( kTypeInfo[i].zReplace && !kTypeInfo[i].zReplace[0] ){
      return SQLITE_MISUSE;
    }
    bStrict = 1;
  }

  bNotNull = 0;
  if( expectWord(zEnd, "NOT", &zEnd) ){
    if( expectWord(zEnd, "NULL", &zEnd) ){
      bNotNull = 1;
    }else{
      
      return SQLITE_MISUSE;
    }
  }

  
  if( findWord(zEnd, &zDummy, &zDummy) ){
    return SQLITE_MISUSE;
  }

  *pTypeMask = kTypeInfo[i].strictMask;
  if( !bStrict ){
    *pTypeMask |= kTypeInfo[i].otherMask;
  }
  if( bNotNull ){
    *pTypeMask &= ~MASK_NULL;
  }
  if( kTypeInfo[i].zReplace ){
    *pzTypeStart = kTypeInfo[i].zReplace;
    *pzTypeEnd = *pzTypeStart + strlen(*pzTypeStart);
  }
  return SQLITE_OK;
}

static int ParseColumnsAndGenerateCreate(unsigned nCols,
                                         const char *const *pCols,
                                         char **pzCreateSql,
                                         unsigned char *pTypes,
                                         char **pzErr){
  unsigned i;
  char *zCreateSql = sqlite3_mprintf("CREATE TABLE x(");
  if( !zCreateSql ){
    return SQLITE_NOMEM;
  }

  for( i=0; i<nCols; i++ ){
    const char *zSep = (i < nCols - 1 ? ", " : ")");
    const char *zNotNull = "";
    const char *zNameStart, *zNameEnd;
    const char *zTypeStart, *zTypeEnd;
    int rc = findNameAndType(pCols[i],
                             &zNameStart, &zNameEnd,
                             &zTypeStart, &zTypeEnd,
                             &pTypes[i]);
    if( rc!=SQLITE_OK ){
      *pzErr = sqlite3_mprintf("unable to parse column %d", i);
      sqlite3_free(zCreateSql);
      return rc;
    }

    if( !(pTypes[i]&MASK_NULL) ){
      zNotNull = " NOT NULL";
    }

    
    zCreateSql = sqlite3_mprintf("%z%.*s %.*s%s%s",
                                 zCreateSql,
                                 zNameEnd - zNameStart, zNameStart,
                                 zTypeEnd - zTypeStart, zTypeStart,
                                 zNotNull, zSep);
    if( !zCreateSql ){
      return SQLITE_NOMEM;
    }
  }

  *pzCreateSql = zCreateSql;
  return SQLITE_OK;
}

static int recoverInit(
  sqlite3 *db,                        
  void *pAux,                         
  int argc, const char *const*argv,   
  sqlite3_vtab **ppVtab,              
  char **pzErr                        
){
  const unsigned kTypeCol = 4;  
  Recover *pRecover;            
  char *zDot;                   
  u32 iRootPage;                
  char *zCreateSql;             
  int rc;

  
  if( ascii_strcasecmp(argv[1], "temp")!=0 ){
    *pzErr = sqlite3_mprintf("recover table must be in temp database");
    return SQLITE_MISUSE;
  }

  
  if( argc<=kTypeCol ){
    *pzErr = sqlite3_mprintf("no columns specified");
    return SQLITE_MISUSE;
  }

  pRecover = sqlite3_malloc(sizeof(Recover));
  if( !pRecover ){
    return SQLITE_NOMEM;
  }
  memset(pRecover, 0, sizeof(*pRecover));
  pRecover->base.pModule = &recoverModule;
  pRecover->db = db;

  
  zDot = strchr(argv[3], '.');
  if( !zDot ){
    pRecover->zDb = sqlite3_strdup(db->aDb[0].zName);
    pRecover->zTable = sqlite3_strdup(argv[3]);
  }else if( zDot>argv[3] && zDot[1]!='\0' ){
    pRecover->zDb = sqlite3_strndup(argv[3], zDot - argv[3]);
    pRecover->zTable = sqlite3_strdup(zDot + 1);
  }else{
    
    *pzErr = sqlite3_mprintf("ill-formed table specifier");
    recoverRelease(pRecover);
    return SQLITE_ERROR;
  }

  pRecover->nCols = argc - kTypeCol;
  pRecover->pTypes = sqlite3_malloc(pRecover->nCols);
  if( !pRecover->zDb || !pRecover->zTable || !pRecover->pTypes ){
    recoverRelease(pRecover);
    return SQLITE_NOMEM;
  }

  
  rc = getRootPage(pRecover->db, pRecover->zDb, pRecover->zTable, &iRootPage);
  if( rc!=SQLITE_OK ){
    *pzErr = sqlite3_mprintf("unable to find backing table");
    recoverRelease(pRecover);
    return rc;
  }

  
  rc = ParseColumnsAndGenerateCreate(pRecover->nCols, argv + kTypeCol,
                                     &zCreateSql, pRecover->pTypes, pzErr);
  if( rc!=SQLITE_OK ){
    recoverRelease(pRecover);
    return rc;
  }

  rc = sqlite3_declare_vtab(db, zCreateSql);
  sqlite3_free(zCreateSql);
  if( rc!=SQLITE_OK ){
    recoverRelease(pRecover);
    return rc;
  }

  *ppVtab = (sqlite3_vtab *)pRecover;
  return SQLITE_OK;
}

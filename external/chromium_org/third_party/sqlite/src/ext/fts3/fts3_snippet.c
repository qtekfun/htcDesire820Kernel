/*
** 2009 Oct 23
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
******************************************************************************
*/

#if !defined(SQLITE_CORE) || defined(SQLITE_ENABLE_FTS3)

#include "fts3Int.h"
#include <string.h>
#include <assert.h>

#define FTS3_MATCHINFO_NPHRASE   'p'        
#define FTS3_MATCHINFO_NCOL      'c'        
#define FTS3_MATCHINFO_NDOC      'n'        
#define FTS3_MATCHINFO_AVGLENGTH 'a'        
#define FTS3_MATCHINFO_LENGTH    'l'        
#define FTS3_MATCHINFO_LCS       's'        
#define FTS3_MATCHINFO_HITS      'x'        

#define FTS3_MATCHINFO_DEFAULT   "pcx"


typedef struct LoadDoclistCtx LoadDoclistCtx;
struct LoadDoclistCtx {
  Fts3Cursor *pCsr;               
  int nPhrase;                    
  int nToken;                     
};

typedef struct SnippetIter SnippetIter;
typedef struct SnippetPhrase SnippetPhrase;
typedef struct SnippetFragment SnippetFragment;

struct SnippetIter {
  Fts3Cursor *pCsr;               
  int iCol;                       
  int nSnippet;                   
  int nPhrase;                    
  SnippetPhrase *aPhrase;         
  int iCurrent;                   
};

struct SnippetPhrase {
  int nToken;                     
  char *pList;                    
  int iHead;                      
  char *pHead;                    
  int iTail;                      
  char *pTail;                    
};

struct SnippetFragment {
  int iCol;                       
  int iPos;                       
  u64 covered;                    
  u64 hlmask;                     
};

typedef struct MatchInfo MatchInfo;
struct MatchInfo {
  Fts3Cursor *pCursor;            
  int nCol;                       
  int nPhrase;                    
  sqlite3_int64 nDoc;             
  u32 *aMatchinfo;                
};



typedef struct StrBuffer StrBuffer;
struct StrBuffer {
  char *z;                        
  int n;                          
  int nAlloc;                     
};


static void fts3GetDeltaPosition(char **pp, int *piPos){
  int iVal;
  *pp += sqlite3Fts3GetVarint32(*pp, &iVal);
  *piPos += (iVal-2);
}

static int fts3ExprIterate2(
  Fts3Expr *pExpr,                
  int *piPhrase,                  
  int (*x)(Fts3Expr*,int,void*),  
  void *pCtx                      
){
  int rc;                         
  int eType = pExpr->eType;       

  if( eType!=FTSQUERY_PHRASE ){
    assert( pExpr->pLeft && pExpr->pRight );
    rc = fts3ExprIterate2(pExpr->pLeft, piPhrase, x, pCtx);
    if( rc==SQLITE_OK && eType!=FTSQUERY_NOT ){
      rc = fts3ExprIterate2(pExpr->pRight, piPhrase, x, pCtx);
    }
  }else{
    rc = x(pExpr, *piPhrase, pCtx);
    (*piPhrase)++;
  }
  return rc;
}

static int fts3ExprIterate(
  Fts3Expr *pExpr,                
  int (*x)(Fts3Expr*,int,void*),  
  void *pCtx                      
){
  int iPhrase = 0;                
  return fts3ExprIterate2(pExpr, &iPhrase, x, pCtx);
}

static int fts3ExprNearTrim(Fts3Expr *pExpr){
  int rc = SQLITE_OK;
  Fts3Expr *pParent = pExpr->pParent;

  assert( pExpr->eType==FTSQUERY_PHRASE );
  while( rc==SQLITE_OK
   && pParent 
   && pParent->eType==FTSQUERY_NEAR 
   && pParent->pRight==pExpr 
  ){
    int nNear = pParent->nNear;
    Fts3Expr *pLeft = pParent->pLeft;

    if( pLeft->eType!=FTSQUERY_PHRASE ){
      assert( pLeft->eType==FTSQUERY_NEAR );
      assert( pLeft->pRight->eType==FTSQUERY_PHRASE );
      pLeft = pLeft->pRight;
    }

    rc = sqlite3Fts3ExprNearTrim(pLeft, pExpr, nNear);

    pExpr = pLeft;
    pParent = pExpr->pParent;
  }

  return rc;
}

static int fts3ExprLoadDoclistsCb(Fts3Expr *pExpr, int iPhrase, void *ctx){
  int rc = SQLITE_OK;
  LoadDoclistCtx *p = (LoadDoclistCtx *)ctx;

  UNUSED_PARAMETER(iPhrase);

  p->nPhrase++;
  p->nToken += pExpr->pPhrase->nToken;

  if( pExpr->isLoaded==0 ){
    rc = sqlite3Fts3ExprLoadDoclist(p->pCsr, pExpr);
    pExpr->isLoaded = 1;
    if( rc==SQLITE_OK ){
      rc = fts3ExprNearTrim(pExpr);
    }
  }

  return rc;
}

static int fts3ExprLoadDoclists(
  Fts3Cursor *pCsr,               
  int *pnPhrase,                  
  int *pnToken                    
){
  int rc;                         
  LoadDoclistCtx sCtx = {0,0,0};  
  sCtx.pCsr = pCsr;
  rc = fts3ExprIterate(pCsr->pExpr, fts3ExprLoadDoclistsCb, (void *)&sCtx);
  if( pnPhrase ) *pnPhrase = sCtx.nPhrase;
  if( pnToken ) *pnToken = sCtx.nToken;
  return rc;
}

static int fts3ExprPhraseCountCb(Fts3Expr *pExpr, int iPhrase, void *ctx){
  (*(int *)ctx)++;
  UNUSED_PARAMETER(pExpr);
  UNUSED_PARAMETER(iPhrase);
  return SQLITE_OK;
}
static int fts3ExprPhraseCount(Fts3Expr *pExpr){
  int nPhrase = 0;
  (void)fts3ExprIterate(pExpr, fts3ExprPhraseCountCb, (void *)&nPhrase);
  return nPhrase;
}

static void fts3SnippetAdvance(char **ppIter, int *piIter, int iNext){
  char *pIter = *ppIter;
  if( pIter ){
    int iIter = *piIter;

    while( iIter<iNext ){
      if( 0==(*pIter & 0xFE) ){
        iIter = -1;
        pIter = 0;
        break;
      }
      fts3GetDeltaPosition(&pIter, &iIter);
    }

    *piIter = iIter;
    *ppIter = pIter;
  }
}

static int fts3SnippetNextCandidate(SnippetIter *pIter){
  int i;                          

  if( pIter->iCurrent<0 ){
    pIter->iCurrent = 0;

    for(i=0; i<pIter->nPhrase; i++){
      SnippetPhrase *pPhrase = &pIter->aPhrase[i];
      fts3SnippetAdvance(&pPhrase->pHead, &pPhrase->iHead, pIter->nSnippet);
    }
  }else{
    int iStart;
    int iEnd = 0x7FFFFFFF;

    for(i=0; i<pIter->nPhrase; i++){
      SnippetPhrase *pPhrase = &pIter->aPhrase[i];
      if( pPhrase->pHead && pPhrase->iHead<iEnd ){
        iEnd = pPhrase->iHead;
      }
    }
    if( iEnd==0x7FFFFFFF ){
      return 1;
    }

    pIter->iCurrent = iStart = iEnd - pIter->nSnippet + 1;
    for(i=0; i<pIter->nPhrase; i++){
      SnippetPhrase *pPhrase = &pIter->aPhrase[i];
      fts3SnippetAdvance(&pPhrase->pHead, &pPhrase->iHead, iEnd+1);
      fts3SnippetAdvance(&pPhrase->pTail, &pPhrase->iTail, iStart);
    }
  }

  return 0;
}

static void fts3SnippetDetails(
  SnippetIter *pIter,             
  u64 mCovered,                   
  int *piToken,                   
  int *piScore,                   
  u64 *pmCover,                   
  u64 *pmHighlight                
){
  int iStart = pIter->iCurrent;   
  int iScore = 0;                 
  int i;                          
  u64 mCover = 0;                 
  u64 mHighlight = 0;             

  for(i=0; i<pIter->nPhrase; i++){
    SnippetPhrase *pPhrase = &pIter->aPhrase[i];
    if( pPhrase->pTail ){
      char *pCsr = pPhrase->pTail;
      int iCsr = pPhrase->iTail;

      while( iCsr<(iStart+pIter->nSnippet) ){
        int j;
        u64 mPhrase = (u64)1 << i;
        u64 mPos = (u64)1 << (iCsr - iStart);
        assert( iCsr>=iStart );
        if( (mCover|mCovered)&mPhrase ){
          iScore++;
        }else{
          iScore += 1000;
        }
        mCover |= mPhrase;

        for(j=0; j<pPhrase->nToken; j++){
          mHighlight |= (mPos>>j);
        }

        if( 0==(*pCsr & 0x0FE) ) break;
        fts3GetDeltaPosition(&pCsr, &iCsr);
      }
    }
  }

  
  *piToken = iStart;
  *piScore = iScore;
  *pmCover = mCover;
  *pmHighlight = mHighlight;
}

static int fts3SnippetFindPositions(Fts3Expr *pExpr, int iPhrase, void *ctx){
  SnippetIter *p = (SnippetIter *)ctx;
  SnippetPhrase *pPhrase = &p->aPhrase[iPhrase];
  char *pCsr;

  pPhrase->nToken = pExpr->pPhrase->nToken;

  pCsr = sqlite3Fts3FindPositions(pExpr, p->pCsr->iPrevId, p->iCol);
  if( pCsr ){
    int iFirst = 0;
    pPhrase->pList = pCsr;
    fts3GetDeltaPosition(&pCsr, &iFirst);
    pPhrase->pHead = pCsr;
    pPhrase->pTail = pCsr;
    pPhrase->iHead = iFirst;
    pPhrase->iTail = iFirst;
  }else{
    assert( pPhrase->pList==0 && pPhrase->pHead==0 && pPhrase->pTail==0 );
  }

  return SQLITE_OK;
}

static int fts3BestSnippet(
  int nSnippet,                   
  Fts3Cursor *pCsr,               
  int iCol,                       
  u64 mCovered,                   
  u64 *pmSeen,                    
  SnippetFragment *pFragment,     
  int *piScore                    
){
  int rc;                         
  int nList;                      
  SnippetIter sIter;              
  int nByte;                      
  int iBestScore = -1;            
  int i;                          

  memset(&sIter, 0, sizeof(sIter));

  rc = fts3ExprLoadDoclists(pCsr, &nList, 0);
  if( rc!=SQLITE_OK ){
    return rc;
  }

  nByte = sizeof(SnippetPhrase) * nList;
  sIter.aPhrase = (SnippetPhrase *)sqlite3_malloc(nByte);
  if( !sIter.aPhrase ){
    return SQLITE_NOMEM;
  }
  memset(sIter.aPhrase, 0, nByte);

  sIter.pCsr = pCsr;
  sIter.iCol = iCol;
  sIter.nSnippet = nSnippet;
  sIter.nPhrase = nList;
  sIter.iCurrent = -1;
  (void)fts3ExprIterate(pCsr->pExpr, fts3SnippetFindPositions, (void *)&sIter);

  
  for(i=0; i<nList; i++){
    if( sIter.aPhrase[i].pHead ){
      *pmSeen |= (u64)1 << i;
    }
  }

  pFragment->iCol = iCol;
  while( !fts3SnippetNextCandidate(&sIter) ){
    int iPos;
    int iScore;
    u64 mCover;
    u64 mHighlight;
    fts3SnippetDetails(&sIter, mCovered, &iPos, &iScore, &mCover, &mHighlight);
    assert( iScore>=0 );
    if( iScore>iBestScore ){
      pFragment->iPos = iPos;
      pFragment->hlmask = mHighlight;
      pFragment->covered = mCover;
      iBestScore = iScore;
    }
  }

  sqlite3_free(sIter.aPhrase);
  *piScore = iBestScore;
  return SQLITE_OK;
}


static int fts3StringAppend(
  StrBuffer *pStr,                
  const char *zAppend,            
  int nAppend                     
){
  if( nAppend<0 ){
    nAppend = (int)strlen(zAppend);
  }

  if( pStr->n+nAppend+1>=pStr->nAlloc ){
    int nAlloc = pStr->nAlloc+nAppend+100;
    char *zNew = sqlite3_realloc(pStr->z, nAlloc);
    if( !zNew ){
      return SQLITE_NOMEM;
    }
    pStr->z = zNew;
    pStr->nAlloc = nAlloc;
  }

  
  memcpy(&pStr->z[pStr->n], zAppend, nAppend);
  pStr->n += nAppend;
  pStr->z[pStr->n] = '\0';

  return SQLITE_OK;
}

static int fts3SnippetShift(
  Fts3Table *pTab,                
  int nSnippet,                   
  const char *zDoc,               
  int nDoc,                       
  int *piPos,                     
  u64 *pHlmask                    
){
  u64 hlmask = *pHlmask;          

  if( hlmask ){
    int nLeft;                    
    int nRight;                   
    int nDesired;                 

    for(nLeft=0; !(hlmask & ((u64)1 << nLeft)); nLeft++);
    for(nRight=0; !(hlmask & ((u64)1 << (nSnippet-1-nRight))); nRight++);
    nDesired = (nLeft-nRight)/2;

    if( nDesired>0 ){
      int nShift;                 
      int iCurrent = 0;           
      int rc;                     
      sqlite3_tokenizer_module *pMod;
      sqlite3_tokenizer_cursor *pC;
      pMod = (sqlite3_tokenizer_module *)pTab->pTokenizer->pModule;

      rc = pMod->xOpen(pTab->pTokenizer, zDoc, nDoc, &pC);
      if( rc!=SQLITE_OK ){
        return rc;
      }
      pC->pTokenizer = pTab->pTokenizer;
      while( rc==SQLITE_OK && iCurrent<(nSnippet+nDesired) ){
        const char *ZDUMMY; int DUMMY1, DUMMY2, DUMMY3;
        rc = pMod->xNext(pC, &ZDUMMY, &DUMMY1, &DUMMY2, &DUMMY3, &iCurrent);
      }
      pMod->xClose(pC);
      if( rc!=SQLITE_OK && rc!=SQLITE_DONE ){ return rc; }

      nShift = (rc==SQLITE_DONE)+iCurrent-nSnippet;
      assert( nShift<=nDesired );
      if( nShift>0 ){
        *piPos += nShift;
        *pHlmask = hlmask >> nShift;
      }
    }
  }
  return SQLITE_OK;
}

static int fts3SnippetText(
  Fts3Cursor *pCsr,               
  SnippetFragment *pFragment,     
  int iFragment,                  
  int isLast,                     
  int nSnippet,                   
  const char *zOpen,              
  const char *zClose,             
  const char *zEllipsis,          
  StrBuffer *pOut                 
){
  Fts3Table *pTab = (Fts3Table *)pCsr->base.pVtab;
  int rc;                         
  const char *zDoc;               
  int nDoc;                       
  int iCurrent = 0;               
  int iEnd = 0;                   
  int isShiftDone = 0;            
  int iPos = pFragment->iPos;     
  u64 hlmask = pFragment->hlmask; 
  int iCol = pFragment->iCol+1;   
  sqlite3_tokenizer_module *pMod; 
  sqlite3_tokenizer_cursor *pC;   
  const char *ZDUMMY;             
  int DUMMY1;                     
  
  zDoc = (const char *)sqlite3_column_text(pCsr->pStmt, iCol);
  if( zDoc==0 ){
    if( sqlite3_column_type(pCsr->pStmt, iCol)!=SQLITE_NULL ){
      return SQLITE_NOMEM;
    }
    return SQLITE_OK;
  }
  nDoc = sqlite3_column_bytes(pCsr->pStmt, iCol);

  
  pMod = (sqlite3_tokenizer_module *)pTab->pTokenizer->pModule;
  rc = pMod->xOpen(pTab->pTokenizer, zDoc, nDoc, &pC);
  if( rc!=SQLITE_OK ){
    return rc;
  }
  pC->pTokenizer = pTab->pTokenizer;

  while( rc==SQLITE_OK ){
    int iBegin;                   
    int iFin;                     
    int isHighlight;              

    rc = pMod->xNext(pC, &ZDUMMY, &DUMMY1, &iBegin, &iFin, &iCurrent);
    if( rc!=SQLITE_OK ){
      if( rc==SQLITE_DONE ){
        rc = fts3StringAppend(pOut, &zDoc[iEnd], -1);
      }
      break;
    }
    if( iCurrent<iPos ){ continue; }

    if( !isShiftDone ){
      int n = nDoc - iBegin;
      rc = fts3SnippetShift(pTab, nSnippet, &zDoc[iBegin], n, &iPos, &hlmask);
      isShiftDone = 1;

      if( rc==SQLITE_OK && (iPos>0 || iFragment>0) ){
        rc = fts3StringAppend(pOut, zEllipsis, -1);
      }
      if( rc!=SQLITE_OK || iCurrent<iPos ) continue;
    }

    if( iCurrent>=(iPos+nSnippet) ){
      if( isLast ){
        rc = fts3StringAppend(pOut, zEllipsis, -1);
      }
      break;
    }

    
    isHighlight = (hlmask & ((u64)1 << (iCurrent-iPos)))!=0;

    if( iCurrent>iPos ) rc = fts3StringAppend(pOut, &zDoc[iEnd], iBegin-iEnd);
    if( rc==SQLITE_OK && isHighlight ) rc = fts3StringAppend(pOut, zOpen, -1);
    if( rc==SQLITE_OK ) rc = fts3StringAppend(pOut, &zDoc[iBegin], iFin-iBegin);
    if( rc==SQLITE_OK && isHighlight ) rc = fts3StringAppend(pOut, zClose, -1);

    iEnd = iFin;
  }

  pMod->xClose(pC);
  return rc;
}


static int fts3ColumnlistCount(char **ppCollist){
  char *pEnd = *ppCollist;
  char c = 0;
  int nEntry = 0;

  
  while( 0xFE & (*pEnd | c) ){
    c = *pEnd++ & 0x80;
    if( !c ) nEntry++;
  }

  *ppCollist = pEnd;
  return nEntry;
}

static void fts3LoadColumnlistCounts(char **pp, u32 *aOut, int isGlobal){
  char *pCsr = *pp;
  while( *pCsr ){
    int nHit;
    sqlite3_int64 iCol = 0;
    if( *pCsr==0x01 ){
      pCsr++;
      pCsr += sqlite3Fts3GetVarint(pCsr, &iCol);
    }
    nHit = fts3ColumnlistCount(&pCsr);
    assert( nHit>0 );
    if( isGlobal ){
      aOut[iCol*3+1]++;
    }
    aOut[iCol*3] += nHit;
  }
  pCsr++;
  *pp = pCsr;
}

static int fts3ExprGlobalHitsCb(
  Fts3Expr *pExpr,                
  int iPhrase,                    
  void *pCtx                      
){
  MatchInfo *p = (MatchInfo *)pCtx;
  Fts3Cursor *pCsr = p->pCursor;
  char *pIter;
  char *pEnd;
  char *pFree = 0;
  u32 *aOut = &p->aMatchinfo[3*iPhrase*p->nCol];

  assert( pExpr->isLoaded );
  assert( pExpr->eType==FTSQUERY_PHRASE );

  if( pCsr->pDeferred ){
    Fts3Phrase *pPhrase = pExpr->pPhrase;
    int ii;
    for(ii=0; ii<pPhrase->nToken; ii++){
      if( pPhrase->aToken[ii].bFulltext ) break;
    }
    if( ii<pPhrase->nToken ){
      int nFree = 0;
      int rc = sqlite3Fts3ExprLoadFtDoclist(pCsr, pExpr, &pFree, &nFree);
      if( rc!=SQLITE_OK ) return rc;
      pIter = pFree;
      pEnd = &pFree[nFree];
    }else{
      int iCol;                   
      for(iCol=0; iCol<p->nCol; iCol++){
        aOut[iCol*3 + 1] = (u32)p->nDoc;
        aOut[iCol*3 + 2] = (u32)p->nDoc;
      }
      return SQLITE_OK;
    }
  }else{
    pIter = pExpr->aDoclist;
    pEnd = &pExpr->aDoclist[pExpr->nDoclist];
  }

  
  while( pIter<pEnd ){
    while( *pIter++ & 0x80 );      
    fts3LoadColumnlistCounts(&pIter, &aOut[1], 1);
  }

  sqlite3_free(pFree);
  return SQLITE_OK;
}

static int fts3ExprLocalHitsCb(
  Fts3Expr *pExpr,                
  int iPhrase,                    
  void *pCtx                      
){
  MatchInfo *p = (MatchInfo *)pCtx;
  int iStart = iPhrase * p->nCol * 3;
  int i;

  for(i=0; i<p->nCol; i++) p->aMatchinfo[iStart+i*3] = 0;

  if( pExpr->aDoclist ){
    char *pCsr;

    pCsr = sqlite3Fts3FindPositions(pExpr, p->pCursor->iPrevId, -1);
    if( pCsr ){
      fts3LoadColumnlistCounts(&pCsr, &p->aMatchinfo[iStart], 0);
    }
  }

  return SQLITE_OK;
}

static int fts3MatchinfoCheck(
  Fts3Table *pTab, 
  char cArg,
  char **pzErr
){
  if( (cArg==FTS3_MATCHINFO_NPHRASE)
   || (cArg==FTS3_MATCHINFO_NCOL)
   || (cArg==FTS3_MATCHINFO_NDOC && pTab->bHasStat)
   || (cArg==FTS3_MATCHINFO_AVGLENGTH && pTab->bHasStat)
   || (cArg==FTS3_MATCHINFO_LENGTH && pTab->bHasDocsize)
   || (cArg==FTS3_MATCHINFO_LCS)
   || (cArg==FTS3_MATCHINFO_HITS)
  ){
    return SQLITE_OK;
  }
  *pzErr = sqlite3_mprintf("unrecognized matchinfo request: %c", cArg);
  return SQLITE_ERROR;
}

static int fts3MatchinfoSize(MatchInfo *pInfo, char cArg){
  int nVal;                       

  switch( cArg ){
    case FTS3_MATCHINFO_NDOC:
    case FTS3_MATCHINFO_NPHRASE: 
    case FTS3_MATCHINFO_NCOL: 
      nVal = 1;
      break;

    case FTS3_MATCHINFO_AVGLENGTH:
    case FTS3_MATCHINFO_LENGTH:
    case FTS3_MATCHINFO_LCS:
      nVal = pInfo->nCol;
      break;

    default:
      assert( cArg==FTS3_MATCHINFO_HITS );
      nVal = pInfo->nCol * pInfo->nPhrase * 3;
      break;
  }

  return nVal;
}

static int fts3MatchinfoSelectDoctotal(
  Fts3Table *pTab,
  sqlite3_stmt **ppStmt,
  sqlite3_int64 *pnDoc,
  const char **paLen
){
  sqlite3_stmt *pStmt;
  const char *a;
  sqlite3_int64 nDoc;

  if( !*ppStmt ){
    int rc = sqlite3Fts3SelectDoctotal(pTab, ppStmt);
    if( rc!=SQLITE_OK ) return rc;
  }
  pStmt = *ppStmt;
  assert( sqlite3_data_count(pStmt)==1 );

  a = sqlite3_column_blob(pStmt, 0);
  a += sqlite3Fts3GetVarint(a, &nDoc);
  if( nDoc==0 ) return SQLITE_CORRUPT;
  *pnDoc = (u32)nDoc;

  if( paLen ) *paLen = a;
  return SQLITE_OK;
}

typedef struct LcsIterator LcsIterator;
struct LcsIterator {
  Fts3Expr *pExpr;                
  char *pRead;                    
  int iPosOffset;                 
  int iCol;                       
  int iPos;                       
};

#define LCS_ITERATOR_FINISHED 0x7FFFFFFF;

static int fts3MatchinfoLcsCb(
  Fts3Expr *pExpr,                
  int iPhrase,                    
  void *pCtx                      
){
  LcsIterator *aIter = (LcsIterator *)pCtx;
  aIter[iPhrase].pExpr = pExpr;
  return SQLITE_OK;
}

static int fts3LcsIteratorAdvance(LcsIterator *pIter){
  char *pRead = pIter->pRead;
  sqlite3_int64 iRead;
  int rc = 0;

  pRead += sqlite3Fts3GetVarint(pRead, &iRead);
  if( iRead==0 ){
    pIter->iCol = LCS_ITERATOR_FINISHED;
    rc = 1;
  }else{
    if( iRead==1 ){
      pRead += sqlite3Fts3GetVarint(pRead, &iRead);
      pIter->iCol = (int)iRead;
      pIter->iPos = pIter->iPosOffset;
      pRead += sqlite3Fts3GetVarint(pRead, &iRead);
      rc = 1;
    }
    pIter->iPos += (int)(iRead-2);
  }

  pIter->pRead = pRead;
  return rc;
}
  
/*
** This function implements the FTS3_MATCHINFO_LCS matchinfo() flag. 
**
** If the call is successful, the longest-common-substring lengths for each
** column are written into the first nCol elements of the pInfo->aMatchinfo[] 
** array before returning. SQLITE_OK is returned in this case.
**
** Otherwise, if an error occurs, an SQLite error code is returned and the
** data written to the first nCol elements of pInfo->aMatchinfo[] is 
** undefined.
*/
static int fts3MatchinfoLcs(Fts3Cursor *pCsr, MatchInfo *pInfo){
  LcsIterator *aIter;
  int i;
  int iCol;
  int nToken = 0;

  aIter = sqlite3_malloc(sizeof(LcsIterator) * pCsr->nPhrase);
  if( !aIter ) return SQLITE_NOMEM;
  memset(aIter, 0, sizeof(LcsIterator) * pCsr->nPhrase);
  (void)fts3ExprIterate(pCsr->pExpr, fts3MatchinfoLcsCb, (void*)aIter);
  for(i=0; i<pInfo->nPhrase; i++){
    LcsIterator *pIter = &aIter[i];
    nToken -= pIter->pExpr->pPhrase->nToken;
    pIter->iPosOffset = nToken;
    pIter->pRead = sqlite3Fts3FindPositions(pIter->pExpr, pCsr->iPrevId, -1);
    if( pIter->pRead ){
      pIter->iPos = pIter->iPosOffset;
      fts3LcsIteratorAdvance(&aIter[i]);
    }else{
      pIter->iCol = LCS_ITERATOR_FINISHED;
    }
  }

  for(iCol=0; iCol<pInfo->nCol; iCol++){
    int nLcs = 0;                 
    int nLive = 0;                

    for(i=0; i<pInfo->nPhrase; i++){
      assert( aIter[i].iCol>=iCol );
      if( aIter[i].iCol==iCol ) nLive++;
    }

    while( nLive>0 ){
      LcsIterator *pAdv = 0;      
      int nThisLcs = 0;           

      for(i=0; i<pInfo->nPhrase; i++){
        LcsIterator *pIter = &aIter[i];
        if( iCol!=pIter->iCol ){  
          
          nThisLcs = 0;
        }else{
          if( pAdv==0 || pIter->iPos<pAdv->iPos ){
            pAdv = pIter;
          }
          if( nThisLcs==0 || pIter->iPos==pIter[-1].iPos ){
            nThisLcs++;
          }else{
            nThisLcs = 1;
          }
          if( nThisLcs>nLcs ) nLcs = nThisLcs;
        }
      }
      if( fts3LcsIteratorAdvance(pAdv) ) nLive--;
    }

    pInfo->aMatchinfo[iCol] = nLcs;
  }

  sqlite3_free(aIter);
  return SQLITE_OK;
}

static int fts3MatchinfoValues(
  Fts3Cursor *pCsr,               
  int bGlobal,                    
  MatchInfo *pInfo,               
  const char *zArg                
){
  int rc = SQLITE_OK;
  int i;
  Fts3Table *pTab = (Fts3Table *)pCsr->base.pVtab;
  sqlite3_stmt *pSelect = 0;

  for(i=0; rc==SQLITE_OK && zArg[i]; i++){

    switch( zArg[i] ){
      case FTS3_MATCHINFO_NPHRASE:
        if( bGlobal ) pInfo->aMatchinfo[0] = pInfo->nPhrase;
        break;

      case FTS3_MATCHINFO_NCOL:
        if( bGlobal ) pInfo->aMatchinfo[0] = pInfo->nCol;
        break;
        
      case FTS3_MATCHINFO_NDOC:
        if( bGlobal ){
          sqlite3_int64 nDoc;
          rc = fts3MatchinfoSelectDoctotal(pTab, &pSelect, &nDoc, 0);
          pInfo->aMatchinfo[0] = (u32)nDoc;
        }
        break;

      case FTS3_MATCHINFO_AVGLENGTH: 
        if( bGlobal ){
          sqlite3_int64 nDoc;     
          const char *a;          

          rc = fts3MatchinfoSelectDoctotal(pTab, &pSelect, &nDoc, &a);
          if( rc==SQLITE_OK ){
            int iCol;
            for(iCol=0; iCol<pInfo->nCol; iCol++){
              u32 iVal;
              sqlite3_int64 nToken;
              a += sqlite3Fts3GetVarint(a, &nToken);
              iVal = (u32)(((u32)(nToken&0xffffffff)+nDoc/2)/nDoc);
              pInfo->aMatchinfo[iCol] = iVal;
            }
          }
        }
        break;

      case FTS3_MATCHINFO_LENGTH: {
        sqlite3_stmt *pSelectDocsize = 0;
        rc = sqlite3Fts3SelectDocsize(pTab, pCsr->iPrevId, &pSelectDocsize);
        if( rc==SQLITE_OK ){
          int iCol;
          const char *a = sqlite3_column_blob(pSelectDocsize, 0);
          for(iCol=0; iCol<pInfo->nCol; iCol++){
            sqlite3_int64 nToken;
            a += sqlite3Fts3GetVarint(a, &nToken);
            pInfo->aMatchinfo[iCol] = (u32)nToken;
          }
        }
        sqlite3_reset(pSelectDocsize);
        break;
      }

      case FTS3_MATCHINFO_LCS:
        rc = fts3ExprLoadDoclists(pCsr, 0, 0);
        if( rc==SQLITE_OK ){
          rc = fts3MatchinfoLcs(pCsr, pInfo);
        }
        break;

      default: {
        Fts3Expr *pExpr;
        assert( zArg[i]==FTS3_MATCHINFO_HITS );
        pExpr = pCsr->pExpr;
        rc = fts3ExprLoadDoclists(pCsr, 0, 0);
        if( rc!=SQLITE_OK ) break;
        if( bGlobal ){
          if( pCsr->pDeferred ){
            rc = fts3MatchinfoSelectDoctotal(pTab, &pSelect, &pInfo->nDoc, 0);
            if( rc!=SQLITE_OK ) break;
          }
          rc = fts3ExprIterate(pExpr, fts3ExprGlobalHitsCb,(void*)pInfo);
          if( rc!=SQLITE_OK ) break;
        }
        (void)fts3ExprIterate(pExpr, fts3ExprLocalHitsCb,(void*)pInfo);
        break;
      }
    }

    pInfo->aMatchinfo += fts3MatchinfoSize(pInfo, zArg[i]);
  }

  sqlite3_reset(pSelect);
  return rc;
}


static int fts3GetMatchinfo(
  Fts3Cursor *pCsr,               
  const char *zArg                
){
  MatchInfo sInfo;
  Fts3Table *pTab = (Fts3Table *)pCsr->base.pVtab;
  int rc = SQLITE_OK;
  int bGlobal = 0;                

  memset(&sInfo, 0, sizeof(MatchInfo));
  sInfo.pCursor = pCsr;
  sInfo.nCol = pTab->nColumn;

  if( pCsr->zMatchinfo && strcmp(pCsr->zMatchinfo, zArg) ){
    assert( pCsr->aMatchinfo );
    sqlite3_free(pCsr->aMatchinfo);
    pCsr->zMatchinfo = 0;
    pCsr->aMatchinfo = 0;
  }

  if( pCsr->aMatchinfo==0 ){
    int nMatchinfo = 0;           
    int nArg;                     
    int i;                        

    
    pCsr->nPhrase = fts3ExprPhraseCount(pCsr->pExpr);
    sInfo.nPhrase = pCsr->nPhrase;

    
    for(i=0; zArg[i]; i++){
      nMatchinfo += fts3MatchinfoSize(&sInfo, zArg[i]);
    }

    
    nArg = (int)strlen(zArg);
    pCsr->aMatchinfo = (u32 *)sqlite3_malloc(sizeof(u32)*nMatchinfo + nArg + 1);
    if( !pCsr->aMatchinfo ) return SQLITE_NOMEM;

    pCsr->zMatchinfo = (char *)&pCsr->aMatchinfo[nMatchinfo];
    pCsr->nMatchinfo = nMatchinfo;
    memcpy(pCsr->zMatchinfo, zArg, nArg+1);
    memset(pCsr->aMatchinfo, 0, sizeof(u32)*nMatchinfo);
    pCsr->isMatchinfoNeeded = 1;
    bGlobal = 1;
  }

  sInfo.aMatchinfo = pCsr->aMatchinfo;
  sInfo.nPhrase = pCsr->nPhrase;
  if( pCsr->isMatchinfoNeeded ){
    rc = fts3MatchinfoValues(pCsr, bGlobal, &sInfo, zArg);
    pCsr->isMatchinfoNeeded = 0;
  }

  return rc;
}

void sqlite3Fts3Snippet(
  sqlite3_context *pCtx,          
  Fts3Cursor *pCsr,               
  const char *zStart,             
  const char *zEnd,               
  const char *zEllipsis,          
  int iCol,                       
  int nToken                      
){
  Fts3Table *pTab = (Fts3Table *)pCsr->base.pVtab;
  int rc = SQLITE_OK;
  int i;
  StrBuffer res = {0, 0, 0};

  int nSnippet = 0;               
  SnippetFragment aSnippet[4];    
  int nFToken = -1;               

  if( !pCsr->pExpr ){
    sqlite3_result_text(pCtx, "", 0, SQLITE_STATIC);
    return;
  }

  for(nSnippet=1; 1; nSnippet++){

    int iSnip;                    
    u64 mCovered = 0;             
    u64 mSeen = 0;                

    if( nToken>=0 ){
      nFToken = (nToken+nSnippet-1) / nSnippet;
    }else{
      nFToken = -1 * nToken;
    }

    for(iSnip=0; iSnip<nSnippet; iSnip++){
      int iBestScore = -1;        
      int iRead;                  
      SnippetFragment *pFragment = &aSnippet[iSnip];

      memset(pFragment, 0, sizeof(*pFragment));

      for(iRead=0; iRead<pTab->nColumn; iRead++){
        SnippetFragment sF = {0, 0, 0, 0};
        int iS;
        if( iCol>=0 && iRead!=iCol ) continue;

        
        rc = fts3BestSnippet(nFToken, pCsr, iRead, mCovered, &mSeen, &sF, &iS);
        if( rc!=SQLITE_OK ){
          goto snippet_out;
        }
        if( iS>iBestScore ){
          *pFragment = sF;
          iBestScore = iS;
        }
      }

      mCovered |= pFragment->covered;
    }

    assert( (mCovered&mSeen)==mCovered );
    if( mSeen==mCovered || nSnippet==SizeofArray(aSnippet) ) break;
  }

  assert( nFToken>0 );

  for(i=0; i<nSnippet && rc==SQLITE_OK; i++){
    rc = fts3SnippetText(pCsr, &aSnippet[i], 
        i, (i==nSnippet-1), nFToken, zStart, zEnd, zEllipsis, &res
    );
  }

 snippet_out:
  sqlite3Fts3SegmentsClose(pTab);
  if( rc!=SQLITE_OK ){
    sqlite3_result_error_code(pCtx, rc);
    sqlite3_free(res.z);
  }else{
    sqlite3_result_text(pCtx, res.z, -1, sqlite3_free);
  }
}


typedef struct TermOffset TermOffset;
typedef struct TermOffsetCtx TermOffsetCtx;

struct TermOffset {
  char *pList;                    
  int iPos;                       
  int iOff;                       
};

struct TermOffsetCtx {
  int iCol;                       
  int iTerm;
  sqlite3_int64 iDocid;
  TermOffset *aTerm;
};

static int fts3ExprTermOffsetInit(Fts3Expr *pExpr, int iPhrase, void *ctx){
  TermOffsetCtx *p = (TermOffsetCtx *)ctx;
  int nTerm;                      
  int iTerm;                      
  char *pList;                    
  int iPos = 0;                   

  UNUSED_PARAMETER(iPhrase);
  pList = sqlite3Fts3FindPositions(pExpr, p->iDocid, p->iCol);
  nTerm = pExpr->pPhrase->nToken;
  if( pList ){
    fts3GetDeltaPosition(&pList, &iPos);
    assert( iPos>=0 );
  }

  for(iTerm=0; iTerm<nTerm; iTerm++){
    TermOffset *pT = &p->aTerm[p->iTerm++];
    pT->iOff = nTerm-iTerm-1;
    pT->pList = pList;
    pT->iPos = iPos;
  }

  return SQLITE_OK;
}

void sqlite3Fts3Offsets(
  sqlite3_context *pCtx,          
  Fts3Cursor *pCsr                
){
  Fts3Table *pTab = (Fts3Table *)pCsr->base.pVtab;
  sqlite3_tokenizer_module const *pMod = pTab->pTokenizer->pModule;
  const char *ZDUMMY;             
  int NDUMMY;                     
  int rc;                         
  int nToken;                     
  int iCol;                       
  StrBuffer res = {0, 0, 0};      
  TermOffsetCtx sCtx;             

  if( !pCsr->pExpr ){
    sqlite3_result_text(pCtx, "", 0, SQLITE_STATIC);
    return;
  }

  memset(&sCtx, 0, sizeof(sCtx));
  assert( pCsr->isRequireSeek==0 );

  
  rc = fts3ExprLoadDoclists(pCsr, 0, &nToken);
  if( rc!=SQLITE_OK ) goto offsets_out;

  
  sCtx.aTerm = (TermOffset *)sqlite3_malloc(sizeof(TermOffset)*nToken);
  if( 0==sCtx.aTerm ){
    rc = SQLITE_NOMEM;
    goto offsets_out;
  }
  sCtx.iDocid = pCsr->iPrevId;

  for(iCol=0; iCol<pTab->nColumn; iCol++){
    sqlite3_tokenizer_cursor *pC; 
    int iStart;
    int iEnd;
    int iCurrent;
    const char *zDoc;
    int nDoc;

    sCtx.iCol = iCol;
    sCtx.iTerm = 0;
    (void)fts3ExprIterate(pCsr->pExpr, fts3ExprTermOffsetInit, (void *)&sCtx);

    zDoc = (const char *)sqlite3_column_text(pCsr->pStmt, iCol+1);
    nDoc = sqlite3_column_bytes(pCsr->pStmt, iCol+1);
    if( zDoc==0 ){
      if( sqlite3_column_type(pCsr->pStmt, iCol+1)==SQLITE_NULL ){
        continue;
      }
      rc = SQLITE_NOMEM;
      goto offsets_out;
    }

    
    rc = pMod->xOpen(pTab->pTokenizer, zDoc, nDoc, &pC);
    if( rc!=SQLITE_OK ) goto offsets_out;
    pC->pTokenizer = pTab->pTokenizer;

    rc = pMod->xNext(pC, &ZDUMMY, &NDUMMY, &iStart, &iEnd, &iCurrent);
    while( rc==SQLITE_OK ){
      int i;                      
      int iMinPos = 0x7FFFFFFF;   
      TermOffset *pTerm = 0;      

      for(i=0; i<nToken; i++){
        TermOffset *pT = &sCtx.aTerm[i];
        if( pT->pList && (pT->iPos-pT->iOff)<iMinPos ){
          iMinPos = pT->iPos-pT->iOff;
          pTerm = pT;
        }
      }

      if( !pTerm ){
        
        break;
      }else{
        assert( iCurrent<=iMinPos );
        if( 0==(0xFE&*pTerm->pList) ){
          pTerm->pList = 0;
        }else{
          fts3GetDeltaPosition(&pTerm->pList, &pTerm->iPos);
        }
        while( rc==SQLITE_OK && iCurrent<iMinPos ){
          rc = pMod->xNext(pC, &ZDUMMY, &NDUMMY, &iStart, &iEnd, &iCurrent);
        }
        if( rc==SQLITE_OK ){
          char aBuffer[64];
          sqlite3_snprintf(sizeof(aBuffer), aBuffer, 
              "%d %d %d %d ", iCol, pTerm-sCtx.aTerm, iStart, iEnd-iStart
          );
          rc = fts3StringAppend(&res, aBuffer, -1);
        }else if( rc==SQLITE_DONE ){
          rc = SQLITE_CORRUPT;
        }
      }
    }
    if( rc==SQLITE_DONE ){
      rc = SQLITE_OK;
    }

    pMod->xClose(pC);
    if( rc!=SQLITE_OK ) goto offsets_out;
  }

 offsets_out:
  sqlite3_free(sCtx.aTerm);
  assert( rc!=SQLITE_DONE );
  sqlite3Fts3SegmentsClose(pTab);
  if( rc!=SQLITE_OK ){
    sqlite3_result_error_code(pCtx,  rc);
    sqlite3_free(res.z);
  }else{
    sqlite3_result_text(pCtx, res.z, res.n-1, sqlite3_free);
  }
  return;
}

void sqlite3Fts3Matchinfo(
  sqlite3_context *pContext,      
  Fts3Cursor *pCsr,               
  const char *zArg                
){
  Fts3Table *pTab = (Fts3Table *)pCsr->base.pVtab;
  int rc;
  int i;
  const char *zFormat;

  if( zArg ){
    for(i=0; zArg[i]; i++){
      char *zErr = 0;
      if( fts3MatchinfoCheck(pTab, zArg[i], &zErr) ){
        sqlite3_result_error(pContext, zErr, -1);
        sqlite3_free(zErr);
        return;
      }
    }
    zFormat = zArg;
  }else{
    zFormat = FTS3_MATCHINFO_DEFAULT;
  }

  if( !pCsr->pExpr ){
    sqlite3_result_blob(pContext, "", 0, SQLITE_STATIC);
    return;
  }

  
  rc = fts3GetMatchinfo(pCsr, zFormat);
  sqlite3Fts3SegmentsClose(pTab);

  if( rc!=SQLITE_OK ){
    sqlite3_result_error_code(pContext, rc);
  }else{
    int n = pCsr->nMatchinfo * sizeof(u32);
    sqlite3_result_blob(pContext, pCsr->aMatchinfo, n, SQLITE_TRANSIENT);
  }
}

#endif

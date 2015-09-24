/*
** 2011 March 24
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
** Code for demonstartion virtual table that generates variations
** on an input word at increasing edit distances from the original.
**
** A fuzzer virtual table is created like this:
**
**     CREATE VIRTUAL TABLE temp.f USING fuzzer;
**
** The name of the new virtual table in the example above is "f".
** Note that all fuzzer virtual tables must be TEMP tables.  The
** "temp." prefix in front of the table name is required when the
** table is being created.  The "temp." prefix can be omitted when
** using the table as long as the name is unambiguous.
**
** Before being used, the fuzzer needs to be programmed by giving it
** character transformations and a cost associated with each transformation.
** Examples:
**
**    INSERT INTO f(cFrom,cTo,Cost) VALUES('','a',100);
**
** The above statement says that the cost of inserting a letter 'a' is
** 100.  (All costs are integers.  We recommend that costs be scaled so
** that the average cost is around 100.)
**
**    INSERT INTO f(cFrom,cTo,Cost) VALUES('b','',87);
**
** The above statement says that the cost of deleting a single letter
** 'b' is 87.
**
**    INSERT INTO f(cFrom,cTo,Cost) VALUES('o','oe',38);
**    INSERT INTO f(cFrom,cTo,Cost) VALUES('oe','o',40);
**
** This third example says that the cost of transforming the single
** letter "o" into the two-letter sequence "oe" is 38 and that the
** cost of transforming "oe" back into "o" is 40.
**
** After all the transformation costs have been set, the fuzzer table
** can be queried as follows:
**
**    SELECT word, distance FROM f
**     WHERE word MATCH 'abcdefg'
**       AND distance<200;
**
** This first query outputs the string "abcdefg" and all strings that
** can be derived from that string by appling the specified transformations.
** The strings are output together with their total transformation cost
** (called "distance") and appear in order of increasing cost.  No string
** is output more than once.  If there are multiple ways to transform the
** target string into the output string then the lowest cost transform is
** the one that is returned.  In the example, the search is limited to 
** strings with a total distance of less than 200.
**
** It is important to put some kind of a limit on the fuzzer output.  This
** can be either in the form of a LIMIT clause at the end of the query,
** or better, a "distance<NNN" constraint where NNN is some number.  The
** running time and memory requirement is exponential in the value of NNN 
** so you want to make sure that NNN is not too big.  A value of NNN that
** is about twice the average transformation cost seems to give good results.
**
** The fuzzer table can be useful for tasks such as spelling correction.
** Suppose there is a second table vocabulary(w) where the w column contains
** all correctly spelled words.   Let $word be a word you want to look up.
**
**   SELECT vocabulary.w FROM f, vocabulary
**    WHERE f.word MATCH $word
**      AND f.distance<=200
**      AND f.word=vocabulary.w
**    LIMIT 20
**
** The query above gives the 20 closest words to the $word being tested.
** (Note that for good performance, the vocubulary.w column should be
** indexed.)
**
** A similar query can be used to find all words in the dictionary that
** begin with some prefix $prefix:
**
**   SELECT vocabulary.w FROM f, vocabulary
**    WHERE f.word MATCH $prefix
**      AND f.distance<=200
**      AND vocabulary.w BETWEEN f.word AND (f.word || x'F7BFBFBF')
**    LIMIT 50
**
** This last query will show up to 50 words out of the vocabulary that
** match or nearly match the $prefix.
*/
#include "sqlite3.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#ifndef SQLITE_OMIT_VIRTUALTABLE

typedef struct fuzzer_vtab fuzzer_vtab;
typedef struct fuzzer_cursor fuzzer_cursor;
typedef struct fuzzer_rule fuzzer_rule;
typedef struct fuzzer_seen fuzzer_seen;
typedef struct fuzzer_stem fuzzer_stem;

typedef int fuzzer_cost;


struct fuzzer_rule {
  fuzzer_rule *pNext;        
  fuzzer_cost rCost;         
  int nFrom, nTo;            
  char *zFrom;               
  char zTo[4];               
};

struct fuzzer_stem {
  char *zBasis;              
  int nBasis;                
  const fuzzer_rule *pRule;  
  int n;                     
  fuzzer_cost rBaseCost;     
  fuzzer_cost rCostX;        
  fuzzer_stem *pNext;        
  fuzzer_stem *pHash;        
};

struct fuzzer_vtab {
  sqlite3_vtab base;         
  char *zClassName;          
  fuzzer_rule *pRule;        
  fuzzer_rule *pNewRule;     
  int nCursor;               
};

#define FUZZER_HASH  4001    
#define FUZZER_NQUEUE  20    

struct fuzzer_cursor {
  sqlite3_vtab_cursor base;  
  sqlite3_int64 iRowid;      
  fuzzer_vtab *pVtab;        
  fuzzer_cost rLimit;        
  fuzzer_stem *pStem;        
  fuzzer_stem *pDone;        
  fuzzer_stem *aQueue[FUZZER_NQUEUE];  
  int mxQueue;               
  char *zBuf;                
  int nBuf;                  
  int nStem;                 
  fuzzer_rule nullRule;      
  fuzzer_stem *apHash[FUZZER_HASH]; 
};

static int fuzzerConnect(
  sqlite3 *db,
  void *pAux,
  int argc, const char *const*argv,
  sqlite3_vtab **ppVtab,
  char **pzErr
){
  fuzzer_vtab *pNew;
  int n;
  if( strcmp(argv[1],"temp")!=0 ){
    *pzErr = sqlite3_mprintf("%s virtual tables must be TEMP", argv[0]);
    return SQLITE_ERROR;
  }
  n = strlen(argv[0]) + 1;
  pNew = sqlite3_malloc( sizeof(*pNew) + n );
  if( pNew==0 ) return SQLITE_NOMEM;
  pNew->zClassName = (char*)&pNew[1];
  memcpy(pNew->zClassName, argv[0], n);
  sqlite3_declare_vtab(db, "CREATE TABLE x(word,distance,cFrom,cTo,cost)");
  memset(pNew, 0, sizeof(*pNew));
  *ppVtab = &pNew->base;
  return SQLITE_OK;
}

static int fuzzerDisconnect(sqlite3_vtab *pVtab){
  fuzzer_vtab *p = (fuzzer_vtab*)pVtab;
  assert( p->nCursor==0 );
  do{
    while( p->pRule ){
      fuzzer_rule *pRule = p->pRule;
      p->pRule = pRule->pNext;
      sqlite3_free(pRule);
    }
    p->pRule = p->pNewRule;
    p->pNewRule = 0;
  }while( p->pRule );
  sqlite3_free(p);
  return SQLITE_OK;
}

static fuzzer_rule *fuzzerMergeRules(fuzzer_rule *pA, fuzzer_rule *pB){
  fuzzer_rule head;
  fuzzer_rule *pTail;

  pTail =  &head;
  while( pA && pB ){
    if( pA->rCost<=pB->rCost ){
      pTail->pNext = pA;
      pTail = pA;
      pA = pA->pNext;
    }else{
      pTail->pNext = pB;
      pTail = pB;
      pB = pB->pNext;
    }
  }
  if( pA==0 ){
    pTail->pNext = pB;
  }else{
    pTail->pNext = pA;
  }
  return head.pNext;
}


static int fuzzerOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor){
  fuzzer_vtab *p = (fuzzer_vtab*)pVTab;
  fuzzer_cursor *pCur;
  pCur = sqlite3_malloc( sizeof(*pCur) );
  if( pCur==0 ) return SQLITE_NOMEM;
  memset(pCur, 0, sizeof(*pCur));
  pCur->pVtab = p;
  *ppCursor = &pCur->base;
  if( p->nCursor==0 && p->pNewRule ){
    unsigned int i;
    fuzzer_rule *pX;
    fuzzer_rule *a[15];
    for(i=0; i<sizeof(a)/sizeof(a[0]); i++) a[i] = 0;
    while( (pX = p->pNewRule)!=0 ){
      p->pNewRule = pX->pNext;
      pX->pNext = 0;
      for(i=0; a[i] && i<sizeof(a)/sizeof(a[0])-1; i++){
        pX = fuzzerMergeRules(a[i], pX);
        a[i] = 0;
      }
      a[i] = fuzzerMergeRules(a[i], pX);
    }
    for(pX=a[0], i=1; i<sizeof(a)/sizeof(a[0]); i++){
      pX = fuzzerMergeRules(a[i], pX);
    }
    p->pRule = fuzzerMergeRules(p->pRule, pX);
  }
  p->nCursor++;
  return SQLITE_OK;
}

static void fuzzerClearStemList(fuzzer_stem *pStem){
  while( pStem ){
    fuzzer_stem *pNext = pStem->pNext;
    sqlite3_free(pStem);
    pStem = pNext;
  }
}

static void fuzzerClearCursor(fuzzer_cursor *pCur, int clearHash){
  int i;
  fuzzerClearStemList(pCur->pStem);
  fuzzerClearStemList(pCur->pDone);
  for(i=0; i<FUZZER_NQUEUE; i++) fuzzerClearStemList(pCur->aQueue[i]);
  pCur->rLimit = (fuzzer_cost)0;
  if( clearHash && pCur->nStem ){
    pCur->mxQueue = 0;
    pCur->pStem = 0;
    pCur->pDone = 0;
    memset(pCur->aQueue, 0, sizeof(pCur->aQueue));
    memset(pCur->apHash, 0, sizeof(pCur->apHash));
  }
  pCur->nStem = 0;
}

static int fuzzerClose(sqlite3_vtab_cursor *cur){
  fuzzer_cursor *pCur = (fuzzer_cursor *)cur;
  fuzzerClearCursor(pCur, 0);
  sqlite3_free(pCur->zBuf);
  pCur->pVtab->nCursor--;
  sqlite3_free(pCur);
  return SQLITE_OK;
}

static int fuzzerRender(
  fuzzer_stem *pStem,   
  char **pzBuf,         
  int *pnBuf            
){
  const fuzzer_rule *pRule = pStem->pRule;
  int n;
  char *z;

  n = pStem->nBasis + pRule->nTo - pRule->nFrom;
  if( (*pnBuf)<n+1 ){
    (*pzBuf) = sqlite3_realloc((*pzBuf), n+100);
    if( (*pzBuf)==0 ) return SQLITE_NOMEM;
    (*pnBuf) = n+100;
  }
  n = pStem->n;
  z = *pzBuf;
  if( n<0 ){
    memcpy(z, pStem->zBasis, pStem->nBasis+1);
  }else{
    memcpy(z, pStem->zBasis, n);
    memcpy(&z[n], pRule->zTo, pRule->nTo);
    memcpy(&z[n+pRule->nTo], &pStem->zBasis[n+pRule->nFrom], 
           pStem->nBasis-n-pRule->nFrom+1);
  }
  return SQLITE_OK;
}

static unsigned int fuzzerHash(const char *z){
  unsigned int h = 0;
  while( *z ){ h = (h<<3) ^ (h>>29) ^ *(z++); }
  return h % FUZZER_HASH;
}

static fuzzer_cost fuzzerCost(fuzzer_stem *pStem){
  return pStem->rCostX = pStem->rBaseCost + pStem->pRule->rCost;
}

#if 0
static void fuzzerStemPrint(
  const char *zPrefix,
  fuzzer_stem *pStem,
  const char *zSuffix
){
  if( pStem->n<0 ){
    fprintf(stderr, "%s[%s](%d)-->self%s",
       zPrefix,
       pStem->zBasis, pStem->rBaseCost,
       zSuffix
    );
  }else{
    char *zBuf = 0;
    int nBuf = 0;
    if( fuzzerRender(pStem, &zBuf, &nBuf)!=SQLITE_OK ) return;
    fprintf(stderr, "%s[%s](%d)-->{%s}(%d)%s",
      zPrefix,
      pStem->zBasis, pStem->rBaseCost, zBuf, pStem->,
      zSuffix
    );
    sqlite3_free(zBuf);
  }
}
#endif

static int fuzzerSeen(fuzzer_cursor *pCur, fuzzer_stem *pStem){
  unsigned int h;
  fuzzer_stem *pLookup;

  if( fuzzerRender(pStem, &pCur->zBuf, &pCur->nBuf)==SQLITE_NOMEM ){
    return -1;
  }
  h = fuzzerHash(pCur->zBuf);
  pLookup = pCur->apHash[h];
    while( pLookup && strcmp(pLookup->zBasis, pCur->zBuf)!=0 ){
    pLookup = pLookup->pHash;
  }
  return pLookup!=0;
}

static int fuzzerAdvance(fuzzer_cursor *pCur, fuzzer_stem *pStem){
  const fuzzer_rule *pRule;
  while( (pRule = pStem->pRule)!=0 ){
    while( pStem->n < pStem->nBasis - pRule->nFrom ){
      pStem->n++;
      if( pRule->nFrom==0
       || memcmp(&pStem->zBasis[pStem->n], pRule->zFrom, pRule->nFrom)==0
      ){
        
        int rc = fuzzerSeen(pCur, pStem);
        if( rc<0 ) return -1;
        if( rc==0 ){
          fuzzerCost(pStem);
          return 1;
        }
      }
    }
    pStem->n = -1;
    pStem->pRule = pRule->pNext;
    if( pStem->pRule && fuzzerCost(pStem)>pCur->rLimit ) pStem->pRule = 0;
  }
  return 0;
}

static fuzzer_stem *fuzzerMergeStems(fuzzer_stem *pA, fuzzer_stem *pB){
  fuzzer_stem head;
  fuzzer_stem *pTail;

  pTail =  &head;
  while( pA && pB ){
    if( pA->rCostX<=pB->rCostX ){
      pTail->pNext = pA;
      pTail = pA;
      pA = pA->pNext;
    }else{
      pTail->pNext = pB;
      pTail = pB;
      pB = pB->pNext;
    }
  }
  if( pA==0 ){
    pTail->pNext = pB;
  }else{
    pTail->pNext = pA;
  }
  return head.pNext;
}

static fuzzer_stem *fuzzerLowestCostStem(fuzzer_cursor *pCur){
  fuzzer_stem *pBest, *pX;
  int iBest;
  int i;

  if( pCur->pStem==0 ){
    iBest = -1;
    pBest = 0;
    for(i=0; i<=pCur->mxQueue; i++){
      pX = pCur->aQueue[i];
      if( pX==0 ) continue;
      if( pBest==0 || pBest->rCostX>pX->rCostX ){
        pBest = pX;
        iBest = i;
      }
    } 
    if( pBest ){
      pCur->aQueue[iBest] = pBest->pNext;
      pBest->pNext = 0;
      pCur->pStem = pBest;
    }
  }
  return pCur->pStem;
}

static fuzzer_stem *fuzzerInsert(fuzzer_cursor *pCur, fuzzer_stem *pNew){
  fuzzer_stem *pX;
  int i;

  if( (pX = pCur->pStem)!=0 && pX->rCostX>pNew->rCostX ){
    pNew->pNext = 0;
    pCur->pStem = pNew;
    pNew = pX;
  }

  
  pNew->pNext = 0;
  pX = pNew;
  for(i=0; i<=pCur->mxQueue; i++){
    if( pCur->aQueue[i] ){
      pX = fuzzerMergeStems(pX, pCur->aQueue[i]);
      pCur->aQueue[i] = 0;
    }else{
      pCur->aQueue[i] = pX;
      break;
    }
  }
  if( i>pCur->mxQueue ){
    if( i<FUZZER_NQUEUE ){
      pCur->mxQueue = i;
      pCur->aQueue[i] = pX;
    }else{
      assert( pCur->mxQueue==FUZZER_NQUEUE-1 );
      pX = fuzzerMergeStems(pX, pCur->aQueue[FUZZER_NQUEUE-1]);
      pCur->aQueue[FUZZER_NQUEUE-1] = pX;
    }
  }

  return fuzzerLowestCostStem(pCur);
}

static fuzzer_stem *fuzzerNewStem(
  fuzzer_cursor *pCur,
  const char *zWord,
  fuzzer_cost rBaseCost
){
  fuzzer_stem *pNew;
  unsigned int h;

  pNew = sqlite3_malloc( sizeof(*pNew) + strlen(zWord) + 1 );
  if( pNew==0 ) return 0;
  memset(pNew, 0, sizeof(*pNew));
  pNew->zBasis = (char*)&pNew[1];
  pNew->nBasis = strlen(zWord);
  memcpy(pNew->zBasis, zWord, pNew->nBasis+1);
  pNew->pRule = pCur->pVtab->pRule;
  pNew->n = -1;
  pNew->rBaseCost = pNew->rCostX = rBaseCost;
  h = fuzzerHash(pNew->zBasis);
  pNew->pHash = pCur->apHash[h];
  pCur->apHash[h] = pNew;
  pCur->nStem++;
  return pNew;
}


static int fuzzerNext(sqlite3_vtab_cursor *cur){
  fuzzer_cursor *pCur = (fuzzer_cursor*)cur;
  int rc;
  fuzzer_stem *pStem, *pNew;

  pCur->iRowid++;

  pStem = pCur->pStem;
  if( pStem->rCostX>0 ){
    rc = fuzzerRender(pStem, &pCur->zBuf, &pCur->nBuf);
    if( rc==SQLITE_NOMEM ) return SQLITE_NOMEM;
    pNew = fuzzerNewStem(pCur, pCur->zBuf, pStem->rCostX);
    if( pNew ){
      if( fuzzerAdvance(pCur, pNew)==0 ){
        pNew->pNext = pCur->pDone;
        pCur->pDone = pNew;
      }else{
        if( fuzzerInsert(pCur, pNew)==pNew ){
          return SQLITE_OK;
        }
      }
    }else{
      return SQLITE_NOMEM;
    }
  }

  while( (pStem = pCur->pStem)!=0 ){
    if( fuzzerAdvance(pCur, pStem) ){
      pCur->pStem = 0;
      pStem = fuzzerInsert(pCur, pStem);
      if( (rc = fuzzerSeen(pCur, pStem))!=0 ){
        if( rc<0 ) return SQLITE_NOMEM;
        continue;
      }
      return SQLITE_OK;  
    }
    pCur->pStem = 0;
    pStem->pNext = pCur->pDone;
    pCur->pDone = pStem;
    if( fuzzerLowestCostStem(pCur) ){
      rc = fuzzerSeen(pCur, pCur->pStem);
      if( rc<0 ) return SQLITE_NOMEM;
      if( rc==0 ){
        return SQLITE_OK;
      }
    }
  }

  pCur->rLimit = (fuzzer_cost)0;
  return SQLITE_OK;
}

static int fuzzerFilter(
  sqlite3_vtab_cursor *pVtabCursor, 
  int idxNum, const char *idxStr,
  int argc, sqlite3_value **argv
){
  fuzzer_cursor *pCur = (fuzzer_cursor *)pVtabCursor;
  const char *zWord = 0;
  fuzzer_stem *pStem;

  fuzzerClearCursor(pCur, 1);
  pCur->rLimit = 2147483647;
  if( idxNum==1 ){
    zWord = (const char*)sqlite3_value_text(argv[0]);
  }else if( idxNum==2 ){
    pCur->rLimit = (fuzzer_cost)sqlite3_value_int(argv[0]);
  }else if( idxNum==3 ){
    zWord = (const char*)sqlite3_value_text(argv[0]);
    pCur->rLimit = (fuzzer_cost)sqlite3_value_int(argv[1]);
  }
  if( zWord==0 ) zWord = "";
  pCur->pStem = pStem = fuzzerNewStem(pCur, zWord, (fuzzer_cost)0);
  if( pStem==0 ) return SQLITE_NOMEM;
  pCur->nullRule.pNext = pCur->pVtab->pRule;
  pCur->nullRule.rCost = 0;
  pCur->nullRule.nFrom = 0;
  pCur->nullRule.nTo = 0;
  pCur->nullRule.zFrom = "";
  pStem->pRule = &pCur->nullRule;
  pStem->n = pStem->nBasis;
  pCur->iRowid = 1;
  return SQLITE_OK;
}

static int fuzzerColumn(sqlite3_vtab_cursor *cur, sqlite3_context *ctx, int i){
  fuzzer_cursor *pCur = (fuzzer_cursor*)cur;
  if( i==0 ){
    
    if( fuzzerRender(pCur->pStem, &pCur->zBuf, &pCur->nBuf)==SQLITE_NOMEM ){
      return SQLITE_NOMEM;
    }
    sqlite3_result_text(ctx, pCur->zBuf, -1, SQLITE_TRANSIENT);
  }else if( i==1 ){
    
    sqlite3_result_int(ctx, pCur->pStem->rCostX);
  }else{
    
    sqlite3_result_null(ctx);
  }
  return SQLITE_OK;
}

static int fuzzerRowid(sqlite3_vtab_cursor *cur, sqlite_int64 *pRowid){
  fuzzer_cursor *pCur = (fuzzer_cursor*)cur;
  *pRowid = pCur->iRowid;
  return SQLITE_OK;
}

static int fuzzerEof(sqlite3_vtab_cursor *cur){
  fuzzer_cursor *pCur = (fuzzer_cursor*)cur;
  return pCur->rLimit<=(fuzzer_cost)0;
}

static int fuzzerBestIndex(sqlite3_vtab *tab, sqlite3_index_info *pIdxInfo){
  int iPlan = 0;
  int iDistTerm = -1;
  int i;
  const struct sqlite3_index_constraint *pConstraint;
  pConstraint = pIdxInfo->aConstraint;
  for(i=0; i<pIdxInfo->nConstraint; i++, pConstraint++){
    if( pConstraint->usable==0 ) continue;
    if( (iPlan & 1)==0 
     && pConstraint->iColumn==0
     && pConstraint->op==SQLITE_INDEX_CONSTRAINT_MATCH
    ){
      iPlan |= 1;
      pIdxInfo->aConstraintUsage[i].argvIndex = 1;
      pIdxInfo->aConstraintUsage[i].omit = 1;
    }
    if( (iPlan & 2)==0
     && pConstraint->iColumn==1
     && (pConstraint->op==SQLITE_INDEX_CONSTRAINT_LT
           || pConstraint->op==SQLITE_INDEX_CONSTRAINT_LE)
    ){
      iPlan |= 2;
      iDistTerm = i;
    }
  }
  if( iPlan==2 ){
    pIdxInfo->aConstraintUsage[iDistTerm].argvIndex = 1;
  }else if( iPlan==3 ){
    pIdxInfo->aConstraintUsage[iDistTerm].argvIndex = 2;
  }
  pIdxInfo->idxNum = iPlan;
  if( pIdxInfo->nOrderBy==1
   && pIdxInfo->aOrderBy[0].iColumn==1
   && pIdxInfo->aOrderBy[0].desc==0
  ){
    pIdxInfo->orderByConsumed = 1;
  }
  pIdxInfo->estimatedCost = (double)10000;
   
  return SQLITE_OK;
}

static int fuzzerUpdate(
  sqlite3_vtab *pVTab,
  int argc,
  sqlite3_value **argv,
  sqlite_int64 *pRowid
){
  fuzzer_vtab *p = (fuzzer_vtab*)pVTab;
  fuzzer_rule *pRule;
  const char *zFrom;
  int nFrom;
  const char *zTo;
  int nTo;
  fuzzer_cost rCost;
  if( argc!=7 ){
    sqlite3_free(pVTab->zErrMsg);
    pVTab->zErrMsg = sqlite3_mprintf("cannot delete from a %s virtual table",
                                     p->zClassName);
    return SQLITE_CONSTRAINT;
  }
  if( sqlite3_value_type(argv[0])!=SQLITE_NULL ){
    sqlite3_free(pVTab->zErrMsg);
    pVTab->zErrMsg = sqlite3_mprintf("cannot update a %s virtual table",
                                     p->zClassName);
    return SQLITE_CONSTRAINT;
  }
  zFrom = (char*)sqlite3_value_text(argv[4]);
  if( zFrom==0 ) zFrom = "";
  zTo = (char*)sqlite3_value_text(argv[5]);
  if( zTo==0 ) zTo = "";
  if( strcmp(zFrom,zTo)==0 ){
    
    return SQLITE_OK;
  }
  rCost = sqlite3_value_int(argv[6]);
  if( rCost<=0 ){
    sqlite3_free(pVTab->zErrMsg);
    pVTab->zErrMsg = sqlite3_mprintf("cost must be positive");
    return SQLITE_CONSTRAINT;    
  }
  nFrom = strlen(zFrom);
  nTo = strlen(zTo);
  pRule = sqlite3_malloc( sizeof(*pRule) + nFrom + nTo );
  if( pRule==0 ){
    return SQLITE_NOMEM;
  }
  pRule->zFrom = &pRule->zTo[nTo+1];
  pRule->nFrom = nFrom;
  memcpy(pRule->zFrom, zFrom, nFrom+1);
  memcpy(pRule->zTo, zTo, nTo+1);
  pRule->nTo = nTo;
  pRule->rCost = rCost;
  pRule->pNext = p->pNewRule;
  p->pNewRule = pRule;
  return SQLITE_OK;
}

static sqlite3_module fuzzerModule = {
  0,                           
  fuzzerConnect,
  fuzzerConnect,
  fuzzerBestIndex,
  fuzzerDisconnect, 
  fuzzerDisconnect,
  fuzzerOpen,                  
  fuzzerClose,                 
  fuzzerFilter,                
  fuzzerNext,                  
  fuzzerEof,                   
  fuzzerColumn,                
  fuzzerRowid,                 
  fuzzerUpdate,                
  0,                           
  0,                           
  0,                           
  0,                           
  0,                           
  0,                           
};

#endif 


int fuzzer_register(sqlite3 *db){
  int rc = SQLITE_OK;
#ifndef SQLITE_OMIT_VIRTUALTABLE
  rc = sqlite3_create_module(db, "fuzzer", &fuzzerModule, 0);
#endif
  return rc;
}

#ifdef SQLITE_TEST
#include <tcl.h>
extern int getDbPointer(Tcl_Interp *interp, const char *zA, sqlite3 **ppDb);

static int register_fuzzer_module(
  ClientData clientData, 
  Tcl_Interp *interp,    
  int objc,              
  Tcl_Obj *CONST objv[]  
){
  sqlite3 *db;
  if( objc!=2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "DB");
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;
  fuzzer_register(db);
  return TCL_OK;
}


int Sqlitetestfuzzer_Init(Tcl_Interp *interp){
  static struct {
     char *zName;
     Tcl_ObjCmdProc *xProc;
     void *clientData;
  } aObjCmd[] = {
     { "register_fuzzer_module",   register_fuzzer_module, 0 },
  };
  int i;
  for(i=0; i<sizeof(aObjCmd)/sizeof(aObjCmd[0]); i++){
    Tcl_CreateObjCommand(interp, aObjCmd[i].zName, 
        aObjCmd[i].xProc, aObjCmd[i].clientData, 0);
  }
  return TCL_OK;
}

#endif 

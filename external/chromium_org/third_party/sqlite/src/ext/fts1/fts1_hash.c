/*
** 2001 September 22
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This is the implementation of generic hash-tables used in SQLite.
** We've modified it slightly to serve as a standalone hash table
** implementation for the full-text indexing module.
*/
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#if !defined(SQLITE_CORE) || defined(SQLITE_ENABLE_FTS1)


#include "fts1_hash.h"

static void *malloc_and_zero(int n){
  void *p = malloc(n);
  if( p ){
    memset(p, 0, n);
  }
  return p;
}

void sqlite3Fts1HashInit(fts1Hash *pNew, int keyClass, int copyKey){
  assert( pNew!=0 );
  assert( keyClass>=FTS1_HASH_STRING && keyClass<=FTS1_HASH_BINARY );
  pNew->keyClass = keyClass;
  pNew->copyKey = copyKey;
  pNew->first = 0;
  pNew->count = 0;
  pNew->htsize = 0;
  pNew->ht = 0;
  pNew->xMalloc = malloc_and_zero;
  pNew->xFree = free;
}

void sqlite3Fts1HashClear(fts1Hash *pH){
  fts1HashElem *elem;         

  assert( pH!=0 );
  elem = pH->first;
  pH->first = 0;
  if( pH->ht ) pH->xFree(pH->ht);
  pH->ht = 0;
  pH->htsize = 0;
  while( elem ){
    fts1HashElem *next_elem = elem->next;
    if( pH->copyKey && elem->pKey ){
      pH->xFree(elem->pKey);
    }
    pH->xFree(elem);
    elem = next_elem;
  }
  pH->count = 0;
}

static int strHash(const void *pKey, int nKey){
  const char *z = (const char *)pKey;
  int h = 0;
  if( nKey<=0 ) nKey = (int) strlen(z);
  while( nKey > 0  ){
    h = (h<<3) ^ h ^ *z++;
    nKey--;
  }
  return h & 0x7fffffff;
}
static int strCompare(const void *pKey1, int n1, const void *pKey2, int n2){
  if( n1!=n2 ) return 1;
  return strncmp((const char*)pKey1,(const char*)pKey2,n1);
}

static int binHash(const void *pKey, int nKey){
  int h = 0;
  const char *z = (const char *)pKey;
  while( nKey-- > 0 ){
    h = (h<<3) ^ h ^ *(z++);
  }
  return h & 0x7fffffff;
}
static int binCompare(const void *pKey1, int n1, const void *pKey2, int n2){
  if( n1!=n2 ) return 1;
  return memcmp(pKey1,pKey2,n1);
}

static int (*hashFunction(int keyClass))(const void*,int){
  if( keyClass==FTS1_HASH_STRING ){
    return &strHash;
  }else{
    assert( keyClass==FTS1_HASH_BINARY );
    return &binHash;
  }
}

static int (*compareFunction(int keyClass))(const void*,int,const void*,int){
  if( keyClass==FTS1_HASH_STRING ){
    return &strCompare;
  }else{
    assert( keyClass==FTS1_HASH_BINARY );
    return &binCompare;
  }
}

static void insertElement(
  fts1Hash *pH,            
  struct _fts1ht *pEntry,  
  fts1HashElem *pNew       
){
  fts1HashElem *pHead;     
  pHead = pEntry->chain;
  if( pHead ){
    pNew->next = pHead;
    pNew->prev = pHead->prev;
    if( pHead->prev ){ pHead->prev->next = pNew; }
    else             { pH->first = pNew; }
    pHead->prev = pNew;
  }else{
    pNew->next = pH->first;
    if( pH->first ){ pH->first->prev = pNew; }
    pNew->prev = 0;
    pH->first = pNew;
  }
  pEntry->count++;
  pEntry->chain = pNew;
}


static void rehash(fts1Hash *pH, int new_size){
  struct _fts1ht *new_ht;          
  fts1HashElem *elem, *next_elem;  
  int (*xHash)(const void*,int);   

  assert( (new_size & (new_size-1))==0 );
  new_ht = (struct _fts1ht *)pH->xMalloc( new_size*sizeof(struct _fts1ht) );
  if( new_ht==0 ) return;
  if( pH->ht ) pH->xFree(pH->ht);
  pH->ht = new_ht;
  pH->htsize = new_size;
  xHash = hashFunction(pH->keyClass);
  for(elem=pH->first, pH->first=0; elem; elem = next_elem){
    int h = (*xHash)(elem->pKey, elem->nKey) & (new_size-1);
    next_elem = elem->next;
    insertElement(pH, &new_ht[h], elem);
  }
}

static fts1HashElem *findElementGivenHash(
  const fts1Hash *pH, 
  const void *pKey,   
  int nKey,
  int h               
){
  fts1HashElem *elem;            
  int count;                     
  int (*xCompare)(const void*,int,const void*,int);  

  if( pH->ht ){
    struct _fts1ht *pEntry = &pH->ht[h];
    elem = pEntry->chain;
    count = pEntry->count;
    xCompare = compareFunction(pH->keyClass);
    while( count-- && elem ){
      if( (*xCompare)(elem->pKey,elem->nKey,pKey,nKey)==0 ){ 
        return elem;
      }
      elem = elem->next;
    }
  }
  return 0;
}

static void removeElementGivenHash(
  fts1Hash *pH,         
  fts1HashElem* elem,   
  int h                 
){
  struct _fts1ht *pEntry;
  if( elem->prev ){
    elem->prev->next = elem->next; 
  }else{
    pH->first = elem->next;
  }
  if( elem->next ){
    elem->next->prev = elem->prev;
  }
  pEntry = &pH->ht[h];
  if( pEntry->chain==elem ){
    pEntry->chain = elem->next;
  }
  pEntry->count--;
  if( pEntry->count<=0 ){
    pEntry->chain = 0;
  }
  if( pH->copyKey && elem->pKey ){
    pH->xFree(elem->pKey);
  }
  pH->xFree( elem );
  pH->count--;
  if( pH->count<=0 ){
    assert( pH->first==0 );
    assert( pH->count==0 );
    fts1HashClear(pH);
  }
}

void *sqlite3Fts1HashFind(const fts1Hash *pH, const void *pKey, int nKey){
  int h;                 
  fts1HashElem *elem;    
  int (*xHash)(const void*,int);  

  if( pH==0 || pH->ht==0 ) return 0;
  xHash = hashFunction(pH->keyClass);
  assert( xHash!=0 );
  h = (*xHash)(pKey,nKey);
  assert( (pH->htsize & (pH->htsize-1))==0 );
  elem = findElementGivenHash(pH,pKey,nKey, h & (pH->htsize-1));
  return elem ? elem->data : 0;
}

void *sqlite3Fts1HashInsert(
  fts1Hash *pH,        
  const void *pKey,    
  int nKey,            
  void *data           
){
  int hraw;                 
  int h;                    
  fts1HashElem *elem;       
  fts1HashElem *new_elem;   
  int (*xHash)(const void*,int);  

  assert( pH!=0 );
  xHash = hashFunction(pH->keyClass);
  assert( xHash!=0 );
  hraw = (*xHash)(pKey, nKey);
  assert( (pH->htsize & (pH->htsize-1))==0 );
  h = hraw & (pH->htsize-1);
  elem = findElementGivenHash(pH,pKey,nKey,h);
  if( elem ){
    void *old_data = elem->data;
    if( data==0 ){
      removeElementGivenHash(pH,elem,h);
    }else{
      elem->data = data;
    }
    return old_data;
  }
  if( data==0 ) return 0;
  new_elem = (fts1HashElem*)pH->xMalloc( sizeof(fts1HashElem) );
  if( new_elem==0 ) return data;
  if( pH->copyKey && pKey!=0 ){
    new_elem->pKey = pH->xMalloc( nKey );
    if( new_elem->pKey==0 ){
      pH->xFree(new_elem);
      return data;
    }
    memcpy((void*)new_elem->pKey, pKey, nKey);
  }else{
    new_elem->pKey = (void*)pKey;
  }
  new_elem->nKey = nKey;
  pH->count++;
  if( pH->htsize==0 ){
    rehash(pH,8);
    if( pH->htsize==0 ){
      pH->count = 0;
      pH->xFree(new_elem);
      return data;
    }
  }
  if( pH->count > pH->htsize ){
    rehash(pH,pH->htsize*2);
  }
  assert( pH->htsize>0 );
  assert( (pH->htsize & (pH->htsize-1))==0 );
  h = hraw & (pH->htsize-1);
  insertElement(pH, &pH->ht[h], new_elem);
  new_elem->data = data;
  return 0;
}

#endif 

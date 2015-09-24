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
** This is the implementation of generic hash-tables
** used in SQLite.
*/
#include "sqliteInt.h"
#include <assert.h>

void sqlite3HashInit(Hash *pNew){
  assert( pNew!=0 );
  pNew->first = 0;
  pNew->count = 0;
  pNew->htsize = 0;
  pNew->ht = 0;
}

void sqlite3HashClear(Hash *pH){
  HashElem *elem;         

  assert( pH!=0 );
  elem = pH->first;
  pH->first = 0;
  sqlite3_free(pH->ht);
  pH->ht = 0;
  pH->htsize = 0;
  while( elem ){
    HashElem *next_elem = elem->next;
    sqlite3_free(elem);
    elem = next_elem;
  }
  pH->count = 0;
}

static unsigned int strHash(const char *z, int nKey){
  int h = 0;
  assert( nKey>=0 );
  while( nKey > 0  ){
    h = (h<<3) ^ h ^ sqlite3UpperToLower[(unsigned char)*z++];
    nKey--;
  }
  return h;
}


static void insertElement(
  Hash *pH,              
  struct _ht *pEntry,    
  HashElem *pNew         
){
  HashElem *pHead;       
  if( pEntry ){
    pHead = pEntry->count ? pEntry->chain : 0;
    pEntry->count++;
    pEntry->chain = pNew;
  }else{
    pHead = 0;
  }
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
}


static int rehash(Hash *pH, unsigned int new_size){
  struct _ht *new_ht;            
  HashElem *elem, *next_elem;    

#if SQLITE_MALLOC_SOFT_LIMIT>0
  if( new_size*sizeof(struct _ht)>SQLITE_MALLOC_SOFT_LIMIT ){
    new_size = SQLITE_MALLOC_SOFT_LIMIT/sizeof(struct _ht);
  }
  if( new_size==pH->htsize ) return 0;
#endif

  sqlite3BeginBenignMalloc();
  new_ht = (struct _ht *)sqlite3Malloc( new_size*sizeof(struct _ht) );
  sqlite3EndBenignMalloc();

  if( new_ht==0 ) return 0;
  sqlite3_free(pH->ht);
  pH->ht = new_ht;
  pH->htsize = new_size = sqlite3MallocSize(new_ht)/sizeof(struct _ht);
  memset(new_ht, 0, new_size*sizeof(struct _ht));
  for(elem=pH->first, pH->first=0; elem; elem = next_elem){
    unsigned int h = strHash(elem->pKey, elem->nKey) % new_size;
    next_elem = elem->next;
    insertElement(pH, &new_ht[h], elem);
  }
  return 1;
}

static HashElem *findElementGivenHash(
  const Hash *pH,     
  const char *pKey,   
  int nKey,           
  unsigned int h      
){
  HashElem *elem;                
  int count;                     

  if( pH->ht ){
    struct _ht *pEntry = &pH->ht[h];
    elem = pEntry->chain;
    count = pEntry->count;
  }else{
    elem = pH->first;
    count = pH->count;
  }
  while( count-- && ALWAYS(elem) ){
    if( elem->nKey==nKey && sqlite3StrNICmp(elem->pKey,pKey,nKey)==0 ){ 
      return elem;
    }
    elem = elem->next;
  }
  return 0;
}

static void removeElementGivenHash(
  Hash *pH,         
  HashElem* elem,   
  unsigned int h    
){
  struct _ht *pEntry;
  if( elem->prev ){
    elem->prev->next = elem->next; 
  }else{
    pH->first = elem->next;
  }
  if( elem->next ){
    elem->next->prev = elem->prev;
  }
  if( pH->ht ){
    pEntry = &pH->ht[h];
    if( pEntry->chain==elem ){
      pEntry->chain = elem->next;
    }
    pEntry->count--;
    assert( pEntry->count>=0 );
  }
  sqlite3_free( elem );
  pH->count--;
  if( pH->count<=0 ){
    assert( pH->first==0 );
    assert( pH->count==0 );
    sqlite3HashClear(pH);
  }
}

void *sqlite3HashFind(const Hash *pH, const char *pKey, int nKey){
  HashElem *elem;    
  unsigned int h;    

  assert( pH!=0 );
  assert( pKey!=0 );
  assert( nKey>=0 );
  if( pH->ht ){
    h = strHash(pKey, nKey) % pH->htsize;
  }else{
    h = 0;
  }
  elem = findElementGivenHash(pH, pKey, nKey, h);
  return elem ? elem->data : 0;
}

void *sqlite3HashInsert(Hash *pH, const char *pKey, int nKey, void *data){
  unsigned int h;       
  HashElem *elem;       
  HashElem *new_elem;   

  assert( pH!=0 );
  assert( pKey!=0 );
  assert( nKey>=0 );
  if( pH->htsize ){
    h = strHash(pKey, nKey) % pH->htsize;
  }else{
    h = 0;
  }
  elem = findElementGivenHash(pH,pKey,nKey,h);
  if( elem ){
    void *old_data = elem->data;
    if( data==0 ){
      removeElementGivenHash(pH,elem,h);
    }else{
      elem->data = data;
      elem->pKey = pKey;
      assert(nKey==elem->nKey);
    }
    return old_data;
  }
  if( data==0 ) return 0;
  new_elem = (HashElem*)sqlite3Malloc( sizeof(HashElem) );
  if( new_elem==0 ) return data;
  new_elem->pKey = pKey;
  new_elem->nKey = nKey;
  new_elem->data = data;
  pH->count++;
  if( pH->count>=10 && pH->count > 2*pH->htsize ){
    if( rehash(pH, pH->count*2) ){
      assert( pH->htsize>0 );
      h = strHash(pKey, nKey) % pH->htsize;
    }
  }
  if( pH->ht ){
    insertElement(pH, &pH->ht[h], new_elem);
  }else{
    insertElement(pH, 0, new_elem);
  }
  return 0;
}

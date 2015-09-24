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
** This is the header file for the generic hash-table implemenation
** used in SQLite.  We've modified it slightly to serve as a standalone
** hash table implementation for the full-text indexing module.
**
*/
#ifndef _HASH_H_
#define _HASH_H_

typedef struct Hash Hash;
typedef struct HashElem HashElem;

struct Hash {
  char keyClass;          
  char copyKey;           
  int count;              
  HashElem *first;        
  void *(*xMalloc)(int);  
  void (*xFree)(void *);  
  int htsize;             
  struct _ht {            
    int count;               
    HashElem *chain;         
  } *ht;
};

struct HashElem {
  HashElem *next, *prev;   
  void *data;              
  void *pKey; int nKey;    
};

#define HASH_STRING    3
#define HASH_BINARY    4

void HashInit(Hash*, int keytype, int copyKey);
void *HashInsert(Hash*, const void *pKey, int nKey, void *pData);
void *HashFind(const Hash*, const void *pKey, int nKey);
void HashClear(Hash*);

#define HashFirst(H)  ((H)->first)
#define HashNext(E)   ((E)->next)
#define HashData(E)   ((E)->data)
#define HashKey(E)    ((E)->pKey)
#define HashKeysize(E) ((E)->nKey)

#define HashCount(H)  ((H)->count)

#endif 

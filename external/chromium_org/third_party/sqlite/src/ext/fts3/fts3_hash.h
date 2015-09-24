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
#ifndef _FTS3_HASH_H_
#define _FTS3_HASH_H_

typedef struct Fts3Hash Fts3Hash;
typedef struct Fts3HashElem Fts3HashElem;

struct Fts3Hash {
  char keyClass;          
  char copyKey;           
  int count;              
  Fts3HashElem *first;    
  int htsize;             
  struct _fts3ht {        
    int count;               
    Fts3HashElem *chain;     
  } *ht;
};

struct Fts3HashElem {
  Fts3HashElem *next, *prev; 
  void *data;                
  void *pKey; int nKey;      
};

#define FTS3_HASH_STRING    1
#define FTS3_HASH_BINARY    2

void sqlite3Fts3HashInit(Fts3Hash *pNew, char keyClass, char copyKey);
void *sqlite3Fts3HashInsert(Fts3Hash*, const void *pKey, int nKey, void *pData);
void *sqlite3Fts3HashFind(const Fts3Hash*, const void *pKey, int nKey);
void sqlite3Fts3HashClear(Fts3Hash*);
Fts3HashElem *sqlite3Fts3HashFindElem(const Fts3Hash *, const void *, int);

#define fts3HashInit     sqlite3Fts3HashInit
#define fts3HashInsert   sqlite3Fts3HashInsert
#define fts3HashFind     sqlite3Fts3HashFind
#define fts3HashClear    sqlite3Fts3HashClear
#define fts3HashFindElem sqlite3Fts3HashFindElem

#define fts3HashFirst(H)  ((H)->first)
#define fts3HashNext(E)   ((E)->next)
#define fts3HashData(E)   ((E)->data)
#define fts3HashKey(E)    ((E)->pKey)
#define fts3HashKeysize(E) ((E)->nKey)

#define fts3HashCount(H)  ((H)->count)

#endif 

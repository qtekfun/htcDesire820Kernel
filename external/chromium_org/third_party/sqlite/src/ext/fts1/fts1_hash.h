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
#ifndef _FTS1_HASH_H_
#define _FTS1_HASH_H_

typedef struct fts1Hash fts1Hash;
typedef struct fts1HashElem fts1HashElem;

struct fts1Hash {
  char keyClass;          
  char copyKey;           
  int count;              
  fts1HashElem *first;    
  void *(*xMalloc)(int);  
  void (*xFree)(void *);  
  int htsize;             
  struct _fts1ht {        
    int count;               
    fts1HashElem *chain;     
  } *ht;
};

struct fts1HashElem {
  fts1HashElem *next, *prev; 
  void *data;                
  void *pKey; int nKey;      
};

#define FTS1_HASH_STRING    1
#define FTS1_HASH_BINARY    2

void sqlite3Fts1HashInit(fts1Hash*, int keytype, int copyKey);
void *sqlite3Fts1HashInsert(fts1Hash*, const void *pKey, int nKey, void *pData);
void *sqlite3Fts1HashFind(const fts1Hash*, const void *pKey, int nKey);
void sqlite3Fts1HashClear(fts1Hash*);

#define fts1HashInit   sqlite3Fts1HashInit
#define fts1HashInsert sqlite3Fts1HashInsert
#define fts1HashFind   sqlite3Fts1HashFind
#define fts1HashClear  sqlite3Fts1HashClear

#define fts1HashFirst(H)  ((H)->first)
#define fts1HashNext(E)   ((E)->next)
#define fts1HashData(E)   ((E)->data)
#define fts1HashKey(E)    ((E)->pKey)
#define fts1HashKeysize(E) ((E)->nKey)

#define fts1HashCount(H)  ((H)->count)

#endif 

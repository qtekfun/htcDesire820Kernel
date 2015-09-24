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
#ifndef _FTS2_HASH_H_
#define _FTS2_HASH_H_

typedef struct fts2Hash fts2Hash;
typedef struct fts2HashElem fts2HashElem;

struct fts2Hash {
  char keyClass;          
  char copyKey;           
  int count;              
  fts2HashElem *first;    
  int htsize;             
  struct _fts2ht {        
    int count;               
    fts2HashElem *chain;     
  } *ht;
};

struct fts2HashElem {
  fts2HashElem *next, *prev; 
  void *data;                
  void *pKey; int nKey;      
};

#define FTS2_HASH_STRING    1
#define FTS2_HASH_BINARY    2

void sqlite3Fts2HashInit(fts2Hash*, int keytype, int copyKey);
void *sqlite3Fts2HashInsert(fts2Hash*, const void *pKey, int nKey, void *pData);
void *sqlite3Fts2HashFind(const fts2Hash*, const void *pKey, int nKey);
void sqlite3Fts2HashClear(fts2Hash*);

#define fts2HashInit   sqlite3Fts2HashInit
#define fts2HashInsert sqlite3Fts2HashInsert
#define fts2HashFind   sqlite3Fts2HashFind
#define fts2HashClear  sqlite3Fts2HashClear

#define fts2HashFirst(H)  ((H)->first)
#define fts2HashNext(E)   ((E)->next)
#define fts2HashData(E)   ((E)->data)
#define fts2HashKey(E)    ((E)->pKey)
#define fts2HashKeysize(E) ((E)->nKey)

#define fts2HashCount(H)  ((H)->count)

#endif 

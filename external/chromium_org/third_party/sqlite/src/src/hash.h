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
** used in SQLite.
*/
#ifndef _SQLITE_HASH_H_
#define _SQLITE_HASH_H_

typedef struct Hash Hash;
typedef struct HashElem HashElem;

struct Hash {
  unsigned int htsize;      
  unsigned int count;       
  HashElem *first;          
  struct _ht {              
    int count;                 
    HashElem *chain;           
  } *ht;
};

struct HashElem {
  HashElem *next, *prev;       
  void *data;                  
  const char *pKey; int nKey;  
};

void sqlite3HashInit(Hash*);
void *sqlite3HashInsert(Hash*, const char *pKey, int nKey, void *pData);
void *sqlite3HashFind(const Hash*, const char *pKey, int nKey);
void sqlite3HashClear(Hash*);

#define sqliteHashFirst(H)  ((H)->first)
#define sqliteHashNext(E)   ((E)->next)
#define sqliteHashData(E)   ((E)->data)


#endif 

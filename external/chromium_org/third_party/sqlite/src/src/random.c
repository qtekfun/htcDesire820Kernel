/*
** 2001 September 15
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This file contains code to implement a pseudo-random number
** generator (PRNG) for SQLite.
**
** Random numbers are used by some of the database backends in order
** to generate random integer keys for tables or random filenames.
*/
#include "sqliteInt.h"


static SQLITE_WSD struct sqlite3PrngType {
  unsigned char isInit;          
  unsigned char i, j;            
  unsigned char s[256];          
} sqlite3Prng;

static u8 randomByte(void){
  unsigned char t;


#ifdef SQLITE_OMIT_WSD
  struct sqlite3PrngType *p = &GLOBAL(struct sqlite3PrngType, sqlite3Prng);
# define wsdPrng p[0]
#else
# define wsdPrng sqlite3Prng
#endif


  if( !wsdPrng.isInit ){
    int i;
    char k[256];
    wsdPrng.j = 0;
    wsdPrng.i = 0;
    sqlite3OsRandomness(sqlite3_vfs_find(0), 256, k);
    for(i=0; i<256; i++){
      wsdPrng.s[i] = (u8)i;
    }
    for(i=0; i<256; i++){
      wsdPrng.j += wsdPrng.s[i] + k[i];
      t = wsdPrng.s[wsdPrng.j];
      wsdPrng.s[wsdPrng.j] = wsdPrng.s[i];
      wsdPrng.s[i] = t;
    }
    wsdPrng.isInit = 1;
  }

  wsdPrng.i++;
  t = wsdPrng.s[wsdPrng.i];
  wsdPrng.j += t;
  wsdPrng.s[wsdPrng.i] = wsdPrng.s[wsdPrng.j];
  wsdPrng.s[wsdPrng.j] = t;
  t += wsdPrng.s[wsdPrng.i];
  return wsdPrng.s[t];
}

void sqlite3_randomness(int N, void *pBuf){
  unsigned char *zBuf = pBuf;
#if SQLITE_THREADSAFE
  sqlite3_mutex *mutex = sqlite3MutexAlloc(SQLITE_MUTEX_STATIC_PRNG);
#endif
  sqlite3_mutex_enter(mutex);
  while( N-- ){
    *(zBuf++) = randomByte();
  }
  sqlite3_mutex_leave(mutex);
}

#ifndef SQLITE_OMIT_BUILTIN_TEST
static SQLITE_WSD struct sqlite3PrngType sqlite3SavedPrng;
void sqlite3PrngSaveState(void){
  memcpy(
    &GLOBAL(struct sqlite3PrngType, sqlite3SavedPrng),
    &GLOBAL(struct sqlite3PrngType, sqlite3Prng),
    sizeof(sqlite3Prng)
  );
}
void sqlite3PrngRestoreState(void){
  memcpy(
    &GLOBAL(struct sqlite3PrngType, sqlite3Prng),
    &GLOBAL(struct sqlite3PrngType, sqlite3SavedPrng),
    sizeof(sqlite3Prng)
  );
}
void sqlite3PrngResetState(void){
  GLOBAL(struct sqlite3PrngType, sqlite3Prng).isInit = 0;
}
#endif 

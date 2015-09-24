/*
** The author disclaims copyright to this source code.
**
*************************************************************************
** Implementation of the "simple" full-text-search tokenizer.
*/

#include <assert.h>
#if !defined(__APPLE__)
#include <malloc.h>
#else
#include <stdlib.h>
#endif
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "tokenizer.h"

static char *string_dup(const char *s){
  char *str = malloc(strlen(s) + 1);
  strcpy(str, s);
  return str;
}

typedef struct simple_tokenizer {
  sqlite3_tokenizer base;
  const char *zDelim;          
} simple_tokenizer;

typedef struct simple_tokenizer_cursor {
  sqlite3_tokenizer_cursor base;
  const char *pInput;          
  int nBytes;                  
  const char *pCurrent;        
  int iToken;                  
  char *zToken;                
  int nTokenBytes;             
  int nTokenAllocated;         
} simple_tokenizer_cursor;

static sqlite3_tokenizer_module simpleTokenizerModule;

static int simpleCreate(
  int argc, const char **argv,
  sqlite3_tokenizer **ppTokenizer
){
  simple_tokenizer *t;

  t = (simple_tokenizer *) malloc(sizeof(simple_tokenizer));
  if( argc>1 ){
    t->zDelim = string_dup(argv[1]);
  } else {
    
    char zDelim[0x80];               
    int i, j;
    for(i=1, j=0; i<0x80; i++){
      if( !isalnum(i) ){
        zDelim[j++] = i;
      }
    }
    zDelim[j++] = '\0';
    assert( j<=sizeof(zDelim) );
    t->zDelim = string_dup(zDelim);
  }

  *ppTokenizer = &t->base;
  return SQLITE_OK;
}

static int simpleDestroy(sqlite3_tokenizer *pTokenizer){
  simple_tokenizer *t = (simple_tokenizer *) pTokenizer;

  free((void *) t->zDelim);
  free(t);

  return SQLITE_OK;
}

static int simpleOpen(
  sqlite3_tokenizer *pTokenizer,
  const char *pInput, int nBytes,
  sqlite3_tokenizer_cursor **ppCursor
){
  simple_tokenizer_cursor *c;

  c = (simple_tokenizer_cursor *) malloc(sizeof(simple_tokenizer_cursor));
  c->pInput = pInput;
  c->nBytes = nBytes<0 ? (int) strlen(pInput) : nBytes;
  c->pCurrent = c->pInput;        
  c->iToken = 0;
  c->zToken = NULL;               
  c->nTokenBytes = 0;
  c->nTokenAllocated = 0;

  *ppCursor = &c->base;
  return SQLITE_OK;
}

static int simpleClose(sqlite3_tokenizer_cursor *pCursor){
  simple_tokenizer_cursor *c = (simple_tokenizer_cursor *) pCursor;

  if( NULL!=c->zToken ){
    free(c->zToken);
  }
  free(c);

  return SQLITE_OK;
}

static int simpleNext(
  sqlite3_tokenizer_cursor *pCursor,
  const char **ppToken, int *pnBytes,
  int *piStartOffset, int *piEndOffset, int *piPosition
){
  simple_tokenizer_cursor *c = (simple_tokenizer_cursor *) pCursor;
  simple_tokenizer *t = (simple_tokenizer *) pCursor->pTokenizer;
  int ii;

  while( c->pCurrent-c->pInput<c->nBytes ){
    int n = (int) strcspn(c->pCurrent, t->zDelim);
    if( n>0 ){
      if( n+1>c->nTokenAllocated ){
        c->zToken = realloc(c->zToken, n+1);
      }
      for(ii=0; ii<n; ii++){
        char ch = c->pCurrent[ii];
        c->zToken[ii] = (unsigned char)ch<0x80 ? tolower(ch) : ch;
      }
      c->zToken[n] = '\0';
      *ppToken = c->zToken;
      *pnBytes = n;
      *piStartOffset = (int) (c->pCurrent-c->pInput);
      *piEndOffset = *piStartOffset+n;
      *piPosition = c->iToken++;
      c->pCurrent += n + 1;

      return SQLITE_OK;
    }
    c->pCurrent += n + 1;
  }
  return SQLITE_DONE;
}

static sqlite3_tokenizer_module simpleTokenizerModule = {
  0,
  simpleCreate,
  simpleDestroy,
  simpleOpen,
  simpleClose,
  simpleNext,
};

void get_simple_tokenizer_module(
  sqlite3_tokenizer_module **ppModule
){
  *ppModule = &simpleTokenizerModule;
}

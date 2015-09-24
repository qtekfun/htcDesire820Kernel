/*
** The author disclaims copyright to this source code.
**
*************************************************************************
** Implementation of the "simple" full-text-search tokenizer.
*/

#if !defined(SQLITE_CORE) || defined(SQLITE_ENABLE_FTS1)


#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "fts1_tokenizer.h"

typedef struct simple_tokenizer {
  sqlite3_tokenizer base;
  char delim[128];             
} simple_tokenizer;

typedef struct simple_tokenizer_cursor {
  sqlite3_tokenizer_cursor base;
  const char *pInput;          
  int nBytes;                  
  int iOffset;                 
  int iToken;                  
  char *pToken;                
  int nTokenAllocated;         
} simple_tokenizer_cursor;


static const sqlite3_tokenizer_module simpleTokenizerModule;

static int isDelim(simple_tokenizer *t, unsigned char c){
  return c<0x80 && t->delim[c];
}

static int simpleCreate(
  int argc, const char * const *argv,
  sqlite3_tokenizer **ppTokenizer
){
  simple_tokenizer *t;

  t = (simple_tokenizer *) calloc(sizeof(*t), 1);
  if( t==NULL ) return SQLITE_NOMEM;

  if( argc>1 ){
    int i, n = strlen(argv[1]);
    for(i=0; i<n; i++){
      unsigned char ch = argv[1][i];
      
      if( ch>=0x80 ){
        free(t);
        return SQLITE_ERROR;
      }
      t->delim[ch] = 1;
    }
  } else {
    
    int i;
    for(i=1; i<0x80; i++){
      t->delim[i] = !isalnum(i);
    }
  }

  *ppTokenizer = &t->base;
  return SQLITE_OK;
}

static int simpleDestroy(sqlite3_tokenizer *pTokenizer){
  free(pTokenizer);
  return SQLITE_OK;
}

static int simpleOpen(
  sqlite3_tokenizer *pTokenizer,         
  const char *pInput, int nBytes,        
  sqlite3_tokenizer_cursor **ppCursor    
){
  simple_tokenizer_cursor *c;

  c = (simple_tokenizer_cursor *) malloc(sizeof(*c));
  if( c==NULL ) return SQLITE_NOMEM;

  c->pInput = pInput;
  if( pInput==0 ){
    c->nBytes = 0;
  }else if( nBytes<0 ){
    c->nBytes = (int)strlen(pInput);
  }else{
    c->nBytes = nBytes;
  }
  c->iOffset = 0;                 
  c->iToken = 0;
  c->pToken = NULL;               
  c->nTokenAllocated = 0;

  *ppCursor = &c->base;
  return SQLITE_OK;
}

static int simpleClose(sqlite3_tokenizer_cursor *pCursor){
  simple_tokenizer_cursor *c = (simple_tokenizer_cursor *) pCursor;
  free(c->pToken);
  free(c);
  return SQLITE_OK;
}

static int simpleNext(
  sqlite3_tokenizer_cursor *pCursor,  
  const char **ppToken,               
  int *pnBytes,                       
  int *piStartOffset,                 
  int *piEndOffset,                   
  int *piPosition                     
){
  simple_tokenizer_cursor *c = (simple_tokenizer_cursor *) pCursor;
  simple_tokenizer *t = (simple_tokenizer *) pCursor->pTokenizer;
  unsigned char *p = (unsigned char *)c->pInput;

  while( c->iOffset<c->nBytes ){
    int iStartOffset;

    
    while( c->iOffset<c->nBytes && isDelim(t, p[c->iOffset]) ){
      c->iOffset++;
    }

    
    iStartOffset = c->iOffset;
    while( c->iOffset<c->nBytes && !isDelim(t, p[c->iOffset]) ){
      c->iOffset++;
    }

    if( c->iOffset>iStartOffset ){
      int i, n = c->iOffset-iStartOffset;
      if( n>c->nTokenAllocated ){
        c->nTokenAllocated = n+20;
        c->pToken = realloc(c->pToken, c->nTokenAllocated);
        if( c->pToken==NULL ) return SQLITE_NOMEM;
      }
      for(i=0; i<n; i++){
        unsigned char ch = p[iStartOffset+i];
        c->pToken[i] = ch<0x80 ? tolower(ch) : ch;
      }
      *ppToken = c->pToken;
      *pnBytes = n;
      *piStartOffset = iStartOffset;
      *piEndOffset = c->iOffset;
      *piPosition = c->iToken++;

      return SQLITE_OK;
    }
  }
  return SQLITE_DONE;
}

static const sqlite3_tokenizer_module simpleTokenizerModule = {
  0,
  simpleCreate,
  simpleDestroy,
  simpleOpen,
  simpleClose,
  simpleNext,
};

void sqlite3Fts1SimpleTokenizerModule(
  sqlite3_tokenizer_module const**ppModule
){
  *ppModule = &simpleTokenizerModule;
}

#endif 

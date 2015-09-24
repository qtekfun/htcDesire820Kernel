// Copyright (c) 2008, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef TCMALLOC_SPAN_H_
#define TCMALLOC_SPAN_H_

#include <config.h>
#include "common.h"

namespace tcmalloc {

struct Span {
  PageID        start;          
  Length        length;         
  Span*         next;           
  Span*         prev;           
  void*         objects;        
  unsigned int  refcount : 16;  
  unsigned int  sizeclass : 8;  
  unsigned int  location : 2;   
  unsigned int  sample : 1;     

#undef SPAN_HISTORY
#ifdef SPAN_HISTORY
  
  int nexthistory;
  char history[64];
  int value[64];
#endif

  
  enum { IN_USE, ON_NORMAL_FREELIST, ON_RETURNED_FREELIST };
};

#ifdef SPAN_HISTORY
void Event(Span* span, char op, int v = 0);
#else
#define Event(s,o,v) ((void) 0)
#endif

Span* NewSpan(PageID p, Length len);
void DeleteSpan(Span* span);


void DLL_Init(Span* list);

void DLL_Remove(Span* span);

inline bool DLL_IsEmpty(const Span* list) {
  return list->next == list;
}

void DLL_Prepend(Span* list, Span* span);

int DLL_Length(const Span* list);

}  

#endif  

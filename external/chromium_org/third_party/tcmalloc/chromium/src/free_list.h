// Copyright (c) 2011, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef TCMALLOC_FREE_LIST_H_
#define TCMALLOC_FREE_LIST_H_

#include <stddef.h>
#include "internal_logging.h"
#include "linked_list.h"
#include "system-alloc.h"

#define TCMALLOC_USE_DOUBLYLINKED_FREELIST

namespace tcmalloc {

#if defined(TCMALLOC_USE_DOUBLYLINKED_FREELIST)

static const bool kSupportsDoublyLinkedList = true;

void FL_PopRange(void **head, int n, void **start, void **end);
void FL_PushRange(void **head, void *start, void *end);
size_t FL_Size(void *head);

template <typename T> inline void FL_EqualityCheck(const T& v0,
                                                   const T& v1,
                                                   const char* file,
                                                   int line) {
  if (v0 != v1) Log(kCrash, file, line, "Memory corruption detected.");
}

inline void EnsureNonLoop(void* node, void* next) {
  
  
  if (node != next) return;
  Log(kCrash, __FILE__, __LINE__, "Circular loop in list detected: ", next);
}

inline void* MaskPtr(void* p) {
  
  const uintptr_t mask = ~(reinterpret_cast<uintptr_t>(TCMalloc_SystemAlloc)
                           >> 13);
  return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(p) ^ mask);
}

inline void* UnmaskPtr(void* p) {
  return MaskPtr(p);
}

inline void *FL_Previous_No_Check(void *t) {
  return UnmaskPtr(reinterpret_cast<void**>(t)[1]);
}

inline void *FL_Next_No_Check(void *t) {
  return UnmaskPtr(reinterpret_cast<void**>(t)[0]);
}

inline void *FL_Previous(void *t) {
  void *previous = FL_Previous_No_Check(t);
  if (previous) {
    FL_EqualityCheck(FL_Next_No_Check(previous), t, __FILE__, __LINE__);
  }
  return previous;
}

inline void FL_SetPrevious(void *t, void *n) {
  EnsureNonLoop(t, n);
  reinterpret_cast<void**>(t)[1] = MaskPtr(n);
}

inline void FL_SetNext(void *t, void *n) {
  EnsureNonLoop(t, n);
  reinterpret_cast<void**>(t)[0] = MaskPtr(n);
}

inline void *FL_Next(void *t) {
  void *next = FL_Next_No_Check(t);
  if (next) {
    FL_EqualityCheck(FL_Previous_No_Check(next), t, __FILE__, __LINE__);
  }
  return next;
}

inline void *FL_Pop(void **list) {
  void *result = *list;
  ASSERT(FL_Previous_No_Check(result) == NULL);
  *list = FL_Next(result);
  if (*list != NULL) {
    FL_SetPrevious(*list, NULL);
  }
  return result;
}

inline void FL_Init(void *t) {
  FL_SetPrevious(t, NULL);
  FL_SetNext(t, NULL);
}

inline void FL_Push(void **list, void *element) {
  void *old = *list;
  if (old == NULL) { 
    FL_Init(element);
  } else {
    ASSERT(FL_Previous_No_Check(old) == NULL);
    FL_SetNext(element, old);
    FL_SetPrevious(old, element);
    FL_SetPrevious(element, NULL);
  }
  *list = element;
}

#else 
static const bool kSupportsDoublyLinkedList = false;

inline void *FL_Next(void *t) {
  return SLL_Next(t);
}

inline void FL_Init(void *t) {
  SLL_SetNext(t, NULL);
}

inline void FL_Push(void **list, void *element) {
  if(*list != element) {
    SLL_Push(list,element);
    return;
  }
  Log(kCrash, __FILE__, __LINE__, "Double Free of %p detected", element);
}

inline void *FL_Pop(void **list) {
  return SLL_Pop(list);
}

inline void FL_PopRange(void **head, int n, void **start, void **end) {
  SLL_PopRange(head, n, start, end);
}

inline void FL_PushRange(void **head, void *start, void *end) {
  SLL_PushRange(head,start,end);
}

inline size_t FL_Size(void *head) {
  return SLL_Size(head);
}

#endif 

} 

#endif 

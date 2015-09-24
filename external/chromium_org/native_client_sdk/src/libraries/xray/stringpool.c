/* Copyright (c) 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file. */




#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include "xray/xray_priv.h"

#if defined(XRAY)

struct XRayStringPoolNode {
  struct XRayStringPoolNode* next;
  char strings[XRAY_STRING_POOL_NODE_SIZE];
};


struct XRayStringPool {
  struct XRayStringPoolNode* head;
  struct XRayStringPoolNode* current;
  int index;
};


static struct XRayStringPoolNode* XRayStringPoolAllocNode() {
  struct XRayStringPoolNode* s;
  s = (struct XRayStringPoolNode *)XRayMalloc(sizeof(*s));
  s->next = NULL;
  return s;
}


static int XRayStringPoolCurrentNodeSpaceAvail(struct XRayStringPool* pool) {
  int i = pool->index;
  return (XRAY_STRING_POOL_NODE_SIZE - i) - 1;
}


char* XRayStringPoolAppend(struct XRayStringPool* pool, const char* src) {
  
  
  int n = strnlen(src, XRAY_STRING_POOL_NODE_SIZE + 1) + 1;
  int a = XRayStringPoolCurrentNodeSpaceAvail(pool);
  char* dst;
  
  if (n >= (XRAY_STRING_POOL_NODE_SIZE - 1))
    return NULL;
  
  if (n > a) {
    pool->current->next = XRayStringPoolAllocNode();
    pool->current = pool->current->next;
    pool->index = 0;
  }
  
  dst = &pool->current->strings[pool->index];
  strcpy(dst, src);
  pool->index += n;
  return dst;
}


struct XRayStringPool* XRayStringPoolCreate() {
  struct XRayStringPool* pool;
  pool = (struct XRayStringPool*)XRayMalloc(sizeof(*pool));
  pool->head = XRayStringPoolAllocNode();
  pool->current = pool->head;
  return pool;
}


void XRayStringPoolFree(struct XRayStringPool* pool) {
  struct XRayStringPoolNode* n = pool->head;
  while (NULL != n) {
    struct XRayStringPoolNode* c = n;
    n = n->next;
    XRayFree(c);
  }
  XRayFree(pool);
}

#endif  


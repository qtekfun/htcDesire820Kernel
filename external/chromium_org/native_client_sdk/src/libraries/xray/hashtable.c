/* Copyright (c) 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file. */



#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xray/xray_priv.h"

#if defined(XRAY)

struct XRayHashTableEntry {
  void* data;
  uint32_t key;
};


struct XRayHashTable {
  int capacity;
  int count;
  struct XRayHashTableEntry* array;
};


XRAY_NO_INSTRUMENT void XRayHashTableGrow(struct XRayHashTable* table);
XRAY_NO_INSTRUMENT uint32_t XRayHashTableHashKey(uint32_t key);
XRAY_NO_INSTRUMENT void XRayHashTableInit(struct XRayHashTable* table,
    int32_t capacity);

#define HASH_HISTO 1024
int g_hash_histo[HASH_HISTO];


uint32_t XRayHashTableHashKey(uint32_t x) {
  uint32_t y = x * 7919;
  uint32_t z;
  size_t c;
  uint8_t* s = (uint8_t*)&y;
  
  uint32_t h = 5381;
  for (c = 0; c < sizeof(y); ++c) {
    z = s[c];
    h = ((h << 5) + h) + z;
  }
  return h;
}


int XRayHashTableGetCapacity(struct XRayHashTable* table) {
  return table->capacity;
}


int XRayHashTableGetCount(struct XRayHashTable* table) {
  return table->count;
}


void* XRayHashTableLookup(struct XRayHashTable* table, uint32_t key) {
  uint32_t h = XRayHashTableHashKey(key);
  uint32_t m = table->capacity - 1;
  uint32_t j = h & m;
  uint32_t i;
  int z = 1;
  for (i = 0; i < m; ++i) {
    
    if (NULL == table->array[j].data) {
      ++g_hash_histo[0];
      return NULL;
    }
    
    if (table->array[j].key == key) {
      if (z >= HASH_HISTO)
        z = HASH_HISTO - 1;
      ++g_hash_histo[z];
      return table->array[j].data;
    }
    j = (j + 1) & m;
    ++z;
  }
  
  return NULL;
}


void* XRayHashTableInsert(struct XRayHashTable* table,
                          void* data, uint32_t key) {
  uint32_t h = XRayHashTableHashKey(key);
  uint32_t m = table->capacity - 1;
  uint32_t j = h & m;
  uint32_t i;
  for (i = 0; i < m; ++i) {
    
    
    if (NULL == table->array[j].data) {
      void* ret;
      float ratio;
      table->array[j].data = data;
      table->array[j].key = key;
      ++table->count;
      ret = data;
      ratio = (float)table->count / (float)table->capacity;
      
      if (ratio > XRAY_SYMBOL_TABLE_MAX_RATIO)
        XRayHashTableGrow(table);
      return ret;
    }
    
    if (table->array[j].key == key) {
      return table->array[j].data;
    }
    j = (j + 1) & m;
  }
  
  return NULL;
}


void* XRayHashTableAtIndex(struct XRayHashTable* table, int i) {
  if ((i < 0) || (i >= table->capacity))
    return NULL;
  return table->array[i].data;
}


void XRayHashTableGrow(struct XRayHashTable* table) {
  struct XRayHashTableEntry* old_array = table->array;
  int old_capacity = table->capacity;
  int new_capacity = old_capacity * 2;
  int i;
  printf("XRay: Growing a hash table...\n");
  XRayHashTableInit(table, new_capacity);
  for (i = 0; i < old_capacity; ++i) {
    void* data = old_array[i].data;
    if (NULL != data) {
      uint32_t key = old_array[i].key;
      XRayHashTableInsert(table, data, key);
    }
  }
  XRayFree(old_array);
}


void XRayHashTableInit(struct XRayHashTable* table, int32_t capacity) {
  size_t bytes;
  if (0 != (capacity & (capacity - 1))) {
    printf("Xray: Hash table capacity should be a power of 2!\n");
    
    
    capacity--;
    capacity |= capacity >> 1;
    capacity |= capacity >> 2;
    capacity |= capacity >> 4;
    capacity |= capacity >> 8;
    capacity |= capacity >> 16;
    capacity++;
  }
  bytes = sizeof(table->array[0]) * capacity;
  table->capacity = capacity;
  table->count = 0;
  table->array = (struct XRayHashTableEntry*)XRayMalloc(bytes);
}


struct XRayHashTable* XRayHashTableCreate(int capacity) {
  struct XRayHashTable* table;
  table = (struct XRayHashTable*)XRayMalloc(sizeof(*table));
  XRayHashTableInit(table, capacity);
  memset(&g_hash_histo[0], 0, sizeof(g_hash_histo[0]) * HASH_HISTO);
  return table;
}


void XRayHashTableHisto(FILE* f) {
  int i;
  for (i = 0; i < HASH_HISTO; ++i) {
    if (0 != g_hash_histo[i])
      fprintf(f, "hash_iterations[%d] = %d\n", i, g_hash_histo[i]);
  }
}


void XRayHashTableFree(struct XRayHashTable* table) {
  XRayFree(table->array);
  table->capacity = 0;
  table->count = 0;
  table->array = NULL;
  XRayFree(table);
}

#endif  


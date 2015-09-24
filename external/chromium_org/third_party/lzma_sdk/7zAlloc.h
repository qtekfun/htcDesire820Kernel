
#ifndef __7Z_ALLOC_H
#define __7Z_ALLOC_H

#include <stdlib.h>

void *SzAlloc(void *p, size_t size);
void SzFree(void *p, void *address);

void *SzAllocTemp(void *p, size_t size);
void SzFreeTemp(void *p, void *address);

#endif

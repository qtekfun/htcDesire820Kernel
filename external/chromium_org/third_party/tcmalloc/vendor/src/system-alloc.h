// Copyright (c) 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef TCMALLOC_SYSTEM_ALLOC_H_
#define TCMALLOC_SYSTEM_ALLOC_H_

#include <config.h>
#include <stddef.h>                     

class SysAllocator;

extern void* TCMalloc_SystemAlloc(size_t bytes, size_t *actual_bytes,
                                  size_t alignment = 0);

extern void TCMalloc_SystemRelease(void* start, size_t length);

extern PERFTOOLS_DLL_DECL SysAllocator* sys_alloc;

#endif 

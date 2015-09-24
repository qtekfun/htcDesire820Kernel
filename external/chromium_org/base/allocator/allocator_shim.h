// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_ALLOCATOR_ALLOCATOR_SHIM_H_
#define BASE_ALLOCATOR_ALLOCATOR_SHIM_H_

#include <stddef.h>

namespace base {
namespace allocator {

void SetupSubprocessAllocator();

void* TCMallocDoMallocForTest(size_t size);
void TCMallocDoFreeForTest(void* ptr);
size_t ExcludeSpaceForMarkForTest(size_t size);

}  
}  

#endif   

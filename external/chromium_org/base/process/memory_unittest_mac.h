// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_PROCESS_MEMORY_UNITTEST_MAC_H_
#define BASE_PROCESS_MEMORY_UNITTEST_MAC_H_

#include "base/basictypes.h"

namespace base {

void* AllocateViaCFAllocatorSystemDefault(ssize_t size);
void* AllocateViaCFAllocatorMalloc(ssize_t size);
void* AllocateViaCFAllocatorMallocZone(ssize_t size);

#if !defined(ARCH_CPU_64_BITS)

void* AllocatePsychoticallyBigObjCObject();

#endif  

}  

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_LIBJINGLE_OVERRIDES_ALLOCATOR_SHIM_ALLOCATOR_STUB_H_
#define THIRD_PARTY_LIBJINGLE_OVERRIDES_ALLOCATOR_SHIM_ALLOCATOR_STUB_H_

#include <new>

#include "base/basictypes.h"

#if !defined(OS_MACOSX) && !defined(OS_ANDROID)

typedef void* (*AllocateFunction)(std::size_t);
typedef void (*DellocateFunction)(void*);

void* Allocate(std::size_t n);
void Dellocate(void* p);

#endif  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_ALLOCATOR_ALLOCATOR_EXTENSION_H
#define BASE_ALLOCATOR_ALLOCATOR_EXTENSION_H

#include <stddef.h> 

#include "base/allocator/allocator_extension_thunks.h"
#include "base/base_export.h"
#include "build/build_config.h"

namespace base {
namespace allocator {

BASE_EXPORT bool GetAllocatorWasteSize(size_t* size);

BASE_EXPORT void GetStats(char* buffer, int buffer_length);

BASE_EXPORT void ReleaseFreeMemory();


BASE_EXPORT void SetGetAllocatorWasteSizeFunction(
    thunks::GetAllocatorWasteSizeFunction get_allocator_waste_size_function);

BASE_EXPORT void SetGetStatsFunction(
    thunks::GetStatsFunction get_stats_function);

BASE_EXPORT void SetReleaseFreeMemoryFunction(
    thunks::ReleaseFreeMemoryFunction release_free_memory_function);

}  
}  

#endif

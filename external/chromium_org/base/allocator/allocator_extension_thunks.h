// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_ALLOCATOR_ALLOCATOR_THUNKS_EXTENSION_H
#define BASE_ALLOCATOR_ALLOCATOR_THUNKS_EXTENSION_H

#include <stddef.h> 

namespace base {
namespace allocator {
namespace thunks {


typedef bool (*GetAllocatorWasteSizeFunction)(size_t* size);
void SetGetAllocatorWasteSizeFunction(
    GetAllocatorWasteSizeFunction get_allocator_waste_size_function);
GetAllocatorWasteSizeFunction GetGetAllocatorWasteSizeFunction();

typedef void (*GetStatsFunction)(char* buffer, int buffer_length);
void SetGetStatsFunction(GetStatsFunction get_stats_function);
GetStatsFunction GetGetStatsFunction();

typedef void (*ReleaseFreeMemoryFunction)();
void SetReleaseFreeMemoryFunction(
    ReleaseFreeMemoryFunction release_free_memory_function);
ReleaseFreeMemoryFunction GetReleaseFreeMemoryFunction();

}  
}  
}  

#endif

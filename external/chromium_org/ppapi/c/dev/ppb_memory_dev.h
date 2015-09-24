/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_MEMORY_DEV_H_
#define PPAPI_C_DEV_PPB_MEMORY_DEV_H_

#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_MEMORY_DEV_INTERFACE_0_1 "PPB_Memory(Dev);0.1"
#define PPB_MEMORY_DEV_INTERFACE PPB_MEMORY_DEV_INTERFACE_0_1



struct PPB_Memory_Dev_0_1 {
  void* (*MemAlloc)(uint32_t num_bytes);
  void (*MemFree)(void* ptr);
};

typedef struct PPB_Memory_Dev_0_1 PPB_Memory_Dev;

#endif  


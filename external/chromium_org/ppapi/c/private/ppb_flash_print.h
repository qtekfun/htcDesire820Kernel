/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPB_FLASH_PRINT_H_
#define PPAPI_C_PRIVATE_PPB_FLASH_PRINT_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_FLASH_PRINT_INTERFACE_1_0 "PPB_Flash_Print;1.0"
#define PPB_FLASH_PRINT_INTERFACE PPB_FLASH_PRINT_INTERFACE_1_0



struct PPB_Flash_Print_1_0 {
  void (*InvokePrinting)(PP_Instance instance);
};

typedef struct PPB_Flash_Print_1_0 PPB_Flash_Print;

#endif  


/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPB_FLASH_MESSAGE_LOOP_H_
#define PPAPI_C_PRIVATE_PPB_FLASH_MESSAGE_LOOP_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_FLASH_MESSAGELOOP_INTERFACE_0_1 "PPB_Flash_MessageLoop;0.1"
#define PPB_FLASH_MESSAGELOOP_INTERFACE PPB_FLASH_MESSAGELOOP_INTERFACE_0_1



struct PPB_Flash_MessageLoop_0_1 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsFlashMessageLoop)(PP_Resource resource);
  int32_t (*Run)(PP_Resource flash_message_loop);
  void (*Quit)(PP_Resource flash_message_loop);
};

typedef struct PPB_Flash_MessageLoop_0_1 PPB_Flash_MessageLoop;

#endif  


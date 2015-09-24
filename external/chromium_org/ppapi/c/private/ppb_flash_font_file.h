/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPB_FLASH_FONT_FILE_H_
#define PPAPI_C_PRIVATE_PPB_FLASH_FONT_FILE_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/private/pp_private_font_charset.h"
#include "ppapi/c/trusted/ppb_browser_font_trusted.h"

#define PPB_FLASH_FONTFILE_INTERFACE_0_1 "PPB_Flash_FontFile;0.1"
#define PPB_FLASH_FONTFILE_INTERFACE PPB_FLASH_FONTFILE_INTERFACE_0_1



struct PPB_Flash_FontFile_0_1 {
  PP_Resource (*Create)(
      PP_Instance instance,
      const struct PP_BrowserFont_Trusted_Description* description,
      PP_PrivateFontCharset charset);
  PP_Bool (*IsFlashFontFile)(PP_Resource resource);
  PP_Bool (*GetFontTable)(PP_Resource font_file,
                          uint32_t table,
                          void* output,
                          uint32_t* output_length);
};

typedef struct PPB_Flash_FontFile_0_1 PPB_Flash_FontFile;

#endif  


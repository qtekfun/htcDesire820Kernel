/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPB_FLASH_H_
#define PPAPI_C_PRIVATE_PPB_FLASH_H_

#include "ppapi/c/pp_array_output.h"
#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_point.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_size.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_time.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/trusted/ppb_browser_font_trusted.h"

#define PPB_FLASH_INTERFACE_12_4 "PPB_Flash;12.4"
#define PPB_FLASH_INTERFACE_12_5 "PPB_Flash;12.5"
#define PPB_FLASH_INTERFACE_12_6 "PPB_Flash;12.6"
#define PPB_FLASH_INTERFACE_13_0 "PPB_Flash;13.0"
#define PPB_FLASH_INTERFACE PPB_FLASH_INTERFACE_13_0



typedef enum {
  PP_FLASHLSORESTRICTIONS_NONE = 1,
  PP_FLASHLSORESTRICTIONS_BLOCK = 2,
  PP_FLASHLSORESTRICTIONS_IN_MEMORY = 3
} PP_FlashLSORestrictions;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_FlashLSORestrictions, 4);

typedef enum {
  PP_FLASHSETTING_3DENABLED = 1,
  PP_FLASHSETTING_INCOGNITO = 2,
  PP_FLASHSETTING_STAGE3DENABLED = 3,
  PP_FLASHSETTING_LANGUAGE = 4,
  PP_FLASHSETTING_NUMCORES = 5,
  PP_FLASHSETTING_LSORESTRICTIONS = 6,
  PP_FLASHSETTING_STAGE3DBASELINEENABLED = 7
} PP_FlashSetting;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_FlashSetting, 4);

typedef enum {
  PP_FLASHCRASHKEY_URL = 1,
  PP_FLASHCRASHKEY_RESOURCE_URL = 2
} PP_FlashCrashKey;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_FlashCrashKey, 4);

struct PPB_Flash_13_0 {
  void (*SetInstanceAlwaysOnTop)(PP_Instance instance, PP_Bool on_top);
  PP_Bool (*DrawGlyphs)(
      PP_Instance instance,
      PP_Resource pp_image_data,
      const struct PP_BrowserFont_Trusted_Description* font_desc,
      uint32_t color,
      const struct PP_Point* position,
      const struct PP_Rect* clip,
      const float transformation[3][3],
      PP_Bool allow_subpixel_aa,
      uint32_t glyph_count,
      const uint16_t glyph_indices[],
      const struct PP_Point glyph_advances[]);
  struct PP_Var (*GetProxyForURL)(PP_Instance instance, const char* url);
  int32_t (*Navigate)(PP_Resource request_info,
                      const char* target,
                      PP_Bool from_user_action);
  double (*GetLocalTimeZoneOffset)(PP_Instance instance, PP_Time t);
  struct PP_Var (*GetCommandLineArgs)(PP_Module module);
  void (*PreloadFontWin)(const void* logfontw);
  PP_Bool (*IsRectTopmost)(PP_Instance instance, const struct PP_Rect* rect);
  void (*UpdateActivity)(PP_Instance instance);
  struct PP_Var (*GetSetting)(PP_Instance instance, PP_FlashSetting setting);
  PP_Bool (*SetCrashData)(PP_Instance instance,
                          PP_FlashCrashKey key,
                          struct PP_Var value);
  int32_t (*EnumerateVideoCaptureDevices)(PP_Instance instance,
                                          PP_Resource video_capture,
                                          struct PP_ArrayOutput devices);
};

typedef struct PPB_Flash_13_0 PPB_Flash;

struct PPB_Flash_12_4 {
  void (*SetInstanceAlwaysOnTop)(PP_Instance instance, PP_Bool on_top);
  PP_Bool (*DrawGlyphs)(
      PP_Instance instance,
      PP_Resource pp_image_data,
      const struct PP_BrowserFont_Trusted_Description* font_desc,
      uint32_t color,
      const struct PP_Point* position,
      const struct PP_Rect* clip,
      const float transformation[3][3],
      PP_Bool allow_subpixel_aa,
      uint32_t glyph_count,
      const uint16_t glyph_indices[],
      const struct PP_Point glyph_advances[]);
  struct PP_Var (*GetProxyForURL)(PP_Instance instance, const char* url);
  int32_t (*Navigate)(PP_Resource request_info,
                      const char* target,
                      PP_Bool from_user_action);
  void (*RunMessageLoop)(PP_Instance instance);
  void (*QuitMessageLoop)(PP_Instance instance);
  double (*GetLocalTimeZoneOffset)(PP_Instance instance, PP_Time t);
  struct PP_Var (*GetCommandLineArgs)(PP_Module module);
  void (*PreloadFontWin)(const void* logfontw);
  PP_Bool (*IsRectTopmost)(PP_Instance instance, const struct PP_Rect* rect);
  int32_t (*InvokePrinting)(PP_Instance instance);
  void (*UpdateActivity)(PP_Instance instance);
  struct PP_Var (*GetDeviceID)(PP_Instance instance);
  int32_t (*GetSettingInt)(PP_Instance instance, PP_FlashSetting setting);
  struct PP_Var (*GetSetting)(PP_Instance instance, PP_FlashSetting setting);
};

struct PPB_Flash_12_5 {
  void (*SetInstanceAlwaysOnTop)(PP_Instance instance, PP_Bool on_top);
  PP_Bool (*DrawGlyphs)(
      PP_Instance instance,
      PP_Resource pp_image_data,
      const struct PP_BrowserFont_Trusted_Description* font_desc,
      uint32_t color,
      const struct PP_Point* position,
      const struct PP_Rect* clip,
      const float transformation[3][3],
      PP_Bool allow_subpixel_aa,
      uint32_t glyph_count,
      const uint16_t glyph_indices[],
      const struct PP_Point glyph_advances[]);
  struct PP_Var (*GetProxyForURL)(PP_Instance instance, const char* url);
  int32_t (*Navigate)(PP_Resource request_info,
                      const char* target,
                      PP_Bool from_user_action);
  void (*RunMessageLoop)(PP_Instance instance);
  void (*QuitMessageLoop)(PP_Instance instance);
  double (*GetLocalTimeZoneOffset)(PP_Instance instance, PP_Time t);
  struct PP_Var (*GetCommandLineArgs)(PP_Module module);
  void (*PreloadFontWin)(const void* logfontw);
  PP_Bool (*IsRectTopmost)(PP_Instance instance, const struct PP_Rect* rect);
  int32_t (*InvokePrinting)(PP_Instance instance);
  void (*UpdateActivity)(PP_Instance instance);
  struct PP_Var (*GetDeviceID)(PP_Instance instance);
  int32_t (*GetSettingInt)(PP_Instance instance, PP_FlashSetting setting);
  struct PP_Var (*GetSetting)(PP_Instance instance, PP_FlashSetting setting);
  PP_Bool (*SetCrashData)(PP_Instance instance,
                          PP_FlashCrashKey key,
                          struct PP_Var value);
};

struct PPB_Flash_12_6 {
  void (*SetInstanceAlwaysOnTop)(PP_Instance instance, PP_Bool on_top);
  PP_Bool (*DrawGlyphs)(
      PP_Instance instance,
      PP_Resource pp_image_data,
      const struct PP_BrowserFont_Trusted_Description* font_desc,
      uint32_t color,
      const struct PP_Point* position,
      const struct PP_Rect* clip,
      const float transformation[3][3],
      PP_Bool allow_subpixel_aa,
      uint32_t glyph_count,
      const uint16_t glyph_indices[],
      const struct PP_Point glyph_advances[]);
  struct PP_Var (*GetProxyForURL)(PP_Instance instance, const char* url);
  int32_t (*Navigate)(PP_Resource request_info,
                      const char* target,
                      PP_Bool from_user_action);
  void (*RunMessageLoop)(PP_Instance instance);
  void (*QuitMessageLoop)(PP_Instance instance);
  double (*GetLocalTimeZoneOffset)(PP_Instance instance, PP_Time t);
  struct PP_Var (*GetCommandLineArgs)(PP_Module module);
  void (*PreloadFontWin)(const void* logfontw);
  PP_Bool (*IsRectTopmost)(PP_Instance instance, const struct PP_Rect* rect);
  int32_t (*InvokePrinting)(PP_Instance instance);
  void (*UpdateActivity)(PP_Instance instance);
  struct PP_Var (*GetDeviceID)(PP_Instance instance);
  int32_t (*GetSettingInt)(PP_Instance instance, PP_FlashSetting setting);
  struct PP_Var (*GetSetting)(PP_Instance instance, PP_FlashSetting setting);
  PP_Bool (*SetCrashData)(PP_Instance instance,
                          PP_FlashCrashKey key,
                          struct PP_Var value);
  int32_t (*EnumerateVideoCaptureDevices)(PP_Instance instance,
                                          PP_Resource video_capture,
                                          struct PP_ArrayOutput devices);
};

#endif  


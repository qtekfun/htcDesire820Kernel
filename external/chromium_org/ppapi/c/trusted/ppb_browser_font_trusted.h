/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_TRUSTED_PPB_BROWSER_FONT_TRUSTED_H_
#define PPAPI_C_TRUSTED_PPB_BROWSER_FONT_TRUSTED_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_point.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_size.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_BROWSERFONT_TRUSTED_INTERFACE_1_0 "PPB_BrowserFont_Trusted;1.0"
#define PPB_BROWSERFONT_TRUSTED_INTERFACE PPB_BROWSERFONT_TRUSTED_INTERFACE_1_0



typedef enum {
  PP_BROWSERFONT_TRUSTED_FAMILY_DEFAULT = 0,
  PP_BROWSERFONT_TRUSTED_FAMILY_SERIF = 1,
  PP_BROWSERFONT_TRUSTED_FAMILY_SANSSERIF = 2,
  PP_BROWSERFONT_TRUSTED_FAMILY_MONOSPACE = 3
} PP_BrowserFont_Trusted_Family;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_BrowserFont_Trusted_Family, 4);

typedef enum {
  PP_BROWSERFONT_TRUSTED_WEIGHT_100 = 0,
  PP_BROWSERFONT_TRUSTED_WEIGHT_200 = 1,
  PP_BROWSERFONT_TRUSTED_WEIGHT_300 = 2,
  PP_BROWSERFONT_TRUSTED_WEIGHT_400 = 3,
  PP_BROWSERFONT_TRUSTED_WEIGHT_500 = 4,
  PP_BROWSERFONT_TRUSTED_WEIGHT_600 = 5,
  PP_BROWSERFONT_TRUSTED_WEIGHT_700 = 6,
  PP_BROWSERFONT_TRUSTED_WEIGHT_800 = 7,
  PP_BROWSERFONT_TRUSTED_WEIGHT_900 = 8,
  PP_BROWSERFONT_TRUSTED_WEIGHT_NORMAL = PP_BROWSERFONT_TRUSTED_WEIGHT_400,
  PP_BROWSERFONT_TRUSTED_WEIGHT_BOLD = PP_BROWSERFONT_TRUSTED_WEIGHT_700
} PP_BrowserFont_Trusted_Weight;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_BrowserFont_Trusted_Weight, 4);

struct PP_BrowserFont_Trusted_Description {
  struct PP_Var face;
  PP_BrowserFont_Trusted_Family family;
  uint32_t size;
  PP_BrowserFont_Trusted_Weight weight;
  PP_Bool italic;
  PP_Bool small_caps;
  int32_t letter_spacing;
  int32_t word_spacing;
  int32_t padding;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_BrowserFont_Trusted_Description, 48);

struct PP_BrowserFont_Trusted_Metrics {
  int32_t height;
  int32_t ascent;
  int32_t descent;
  int32_t line_spacing;
  int32_t x_height;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_BrowserFont_Trusted_Metrics, 20);

struct PP_BrowserFont_Trusted_TextRun {
  struct PP_Var text;
  PP_Bool rtl;
  PP_Bool override_direction;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_BrowserFont_Trusted_TextRun, 24);

struct PPB_BrowserFont_Trusted_1_0 {
  struct PP_Var (*GetFontFamilies)(PP_Instance instance);
  PP_Resource (*Create)(
      PP_Instance instance,
      const struct PP_BrowserFont_Trusted_Description* description);
  PP_Bool (*IsFont)(PP_Resource resource);
  PP_Bool (*Describe)(PP_Resource font,
                      struct PP_BrowserFont_Trusted_Description* description,
                      struct PP_BrowserFont_Trusted_Metrics* metrics);
  PP_Bool (*DrawTextAt)(PP_Resource font,
                        PP_Resource image_data,
                        const struct PP_BrowserFont_Trusted_TextRun* text,
                        const struct PP_Point* position,
                        uint32_t color,
                        const struct PP_Rect* clip,
                        PP_Bool image_data_is_opaque);
  int32_t (*MeasureText)(PP_Resource font,
                         const struct PP_BrowserFont_Trusted_TextRun* text);
  uint32_t (*CharacterOffsetForPixel)(
      PP_Resource font,
      const struct PP_BrowserFont_Trusted_TextRun* text,
      int32_t pixel_position);
  int32_t (*PixelOffsetForCharacter)(
      PP_Resource font,
      const struct PP_BrowserFont_Trusted_TextRun* text,
      uint32_t char_offset);
};

typedef struct PPB_BrowserFont_Trusted_1_0 PPB_BrowserFont_Trusted;

#endif  


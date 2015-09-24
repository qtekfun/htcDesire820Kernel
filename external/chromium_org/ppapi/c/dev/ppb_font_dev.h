/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_FONT_DEV_H_
#define PPAPI_C_DEV_PPB_FONT_DEV_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_point.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_size.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_FONT_DEV_INTERFACE_0_6 "PPB_Font(Dev);0.6"
#define PPB_FONT_DEV_INTERFACE PPB_FONT_DEV_INTERFACE_0_6



typedef enum {
  PP_FONTFAMILY_DEFAULT = 0,
  PP_FONTFAMILY_SERIF = 1,
  PP_FONTFAMILY_SANSSERIF = 2,
  PP_FONTFAMILY_MONOSPACE = 3
} PP_FontFamily_Dev;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_FontFamily_Dev, 4);

typedef enum {
  PP_FONTWEIGHT_100 = 0,
  PP_FONTWEIGHT_200 = 1,
  PP_FONTWEIGHT_300 = 2,
  PP_FONTWEIGHT_400 = 3,
  PP_FONTWEIGHT_500 = 4,
  PP_FONTWEIGHT_600 = 5,
  PP_FONTWEIGHT_700 = 6,
  PP_FONTWEIGHT_800 = 7,
  PP_FONTWEIGHT_900 = 8,
  PP_FONTWEIGHT_NORMAL = PP_FONTWEIGHT_400,
  PP_FONTWEIGHT_BOLD = PP_FONTWEIGHT_700
} PP_FontWeight_Dev;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_FontWeight_Dev, 4);

struct PP_FontDescription_Dev {
  struct PP_Var face;
  PP_FontFamily_Dev family;
  uint32_t size;
  PP_FontWeight_Dev weight;
  PP_Bool italic;
  PP_Bool small_caps;
  int32_t letter_spacing;
  int32_t word_spacing;
  int32_t padding;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_FontDescription_Dev, 48);

struct PP_FontMetrics_Dev {
  int32_t height;
  int32_t ascent;
  int32_t descent;
  int32_t line_spacing;
  int32_t x_height;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_FontMetrics_Dev, 20);

struct PP_TextRun_Dev {
  struct PP_Var text;
  PP_Bool rtl;
  PP_Bool override_direction;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_TextRun_Dev, 24);

struct PPB_Font_Dev_0_6 {
  struct PP_Var (*GetFontFamilies)(PP_Instance instance);
  PP_Resource (*Create)(PP_Instance instance,
                        const struct PP_FontDescription_Dev* description);
  PP_Bool (*IsFont)(PP_Resource resource);
  PP_Bool (*Describe)(PP_Resource font,
                      struct PP_FontDescription_Dev* description,
                      struct PP_FontMetrics_Dev* metrics);
  PP_Bool (*DrawTextAt)(PP_Resource font,
                        PP_Resource image_data,
                        const struct PP_TextRun_Dev* text,
                        const struct PP_Point* position,
                        uint32_t color,
                        const struct PP_Rect* clip,
                        PP_Bool image_data_is_opaque);
  int32_t (*MeasureText)(PP_Resource font, const struct PP_TextRun_Dev* text);
  uint32_t (*CharacterOffsetForPixel)(PP_Resource font,
                                      const struct PP_TextRun_Dev* text,
                                      int32_t pixel_position);
  int32_t (*PixelOffsetForCharacter)(PP_Resource font,
                                     const struct PP_TextRun_Dev* text,
                                     uint32_t char_offset);
};

typedef struct PPB_Font_Dev_0_6 PPB_Font_Dev;

#endif  


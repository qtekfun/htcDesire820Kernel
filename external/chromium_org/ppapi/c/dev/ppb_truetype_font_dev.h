/* Copyright (c) 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_TRUETYPE_FONT_DEV_H_
#define PPAPI_C_DEV_PPB_TRUETYPE_FONT_DEV_H_

#include "ppapi/c/pp_array_output.h"
#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_TRUETYPEFONT_DEV_INTERFACE_0_1 "PPB_TrueTypeFont(Dev);0.1"
#define PPB_TRUETYPEFONT_DEV_INTERFACE PPB_TRUETYPEFONT_DEV_INTERFACE_0_1



typedef enum {
  PP_TRUETYPEFONTFAMILY_SERIF = 0,
  PP_TRUETYPEFONTFAMILY_SANSSERIF = 1,
  PP_TRUETYPEFONTFAMILY_CURSIVE = 2,
  PP_TRUETYPEFONTFAMILY_FANTASY = 3,
  PP_TRUETYPEFONTFAMILY_MONOSPACE = 4
} PP_TrueTypeFontFamily_Dev;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_TrueTypeFontFamily_Dev, 4);

typedef enum {
  PP_TRUETYPEFONTSTYLE_NORMAL = 0,
  PP_TRUETYPEFONTSTYLE_ITALIC = 1
} PP_TrueTypeFontStyle_Dev;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_TrueTypeFontStyle_Dev, 4);

typedef enum {
  PP_TRUETYPEFONTWEIGHT_THIN = 100,
  PP_TRUETYPEFONTWEIGHT_ULTRALIGHT = 200,
  PP_TRUETYPEFONTWEIGHT_LIGHT = 300,
  PP_TRUETYPEFONTWEIGHT_NORMAL = 400,
  PP_TRUETYPEFONTWEIGHT_MEDIUM = 500,
  PP_TRUETYPEFONTWEIGHT_SEMIBOLD = 600,
  PP_TRUETYPEFONTWEIGHT_BOLD = 700,
  PP_TRUETYPEFONTWEIGHT_ULTRABOLD = 800,
  PP_TRUETYPEFONTWEIGHT_HEAVY = 900
} PP_TrueTypeFontWeight_Dev;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_TrueTypeFontWeight_Dev, 4);

typedef enum {
  PP_TRUETYPEFONTWIDTH_ULTRACONDENSED = 0,
  PP_TRUETYPEFONTWIDTH_EXTRACONDENSED = 1,
  PP_TRUETYPEFONTWIDTH_CONDENSED = 2,
  PP_TRUETYPEFONTWIDTH_SEMICONDENSED = 3,
  PP_TRUETYPEFONTWIDTH_NORMAL = 4,
  PP_TRUETYPEFONTWIDTH_SEMIEXPANDED = 5,
  PP_TRUETYPEFONTWIDTH_EXPANDED = 6,
  PP_TRUETYPEFONTWIDTH_EXTRAEXPANDED = 7,
  PP_TRUETYPEFONTWIDTH_ULTRAEXPANDED = 8
} PP_TrueTypeFontWidth_Dev;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_TrueTypeFontWidth_Dev, 4);

typedef enum {
  PP_TRUETYPEFONTCHARSET_ANSI = 0,
  PP_TRUETYPEFONTCHARSET_DEFAULT = 1,
  PP_TRUETYPEFONTCHARSET_SYMBOL = 2,
  PP_TRUETYPEFONTCHARSET_MAC = 77,
  PP_TRUETYPEFONTCHARSET_SHIFTJIS = 128,
  PP_TRUETYPEFONTCHARSET_HANGUL = 129,
  PP_TRUETYPEFONTCHARSET_JOHAB = 130,
  PP_TRUETYPEFONTCHARSET_GB2312 = 134,
  PP_TRUETYPEFONTCHARSET_CHINESEBIG5 = 136,
  PP_TRUETYPEFONTCHARSET_GREEK = 161,
  PP_TRUETYPEFONTCHARSET_TURKISH = 162,
  PP_TRUETYPEFONTCHARSET_VIETNAMESE = 163,
  PP_TRUETYPEFONTCHARSET_HEBREW = 177,
  PP_TRUETYPEFONTCHARSET_ARABIC = 178,
  PP_TRUETYPEFONTCHARSET_BALTIC = 186,
  PP_TRUETYPEFONTCHARSET_RUSSIAN = 204,
  PP_TRUETYPEFONTCHARSET_THAI = 222,
  PP_TRUETYPEFONTCHARSET_EASTEUROPE = 238,
  PP_TRUETYPEFONTCHARSET_OEM = 255
} PP_TrueTypeFontCharset_Dev;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_TrueTypeFontCharset_Dev, 4);

struct PP_TrueTypeFontDesc_Dev {
  struct PP_Var family;
  PP_TrueTypeFontFamily_Dev generic_family;
  
  PP_TrueTypeFontStyle_Dev style;
  
  PP_TrueTypeFontWeight_Dev weight;
  
  PP_TrueTypeFontWidth_Dev width;
  
  PP_TrueTypeFontCharset_Dev charset;
  int32_t padding;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_TrueTypeFontDesc_Dev, 40);

struct PPB_TrueTypeFont_Dev_0_1 {
  int32_t (*GetFontFamilies)(PP_Instance instance,
                             struct PP_ArrayOutput output,
                             struct PP_CompletionCallback callback);
  int32_t (*GetFontsInFamily)(PP_Instance instance,
                              struct PP_Var family,
                              struct PP_ArrayOutput output,
                              struct PP_CompletionCallback callback);
  PP_Resource (*Create)(PP_Instance instance,
                        const struct PP_TrueTypeFontDesc_Dev* desc);
  PP_Bool (*IsTrueTypeFont)(PP_Resource resource);
  int32_t (*Describe)(PP_Resource font,
                      struct PP_TrueTypeFontDesc_Dev* desc,
                      struct PP_CompletionCallback callback);
  int32_t (*GetTableTags)(PP_Resource font,
                          struct PP_ArrayOutput output,
                          struct PP_CompletionCallback callback);
  int32_t (*GetTable)(PP_Resource font,
                      uint32_t table,
                      int32_t offset,
                      int32_t max_data_length,
                      struct PP_ArrayOutput output,
                      struct PP_CompletionCallback callback);
};

typedef struct PPB_TrueTypeFont_Dev_0_1 PPB_TrueTypeFont_Dev;

#endif  


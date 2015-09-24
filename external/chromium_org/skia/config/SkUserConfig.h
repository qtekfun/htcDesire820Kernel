/*
 * Copyright (C) 2006 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SkUserConfig_DEFINED
#define SkUserConfig_DEFINED

















#define SK_ZLIB_INCLUDE "third_party/zlib/zlib.h"


#define SK_SFNTLY_SUBSETTER \
    "third_party/sfntly/cpp/src/sample/chromium/font_subsetter.h"



#ifdef SK_DEBUG
#define SK_SUPPORT_UNITTEST
#endif

#undef SK_USER_TRACE_INCLUDE_FILE


#ifdef SK_DEBUG
#define SK_REF_CNT_MIXIN_INCLUDE "sk_ref_cnt_ext_debug.h"
#else
#define SK_REF_CNT_MIXIN_INCLUDE "sk_ref_cnt_ext_release.h"
#endif

#define SK_SCALAR_IS_FLOAT
#undef SK_SCALAR_IS_FIXED

#define SK_MSCALAR_IS_FLOAT
#undef SK_MSCALAR_IS_DOUBLE

#define GR_MAX_OFFSCREEN_AA_DIM     512

#define SkDebugf(...) SkDebugf_FileLine(__FILE__, __LINE__, false, __VA_ARGS__)
SK_API void SkDebugf_FileLine(const char* file, int line, bool fatal,
                              const char* format, ...);

#define SK_DEBUGBREAK(cond) do { if (!(cond)) { \
    SkDebugf_FileLine(__FILE__, __LINE__, true, \
    "%s:%d: failed assertion \"%s\"\n", \
    __FILE__, __LINE__, #cond); } } while (false)

#if !defined(ANDROID)   
#define SK_A32_SHIFT    24
#define SK_R32_SHIFT    16
#define SK_G32_SHIFT    8
#define SK_B32_SHIFT    0
#endif

#if defined(SK_BUILD_FOR_WIN32)

#define SK_BUILD_FOR_WIN

#define bzero(str, len) memset(str, 0, len)

#elif defined(SK_BUILD_FOR_MAC)

#define SK_CPU_LENDIAN
#undef  SK_CPU_BENDIAN

#elif defined(SK_BUILD_FOR_UNIX)

#define SK_USE_FREETYPE_EMBOLDEN

#ifdef SK_CPU_BENDIAN
#error Read the comment at this location
#endif

#endif

#define SK_CRASH() SkDebugf_FileLine(__FILE__, __LINE__, true, "SK_CRASH")



#endif

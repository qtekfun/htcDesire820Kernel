/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file. */

#ifndef LIBRARIES_SDK_UTIL_MACROS_H_
#define LIBRARIES_SDK_UTIL_MACROS_H_

#define DISALLOW_COPY_AND_ASSIGN(TypeName)      \
  TypeName(const TypeName&);                    \
  void operator=(const TypeName&)

#define MEMBER_SIZE(struct_name, member) sizeof(((struct_name*)0)->member)

#ifdef __cplusplus
# define EXTERN_C_BEGIN  extern "C" {
# define EXTERN_C_END    }
#else
# define EXTERN_C_BEGIN
# define EXTERN_C_END
#endif  

#define FORCE_LINK_THIS(x) int force_link_##x = 0;
#define FORCE_LINK_THAT(x) \
  void force_link_function_##x() { \
    extern int force_link_##x; \
    force_link_##x = 1; \
  }

#endif  

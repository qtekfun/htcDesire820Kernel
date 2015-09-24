/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PP_MACROS_H_
#define PPAPI_C_PP_MACROS_H_


#define PPAPI_RELEASE 33





#if defined(__cplusplus)
# define PP_INLINE inline
#else
# if defined(_MSC_VER)
#  define PP_INLINE static __forceinline
# else
#  define PP_INLINE static __inline__
# endif
#endif

#define PP_COMPILE_ASSERT_SIZE_IN_BYTES_IMPL(NAME, CTYPENAME, SIZE) \
struct PP_Dummy_Struct_For_##NAME { \
char _COMPILE_ASSERT_FAILED_The_type_named_ \
## NAME ## _is_not_ ## SIZE ## \
_bytes_wide[(sizeof(CTYPENAME) == SIZE) ? 1 : -1]; }

#define PP_COMPILE_ASSERT_SIZE_IN_BYTES(NAME, SIZE) \
PP_COMPILE_ASSERT_SIZE_IN_BYTES_IMPL(NAME, NAME, SIZE)

#define PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(NAME, SIZE) \
PP_COMPILE_ASSERT_SIZE_IN_BYTES_IMPL(NAME, struct NAME, SIZE)

#define PP_COMPILE_ASSERT_ENUM_SIZE_IN_BYTES(NAME, SIZE) \
PP_COMPILE_ASSERT_SIZE_IN_BYTES_IMPL(NAME, enum NAME, SIZE)

#if defined(_MSC_VER)
# define PP_ALLOW_THIS_IN_INITIALIZER_LIST(code) \
    __pragma(warning(push)) \
    __pragma(warning(disable:4355)) \
    code \
    __pragma(warning(pop))
#else
# define PP_ALLOW_THIS_IN_INITIALIZER_LIST(code) code
#endif


#endif  


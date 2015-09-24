/* Copyright (c) 2010 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef PPAPI_C_DEV_DEPRECATED_BOOL_H_
#define PPAPI_C_DEV_DEPRECATED_BOOL_H_

#if !defined(__cplusplus)
# if defined(_MSC_VER) || !defined(__STDC_VERSION__) || \
    (__STDC_VERSION__ < 199901L)
typedef char bool;
#  define false 0
#  define true 1
# else
#  include <stdbool.h>
# endif
#endif


#endif  


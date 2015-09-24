/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
#ifndef TOOLS_COMMON_H_
#define TOOLS_COMMON_H_

#include <stdio.h>

#include "./vpx_config.h"

#if defined(_MSC_VER)
typedef __int64 off_t;
#define fseeko _fseeki64
#define ftello _ftelli64
#elif defined(_WIN32)
#define fseeko fseeko64
#define ftello ftello64
#define off_t off64_t
#endif  

#if CONFIG_OS_SUPPORT
#if defined(_MSC_VER)
#include <io.h>  
#define snprintf _snprintf
#define isatty   _isatty
#define fileno   _fileno
#else
#include <unistd.h>  
#endif  
#endif  

#if !CONFIG_OS_SUPPORT
typedef long off_t;  
#define fseeko fseek
#define ftello ftell
#endif  

#define LITERALU64(hi, lo) ((((uint64_t)hi) << 32) | lo)

#ifndef PATH_MAX
#define PATH_MAX 512
#endif

#define VP8_FOURCC (0x30385056)
#define VP9_FOURCC (0x30395056)
#define VP8_FOURCC_MASK (0x00385056)
#define VP9_FOURCC_MASK (0x00395056)

FILE *set_binary_mode(FILE *stream);

void die(const char *fmt, ...);
void fatal(const char *fmt, ...);
void warn(const char *fmt, ...);

void usage_exit();

#endif  

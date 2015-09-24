// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BUILD_BUILD_CONFIG_H_
#define BUILD_BUILD_CONFIG_H_

#if defined(__APPLE__)
#define OS_MACOSX 1
#elif defined(ANDROID)
#define OS_ANDROID 1
#elif defined(__linux__)
#define OS_LINUX 1
#if !defined(TOOLKIT_VIEWS)
#define TOOLKIT_GTK
#endif
#elif defined(_WIN32)
#define OS_WIN 1
#define TOOLKIT_VIEWS 1
#elif defined(__FreeBSD__)
#define OS_FREEBSD 1
#define TOOLKIT_GTK
#elif defined(__OpenBSD__)
#define OS_OPENBSD 1
#define TOOLKIT_GTK
#elif defined(__sun)
#define OS_SOLARIS 1
#define TOOLKIT_GTK
#else
#error Please add support for your platform in build/build_config.h
#endif

#if defined(OS_LINUX) || defined(OS_FREEBSD) || defined(OS_OPENBSD) || \
    defined(OS_ANDROID)
#define USE_NSS 1  
#ifndef OS_ANDROID
#define USE_X11 1  
#endif
#endif

#if defined(OS_MACOSX) || defined(OS_LINUX) || defined(OS_FREEBSD) || \
    defined(OS_OPENBSD) || defined(OS_SOLARIS) || defined(OS_ANDROID)
#define OS_POSIX 1
#define USE_BASE_DATA_PACK 1
#endif

#if defined(OS_WIN) && ! defined(NO_TCMALLOC)
#define USE_TCMALLOC 1
#endif

#if defined(__GNUC__)
#define COMPILER_GCC 1
#elif defined(_MSC_VER)
#define COMPILER_MSVC 1
#else
#error Please add support for your compiler in build/build_config.h
#endif

#if defined(_M_X64) || defined(__x86_64__)
#define ARCH_CPU_X86_FAMILY 1
#define ARCH_CPU_X86_64 1
#define ARCH_CPU_64_BITS 1
#elif defined(_M_IX86) || defined(__i386__)
#define ARCH_CPU_X86_FAMILY 1
#define ARCH_CPU_X86 1
#define ARCH_CPU_32_BITS 1
#elif defined(__ARMEL__)
#define ARCH_CPU_ARM_FAMILY 1
#define ARCH_CPU_ARMEL 1
#define ARCH_CPU_32_BITS 1
#define WCHAR_T_IS_UNSIGNED 1
#elif defined(__MIPSEL__)
#define ARCH_CPU_MIPS_FAMILY 1
#define ARCH_CPU_MIPSEL 1
#define ARCH_CPU_32_BITS 1
#define WCHAR_T_IS_UNSIGNED 0
#else
#error Please add support for your architecture in build/build_config.h
#endif

#if defined(OS_WIN)
#define WCHAR_T_IS_UTF16
#elif defined(OS_POSIX) && defined(COMPILER_GCC) && \
    defined(__WCHAR_MAX__) && \
    (__WCHAR_MAX__ == 0x7fffffff || __WCHAR_MAX__ == 0xffffffff)
#define WCHAR_T_IS_UTF32
#elif defined(OS_POSIX) && defined(COMPILER_GCC) && \
    defined(__WCHAR_MAX__) && \
    (__WCHAR_MAX__ == 0x7fff || __WCHAR_MAX__ == 0xffff)
#define WCHAR_T_IS_UTF16
#else
#error Please add support for your compiler in build/build_config.h
#endif

#endif  

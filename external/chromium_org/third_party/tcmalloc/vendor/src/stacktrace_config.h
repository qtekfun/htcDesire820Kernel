// Copyright (c) 2009, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef BASE_STACKTRACE_CONFIG_H_
#define BASE_STACKTRACE_CONFIG_H_

#if (defined(__i386__) || defined(__x86_64__)) && __GNUC__ >= 2
# if !defined(NO_FRAME_POINTER)
#   define STACKTRACE_INL_HEADER "stacktrace_x86-inl.h"
#   define STACKTRACE_SKIP_CONTEXT_ROUTINES 1
# elif defined(HAVE_LIBUNWIND_H)  
#   define STACKTRACE_INL_HEADER "stacktrace_libunwind-inl.h"
#   define STACKTRACE_USES_LIBUNWIND 1
# elif defined(__linux)
#   error Cannnot calculate stack trace: need either libunwind or frame-pointers (see INSTALL file)
# else
#   error Cannnot calculate stack trace: need libunwind (see INSTALL file)
# endif

#elif (defined(__ppc__) || defined(__PPC__)) && __GNUC__ >= 2
# if !defined(NO_FRAME_POINTER)
#   define STACKTRACE_INL_HEADER "stacktrace_powerpc-inl.h"
# else
#   define STACKTRACE_INL_HEADER "stacktrace_generic-inl.h"
# endif

#elif defined(__arm__)  && __GNUC__ >= 2
# if !defined(NO_FRAME_POINTER)
#   define STACKTRACE_INL_HEADER "stacktrace_arm-inl.h"
# else
#   error stacktrace without frame pointer is not supported on ARM
# endif

#elif defined(_WIN32) || defined(__CYGWIN__) || defined(__CYGWIN32__) || defined(__MINGW32__)
# define STACKTRACE_INL_HEADER "stacktrace_win32-inl.h"

#endif  
#endif  

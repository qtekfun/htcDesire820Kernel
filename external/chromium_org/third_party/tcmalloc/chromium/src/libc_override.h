// Copyright (c) 2011, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef TCMALLOC_LIBC_OVERRIDE_INL_H_
#define TCMALLOC_LIBC_OVERRIDE_INL_H_

#include <config.h>
#ifdef HAVE_FEATURES_H
#include <features.h>   
#endif
#include <gperftools/tcmalloc.h>

static void ReplaceSystemAlloc();  

#if defined(_WIN32) && defined(WIN32_DO_PATCHING)
void PatchWindowsFunctions();   
static void ReplaceSystemAlloc() { PatchWindowsFunctions(); }

#elif defined(_WIN32) && !defined(WIN32_DO_PATCHING)
static void ReplaceSystemAlloc() { }

#elif defined(__APPLE__)
#include "libc_override_osx.h"

#elif defined(__GLIBC__)
#include "libc_override_glibc.h"

#elif defined(__GNUC__)
#include "libc_override_gcc_and_weak.h"

#else
#error Need to add support for your libc/OS here

#endif

#endif  

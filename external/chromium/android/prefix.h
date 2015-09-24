/*
 * Copyright 2010, The Android Open Source Project
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ANDROID_CHROMIUM_PREFIX_H
#define ANDROID_CHROMIUM_PREFIX_H

#ifdef __cplusplus
#include <unistd.h>
#include <sys/prctl.h>
#include <algorithm>
using std::find;
using std::reverse;
using std::search;

#define prctl() (0)

namespace std {
inline void set_new_handler(void (*p)()) {}
}

inline size_t abs(size_t x) { return x; }
#endif

#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <ctype.h>
#include <netinet/in.h>

extern char* mkdtemp(char* path);

#define FRIEND_TEST(test_case_name, test_name)\
friend class test_case_name##_##test_name##_Test


#define U_GAVE_NL_LANGINFO_CODESET 0

#define HAVE_UINT16_T 1

#endif

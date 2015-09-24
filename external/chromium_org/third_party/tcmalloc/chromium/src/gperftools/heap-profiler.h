/* Copyright (c) 2005, Google Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ---
 * Author: Sanjay Ghemawat
 *
 * Module for heap-profiling.
 *
 * For full(er) information, see doc/heapprofile.html
 *
 * This module can be linked into your program with
 * no slowdown caused by this unless you activate the profiler
 * using one of the following methods:
 *
 *    1. Before starting the program, set the environment variable
 *       "HEAPPROFILE" to be the name of the file to which the profile
 *       data should be written.
 *
 *    2. Programmatically, start and stop the profiler using the
 *       routines "HeapProfilerStart(filename)" and "HeapProfilerStop()".
 *
 */

#ifndef BASE_HEAP_PROFILER_H_
#define BASE_HEAP_PROFILER_H_

#include <stddef.h>

#ifndef PERFTOOLS_DLL_DECL
# ifdef _WIN32
#   define PERFTOOLS_DLL_DECL  __declspec(dllimport)
# else
#   define PERFTOOLS_DLL_DECL
# endif
#endif

#if defined(_WIN64)
#pragma comment(linker, "/INCLUDE:HeapProfilerStart")
#elif defined(_WIN32)
#pragma comment(linker, "/INCLUDE:_HeapProfilerStart")
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Start profiling and arrange to write profile data to file names
 * of the form: "prefix.0000", "prefix.0001", ...
 *
 * If |prefix| is NULL then dumps will not be written to disk. Applications
 * can use GetHeapProfile() to get profile data, but HeapProfilerDump() will do
 * nothing.
 */
PERFTOOLS_DLL_DECL void HeapProfilerStart(const char* prefix);

/* Start profiling with a callback function that returns application-generated
 * stacks. Profiles are not written to disk, but may be obtained via
 * GetHeapProfile(). The callback:
 * 1. May optionally skip the first |skip_count| items on the stack.
 * 2. Must provide a |stack| buffer of at least size 32 * sizeof(void*).
 * 3. Must return the number of items copied or zero.
 */
typedef int (*StackGeneratorFunction)(int skip_count, void** stack);
PERFTOOLS_DLL_DECL void HeapProfilerWithPseudoStackStart(
    StackGeneratorFunction callback);

int IsHeapProfilerRunning();

PERFTOOLS_DLL_DECL void HeapProfilerStop();

PERFTOOLS_DLL_DECL void HeapProfilerDump(const char *reason);

PERFTOOLS_DLL_DECL char* GetHeapProfile();

typedef void (*AddressVisitor)(void* data, const void* ptr);

PERFTOOLS_DLL_DECL void IterateAllocatedObjects(AddressVisitor callback,
                                                void* data);

#ifdef __cplusplus
}  
#endif

#endif  

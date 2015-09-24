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
 * Module for CPU profiling based on periodic pc-sampling.
 *
 * For full(er) information, see doc/cpuprofile.html
 *
 * This module is linked into your program with
 * no slowdown caused by this unless you activate the profiler
 * using one of the following methods:
 *
 *    1. Before starting the program, set the environment variable
 *       "PROFILE" to be the name of the file to which the profile
 *       data should be written.
 *
 *    2. Programmatically, start and stop the profiler using the
 *       routines "ProfilerStart(filename)" and "ProfilerStop()".
 *
 *
 * (Note: if using linux 2.4 or earlier, only the main thread may be
 * profiled.)
 *
 * Use pprof to view the resulting profile output.
 *    % pprof <path_to_executable> <profile_file_name>
 *    % pprof --gv  <path_to_executable> <profile_file_name>
 *
 * These functions are thread-safe.
 */

#ifndef BASE_PROFILER_H_
#define BASE_PROFILER_H_

#include <time.h>       

#ifndef PERFTOOLS_DLL_DECL
# ifdef _WIN32
#   define PERFTOOLS_DLL_DECL  __declspec(dllimport)
# else
#   define PERFTOOLS_DLL_DECL
# endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct ProfilerOptions {
  int (*filter_in_thread)(void *arg);
  void *filter_in_thread_arg;
};

PERFTOOLS_DLL_DECL int ProfilerStart(const char* fname);

PERFTOOLS_DLL_DECL int ProfilerStartWithOptions(
    const char *fname, const struct ProfilerOptions *options);

PERFTOOLS_DLL_DECL void ProfilerStop();

PERFTOOLS_DLL_DECL void ProfilerFlush();


PERFTOOLS_DLL_DECL void ProfilerEnable();
PERFTOOLS_DLL_DECL void ProfilerDisable();

PERFTOOLS_DLL_DECL int ProfilingIsEnabledForAllThreads();

PERFTOOLS_DLL_DECL void ProfilerRegisterThread();

struct ProfilerState {
  int    enabled;             
  time_t start_time;          
  char   profile_name[1024];  /* Name of profile file being written, or '\0' */
  int    samples_gathered;    
};
PERFTOOLS_DLL_DECL void ProfilerGetCurrentState(struct ProfilerState* state);

#ifdef __cplusplus
}  
#endif

#endif  

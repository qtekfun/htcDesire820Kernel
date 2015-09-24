/* Copyright (c) 2009, Google Inc.
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
 * Author: Nabeel Mian
 *
 * This module manages the cpu profile timers and the associated interrupt
 * handler. When enabled, all registered threads in the program are profiled.
 * (Note: if using linux 2.4 or earlier, you must use the Thread class, in
 * google3/thread, to ensure all threads are profiled.)
 *
 * Any component interested in receiving a profile timer interrupt can do so by
 * registering a callback. All registered callbacks must be async-signal-safe.
 *
 * Note: This module requires the sole ownership of ITIMER_PROF timer and the
 * SIGPROF signal.
 */

#ifndef BASE_PROFILE_HANDLER_H_
#define BASE_PROFILE_HANDLER_H_

#include "config.h"
#include <signal.h>
#ifdef COMPILER_MSVC
#include "conflict-signal.h"
#endif
#include "base/basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ProfileHandlerToken;

typedef void (*ProfileHandlerCallback)(int sig, siginfo_t* sig_info,
                                       void* ucontext, void* callback_arg);

void ProfileHandlerRegisterThread();

ProfileHandlerToken* ProfileHandlerRegisterCallback(
    ProfileHandlerCallback callback, void* callback_arg);

void ProfileHandlerUnregisterCallback(ProfileHandlerToken* token);

void ProfileHandlerReset();

struct ProfileHandlerState {
  int32 frequency;  
  int32 callback_count;  
  int64 interrupts;  
  bool allowed; 
};
void ProfileHandlerGetState(struct ProfileHandlerState* state);

#ifdef __cplusplus
}  
#endif

#endif  

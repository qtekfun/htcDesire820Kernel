// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_WIN_LAUNCH_PROCESS_WITH_TOKEN_H_
#define REMOTING_HOST_WIN_LAUNCH_PROCESS_WITH_TOKEN_H_

#include <windows.h>
#include <string>

#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/lazy_instance.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "base/win/scoped_handle.h"

namespace remoting {

extern base::LazyInstance<base::Lock>::Leaky g_inherit_handles_lock;

bool CreateSessionToken(uint32 session_id, base::win::ScopedHandle* token_out);

bool LaunchProcessWithToken(const base::FilePath& binary,
                            const CommandLine::StringType& command_line,
                            HANDLE user_token,
                            SECURITY_ATTRIBUTES* process_attributes,
                            SECURITY_ATTRIBUTES* thread_attributes,
                            bool inherit_handles,
                            DWORD creation_flags,
                            const char16* desktop_name,
                            base::win::ScopedHandle* process_out,
                            base::win::ScopedHandle* thread_out);

} 

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_ANDROID_CHILD_PROCESS_LAUNCHER_ANDROID_H_
#define CONTENT_BROWSER_ANDROID_CHILD_PROCESS_LAUNCHER_ANDROID_H_

#include <jni.h>

#include "base/callback.h"
#include "base/command_line.h"
#include "base/platform_file.h"
#include "base/process/process.h"
#include "content/public/browser/file_descriptor_info.h"

namespace content {

typedef base::Callback<void(base::ProcessHandle)> StartChildProcessCallback;
void StartChildProcess(
    const CommandLine::StringVector& argv,
    const std::vector<FileDescriptorInfo>& files_to_register,
    const StartChildProcessCallback& callback);

void StopChildProcess(base::ProcessHandle handle);

bool IsChildProcessOomProtected(base::ProcessHandle handle);

bool RegisterChildProcessLauncher(JNIEnv* env);

}  

#endif  

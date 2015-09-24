// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_CONTENT_DESCRIPTORS_H_
#define CONTENT_PUBLIC_COMMON_CONTENT_DESCRIPTORS_H_

#include "ipc/ipc_descriptors.h"

enum {
  kCrashDumpSignal = kIPCDescriptorMax,
  kSandboxIPCChannel,  

#if defined(OS_ANDROID)
  kAndroidPropertyDescriptor,
#endif

  
  
  kContentIPCDescriptorMax
};

#endif  

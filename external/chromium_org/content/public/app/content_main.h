// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_APP_CONTENT_MAIN_H_
#define CONTENT_PUBLIC_APP_CONTENT_MAIN_H_

#include "build/build_config.h"
#include "content/common/content_export.h"

#if defined(OS_WIN)
#include <windows.h>
#endif

namespace sandbox {
struct SandboxInterfaceInfo;
}

namespace content {

class ContentMainDelegate;

#if defined(OS_WIN)

CONTENT_EXPORT int ContentMain(HINSTANCE instance,
                               sandbox::SandboxInterfaceInfo* sandbox_info,
                               ContentMainDelegate* delegate);
#elif defined(OS_ANDROID)
CONTENT_EXPORT void SetContentMainDelegate(ContentMainDelegate* delegate);
#else
CONTENT_EXPORT int ContentMain(int argc,
                               const char** argv,
                               ContentMainDelegate* delegate);
#endif  

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_ANDROID_CONTENT_STARTUP_FLAGS_H_
#define CONTENT_BROWSER_ANDROID_CONTENT_STARTUP_FLAGS_H_

#include <string>

namespace content {

void SetContentCommandLineFlags(int max_render_process_count,
                                const std::string& plugin_descriptor);

}  

#endif  

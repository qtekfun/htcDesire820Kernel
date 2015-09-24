// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_BRANDING_H_
#define REMOTING_HOST_BRANDING_H_

#include "base/files/file_path.h"

namespace remoting {

#if defined(OS_WIN)
extern const wchar_t kWindowsServiceName[];
#endif

base::FilePath GetConfigDir();

} 

#endif 

// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MAC_LAUNCH_SERVICES_UTIL_H_
#define BASE_MAC_LAUNCH_SERVICES_UTIL_H_

#include <ApplicationServices/ApplicationServices.h>

#include "base/base_export.h"
#include "base/command_line.h"
#include "base/files/file_path.h"

struct ProcessSerialNumber;

namespace base {
namespace mac {

BASE_EXPORT bool OpenApplicationWithPath(const base::FilePath& bundle_path,
                                         const CommandLine& command_line,
                                         LSLaunchFlags launch_flags,
                                         ProcessSerialNumber* out_psn);

}  
}  

#endif  

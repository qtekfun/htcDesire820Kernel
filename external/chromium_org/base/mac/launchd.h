// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MAC_LAUNCHD_H_
#define BASE_MAC_LAUNCHD_H_

#include <launch.h>
#include <sys/types.h>

#include <string>

#include "base/base_export.h"

namespace base {
namespace mac {

BASE_EXPORT
launch_data_t MessageForJob(const std::string& job_label,
                            const char* operation);

BASE_EXPORT
pid_t PIDForJob(const std::string& job_label);

}  
}  

#endif  

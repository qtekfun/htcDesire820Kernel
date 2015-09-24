// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_USER_AGENT_H_
#define WEBKIT_GLUE_USER_AGENT_H_

#include <string>

#include "base/basictypes.h"

namespace webkit_glue {

void BuildUserAgent(bool mimic_windows, std::string* result);

std::string BuildOSCpuInfo();

std::string GetWebKitVersion();

}  

#endif  


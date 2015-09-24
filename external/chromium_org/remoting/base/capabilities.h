// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_BASE_CAPABILITIES_H_
#define REMOTING_BASE_CAPABILITIES_H_

#include <string>

#include "base/basictypes.h"

namespace remoting {

bool HasCapability(const std::string& capabilities, const std::string& key);

std::string IntersectCapabilities(const std::string& client_capabilities,
                                  const std::string& host_capabilities);

}  

#endif  

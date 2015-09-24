// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_VARIATIONS_METRICS_UTIL_H_
#define COMPONENTS_VARIATIONS_METRICS_UTIL_H_

#include <string>

#include "base/basictypes.h"

namespace metrics {

uint32 HashName(const std::string& name);

}  

#endif  

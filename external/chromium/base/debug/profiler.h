// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_DEBUG_PROFILER_H
#define BASE_DEBUG_PROFILER_H
#pragma once

#include <string>

namespace base {
namespace debug {

void StartProfiling(const std::string& name);

void StopProfiling();

// Force data to be written to file.
void FlushProfiling();

bool BeingProfiled();

}  
}  

#endif  

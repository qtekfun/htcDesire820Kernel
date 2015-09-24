// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_BASE_TRACING_H_
#define CHROME_TEST_BASE_TRACING_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/time/time.h"

namespace tracing {

bool BeginTracing(const std::string& category_patterns) WARN_UNUSED_RESULT;

bool BeginTracingWithWatch(const std::string& category_patterns,
                           const std::string& category_name,
                           const std::string& event_name,
                           int num_occurrences) WARN_UNUSED_RESULT;

bool WaitForWatchEvent(base::TimeDelta timeout) WARN_UNUSED_RESULT;

bool EndTracing(std::string* json_trace_output) WARN_UNUSED_RESULT;

}  

#endif  

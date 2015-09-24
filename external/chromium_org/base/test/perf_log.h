// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_PERF_LOG_H_
#define BASE_TEST_PERF_LOG_H_

namespace base {

class FilePath;

bool InitPerfLog(const FilePath& log_path);
void FinalizePerfLog();

void LogPerfResult(const char* test_name, double value, const char* units);

}  

#endif  

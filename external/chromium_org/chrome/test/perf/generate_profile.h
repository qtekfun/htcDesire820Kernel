// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_PERF_GENERATE_PROFILE_H_
#define CHROME_TEST_PERF_GENERATE_PROFILE_H_

#include "base/compiler_specific.h"

namespace base {
class FilePath;
}

enum GenerateProfileTypes {
  TOP_SITES = 1 << 0
};

bool GenerateProfile(GenerateProfileTypes types,
                     int urlcount,
                     const base::FilePath& dst_dir) WARN_UNUSED_RESULT;

#endif  

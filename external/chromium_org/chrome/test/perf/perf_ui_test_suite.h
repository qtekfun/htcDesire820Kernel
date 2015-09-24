// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_PERF_PERF_UI_TEST_SUITE_H_
#define CHROME_TEST_PERF_PERF_UI_TEST_SUITE_H_

#include "base/files/scoped_temp_dir.h"
#include "chrome/test/ui/ui_test_suite.h"

namespace base {
class FilePath;
}

class PerfUITestSuite : public UITestSuite {
 public:
  PerfUITestSuite(int argc, char** argv);
  virtual ~PerfUITestSuite();

  
  enum ProfileType {
    DEFAULT_THEME = 0,
    COMPLEX_THEME = 1,
  };

  
  
  static base::FilePath GetPathForProfileType(ProfileType profile_type);

  
  virtual void Initialize() OVERRIDE;

 private:
  
  void BuildCachedThemePakIn(const base::FilePath& extension_base);

  base::ScopedTempDir default_profile_dir_;
  base::ScopedTempDir complex_profile_dir_;
};

#endif  

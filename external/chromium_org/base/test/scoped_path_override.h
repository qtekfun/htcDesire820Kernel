// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_SCOPED_PATH_OVERRIDE_H_
#define BASE_TEST_SCOPED_PATH_OVERRIDE_H_

#include "base/basictypes.h"
#include "base/files/scoped_temp_dir.h"

namespace base {

class FilePath;

class ScopedPathOverride {
 public:
  
  explicit ScopedPathOverride(int key);
  
  ScopedPathOverride(int key, const FilePath& dir);
  ~ScopedPathOverride();

 private:
  int key_;
  ScopedTempDir temp_dir_;

  DISALLOW_COPY_AND_ASSIGN(ScopedPathOverride);
};

}  

#endif  

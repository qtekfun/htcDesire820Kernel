// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_SELF_CLEANING_TEMP_DIR_H_
#define CHROME_INSTALLER_UTIL_SELF_CLEANING_TEMP_DIR_H_

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"

namespace installer {

class SelfCleaningTempDir {
 public:
  typedef base::FilePath::StringType StringType;

  SelfCleaningTempDir();

  
  ~SelfCleaningTempDir();

  
  
  bool Initialize(const base::FilePath& parent_dir,
                  const StringType& temp_name);

  
  const base::FilePath& path() const { return temp_dir_; }

  
  
  
  bool Delete();

 private:
  static void GetTopDirToCreate(const base::FilePath& temp_parent_dir,
                                base::FilePath* base_dir);

  
  base::FilePath base_dir_;

  
  base::FilePath temp_dir_;

  FRIEND_TEST_ALL_PREFIXES(SelfCleaningTempDirTest, TopLevel);
  FRIEND_TEST_ALL_PREFIXES(SelfCleaningTempDirTest, TopLevelPlusOne);
  DISALLOW_COPY_AND_ASSIGN(SelfCleaningTempDir);
};

}  

#endif  

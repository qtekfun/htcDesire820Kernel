// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_HISTORY_UNITTEST_BASE_H_
#define CHROME_BROWSER_HISTORY_HISTORY_UNITTEST_BASE_H_

#include "base/file_util.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace history {
class HistoryUnitTestBase : public testing::Test {
 public:
  virtual ~HistoryUnitTestBase();

  
  
  
  static void ExecuteSQLScript(const FilePath& sql_path,
                               const FilePath& db_path);

 protected:
  HistoryUnitTestBase();

 private:
  DISALLOW_COPY_AND_ASSIGN(HistoryUnitTestBase);
};

} 

#endif  

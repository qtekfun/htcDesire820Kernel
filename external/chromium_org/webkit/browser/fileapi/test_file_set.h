// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_TEST_FILE_SET_H_
#define WEBKIT_BROWSER_FILEAPI_TEST_FILE_SET_H_

#include <set>

#include "base/files/file_path.h"


namespace fileapi {

class FileSystemFileUtil;

namespace test {

struct TestCaseRecord {
  bool is_directory;
  const base::FilePath::CharType path[64];
  int64 data_file_size;
};

extern const TestCaseRecord kRegularTestCases[];
extern const size_t kRegularTestCaseSize;

size_t GetRegularTestCaseSize();

void SetUpOneTestCase(const base::FilePath& root_path,
                      const TestCaseRecord& record);

void SetUpRegularTestCases(const base::FilePath& root_path);

}  

}  

#endif  

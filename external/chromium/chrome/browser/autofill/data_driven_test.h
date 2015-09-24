// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_DATA_DRIVEN_TEST_H_
#define CHROME_BROWSER_AUTOFILL_DATA_DRIVEN_TEST_H_
#pragma once

#include <string>

#include "base/file_path.h"
#include "base/string16.h"

class DataDrivenTest {
 public:
  
  
  
  
  
  void RunDataDrivenTest(const FilePath& input_directory,
                         const FilePath& output_directory,
                         const FilePath::StringType& file_name_pattern);

  
  
  
  
  virtual void GenerateResults(const std::string& input,
                               std::string* output) = 0;

  
  
  FilePath GetInputDirectory(const FilePath::StringType& test_name);
  FilePath GetOutputDirectory(const FilePath::StringType& test_name);

 protected:
  DataDrivenTest();
  virtual ~DataDrivenTest();

 private:
  DISALLOW_COPY_AND_ASSIGN(DataDrivenTest);
};

#endif  

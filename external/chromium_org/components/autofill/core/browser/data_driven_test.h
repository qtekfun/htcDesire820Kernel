// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_DATA_DRIVEN_TEST_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_DATA_DRIVEN_TEST_H_

#include <string>

#include "base/files/file_path.h"
#include "base/strings/string16.h"

namespace autofill {

class DataDrivenTest {
 public:
  
  
  
  
  
  void RunDataDrivenTest(const base::FilePath& input_directory,
                         const base::FilePath& output_directory,
                         const base::FilePath::StringType& file_name_pattern);

  
  
  
  
  virtual void GenerateResults(const std::string& input,
                               std::string* output) = 0;

  
  
  base::FilePath GetInputDirectory(const base::FilePath::StringType& test_name);
  base::FilePath GetOutputDirectory(
      const base::FilePath::StringType& test_name);

 protected:
  DataDrivenTest(const base::FilePath& test_data_directory);
  virtual ~DataDrivenTest();

 private:
  base::FilePath test_data_directory_;

  DISALLOW_COPY_AND_ASSIGN(DataDrivenTest);
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_CHROMEOS_TEST_UTILS_H_
#define CHROMEOS_CHROMEOS_TEST_UTILS_H_

#include <string>

namespace base {
class FilePath;
}

namespace chromeos {
namespace test_utils {

bool GetTestDataPath(const std::string& component,
                     const std::string& filename,
                     base::FilePath* data_dir);

}  
}  

#endif  

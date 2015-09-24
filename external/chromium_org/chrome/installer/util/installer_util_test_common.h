// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_INSTALLER_UTIL_TEST_COMMON_H_
#define CHROME_INSTALLER_UTIL_INSTALLER_UTIL_TEST_COMMON_H_

namespace base {
class FilePath;
}

namespace installer {

namespace test {

bool CopyFileHierarchy(const base::FilePath& from, const base::FilePath& to);

}  

}  

#endif  

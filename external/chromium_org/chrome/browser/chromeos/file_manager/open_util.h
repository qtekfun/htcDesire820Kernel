// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FILE_MANAGER_OPEN_UTIL_H_
#define CHROME_BROWSER_CHROMEOS_FILE_MANAGER_OPEN_UTIL_H_

class Profile;

namespace base {
class FilePath;
}

namespace file_manager {
namespace util {

void OpenRemovableDrive(Profile* profile, const base::FilePath& file_path);

void OpenItem(Profile* profile, const base::FilePath& file_path);

void ShowItemInFolder(Profile* profile, const base::FilePath& file_path);

}  
}  

#endif  

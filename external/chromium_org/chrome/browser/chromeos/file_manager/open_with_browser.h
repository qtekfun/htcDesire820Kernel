// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FILE_MANAGER_OPEN_WITH_BROWSER_H_
#define CHROME_BROWSER_CHROMEOS_FILE_MANAGER_OPEN_WITH_BROWSER_H_

#include "base/files/file_path.h"

class Profile;

namespace file_manager {
namespace util {

bool OpenFileWithBrowser(Profile* profile, const base::FilePath& file_path);

bool ShouldBeOpenedWithPlugin(
    Profile* profile,
    const base::FilePath::StringType& file_extension);

}  
}  

#endif  

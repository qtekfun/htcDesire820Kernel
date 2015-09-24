// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_APP_CHROME_MAIN_MAC_H_
#define CHROME_APP_CHROME_MAIN_MAC_H_

namespace base {
class FilePath;
}

void CheckUserDataDirPolicy(base::FilePath* user_data_dir);

void SetUpBundleOverrides();

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_USER_DATA_DIR_DIALOG_H_
#define CHROME_BROWSER_UI_USER_DATA_DIR_DIALOG_H_

namespace base {
class FilePath;
}

namespace chrome {

base::FilePath ShowUserDataDirDialog(const base::FilePath& user_data_dir);

}  

#endif  

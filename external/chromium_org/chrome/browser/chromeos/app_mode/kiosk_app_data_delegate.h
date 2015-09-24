// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_APP_MODE_KIOSK_APP_DATA_DELEGATE_H_
#define CHROME_BROWSER_CHROMEOS_APP_MODE_KIOSK_APP_DATA_DELEGATE_H_

#include <string>

namespace base {
class FilePath;
}

namespace chromeos {

class KioskAppDataDelegate {
 public:
  
  virtual void GetKioskAppIconCacheDir(base::FilePath* cache_dir) = 0;

  
  virtual void OnKioskAppDataChanged(const std::string& app_id) = 0;

  
  virtual void OnKioskAppDataLoadFailure(const std::string& app_id) = 0;

 protected:
  virtual ~KioskAppDataDelegate() {}
};

}  

#endif  

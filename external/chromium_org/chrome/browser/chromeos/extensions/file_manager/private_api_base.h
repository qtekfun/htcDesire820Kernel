// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTENSIONS_FILE_MANAGER_PRIVATE_API_BASE_H_
#define CHROME_BROWSER_CHROMEOS_EXTENSIONS_FILE_MANAGER_PRIVATE_API_BASE_H_

#include "base/time/time.h"
#include "chrome/browser/extensions/chrome_extension_function.h"

namespace extensions {

class LoggedAsyncExtensionFunction : public ChromeAsyncExtensionFunction {
 public:
  LoggedAsyncExtensionFunction();

 protected:
  virtual ~LoggedAsyncExtensionFunction();

  
  virtual void SendResponse(bool success) OVERRIDE;

  
  void set_log_on_completion(bool log_on_completion) {
    log_on_completion_ = log_on_completion;
  }

 private:
  base::Time start_time_;
  bool log_on_completion_;
};

}  

#endif  

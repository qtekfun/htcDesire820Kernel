// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_ERROR_REPORTER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_ERROR_REPORTER_H_

#include <string>
#include <vector>

#include "base/strings/string16.h"

namespace base {
class MessageLoop;
}

class ExtensionErrorReporter {
 public:
  
  
  static void Init(bool enable_noisy_errors);

  
  static ExtensionErrorReporter* GetInstance();

  
  
  void ReportError(const base::string16& message, bool be_noisy);

  
  const std::vector<base::string16>* GetErrors();

  
  void ClearErrors();

 private:
  static ExtensionErrorReporter* instance_;

  explicit ExtensionErrorReporter(bool enable_noisy_errors);
  ~ExtensionErrorReporter();

  base::MessageLoop* ui_loop_;
  std::vector<base::string16> errors_;
  bool enable_noisy_errors_;
};

#endif  

// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_ERROR_REPORTER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_ERROR_REPORTER_H_
#pragma once

#include <string>
#include <vector>

class MessageLoop;

class ExtensionErrorReporter {
 public:
  
  
  static void Init(bool enable_noisy_errors);

  
  static ExtensionErrorReporter* GetInstance();

  
  
  void ReportError(const std::string& message, bool be_noisy);

  
  const std::vector<std::string>* GetErrors();

  
  void ClearErrors();

 private:
  static ExtensionErrorReporter* instance_;

  explicit ExtensionErrorReporter(bool enable_noisy_errors);
  ~ExtensionErrorReporter();

  MessageLoop* ui_loop_;
  std::vector<std::string> errors_;
  bool enable_noisy_errors_;
};

#endif  

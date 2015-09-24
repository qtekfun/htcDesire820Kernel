// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_INPUT_API_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_INPUT_API_H_
#pragma once

#include "chrome/browser/extensions/extension_function.h"

namespace views {
  class RootView;
}  

class InputFunction : public AsyncExtensionFunction {
 public:
  virtual void Run();
  virtual bool RunImpl() = 0;
};

class SendKeyboardEventInputFunction : public InputFunction {
 public:
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("experimental.input.sendKeyboardEvent");

 private:
  views::RootView* GetRootView();
};

#endif  

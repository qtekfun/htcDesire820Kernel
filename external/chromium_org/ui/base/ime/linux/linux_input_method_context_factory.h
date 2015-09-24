// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_LINUX_LINUX_INPUT_METHOD_CONTEXT_FACTORY_H_
#define UI_BASE_IME_LINUX_LINUX_INPUT_METHOD_CONTEXT_FACTORY_H_

#include "base/memory/scoped_ptr.h"
#include "ui/base/ui_export.h"

namespace ui {

class LinuxInputMethodContext;
class LinuxInputMethodContextDelegate;

class UI_EXPORT LinuxInputMethodContextFactory {
 public:
  
  static const LinuxInputMethodContextFactory* instance();

  
  
  
  static void SetInstance(const LinuxInputMethodContextFactory* instance);

  virtual ~LinuxInputMethodContextFactory() {}

  
  virtual scoped_ptr<LinuxInputMethodContext> CreateInputMethodContext(
      LinuxInputMethodContextDelegate* delegate) const = 0;
};

}  

#endif  

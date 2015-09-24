// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_LINUX_FAKE_INPUT_METHOD_CONTEXT_FACTORY_H_
#define UI_BASE_IME_LINUX_FAKE_INPUT_METHOD_CONTEXT_FACTORY_H_

#include "ui/base/ime/linux/linux_input_method_context_factory.h"

namespace ui {

class FakeInputMethodContextFactory : public LinuxInputMethodContextFactory {
 public:
  FakeInputMethodContextFactory();

  
  virtual scoped_ptr<LinuxInputMethodContext> CreateInputMethodContext(
      LinuxInputMethodContextDelegate* delegate) const OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(FakeInputMethodContextFactory);
};

}  

#endif  

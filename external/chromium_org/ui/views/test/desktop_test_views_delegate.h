// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_TEST_DESKTOP_TEST_VIEWS_DELEGATE_H_
#define UI_VIEWS_TEST_DESKTOP_TEST_VIEWS_DELEGATE_H_

#include "ui/views/test/test_views_delegate.h"

namespace views {

// Most aura test code is written assuming a single RootWindow view, however,
class DesktopTestViewsDelegate : public TestViewsDelegate {
 public:
  DesktopTestViewsDelegate();
  virtual ~DesktopTestViewsDelegate();

  
  virtual void OnBeforeWidgetInit(Widget::InitParams* params,
                            internal::NativeWidgetDelegate* delegate) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(DesktopTestViewsDelegate);
};

}  

#endif

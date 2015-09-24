// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_APPS_NATIVE_APP_WINDOW_VIEWS_WIN_H_
#define CHROME_BROWSER_UI_VIEWS_APPS_NATIVE_APP_WINDOW_VIEWS_WIN_H_

#include "chrome/browser/ui/views/apps/native_app_window_views.h"

class NativeAppWindowViewsWin : public NativeAppWindowViews {
 public:
  NativeAppWindowViewsWin();

 private:
  void ActivateParentDesktopIfNecessary();

  
  virtual void OnBeforeWidgetInit(views::Widget::InitParams* init_params,
                                  views::Widget* widget) OVERRIDE;

  
  virtual void Show() OVERRIDE;
  virtual void Activate() OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(NativeAppWindowViewsWin);
};

#endif  

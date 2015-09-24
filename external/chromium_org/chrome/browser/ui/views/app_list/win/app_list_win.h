// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_APP_LIST_WIN_APP_LIST_WIN_H_
#define CHROME_BROWSER_UI_VIEWS_APP_LIST_WIN_APP_LIST_WIN_H_

#include "base/callback_forward.h"
#include "chrome/browser/ui/app_list/app_list.h"
#include "chrome/browser/ui/views/app_list/win/activation_tracker_win.h"
#include "ui/app_list/views/app_list_view.h"

namespace gfx {
class Display;
class Point;
class Size;
}  

class AppListWin : public AppList {
 public:
  AppListWin(app_list::AppListView* view,
             const base::Closure& on_should_dismiss);
  virtual ~AppListWin();

  
  
  
  
  
  static gfx::Point FindAnchorPoint(const gfx::Size& view_size,
                                    const gfx::Display& display,
                                    const gfx::Point& cursor,
                                    const gfx::Rect& taskbar_rect);

  
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void MoveNearCursor() OVERRIDE;
  virtual bool IsVisible() OVERRIDE;
  virtual void Prerender() OVERRIDE;
  virtual void ReactivateOnNextFocusLoss() OVERRIDE;
  virtual gfx::NativeWindow GetWindow() OVERRIDE;
  virtual void SetProfile(Profile* profile) OVERRIDE;

 private:
  
  app_list::AppListView* view_;
  ActivationTrackerWin activation_tracker_;
  bool window_icon_updated_;

  DISALLOW_COPY_AND_ASSIGN(AppListWin);
};

#endif  

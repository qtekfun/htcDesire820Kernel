// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_APP_LIST_LINUX_APP_LIST_LINUX_H_
#define CHROME_BROWSER_UI_VIEWS_APP_LIST_LINUX_APP_LIST_LINUX_H_

#include "base/callback.h"
#include "chrome/browser/ui/app_list/app_list.h"
#include "chrome/browser/ui/app_list/app_list_positioner.h"
#include "ui/app_list/views/app_list_view_observer.h"

namespace app_list {
class AppListView;
}

namespace gfx {
class Display;
class Point;
class Size;
}  

class AppListLinux : public AppList,
                     public app_list::AppListViewObserver {
 public:
  AppListLinux(app_list::AppListView* view,
               const base::Closure& on_should_dismiss);
  virtual ~AppListLinux();

  
  
  
  
  
  static gfx::Point FindAnchorPoint(const gfx::Size& view_size,
                                    const gfx::Display& display,
                                    const gfx::Point& cursor,
                                    AppListPositioner::ScreenEdge edge);

  
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void MoveNearCursor() OVERRIDE;
  virtual bool IsVisible() OVERRIDE;
  virtual void Prerender() OVERRIDE;
  virtual void ReactivateOnNextFocusLoss() OVERRIDE;
  virtual gfx::NativeWindow GetWindow() OVERRIDE;
  virtual void SetProfile(Profile* profile) OVERRIDE;

  
  virtual void OnActivationChanged(views::Widget* widget, bool active) OVERRIDE;

 private:
  
  app_list::AppListView* view_;
  bool window_icon_updated_;

  
  base::Closure on_should_dismiss_;

  DISALLOW_COPY_AND_ASSIGN(AppListLinux);
};

#endif  

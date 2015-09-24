// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_APP_LIST_WIN_ACTIVATION_TRACKER_WIN_H_
#define CHROME_BROWSER_UI_VIEWS_APP_LIST_WIN_ACTIVATION_TRACKER_WIN_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/timer/timer.h"
#include "ui/app_list/views/app_list_view_observer.h"

namespace app_list {
class AppListView;
}

class ActivationTrackerWin : public app_list::AppListViewObserver {
 public:
  ActivationTrackerWin(app_list::AppListView* view,
                       const base::Closure& on_should_dismiss);
  ~ActivationTrackerWin();

  void ReactivateOnNextFocusLoss() {
    reactivate_on_next_focus_loss_ = true;
  }

  
  virtual void OnActivationChanged(views::Widget* widget, bool active) OVERRIDE;

  void OnViewHidden();

 private:
  
  
  
  
  void MaybeDismissAppList();

  
  
  
  
  bool ShouldDismissAppList();

  
  app_list::AppListView* view_;

  
  base::Closure on_should_dismiss_;

  
  
  
  bool reactivate_on_next_focus_loss_;

  
  
  
  
  
  bool taskbar_has_focus_;

  
  
  
  base::RepeatingTimer<ActivationTrackerWin> timer_;

  DISALLOW_COPY_AND_ASSIGN(ActivationTrackerWin);
};

#endif  

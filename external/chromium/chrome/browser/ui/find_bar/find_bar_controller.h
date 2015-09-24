// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_FIND_BAR_FIND_BAR_CONTROLLER_H_
#define CHROME_BROWSER_UI_FIND_BAR_FIND_BAR_CONTROLLER_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

namespace gfx {
class Rect;
}

class FindBar;
class TabContentsWrapper;

class FindBarController : public NotificationObserver {
 public:
  
  enum SelectionAction {
    kKeepSelection,  
    kClearSelection,  
    kActivateSelection  
  };

  
  explicit FindBarController(FindBar* find_bar);

  virtual ~FindBarController();

  
  void Show();

  
  void EndFindSession(SelectionAction action);

  
  TabContentsWrapper* tab_contents() const { return tab_contents_; }

  
  
  void ChangeTabContents(TabContentsWrapper* contents);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  FindBar* find_bar() const { return find_bar_.get(); }

  
  
  static gfx::Rect GetLocationForFindbarView(
      gfx::Rect view_location,
      const gfx::Rect& dialog_bounds,
      const gfx::Rect& avoid_overlapping_rect);

 private:
  
  
  
  void UpdateFindBarForCurrentResult();

  
  
  
  
  
  void MaybeSetPrepopulateText();

  NotificationRegistrar registrar_;

  scoped_ptr<FindBar> find_bar_;

  
  TabContentsWrapper* tab_contents_;

  
  
  int last_reported_matchcount_;

  DISALLOW_COPY_AND_ASSIGN(FindBarController);
};

#endif  

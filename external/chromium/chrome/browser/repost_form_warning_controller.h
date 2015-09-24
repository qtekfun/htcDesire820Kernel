// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_REPOST_FORM_WARNING_CONTROLLER_H_
#define CHROME_BROWSER_REPOST_FORM_WARNING_CONTROLLER_H_
#pragma once

#include "content/browser/tab_contents/constrained_window.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class TabContents;

class RepostFormWarningController : public NotificationObserver {
 public:
  explicit RepostFormWarningController(TabContents* tab_contents);
  virtual ~RepostFormWarningController();

  
  void Show(ConstrainedWindowDelegate* window_delegate);

  
  void Cancel();

  
  void Continue();

 private:
  
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  void CloseDialog();

  NotificationRegistrar registrar_;

  
  TabContents* tab_contents_;

  ConstrainedWindow* window_;

  DISALLOW_COPY_AND_ASSIGN(RepostFormWarningController);
};

#endif  

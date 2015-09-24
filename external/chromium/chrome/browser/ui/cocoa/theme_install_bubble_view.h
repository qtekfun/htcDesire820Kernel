// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_THEME_INSTALL_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_THEME_INSTALL_BUBBLE_VIEW_H_
#pragma once

#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

@class NSWindow;
@class ThemeInstallBubbleViewCocoa;

class ThemeInstallBubbleView : public NotificationObserver {
 public:
  ~ThemeInstallBubbleView();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  static void Show(NSWindow* window);

 private:
  explicit ThemeInstallBubbleView(NSWindow* window);

  
  static ThemeInstallBubbleView* view_;

  
  NotificationRegistrar registrar_;

  
  void Close();

  
  ThemeInstallBubbleViewCocoa* cocoa_view_;

  
  
  int num_loads_extant_;

  DISALLOW_COPY_AND_ASSIGN(ThemeInstallBubbleView);
};

#endif  

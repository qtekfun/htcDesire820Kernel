// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_THEME_INSTALL_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_THEME_INSTALL_BUBBLE_VIEW_H_
#pragma once

#include "base/string16.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/gfx/canvas.h"
#include "views/controls/label.h"

class TabContents;

namespace views {
class Widget;
}

class ThemeInstallBubbleView : public NotificationObserver,
                               public views::Label {
 public:
  virtual ~ThemeInstallBubbleView();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  static void Show(TabContents* tab_contents);

 private:
  explicit ThemeInstallBubbleView(TabContents* tab_contents);

  
  void Reposition();

  
  virtual gfx::Size GetPreferredSize();

  
  void Close();

  virtual void OnPaint(gfx::Canvas* canvas);

  
  gfx::Rect tab_contents_bounds_;

  
  views::Widget* popup_;

  
  string16 text_;

  
  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(ThemeInstallBubbleView);
};

#endif  

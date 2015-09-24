// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_OUTDATED_UPGRADE_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_OUTDATED_UPGRADE_BUBBLE_VIEW_H_

#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/controls/button/button.h"

namespace views {
class LabelButton;
}

namespace content {
class PageNavigator;
}

class OutdatedUpgradeBubbleView : public views::BubbleDelegateView,
                                  public views::ButtonListener {
 public:
  static void ShowBubble(views::View* anchor_view,
                         content::PageNavigator* navigator);

  
  
  static bool IsAvailable();

  
  virtual views::View* GetInitiallyFocusedView() OVERRIDE;

  
  virtual void WindowClosing() OVERRIDE;

 private:
  OutdatedUpgradeBubbleView(views::View* anchor_view,
                            content::PageNavigator* navigator);
  virtual ~OutdatedUpgradeBubbleView();

  static bool IsShowing() { return upgrade_bubble_ != NULL; }

  
  virtual void Init() OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  void HandleButtonPressed(views::Button* sender);

  
  static OutdatedUpgradeBubbleView* upgrade_bubble_;

  
  
  static int num_ignored_bubbles_;

  
  bool chose_to_reinstall_;

  
  views::LabelButton* reinstall_button_;

  
  views::LabelButton* later_button_;

  
  content::PageNavigator* navigator_;

  DISALLOW_COPY_AND_ASSIGN(OutdatedUpgradeBubbleView);
};

#endif  

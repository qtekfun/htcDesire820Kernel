// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_CRITICAL_NOTIFICATION_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_CRITICAL_NOTIFICATION_BUBBLE_VIEW_H_

#include "base/timer/timer.h"
#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/controls/button/button.h"

namespace ui {
class Accelerator;
}

namespace views {
class Label;
class LabelButton;
}

class CriticalNotificationBubbleView : public views::BubbleDelegateView,
                                       public views::ButtonListener {
 public:
  explicit CriticalNotificationBubbleView(views::View* anchor_view);
  virtual ~CriticalNotificationBubbleView();

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void WindowClosing() OVERRIDE;

  
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;

 protected:
  
  virtual bool AcceleratorPressed(
      const ui::Accelerator& accelerator) OVERRIDE;
  virtual void Init() OVERRIDE;

 private:
  
  
  int GetRemainingTime();

  
  void UpdateBubbleHeadline(int seconds);

  
  void OnCountdown();

  
  views::Label* headline_;
  views::LabelButton* restart_button_;
  views::LabelButton* dismiss_button_;

  
  base::RepeatingTimer<CriticalNotificationBubbleView> refresh_timer_;

  
  base::Time bubble_created_;

  DISALLOW_COPY_AND_ASSIGN(CriticalNotificationBubbleView);
};

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_VIEWS_MESSAGE_CENTER_BUTTON_BAR_H_
#define UI_MESSAGE_CENTER_VIEWS_MESSAGE_CENTER_BUTTON_BAR_H_

#include "ui/views/controls/button/button.h"
#include "ui/views/controls/button/image_button.h"
#include "ui/views/view.h"

namespace views {
class Label;
}

namespace message_center {

class ButtonBarSettingsLabel;
class MessageCenter;
class MessageCenterTray;
class MessageCenterView;
class NotificationCenterButton;
class NotifierSettingsProvider;

class MessageCenterButtonBar : public views::View,
                               public views::ButtonListener {
 public:
  MessageCenterButtonBar(MessageCenterView* message_center_view,
                         MessageCenter* message_center,
                         NotifierSettingsProvider* notifier_settings_provider,
                         bool settings_initially_visible);
  virtual ~MessageCenterButtonBar();

  
  
  virtual void SetAllButtonsEnabled(bool enabled);

  
  void SetCloseAllButtonEnabled(bool enabled);

  
  void SetBackArrowVisible(bool visible);

 private:
  
  
  void ViewVisibilityChanged();

  
  virtual void ChildVisibilityChanged(views::View* child) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  MessageCenterView* message_center_view() const {
    return message_center_view_;
  }
  MessageCenter* message_center() const { return message_center_; }

  MessageCenterView* message_center_view_;  
  MessageCenter* message_center_;           

  
  NotificationCenterButton* title_arrow_;
  views::Label* notification_label_;
  views::View* button_container_;
  NotificationCenterButton* close_all_button_;
  NotificationCenterButton* settings_button_;
  NotificationCenterButton* quiet_mode_button_;

  DISALLOW_COPY_AND_ASSIGN(MessageCenterButtonBar);
};

}  

#endif  

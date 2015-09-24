// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_VIEWS_NOTIFICATION_VIEW_H_
#define UI_MESSAGE_CENTER_VIEWS_NOTIFICATION_VIEW_H_

#include <vector>

#include "ui/message_center/message_center_export.h"
#include "ui/message_center/views/message_view.h"

namespace views {
class ProgressBar;
}

namespace message_center {

class BoundedLabel;
class MessageCenter;
class MessageCenterController;
class NotificationView;
class PaddedButton;

class MESSAGE_CENTER_EXPORT NotificationView : public MessageView,
                                               public MessageViewController {
 public:
  
  
  
  
  
  
  
  static NotificationView* Create(MessageCenterController* controller,
                                  const Notification& notification,
                                  bool expanded,
                                  bool top_level);

    virtual ~NotificationView();

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual int GetHeightForWidth(int width) OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void ScrollRectToVisible(const gfx::Rect& rect) OVERRIDE;
  virtual views::View* GetEventHandlerForRect(const gfx::Rect& rect) OVERRIDE;
  virtual gfx::NativeCursor GetCursor(const ui::MouseEvent& event) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void ClickOnNotification(const std::string& notification_id) OVERRIDE;
  virtual void RemoveNotification(const std::string& notification_id,
                                  bool by_user) OVERRIDE;
  virtual void DisableNotificationsFromThisSource(
      const NotifierId& notifier_id) OVERRIDE;
  virtual void ShowNotifierSettingsBubble() OVERRIDE;

  void set_controller(MessageCenterController* controller) {
    controller_ = controller;
  }

 protected:
  NotificationView(MessageCenterController* controller,
                   const Notification& notification,
                   bool expanded);

 private:
  bool IsExpansionNeeded(int width);
  bool IsMessageExpansionNeeded(int width);
  int GetMessageLineLimit(int width);
  int GetMessageLines(int width, int limit);
  int GetMessageHeight(int width, int limit);

  MessageCenterController* controller_;  

  
  bool clickable_;
  bool is_expanded_;

  
  views::View* background_view_;
  views::View* top_view_;
  BoundedLabel* title_view_;
  BoundedLabel* message_view_;
  BoundedLabel* context_message_view_;
  std::vector<views::View*> item_views_;
  views::View* icon_view_;
  views::View* bottom_view_;
  views::View* image_view_;
  views::ProgressBar* progress_bar_view_;
  std::vector<views::View*> action_buttons_;
  PaddedButton* expand_button_;

  DISALLOW_COPY_AND_ASSIGN(NotificationView);
};

}  

#endif 

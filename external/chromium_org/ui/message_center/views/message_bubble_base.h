// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_VIEWS_MESSAGE_BUBBLE_BASE_H_
#define UI_MESSAGE_CENTER_VIEWS_MESSAGE_BUBBLE_BASE_H_

#include "base/memory/scoped_ptr.h"
#include "ui/message_center/message_center.h"
#include "ui/message_center/message_center_export.h"
#include "ui/views/bubble/tray_bubble_view.h"

namespace message_center {
class MessageCenterTray;

class MESSAGE_CENTER_EXPORT MessageBubbleBase {
 public:
  MessageBubbleBase(MessageCenter* message_center, MessageCenterTray* tray);

  virtual ~MessageBubbleBase();

  
  
  void BubbleViewDestroyed();

  
  
  
  void SetMaxHeight(int height);
  int max_height() const { return max_height_; }

  
  virtual views::TrayBubbleView::InitParams GetInitParams(
      views::TrayBubbleView::AnchorAlignment anchor_alignment) = 0;

  
  
  virtual void InitializeContents(views::TrayBubbleView* bubble_view) = 0;

  
  virtual void OnBubbleViewDestroyed() = 0;

  
  virtual void UpdateBubbleView() = 0;

  
  virtual void OnMouseEnteredView() = 0;
  virtual void OnMouseExitedView() = 0;

  
  
  void ScheduleUpdate();

  bool IsVisible() const;

  views::TrayBubbleView* bubble_view() const { return bubble_view_; }

  static const SkColor kBackgroundColor;

 protected:
  views::TrayBubbleView::InitParams GetDefaultInitParams(
      views::TrayBubbleView::AnchorAlignment anchor_alignment);
  MessageCenter* message_center() { return message_center_; }
  MessageCenterTray* tray() { return tray_; }
  void set_bubble_view(views::TrayBubbleView* bubble_view) {
    bubble_view_ = bubble_view;
  }

 private:
  MessageCenter* message_center_;
  MessageCenterTray* tray_;
  views::TrayBubbleView* bubble_view_;
  base::WeakPtrFactory<MessageBubbleBase> weak_ptr_factory_;
  int max_height_;

  DISALLOW_COPY_AND_ASSIGN(MessageBubbleBase);
};

}  

#endif 

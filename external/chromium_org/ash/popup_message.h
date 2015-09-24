// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_POPUP_MESSAGE_H_
#define ASH_POPUP_MESSAGE_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "ui/gfx/rect.h"
#include "ui/views/bubble/bubble_border.h"

namespace views {
class BubbleDelegateView;
class Widget;
}

namespace ash {

class ASH_EXPORT PopupMessage {
 public:
  enum IconType {
    ICON_WARNING,
    ICON_NONE
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  PopupMessage(const base::string16& caption,
               const base::string16& message,
               IconType message_type,
               views::View* anchor,
               views::BubbleBorder::Arrow arrow,
               const gfx::Size& size_override,
               int arrow_offset);
  
  
  virtual ~PopupMessage();

  
  void Close();

 private:
  class MessageBubble;

  void CancelHidingAnimation();

  MessageBubble* view_;
  views::Widget* widget_;

  
  views::View* anchor_;
  base::string16 caption_;
  base::string16 message_;
  IconType message_type_;
  views::BubbleBorder::Arrow arrow_orientation_;

  DISALLOW_COPY_AND_ASSIGN(PopupMessage);
};

}  

#endif  

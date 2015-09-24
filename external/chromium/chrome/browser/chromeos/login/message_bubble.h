// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_MESSAGE_BUBBLE_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_MESSAGE_BUBBLE_H_
#pragma once

#include "chrome/browser/ui/views/bubble/bubble.h"
#include "views/controls/button/button.h"
#include "views/controls/link.h"
#include "views/view.h"
#include "views/widget/widget_gtk.h"

class SkBitmap;

namespace views {
class ImageButton;
class ImageView;
class Label;
}

namespace chromeos {

class MessageBubbleDelegate : public BubbleDelegate {
 public:
  
  virtual void OnHelpLinkActivated() = 0;
};

class MessageBubble : public Bubble,
                      public views::ButtonListener,
                      public views::LinkController {
 public:
  
  static MessageBubble* Show(views::Widget* parent,
                             const gfx::Rect& position_relative_to,
                             BubbleBorder::ArrowLocation arrow_location,
                             SkBitmap* image,
                             const std::wstring& text,
                             const std::wstring& help,
                             MessageBubbleDelegate* delegate);

  
  
  
  static MessageBubble* ShowNoGrab(views::Widget* parent,
                                   const gfx::Rect& position_relative_to,
                                   BubbleBorder::ArrowLocation arrow_location,
                                   SkBitmap* image,
                                   const std::wstring& text,
                                   const std::wstring& help,
                                   MessageBubbleDelegate* delegate);

  
  virtual void Close() OVERRIDE;

  virtual gboolean OnButtonPress(GtkWidget* widget, GdkEventButton* event) {
    WidgetGtk::OnButtonPress(widget, event);
    
    return true;
  }

 protected:
  
  virtual void ButtonPressed(views::Button* sender,
                             const views::Event& event);

  
  virtual void LinkActivated(views::Link* source, int event_flags);

  
  virtual void IsActiveChanged();
  virtual void SetMouseCapture();

 private:
  MessageBubble(views::WidgetGtk::Type type,
                views::Widget* parent,
                SkBitmap* image,
                const std::wstring& text,
                const std::wstring& help,
                bool grab_enabled,
                MessageBubbleDelegate* delegate);

  views::Widget* parent_;
  views::ImageView* icon_;
  views::Label* text_;
  views::ImageButton* close_button_;
  views::Link* help_link_;
  MessageBubbleDelegate* message_delegate_;
  bool grab_enabled_;

  DISALLOW_COPY_AND_ASSIGN(MessageBubble);
};

}  

#endif  

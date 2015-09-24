// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_VALIDATION_MESSAGE_BUBBLE_DELEGATE_H_
#define CHROME_BROWSER_UI_VIEWS_VALIDATION_MESSAGE_BUBBLE_DELEGATE_H_

#include "ui/views/bubble/bubble_delegate.h"

class ValidationMessageBubbleDelegate : public views::BubbleDelegateView {
 public:
  
  class Observer {
   public:
    virtual void WindowClosing() = 0;

   protected:
    virtual ~Observer() {}
  };

  static const int kWindowMinWidth;
  static const int kWindowMaxWidth;

  ValidationMessageBubbleDelegate(const gfx::Rect& anchor_in_screen,
                                  const base::string16& main_text,
                                  const base::string16& sub_text,
                                  Observer* observer);
  virtual ~ValidationMessageBubbleDelegate();

  void Close();
  void SetPositionRelativeToAnchor(const gfx::Rect& anchor_in_screen);

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void DeleteDelegate() OVERRIDE;
  virtual void WindowClosing() OVERRIDE;

 private:
  Observer* observer_;
  int width_;
  int height_;

  DISALLOW_COPY_AND_ASSIGN(ValidationMessageBubbleDelegate);
};

#endif  

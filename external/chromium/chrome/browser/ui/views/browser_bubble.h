// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_BROWSER_BUBBLE_H_
#define CHROME_BROWSER_UI_VIEWS_BROWSER_BUBBLE_H_
#pragma once

#include "chrome/browser/ui/views/bubble/bubble_border.h"
#include "views/view.h"
#include "views/widget/widget.h"

class BrowserBubbleHost;

class BrowserBubble {
 public:
  
  class Delegate {
   public:
    
    virtual void BubbleBrowserWindowMoved(BrowserBubble* bubble) {}

    
    
    virtual void BubbleBrowserWindowClosing(BrowserBubble* bubble) {}

    
    virtual void BubbleGotFocus(BrowserBubble* bubble) {}

    
    
    virtual void BubbleLostFocus(BrowserBubble* bubble,
                                 bool lost_focus_to_child) {}
  };

  
  
  
  
  
  
  
  BrowserBubble(views::View* view,
                views::Widget* frame,
                const gfx::Rect& relative_to,
                BubbleBorder::ArrowLocation arrow_location);
  virtual ~BrowserBubble();

  
  
  
  void DetachFromBrowser();

  
  
  void AttachToBrowser();
  bool attached() const { return attached_; }

  
  Delegate* delegate() const { return delegate_; }
  void set_delegate(Delegate* del) { delegate_ = del; }

  
  
  virtual void BrowserWindowMoved();
  virtual void BrowserWindowClosing();

  
  virtual void Show(bool activate);
  virtual void Hide();
  bool visible() const { return visible_; }

  
  views::View* view() const { return view_; }

  
  void SetBounds(int x, int y, int w, int h);
  void MoveTo(int x, int y);
  int width() { return bounds_.width(); }
  int height() { return bounds_.height(); }
  const gfx::Rect& bounds() const { return bounds_; }

  
  
  void Reposition();

  
  void ResizeToView();

  
  gfx::NativeView native_view() const { return popup_->GetNativeView(); }

 protected:
  
  virtual void InitPopup();

  
  gfx::Rect GetAbsoluteRelativeTo();

  
  void SetAbsoluteBounds(const gfx::Rect& window_bounds);

  
  void MovePopup(int x, int y, int w, int h);

  
  views::Widget* popup_;

  
  views::Widget* frame_;

 private:
  
  views::View* view_;

  
  gfx::Rect relative_to_;

  
  BubbleBorder::ArrowLocation arrow_location_;

  
  gfx::Rect bounds_;

  
  bool visible_;

  
  Delegate* delegate_;

  
  bool attached_;

  
  BrowserBubbleHost* bubble_host_;

  DISALLOW_COPY_AND_ASSIGN(BrowserBubble);
};

#endif  

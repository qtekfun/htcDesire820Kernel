// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FIRST_RUN_BUBBLE_H_
#define CHROME_BROWSER_UI_VIEWS_FIRST_RUN_BUBBLE_H_

#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/controls/link_listener.h"

class Browser;

class FirstRunBubble : public views::BubbleDelegateView,
                       public views::LinkListener {
 public:
  
  static FirstRunBubble* ShowBubble(Browser* browser, views::View* anchor_view);

 protected:
  
  virtual void Init() OVERRIDE;

 private:
  FirstRunBubble(Browser* browser, views::View* anchor_view);
  virtual ~FirstRunBubble();

  
  virtual void LinkClicked(views::Link* source, int event_flags) OVERRIDE;

  Browser* browser_;

  DISALLOW_COPY_AND_ASSIGN(FirstRunBubble);
};

#endif  

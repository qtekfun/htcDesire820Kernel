// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_BUBBLE_HOST_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_BUBBLE_HOST_H_
#pragma once

#include <set>

#include "base/basictypes.h"

class BrowserBubble;

class BrowserBubbleHost {
 public:
  BrowserBubbleHost();
  ~BrowserBubbleHost();

  
  
  void WindowMoved();

  
  
  void Close();

  
  
  void AttachBrowserBubble(BrowserBubble* bubble);
  void DetachBrowserBubble(BrowserBubble* bubble);

 private:
  
  typedef std::set<BrowserBubble*> BubbleSet;
  BubbleSet browser_bubbles_;

  DISALLOW_COPY_AND_ASSIGN(BrowserBubbleHost);
};

#endif  

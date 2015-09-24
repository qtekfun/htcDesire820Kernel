// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_STATUS_BUBBLE_H_
#define CHROME_BROWSER_UI_STATUS_BUBBLE_H_

#include <string>

#include "base/strings/string16.h"

class GURL;
namespace gfx {
class Point;
}

class StatusBubble {
 public:
  
  static const int kExpandHoverDelay = 1600;

  virtual ~StatusBubble() {}

  
  
  
  
  
  virtual void SetStatus(const base::string16& status) = 0;

  
  
  
  
  virtual void SetURL(const GURL& url, const std::string& languages) = 0;

  
  virtual void Hide() = 0;

  
  
  
  
  
  
  virtual void MouseMoved(const gfx::Point& position, bool left_content) = 0;

  
  
  
  virtual void UpdateDownloadShelfVisibility(bool visible) = 0;
};

#endif  

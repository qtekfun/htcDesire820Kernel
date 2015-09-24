// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VALIDATION_MESSAGE_BUBBLE_H_
#define CHROME_BROWSER_UI_VALIDATION_MESSAGE_BUBBLE_H_

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"

namespace content {
class RenderWidgetHost;
}

namespace gfx {
class Rect;
}

namespace chrome {

class ValidationMessageBubble {
 public:
  
  
  static scoped_ptr<ValidationMessageBubble> CreateAndShow(
      content::RenderWidgetHost* widget_host,
      const gfx::Rect& anchor_in_root_view,
      const base::string16& main_text,
      const base::string16& sub_text);

  
  virtual ~ValidationMessageBubble() {}

  
  
  
  virtual void SetPositionRelativeToAnchor(
      content::RenderWidgetHost* widget_host,
      const gfx::Rect& anchor_in_root_view) = 0;
};

}  

#endif  

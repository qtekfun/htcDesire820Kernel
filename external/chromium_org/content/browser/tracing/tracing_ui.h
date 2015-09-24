// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_TRACING_UI_H_
#define CONTENT_BROWSER_TRACING_UI_H_

#include "content/public/browser/web_ui_controller.h"

namespace content {

class TracingUI : public WebUIController {
 public:
  explicit TracingUI(WebUI* web_ui);

 private:
  DISALLOW_COPY_AND_ASSIGN(TracingUI);
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_WEB_UI_CONTROLLER_H_
#define CONTENT_PUBLIC_BROWSER_WEB_UI_CONTROLLER_H_

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "content/common/content_export.h"

class GURL;

namespace base {
class ListValue;
}

namespace content {

class RenderViewHost;
class WebUI;

class CONTENT_EXPORT WebUIController {
 public:
  explicit WebUIController(WebUI* web_ui) : web_ui_(web_ui) {}
  virtual ~WebUIController() {}

  
  
  virtual bool OverrideHandleWebUIMessage(const GURL& source_url,
                                          const std::string& message,
                                          const base::ListValue& args);

  
  
  
  virtual void RenderViewCreated(RenderViewHost* render_view_host) {}

  
  virtual void RenderViewReused(RenderViewHost* render_view_host) {}

  WebUI* web_ui() const { return web_ui_; }

 private:
  WebUI* web_ui_;
};

}  

#endif  

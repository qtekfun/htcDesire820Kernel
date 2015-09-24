// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WEBUI_GENERIC_HANDLER_H_
#define CONTENT_BROWSER_WEBUI_GENERIC_HANDLER_H_

#include "base/compiler_specific.h"
#include "content/public/browser/web_ui_message_handler.h"

namespace base {
class ListValue;
}

namespace content {

class GenericHandler : public WebUIMessageHandler {
 public:
  GenericHandler();
  virtual ~GenericHandler();

  
  virtual void RegisterMessages() OVERRIDE;

 private:
  void HandleNavigateToUrl(const base::ListValue* args);

  DISALLOW_COPY_AND_ASSIGN(GenericHandler);
};

}  

#endif  

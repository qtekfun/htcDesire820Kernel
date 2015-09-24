// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_SYNC_INTERNALS_UI_H_
#define CHROME_BROWSER_UI_WEBUI_SYNC_INTERNALS_UI_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "chrome/browser/sync/js_event_handler.h"
#include "content/browser/webui/web_ui.h"

namespace browser_sync {
class JsFrontend;
}  

class SyncInternalsUI : public WebUI, public browser_sync::JsEventHandler {
 public:
  explicit SyncInternalsUI(TabContents* contents);
  virtual ~SyncInternalsUI();

  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void ProcessWebUIMessage(
      const ExtensionHostMsg_DomMessage_Params& params);

  
  virtual void HandleJsEvent(const std::string& name,
                             const browser_sync::JsArgList& args);

 private:
  
  
  browser_sync::JsFrontend* GetJsFrontend();

  DISALLOW_COPY_AND_ASSIGN(SyncInternalsUI);
};

#endif  

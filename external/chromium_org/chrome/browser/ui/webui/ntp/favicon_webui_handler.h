// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NTP_FAVICON_WEBUI_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_NTP_FAVICON_WEBUI_HANDLER_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/favicon/favicon_service.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "content/public/browser/web_ui_message_handler.h"

class ExtensionIconColorManager;

namespace base {
class ListValue;
}

class FaviconWebUIHandler : public content::WebUIMessageHandler {
 public:
  FaviconWebUIHandler();
  virtual ~FaviconWebUIHandler();

  
  virtual void RegisterMessages() OVERRIDE;

  
  
  
  void HandleGetFaviconDominantColor(const base::ListValue* args);

  
  void HandleGetAppIconDominantColor(const base::ListValue* args);

  
  void NotifyAppIconReady(const std::string& extension_id);

 private:
  
  void OnFaviconDataAvailable(
      FaviconService::Handle request_handle,
      const chrome::FaviconBitmapResult& bitmap_result);

  CancelableTaskTracker cancelable_task_tracker_;

  
  
  
  std::map<int, std::string> dom_id_map_;
  
  int id_;

  
  
  scoped_refptr<base::RefCountedMemory> default_favicon_;

  
  scoped_ptr<ExtensionIconColorManager> app_icon_color_manager_;

  DISALLOW_COPY_AND_ASSIGN(FaviconWebUIHandler);
};

#endif  

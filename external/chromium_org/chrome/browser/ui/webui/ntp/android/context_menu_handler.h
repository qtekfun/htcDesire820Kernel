// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NTP_ANDROID_CONTEXT_MENU_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_NTP_ANDROID_CONTEXT_MENU_HANDLER_H_

#include "base/memory/weak_ptr.h"
#include "content/public/browser/web_ui_message_handler.h"
#include "ui/base/window_open_disposition.h"

namespace base {
class ListValue;
}

class ContextMenuHandler : public content::WebUIMessageHandler {
 public:
  ContextMenuHandler();
  virtual ~ContextMenuHandler();

  
  virtual void RegisterMessages() OVERRIDE;

  
  void OnItemSelected(int item_id);

  
  void GetIncognitoDisabled(const base::ListValue* args);

  
  void HandleShowContextMenu(const base::ListValue* args);

  
  

  
  void HandleOpenInNewTab(const base::ListValue* args);

  
  void HandleOpenInIncognitoTab(const base::ListValue* args);

 private:
  
  
  void OpenUrl(const base::ListValue* args, WindowOpenDisposition disposition);

  
  base::WeakPtrFactory<ContextMenuHandler> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ContextMenuHandler);
};

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_EXTENSIONS_INSTALL_EXTENSION_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_EXTENSIONS_INSTALL_EXTENSION_HANDLER_H_

#include "base/files/file_path.h"
#include "base/strings/string16.h"
#include "base/values.h"
#include "content/public/browser/web_ui_message_handler.h"

namespace content {
class WebUIDataSource;
}

namespace extensions {

class InstallExtensionHandler : public content::WebUIMessageHandler {
 public:
  InstallExtensionHandler();
  virtual ~InstallExtensionHandler();

  void GetLocalizedValues(content::WebUIDataSource* source);

  
  virtual void RegisterMessages() OVERRIDE;

 private:
  
  
  
  
  void HandleStartDragMessage(const ListValue* args);

  
  void HandleStopDragMessage(const ListValue* args);

  
  
  
  
  
  
  void HandleInstallMessage(const ListValue* args);

  
  
  void HandleInstallDirectoryMessage(const ListValue* args);

  
  
  base::FilePath file_to_install_;
  base::string16 file_display_name_;

  DISALLOW_COPY_AND_ASSIGN(InstallExtensionHandler);
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_EXTENSIONS_EXTENSION_INFO_UI_H_
#define CHROME_BROWSER_UI_WEBUI_EXTENSIONS_EXTENSION_INFO_UI_H_

#include "content/public/browser/web_ui_controller.h"
#include "url/gurl.h"


namespace base {
class ListValue;
}

namespace content {
class WebUIDataSource;
}

namespace extensions {

class ExtensionInfoUI : public content::WebUIController {
 public:
  explicit ExtensionInfoUI(content::WebUI* web_ui, const GURL& url);

  
  static GURL GetURL(const std::string& extension_id);

 private:
  virtual ~ExtensionInfoUI();

  
  void AddExtensionDataToSource(const std::string& extension_id);

  content::WebUIDataSource* source_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionInfoUI);
};

}  

#endif  

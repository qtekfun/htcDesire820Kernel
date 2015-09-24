// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_EXTENSIONS_EXTENSION_ERROR_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_EXTENSIONS_EXTENSION_ERROR_HANDLER_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "content/public/browser/web_ui_message_handler.h"

namespace base {
class DictionaryValue;
class ListValue;
}

namespace content {
class WebUIDataSource;
}

class Profile;

namespace extensions {

class Extension;

class ExtensionErrorHandler : public content::WebUIMessageHandler {
 public:
  explicit ExtensionErrorHandler(Profile* profile);
  virtual ~ExtensionErrorHandler();

  
  void GetLocalizedValues(content::WebUIDataSource* source);

  
  virtual void RegisterMessages() OVERRIDE;

 private:
  friend class ManifestHighlightUnitTest;

  
  void HandleRequestFileSource(const base::ListValue* args);
  
  void HandleOpenDevTools(const base::ListValue* args);

  
  
  
  
  void GetManifestFileCallback(base::DictionaryValue* dict,
                               const std::string& key,
                               const std::string& specific,
                               const std::string& contents);

  
  
  
  void GetSourceFileCallback(base::DictionaryValue* results,
                             int line_number,
                             const std::string& contents);

  
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionErrorHandler);
};

}  

#endif  

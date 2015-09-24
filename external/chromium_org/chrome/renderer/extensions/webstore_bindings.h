// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_WEBSTORE_BINDINGS_H_
#define CHROME_RENDERER_EXTENSIONS_WEBSTORE_BINDINGS_H_

#include "base/compiler_specific.h"
#include "chrome/renderer/extensions/chrome_v8_extension.h"
#include "third_party/WebKit/public/web/WebFrame.h"

namespace extensions {
class ChromeV8Context;

class WebstoreBindings : public ChromeV8Extension,
                         public ChromeV8ExtensionHandler {
 public:
  explicit WebstoreBindings(Dispatcher* dispatcher,
                            ChromeV8Context* context);

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

 private:
  void Install(const v8::FunctionCallbackInfo<v8::Value>& args);

  void OnInlineWebstoreInstallResponse(
      int install_id, bool success, const std::string& error);

  
  
  
  
  
  static bool GetWebstoreItemIdFromFrame(
      blink::WebFrame* frame, const std::string& preferred_store_link_url,
      std::string* webstore_item_id, std::string* error);

  DISALLOW_COPY_AND_ASSIGN(WebstoreBindings);
};

}  

#endif  

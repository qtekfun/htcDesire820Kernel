// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_RENDERER_EXTENSIONS_APP_BINDINGS_H_
#define CHROME_RENDERER_EXTENSIONS_APP_BINDINGS_H_

#include "base/compiler_specific.h"
#include "chrome/renderer/extensions/chrome_v8_extension.h"
#include "third_party/WebKit/public/web/WebFrame.h"

namespace extensions {
class ChromeV8Context;

class AppBindings : public ChromeV8Extension,
                    public ChromeV8ExtensionHandler {
 public:
  AppBindings(Dispatcher* dispatcher, ChromeV8Context* context);

 private:
  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  void GetIsInstalled(const v8::FunctionCallbackInfo<v8::Value>& args);
  void GetDetails(const v8::FunctionCallbackInfo<v8::Value>& args);
  void GetDetailsForFrame(const v8::FunctionCallbackInfo<v8::Value>& args);
  void GetInstallState(const v8::FunctionCallbackInfo<v8::Value>& args);
  void GetRunningState(const v8::FunctionCallbackInfo<v8::Value>& args);

  v8::Handle<v8::Value> GetDetailsForFrameImpl(blink::WebFrame* frame);

  void OnAppInstallStateResponse(const std::string& state, int callback_id);

  DISALLOW_COPY_AND_ASSIGN(AppBindings);
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_RUNTIME_CUSTOM_BINDINGS_H_
#define CHROME_RENDERER_EXTENSIONS_RUNTIME_CUSTOM_BINDINGS_H_

#include "base/compiler_specific.h"
#include "chrome/renderer/extensions/chrome_v8_extension.h"
#include "v8/include/v8.h"

class ExtensionDispatcher;
class ChromeV8Context;

namespace extensions {

class RuntimeCustomBindings : public ChromeV8Extension {
 public:
  RuntimeCustomBindings(Dispatcher* dispatcher, ChromeV8Context* context);

  virtual ~RuntimeCustomBindings();

  
  void OpenChannelToExtension(const v8::FunctionCallbackInfo<v8::Value>& args);

  
  void OpenChannelToNativeApp(const v8::FunctionCallbackInfo<v8::Value>& args);

 private:
  void GetManifest(const v8::FunctionCallbackInfo<v8::Value>& args);
  void GetExtensionViews(const v8::FunctionCallbackInfo<v8::Value>& args);
};

}  

#endif  

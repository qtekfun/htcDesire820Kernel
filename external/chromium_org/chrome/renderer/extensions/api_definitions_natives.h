// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_API_DEFINITIONS_NATIVES_H_
#define CHROME_RENDERER_EXTENSIONS_API_DEFINITIONS_NATIVES_H_

#include "chrome/renderer/extensions/chrome_v8_extension.h"
#include "chrome/renderer/extensions/dispatcher.h"

#include "v8/include/v8.h"

class ChromeV8Context;

namespace extensions {

class ApiDefinitionsNatives : public ChromeV8Extension {
 public:
  ApiDefinitionsNatives(Dispatcher* dispatcher, ChromeV8Context* context);

 private:
  
  void GetExtensionAPIDefinitionsForTest(
      const v8::FunctionCallbackInfo<v8::Value>& args);
  DISALLOW_COPY_AND_ASSIGN(ApiDefinitionsNatives);
};

}  

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_PEPPER_REQUEST_NATIVES_H_
#define CHROME_RENDERER_EXTENSIONS_PEPPER_REQUEST_NATIVES_H_

#include "base/compiler_specific.h"
#include "chrome/renderer/extensions/object_backed_native_handler.h"

namespace base {
class Value;
}

namespace extensions {

class PepperRequestNatives : public ObjectBackedNativeHandler {
 public:
  explicit PepperRequestNatives(ChromeV8Context* context);

 private:
  
  
  
  
  
  
  void SendResponse(const v8::FunctionCallbackInfo<v8::Value>& args);

  DISALLOW_COPY_AND_ASSIGN(PepperRequestNatives);
};

}  

#endif  

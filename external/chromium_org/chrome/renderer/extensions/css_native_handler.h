// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_CSS_NATIVE_HANDLER_H_
#define CHROME_RENDERER_EXTENSIONS_CSS_NATIVE_HANDLER_H_

#include "chrome/renderer/extensions/object_backed_native_handler.h"

namespace extensions {

class CssNativeHandler : public ObjectBackedNativeHandler {
 public:
  explicit CssNativeHandler(ChromeV8Context* context);

 private:
  
  
  
  
  void CanonicalizeCompoundSelector(
      const v8::FunctionCallbackInfo<v8::Value>& args);
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_BINDING_GENERATING_NATIVE_HANDLER_H_
#define CHROME_RENDERER_EXTENSIONS_BINDING_GENERATING_NATIVE_HANDLER_H_

#include <string>

#include "base/compiler_specific.h"
#include "chrome/renderer/extensions/native_handler.h"

namespace extensions {

class ModuleSystem;

class BindingGeneratingNativeHandler : public NativeHandler {
 public:
  
  
  BindingGeneratingNativeHandler(ModuleSystem* module_system,
                                 const std::string& api_name,
                                 const std::string& bind_to);

  virtual v8::Handle<v8::Object> NewInstance() OVERRIDE;

 private:
  ModuleSystem* module_system_;
  std::string api_name_;
  std::string bind_to_;
};

} 

#endif  

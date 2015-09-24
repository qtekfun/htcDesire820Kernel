// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_OBJECT_BACKED_NATIVE_HANDLER_H_
#define CHROME_RENDERER_EXTENSIONS_OBJECT_BACKED_NATIVE_HANDLER_H_

#include <string>
#include <vector>

#include "base/bind.h"
#include "base/memory/linked_ptr.h"
#include "chrome/renderer/extensions/native_handler.h"
#include "chrome/renderer/extensions/scoped_persistent.h"
#include "chrome/renderer/extensions/unsafe_persistent.h"
#include "v8/include/v8.h"

namespace extensions {
class ChromeV8Context;

class ObjectBackedNativeHandler : public NativeHandler {
 public:
  explicit ObjectBackedNativeHandler(ChromeV8Context* context);
  virtual ~ObjectBackedNativeHandler();

  
  
  virtual v8::Handle<v8::Object> NewInstance() OVERRIDE;

 protected:
  typedef base::Callback<void(const v8::FunctionCallbackInfo<v8::Value>&)>
      HandlerFunction;

  
  
  
  void RouteFunction(const std::string& name,
                     const HandlerFunction& handler_function);

  ChromeV8Context* context() const { return context_; }

  virtual void Invalidate() OVERRIDE;

 private:
  
  
  static void Router(const v8::FunctionCallbackInfo<v8::Value>& args);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  typedef std::vector<UnsafePersistent<v8::Object> > RouterData;
  RouterData router_data_;

  ChromeV8Context* context_;

  ScopedPersistent<v8::ObjectTemplate> object_template_;

  DISALLOW_COPY_AND_ASSIGN(ObjectBackedNativeHandler);
};

}  

#endif  

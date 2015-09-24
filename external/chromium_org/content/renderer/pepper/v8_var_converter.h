// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_V8_VAR_CONVERTER_H
#define CONTENT_RENDERER_PEPPER_V8_VAR_CONVERTER_H


#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/message_loop/message_loop_proxy.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_var.h"
#include "v8/include/v8.h"
#include "content/common/content_export.h"

namespace ppapi {
class ScopedPPVar;
}

namespace content {

class ResourceConverter;

class CONTENT_EXPORT V8VarConverter {
 public:
  explicit V8VarConverter(PP_Instance instance);
  
  V8VarConverter(
      PP_Instance instance,
      scoped_ptr<ResourceConverter> resource_converter);
  ~V8VarConverter();

  
  bool ToV8Value(const PP_Var& var,
                 v8::Handle<v8::Context> context,
                 v8::Handle<v8::Value>* result);

  
  
  
  
  
  
  
  void FromV8Value(
      v8::Handle<v8::Value> val,
      v8::Handle<v8::Context> context,
      const base::Callback<void(const ppapi::ScopedPPVar&, bool)>& callback);

 private:
  
  scoped_refptr<base::MessageLoopProxy> message_loop_proxy_;

  
  scoped_ptr<ResourceConverter> resource_converter_;

  DISALLOW_COPY_AND_ASSIGN(V8VarConverter);
};

}  

#endif  

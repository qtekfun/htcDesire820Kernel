// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_RENDERER_V8_VALUE_CONVERTER_H_
#define CONTENT_PUBLIC_RENDERER_V8_VALUE_CONVERTER_H_

#include "content/common/content_export.h"
#include "v8/include/v8.h"

namespace base {
class Value;
}

namespace content {

class CONTENT_EXPORT V8ValueConverter {
 public:
  
  class CONTENT_EXPORT Strategy {
   public:
    virtual ~Strategy() {}
    
    
    virtual bool FromV8Object(v8::Handle<v8::Object> value,
                              base::Value** out,
                              v8::Isolate* isolate) const = 0;
    
    
    virtual bool FromV8Array(v8::Handle<v8::Array> value,
                             base::Value** out,
                             v8::Isolate* isolate) const = 0;
  };

  static V8ValueConverter* create();

  virtual ~V8ValueConverter() {}

  
  
  
  
  
  virtual void SetDateAllowed(bool val) = 0;

  
  
  
  
  
  virtual void SetRegExpAllowed(bool val) = 0;

  
  
  
  
  virtual void SetFunctionAllowed(bool val) = 0;

  
  
  virtual void SetStripNullFromObjects(bool val) = 0;

  
  virtual void SetStrategy(Strategy* strategy) = 0;

  
  
  
  
  
  virtual v8::Handle<v8::Value> ToV8Value(
      const base::Value* value,
      v8::Handle<v8::Context> context) const = 0;

  
  
  
  
  
  
  
  
  
  virtual base::Value* FromV8Value(v8::Handle<v8::Value> value,
                                   v8::Handle<v8::Context> context) const = 0;
};

}  

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_SAFE_BUILTINS_H_
#define CHROME_RENDERER_EXTENSIONS_SAFE_BUILTINS_H_

#include "v8/include/v8.h"

namespace extensions {
class ChromeV8Context;

class SafeBuiltins {
 public:
  
  static v8::Extension* CreateV8Extension();

  explicit SafeBuiltins(ChromeV8Context* context);

  virtual ~SafeBuiltins();

  
  
  
  
  
  
  
  v8::Local<v8::Object> GetArray() const;
  v8::Local<v8::Object> GetFunction() const;
  v8::Local<v8::Object> GetJSON() const;
  
  
  v8::Local<v8::Object> GetObjekt() const;
  v8::Local<v8::Object> GetRegExp() const;
  v8::Local<v8::Object> GetString() const;

 private:
  ChromeV8Context* context_;
};

} 

#endif  

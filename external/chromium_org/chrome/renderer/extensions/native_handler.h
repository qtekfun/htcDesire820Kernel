// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_NATIVE_HANDLER_H_
#define CHROME_RENDERER_EXTENSIONS_NATIVE_HANDLER_H_

#include "base/basictypes.h"
#include "v8/include/v8.h"

namespace extensions {

class NativeHandler {
 public:
  NativeHandler();
  virtual ~NativeHandler();

  
  virtual v8::Handle<v8::Object> NewInstance() = 0;

  
  
  
  
  
  
  virtual void Invalidate();

 protected:
  
  bool is_valid() { return is_valid_; }

 private:
  bool is_valid_;

  DISALLOW_COPY_AND_ASSIGN(NativeHandler);
};

}  

#endif  

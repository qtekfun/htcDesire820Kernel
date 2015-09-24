// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_UNSAFE_PERSISTENT_H_
#define CHROME_RENDERER_EXTENSIONS_UNSAFE_PERSISTENT_H_

#include "v8/include/v8.h"

namespace extensions {

template<typename T> class UnsafePersistent {
 public:
  UnsafePersistent() : value_(0) { }

  explicit UnsafePersistent(v8::Persistent<T>* handle) {
    value_ = handle->ClearAndLeak();
  }

  UnsafePersistent(v8::Isolate* isolate, const v8::Handle<T>& handle) {
    v8::Persistent<T> persistent(isolate, handle);
    value_ = persistent.ClearAndLeak();
  }

  
  
  void dispose() {
    v8::Persistent<T> handle(value_);
    handle.Reset();
    value_ = 0;
  }

  
  
  v8::Local<T> newLocal(v8::Isolate* isolate) {
    return v8::Local<T>::New(isolate, v8::Local<T>(value_));
  }

 private:
  T* value_;
};

}  

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GIN_HANDLE_H_
#define GIN_HANDLE_H_

#include "gin/converter.h"

namespace gin {

template<typename T>
class Handle {
 public:
  Handle() : object_(NULL) {}

  Handle(v8::Handle<v8::Value> wrapper, T* object)
    : wrapper_(wrapper),
      object_(object) {
  }

  bool IsEmpty() const { return !object_; }

  void Clear() {
    wrapper_.Clear();
    object_ = NULL;
  }

  T* operator->() const { return object_; }
  v8::Handle<v8::Value> ToV8() const { return wrapper_; }
  T* get() const { return object_; }

 private:
  v8::Handle<v8::Value> wrapper_;
  T* object_;
};

template<typename T>
struct Converter<gin::Handle<T> > {
  static v8::Handle<v8::Value> ToV8(v8::Isolate* isolate,
                                    const gin::Handle<T>& val) {
    return val.ToV8();
  }
  static bool FromV8(v8::Isolate* isolate, v8::Handle<v8::Value> val,
                     gin::Handle<T>* out) {
    T* object = NULL;
    if (!Converter<T*>::FromV8(isolate, val, &object)) {
      return false;
    }
    *out = gin::Handle<T>(val, object);
    return true;
  }
};

template<typename T>
gin::Handle<T> CreateHandle(v8::Isolate* isolate, T* object) {
  return gin::Handle<T>(object->GetWrapper(isolate), object);
}

}  

#endif  

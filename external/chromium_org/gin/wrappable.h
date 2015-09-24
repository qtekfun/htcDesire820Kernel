// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GIN_WRAPPABLE_H_
#define GIN_WRAPPABLE_H_

#include "base/template_util.h"
#include "gin/converter.h"
#include "gin/gin_export.h"
#include "gin/public/wrapper_info.h"

namespace gin {

namespace internal {

GIN_EXPORT void* FromV8Impl(v8::Isolate* isolate,
                            v8::Handle<v8::Value> val,
                            WrapperInfo* info);

}  


template<typename T>
class Wrappable;


class GIN_EXPORT WrappableBase {
 protected:
  WrappableBase();
  virtual ~WrappableBase();
  v8::Handle<v8::Object> GetWrapperImpl(v8::Isolate* isolate,
                                        WrapperInfo* wrapper_info);
  v8::Handle<v8::Object> CreateWrapper(v8::Isolate* isolate,
                                       WrapperInfo* wrapper_info);
  v8::Persistent<v8::Object> wrapper_;  

 private:
  static void WeakCallback(
      const v8::WeakCallbackData<v8::Object, WrappableBase>& data);

  DISALLOW_COPY_AND_ASSIGN(WrappableBase);
};


template<typename T>
class Wrappable : public WrappableBase {
 public:
  
  
  
  v8::Handle<v8::Object> GetWrapper(v8::Isolate* isolate) {
    return GetWrapperImpl(isolate, &T::kWrapperInfo);
  }

 protected:
  Wrappable() {}
  virtual ~Wrappable() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(Wrappable);
};


template<typename T>
struct Converter<T*, typename base::enable_if<
                       base::is_convertible<T*, Wrappable<T>*>::value>::type> {
  static v8::Handle<v8::Value> ToV8(v8::Isolate* isolate, T* val) {
    return val->GetWrapper(isolate);
  }

  static bool FromV8(v8::Isolate* isolate, v8::Handle<v8::Value> val, T** out) {
    *out = static_cast<T*>(internal::FromV8Impl(isolate, val,
                                                &T::kWrapperInfo));
    return *out != NULL;
  }
};

}  

#endif  

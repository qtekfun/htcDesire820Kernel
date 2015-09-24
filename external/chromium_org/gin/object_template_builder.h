// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GIN_OBJECT_TEMPLATE_BUILDER_H_
#define GIN_OBJECT_TEMPLATE_BUILDER_H_

#include "base/bind.h"
#include "base/callback.h"
#include "base/strings/string_piece.h"
#include "base/template_util.h"
#include "gin/converter.h"
#include "gin/function_template.h"
#include "gin/gin_export.h"
#include "v8/include/v8.h"

namespace gin {

namespace {

template<typename T, typename Enable = void>
struct CallbackTraits {
  static v8::Handle<v8::FunctionTemplate> CreateTemplate(v8::Isolate* isolate,
                                                         T callback) {
    return CreateFunctionTemplate(isolate, base::Bind(callback));
  }
};

template<typename T>
struct CallbackTraits<base::Callback<T> > {
  static v8::Handle<v8::FunctionTemplate> CreateTemplate(
      v8::Isolate* isolate, const base::Callback<T>& callback) {
    return CreateFunctionTemplate(isolate, callback);
  }
};

template<typename T>
struct CallbackTraits<T, typename base::enable_if<
                           base::is_member_function_pointer<T>::value>::type> {
  static v8::Handle<v8::FunctionTemplate> CreateTemplate(v8::Isolate* isolate,
                                                         T callback) {
    return CreateFunctionTemplate(isolate, base::Bind(callback),
                                  HolderIsFirstArgument);
  }
};

template<>
struct GIN_EXPORT CallbackTraits<v8::Handle<v8::FunctionTemplate> > {
  static v8::Handle<v8::FunctionTemplate> CreateTemplate(
      v8::Handle<v8::FunctionTemplate> templ) {
    return templ;
  }
};

}  


class GIN_EXPORT ObjectTemplateBuilder {
 public:
  explicit ObjectTemplateBuilder(v8::Isolate* isolate);
  ~ObjectTemplateBuilder();

  
  // poetic license here in order that all calls to Set() can be via the '.'
  
  template<typename T>
  ObjectTemplateBuilder& SetValue(const base::StringPiece& name, T val) {
    return SetImpl(name, ConvertToV8(isolate_, val));
  }

  
  
  
  
  template<typename T>
  ObjectTemplateBuilder& SetMethod(const base::StringPiece& name,
                                   const T& callback) {
    return SetImpl(name, CallbackTraits<T>::CreateTemplate(isolate_, callback));
  }
  template<typename T>
  ObjectTemplateBuilder& SetProperty(const base::StringPiece& name,
                                     const T& getter) {
    return SetPropertyImpl(name,
                           CallbackTraits<T>::CreateTemplate(isolate_, getter),
                           v8::Local<v8::FunctionTemplate>());
  }
  template<typename T, typename U>
  ObjectTemplateBuilder& SetProperty(const base::StringPiece& name,
                                     const T& getter, const U& setter) {
    return SetPropertyImpl(name,
                           CallbackTraits<T>::CreateTemplate(isolate_, getter),
                           CallbackTraits<U>::CreateTemplate(isolate_, setter));
  }

  v8::Local<v8::ObjectTemplate> Build();

 private:
  ObjectTemplateBuilder& SetImpl(const base::StringPiece& name,
                                 v8::Handle<v8::Data> val);
  ObjectTemplateBuilder& SetPropertyImpl(
      const base::StringPiece& name, v8::Handle<v8::FunctionTemplate> getter,
      v8::Handle<v8::FunctionTemplate> setter);

  v8::Isolate* isolate_;

  
  v8::Local<v8::ObjectTemplate> template_;
};

}  

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_MODULE_IMPL_H_
#define PPAPI_CPP_MODULE_IMPL_H_


#include "ppapi/cpp/module.h"

namespace pp {

namespace {

template <typename T> const char* interface_name() {
  return NULL;
}

template <typename T> inline T const* get_interface() {
  static T const* funcs = reinterpret_cast<T const*>(
      pp::Module::Get()->GetBrowserInterface(interface_name<T>()));
  return funcs;
}

template <typename T> inline bool has_interface() {
  return get_interface<T>() != NULL;
}

}  

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_CALLBACK_SOURCE_H
#define NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_CALLBACK_SOURCE_H

#include "ppapi/cpp/completion_callback.h"

namespace plugin {


template<class T>
class CallbackSource {
 public:
  virtual ~CallbackSource();
  
  
  virtual pp::CompletionCallbackWithOutput<T> GetCallback() = 0;
};
}
#endif 

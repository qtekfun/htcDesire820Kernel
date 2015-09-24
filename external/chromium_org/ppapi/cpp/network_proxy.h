// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_NETWORK_PROXY_H_
#define PPAPI_CPP_NETWORK_PROXY_H_

#include <string>

#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/instance_handle.h"

namespace pp {

class NetworkProxy {
 public:
  
  static bool IsAvailable();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static int32_t GetProxyForURL(
      const InstanceHandle& instance,
      const Var& url,
      const pp::CompletionCallbackWithOutput<Var>& callback);
};

}  

#endif  

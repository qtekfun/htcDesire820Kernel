// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_HOST_HOST_FACTORY_H_
#define PPAPI_HOST_HOST_FACTORY_H_

#include "base/memory/scoped_ptr.h"
#include "ppapi/c/pp_instance.h"

namespace IPC {
class Message;
}

namespace ppapi {

namespace proxy {
class ResourceMessageCallParams;
}

namespace host {

class PpapiHost;
class ResourceHost;

class HostFactory {
 public:
  virtual ~HostFactory() {}

  virtual scoped_ptr<ResourceHost> CreateResourceHost(
      PpapiHost* host,
      const proxy::ResourceMessageCallParams& params,
      PP_Instance instance,
      const IPC::Message& message) = 0;
};

}  
}  

#endif  

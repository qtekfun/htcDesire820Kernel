// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_INTERFACE_PROXY_H_
#define PPAPI_PROXY_INTERFACE_PROXY_H_

#include "base/basictypes.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_sender.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/shared_impl/api_id.h"

namespace ppapi {
namespace proxy {

class Dispatcher;

class InterfaceProxy : public IPC::Listener, public IPC::Sender {
 public:
  
  
  typedef InterfaceProxy* (*Factory)(Dispatcher* dispatcher);

  
  
  
  
  
  
  
  struct Info {
    const void* interface_ptr;

    const char* name;
    ApiID id;

    bool is_trusted;

    InterfaceProxy::Factory create_proxy;
  };

  virtual ~InterfaceProxy();

  Dispatcher* dispatcher() const { return dispatcher_; }

  
  virtual bool Send(IPC::Message* msg);

  
  

 protected:
  
  
  InterfaceProxy(Dispatcher* dispatcher);

 private:
  Dispatcher* dispatcher_;
};

}  
}  

#endif  


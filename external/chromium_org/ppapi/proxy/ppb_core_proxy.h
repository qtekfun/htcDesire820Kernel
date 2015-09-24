// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PPB_CORE_PROXY_H_
#define PPAPI_PPB_CORE_PROXY_H_

#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/ppb_core.h"
#include "ppapi/proxy/interface_proxy.h"
#include "ppapi/shared_impl/host_resource.h"

namespace ppapi {
namespace proxy {

class PPB_Core_Proxy : public InterfaceProxy {
 public:
  PPB_Core_Proxy(Dispatcher* dispatcher);
  virtual ~PPB_Core_Proxy();

  static const PPB_Core* GetPPB_Core_Interface();

  
  virtual bool OnMessageReceived(const IPC::Message& msg);

  static const ApiID kApiID = API_ID_PPB_CORE;

 private:
  
  void OnMsgAddRefResource(const ppapi::HostResource& resource);
  void OnMsgReleaseResource(const ppapi::HostResource& resource);

  
  
  
  const PPB_Core* ppb_core_impl_;
};

}  
}  

#endif  

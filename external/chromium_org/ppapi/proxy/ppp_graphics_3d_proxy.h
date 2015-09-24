// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_PPP_GRAPHICS_3D_PROXY_H_
#define PPAPI_PROXY_PPP_GRAPHICS_3D_PROXY_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/ppp_graphics_3d.h"
#include "ppapi/proxy/interface_proxy.h"
#include "ppapi/shared_impl/host_resource.h"

namespace ppapi {
namespace proxy {

class PPP_Graphics3D_Proxy : public InterfaceProxy {
 public:
  PPP_Graphics3D_Proxy(Dispatcher* dispatcher);
  virtual ~PPP_Graphics3D_Proxy();

  static const Info* GetInfo();

  
  virtual bool OnMessageReceived(const IPC::Message& msg);

 private:
  
  void OnMsgContextLost(PP_Instance instance);

  
  
  
  const PPP_Graphics3D* ppp_graphics_3d_impl_;

  DISALLOW_COPY_AND_ASSIGN(PPP_Graphics3D_Proxy);
};

}  
}  

#endif  

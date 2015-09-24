// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_DISPATCHER_H_
#define PPAPI_PROXY_DISPATCHER_H_

#include <set>
#include <string>
#include <vector>

#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/tracked_objects.h"
#include "ipc/ipc_channel_proxy.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/ppp.h"
#include "ppapi/proxy/proxy_channel.h"
#include "ppapi/proxy/interface_list.h"
#include "ppapi/proxy/interface_proxy.h"
#include "ppapi/proxy/plugin_var_tracker.h"
#include "ppapi/shared_impl/api_id.h"

namespace ppapi {

namespace proxy {

class VarSerializationRules;

class PPAPI_PROXY_EXPORT Dispatcher : public ProxyChannel {
 public:
  virtual ~Dispatcher();

  
  
  virtual bool IsPlugin() const = 0;

  void AddFilter(IPC::Listener* listener);

  VarSerializationRules* serialization_rules() const {
    return serialization_rules_.get();
  }

  
  
  
  InterfaceProxy* GetInterfaceProxy(ApiID id);

  
  
  
  
  base::MessageLoopProxy* GetIPCMessageLoop();

  
  void AddIOThreadMessageFilter(IPC::ChannelProxy::MessageFilter* filter);

  
  
  
  

  
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;

  PP_GetInterface_Func local_get_interface() const {
    return local_get_interface_;
  }

  const PpapiPermissions& permissions() const { return permissions_; }

 protected:
  explicit Dispatcher(PP_GetInterface_Func local_get_interface,
                      const PpapiPermissions& permissions);

  
  
  void SetSerializationRules(VarSerializationRules* var_serialization_rules);

  
  
  virtual void OnInvalidMessageReceived();

 protected:
  std::vector<IPC::Listener*> filters_;

 private:
  friend class HostDispatcherTest;
  friend class PluginDispatcherTest;

  
  scoped_ptr<InterfaceProxy> proxies_[API_ID_COUNT];

  bool disallow_trusted_interfaces_;

  PP_GetInterface_Func local_get_interface_;

  scoped_refptr<VarSerializationRules> serialization_rules_;

  PpapiPermissions permissions_;

  DISALLOW_COPY_AND_ASSIGN(Dispatcher);
};

}  
}  

#endif  

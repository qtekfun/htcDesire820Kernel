// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_HOST_DISPATCHER_WRAPPER_H_
#define CONTENT_RENDERER_PEPPER_HOST_DISPATCHER_WRAPPER_H_

#include "base/process/process_handle.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/ppp.h"
#include "ppapi/proxy/host_dispatcher.h"
#include "ppapi/shared_impl/ppapi_permissions.h"

namespace IPC {
struct ChannelHandle;
}

namespace content {
class PepperHungPluginFilter;
class PluginModule;

class HostDispatcherWrapper {
 public:
  HostDispatcherWrapper(PluginModule* module,
                        base::ProcessId peer_pid,
                        int plugin_child_id,
                        const ppapi::PpapiPermissions& perms,
                        bool is_external);
  virtual ~HostDispatcherWrapper();

  bool Init(const IPC::ChannelHandle& channel_handle,
            PP_GetInterface_Func local_get_interface,
            const ppapi::Preferences& preferences,
            PepperHungPluginFilter* filter);

  
  const void* GetProxiedInterface(const char* name);

  
  
  
  
  void AddInstance(PP_Instance instance);

  
  
  
  void RemoveInstance(PP_Instance instance);

  base::ProcessId peer_pid() { return peer_pid_; }
  int plugin_child_id() { return plugin_child_id_; }
  ppapi::proxy::HostDispatcher* dispatcher() { return dispatcher_.get(); }

 private:
  PluginModule* module_;

  base::ProcessId peer_pid_;

  
  
  
  int plugin_child_id_;

  ppapi::PpapiPermissions permissions_;
  bool is_external_;

  scoped_ptr<ppapi::proxy::HostDispatcher> dispatcher_;
  scoped_ptr<ppapi::proxy::ProxyChannel::Delegate> dispatcher_delegate_;
};

}  

#endif  

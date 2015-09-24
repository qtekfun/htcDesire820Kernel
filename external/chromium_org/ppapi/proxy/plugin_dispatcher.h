// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_PLUGIN_DISPATCHER_H_
#define PPAPI_PROXY_PLUGIN_DISPATCHER_H_

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/process/process.h"
#include "build/build_config.h"
#include "ipc/ipc_sync_channel.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/c/ppb_console.h"
#include "ppapi/proxy/dispatcher.h"
#include "ppapi/shared_impl/ppapi_preferences.h"
#include "ppapi/shared_impl/ppb_view_shared.h"
#include "ppapi/shared_impl/singleton_resource_id.h"
#include "ppapi/shared_impl/tracked_callback.h"

namespace IPC {
class SyncMessageFilter;
}

namespace ppapi {

struct Preferences;
class Resource;

namespace thunk {
class PPB_Instance_API;
class ResourceCreationAPI;
}

namespace proxy {

struct InstanceData {
  InstanceData();
  ~InstanceData();

  ViewData view;

  
  scoped_refptr<TrackedCallback> mouse_lock_callback;

  
  typedef std::map<SingletonResourceID, scoped_refptr<Resource> >
      SingletonResourceMap;
  SingletonResourceMap singleton_resources;

  
  
  
  
  bool is_request_surrounding_text_pending;
  bool should_do_request_surrounding_text;
};

class PPAPI_PROXY_EXPORT PluginDispatcher
    : public Dispatcher,
      public base::SupportsWeakPtr<PluginDispatcher> {
 public:
  class PPAPI_PROXY_EXPORT PluginDelegate : public ProxyChannel::Delegate {
   public:
    
    
    
    
    virtual std::set<PP_Instance>* GetGloballySeenInstanceIDSet() = 0;

    
    
    
    virtual uint32 Register(PluginDispatcher* plugin_dispatcher) = 0;
    virtual void Unregister(uint32 plugin_dispatcher_id) = 0;
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  PluginDispatcher(PP_GetInterface_Func get_interface,
                   const PpapiPermissions& permissions,
                   bool incognito);
  virtual ~PluginDispatcher();

  
  
  
  
  static PluginDispatcher* GetForInstance(PP_Instance instance);

  
  
  static PluginDispatcher* GetForResource(const Resource* resource);

  
  
  static const void* GetBrowserInterface(const char* interface_name);

  
  
  
  static void LogWithSource(PP_Instance instance,
                            PP_LogLevel level,
                            const std::string& source,
                            const std::string& value);

  const void* GetPluginInterface(const std::string& interface_name);

  
  
  
  bool InitPluginWithChannel(PluginDelegate* delegate,
                             base::ProcessId peer_pid,
                             const IPC::ChannelHandle& channel_handle,
                             bool is_client);

  
  virtual bool IsPlugin() const;
  virtual bool Send(IPC::Message* msg);

  
  virtual bool OnMessageReceived(const IPC::Message& msg);
  virtual void OnChannelError();

  
  
  void DidCreateInstance(PP_Instance instance);
  void DidDestroyInstance(PP_Instance instance);

  
  
  InstanceData* GetInstanceData(PP_Instance instance);

  
  
  thunk::PPB_Instance_API* GetInstanceAPI();
  thunk::ResourceCreationAPI* GetResourceCreationAPI();

  
  const Preferences& preferences() const { return preferences_; }

  uint32 plugin_dispatcher_id() const { return plugin_dispatcher_id_; }
  bool incognito() const { return incognito_; }

 private:
  friend class PluginDispatcherTest;

  
  
  void ForceFreeAllInstances();

  
  void OnMsgSupportsInterface(const std::string& interface_name, bool* result);
  void OnMsgSetPreferences(const Preferences& prefs);

  virtual bool SendMessage(IPC::Message* msg);

  PluginDelegate* plugin_delegate_;

  
  
  
  
  typedef base::hash_map<std::string, const void*> InterfaceMap;
  InterfaceMap plugin_interfaces_;

  typedef base::hash_map<PP_Instance, InstanceData> InstanceDataMap;
  InstanceDataMap instance_map_;

  
  
  bool received_preferences_;
  Preferences preferences_;

  uint32 plugin_dispatcher_id_;

  
  
  bool incognito_;

  
  scoped_refptr<IPC::SyncMessageFilter> sync_filter_;

  DISALLOW_COPY_AND_ASSIGN(PluginDispatcher);
};

}  
}  

#endif  

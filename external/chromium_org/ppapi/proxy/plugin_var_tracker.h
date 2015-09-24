// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_PLUGIN_VAR_TRACKER_H_
#define PPAPI_PROXY_PLUGIN_VAR_TRACKER_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/shared_memory.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/proxy/ppapi_proxy_export.h"
#include "ppapi/shared_impl/var_tracker.h"

template<typename T> struct DefaultSingletonTraits;
struct PPP_Class_Deprecated;

namespace ppapi {

class ProxyObjectVar;

namespace proxy {

class PluginDispatcher;

class PPAPI_PROXY_EXPORT PluginVarTracker : public VarTracker {
 public:
  PluginVarTracker();
  ~PluginVarTracker();

  
  
  
  PP_Var ReceiveObjectPassRef(const PP_Var& var, PluginDispatcher* dispatcher);

  
  
  
  PP_Var TrackObjectWithNoReference(const PP_Var& host_var,
                                    PluginDispatcher* dispatcher);
  void StopTrackingObjectWithNoReference(const PP_Var& plugin_var);

  
  
  PP_Var GetHostObject(const PP_Var& plugin_object) const;

  PluginDispatcher* DispatcherForPluginObject(
      const PP_Var& plugin_object) const;

  
  
  void ReleaseHostObject(PluginDispatcher* dispatcher,
                         const PP_Var& host_object);

  
  virtual PP_Var MakeResourcePPVarFromMessage(
      PP_Instance instance,
      const IPC::Message& creation_message,
      int pending_renderer_id,
      int pending_browser_id) OVERRIDE;
  virtual ResourceVar* MakeResourceVar(PP_Resource pp_resource) OVERRIDE;
  virtual void DidDeleteInstance(PP_Instance instance) OVERRIDE;
  virtual int TrackSharedMemoryHandle(PP_Instance instance,
                                      base::SharedMemoryHandle file,
                                      uint32 size_in_bytes) OVERRIDE;
  virtual bool StopTrackingSharedMemoryHandle(int id,
                                              PP_Instance instance,
                                              base::SharedMemoryHandle* handle,
                                              uint32* size_in_bytes) OVERRIDE;

  
  
  void PluginImplementedObjectCreated(PP_Instance instance,
                                      const PP_Var& created_var,
                                      const PPP_Class_Deprecated* ppp_class,
                                      void* ppp_class_data);
  void PluginImplementedObjectDestroyed(void* ppp_class_data);

  
  
  
  
  bool IsPluginImplementedObjectAlive(void* user_data);

  
  
  bool ValidatePluginObjectCall(const PPP_Class_Deprecated* ppp_class,
                                void* user_data);

  void DidDeleteDispatcher(PluginDispatcher* dispatcher);

 private:
  
  virtual int32 AddVarInternal(Var* var, AddVarRefMode mode) OVERRIDE;
  virtual void TrackedObjectGettingOneRef(VarMap::const_iterator iter) OVERRIDE;
  virtual void ObjectGettingZeroRef(VarMap::iterator iter) OVERRIDE;
  virtual bool DeleteObjectInfoIfNecessary(VarMap::iterator iter) OVERRIDE;
  virtual ArrayBufferVar* CreateArrayBuffer(uint32 size_in_bytes) OVERRIDE;
  virtual ArrayBufferVar* CreateShmArrayBuffer(
      uint32 size_in_bytes,
      base::SharedMemoryHandle handle) OVERRIDE;

 private:
  friend struct DefaultSingletonTraits<PluginVarTracker>;
  friend class PluginProxyTestHarness;

  
  struct HostVar {
    HostVar(PluginDispatcher* d, int32 i);

    bool operator<(const HostVar& other) const;

    
    
    PluginDispatcher* dispatcher;

    
    
    
    int32 host_object_id;
  };

  struct PluginImplementedVar {
    const PPP_Class_Deprecated* ppp_class;

    
    
    PP_Instance instance;

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    int32 plugin_object_id;
  };

  
  
  
  
  
  PP_Var GetOrCreateObjectVarID(ProxyObjectVar* object);

  
  void SendAddRefObjectMsg(const ProxyObjectVar& proxy_object);
  void SendReleaseObjectMsg(const ProxyObjectVar& proxy_object);

  
  
  
  scoped_refptr<ProxyObjectVar> FindOrMakePluginVarFromHostVar(
      const PP_Var& var,
      PluginDispatcher* dispatcher);

  
  typedef std::map<HostVar, int32> HostVarToPluginVarMap;
  HostVarToPluginVarMap host_var_to_plugin_var_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  typedef std::map<void*, PluginImplementedVar>
      UserDataToPluginImplementedVarMap;
  UserDataToPluginImplementedVarMap user_data_to_plugin_;

  DISALLOW_COPY_AND_ASSIGN(PluginVarTracker);
};

}  
}  

#endif  

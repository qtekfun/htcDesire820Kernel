// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_HOST_PPAPI_HOST_H_
#define PPAPI_HOST_PPAPI_HOST_H_

#include <map>

#include "base/compiler_specific.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/observer_list.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_sender.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/host/ppapi_host_export.h"
#include "ppapi/shared_impl/ppapi_permissions.h"

namespace ppapi {

namespace proxy {
class ResourceMessageCallParams;
class ResourceMessageReplyParams;
}

namespace host {

class HostFactory;
struct HostMessageContext;
class InstanceMessageFilter;
struct ReplyMessageContext;
class ResourceHost;

class PPAPI_HOST_EXPORT PpapiHost : public IPC::Sender, public IPC::Listener {
 public:
  
  
  
  
  PpapiHost(IPC::Sender* sender, const PpapiPermissions& perms);
  virtual ~PpapiHost();

  const PpapiPermissions& permissions() const { return permissions_; }

  
  virtual bool Send(IPC::Message* msg) OVERRIDE;

  
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;

  
  void SendReply(const ReplyMessageContext& context,
                 const IPC::Message& msg);

  
  void SendUnsolicitedReply(PP_Resource resource, const IPC::Message& msg);

  
  scoped_ptr<ResourceHost> CreateResourceHost(
      const proxy::ResourceMessageCallParams& params,
      PP_Instance instance,
      const IPC::Message& nested_msg);

  
  
  
  int AddPendingResourceHost(scoped_ptr<ResourceHost> resource_host);

  
  
  void AddHostFactoryFilter(scoped_ptr<HostFactory> filter);

  
  
  void AddInstanceMessageFilter(scoped_ptr<InstanceMessageFilter> filter);

  
  host::ResourceHost* GetResourceHost(PP_Resource resource) const;

 private:
  friend class InstanceMessageFilter;

  void HandleResourceCall(
      const proxy::ResourceMessageCallParams& params,
      const IPC::Message& nested_msg,
      HostMessageContext* context);

  
  void OnHostMsgResourceCall(const proxy::ResourceMessageCallParams& params,
                             const IPC::Message& nested_msg);
  void OnHostMsgInProcessResourceCall(
      int routing_id,
      const proxy::ResourceMessageCallParams& params,
      const IPC::Message& nested_msg);
  void OnHostMsgResourceSyncCall(
      const proxy::ResourceMessageCallParams& params,
      const IPC::Message& nested_msg,
      IPC::Message* reply_msg);
  void OnHostMsgResourceCreated(const proxy::ResourceMessageCallParams& param,
                                PP_Instance instance,
                                const IPC::Message& nested_msg);
  void OnHostMsgAttachToPendingHost(PP_Resource resource, int pending_host_id);
  void OnHostMsgResourceDestroyed(PP_Resource resource);

  
  IPC::Sender* sender_;

  PpapiPermissions permissions_;

  
  
  
  
  ScopedVector<HostFactory> host_factory_filters_;

  
  
  
  
  ScopedVector<InstanceMessageFilter> instance_message_filters_;

  typedef std::map<PP_Resource, linked_ptr<ResourceHost> > ResourceMap;
  ResourceMap resources_;

  
  
  
  typedef std::map<int, linked_ptr<ResourceHost> > PendingHostResourceMap;
  PendingHostResourceMap pending_resource_hosts_;
  int next_pending_resource_host_id_;

  DISALLOW_COPY_AND_ASSIGN(PpapiHost);
};

}  
}  

#endif  

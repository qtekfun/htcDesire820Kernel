// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_NPAPI_NP_CHANNEL_BASE_H_
#define CONTENT_CHILD_NPAPI_NP_CHANNEL_BASE_H_

#include <string>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/process/process.h"
#include "content/child/npapi/npobject_base.h"
#include "content/common/message_router.h"
#include "ipc/ipc_channel_handle.h"
#include "ipc/ipc_sync_channel.h"

namespace base {
class MessageLoopProxy;
}

#if defined(COMPILER_GCC)
namespace BASE_HASH_NAMESPACE {

template<>
struct hash<NPObject*> {
  std::size_t operator()(NPObject* const& ptr) const {
    return hash<size_t>()(reinterpret_cast<size_t>(ptr));
  }
};

template<>
struct hash<struct _NPP*> {
  std::size_t operator()(struct _NPP* const& ptr) const {
    return hash<size_t>()(reinterpret_cast<size_t>(ptr));
  }
};

}  
#elif defined(COMPILER_MSVC)
namespace stdext {

template<>
inline size_t hash_value(NPObject* const& ptr) {
  return hash_value(reinterpret_cast<size_t>(ptr));
}

}  
#endif 

namespace content {

class NPChannelBase : public IPC::Listener,
                      public IPC::Sender,
                      public base::RefCountedThreadSafe<NPChannelBase> {
 public:

  
  
  
  
  
  
  void AddRoute(int route_id, IPC::Listener* listener, NPObjectBase* npobject);
  void RemoveRoute(int route_id);

  void AddMappingForNPObjectProxy(int route_id, NPObject* object);
  void RemoveMappingForNPObjectProxy(int route_id);

  void AddMappingForNPObjectStub(int route_id, NPObject* object);
  void RemoveMappingForNPObjectStub(int route_id, NPObject* object);

  void AddMappingForNPObjectOwner(int route_id, struct _NPP* owner);
  void SetDefaultNPObjectOwner(struct _NPP* owner);
  void RemoveMappingForNPObjectOwner(int route_id);

  NPObject* GetExistingNPObjectProxy(int route_id);
  int GetExistingRouteForNPObjectStub(NPObject* npobject);
  struct _NPP* GetExistingNPObjectOwner(int route_id);
  int GetExistingRouteForNPObjectOwner(struct _NPP* owner);

  
  virtual bool Send(IPC::Message* msg) OVERRIDE;

  base::ProcessId peer_pid() { return channel_->peer_pid(); }
  IPC::ChannelHandle channel_handle() const { return channel_handle_; }

  
  static int Count();

  
  virtual int GenerateRouteID() = 0;

  
  
  bool channel_valid() {
    return channel_valid_;
  }

  
  
  static NPChannelBase* GetCurrentChannel();

  static void CleanupChannels();

  
  
  NPObjectBase* GetNPObjectListenerForRoute(int route_id);

  
  
  
  virtual base::WaitableEvent* GetModalDialogEvent(int render_view_id);

 protected:
  typedef NPChannelBase* (*ChannelFactory)();

  friend class base::RefCountedThreadSafe<NPChannelBase>;

  virtual ~NPChannelBase();

  
  
  
  
  
  static NPChannelBase* GetChannel(
      const IPC::ChannelHandle& channel_handle, IPC::Channel::Mode mode,
      ChannelFactory factory, base::MessageLoopProxy* ipc_message_loop,
      bool create_pipe_now, base::WaitableEvent* shutdown_event);

  
  static void Broadcast(IPC::Message* message);

  
  NPChannelBase();

  virtual void CleanUp() { }

  
  virtual bool OnControlMessageReceived(const IPC::Message& msg);

  
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;
  virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

  void set_send_unblocking_only_during_unblock_dispatch() {
    send_unblocking_only_during_unblock_dispatch_ = true;
  }

  virtual bool Init(base::MessageLoopProxy* ipc_message_loop,
                    bool create_pipe_now,
                    base::WaitableEvent* shutdown_event);

  scoped_ptr<IPC::SyncChannel> channel_;
  IPC::ChannelHandle channel_handle_;

 private:
  IPC::Channel::Mode mode_;
  
  
  
  int non_npobject_count_;
  int peer_pid_;

  
  bool in_remove_route_;

  
  
  typedef base::hash_map<int, NPObjectBase*> ListenerMap;
  ListenerMap npobject_listeners_;

  typedef base::hash_map<int, NPObject*> ProxyMap;
  ProxyMap proxy_map_;

  typedef base::hash_map<NPObject*, int> StubMap;
  StubMap stub_map_;

  typedef base::hash_map<struct _NPP*, int> OwnerToRouteMap;
  OwnerToRouteMap owner_to_route_;

  typedef base::hash_map<int, struct _NPP*> RouteToOwnerMap;
  RouteToOwnerMap route_to_owner_;

  
  
  struct _NPP* default_owner_;

  
  
  MessageRouter router_;

  
  
  bool channel_valid_;

  
  
  int in_unblock_dispatch_;

  
  
  
  
  
  
  
  
  
  
  bool send_unblocking_only_during_unblock_dispatch_;

  DISALLOW_COPY_AND_ASSIGN(NPChannelBase);
};

}  

#endif  

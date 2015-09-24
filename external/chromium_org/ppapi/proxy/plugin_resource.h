// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_PLUGIN_RESOURCE_H_
#define PPAPI_PROXY_PLUGIN_RESOURCE_H_

#include <map>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "ipc/ipc_message.h"
#include "ipc/ipc_sender.h"
#include "ppapi/c/pp_errors.h"
#include "ppapi/proxy/connection.h"
#include "ppapi/proxy/plugin_resource_callback.h"
#include "ppapi/proxy/ppapi_message_utils.h"
#include "ppapi/proxy/ppapi_proxy_export.h"
#include "ppapi/proxy/resource_message_params.h"
#include "ppapi/proxy/resource_reply_thread_registrar.h"
#include "ppapi/shared_impl/resource.h"
#include "ppapi/shared_impl/tracked_callback.h"
namespace ppapi {
namespace proxy {

class PluginDispatcher;

class PPAPI_PROXY_EXPORT PluginResource : public Resource {
 public:
  enum Destination {
    RENDERER = 0,
    BROWSER = 1
  };

  PluginResource(Connection connection, PP_Instance instance);
  virtual ~PluginResource();

  
  
  
  bool sent_create_to_browser() const { return sent_create_to_browser_; }
  bool sent_create_to_renderer() const { return sent_create_to_renderer_; }

  
  
  
  virtual void OnReplyReceived(const proxy::ResourceMessageReplyParams& params,
                               const IPC::Message& msg) OVERRIDE;

  
  
  
  
  virtual void NotifyLastPluginRefWasDeleted() OVERRIDE;
  virtual void NotifyInstanceWasDeleted() OVERRIDE;


  
  void SendCreate(Destination dest, const IPC::Message& msg);

  
  
  
  
  
  void AttachToPendingHost(Destination dest, int pending_host_id);

  
  
  void Post(Destination dest, const IPC::Message& msg);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  template<typename ReplyMsgClass, typename CallbackType>
  int32_t Call(Destination dest,
               const IPC::Message& msg,
               const CallbackType& callback);

  
  
  
  
  
  
  
  
  
  
  
  template<typename ReplyMsgClass, typename CallbackType>
  int32_t Call(Destination dest,
               const IPC::Message& msg,
               const CallbackType& callback,
               scoped_refptr<TrackedCallback> reply_thread_hint);

  
  
  
  
  
  
  
  
  
  
  
  
  template <class ReplyMsgClass>
  int32_t SyncCall(Destination dest, const IPC::Message& msg);
  template <class ReplyMsgClass, class A>
  int32_t SyncCall(Destination dest, const IPC::Message& msg, A* a);
  template <class ReplyMsgClass, class A, class B>
  int32_t SyncCall(Destination dest, const IPC::Message& msg, A* a, B* b);
  template <class ReplyMsgClass, class A, class B, class C>
  int32_t SyncCall(Destination dest, const IPC::Message& msg, A* a, B* b, C* c);
  template <class ReplyMsgClass, class A, class B, class C, class D>
  int32_t SyncCall(
      Destination dest, const IPC::Message& msg, A* a, B* b, C* c, D* d);
  template <class ReplyMsgClass, class A, class B, class C, class D, class E>
  int32_t SyncCall(
      Destination dest, const IPC::Message& msg, A* a, B* b, C* c, D* d, E* e);

  int32_t GenericSyncCall(Destination dest,
                          const IPC::Message& msg,
                          IPC::Message* reply_msg,
                          ResourceMessageReplyParams* reply_params);

  const Connection& connection() { return connection_; }

 private:
  IPC::Sender* GetSender(Destination dest) {
    return dest == RENDERER ? connection_.renderer_sender :
                              connection_.browser_sender;
  }

  
  
  bool SendResourceCall(Destination dest,
                        const ResourceMessageCallParams& call_params,
                        const IPC::Message& nested_msg);

  int32_t GetNextSequence();

  Connection connection_;

  
  int32_t next_sequence_number_;

  bool sent_create_to_browser_;
  bool sent_create_to_renderer_;

  typedef std::map<int32_t, scoped_refptr<PluginResourceCallbackBase> >
      CallbackMap;
  CallbackMap callbacks_;

  scoped_refptr<ResourceReplyThreadRegistrar> resource_reply_thread_registrar_;

  DISALLOW_COPY_AND_ASSIGN(PluginResource);
};

template<typename ReplyMsgClass, typename CallbackType>
int32_t PluginResource::Call(Destination dest,
                             const IPC::Message& msg,
                             const CallbackType& callback) {
  return Call<ReplyMsgClass>(dest, msg, callback, NULL);
}

template<typename ReplyMsgClass, typename CallbackType>
int32_t PluginResource::Call(
    Destination dest,
    const IPC::Message& msg,
    const CallbackType& callback,
    scoped_refptr<TrackedCallback> reply_thread_hint) {
  TRACE_EVENT2("ppapi proxy", "PluginResource::Call",
               "Class", IPC_MESSAGE_ID_CLASS(msg.type()),
               "Line", IPC_MESSAGE_ID_LINE(msg.type()));
  ResourceMessageCallParams params(pp_resource(), next_sequence_number_++);
  
  
  scoped_refptr<PluginResourceCallbackBase> plugin_callback(
      new PluginResourceCallback<ReplyMsgClass, CallbackType>(callback));
  callbacks_.insert(std::make_pair(params.sequence(), plugin_callback));
  params.set_has_callback();

  if (resource_reply_thread_registrar_) {
    resource_reply_thread_registrar_->Register(
        pp_resource(), params.sequence(), reply_thread_hint);
  }
  SendResourceCall(dest, params, msg);
  return params.sequence();
}

template <class ReplyMsgClass>
int32_t PluginResource::SyncCall(Destination dest, const IPC::Message& msg) {
  IPC::Message reply;
  ResourceMessageReplyParams reply_params;
  return GenericSyncCall(dest, msg, &reply, &reply_params);
}

template <class ReplyMsgClass, class A>
int32_t PluginResource::SyncCall(
    Destination dest, const IPC::Message& msg, A* a) {
  IPC::Message reply;
  ResourceMessageReplyParams reply_params;
  int32_t result = GenericSyncCall(dest, msg, &reply, &reply_params);

  if (UnpackMessage<ReplyMsgClass>(reply, a))
    return result;
  return PP_ERROR_FAILED;
}

template <class ReplyMsgClass, class A, class B>
int32_t PluginResource::SyncCall(
    Destination dest, const IPC::Message& msg, A* a, B* b) {
  IPC::Message reply;
  ResourceMessageReplyParams reply_params;
  int32_t result = GenericSyncCall(dest, msg, &reply, &reply_params);

  if (UnpackMessage<ReplyMsgClass>(reply, a, b))
    return result;
  return PP_ERROR_FAILED;
}

template <class ReplyMsgClass, class A, class B, class C>
int32_t PluginResource::SyncCall(
    Destination dest, const IPC::Message& msg, A* a, B* b, C* c) {
  IPC::Message reply;
  ResourceMessageReplyParams reply_params;
  int32_t result = GenericSyncCall(dest, msg, &reply, &reply_params);

  if (UnpackMessage<ReplyMsgClass>(reply, a, b, c))
    return result;
  return PP_ERROR_FAILED;
}

template <class ReplyMsgClass, class A, class B, class C, class D>
int32_t PluginResource::SyncCall(
    Destination dest, const IPC::Message& msg, A* a, B* b, C* c, D* d) {
  IPC::Message reply;
  ResourceMessageReplyParams reply_params;
  int32_t result = GenericSyncCall(dest, msg, &reply, &reply_params);

  if (UnpackMessage<ReplyMsgClass>(reply, a, b, c, d))
    return result;
  return PP_ERROR_FAILED;
}

template <class ReplyMsgClass, class A, class B, class C, class D, class E>
int32_t PluginResource::SyncCall(
    Destination dest, const IPC::Message& msg, A* a, B* b, C* c, D* d, E* e) {
  IPC::Message reply;
  ResourceMessageReplyParams reply_params;
  int32_t result = GenericSyncCall(dest, msg, &reply, &reply_params);

  if (UnpackMessage<ReplyMsgClass>(reply, a, b, c, d, e))
    return result;
  return PP_ERROR_FAILED;
}

}  
}  

#endif  

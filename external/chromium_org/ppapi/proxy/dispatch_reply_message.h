// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef PPAPI_PROXY_DISPATCH_REPLY_MESSAGE_H_
#define PPAPI_PROXY_DISPATCH_REPLY_MESSAGE_H_

#include "base/callback.h"
#include "base/profiler/scoped_profile.h"  
#include "ipc/ipc_message_macros.h"
#include "ppapi/c/pp_errors.h"

namespace ppapi {
namespace proxy {

class ResourceMessageReplyParams;

template <class ObjT, class Method>
inline void DispatchResourceReply(ObjT* obj, Method method,
                                  const ResourceMessageReplyParams& params,
                                  const Tuple0& arg) {
  (obj->*method)(params);
}

template <class ObjT, class Method, class A>
inline void DispatchResourceReply(ObjT* obj, Method method,
                                  const ResourceMessageReplyParams& params,
                                  const Tuple1<A>& arg) {
  (obj->*method)(params, arg.a);
}

template<class ObjT, class Method, class A, class B>
inline void DispatchResourceReply(ObjT* obj, Method method,
                                  const ResourceMessageReplyParams& params,
                                  const Tuple2<A, B>& arg) {
  (obj->*method)(params, arg.a, arg.b);
}

template<class ObjT, class Method, class A, class B, class C>
inline void DispatchResourceReply(ObjT* obj, Method method,
                                  const ResourceMessageReplyParams& params,
                                  const Tuple3<A, B, C>& arg) {
  (obj->*method)(params, arg.a, arg.b, arg.c);
}

template<class ObjT, class Method, class A, class B, class C, class D>
inline void DispatchResourceReply(ObjT* obj, Method method,
                                  const ResourceMessageReplyParams& params,
                                  const Tuple4<A, B, C, D>& arg) {
  (obj->*method)(params, arg.a, arg.b, arg.c, arg.d);
}

template<class ObjT, class Method, class A, class B, class C, class D, class E>
inline void DispatchResourceReply(ObjT* obj, Method method,
                                  const ResourceMessageReplyParams& params,
                                  const Tuple5<A, B, C, D, E>& arg) {
  (obj->*method)(params, arg.a, arg.b, arg.c, arg.d, arg.e);
}

template<class MsgClass, class ObjT, class Method>
void DispatchResourceReplyOrDefaultParams(
    ObjT* obj,
    Method method,
    const ResourceMessageReplyParams& reply_params,
    const IPC::Message& msg) {
  typename MsgClass::Schema::Param msg_params;
  
  
  
  
  DCHECK(msg.type() == MsgClass::ID || msg.type() == 0)
      << "Resource reply message of unexpected type.";
  if (msg.type() == MsgClass::ID && MsgClass::Read(&msg, &msg_params)) {
    
    DispatchResourceReply(obj, method, reply_params, msg_params);
  } else {
    
    
    
    
    
    
    
    
    DispatchResourceReply(obj, method, reply_params,
        typename MsgClass::Schema::Param());
  }
}

template<class MsgClass, class Method>
void DispatchResourceReplyOrDefaultParams(
    base::Callback<void(const ResourceMessageReplyParams&)>* obj,
    Method method,
    const ResourceMessageReplyParams& reply_params,
    const IPC::Message& msg) {
  DCHECK(msg.type() == MsgClass::ID || msg.type() == 0)
      << "Resource reply message of unexpected type.";
  (obj->*method)(reply_params);
}

#define PPAPI_DISPATCH_PLUGIN_RESOURCE_CALL(msg_class, member_func) \
    case msg_class::ID: { \
      msg_class::Schema::Param p; \
      if (msg_class::Read(&ipc_message__, &p)) { \
        ppapi::proxy::DispatchResourceReply( \
            this, \
            &_IpcMessageHandlerClass::member_func, \
            params, p); \
      } else { \
        NOTREACHED(); \
      } \
      break; \
    }

#define PPAPI_DISPATCH_PLUGIN_RESOURCE_CALL_0(msg_class, member_func) \
  case msg_class::ID: { \
    member_func(params); \
    break; \
  }

#define PPAPI_DISPATCH_PLUGIN_RESOURCE_CALL_UNHANDLED(code) \
    default: { \
        code; \
    } \
    break;

}  
}  

#endif  

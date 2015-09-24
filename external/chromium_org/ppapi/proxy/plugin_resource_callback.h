// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_PLUGIN_RESOURCE_CALLBACK_H_
#define PPAPI_PROXY_PLUGIN_RESOURCE_CALLBACK_H_

#include "base/memory/ref_counted.h"
#include "ipc/ipc_message.h"
#include "ppapi/proxy/dispatch_reply_message.h"
#include "ppapi/proxy/resource_message_params.h"

namespace ppapi {
namespace proxy {

class PluginResourceCallbackBase
    : public base::RefCounted<PluginResourceCallbackBase> {
 public:
  virtual void Run(const ResourceMessageReplyParams& params,
                   const IPC::Message& msg) = 0;
 protected:
  friend class base::RefCounted<PluginResourceCallbackBase>;
  virtual ~PluginResourceCallbackBase() {}
};

template<typename MsgClass, typename CallbackType>
class PluginResourceCallback : public PluginResourceCallbackBase {
 public:
  explicit PluginResourceCallback(const CallbackType& callback)
      : callback_(callback) {}

  virtual void Run(
      const ResourceMessageReplyParams& reply_params,
      const IPC::Message& msg) OVERRIDE {
    DispatchResourceReplyOrDefaultParams<MsgClass>(
        &callback_, &CallbackType::Run, reply_params, msg);
  }

 private:
  virtual ~PluginResourceCallback() {}

  CallbackType callback_;
};

}  
}  

#endif  

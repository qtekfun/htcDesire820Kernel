// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_BROKER_DISPATCHER_H_
#define PPAPI_PROXY_BROKER_DISPATCHER_H_

#include "base/compiler_specific.h"
#include "ppapi/c/trusted/ppp_broker.h"
#include "ppapi/proxy/proxy_channel.h"

namespace ppapi {
namespace proxy {

class PPAPI_PROXY_EXPORT BrokerDispatcher : public ProxyChannel {
 public:
  virtual ~BrokerDispatcher();

  
  
  
  virtual bool InitBrokerWithChannel(ProxyChannel::Delegate* delegate,
                                     base::ProcessId peer_pid,
                                     const IPC::ChannelHandle& channel_handle,
                                     bool is_client);

  
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;

 protected:
  
  explicit BrokerDispatcher(PP_ConnectInstance_Func connect_instance);

  void OnMsgConnectToPlugin(PP_Instance instance,
                            IPC::PlatformFileForTransit handle,
                            int32_t* result);

  PP_ConnectInstance_Func connect_instance_;

 private:
  DISALLOW_COPY_AND_ASSIGN(BrokerDispatcher);
};

class PPAPI_PROXY_EXPORT BrokerHostDispatcher : public BrokerDispatcher {
 public:
  BrokerHostDispatcher();

  
  virtual void OnChannelError() OVERRIDE;
};

class PPAPI_PROXY_EXPORT BrokerSideDispatcher : public BrokerDispatcher {
 public:
  explicit BrokerSideDispatcher(PP_ConnectInstance_Func connect_instance);

  
  virtual void OnChannelError() OVERRIDE;
};

}  
}  

#endif  

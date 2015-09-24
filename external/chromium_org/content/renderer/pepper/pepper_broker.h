// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_PEPPER_BROKER_H_
#define CONTENT_RENDERER_PEPPER_PEPPER_BROKER_H_

#include "base/memory/ref_counted.h"
#include "base/process/process.h"
#include "base/sync_socket.h"
#include "content/common/content_export.h"
#include "content/renderer/pepper/ppb_broker_impl.h"
#include "ppapi/proxy/proxy_channel.h"

namespace IPC {
struct ChannelHandle;
}

namespace ppapi {
namespace proxy {
class BrokerDispatcher;
}
}

namespace content {

class PluginModule;

class CONTENT_EXPORT PepperBrokerDispatcherWrapper {
 public:
  PepperBrokerDispatcherWrapper();
  ~PepperBrokerDispatcherWrapper();

  bool Init(base::ProcessId broker_pid,
            const IPC::ChannelHandle& channel_handle);

  int32_t SendHandleToBroker(PP_Instance instance,
                             base::SyncSocket::Handle handle);

 private:
  scoped_ptr<ppapi::proxy::BrokerDispatcher> dispatcher_;
  scoped_ptr<ppapi::proxy::ProxyChannel::Delegate> dispatcher_delegate_;
};

class PepperBroker : public base::RefCountedThreadSafe<PepperBroker>{
 public:
  explicit PepperBroker(PluginModule* plugin_module);

  
  
  
  
  void Disconnect(PPB_Broker_Impl* client);

  
  void AddPendingConnect(PPB_Broker_Impl* client);

  
  void OnBrokerChannelConnected(base::ProcessId broker_pid,
                                const IPC::ChannelHandle& channel_handle);

  
  
  void OnBrokerPermissionResult(PPB_Broker_Impl* client,
                                bool result);

 private:
  friend class base::RefCountedThreadSafe<PepperBroker>;

  struct PendingConnection {
    PendingConnection();
    ~PendingConnection();

    bool is_authorized;
    base::WeakPtr<PPB_Broker_Impl> client;
  };

  virtual ~PepperBroker();

  
  void ReportFailureToClients(int error_code);

  
  void ConnectPluginToBroker(PPB_Broker_Impl* client);

  scoped_ptr<PepperBrokerDispatcherWrapper> dispatcher_;

  
  
  
  typedef std::map<PPB_Broker_Impl*, PendingConnection> ClientMap;
  ClientMap pending_connects_;

  
  
  PluginModule* plugin_module_;

  DISALLOW_COPY_AND_ASSIGN(PepperBroker);
};

}  

#endif  

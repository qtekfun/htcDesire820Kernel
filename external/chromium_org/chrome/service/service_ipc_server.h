// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_SERVICE_SERVICE_IPC_SERVER_H_
#define CHROME_SERVICE_SERVICE_IPC_SERVER_H_

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "ipc/ipc_channel_handle.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_sync_channel.h"
#include "ipc/ipc_sync_message_filter.h"
#include "ipc/ipc_sender.h"

namespace base {

class DictionaryValue;
class HistogramDeltaSerialization;

}  

class ServiceIPCServer : public IPC::Listener, public IPC::Sender {
 public:
  explicit ServiceIPCServer(const IPC::ChannelHandle& handle);
  virtual ~ServiceIPCServer();

  bool Init();

  
  virtual bool Send(IPC::Message* msg) OVERRIDE;

  IPC::SyncChannel* channel() { return channel_.get(); }

  
  
  IPC::SyncMessageFilter* sync_message_filter() {
    return sync_message_filter_.get();
  }

  bool is_client_connected() const { return client_connected_; }


 private:
  friend class MockServiceIPCServer;

  
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;
  virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

  
  void OnEnableCloudPrintProxyWithRobot(
      const std::string& robot_auth_code,
      const std::string& robot_email,
      const std::string& user_email,
      const base::DictionaryValue& user_settings);
  void OnGetCloudPrintProxyInfo();
  void OnGetHistograms();
  void OnDisableCloudPrintProxy();

  void OnShutdown();
  void OnUpdateAvailable();

  
  void CreateChannel();

  IPC::ChannelHandle channel_handle_;
  scoped_ptr<IPC::SyncChannel> channel_;
  
  bool client_connected_;

  
  scoped_refptr<IPC::SyncMessageFilter> sync_message_filter_;

  
  scoped_ptr<base::HistogramDeltaSerialization> histogram_delta_serializer_;

  DISALLOW_COPY_AND_ASSIGN(ServiceIPCServer);
};

#endif  

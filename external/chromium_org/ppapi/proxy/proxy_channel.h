// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_PROXY_CHANNEL_H_
#define PPAPI_PROXY_PROXY_CHANNEL_H_

#include "base/memory/scoped_ptr.h"
#include "base/process/process.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_platform_file.h"
#include "ipc/ipc_sender.h"
#include "ipc/ipc_sync_channel.h"
#include "ppapi/proxy/ppapi_proxy_export.h"

namespace base {
class MessageLoopProxy;
class WaitableEvent;
}

namespace IPC {
class TestSink;
}

namespace ppapi {
namespace proxy {

class PPAPI_PROXY_EXPORT ProxyChannel
    : public IPC::Listener,
      public IPC::Sender {
 public:
  class PPAPI_PROXY_EXPORT Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual base::MessageLoopProxy* GetIPCMessageLoop() = 0;

    
    
    virtual base::WaitableEvent* GetShutdownEvent() = 0;

    
    
    
    
    
    virtual IPC::PlatformFileForTransit ShareHandleWithRemote(
        base::PlatformFile handle,
        base::ProcessId remote_pid,
        bool should_close_source) = 0;
  };

  virtual ~ProxyChannel();

  
  
  
  
  void InitWithTestSink(IPC::TestSink* test_sink);

  
  
  
  
  
  
  IPC::PlatformFileForTransit ShareHandleWithRemote(
      base::PlatformFile handle,
      bool should_close_source);

  
  virtual bool Send(IPC::Message* msg) OVERRIDE;

  
  virtual void OnChannelError() OVERRIDE;

  
  IPC::SyncChannel* channel() const {
    return channel_.get();
  }

#if defined(OS_POSIX) && !defined(OS_NACL)
  int TakeRendererFD();
#endif

 protected:
  explicit ProxyChannel();

  
  
  
  virtual bool InitWithChannel(Delegate* delegate,
                               base::ProcessId peer_pid,
                               const IPC::ChannelHandle& channel_handle,
                               bool is_client);

  ProxyChannel::Delegate* delegate() const {
    return delegate_;
  }

 private:
  
  ProxyChannel::Delegate* delegate_;

  
  
  
  base::ProcessId peer_pid_;

  
  
  
  IPC::TestSink* test_sink_;

  
  
  scoped_ptr<IPC::SyncChannel> channel_;

  DISALLOW_COPY_AND_ASSIGN(ProxyChannel);
};

}  
}  

#endif  

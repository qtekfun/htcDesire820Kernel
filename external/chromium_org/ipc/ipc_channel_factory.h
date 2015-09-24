// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_IPC_CHANNEL_FACTORY_H_
#define IPC_IPC_CHANNEL_FACTORY_H_

#include "base/files/file_path.h"
#include "base/message_loop/message_loop.h"
#include "ipc/ipc_channel_handle.h"
#include "ipc/ipc_export.h"

namespace IPC {

class IPC_EXPORT ChannelFactory : public base::MessageLoopForIO::Watcher {
 public:
  class Delegate {
   public:
    
    
    
    virtual void OnClientConnected(const ChannelHandle& handle) = 0;

    
    virtual void OnListenError() = 0;
  };

  ChannelFactory(const base::FilePath& path, Delegate* delegate);

  virtual ~ChannelFactory();

  
  bool Listen();

  
  void Close();

 private:
  bool CreateSocket();
  virtual void OnFileCanReadWithoutBlocking(int fd) OVERRIDE;
  virtual void OnFileCanWriteWithoutBlocking(int fd) OVERRIDE;

  base::MessageLoopForIO::FileDescriptorWatcher
  server_listen_connection_watcher_;
  base::FilePath path_;
  Delegate* delegate_;
  int listen_fd_;

  DISALLOW_COPY_AND_ASSIGN(ChannelFactory);
};

}  

#endif  

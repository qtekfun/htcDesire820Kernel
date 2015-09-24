// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_CHILD_PROCESS_HOST_IMPL_H_
#define CONTENT_COMMON_CHILD_PROCESS_HOST_IMPL_H_

#include <string>
#include <vector>

#include "build/build_config.h"

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "base/memory/singleton.h"
#include "base/strings/string16.h"
#include "content/public/common/child_process_host.h"
#include "ipc/ipc_listener.h"

namespace base {
class FilePath;
}

namespace gfx {
struct GpuMemoryBufferHandle;
}

namespace content {
class ChildProcessHostDelegate;

class CONTENT_EXPORT ChildProcessHostImpl : public ChildProcessHost,
                                            public IPC::Listener {
 public:
  virtual ~ChildProcessHostImpl();

  
  
  static int kInvalidChildProcessId;

  
  static void AllocateSharedMemory(
      size_t buffer_size, base::ProcessHandle child_process,
      base::SharedMemoryHandle* handle);

  
  
  
  
  
  
  
  static int GenerateChildProcessUniqueId();

  
  virtual bool Send(IPC::Message* message) OVERRIDE;
  virtual void ForceShutdown() OVERRIDE;
  virtual std::string CreateChannel() OVERRIDE;
  virtual bool IsChannelOpening() OVERRIDE;
  virtual void AddFilter(IPC::ChannelProxy::MessageFilter* filter) OVERRIDE;
#if defined(OS_POSIX)
  virtual int TakeClientFileDescriptor() OVERRIDE;
#endif

 private:
  friend class ChildProcessHost;

  explicit ChildProcessHostImpl(ChildProcessHostDelegate* delegate);

  
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;
  virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

  
  void OnShutdownRequest();
  void OnAllocateSharedMemory(uint32 buffer_size,
                              base::SharedMemoryHandle* handle);
  void OnAllocateGpuMemoryBuffer(uint32 width,
                                 uint32 height,
                                 uint32 internalformat,
                                 gfx::GpuMemoryBufferHandle* handle);

  ChildProcessHostDelegate* delegate_;
  base::ProcessHandle peer_handle_;
  bool opening_channel_;  
  scoped_ptr<IPC::Channel> channel_;
  std::string channel_id_;

  
  
  
  std::vector<scoped_refptr<IPC::ChannelProxy::MessageFilter> > filters_;

  DISALLOW_COPY_AND_ASSIGN(ChildProcessHostImpl);
};

}  

#endif  

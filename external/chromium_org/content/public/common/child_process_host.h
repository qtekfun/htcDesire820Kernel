// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PULIC_COMMON_CHILD_PROCESS_HOST_H_
#define CONTENT_PULIC_COMMON_CHILD_PROCESS_HOST_H_

#include "build/build_config.h"
#include "content/common/content_export.h"
#include "ipc/ipc_channel_proxy.h"

namespace base {
class FilePath;
}

namespace content {

class ChildProcessHostDelegate;

class CONTENT_EXPORT ChildProcessHost : public IPC::Sender {
 public:
  virtual ~ChildProcessHost() {}

  
  static ChildProcessHost* Create(
      ChildProcessHostDelegate* delegate);

  
  
  enum {
    
    CHILD_NORMAL = 0,

#if defined(OS_LINUX)
    
    
    
    
    
    
    
    CHILD_ALLOW_SELF = 1 << 0,
#elif defined(OS_MACOSX)

    
    
    
    
    
    
    CHILD_NO_PIE = 1 << 1,

    
    
    
    
    
    
    CHILD_ALLOW_HEAP_EXECUTION = 1 << 2,
#endif
  };

  
  
  
  
  
  
  
  
  
  
  static base::FilePath GetChildPath(int flags);

  
  
  virtual void ForceShutdown() = 0;

  
  
  virtual std::string CreateChannel() = 0;

  
  virtual bool IsChannelOpening() = 0;

  
  virtual void AddFilter(IPC::ChannelProxy::MessageFilter* filter) = 0;

#if defined(OS_POSIX)
  
  virtual int TakeClientFileDescriptor() = 0;
#endif
};

};  

#endif  

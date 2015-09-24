// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_NACL_NACL_IPC_ADAPTER_H_
#define CHROME_NACL_NACL_IPC_ADAPTER_H_

#include <map>
#include <queue>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/shared_memory.h"
#include "base/pickle.h"
#include "base/synchronization/condition_variable.h"
#include "base/synchronization/lock.h"
#include "base/task_runner.h"
#include "ipc/ipc_listener.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/proxy/nacl_message_scanner.h"

struct NaClDesc;
struct NaClImcTypedMsgHdr;
struct PP_Size;

namespace IPC {
class Channel;
struct ChannelHandle;
}

namespace ppapi {
class HostResource;
}

class NaClIPCAdapter : public base::RefCountedThreadSafe<NaClIPCAdapter>,
                       public IPC::Listener {
 public:
  
  
  
  
  
#pragma pack(push, 4)
  struct NaClMessageHeader : public Pickle::Header {
    int32 routing;
    uint32 type;
    uint32 flags;
    uint16 num_fds;
    uint16 pad;
  };
#pragma pack(pop)

  
  
  
  
  
  
  
  NaClIPCAdapter(const IPC::ChannelHandle& handle, base::TaskRunner* runner);

  
  
  NaClIPCAdapter(scoped_ptr<IPC::Channel> channel, base::TaskRunner* runner);

  
  
  
  void ConnectChannel();

  // Implementation of sendmsg. Returns the number of bytes written or -1 on
  
  int Send(const NaClImcTypedMsgHdr* msg);

  
  
  
  int BlockingReceive(NaClImcTypedMsgHdr* msg);

  
  void CloseChannel();

  
  
  NaClDesc* MakeNaClDesc();

#if defined(OS_POSIX)
  int TakeClientFileDescriptor();
#endif

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

 private:
  friend class base::RefCountedThreadSafe<NaClIPCAdapter>;

  class RewrittenMessage;

  
  
  struct LockedData {
    LockedData();
    ~LockedData();

    
    
    std::queue< scoped_refptr<RewrittenMessage> > to_be_received_;

    ppapi::proxy::NaClMessageScanner nacl_msg_scanner_;

    
    
    
    
    
    
    
    std::string to_be_sent_;

    bool channel_closed_;
  };

  
  
  struct IOThreadData {
    IOThreadData();
    ~IOThreadData();

    scoped_ptr<IPC::Channel> channel_;
  };

  virtual ~NaClIPCAdapter();

  
  int LockedReceive(NaClImcTypedMsgHdr* msg);

  
  bool SendCompleteMessage(const char* buffer, size_t buffer_len);

  
  
  
  void ClearToBeSent();

  void ConnectChannelOnIOThread();
  void CloseChannelOnIOThread();
  void SendMessageOnIOThread(scoped_ptr<IPC::Message> message);

  
  
  void SaveMessage(const IPC::Message& message,
                   RewrittenMessage* rewritten_message);

  base::Lock lock_;
  base::ConditionVariable cond_var_;

  scoped_refptr<base::TaskRunner> task_runner_;

  
  LockedData locked_data_;

  
  IOThreadData io_thread_data_;

  DISALLOW_COPY_AND_ASSIGN(NaClIPCAdapter);
};

int TranslatePepperFileReadWriteOpenFlagsForTesting(int32_t pp_open_flags);

#endif  

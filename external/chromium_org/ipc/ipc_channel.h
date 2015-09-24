// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_IPC_CHANNEL_H_
#define IPC_IPC_CHANNEL_H_

#include <string>

#if defined(OS_POSIX)
#include <sys/types.h>
#endif

#include "base/compiler_specific.h"
#include "base/process/process.h"
#include "ipc/ipc_channel_handle.h"
#include "ipc/ipc_message.h"
#include "ipc/ipc_sender.h"

namespace IPC {

class Listener;



class IPC_EXPORT Channel : public Sender {
  
  friend class ChannelTest;

 public:
  
  enum ModeFlags {
    MODE_NO_FLAG = 0x0,
    MODE_SERVER_FLAG = 0x1,
    MODE_CLIENT_FLAG = 0x2,
    MODE_NAMED_FLAG = 0x4,
#if defined(OS_POSIX)
    MODE_OPEN_ACCESS_FLAG = 0x8, 
#endif
  };

  
  enum Mode {
    MODE_NONE = MODE_NO_FLAG,
    MODE_SERVER = MODE_SERVER_FLAG,
    MODE_CLIENT = MODE_CLIENT_FLAG,
    
    
    
    
    
    MODE_NAMED_SERVER = MODE_SERVER_FLAG | MODE_NAMED_FLAG,
    MODE_NAMED_CLIENT = MODE_CLIENT_FLAG | MODE_NAMED_FLAG,
#if defined(OS_POSIX)
    
    
    
    MODE_OPEN_NAMED_SERVER = MODE_OPEN_ACCESS_FLAG | MODE_SERVER_FLAG |
                             MODE_NAMED_FLAG
#endif
  };

  
  
  
  enum {
    
    
    
    
    HELLO_MESSAGE_TYPE = kuint16max,
    
    
    
    
    
    
    CLOSE_FD_MESSAGE_TYPE = HELLO_MESSAGE_TYPE - 1
  };

  
  
  static const size_t kMaximumMessageSize = 128 * 1024 * 1024;

  
  static const size_t kReadBufferSize = 4 * 1024;

  
  
  
  
  
  
  
  
  
  
  
  
  
  Channel(const IPC::ChannelHandle &channel_handle, Mode mode,
          Listener* listener);

  virtual ~Channel();

  
  
  
  
  
  bool Connect() WARN_UNUSED_RESULT;

  
  
  
  
  
  void Close();

  
  
  
  
  
  
  
  
  
  base::ProcessId peer_pid() const;

  
  
  
  
  virtual bool Send(Message* message) OVERRIDE;

#if defined(OS_POSIX)
  
  
  
  
  int GetClientFileDescriptor() const;

  
  
  
  int TakeClientFileDescriptor();

  
  
  
  

  
  bool AcceptsConnections() const;

  
  
  bool HasAcceptedConnection() const;

  
  
  bool GetPeerEuid(uid_t* peer_euid) const;

  
  
  void ResetToAcceptingConnectionState();
#endif  

  
  
  static bool IsNamedServerInitialized(const std::string& channel_id);

#if !defined(OS_NACL)
  
  static std::string GenerateUniqueRandomChannelID();

  
  
  
  
  static std::string GenerateVerifiedChannelID(const std::string& prefix);
#endif

#if defined(OS_LINUX)
  
  
  
  static void SetGlobalPid(int pid);
#endif

 protected:
  
  
  
  
  Channel() : channel_impl_(0) { }

 private:
  
  class ChannelImpl;
  ChannelImpl *channel_impl_;
};

}  

#endif  

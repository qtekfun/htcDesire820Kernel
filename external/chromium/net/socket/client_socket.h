// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_CLIENT_SOCKET_H_
#define NET_SOCKET_CLIENT_SOCKET_H_
#pragma once

#include "net/base/net_log.h"
#include "net/socket/socket.h"

namespace net {

class AddressList;
class IPEndPoint;

class ClientSocket : public Socket {
 public:
  virtual ~ClientSocket() {}

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int Connect(CompletionCallback* callback
#ifdef ANDROID
                      , bool wait_for_connect
                      , bool valid_uid
                      , uid_t calling_uid
#endif
                     ) = 0;

  
  
  
  
  
  
  
  virtual void Disconnect() = 0;

  
  
  virtual bool IsConnected() const = 0;

  
  
  
  virtual bool IsConnectedAndIdle() const = 0;

  
  
  
  virtual int GetPeerAddress(AddressList* address) const = 0;

  
  
  virtual int GetLocalAddress(IPEndPoint* address) const = 0;

  
  virtual const BoundNetLog& NetLog() const = 0;

  
  
  
  virtual void SetSubresourceSpeculation() = 0;
  virtual void SetOmniboxSpeculation() = 0;

  
  
  
  virtual bool WasEverUsed() const = 0;

  
  
  virtual bool UsingTCPFastOpen() const = 0;

 protected:
  
  
  
  
  
  class UseHistory {
   public:
    UseHistory();
    ~UseHistory();

    
    
    void Reset();

    void set_was_ever_connected();
    void set_was_used_to_convey_data();

    
    
    
    
    void set_subresource_speculation();
    void set_omnibox_speculation();

    bool was_used_to_convey_data() const;

   private:
    
    void EmitPreconnectionHistograms() const;
    
    bool was_ever_connected_;
    
    bool was_used_to_convey_data_;

    
    
    bool omnibox_speculation_;
    bool subresource_speculation_;
    DISALLOW_COPY_AND_ASSIGN(UseHistory);
  };

  
  
  
  void LogByteTransfer(const BoundNetLog& net_log, NetLog::EventType event_type,
                       int byte_count, char* bytes) const;
};

}  

#endif  

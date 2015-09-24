// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_SESSION_H_
#define REMOTING_PROTOCOL_SESSION_H_

#include <string>

#include "remoting/protocol/errors.h"
#include "remoting/protocol/session_config.h"

namespace net {
class IPEndPoint;
}  

namespace remoting {
namespace protocol {

class ChannelFactory;
struct TransportRoute;

class Session {
 public:
  enum State {
    
    INITIALIZING,

    
    CONNECTING,

    
    ACCEPTING,

    
    CONNECTED,

    
    AUTHENTICATED,

    
    CLOSED,

    
    FAILED,
  };

  class EventHandler {
   public:
    EventHandler() {}
    virtual ~EventHandler() {}

    
    
    
    virtual void OnSessionStateChange(State state) = 0;

    
    
    
    virtual void OnSessionRouteChange(const std::string& channel_name,
                                      const TransportRoute& route) = 0;

    
    
    
    virtual void OnSessionChannelReady(const std::string& channel_name,
                                       bool ready) {}
  };


  Session() {}
  virtual ~Session() {}

  
  
  virtual void SetEventHandler(EventHandler* event_handler) = 0;

  
  virtual ErrorCode error() = 0;

  
  virtual const std::string& jid() = 0;

  
  
  
  virtual const CandidateSessionConfig* candidate_config() = 0;

  
  
  virtual const SessionConfig& config() = 0;

  
  
  
  virtual void set_config(const SessionConfig& config) = 0;

  
  
  
  virtual ChannelFactory* GetTransportChannelFactory() = 0;
  virtual ChannelFactory* GetMultiplexedChannelFactory() = 0;

  
  
  
  virtual void Close() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(Session);
};

}  
}  

#endif  

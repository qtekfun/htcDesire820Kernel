// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_P2P_TRANSPORT_H_
#define WEBKIT_GLUE_P2P_TRANSPORT_H_

#include <string>

namespace net {
class Socket;
}  

namespace webkit_glue {

class P2PTransport {
 public:
  enum State {
    STATE_NONE = 0,
    STATE_WRITABLE = 1,
    STATE_READABLE = 2,
  };

  class EventHandler {
   public:
    virtual ~EventHandler() {}

    
    virtual void OnCandidateReady(const std::string& address) = 0;

    
    virtual void OnStateChange(State state) = 0;
  };

  virtual ~P2PTransport() {}

  
  
  virtual bool Init(const std::string& name,
                    const std::string& config,
                    EventHandler* event_handler) = 0;

  
  
  virtual bool AddRemoteCandidate(const std::string& address) = 0;

  
  
  
  virtual net::Socket* GetChannel() = 0;
};

}  

#endif  

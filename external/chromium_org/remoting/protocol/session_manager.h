// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef REMOTING_PROTOCOL_SESSION_MANAGER_H_
#define REMOTING_PROTOCOL_SESSION_MANAGER_H_

#include <string>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/threading/non_thread_safe.h"
#include "remoting/protocol/session.h"

namespace remoting {

class SignalStrategy;

namespace protocol {

class Authenticator;
class AuthenticatorFactory;

class SessionManager : public base::NonThreadSafe {
 public:
  SessionManager() {}
  virtual ~SessionManager() {}

  enum IncomingSessionResponse {
    
    ACCEPT,

    
    INCOMPATIBLE,

    
    
    OVERLOAD,

    
    
    DECLINE,
  };

  class Listener {
   public:
    Listener() {}

    
    
    
    virtual void OnSessionManagerReady() = 0;

    
    
    
    
    
    
    
    
    virtual void OnIncomingSession(Session* session,
                                   IncomingSessionResponse* response) = 0;

   protected:
    ~Listener() {}
  };

  
  
  virtual void Init(SignalStrategy* signal_strategy,
                    Listener* listener) = 0;

  
  
  
  
  
  
  
  virtual scoped_ptr<Session> Connect(
      const std::string& host_jid,
      scoped_ptr<Authenticator> authenticator,
      scoped_ptr<CandidateSessionConfig> config) = 0;

  
  
  
  virtual void Close() = 0;

  
  
  
  
  
  virtual void set_authenticator_factory(
      scoped_ptr<AuthenticatorFactory> authenticator_factory) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(SessionManager);
};

}  
}  

#endif  

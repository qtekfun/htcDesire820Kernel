// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_JINGLE_GLUE_SIGNAL_STRATEGY_H_
#define REMOTING_JINGLE_GLUE_SIGNAL_STRATEGY_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

namespace buzz {
class XmlElement;
}  

namespace remoting {

class SignalStrategy {
 public:
  enum State {
    
    CONNECTING,

    
    CONNECTED,

    
    
    DISCONNECTED,
  };

  enum Error {
    OK,
    AUTHENTICATION_FAILED,
    NETWORK_ERROR,
  };

  
  
  
  class Listener {
   public:
    virtual ~Listener() {}

    
    
    
    virtual void OnSignalStrategyStateChange(State state) = 0;

    
    
    
    virtual bool OnSignalStrategyIncomingStanza(
        const buzz::XmlElement* stanza) = 0;
  };

  SignalStrategy() {}
  virtual ~SignalStrategy() {}

  
  
  
  
  virtual void Connect() = 0;

  
  
  virtual void Disconnect() = 0;

  
  virtual State GetState() const = 0;

  
  virtual Error GetError() const = 0;

  
  virtual std::string GetLocalJid() const = 0;

  
  
  
  virtual void AddListener(Listener* listener) = 0;

  
  virtual void RemoveListener(Listener* listener) = 0;

  
  virtual bool SendStanza(scoped_ptr<buzz::XmlElement> stanza) = 0;

  
  
  virtual std::string GetNextId() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(SignalStrategy);
};

}  

#endif  

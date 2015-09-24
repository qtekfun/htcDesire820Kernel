// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_HOST_CHANGE_NOTIFICATION_LISTENER_H
#define REMOTING_HOST_HOST_CHANGE_NOTIFICATION_LISTENER_H

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "remoting/jingle_glue/signal_strategy.h"

namespace buzz {
class XmlElement;
}  

namespace remoting {

class HostChangeNotificationListener : public SignalStrategy::Listener {
 public:
  class Listener {
   protected:
    virtual ~Listener() {}
    
   public:
    virtual void OnHostDeleted() = 0;
  };

  
  HostChangeNotificationListener(Listener* listener,
                                 const std::string& host_id,
                                 SignalStrategy* signal_strategy,
                                 const std::string& directory_bot_jid);
  virtual ~HostChangeNotificationListener();

  
  virtual void OnSignalStrategyStateChange(
      SignalStrategy::State state) OVERRIDE;
  virtual bool OnSignalStrategyIncomingStanza(
      const buzz::XmlElement* stanza) OVERRIDE;

 private:
  void OnHostDeleted();

  Listener* listener_;
  std::string host_id_;
  SignalStrategy* signal_strategy_;
  std::string directory_bot_jid_;
  base::WeakPtrFactory<HostChangeNotificationListener> weak_factory_;
  DISALLOW_COPY_AND_ASSIGN(HostChangeNotificationListener);
};

}  

#endif  

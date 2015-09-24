// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef REMOTING_JINGLE_GLUE_XMPP_SIGNAL_STRATEGY_H_
#define REMOTING_JINGLE_GLUE_XMPP_SIGNAL_STRATEGY_H_

#include "remoting/jingle_glue/signal_strategy.h"

#include <vector>

#include "base/compiler_specific.h"
#include "base/observer_list.h"
#include "base/threading/non_thread_safe.h"
#include "base/timer/timer.h"
#include "third_party/libjingle/source/talk/base/sigslot.h"
#include "third_party/libjingle/source/talk/xmpp/xmppclient.h"

namespace net {
class ClientSocketFactory;
class URLRequestContextGetter;
}  

namespace talk_base {
class TaskRunner;
}  

namespace remoting {

class JingleThread;

class XmppSignalStrategy : public base::NonThreadSafe,
                           public SignalStrategy,
                           public buzz::XmppStanzaHandler,
                           public sigslot::has_slots<> {
 public:
  
  struct XmppServerConfig {
    XmppServerConfig();
    ~XmppServerConfig();

    std::string host;
    int port;
    bool use_tls;

    std::string username;
    std::string auth_token;
    std::string auth_service;
  };

  XmppSignalStrategy(
      net::ClientSocketFactory* socket_factory,
      scoped_refptr<net::URLRequestContextGetter> request_context_getter,
      const XmppServerConfig& xmpp_server_config);
  virtual ~XmppSignalStrategy();

  
  virtual void Connect() OVERRIDE;
  virtual void Disconnect() OVERRIDE;
  virtual State GetState() const OVERRIDE;
  virtual Error GetError() const OVERRIDE;
  virtual std::string GetLocalJid() const OVERRIDE;
  virtual void AddListener(Listener* listener) OVERRIDE;
  virtual void RemoveListener(Listener* listener) OVERRIDE;
  virtual bool SendStanza(scoped_ptr<buzz::XmlElement> stanza) OVERRIDE;
  virtual std::string GetNextId() OVERRIDE;

  
  virtual bool HandleStanza(const buzz::XmlElement* stanza) OVERRIDE;

  
  
  
  void SetAuthInfo(const std::string& username,
                   const std::string& auth_token,
                   const std::string& auth_service);

  
  
  void SetResourceName(const std::string& resource_name);

 private:
  static buzz::PreXmppAuth* CreatePreXmppAuth(
      const buzz::XmppClientSettings& settings);

  void OnConnectionStateChanged(buzz::XmppEngine::State state);
  void SetState(State new_state);

  void SendKeepAlive();

  net::ClientSocketFactory* socket_factory_;
  scoped_refptr<net::URLRequestContextGetter> request_context_getter_;
  std::string resource_name_;
  scoped_ptr<talk_base::TaskRunner> task_runner_;
  buzz::XmppClient* xmpp_client_;
  XmppServerConfig xmpp_server_config_;

  State state_;
  Error error_;

  ObserverList<Listener, true> listeners_;

  base::RepeatingTimer<XmppSignalStrategy> keep_alive_timer_;

  DISALLOW_COPY_AND_ASSIGN(XmppSignalStrategy);
};

}  

#endif  

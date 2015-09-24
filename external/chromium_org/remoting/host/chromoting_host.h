// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_CHROMOTING_HOST_H_
#define REMOTING_HOST_CHROMOTING_HOST_H_

#include <list>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/threading/non_thread_safe.h"
#include "base/threading/thread.h"
#include "net/base/backoff_entry.h"
#include "remoting/host/client_session.h"
#include "remoting/host/host_status_monitor.h"
#include "remoting/host/host_status_observer.h"
#include "remoting/protocol/authenticator.h"
#include "remoting/protocol/connection_to_client.h"
#include "remoting/protocol/pairing_registry.h"
#include "remoting/protocol/session_manager.h"

namespace base {
class SingleThreadTaskRunner;
}  

namespace remoting {

namespace protocol {
class InputStub;
class SessionConfig;
class CandidateSessionConfig;
}  

class DesktopEnvironmentFactory;

class ChromotingHost : public base::NonThreadSafe,
                       public ClientSession::EventHandler,
                       public protocol::SessionManager::Listener,
                       public HostStatusMonitor {
 public:
  
  
  ChromotingHost(
      SignalStrategy* signal_strategy,
      DesktopEnvironmentFactory* desktop_environment_factory,
      scoped_ptr<protocol::SessionManager> session_manager,
      scoped_refptr<base::SingleThreadTaskRunner> audio_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> input_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> video_capture_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> video_encode_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> network_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> ui_task_runner);
  virtual ~ChromotingHost();

  
  
  
  
  
  
  void Start(const std::string& host_owner);

  
  virtual void AddStatusObserver(HostStatusObserver* observer) OVERRIDE;
  virtual void RemoveStatusObserver(HostStatusObserver* observer) OVERRIDE;

  
  
  
  void RejectAuthenticatingClient();

  
  
  
  
  
  
  void SetAuthenticatorFactory(
      scoped_ptr<protocol::AuthenticatorFactory> authenticator_factory);

  
  
  void SetEnableCurtaining(bool enable);

  
  
  void SetMaximumSessionDuration(const base::TimeDelta& max_session_duration);

  
  
  virtual bool OnSessionAuthenticated(ClientSession* client) OVERRIDE;
  virtual void OnSessionChannelsConnected(ClientSession* client) OVERRIDE;
  virtual void OnSessionAuthenticationFailed(ClientSession* client) OVERRIDE;
  virtual void OnSessionClosed(ClientSession* session) OVERRIDE;
  virtual void OnSessionSequenceNumber(ClientSession* session,
                                       int64 sequence_number) OVERRIDE;
  virtual void OnSessionRouteChange(
      ClientSession* session,
      const std::string& channel_name,
      const protocol::TransportRoute& route) OVERRIDE;

  
  virtual void OnSessionManagerReady() OVERRIDE;
  virtual void OnIncomingSession(
      protocol::Session* session,
      protocol::SessionManager::IncomingSessionResponse* response) OVERRIDE;

  
  void set_protocol_config(scoped_ptr<protocol::CandidateSessionConfig> config);

  base::WeakPtr<ChromotingHost> AsWeakPtr() {
    return weak_factory_.GetWeakPtr();
  }

  
  
  scoped_refptr<protocol::PairingRegistry> pairing_registry() const {
    return pairing_registry_;
  }
  void set_pairing_registry(
      scoped_refptr<protocol::PairingRegistry> pairing_registry) {
    pairing_registry_ = pairing_registry;
  }

 private:
  friend class ChromotingHostTest;

  typedef std::list<ClientSession*> ClientList;

  
  
  
  void DisconnectAllClients();

  
  

  
  DesktopEnvironmentFactory* desktop_environment_factory_;
  scoped_ptr<protocol::SessionManager> session_manager_;
  scoped_refptr<base::SingleThreadTaskRunner> audio_task_runner_;
  scoped_refptr<base::SingleThreadTaskRunner> input_task_runner_;
  scoped_refptr<base::SingleThreadTaskRunner> video_capture_task_runner_;
  scoped_refptr<base::SingleThreadTaskRunner> video_encode_task_runner_;
  scoped_refptr<base::SingleThreadTaskRunner> network_task_runner_;
  scoped_refptr<base::SingleThreadTaskRunner> ui_task_runner_;

  
  SignalStrategy* signal_strategy_;

  
  ObserverList<HostStatusObserver> status_observers_;

  
  ClientList clients_;

  
  bool started_;

  
  scoped_ptr<protocol::CandidateSessionConfig> protocol_config_;

  
  net::BackoffEntry login_backoff_;

  
  bool authenticating_client_;
  bool reject_authenticating_client_;

  
  bool enable_curtaining_;

  
  base::TimeDelta max_session_duration_;

  
  scoped_refptr<protocol::PairingRegistry> pairing_registry_;

  base::WeakPtrFactory<ChromotingHost> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(ChromotingHost);
};

}  

#endif  

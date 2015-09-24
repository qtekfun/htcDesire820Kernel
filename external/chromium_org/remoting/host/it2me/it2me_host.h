// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_IT2ME_IT2ME_HOST_H_
#define REMOTING_HOST_IT2ME_IT2ME_HOST_H_

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/single_thread_task_runner.h"
#include "remoting/host/log_to_server.h"
#include "remoting/jingle_glue/xmpp_signal_strategy.h"

namespace remoting {

class RegisterSupportHostRequest;
class HostNPScriptObject;
class DesktopEnvironmentFactory;
class HostEventLogger;
class ChromotingHost;
class ChromotingHostContext;

namespace policy_hack {

class PolicyWatcher;

}  

enum It2MeHostState {
  kDisconnected,
  kStarting,
  kRequestedAccessCode,
  kReceivedAccessCode,
  kConnected,
  kDisconnecting,
  kError,
  kInvalidDomainError
};

class It2MeHost
    : public base::RefCountedThreadSafe<It2MeHost>,
      public HostStatusObserver {
 public:

  class Observer {
   public:
    virtual void OnClientAuthenticated(const std::string& client_username) = 0;
    virtual void OnStoreAccessCode(const std::string& access_code,
                                   base::TimeDelta access_code_lifetime) = 0;
    virtual void OnNatPolicyChanged(bool nat_traversal_enabled) = 0;
    virtual void OnStateChanged(It2MeHostState state) = 0;
  };

  It2MeHost(
      ChromotingHostContext* context,
      scoped_refptr<base::SingleThreadTaskRunner> task_runner,
      base::WeakPtr<It2MeHost::Observer> observer,
      const XmppSignalStrategy::XmppServerConfig& xmpp_server_config,
      const std::string& directory_bot_jid);

  

  
  virtual void Connect();

  
  
  virtual void Disconnect();

  
  
  virtual void RequestNatPolicy();

  
  virtual void OnAccessDenied(const std::string& jid) OVERRIDE;
  virtual void OnClientAuthenticated(const std::string& jid) OVERRIDE;
  virtual void OnClientDisconnected(const std::string& jid) OVERRIDE;

  void SetStateForTesting(It2MeHostState state) { SetState(state); }

 protected:
  friend class base::RefCountedThreadSafe<It2MeHost>;

  virtual ~It2MeHost();

  ChromotingHostContext* host_context() { return host_context_; }
  scoped_refptr<base::SingleThreadTaskRunner> task_runner() {
    return task_runner_;
  }
  base::WeakPtr<It2MeHost::Observer> observer() { return observer_; }

 private:
  
  void SetState(It2MeHostState state);

  
  bool IsConnected() const;

  
  void ReadPolicyAndConnect();

  
  void FinishConnect();

  
  void OnReceivedSupportID(bool success,
                           const std::string& support_id,
                           const base::TimeDelta& lifetime);

  
  
  void ShutdownOnNetworkThread();

  
  
  void ShutdownOnUiThread();

  
  void OnPolicyUpdate(scoped_ptr<base::DictionaryValue> policies);

  
  void UpdateNatPolicy(bool nat_traversal_enabled);
  void UpdateHostDomainPolicy(const std::string& host_domain);

  

  
  
  ChromotingHostContext* host_context_;
  scoped_refptr<base::SingleThreadTaskRunner> task_runner_;
  base::WeakPtr<It2MeHost::Observer> observer_;
  XmppSignalStrategy::XmppServerConfig xmpp_server_config_;
  std::string directory_bot_jid_;

  It2MeHostState state_;

  scoped_refptr<RsaKeyPair> host_key_pair_;
  scoped_ptr<SignalStrategy> signal_strategy_;
  scoped_ptr<RegisterSupportHostRequest> register_request_;
  scoped_ptr<LogToServer> log_to_server_;
  scoped_ptr<DesktopEnvironmentFactory> desktop_environment_factory_;
  scoped_ptr<HostEventLogger> host_event_logger_;

  scoped_ptr<ChromotingHost> host_;
  int failed_login_attempts_;

  scoped_ptr<policy_hack::PolicyWatcher> policy_watcher_;

  
  bool nat_traversal_enabled_;

  
  std::string required_host_domain_;

  
  
  
  bool policy_received_;

  
  
  
  
  base::Closure pending_connect_;

  DISALLOW_COPY_AND_ASSIGN(It2MeHost);
};

class It2MeHostFactory {
 public:
  It2MeHostFactory();
  virtual ~It2MeHostFactory();

  virtual scoped_refptr<It2MeHost> CreateIt2MeHost(
      ChromotingHostContext* context,
      scoped_refptr<base::SingleThreadTaskRunner> task_runner,
      base::WeakPtr<It2MeHost::Observer> observer,
      const XmppSignalStrategy::XmppServerConfig& xmpp_server_config,
      const std::string& directory_bot_jid);

 private:
  DISALLOW_COPY_AND_ASSIGN(It2MeHostFactory);
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_PLUGIN_HOST_SCRIPT_OBJECT_H_
#define REMOTING_HOST_PLUGIN_HOST_SCRIPT_OBJECT_H_

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/thread_task_runner_handle.h"
#include "base/time/time.h"
#include "remoting/base/auto_thread_task_runner.h"
#include "remoting/host/it2me/it2me_host.h"
#include "remoting/host/plugin/host_plugin_utils.h"
#include "remoting/host/setup/daemon_controller.h"
#include "remoting/jingle_glue/xmpp_signal_strategy.h"
#include "remoting/protocol/pairing_registry.h"

namespace remoting {

class HostNPScriptObject : public It2MeHost::Observer {
 public:
  HostNPScriptObject(NPP plugin,
                     NPObject* parent,
                     scoped_refptr<AutoThreadTaskRunner> plugin_task_runner);
  virtual ~HostNPScriptObject();

  
  bool HasMethod(const std::string& method_name);
  bool InvokeDefault(const NPVariant* args,
                     uint32_t arg_count,
                     NPVariant* result);
  bool Invoke(const std::string& method_name,
              const NPVariant* args,
              uint32_t arg_count,
              NPVariant* result);
  bool HasProperty(const std::string& property_name);
  bool GetProperty(const std::string& property_name, NPVariant* result);
  bool SetProperty(const std::string& property_name, const NPVariant* value);
  bool RemoveProperty(const std::string& property_name);
  bool Enumerate(std::vector<std::string>* values);

  
  
  
  void PostLogDebugInfo(const std::string& message);

  void SetWindow(NPWindow* np_window);

 private:
  
  

  
  
  
  bool Connect(const NPVariant* args, uint32_t arg_count, NPVariant* result);

  
  bool Disconnect(const NPVariant* args, uint32_t arg_count, NPVariant* result);

  
  
  
  
  bool Localize(const NPVariant* args, uint32_t arg_count, NPVariant* result);

  
  

  
  bool ClearPairedClients(const NPVariant* args,
                          uint32_t arg_count,
                          NPVariant* result);

  
  bool DeletePairedClient(const NPVariant* args,
                          uint32_t arg_count,
                          NPVariant* result);

  
  
  
  bool GetHostName(const NPVariant* args,
                   uint32_t arg_count,
                   NPVariant* result);

  
  
  
  
  
  
  bool GetPinHash(const NPVariant* args,
                  uint32_t arg_count,
                  NPVariant* result);

  
  
  
  
  
  bool GenerateKeyPair(const NPVariant* args,
                       uint32_t arg_count,
                       NPVariant* result);

  
  
  
  bool UpdateDaemonConfig(const NPVariant* args,
                          uint32_t arg_count,
                          NPVariant* result);

  
  
  
  
  bool GetDaemonConfig(const NPVariant* args,
                       uint32_t arg_count,
                       NPVariant* result);

  
  
  
  bool GetDaemonVersion(const NPVariant* args,
                        uint32_t arg_count,
                        NPVariant* result);

  
  bool GetPairedClients(const NPVariant* args,
                        uint32_t arg_count,
                        NPVariant* result);

  
  
  
  bool GetUsageStatsConsent(const NPVariant* args,
                            uint32_t arg_count,
                            NPVariant* result);

  
  
  
  bool StartDaemon(const NPVariant* args,
                   uint32_t arg_count,
                   NPVariant* result);

  
  
  bool StopDaemon(const NPVariant* args, uint32_t arg_count, NPVariant* result);

  
  

  
  virtual void OnStateChanged(It2MeHostState state) OVERRIDE;

  
  
  virtual void OnNatPolicyChanged(bool nat_traversal_enabled) OVERRIDE;

  
  virtual void OnStoreAccessCode(const std::string& access_code,
                                 base::TimeDelta access_code_lifetime) OVERRIDE;

  
  virtual void OnClientAuthenticated(
      const std::string& client_username) OVERRIDE;

  
  void LocalizeStrings(NPObject* localize_func);

  
  
  
  
  bool LocalizeString(NPObject* localize_func, const char* tag,
                      base::string16* result);

  
  
  
  
  bool LocalizeStringWithSubstitution(NPObject* localize_func,
                                      const char* tag,
                                      const char* substitution,
                                      base::string16* result);

  
  

  
  static void DoGenerateKeyPair(
      const scoped_refptr<AutoThreadTaskRunner>& plugin_task_runner,
      const base::Callback<void (const std::string&,
                                 const std::string&)>& callback);
  void InvokeGenerateKeyPairCallback(scoped_ptr<ScopedRefNPObject> callback,
                                     const std::string& private_key,
                                     const std::string& public_key);

  
  
  void InvokeAsyncResultCallback(scoped_ptr<ScopedRefNPObject> callback,
                                 DaemonController::AsyncResult result);

  
  
  void InvokeBooleanCallback(scoped_ptr<ScopedRefNPObject> callback,
                             bool result);

  
  void InvokeGetDaemonConfigCallback(scoped_ptr<ScopedRefNPObject> callback,
                                     scoped_ptr<base::DictionaryValue> config);

  
  void InvokeGetDaemonVersionCallback(scoped_ptr<ScopedRefNPObject> callback,
                                      const std::string& version);

  
  void InvokeGetPairedClientsCallback(
      scoped_ptr<ScopedRefNPObject> callback,
      scoped_ptr<base::ListValue> paired_clients);

  
  void InvokeGetUsageStatsConsentCallback(
      scoped_ptr<ScopedRefNPObject> callback,
      const DaemonController::UsageStatsConsent& consent);

  
  

  
  
  void LogDebugInfo(const std::string& message);

  
  
  bool InvokeAndIgnoreResult(const ScopedRefNPObject& func,
                             const NPVariant* args,
                             uint32_t arg_count);

  
  void SetException(const std::string& exception_string);

  
  

  NPP plugin_;
  NPObject* parent_;
  scoped_refptr<AutoThreadTaskRunner> plugin_task_runner_;
  scoped_ptr<base::ThreadTaskRunnerHandle> plugin_task_runner_handle_;

  
  bool am_currently_logging_;

  ScopedRefNPObject log_debug_info_func_;

  
  

  
  scoped_ptr<ChromotingHostContext> host_context_;
  scoped_refptr<It2MeHost> it2me_host_;

  
  It2MeHostState state_;
  std::string access_code_;
  base::TimeDelta access_code_lifetime_;
  std::string client_username_;

  
  XmppSignalStrategy::XmppServerConfig xmpp_server_config_;

  
  std::string directory_bot_jid_;

  
  ScopedRefNPObject on_nat_traversal_policy_changed_func_;
  ScopedRefNPObject on_state_changed_func_;

  
  

  
  scoped_refptr<DaemonController> daemon_controller_;

  
  
  
  scoped_refptr<AutoThreadTaskRunner> worker_thread_;

  
  scoped_refptr<protocol::PairingRegistry> pairing_registry_;

  
  

  
  base::WeakPtr<HostNPScriptObject> weak_ptr_;
  base::WeakPtrFactory<HostNPScriptObject> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(HostNPScriptObject);
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_NETWORK_CONNECTION_HANDLER_H_
#define CHROMEOS_NETWORK_NETWORK_CONNECTION_HANDLER_H_

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "base/values.h"
#include "chromeos/cert_loader.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_method_call_status.h"
#include "chromeos/login/login_state.h"
#include "chromeos/network/network_handler.h"
#include "chromeos/network/network_handler_callbacks.h"
#include "chromeos/network/network_state_handler_observer.h"

namespace chromeos {

class NetworkState;
class NetworkUIData;


class CHROMEOS_EXPORT NetworkConnectionHandler
    : public LoginState::Observer,
      public CertLoader::Observer,
      public NetworkStateHandlerObserver,
      public base::SupportsWeakPtr<NetworkConnectionHandler> {
 public:
  

  
  
  static const char kErrorNotFound[];

  
  static const char kErrorConnected[];

  
  static const char kErrorConnecting[];

  
  static const char kErrorPassphraseRequired[];

  static const char kErrorActivationRequired[];

  
  static const char kErrorCertificateRequired[];

  
  
  static const char kErrorAuthenticationRequired[];

  
  static const char kErrorConfigurationRequired[];

  
  static const char kErrorConfigureFailed[];

  
  static const char kErrorShillError[];

  
  static const char kErrorConnectCanceled[];

  
  static const char kErrorNotConnected[];

  virtual ~NetworkConnectionHandler();

  
  
  
  
  
  
  
  
  
  
  void ConnectToNetwork(const std::string& service_path,
                        const base::Closure& success_callback,
                        const network_handler::ErrorCallback& error_callback,
                        bool check_error_state);

  
  
  
  
  
  
  
  void DisconnectNetwork(const std::string& service_path,
                         const base::Closure& success_callback,
                         const network_handler::ErrorCallback& error_callback);

  
  
  bool HasConnectingNetwork(const std::string& service_path);

  
  bool HasPendingConnectRequest();

  
  virtual void NetworkListChanged() OVERRIDE;
  virtual void NetworkPropertiesUpdated(const NetworkState* network) OVERRIDE;

  
  virtual void LoggedInStateChanged() OVERRIDE;

  
  virtual void OnCertificatesLoaded(const net::CertificateList& cert_list,
                                    bool initial_load) OVERRIDE;

 private:
  friend class NetworkHandler;
  friend class NetworkConnectionHandlerTest;

  struct ConnectRequest;

  NetworkConnectionHandler();

  void Init(NetworkStateHandler* network_state_handler,
            NetworkConfigurationHandler* network_configuration_handler);

  ConnectRequest* GetPendingRequest(const std::string& service_path);

  
  
  
  
  
  void VerifyConfiguredAndConnect(bool check_error_state,
                                  const std::string& service_path,
                                  const base::DictionaryValue& properties);

  
  void CallShillConnect(const std::string& service_path);

  
  void HandleConfigurationFailure(
      const std::string& service_path,
      const std::string& error_name,
      scoped_ptr<base::DictionaryValue> error_data);

  
  void HandleShillConnectSuccess(const std::string& service_path);
  void HandleShillConnectFailure(const std::string& service_path,
                                 const std::string& error_name,
                                 const std::string& error_message);

  void CheckPendingRequest(const std::string service_path);
  void CheckAllPendingRequests();

  
  
  std::string CertificateIsConfigured(NetworkUIData* ui_data);
  void ErrorCallbackForPendingRequest(const std::string& service_path,
                                      const std::string& error_name);

  
  void CallShillDisconnect(
      const std::string& service_path,
      const base::Closure& success_callback,
      const network_handler::ErrorCallback& error_callback);

  
  void HandleShillDisconnectSuccess(const std::string& service_path,
                                    const base::Closure& success_callback);


  
  CertLoader* cert_loader_;
  NetworkStateHandler* network_state_handler_;
  NetworkConfigurationHandler* network_configuration_handler_;

  
  
  std::map<std::string, ConnectRequest> pending_requests_;
  scoped_ptr<ConnectRequest> queued_connect_;

  
  bool logged_in_;
  bool certificates_loaded_;

  DISALLOW_COPY_AND_ASSIGN(NetworkConnectionHandler);
};

}  

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_CLIENT_CERT_RESOLVER_H_
#define CHROMEOS_NETWORK_CLIENT_CERT_RESOLVER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chromeos/cert_loader.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/network/network_policy_observer.h"
#include "chromeos/network/network_state_handler_observer.h"

namespace base {
class TaskRunner;
}

namespace chromeos {

class FavoriteState;
class NetworkStateHandler;
class ManagedNetworkConfigurationHandler;

class CHROMEOS_EXPORT ClientCertResolver : public NetworkStateHandlerObserver,
                                           public CertLoader::Observer,
                                           public NetworkPolicyObserver {
 public:
  struct NetworkAndMatchingCert;

  ClientCertResolver();
  virtual ~ClientCertResolver();

  void Init(NetworkStateHandler* network_state_handler,
            ManagedNetworkConfigurationHandler* managed_network_config_handler);

  
  
  void SetSlowTaskRunnerForTest(
      const scoped_refptr<base::TaskRunner>& task_runner);

 private:
  typedef std::vector<const FavoriteState*> FavoriteStateList;

   
  virtual void NetworkListChanged() OVERRIDE;

  
  virtual void OnCertificatesLoaded(const net::CertificateList& cert_list,
                                    bool initial_load) OVERRIDE;

  
  virtual void PolicyApplied(const std::string& service_path) OVERRIDE;

  
  
  
  void ResolveNetworks(const FavoriteStateList& networks);

  
  
  void ConfigureCertificates(std::vector<NetworkAndMatchingCert>* matches);

  
  
  std::set<std::string> resolved_networks_;

  
  NetworkStateHandler* network_state_handler_;

  
  ManagedNetworkConfigurationHandler* managed_network_config_handler_;

  
  scoped_refptr<base::TaskRunner> slow_task_runner_for_test_;

  base::WeakPtrFactory<ClientCertResolver> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ClientCertResolver);
};

}  

#endif  

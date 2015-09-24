// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_LOCAL_DISCOVERY_SERVICE_DISCOVERY_CLIENT_H_
#define CHROME_COMMON_LOCAL_DISCOVERY_SERVICE_DISCOVERY_CLIENT_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "net/base/address_family.h"
#include "net/base/host_port_pair.h"
#include "net/base/net_util.h"

namespace net {
class MDnsClient;
}

namespace local_discovery {

struct ServiceDescription {
 public:
  ServiceDescription();
  ~ServiceDescription();

  
  
  std::string instance_name() const;
  std::string service_type() const;

  
  std::string service_name;
  
  net::HostPortPair address;
  
  std::vector<std::string> metadata;
  
  net::IPAddressNumber ip_address;
  
  base::Time last_seen;
};

class ServiceWatcher {
 public:
  enum UpdateType {
    UPDATE_ADDED,
    UPDATE_CHANGED,
    UPDATE_REMOVED,
    UPDATE_INVALIDATED
  };

  
  typedef base::Callback<void(UpdateType, const std::string&)> UpdatedCallback;

  
  virtual ~ServiceWatcher() {}

  
  virtual void Start() = 0;

  
  virtual void DiscoverNewServices(bool force_update) = 0;

  virtual std::string GetServiceType() const = 0;
};

class ServiceResolver {
 public:
  enum RequestStatus {
    STATUS_SUCCESS,
    STATUS_REQUEST_TIMEOUT,
    STATUS_KNOWN_NONEXISTENT
  };

  
  typedef base::Callback<void(RequestStatus, const ServiceDescription&)>
      ResolveCompleteCallback;

  
  virtual ~ServiceResolver() {}

  
  virtual void StartResolving() = 0;

  virtual std::string GetName() const = 0;
};

class LocalDomainResolver {
 public:
  typedef base::Callback<void(bool ,
                              const net::IPAddressNumber& ,
                              const net::IPAddressNumber& )>
      IPAddressCallback;

  virtual ~LocalDomainResolver() {}

  virtual void Start() = 0;
};

class ServiceDiscoveryClient {
 public:
  virtual ~ServiceDiscoveryClient() {}

  
  
  virtual scoped_ptr<ServiceWatcher> CreateServiceWatcher(
      const std::string& service_type,
      const ServiceWatcher::UpdatedCallback& callback) = 0;

  
  
  virtual scoped_ptr<ServiceResolver> CreateServiceResolver(
      const std::string& service_name,
      const ServiceResolver::ResolveCompleteCallback& callback) = 0;

  
  virtual scoped_ptr<LocalDomainResolver> CreateLocalDomainResolver(
      const std::string& domain,
      net::AddressFamily address_family,
      const LocalDomainResolver::IPAddressCallback& callback) = 0;
};

}  

#endif  

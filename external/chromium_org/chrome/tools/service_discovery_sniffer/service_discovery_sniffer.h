// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TOOLS_SERVICE_DISCOVERY_SNIFFER_SERVICE_DISCOVERY_SNIFFER_H_
#define CHROME_TOOLS_SERVICE_DISCOVERY_SNIFFER_SERVICE_DISCOVERY_SNIFFER_H_

#include <map>
#include <string>

#include "base/memory/linked_ptr.h"
#include "chrome/common/local_discovery/service_discovery_client.h"

namespace local_discovery {

class ServicePrinter {
 public:
  ServicePrinter(ServiceDiscoveryClient* client,
                 const std::string& service_name);
  ~ServicePrinter();

  void Added();
  void Changed();
  void Removed();

 private:
  void OnServiceResolved(ServiceResolver::RequestStatus status,
                         const ServiceDescription& service);

  bool changed_;
  scoped_ptr<ServiceResolver> service_resolver_;

  DISALLOW_COPY_AND_ASSIGN(ServicePrinter);
};

class ServiceTypePrinter {
 public:
  ServiceTypePrinter(ServiceDiscoveryClient* client,
                     const std::string& service_type);
  virtual ~ServiceTypePrinter();

  void Start();
  void OnServiceUpdated(ServiceWatcher::UpdateType,
                        const std::string& service_name);

 private:
  typedef std::map<std::string, linked_ptr<ServicePrinter> > ServiceMap;

  ServiceMap services_;
  scoped_ptr<ServiceWatcher> watcher_;
  ServiceDiscoveryClient* client_;

  DISALLOW_COPY_AND_ASSIGN(ServiceTypePrinter);
};

}  

#endif  

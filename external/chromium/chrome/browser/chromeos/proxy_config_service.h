// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_PROXY_CONFIG_SERVICE_H_
#define CHROME_BROWSER_CHROMEOS_PROXY_CONFIG_SERVICE_H_
#pragma once

#include "chrome/browser/chromeos/proxy_config_service_impl.h"

namespace chromeos {

class ProxyConfigService : public net::ProxyConfigService {
 public:
  explicit ProxyConfigService(const scoped_refptr<ProxyConfigServiceImpl>& impl)
      : impl_(impl) {}
  virtual ~ProxyConfigService() {}

  
  virtual void AddObserver(Observer* observer) {
    impl_->AddObserver(observer);
  }
  virtual void RemoveObserver(Observer* observer) {
    impl_->RemoveObserver(observer);
  }
  virtual ConfigAvailability GetLatestProxyConfig(net::ProxyConfig* config) {
    return impl_->IOGetProxyConfig(config);
  }

 private:
  scoped_refptr<ProxyConfigServiceImpl> impl_;

  DISALLOW_COPY_AND_ASSIGN(ProxyConfigService);
};

}  

#endif  

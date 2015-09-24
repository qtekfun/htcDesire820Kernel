// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SSL_SSL_CONFIG_SERVICE_DEFAULTS_H_
#define NET_SSL_SSL_CONFIG_SERVICE_DEFAULTS_H_

#include "net/base/net_export.h"
#include "net/ssl/ssl_config_service.h"

namespace net {

class NET_EXPORT SSLConfigServiceDefaults : public SSLConfigService {
 public:
  SSLConfigServiceDefaults();

  
  virtual void GetSSLConfig(SSLConfig* config) OVERRIDE;

 private:
  virtual ~SSLConfigServiceDefaults();

  
  const SSLConfig default_config_;

  DISALLOW_COPY_AND_ASSIGN(SSLConfigServiceDefaults);
};

}  

#endif  

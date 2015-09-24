// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_SSL_CONFIG_SERVICE_DEFAULTS_H_
#define NET_BASE_SSL_CONFIG_SERVICE_DEFAULTS_H_
#pragma once

#include "net/base/ssl_config_service.h"

namespace net {

class SSLConfigServiceDefaults : public SSLConfigService {
 public:
  SSLConfigServiceDefaults();

  
  virtual void GetSSLConfig(SSLConfig* config);

 private:
  virtual ~SSLConfigServiceDefaults();

  
  const SSLConfig default_config_;

  DISALLOW_COPY_AND_ASSIGN(SSLConfigServiceDefaults);
};

}  

#endif  

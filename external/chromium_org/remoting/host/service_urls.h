// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_SERVICE_URLS_H_
#define REMOTING_HOST_SERVICE_URLS_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/singleton.h"

namespace remoting {

class ServiceUrls {
 public:
  static ServiceUrls* GetInstance();

  
  const std::string& directory_base_url() const;
  const std::string& directory_hosts_url() const;

  
  const std::string& xmpp_server_address() const;
  bool xmpp_server_use_tls() const;

  
  const std::string& directory_bot_jid() const;

 private:
  friend struct DefaultSingletonTraits<ServiceUrls>;

  ServiceUrls();
  virtual ~ServiceUrls();

  std::string directory_base_url_;
  std::string directory_hosts_url_;
  std::string xmpp_server_address_;
  bool xmpp_server_use_tls_;
  std::string directory_bot_jid_;

  DISALLOW_COPY_AND_ASSIGN(ServiceUrls);
};

}  

#endif  

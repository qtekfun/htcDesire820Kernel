// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_SETUP_SERVICE_CLIENT_H_
#define REMOTING_HOST_SETUP_SERVICE_CLIENT_H_

#include <string>

#include "base/memory/ref_counted.h"

namespace net {
class URLRequestContextGetter;
}

namespace remoting {

class ServiceClient {
 public:
  
  class Delegate {
   public:
    
    virtual void OnHostRegistered(const std::string& authorization_code) = 0;
    
    virtual void OnHostUnregistered() = 0;
    
    virtual void OnOAuthError() = 0;
    
    
    virtual void OnNetworkError(int response_code) = 0;

   protected:
    virtual ~Delegate() {}
  };
  ServiceClient(const std::string& chromoting_hosts_url,
                net::URLRequestContextGetter* context_getter);
  ~ServiceClient();

  
  void RegisterHost(const std::string& host_id,
                    const std::string& host_name,
                    const std::string& public_key,
                    const std::string& host_client_id,
                    const std::string& oauth_access_token,
                    Delegate* delegate);
  
  void UnregisterHost(const std::string& host_id,
                      const std::string& oauth_access_token,
                      Delegate* delegate);

 private:
  
  class Core;
  scoped_refptr<Core> core_;
  DISALLOW_COPY_AND_ASSIGN(ServiceClient);
};

}  

#endif  

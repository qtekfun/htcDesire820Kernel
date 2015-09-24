// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_SINGLE_REQUEST_HOST_RESOLVER_H_
#define NET_DNS_SINGLE_REQUEST_HOST_RESOLVER_H_

#include "base/basictypes.h"

#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "net/base/request_priority.h"
#include "net/dns/host_resolver.h"

namespace net {

class AddressList;
class BoundNetLog;

class NET_EXPORT SingleRequestHostResolver {
 public:
  
  explicit SingleRequestHostResolver(HostResolver* resolver);

  
  
  
  ~SingleRequestHostResolver();

  
  
  int Resolve(const HostResolver::RequestInfo& info,
              RequestPriority priority,
              AddressList* addresses,
              const CompletionCallback& callback,
              const BoundNetLog& net_log);

  
  
  void Cancel();

 private:
  
  
  void OnResolveCompletion(int result);

  
  HostResolver* const resolver_;

  
  HostResolver::RequestHandle cur_request_;
  CompletionCallback cur_request_callback_;

  
  CompletionCallback callback_;

  DISALLOW_COPY_AND_ASSIGN(SingleRequestHostResolver);
};

}  

#endif  

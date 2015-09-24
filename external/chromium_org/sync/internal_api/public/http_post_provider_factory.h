// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_HTTP_POST_PROVIDER_FACTORY_H_
#define SYNC_INTERNAL_API_PUBLIC_HTTP_POST_PROVIDER_FACTORY_H_

#include <string>

#include "sync/base/sync_export.h"

namespace syncer {

class HttpPostProviderInterface;

class SYNC_EXPORT HttpPostProviderFactory {
 public:
  virtual ~HttpPostProviderFactory() {}

  virtual void Init(const std::string& user_agent) = 0;

  
  virtual HttpPostProviderInterface* Create() = 0;

  
  
  
  
  
  virtual void Destroy(HttpPostProviderInterface* http) = 0;
};

}  

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ENGINE_HTTP_POST_PROVIDER_FACTORY_H_
#define CHROME_BROWSER_SYNC_ENGINE_HTTP_POST_PROVIDER_FACTORY_H_
#pragma once

namespace sync_api {

class HttpPostProviderInterface;

class HttpPostProviderFactory {
 public:
  virtual ~HttpPostProviderFactory() {}

  
  virtual HttpPostProviderInterface* Create() = 0;

  
  
  
  
  
  virtual void Destroy(HttpPostProviderInterface* http) = 0;
};

}  

#endif  

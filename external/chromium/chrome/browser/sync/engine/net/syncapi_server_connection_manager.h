// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ENGINE_NET_SYNCAPI_SERVER_CONNECTION_MANAGER_H_
#define CHROME_BROWSER_SYNC_ENGINE_NET_SYNCAPI_SERVER_CONNECTION_MANAGER_H_
#pragma once

#include <string>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/sync/engine/net/server_connection_manager.h"

namespace sync_api {

class HttpPostProviderFactory;

class SyncAPIBridgedPost
    : public browser_sync::ServerConnectionManager::Post {
 public:
  SyncAPIBridgedPost(browser_sync::ServerConnectionManager* scm,
                     HttpPostProviderFactory* factory);

  virtual ~SyncAPIBridgedPost();

  virtual bool Init(const char* path,
                    const std::string& auth_token,
                    const std::string& payload,
                    browser_sync::HttpResponse* response);

 private:
  
  
  HttpPostProviderFactory* factory_;

  DISALLOW_COPY_AND_ASSIGN(SyncAPIBridgedPost);
};

class SyncAPIServerConnectionManager
    : public browser_sync::ServerConnectionManager {
 public:
  
  SyncAPIServerConnectionManager(const std::string& server,
                                 int port,
                                 bool use_ssl,
                                 const std::string& client_version,
                                 HttpPostProviderFactory* factory);
  virtual ~SyncAPIServerConnectionManager();

 protected:
  virtual Post* MakePost();

 private:
  
  
  scoped_ptr<HttpPostProviderFactory> post_provider_factory_;

  DISALLOW_COPY_AND_ASSIGN(SyncAPIServerConnectionManager);
};

}  

#endif  

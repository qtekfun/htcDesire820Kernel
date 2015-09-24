// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_SYNCAPI_SERVER_CONNECTION_MANAGER_H_
#define SYNC_INTERNAL_API_SYNCAPI_SERVER_CONNECTION_MANAGER_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "sync/base/sync_export.h"
#include "sync/engine/net/server_connection_manager.h"

namespace syncer {

class HttpPostProviderFactory;
class HttpPostProviderInterface;

class SyncAPIBridgedConnection : public ServerConnectionManager::Connection {
 public:
  SyncAPIBridgedConnection(ServerConnectionManager* scm,
                           HttpPostProviderFactory* factory);

  virtual ~SyncAPIBridgedConnection();

  virtual bool Init(const char* path,
                    const std::string& auth_token,
                    const std::string& payload,
                    HttpResponse* response) OVERRIDE;

  virtual void Abort() OVERRIDE;

 private:
  
  
  HttpPostProviderFactory* factory_;

  HttpPostProviderInterface* post_provider_;

  DISALLOW_COPY_AND_ASSIGN(SyncAPIBridgedConnection);
};

class SYNC_EXPORT_PRIVATE SyncAPIServerConnectionManager
    : public ServerConnectionManager {
 public:
  
  SyncAPIServerConnectionManager(const std::string& server,
                                 int port,
                                 bool use_ssl,
                                 HttpPostProviderFactory* factory,
                                 CancelationSignal* cancelation_signal);
  virtual ~SyncAPIServerConnectionManager();

  
  virtual Connection* MakeConnection() OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(SyncAPIServerConnectionManagerTest,
                           VeryEarlyAbortPost);
  FRIEND_TEST_ALL_PREFIXES(SyncAPIServerConnectionManagerTest, EarlyAbortPost);
  FRIEND_TEST_ALL_PREFIXES(SyncAPIServerConnectionManagerTest, AbortPost);

  
  
  scoped_ptr<HttpPostProviderFactory> post_provider_factory_;

  DISALLOW_COPY_AND_ASSIGN(SyncAPIServerConnectionManager);
};

}  

#endif  

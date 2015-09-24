// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TEST_SPAWNED_TEST_SERVER_REMOTE_TEST_SERVER_H_
#define NET_TEST_SPAWNED_TEST_SERVER_REMOTE_TEST_SERVER_H_

#include <string>

#include "net/test/spawned_test_server/base_test_server.h"

namespace net {

class SpawnerCommunicator;

class RemoteTestServer : public BaseTestServer {
 public:
  
  
  RemoteTestServer(Type type,
                   const std::string& host,
                   const base::FilePath& document_root);

  
  
  RemoteTestServer(Type type,
                   const SSLOptions& ssl_options,
                   const base::FilePath& document_root);

  virtual ~RemoteTestServer();

  
  
  bool Start() WARN_UNUSED_RESULT;

  
  
  bool StartInBackground() WARN_UNUSED_RESULT;
  bool BlockUntilStarted() WARN_UNUSED_RESULT;

  
  bool Stop();

  
  
  
  
  base::FilePath GetDocumentRoot() const;

 private:
  bool Init(const base::FilePath& document_root);

  
  
  
  
  int spawner_server_port_;

  
  
  scoped_ptr<SpawnerCommunicator> spawner_communicator_;

  DISALLOW_COPY_AND_ASSIGN(RemoteTestServer);
};

}  

#endif  


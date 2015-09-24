// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_TEST_LOCAL_SYNC_TEST_SERVER_H_
#define SYNC_TEST_LOCAL_SYNC_TEST_SERVER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "net/test/spawned_test_server/local_test_server.h"

namespace syncer {

class LocalSyncTestServer : public net::LocalTestServer {
 public:
  
  
  LocalSyncTestServer();

  
  
  LocalSyncTestServer(uint16 port, uint16 xmpp_port);

  virtual ~LocalSyncTestServer();

  
  virtual bool AddCommandLineArguments(
      CommandLine* command_line) const OVERRIDE;
  virtual bool SetPythonPath() const OVERRIDE;
  virtual bool GetTestServerPath(
      base::FilePath* testserver_path) const OVERRIDE;

  
  
  bool GetTestScriptPath(const base::FilePath::StringType& test_script_name,
                         base::FilePath* test_script_path) const;

 private:
  
  uint16 xmpp_port_;

  DISALLOW_COPY_AND_ASSIGN(LocalSyncTestServer);
};

}  

#endif  

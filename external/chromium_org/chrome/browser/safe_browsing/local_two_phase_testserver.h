// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_LOCAL_TWO_PHASE_TESTSERVER_H_
#define CHROME_BROWSER_SAFE_BROWSING_LOCAL_TWO_PHASE_TESTSERVER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "net/test/spawned_test_server/local_test_server.h"

class LocalTwoPhaseTestServer : public net::LocalTestServer {
 public:
  
  LocalTwoPhaseTestServer();

  virtual ~LocalTwoPhaseTestServer();

  
  virtual bool GetTestServerPath(
      base::FilePath* testserver_path) const OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(LocalTwoPhaseTestServer);
};

#endif  


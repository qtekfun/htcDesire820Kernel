// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PAPPI_TESTS_TEST_TCP_SOCKET_PRIVATE_TRUSTED_H_
#define PAPPI_TESTS_TEST_TCP_SOCKET_PRIVATE_TRUSTED_H_

#include <string>

#include "ppapi/c/pp_stdint.h"
#include "ppapi/tests/test_case.h"

class TestTCPSocketPrivateTrusted : public TestCase {
 public:
  explicit TestTCPSocketPrivateTrusted(TestingInstance* instance);

  
  virtual bool Init();
  virtual void RunTests(const std::string& filter);

 private:
  std::string TestGetServerCertificate();

  std::string host_;
  uint16_t port_;
  uint16_t ssl_port_;
};

#endif  

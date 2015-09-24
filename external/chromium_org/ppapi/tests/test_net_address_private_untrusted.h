// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PAPPI_TESTS_TEST_NET_ADDRESS_PRIVATE_UNTRUSTED_H_
#define PAPPI_TESTS_TEST_NET_ADDRESS_PRIVATE_UNTRUSTED_H_

#include <string>

#include "ppapi/cpp/private/tcp_socket_private.h"
#include "ppapi/tests/test_case.h"

// NaCl. TestNetAddressPrivateUntrusted is written only for check that
class TestNetAddressPrivateUntrusted : public TestCase {
 public:
  explicit TestNetAddressPrivateUntrusted(TestingInstance* instance);

  
  virtual bool Init();
  virtual void RunTests(const std::string& filter);

 private:
  int32_t Connect(pp::TCPSocketPrivate* socket,
                  const std::string& host,
                  uint16_t port);

  std::string TestAreEqual();
  std::string TestAreHostsEqual();
  std::string TestDescribe();
  std::string TestReplacePort();
  std::string TestGetAnyAddress();
  std::string TestGetFamily();
  std::string TestGetPort();
  std::string TestGetAddress();

  std::string host_;
  uint16_t port_;
};

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SSL_CLIENT_CERT_STORE_UNITTEST_INL_H_
#define NET_SSL_CLIENT_CERT_STORE_UNITTEST_INL_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/test_data_directory.h"
#include "net/test/cert_test_util.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace net {

namespace {

const unsigned char kAuthority1DN[] = {
  0x30, 0x0f, 0x31, 0x0d, 0x30, 0x0b, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c,
  0x04, 0x42, 0x20, 0x43, 0x41
};

unsigned char kAuthority2DN[] = {
  0x30, 0x0f, 0x31, 0x0d, 0x30, 0x0b, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c,
  0x04, 0x45, 0x20, 0x43, 0x41
};

}  

template <typename T>
class ClientCertStoreTest : public ::testing::Test {
 public:
  T delegate_;
};

TYPED_TEST_CASE_P(ClientCertStoreTest);

TYPED_TEST_P(ClientCertStoreTest, EmptyQuery) {
  std::vector<scoped_refptr<X509Certificate> > certs;
  scoped_refptr<SSLCertRequestInfo> request(new SSLCertRequestInfo());

  std::vector<scoped_refptr<X509Certificate> > selected_certs;
  bool rv = this->delegate_.SelectClientCerts(
      certs, *request.get(), &selected_certs);
  EXPECT_TRUE(rv);
  EXPECT_EQ(0u, selected_certs.size());
}

TYPED_TEST_P(ClientCertStoreTest, AllIssuersAllowed) {
  scoped_refptr<X509Certificate> cert(
      ImportCertFromFile(GetTestCertsDirectory(), "client_1.pem"));
  ASSERT_TRUE(cert.get());

  std::vector<scoped_refptr<X509Certificate> > certs;
  certs.push_back(cert);
  scoped_refptr<SSLCertRequestInfo> request(new SSLCertRequestInfo());

  std::vector<scoped_refptr<X509Certificate> > selected_certs;
  bool rv = this->delegate_.SelectClientCerts(
      certs, *request.get(), &selected_certs);
  EXPECT_TRUE(rv);
  ASSERT_EQ(1u, selected_certs.size());
  EXPECT_TRUE(selected_certs[0]->Equals(cert.get()));
}

TYPED_TEST_P(ClientCertStoreTest, CertAuthorityFiltering) {
  scoped_refptr<X509Certificate> cert_1(
      ImportCertFromFile(GetTestCertsDirectory(), "client_1.pem"));
  ASSERT_TRUE(cert_1.get());
  scoped_refptr<X509Certificate> cert_2(
      ImportCertFromFile(GetTestCertsDirectory(), "client_2.pem"));
  ASSERT_TRUE(cert_2.get());

  std::vector<std::string> authority_1(
      1, std::string(reinterpret_cast<const char*>(kAuthority1DN),
                     sizeof(kAuthority1DN)));
  std::vector<std::string> authority_2(
      1, std::string(reinterpret_cast<const char*>(kAuthority2DN),
                     sizeof(kAuthority2DN)));
  EXPECT_TRUE(cert_1->IsIssuedByEncoded(authority_1));
  EXPECT_FALSE(cert_1->IsIssuedByEncoded(authority_2));
  EXPECT_TRUE(cert_2->IsIssuedByEncoded(authority_2));
  EXPECT_FALSE(cert_2->IsIssuedByEncoded(authority_1));

  std::vector<scoped_refptr<X509Certificate> > certs;
  certs.push_back(cert_1);
  certs.push_back(cert_2);
  scoped_refptr<SSLCertRequestInfo> request(new SSLCertRequestInfo());
  request->cert_authorities = authority_1;

  std::vector<scoped_refptr<X509Certificate> > selected_certs;
  bool rv = this->delegate_.SelectClientCerts(
      certs, *request.get(), &selected_certs);
  EXPECT_TRUE(rv);
  ASSERT_EQ(1u, selected_certs.size());
  EXPECT_TRUE(selected_certs[0]->Equals(cert_1.get()));
}

REGISTER_TYPED_TEST_CASE_P(ClientCertStoreTest,
                           EmptyQuery,
                           AllIssuersAllowed,
                           CertAuthorityFiltering);

}  

#endif  

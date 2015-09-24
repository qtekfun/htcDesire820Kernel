// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_CT_TEST_UTIL_H_
#define NET_CERT_CT_TEST_UTIL_H_

#include <string>

#include "base/memory/ref_counted.h"

namespace net {

namespace ct {

struct LogEntry;
struct SignedCertificateTimestamp;


void GetX509CertLogEntry(LogEntry* entry);

std::string GetDerEncodedX509Cert();

void GetPrecertLogEntry(LogEntry* entry);

std::string GetTestDigitallySigned();

std::string GetTestSignedCertificateTimestamp();

std::string GetTestPublicKey();

std::string GetTestPublicKeyId();

void GetX509CertSCT(scoped_refptr<SignedCertificateTimestamp>* sct);

void GetPrecertSCT(scoped_refptr<SignedCertificateTimestamp>* sct);

std::string GetDefaultIssuerKeyHash();

std::string GetDerEncodedFakeOCSPResponse();

std::string GetFakeOCSPExtensionValue();

std::string GetDerEncodedFakeOCSPResponseCert();

std::string GetDerEncodedFakeOCSPResponseIssuerCert();

}  

}  

#endif  

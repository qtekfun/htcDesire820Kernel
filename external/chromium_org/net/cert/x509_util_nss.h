// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_X509_UTIL_NSS_H_
#define NET_CERT_X509_UTIL_NSS_H_

#include <string>
#include <vector>

#include "base/time/time.h"
#include "net/base/net_export.h"
#include "net/cert/x509_certificate.h"

class PickleIterator;

typedef struct CERTCertificateStr CERTCertificate;
typedef struct CERTNameStr CERTName;
typedef struct PK11SlotInfoStr PK11SlotInfo;
typedef struct PLArenaPool PLArenaPool;
typedef struct SECItemStr SECItem;

namespace net {

namespace x509_util {

#if defined(USE_NSS) || defined(OS_IOS)
void ParsePrincipal(CERTName* name,
                    CertPrincipal* principal);

void ParseDate(const SECItem* der_date, base::Time* result);

std::string ParseSerialNumber(const CERTCertificate* certificate);

void GetSubjectAltName(CERTCertificate* cert_handle,
                       std::vector<std::string>* dns_names,
                       std::vector<std::string>* ip_addrs);

X509Certificate::OSCertHandles CreateOSCertHandlesFromBytes(
    const char* data,
    int length,
    X509Certificate::Format format);

X509Certificate::OSCertHandle ReadOSCertHandleFromPickle(
    PickleIterator* pickle_iter);

void GetPublicKeyInfo(CERTCertificate* handle,
                      size_t* size_bits,
                      X509Certificate::PublicKeyType* type);

bool GetIssuersFromEncodedList(
    const std::vector<std::string>& issuers,
    PLArenaPool* arena,
    std::vector<CERTName*>* out);

bool IsCertificateIssuedBy(const std::vector<CERTCertificate*>& cert_chain,
                           const std::vector<CERTName*>& valid_issuers);

std::string GetUniqueNicknameForSlot(const std::string& nickname,
                                     const SECItem* subject,
                                     PK11SlotInfo* slot);
#endif  

} 

} 

#endif  

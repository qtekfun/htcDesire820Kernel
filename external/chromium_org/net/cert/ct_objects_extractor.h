// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_CT_OBJECTS_EXTRACTOR_H_
#define NET_CERT_CT_OBJECTS_EXTRACTOR_H_

#include <string>

#include "net/base/net_export.h"
#include "net/cert/x509_certificate.h"

namespace net {

namespace ct {

struct LogEntry;

NET_EXPORT_PRIVATE bool ExtractEmbeddedSCTList(
    X509Certificate::OSCertHandle cert,
    std::string* sct_list);

NET_EXPORT_PRIVATE bool GetPrecertLogEntry(X509Certificate::OSCertHandle leaf,
                                           X509Certificate::OSCertHandle issuer,
                                           LogEntry* result);

NET_EXPORT_PRIVATE bool GetX509LogEntry(X509Certificate::OSCertHandle leaf,
                                        LogEntry* result);

NET_EXPORT_PRIVATE bool ExtractSCTListFromOCSPResponse(
    X509Certificate::OSCertHandle issuer,
    const std::string& cert_serial_number,
    const std::string& ocsp_response,
    std::string* sct_list);

}  

}  

#endif  

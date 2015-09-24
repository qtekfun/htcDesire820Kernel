// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_CT_SIGNED_CERTIFICATE_TIMESTAMP_LOG_PARAM_H_
#define NET_CERT_CT_SIGNED_CERTIFICATE_TIMESTAMP_LOG_PARAM_H_

#include "net/base/net_log.h"

namespace net {

namespace ct {
struct CTVerifyResult;
}

base::Value* NetLogSignedCertificateTimestampCallback(
    const ct::CTVerifyResult* ct_result, NetLog::LogLevel log_level);

base::Value* NetLogRawSignedCertificateTimestampCallback(
    const std::string* embedded_scts,
    const std::string* sct_list_from_ocsp,
    const std::string* sct_list_from_tls_extension,
    NetLog::LogLevel log_level);

}  

#endif  

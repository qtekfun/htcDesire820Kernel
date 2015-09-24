// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_CERT_STATUS_FLAGS_H_
#define NET_CERT_CERT_STATUS_FLAGS_H_

#include "base/basictypes.h"
#include "net/base/net_export.h"

namespace net {

typedef uint32 CertStatus;

static const CertStatus CERT_STATUS_ALL_ERRORS                 = 0xFFFF;
static const CertStatus CERT_STATUS_COMMON_NAME_INVALID        = 1 << 0;
static const CertStatus CERT_STATUS_DATE_INVALID               = 1 << 1;
static const CertStatus CERT_STATUS_AUTHORITY_INVALID          = 1 << 2;
static const CertStatus CERT_STATUS_NO_REVOCATION_MECHANISM    = 1 << 4;
static const CertStatus CERT_STATUS_UNABLE_TO_CHECK_REVOCATION = 1 << 5;
static const CertStatus CERT_STATUS_REVOKED                    = 1 << 6;
static const CertStatus CERT_STATUS_INVALID                    = 1 << 7;
static const CertStatus CERT_STATUS_WEAK_SIGNATURE_ALGORITHM   = 1 << 8;
static const CertStatus CERT_STATUS_NON_UNIQUE_NAME            = 1 << 10;
static const CertStatus CERT_STATUS_WEAK_KEY                   = 1 << 11;
static const CertStatus CERT_STATUS_WEAK_DH_KEY                = 1 << 12;
static const CertStatus CERT_STATUS_PINNED_KEY_MISSING         = 1 << 13;
static const CertStatus CERT_STATUS_NAME_CONSTRAINT_VIOLATION  = 1 << 14;

static const CertStatus CERT_STATUS_IS_EV                      = 1 << 16;
static const CertStatus CERT_STATUS_REV_CHECKING_ENABLED       = 1 << 17;

static inline bool IsCertStatusError(CertStatus status) {
  return (CERT_STATUS_ALL_ERRORS & status) != 0;
}

NET_EXPORT bool IsCertStatusMinorError(CertStatus cert_status);

NET_EXPORT CertStatus MapNetErrorToCertStatus(int error);

NET_EXPORT int MapCertStatusToNetError(CertStatus cert_status);

}  

#endif  

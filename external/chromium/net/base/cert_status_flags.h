// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_CERT_STATUS_FLAGS_H_
#define NET_BASE_CERT_STATUS_FLAGS_H_
#pragma once

namespace net {

enum {
  
  CERT_STATUS_ALL_ERRORS                 = 0xFFFF,
  CERT_STATUS_COMMON_NAME_INVALID        = 1 << 0,
  CERT_STATUS_DATE_INVALID               = 1 << 1,
  CERT_STATUS_AUTHORITY_INVALID          = 1 << 2,
  
  CERT_STATUS_NO_REVOCATION_MECHANISM    = 1 << 4,
  CERT_STATUS_UNABLE_TO_CHECK_REVOCATION = 1 << 5,
  CERT_STATUS_REVOKED                    = 1 << 6,
  CERT_STATUS_INVALID                    = 1 << 7,
  CERT_STATUS_WEAK_SIGNATURE_ALGORITHM   = 1 << 8,
  CERT_STATUS_NOT_IN_DNS                 = 1 << 9,
  CERT_STATUS_NON_UNIQUE_NAME            = 1 << 10,

  
  CERT_STATUS_IS_EV                      = 1 << 16,
  CERT_STATUS_REV_CHECKING_ENABLED       = 1 << 17,
  CERT_STATUS_IS_DNSSEC                  = 1 << 18,

  
  
};

static inline bool IsCertStatusError(int status) {
  return (CERT_STATUS_ALL_ERRORS & status) != 0;
}

int MapNetErrorToCertStatus(int error);

int MapCertStatusToNetError(int cert_status);

}  

#endif  

// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SSL_SSL_ERROR_INFO_H_
#define CHROME_BROWSER_SSL_SSL_ERROR_INFO_H_
#pragma once

#include <string>
#include <vector>

#include "base/string16.h"
#include "net/base/x509_certificate.h"

class GURL;

class SSLErrorInfo {
 public:
  enum ErrorType {
    CERT_COMMON_NAME_INVALID = 0,
    CERT_DATE_INVALID,
    CERT_AUTHORITY_INVALID,
    CERT_CONTAINS_ERRORS,
    CERT_NO_REVOCATION_MECHANISM,
    CERT_UNABLE_TO_CHECK_REVOCATION,
    CERT_REVOKED,
    CERT_INVALID,
    CERT_WEAK_SIGNATURE_ALGORITHM,
    CERT_NOT_IN_DNS,
    UNKNOWN
  };

  virtual ~SSLErrorInfo();

  
  static ErrorType NetErrorToErrorType(int net_error);

  static SSLErrorInfo CreateError(ErrorType error_type,
                                  net::X509Certificate* cert,
                                  const GURL& request_url);

  
  
  
  static int GetErrorsForCertStatus(int cert_status,
                                    int cert_id,
                                    const GURL& request_url,
                                    std::vector<SSLErrorInfo>* errors);

  
  const string16& title() const { return title_; }

  
  const string16& details() const { return details_; }

  
  const string16& short_description() const { return short_description_; }

  
  
  const std::vector<string16>& extra_information() const {
    return extra_information_;
  }

 private:
  SSLErrorInfo(const string16& title,
               const string16& details,
               const string16& short_description,
               const std::vector<string16>& extra_info);

  string16 title_;
  string16 details_;
  string16 short_description_;
  
  
  std::vector<string16> extra_information_;
};

#endif  

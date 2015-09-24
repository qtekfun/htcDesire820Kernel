// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SSL_SSL_ERROR_INFO_H_
#define CHROME_BROWSER_SSL_SSL_ERROR_INFO_H_

#include <string>
#include <vector>

#include "base/strings/string16.h"
#include "net/cert/cert_status_flags.h"
#include "net/cert/x509_certificate.h"

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
    CERT_WEAK_KEY,
    CERT_NAME_CONSTRAINT_VIOLATION,
    UNKNOWN,
    CERT_WEAK_KEY_DH,
    CERT_PINNED_KEY_MISSING,
    END_OF_ENUM
  };

  virtual ~SSLErrorInfo();

  
  static ErrorType NetErrorToErrorType(int net_error);

  static SSLErrorInfo CreateError(ErrorType error_type,
                                  net::X509Certificate* cert,
                                  const GURL& request_url);

  
  
  
  
  static int GetErrorsForCertStatus(int cert_id,
                                    net::CertStatus cert_status,
                                    const GURL& url,
                                    std::vector<SSLErrorInfo>* errors);

  
  const base::string16& title() const { return title_; }

  
  const base::string16& details() const { return details_; }

  
  const base::string16& short_description() const { return short_description_; }

  
  
  const std::vector<base::string16>& extra_information() const {
    return extra_information_;
  }

 private:
  SSLErrorInfo(const base::string16& title,
               const base::string16& details,
               const base::string16& short_description,
               const std::vector<base::string16>& extra_info);

  base::string16 title_;
  base::string16 details_;
  base::string16 short_description_;
  
  
  std::vector<base::string16> extra_information_;
};

#endif  

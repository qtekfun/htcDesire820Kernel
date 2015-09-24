// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_NETWORK_UI_DATA_H_
#define CHROMEOS_NETWORK_NETWORK_UI_DATA_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/network/certificate_pattern.h"
#include "components/onc/onc_constants.h"

namespace base {
class DictionaryValue;
}

namespace chromeos {

enum ClientCertType {
  CLIENT_CERT_TYPE_NONE    = 0,
  CLIENT_CERT_TYPE_REF     = 1,
  CLIENT_CERT_TYPE_PATTERN = 2
};

class CHROMEOS_EXPORT NetworkUIData {
 public:
  NetworkUIData();
  NetworkUIData(const NetworkUIData& other);
  NetworkUIData& operator=(const NetworkUIData& other);
  explicit NetworkUIData(const base::DictionaryValue& dict);
  ~NetworkUIData();

  void set_onc_source(::onc::ONCSource onc_source) { onc_source_ = onc_source; }
  ::onc::ONCSource onc_source() const { return onc_source_; }

  void set_certificate_pattern(const CertificatePattern& pattern) {
    certificate_pattern_ = pattern;
  }
  const CertificatePattern& certificate_pattern() const {
    return certificate_pattern_;
  }
  void set_certificate_type(ClientCertType type) {
    certificate_type_ = type;
  }
  ClientCertType certificate_type() const {
    return certificate_type_;
  }
  bool is_managed() const {
    return onc_source_ == ::onc::ONC_SOURCE_DEVICE_POLICY ||
        onc_source_ == ::onc::ONC_SOURCE_USER_POLICY;
  }
  const base::DictionaryValue* user_settings() const {
    return user_settings_.get();
  }
  void set_user_settings(scoped_ptr<base::DictionaryValue> dict);
  const std::string& policy_guid() const {
    return policy_guid_;
  }
  void set_policy_guid(const std::string& guid) {
    policy_guid_ = guid;
  }

  
  
  void FillDictionary(base::DictionaryValue* dict) const;

  
  
  
  
  static scoped_ptr<NetworkUIData> CreateFromONC(
      ::onc::ONCSource onc_source,
      const base::DictionaryValue& onc_network);

  
  static const char kKeyONCSource[];

  
  static const char kKeyCertificatePattern[];

  
  static const char kKeyCertificateType[];

  
  static const char kKeyUserSettings[];

 private:
  CertificatePattern certificate_pattern_;
  ::onc::ONCSource onc_source_;
  ClientCertType certificate_type_;
  scoped_ptr<base::DictionaryValue> user_settings_;
  std::string policy_guid_;
};

}  

#endif  

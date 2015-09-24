// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_ONC_ONC_VALIDATOR_H_
#define CHROMEOS_NETWORK_ONC_ONC_VALIDATOR_H_

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/network/onc/onc_mapper.h"
#include "components/onc/onc_constants.h"

namespace base {
class DictionaryValue;
class Value;
}

namespace chromeos {
namespace onc {

struct OncValueSignature;

class CHROMEOS_EXPORT Validator : public Mapper {
 public:
  enum Result {
    VALID,
    VALID_WITH_WARNINGS,
    INVALID
  };

  
  Validator(bool error_on_unknown_field,
            bool error_on_wrong_recommended,
            bool error_on_missing_field,
            bool managed_onc);

  virtual ~Validator();

  
  
  
  
  
  void SetOncSource(::onc::ONCSource source) {
    onc_source_ = source;
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  scoped_ptr<base::DictionaryValue> ValidateAndRepairObject(
      const OncValueSignature* object_signature,
      const base::DictionaryValue& onc_object,
      Result* result);

 private:
  
  
  
  virtual scoped_ptr<base::Value> MapValue(const OncValueSignature& signature,
                                           const base::Value& onc_value,
                                           bool* error) OVERRIDE;

  
  
  
  
  virtual scoped_ptr<base::DictionaryValue> MapObject(
      const OncValueSignature& signature,
      const base::DictionaryValue& onc_object,
      bool* error) OVERRIDE;

  
  virtual scoped_ptr<base::Value> MapField(
      const std::string& field_name,
      const OncValueSignature& object_signature,
      const base::Value& onc_value,
      bool* found_unknown_field,
      bool* error) OVERRIDE;

  
  
  virtual scoped_ptr<base::ListValue> MapArray(
      const OncValueSignature& array_signature,
      const base::ListValue& onc_array,
      bool* nested_error) OVERRIDE;

  
  virtual scoped_ptr<base::Value> MapEntry(int index,
                                           const OncValueSignature& signature,
                                           const base::Value& onc_value,
                                           bool* error) OVERRIDE;

  
  
  // dictionary into which the repaired fields are written.
  bool ValidateObjectDefault(const OncValueSignature& object_signature,
                             const base::DictionaryValue& onc_object,
                             base::DictionaryValue* result);

  
  
  bool ValidateRecommendedField(const OncValueSignature& object_signature,
                                base::DictionaryValue* result);

  bool ValidateToplevelConfiguration(base::DictionaryValue* result);
  bool ValidateNetworkConfiguration(base::DictionaryValue* result);
  bool ValidateEthernet(base::DictionaryValue* result);
  bool ValidateIPConfig(base::DictionaryValue* result);
  bool ValidateWiFi(base::DictionaryValue* result);
  bool ValidateVPN(base::DictionaryValue* result);
  bool ValidateIPsec(base::DictionaryValue* result);
  bool ValidateOpenVPN(base::DictionaryValue* result);
  bool ValidateVerifyX509(base::DictionaryValue* result);
  bool ValidateCertificatePattern(base::DictionaryValue* result);
  bool ValidateProxySettings(base::DictionaryValue* result);
  bool ValidateProxyLocation(base::DictionaryValue* result);
  bool ValidateEAP(base::DictionaryValue* result);
  bool ValidateCertificate(base::DictionaryValue* result);

  bool FieldExistsAndHasNoValidValue(const base::DictionaryValue& object,
                                     const std::string &field_name,
                                     const char** valid_values);

  bool FieldExistsAndIsNotInRange(const base::DictionaryValue& object,
                                  const std::string &field_name,
                                  int lower_bound,
                                  int upper_bound);

  bool FieldExistsAndIsEmpty(const base::DictionaryValue& object,
                             const std::string& field_name);

  bool RequireField(const base::DictionaryValue& dict, const std::string& key);

  
  
  bool IsCertPatternInDevicePolicy(const std::string& cert_type);

  
  bool IsGlobalNetworkConfigInUserImport(
      const base::DictionaryValue& onc_object);

  std::string MessageHeader();

  const bool error_on_unknown_field_;
  const bool error_on_wrong_recommended_;
  const bool error_on_missing_field_;
  const bool managed_onc_;

  ::onc::ONCSource onc_source_;

  
  
  std::vector<std::string> path_;

  
  
  bool error_or_warning_found_;

  DISALLOW_COPY_AND_ASSIGN(Validator);
};

}  
}  

#endif  

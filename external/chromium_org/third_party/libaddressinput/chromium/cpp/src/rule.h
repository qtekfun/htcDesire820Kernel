// Copyright (C) 2013 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef I18N_ADDRESSINPUT_RULE_H_
#define I18N_ADDRESSINPUT_RULE_H_

#include <libaddressinput/address_field.h>
#include <libaddressinput/util/basictypes.h>

#include <string>
#include <vector>

namespace i18n {
namespace addressinput {

class Rule {
 public:
  Rule();
  ~Rule();

  
  void CopyFrom(const Rule& rule);

  
  
  bool ParseSerializedRule(const std::string& serialized_rule);

  
  
  const std::vector<AddressField>& GetFormat() const { return format_; }

  
  
  
  
  const std::vector<std::string>& GetSubKeys() const { return sub_keys_; }

  
  
  const std::vector<std::string>& GetLanguages() const { return languages_; }

  
  const std::string& GetLanguage() const { return language_; }

  
  
  int GetAdminAreaNameMessageId() const { return admin_area_name_message_id_; }

  
  
  int GetPostalCodeNameMessageId() const {
    return postal_code_name_message_id_;
  }

 private:
  std::vector<AddressField> format_;
  std::vector<std::string> sub_keys_;
  std::vector<std::string> languages_;
  std::string language_;
  int admin_area_name_message_id_;
  int postal_code_name_message_id_;

  DISALLOW_COPY_AND_ASSIGN(Rule);
};

}  
}  

#endif  

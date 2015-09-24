// Copyright (C) 2013 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef I18N_ADDRESSINPUT_ADDRESS_VALIDATOR_H_
#define I18N_ADDRESSINPUT_ADDRESS_VALIDATOR_H_

#include <libaddressinput/address_field.h>
#include <libaddressinput/address_problem.h>
#include <libaddressinput/util/basictypes.h>
#include <libaddressinput/util/scoped_ptr.h>

#include <map>
#include <string>
#include <vector>

namespace i18n {
namespace addressinput {

class Downloader;
class LoadRulesDelegate;
class Localization;
class RuleRetriever;
class Storage;
struct AddressData;

typedef std::vector<AddressProblem> AddressProblems;
typedef std::multimap<AddressField, AddressProblem::Type> AddressProblemFilter;

class AddressValidator {
 public:
  
  enum Status {
    
    
    SUCCESS,

    
    
    RULES_UNAVAILABLE,

    
    RULES_NOT_READY
  };

  
  
  AddressValidator(const Downloader* downloader,
                   Storage* storage,
                   LoadRulesDelegate* load_rules_delegate);
  ~AddressValidator();

  
  
  
  
  
  
  
  void LoadRules(const std::string& country_code);

  
  
  
  
  
  
  
  
  
  Status ValidateAddress(const AddressData& address,
                         const AddressProblemFilter& filter,
                         const Localization& localization,
                         AddressProblems* problems) const;
 private:
  scoped_ptr<RuleRetriever> rule_retriever_;
  LoadRulesDelegate* load_rules_delegate_;

  DISALLOW_COPY_AND_ASSIGN(AddressValidator);
};

}  
}  

#endif  

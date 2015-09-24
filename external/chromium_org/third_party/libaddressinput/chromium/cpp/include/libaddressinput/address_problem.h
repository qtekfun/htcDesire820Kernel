// Copyright (C) 2013 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef I18N_ADDRESSINPUT_ADDRESS_PROBLEM_H_
#define I18N_ADDRESSINPUT_ADDRESS_PROBLEM_H_

#include <libaddressinput/address_field.h>

#include <iosfwd>
#include <string>

namespace i18n {
namespace addressinput {

struct AddressProblem {
  
  enum Type {
    
    
    
    
    MISSING_REQUIRED_FIELD,

    
    
    
    
    
    
    UNKNOWN_VALUE,

    
    
    
    
    
    
    
    UNRECOGNIZED_FORMAT,

    
    
    
    
    
    
    MISMATCHING_VALUE
  };

  
  AddressField field;

  
  Type type;

  
  std::string description;
};

std::ostream& operator<<(std::ostream& o, AddressProblem::Type problem_type);

std::ostream& operator<<(std::ostream& o, const AddressProblem& problem);

}  
}  

#endif  

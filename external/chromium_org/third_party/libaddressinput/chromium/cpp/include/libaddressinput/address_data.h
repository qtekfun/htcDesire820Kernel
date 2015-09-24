// Copyright (C) 2013 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef I18N_ADDRESSINPUT_ADDRESS_DATA_H_
#define I18N_ADDRESSINPUT_ADDRESS_DATA_H_

#include <string>
#include <vector>

namespace i18n {
namespace addressinput {

struct AddressData {
  
  
  
  
  std::string language_code;

  
  
  
  std::string country_code;

  
  
  std::string administrative_area;

  
  
  std::string locality;

  
  
  std::string dependent_locality;

  
  
  
  std::string sorting_code;

  
  
  
  std::string postal_code;

  
  std::vector<std::string> address_lines;

  
  std::string organization;

  
  std::string recipient;
};

}  
}  

#endif  

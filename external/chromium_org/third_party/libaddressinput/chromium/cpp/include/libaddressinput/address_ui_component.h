// Copyright (C) 2013 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef I18N_ADDRESSINPUT_ADDRESS_UI_COMPONENT_H_
#define I18N_ADDRESSINPUT_ADDRESS_UI_COMPONENT_H_

#include <libaddressinput/address_field.h>

#include <string>

namespace i18n {
namespace addressinput {

struct AddressUiComponent {
  
  
  enum LengthHint {
    HINT_LONG,  
    HINT_SHORT  
  };

  
  AddressField field;

  
  std::string name;

  
  
  LengthHint length_hint;
};

}  
}  

#endif  

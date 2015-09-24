// Copyright (C) 2013 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef I18N_ADDRESSINPUT_UTIL_JSON_H_
#define I18N_ADDRESSINPUT_UTIL_JSON_H_

#include <string>

namespace i18n {
namespace addressinput {

class Json {
 public:
  virtual ~Json();

  
  static Json* Build();

  
  
  virtual bool ParseObject(const std::string& json) = 0;

  
  
  
  virtual bool HasStringValueForKey(const std::string& key) const = 0;

  
  
  
  virtual std::string GetStringValueForKey(const std::string& key) const = 0;

 protected:
  Json();
};

}  
}  

#endif  

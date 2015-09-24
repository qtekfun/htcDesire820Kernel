// Copyright (C) 2013 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef I18N_ADDRESSINPUT_LOOKUP_KEY_UTIL_H_
#define I18N_ADDRESSINPUT_LOOKUP_KEY_UTIL_H_

#include <string>

namespace i18n {
namespace addressinput {

class LookupKeyUtil {
 public:
  
  
  explicit LookupKeyUtil(const std::string& validation_data_url);
  ~LookupKeyUtil();

  
  
  
  std::string GetUrlForKey(const std::string& key) const;

  
  
  
  
  
  std::string GetKeyForUrl(const std::string& url) const;

  
  
  bool IsValidationDataUrl(const std::string& url) const;

 private:
  const std::string validation_data_url_;
};

}  
}  

#endif  

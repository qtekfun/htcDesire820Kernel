// Copyright (C) 2013 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef I18N_ADDRESSINPUT_STORAGE_H_
#define I18N_ADDRESSINPUT_STORAGE_H_

#include <libaddressinput/callback.h>

#include <string>

namespace i18n {
namespace addressinput {

class Storage {
 public:
  typedef i18n::addressinput::Callback<std::string, std::string> Callback;

  virtual ~Storage() {}

  
  virtual void Put(const std::string& key, const std::string& data) = 0;

  
  virtual void Get(const std::string& key,
                   const Callback& data_ready) const = 0;
};

}  
}  

#endif  

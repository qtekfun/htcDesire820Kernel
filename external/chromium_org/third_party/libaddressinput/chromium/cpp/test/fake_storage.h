// Copyright (C) 2013 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef I18N_ADDRESSINPUT_FAKE_STORAGE_H_
#define I18N_ADDRESSINPUT_FAKE_STORAGE_H_

#include <libaddressinput/storage.h>

#include <map>
#include <string>

namespace i18n {
namespace addressinput {

class FakeStorage : public Storage {
 public:
  FakeStorage();
  virtual ~FakeStorage();

  
  virtual void Put(const std::string& key, const std::string& data);
  virtual void Get(const std::string& key, const Callback& data_ready) const;

 private:
  std::map<std::string, std::string> data_;
};

}  
}  

#endif  

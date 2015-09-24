// Copyright (C) 2013 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef I18N_ADDRESSINPUT_VALIDATING_STORAGE_H_
#define I18N_ADDRESSINPUT_VALIDATING_STORAGE_H_

#include <libaddressinput/storage.h>
#include <libaddressinput/util/basictypes.h>
#include <libaddressinput/util/scoped_ptr.h>

#include <string>

namespace i18n {
namespace addressinput {

class ValidatingStorage : public Storage {
 public:
  
  explicit ValidatingStorage(Storage* storage);
  virtual ~ValidatingStorage();

  
  virtual void Put(const std::string& key, const std::string& data);
  virtual void Get(const std::string& key, const Callback& data_ready) const;

 private:
  
  scoped_ptr<Storage> wrapped_storage_;

  DISALLOW_COPY_AND_ASSIGN(ValidatingStorage);
};

}  
}  

#endif  

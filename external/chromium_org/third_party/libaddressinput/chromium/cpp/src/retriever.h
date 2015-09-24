// Copyright (C) 2013 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef I18N_ADDRESSINPUT_RETRIEVER_H_
#define I18N_ADDRESSINPUT_RETRIEVER_H_

#include <libaddressinput/callback.h>
#include <libaddressinput/util/basictypes.h>
#include <libaddressinput/util/scoped_ptr.h>

#include <string>

#include "lookup_key_util.h"

namespace i18n {
namespace addressinput {

class Downloader;
class Storage;

class Retriever {
 public:
  typedef i18n::addressinput::Callback<std::string, std::string> Callback;

  
  Retriever(const std::string& validation_data_url,
            const Downloader* downloader,
            Storage* storage);
  ~Retriever();

  
  
  
  void Retrieve(const std::string& key, const Callback& retrieved) const;

 private:
  const LookupKeyUtil lookup_key_util_;
  scoped_ptr<const Downloader> downloader_;
  scoped_ptr<Storage> storage_;

  DISALLOW_COPY_AND_ASSIGN(Retriever);
};

}  
}  

#endif  

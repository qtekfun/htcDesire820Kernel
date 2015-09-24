// Copyright 2008 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//      http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPEN_VCDIFF_OUTPUT_STRING_H_
#define OPEN_VCDIFF_OUTPUT_STRING_H_

#include <stddef.h>  

namespace open_vcdiff {

class OutputStringInterface {
 public:
  virtual ~OutputStringInterface() { }

  virtual OutputStringInterface& append(const char* s, size_t n) = 0;

  virtual void clear() = 0;

  virtual void push_back(char c) = 0;

  virtual void ReserveAdditionalBytes(size_t res_arg) = 0;

  virtual size_t size() const = 0;
};

template<class StringClass>
class OutputString : public OutputStringInterface {
 public:
  explicit OutputString(StringClass* impl) : impl_(impl) { }

  virtual ~OutputString() { }

  virtual OutputString& append(const char* s, size_t n) {
    impl_->append(s, n);
    return *this;
  }

  virtual void clear() {
    impl_->clear();
  }

  virtual void push_back(char c) {
    impl_->push_back(c);
  }

  virtual void ReserveAdditionalBytes(size_t res_arg) {
    impl_->reserve(impl_->size() + res_arg);
  }

  virtual size_t size() const {
    return impl_->size();
  }

 protected:
  StringClass* impl_;

 private:
  
  OutputString(const OutputString&);
  void operator=(const OutputString&);
};

template<> class OutputString<OutputStringInterface> { };

}  

#endif  

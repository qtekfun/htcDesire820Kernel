// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_I18N_STRING_SEARCH_H_
#define BASE_I18N_STRING_SEARCH_H_

#include "base/i18n/base_i18n_export.h"
#include "base/strings/string16.h"

struct UStringSearch;

namespace base {
namespace i18n {

BASE_I18N_EXPORT
    bool StringSearchIgnoringCaseAndAccents(const string16& find_this,
                                            const string16& in_this,
                                            size_t* match_index,
                                            size_t* match_length);

class BASE_I18N_EXPORT FixedPatternStringSearchIgnoringCaseAndAccents {
 public:
  explicit FixedPatternStringSearchIgnoringCaseAndAccents(
      const string16& find_this);
  ~FixedPatternStringSearchIgnoringCaseAndAccents();

  
  
  
  bool Search(const string16& in_this,
              size_t* match_index,
              size_t* match_length);

 private:
  string16 find_this_;
  UStringSearch* search_;
};

}  
}  

#endif  

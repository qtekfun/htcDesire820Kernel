// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_URL_MATCHER_STRING_PATTERN_H_
#define COMPONENTS_URL_MATCHER_STRING_PATTERN_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "components/url_matcher/url_matcher_export.h"

namespace url_matcher {

class URL_MATCHER_EXPORT StringPattern {
 public:
  typedef int ID;

  StringPattern(const std::string& pattern, ID id);
  ~StringPattern();
  const std::string& pattern() const { return pattern_; }
  ID id() const { return id_; }

  bool operator<(const StringPattern& rhs) const;

 private:
  std::string pattern_;
  ID id_;

  DISALLOW_COPY_AND_ASSIGN(StringPattern);
};

}  

#endif  

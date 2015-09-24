// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_SCOPED_LOCALE_H_
#define BASE_TEST_SCOPED_LOCALE_H_

#include <string>

#include "base/basictypes.h"

namespace base {

class ScopedLocale {
 public:
  explicit ScopedLocale(const std::string& locale);
  ~ScopedLocale();

 private:
  std::string prev_locale_;

  DISALLOW_COPY_AND_ASSIGN(ScopedLocale);
};

}  

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_FLAGS_STORAGE_H_
#define CHROME_BROWSER_FLAGS_STORAGE_H_

#include <set>
#include <string>

namespace about_flags {

class FlagsStorage {
 public:
  virtual ~FlagsStorage() {}

  
  virtual std::set<std::string> GetFlags() = 0;
  
  virtual bool SetFlags(const std::set<std::string>& flags) = 0;
};

}  

#endif  

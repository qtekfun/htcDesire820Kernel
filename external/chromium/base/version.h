// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_VERSION_H_
#define BASE_VERSION_H_
#pragma once

#include <string>
#include <vector>

#include "base/base_api.h"
#include "base/basictypes.h"
#include "base/gtest_prod_util.h"

class BASE_API Version {
 public:
  
  
  
  Version();

  ~Version();

  
  
  
  static Version* GetVersionFromString(const std::string& version_str);

  
  Version* Clone() const;

  bool Equals(const Version& other) const;

  
  int CompareTo(const Version& other) const;

  
  const std::string GetString() const;

  const std::vector<uint16>& components() const { return components_; }

 private:
  bool InitFromString(const std::string& version_str);

  bool is_valid_;
  std::vector<uint16> components_;

  FRIEND_TEST_ALL_PREFIXES(VersionTest, DefaultConstructor);
  FRIEND_TEST_ALL_PREFIXES(VersionTest, GetVersionFromString);
  FRIEND_TEST_ALL_PREFIXES(VersionTest, Compare);
};

#endif  

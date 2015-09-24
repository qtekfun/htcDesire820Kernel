// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CLOUD_PRINT_GCP20_PROTOTYPE_X_PRIVET_TOKEN_H_
#define CLOUD_PRINT_GCP20_PROTOTYPE_X_PRIVET_TOKEN_H_

#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/time/time.h"

class XPrivetToken {
 public:
  
  XPrivetToken();

  
  ~XPrivetToken() {}

  
  
  std::string GenerateXToken();

  
  bool CheckValidXToken(const std::string& token) const;

 private:
  FRIEND_TEST_ALL_PREFIXES(XPrivetTokenTest, Generation);
  FRIEND_TEST_ALL_PREFIXES(XPrivetTokenTest, CheckingValid);
  FRIEND_TEST_ALL_PREFIXES(XPrivetTokenTest, CheckingInvalid);

  
  XPrivetToken(const std::string& secret, const base::Time& gen_time);

  
  std::string GenerateXTokenWithTime(uint64 issue_time) const;

  
  void UpdateSecret();

  
  std::string secret_;

  
  base::Time last_gen_time_;
};

#endif  


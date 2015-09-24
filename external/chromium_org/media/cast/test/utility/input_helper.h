// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CAST_TEST_UTILITY_INPUT_HELPER_
#define MEDIA_CAST_TEST_UTILITY_INPUT_HELPER_

#include <string>

namespace media {
namespace cast {
namespace test {

class InputBuilder {
 public:
  InputBuilder(const std::string& title,
               const std::string& default_value,
               int low_range,
               int high_range);
  virtual ~InputBuilder();

  
  
  
  
  
  
  std::string GetStringInput() const;
  int GetIntInput() const;

 private:
  bool ValidateInput(const std::string input) const;

  const std::string title_;
  const std::string default_value_;
  
  const int low_range_;
  const int high_range_;
};

}  
}  
}  

#endif  

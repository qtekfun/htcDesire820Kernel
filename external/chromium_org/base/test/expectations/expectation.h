// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_EXPECTATIONS_EXPECTATION_H_
#define BASE_TEST_EXPECTATIONS_EXPECTATION_H_

#include <string>
#include <vector>

#include "base/base_export.h"
#include "base/compiler_specific.h"
#include "base/strings/string_piece.h"

namespace test_expectations {

enum Result {
  
  RESULT_FAILURE,

  
  RESULT_TIMEOUT,

  
  RESULT_CRASH,

  
  RESULT_SKIP,

  
  RESULT_PASS,
};

// Converts a text string form of a |result| to its enum value, written to
bool ResultFromString(const base::StringPiece& result,
                      Result* out_result) WARN_UNUSED_RESULT;

struct Platform {
  
  std::string name;

  
  
  std::string variant;
};

// Converts a text string |modifier| to a Platform struct, written to
bool PlatformFromString(const base::StringPiece& modifier,
                        Platform* out_platform) WARN_UNUSED_RESULT;

Platform GetCurrentPlatform();

enum Configuration {
  CONFIGURATION_UNSPECIFIED,
  CONFIGURATION_DEBUG,
  CONFIGURATION_RELEASE,
};

bool ConfigurationFromString(const base::StringPiece& modifier,
    Configuration* out_configuration) WARN_UNUSED_RESULT;

Configuration GetCurrentConfiguration();

struct Expectation {
  Expectation();
  ~Expectation();

  
  std::string test_name;

  
  std::vector<Platform> platforms;

  
  Configuration configuration;

  
  Result result;
};

}  

#endif  

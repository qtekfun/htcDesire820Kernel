// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_SANDBOX_MAC_H_
#define CONTENT_COMMON_SANDBOX_MAC_H_

#include <string>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/gtest_prod_util.h"
#include "content/common/content_export.h"
#include "content/public/common/sandbox_type_mac.h"

namespace base {
class FilePath;
}

#if __OBJC__
@class NSArray;
@class NSString;
#else
class NSArray;
class NSString;
#endif

namespace content {

class SandboxSubstring {
 public:
  enum SandboxSubstringType {
    PLAIN,    
    LITERAL,  
    REGEX,    
  };

  SandboxSubstring() {}

  explicit SandboxSubstring(const std::string& value)
      : value_(value),
        type_(PLAIN) {}

  SandboxSubstring(const std::string& value, SandboxSubstringType type)
      : value_(value),
        type_(type) {}

  const std::string& value() { return value_; }
  SandboxSubstringType type() { return type_; }

 private:
  std::string value_;
  SandboxSubstringType type_;
};

class CONTENT_EXPORT Sandbox {
 public:
  
  typedef base::hash_map<std::string, SandboxSubstring>
      SandboxVariableSubstitions;

  
  
  
  
  
  static void SandboxWarmup(int sandbox_type);

  
  
  
  
  
  
  
  static bool EnableSandbox(int sandbox_type,
                            const base::FilePath& allowed_dir);

  
  static bool SandboxIsCurrentlyActive();

  
  

  
  
  
  

  
  
  
  
  static NSString* BuildAllowDirectoryAccessSandboxString(
                       const base::FilePath& allowed_dir,
                       SandboxVariableSubstitions* substitutions);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static bool PostProcessSandboxProfile(
                  NSString* in_sandbox_data,
                  NSArray* comments_to_remove,
                  SandboxVariableSubstitions& substitutions,
                  std::string *final_sandbox_profile_str);

 private:
  
  
  static NSString* AllowMetadataForPath(const base::FilePath& allowed_path);

  
  
  
  static bool QuotePlainString(const std::string& src_utf8, std::string* dst);

  
  
  
  
  
  
  
  
  
  
  
  
  static bool QuoteStringForRegex(const std::string& str_utf8,
                                  std::string* dst);

  
  
  
  static base::FilePath GetCanonicalSandboxPath(const base::FilePath& path);

  FRIEND_TEST_ALL_PREFIXES(MacDirAccessSandboxTest, StringEscape);
  FRIEND_TEST_ALL_PREFIXES(MacDirAccessSandboxTest, RegexEscape);
  FRIEND_TEST_ALL_PREFIXES(MacDirAccessSandboxTest, SandboxAccess);

  DISALLOW_IMPLICIT_CONSTRUCTORS(Sandbox);
};

}  

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_TLD_CLEANUP_TLD_CLEANUP_UTIL_H_
#define NET_TOOLS_TLD_CLEANUP_TLD_CLEANUP_UTIL_H_

#include <map>
#include <string>

namespace base {
class FilePath;
}  

namespace net {
namespace tld_cleanup {

struct Rule {
  bool exception;
  bool wildcard;
  bool is_private;
};

typedef std::map<std::string, Rule> RuleMap;

typedef enum {
  kSuccess,
  kWarning,
  kError,
} NormalizeResult;

NormalizeResult NormalizeFile(const base::FilePath& in_filename,
                              const base::FilePath& out_filename);

NormalizeResult NormalizeDataToRuleMap(const std::string data,
                                       RuleMap* rules);

}  
}  

#endif  

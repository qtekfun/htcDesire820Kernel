// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_NAME_VALUE_PAIRS_PARSER_H_
#define CHROME_BROWSER_CHROMEOS_NAME_VALUE_PAIRS_PARSER_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"

namespace chromeos {

class NameValuePairsParser {
 public:
  typedef std::map<std::string, std::string> NameValueMap;

  // The obtained info will be written into machine_info.
  explicit NameValuePairsParser(NameValueMap* map);

  void AddNameValuePair(const std::string& key, const std::string& value);

  
  bool GetSingleValueFromTool(int argc, const char* argv[],
                              const std::string& key);
  
  
  bool ParseNameValuePairsFromTool(int argc, const char* argv[],
                                   const std::string& eq,
                                   const std::string& delim);

 private:
  
  
  
  bool ParseNameValuePairs(const std::string& in_string,
                           const std::string& eq,
                           const std::string& delim);

  NameValueMap* map_;

  DISALLOW_COPY_AND_ASSIGN(NameValuePairsParser);
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_SYSTEM_NAME_VALUE_PAIRS_PARSER_H_
#define CHROMEOS_SYSTEM_NAME_VALUE_PAIRS_PARSER_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "chromeos/chromeos_export.h"

namespace base {
class FilePath;
}

namespace chromeos {
namespace system {

class CHROMEOS_EXPORT NameValuePairsParser {
 public:
  typedef std::map<std::string, std::string> NameValueMap;

  // The obtained info will be written into the given map.
  explicit NameValuePairsParser(NameValueMap* map);

  void AddNameValuePair(const std::string& key, const std::string& value);

  
  
  
  
  bool GetSingleValueFromTool(int argc, const char* argv[],
                              const std::string& key);

  
  
  
  bool GetNameValuePairsFromFile(const base::FilePath& file_path,
                                 const std::string& eq,
                                 const std::string& delim);

  
  
  
  
  
  bool ParseNameValuePairs(const std::string& in_string,
                           const std::string& eq,
                           const std::string& delim);

  
  
  
  
  
  bool ParseNameValuePairsWithComments(const std::string& in_string,
                                       const std::string& eq,
                                       const std::string& delim,
                                       const std::string& comment_delim);

  
  
  bool ParseNameValuePairsFromTool(
      int argc,
      const char* argv[],
      const std::string& eq,
      const std::string& delim,
      const std::string& comment_delim);

 private:
  NameValueMap* map_;

  DISALLOW_COPY_AND_ASSIGN(NameValuePairsParser);
};

}  
}  

#endif  

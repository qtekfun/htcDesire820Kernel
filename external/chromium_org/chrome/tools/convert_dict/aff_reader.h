// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TOOLS_CONVERT_DICT_AFF_READER_H__
#define CHROME_TOOLS_CONVERT_DICT_AFF_READER_H__

#include <map>
#include <stdio.h>
#include <string>
#include <vector>

namespace base {
class FilePath;
}

namespace convert_dict {

class AffReader {
 public:
  explicit AffReader(const base::FilePath& path);
  ~AffReader();

  bool Read();

  
  
  
  bool has_indexed_affixes() const { return has_indexed_affixes_; }

  
  
  const char* encoding() const { return encoding_.c_str(); }

  
  
  bool EncodingToUTF8(const std::string& encoded, std::string* utf8) const;

  
  
  
  
  int GetAFIndexForAFString(const std::string& af_string);

  
  const std::string& comments() const { return intro_comment_; }
  const std::vector<std::string>& affix_rules() const { return affix_rules_; }
  const std::vector< std::pair<std::string, std::string> >&
      replacements() const {
    return replacements_;
  }
  const std::vector<std::string>& other_commands() const {
    return other_commands_;
  }

  
  
  
  std::vector<std::string> GetAffixGroups() const;

 private:
  
  
  int AddAffixGroup(std::string* rule);  
  void AddAffix(std::string* rule);  
  void AddReplacement(std::string* rule);
  

  
  
  void HandleRawCommand(const std::string& line);
  void HandleEncodedCommand(const std::string& line);

  FILE* file_;

  
  // first command. We want to store this since it often contains the copyright
  
  std::string intro_comment_;

  
  std::string encoding_;

  
  
  
  
  
  std::map<std::string, int> affix_groups_;

  
  
  
  bool has_indexed_affixes_;

  
  
  std::vector<std::string> affix_rules_;

  
  
  std::vector< std::pair<std::string, std::string> > replacements_;

  
  std::vector<std::string> other_commands_;
};

}  

#endif  

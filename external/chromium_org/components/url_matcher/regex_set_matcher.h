// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_URL_MATCHER_REGEX_SET_MATCHER_H_
#define COMPONENTS_URL_MATCHER_REGEX_SET_MATCHER_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "components/url_matcher/string_pattern.h"
#include "components/url_matcher/substring_set_matcher.h"
#include "components/url_matcher/url_matcher_export.h"

namespace re2 {
class FilteredRE2;
}

namespace url_matcher {

class URL_MATCHER_EXPORT RegexSetMatcher {
 public:
  RegexSetMatcher();
  virtual ~RegexSetMatcher();

  
  
  
  
  void AddPatterns(const std::vector<const StringPattern*>& regex_list);

  
  void ClearPatterns();

  
  
  bool Match(const std::string& text,
             std::set<StringPattern::ID>* matches) const;

  bool IsEmpty() const;

 private:
  typedef int RE2ID;
  typedef std::map<StringPattern::ID, const StringPattern*> RegexMap;
  typedef std::vector<StringPattern::ID> RE2IDMap;

  
  
  std::vector<RE2ID> FindSubstringMatches(const std::string& text) const;

  
  
  
  
  void RebuildMatcher();

  
  void DeleteSubstringPatterns();

  
  RegexMap regexes_;
  
  
  RE2IDMap re2_id_map_;

  scoped_ptr<re2::FilteredRE2> filtered_re2_;
  scoped_ptr<SubstringSetMatcher> substring_matcher_;

  
  
  std::vector<const StringPattern*> substring_patterns_;
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMMON_URL_PATTERN_SET_H_
#define EXTENSIONS_COMMMON_URL_PATTERN_SET_H_

#include <set>

#include "base/memory/scoped_ptr.h"
#include "extensions/common/url_pattern.h"

class GURL;

namespace base {
class ListValue;
class Value;
}

namespace extensions {

class URLPatternSet {
 public:
  typedef std::set<URLPattern>::const_iterator const_iterator;
  typedef std::set<URLPattern>::iterator iterator;

  
  static void CreateDifference(const URLPatternSet& set1,
                               const URLPatternSet& set2,
                               URLPatternSet* out);

  
  
  static void CreateIntersection(const URLPatternSet& set1,
                                 const URLPatternSet& set2,
                                 URLPatternSet* out);

  
  static void CreateUnion(const URLPatternSet& set1,
                          const URLPatternSet& set2,
                          URLPatternSet* out);

  
  static void CreateUnion(const std::vector<URLPatternSet>& sets,
                          URLPatternSet* out);

  URLPatternSet();
  URLPatternSet(const URLPatternSet& rhs);
  explicit URLPatternSet(const std::set<URLPattern>& patterns);
  ~URLPatternSet();

  URLPatternSet& operator=(const URLPatternSet& rhs);
  bool operator==(const URLPatternSet& rhs) const;

  bool is_empty() const;
  size_t size() const;
  const std::set<URLPattern>& patterns() const { return patterns_; }
  const_iterator begin() const { return patterns_.begin(); }
  const_iterator end() const { return patterns_.end(); }

  
  
  bool AddPattern(const URLPattern& pattern);

  
  void AddPatterns(const URLPatternSet& set);

  void ClearPatterns();

  
  
  bool Contains(const URLPatternSet& set) const;

  
  bool ContainsPattern(const URLPattern& pattern) const;

  
  bool MatchesURL(const GURL& url) const;

  bool MatchesSecurityOrigin(const GURL& origin) const;

  
  bool OverlapsWith(const URLPatternSet& other) const;

  
  scoped_ptr<base::ListValue> ToValue() const;
  bool Populate(const base::ListValue& value,
                int valid_schemes,
                bool allow_file_access,
                std::string* error);

  bool Populate(const std::vector<std::string>& patterns,
                int valid_schemes,
                bool allow_file_access,
                std::string* error);

 private:
  
  std::set<URLPattern> patterns_;
};

}  

#endif  

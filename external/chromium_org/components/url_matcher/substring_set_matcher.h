// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_URL_MATCHER_SUBSTRING_SET_MATCHER_H_
#define COMPONENTS_URL_MATCHER_SUBSTRING_SET_MATCHER_H_

#include <limits>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "components/url_matcher/string_pattern.h"
#include "components/url_matcher/url_matcher_export.h"

namespace url_matcher {

class URL_MATCHER_EXPORT SubstringSetMatcher {
 public:
  SubstringSetMatcher();
  ~SubstringSetMatcher();

  
  
  
  
  void RegisterPatterns(const std::vector<const StringPattern*>& patterns);

  
  void UnregisterPatterns(const std::vector<const StringPattern*>& patterns);

  
  
  void RegisterAndUnregisterPatterns(
      const std::vector<const StringPattern*>& to_register,
      const std::vector<const StringPattern*>& to_unregister);

  
  
  
  bool Match(const std::string& text,
             std::set<StringPattern::ID>* matches) const;

  
  bool IsEmpty() const;

 private:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  class AhoCorasickNode {
   public:
    
    typedef std::map<char, uint32> Edges;
    typedef std::set<StringPattern::ID> Matches;

    static const uint32 kNoSuchEdge;  

    AhoCorasickNode();
    ~AhoCorasickNode();
    AhoCorasickNode(const AhoCorasickNode& other);
    AhoCorasickNode& operator=(const AhoCorasickNode& other);

    uint32 GetEdge(char c) const;
    void SetEdge(char c, uint32 node);
    const Edges& edges() const { return edges_; }

    uint32 failure() const { return failure_; }
    void set_failure(uint32 failure) { failure_ = failure; }

    void AddMatch(StringPattern::ID id);
    void AddMatches(const Matches& matches);
    const Matches& matches() const { return matches_; }

   private:
    
    Edges edges_;

    
    uint32 failure_;

    
    Matches matches_;
  };

  typedef std::map<StringPattern::ID, const StringPattern*> SubstringPatternMap;
  typedef std::vector<const StringPattern*> SubstringPatternVector;

  
  void RebuildAhoCorasickTree(const SubstringPatternVector& sorted_patterns);

  
  
  
  void InsertPatternIntoAhoCorasickTree(const StringPattern* pattern);
  void CreateFailureEdges();

  
  
  SubstringPatternMap patterns_;

  
  std::vector<AhoCorasickNode> tree_;

  DISALLOW_COPY_AND_ASSIGN(SubstringSetMatcher);
};

}  

#endif  

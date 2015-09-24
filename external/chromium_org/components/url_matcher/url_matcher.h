// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_URL_MATCHER_URL_MATCHER_H_
#define COMPONENTS_URL_MATCHER_URL_MATCHER_H_

#include <set>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "components/url_matcher/regex_set_matcher.h"
#include "components/url_matcher/substring_set_matcher.h"
#include "components/url_matcher/url_matcher_export.h"

class GURL;

namespace base {
class DictionaryValue;
}

namespace url_matcher {

class URL_MATCHER_EXPORT URLMatcherCondition {
 public:
  enum Criterion {
    HOST_PREFIX,
    HOST_SUFFIX,
    HOST_CONTAINS,
    HOST_EQUALS,
    PATH_PREFIX,
    PATH_SUFFIX,
    PATH_CONTAINS,
    PATH_EQUALS,
    QUERY_PREFIX,
    QUERY_SUFFIX,
    QUERY_CONTAINS,
    QUERY_EQUALS,
    HOST_SUFFIX_PATH_PREFIX,
    HOST_EQUALS_PATH_PREFIX,
    URL_PREFIX,
    URL_SUFFIX,
    URL_CONTAINS,
    URL_EQUALS,
    URL_MATCHES,
    ORIGIN_AND_PATH_MATCHES,  
  };

  URLMatcherCondition();
  ~URLMatcherCondition();
  URLMatcherCondition(Criterion criterion,
                      const StringPattern* substring_pattern);
  URLMatcherCondition(const URLMatcherCondition& rhs);
  URLMatcherCondition& operator=(const URLMatcherCondition& rhs);
  bool operator<(const URLMatcherCondition& rhs) const;

  Criterion criterion() const { return criterion_; }
  const StringPattern* string_pattern() const {
    return string_pattern_;
  }

  
  
  
  bool IsFullURLCondition() const;

  
  
  bool IsRegexCondition() const;

  
  
  bool IsOriginAndPathRegexCondition() const;

  
  
  bool IsMatch(const std::set<StringPattern::ID>& matching_patterns,
               const GURL& url) const;

 private:
  
  
  Criterion criterion_;

  
  const StringPattern* string_pattern_;
};

class URL_MATCHER_EXPORT URLMatcherConditionFactory {
 public:
  URLMatcherConditionFactory();
  ~URLMatcherConditionFactory();

  
  std::string CanonicalizeURLForComponentSearches(const GURL& url) const;

  
  
  
  
  
  
  URLMatcherCondition CreateHostPrefixCondition(const std::string& prefix);
  URLMatcherCondition CreateHostSuffixCondition(const std::string& suffix);
  URLMatcherCondition CreateHostContainsCondition(const std::string& str);
  URLMatcherCondition CreateHostEqualsCondition(const std::string& str);

  URLMatcherCondition CreatePathPrefixCondition(const std::string& prefix);
  URLMatcherCondition CreatePathSuffixCondition(const std::string& suffix);
  URLMatcherCondition CreatePathContainsCondition(const std::string& str);
  URLMatcherCondition CreatePathEqualsCondition(const std::string& str);

  URLMatcherCondition CreateQueryPrefixCondition(const std::string& prefix);
  URLMatcherCondition CreateQuerySuffixCondition(const std::string& suffix);
  URLMatcherCondition CreateQueryContainsCondition(const std::string& str);
  URLMatcherCondition CreateQueryEqualsCondition(const std::string& str);

  
  
  
  URLMatcherCondition CreateHostSuffixPathPrefixCondition(
      const std::string& host_suffix,
      const std::string& path_prefix);
  URLMatcherCondition CreateHostEqualsPathPrefixCondition(
      const std::string& host,
      const std::string& path_prefix);

  
  std::string CanonicalizeURLForFullSearches(const GURL& url) const;

  
  std::string CanonicalizeURLForRegexSearches(const GURL& url) const;
  
  std::string CanonicalizeURLForOriginAndPathRegexSearches(
      const GURL& url) const;

  URLMatcherCondition CreateURLPrefixCondition(const std::string& prefix);
  URLMatcherCondition CreateURLSuffixCondition(const std::string& suffix);
  URLMatcherCondition CreateURLContainsCondition(const std::string& str);
  URLMatcherCondition CreateURLEqualsCondition(const std::string& str);

  URLMatcherCondition CreateURLMatchesCondition(const std::string& regex);
  URLMatcherCondition CreateOriginAndPathMatchesCondition(
      const std::string& regex);

  
  
  
  void ForgetUnusedPatterns(
      const std::set<StringPattern::ID>& used_patterns);

  
  bool IsEmpty() const;

 private:
  
  
  
  URLMatcherCondition CreateCondition(URLMatcherCondition::Criterion criterion,
                                      const std::string& pattern);

  
  std::string CanonicalizeHostname(const std::string& hostname) const;

  
  
  int id_counter_;

  
  
  struct StringPatternPointerCompare {
    bool operator()(StringPattern* lhs, StringPattern* rhs) const;
  };
  
  
  typedef std::set<StringPattern*, StringPatternPointerCompare>
      PatternSingletons;
  PatternSingletons substring_pattern_singletons_;
  PatternSingletons regex_pattern_singletons_;
  PatternSingletons origin_and_path_regex_pattern_singletons_;

  DISALLOW_COPY_AND_ASSIGN(URLMatcherConditionFactory);
};

class URL_MATCHER_EXPORT URLMatcherSchemeFilter {
 public:
  explicit URLMatcherSchemeFilter(const std::string& filter);
  explicit URLMatcherSchemeFilter(const std::vector<std::string>& filters);
  ~URLMatcherSchemeFilter();
  bool IsMatch(const GURL& url) const;

 private:
  std::vector<std::string> filters_;

  DISALLOW_COPY_AND_ASSIGN(URLMatcherSchemeFilter);
};

class URL_MATCHER_EXPORT URLMatcherPortFilter {
 public:
  
  typedef std::pair<int, int> Range;
  explicit URLMatcherPortFilter(const std::vector<Range>& ranges);
  ~URLMatcherPortFilter();
  bool IsMatch(const GURL& url) const;

  
  static Range CreateRange(int from, int to);
  
  static Range CreateRange(int port);

 private:
  std::vector<Range> ranges_;

  DISALLOW_COPY_AND_ASSIGN(URLMatcherPortFilter);
};

class URL_MATCHER_EXPORT URLMatcherConditionSet
    : public base::RefCounted<URLMatcherConditionSet> {
 public:
  typedef int ID;
  typedef std::set<URLMatcherCondition> Conditions;
  typedef std::vector<scoped_refptr<URLMatcherConditionSet> > Vector;

  
  URLMatcherConditionSet(ID id, const Conditions& conditions);

  
  
  
  URLMatcherConditionSet(ID id, const Conditions& conditions,
                         scoped_ptr<URLMatcherSchemeFilter> scheme_filter,
                         scoped_ptr<URLMatcherPortFilter> port_filter);

  ID id() const { return id_; }
  const Conditions& conditions() const { return conditions_; }

  bool IsMatch(const std::set<StringPattern::ID>& matching_patterns,
               const GURL& url) const;

 private:
  friend class base::RefCounted<URLMatcherConditionSet>;
  ~URLMatcherConditionSet();
  ID id_;
  Conditions conditions_;
  scoped_ptr<URLMatcherSchemeFilter> scheme_filter_;
  scoped_ptr<URLMatcherPortFilter> port_filter_;

  DISALLOW_COPY_AND_ASSIGN(URLMatcherConditionSet);
};

class URL_MATCHER_EXPORT URLMatcher {
 public:
  URLMatcher();
  ~URLMatcher();

  
  
  
  
  
  void AddConditionSets(const URLMatcherConditionSet::Vector& condition_sets);

  
  
  
  void RemoveConditionSets(
      const std::vector<URLMatcherConditionSet::ID>& condition_set_ids);

  
  void ClearUnusedConditionSets();

  
  std::set<URLMatcherConditionSet::ID> MatchURL(const GURL& url) const;

  
  
  URLMatcherConditionFactory* condition_factory() {
    return &condition_factory_;
  }

  
  bool IsEmpty() const;

 private:
  void UpdateSubstringSetMatcher(bool full_url_conditions);
  void UpdateRegexSetMatcher();
  void UpdateTriggers();
  void UpdateConditionFactory();
  void UpdateInternalDatastructures();

  URLMatcherConditionFactory condition_factory_;

  
  
  typedef std::map<URLMatcherConditionSet::ID,
                   scoped_refptr<URLMatcherConditionSet> >
      URLMatcherConditionSets;
  URLMatcherConditionSets url_matcher_condition_sets_;

  
  
  typedef std::map<StringPattern::ID, std::set<URLMatcherConditionSet::ID> >
      StringPatternTriggers;
  StringPatternTriggers substring_match_triggers_;

  SubstringSetMatcher full_url_matcher_;
  SubstringSetMatcher url_component_matcher_;
  RegexSetMatcher regex_set_matcher_;
  RegexSetMatcher origin_and_path_regex_set_matcher_;
  std::set<const StringPattern*> registered_full_url_patterns_;
  std::set<const StringPattern*> registered_url_component_patterns_;

  DISALLOW_COPY_AND_ASSIGN(URLMatcher);
};

}  

#endif  

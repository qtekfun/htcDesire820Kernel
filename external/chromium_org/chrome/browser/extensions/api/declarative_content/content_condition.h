// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_CONTENT_CONTENT_CONDITION_H_
#define CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_CONTENT_CONTENT_CONDITION_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/values.h"
#include "chrome/browser/extensions/api/declarative/declarative_rule.h"
#include "components/url_matcher/url_matcher.h"
#include "url/gurl.h"

namespace extensions {

struct RendererContentMatchData {
  RendererContentMatchData();
  ~RendererContentMatchData();
  
  
  std::set<url_matcher::URLMatcherConditionSet::ID> page_url_matches;
  
  
  base::hash_set<std::string> css_selectors;
};

class ContentCondition {
 public:
  typedef RendererContentMatchData MatchData;

  ContentCondition(
      scoped_refptr<url_matcher::URLMatcherConditionSet> url_matcher_conditions,
      const std::vector<std::string>& css_selectors);
  ~ContentCondition();

  
  
  
  static scoped_ptr<ContentCondition> Create(
      const Extension* extension,
      url_matcher::URLMatcherConditionFactory* url_matcher_condition_factory,
      const base::Value& condition,
      std::string* error);

  
  
  bool IsFulfilled(const RendererContentMatchData& renderer_data) const;

  
  
  
  
  url_matcher::URLMatcherConditionSet::ID url_matcher_condition_set_id() const {
    return url_matcher_conditions_->id();
  }

  
  void GetURLMatcherConditionSets(
      url_matcher::URLMatcherConditionSet::Vector* condition_sets) const {
    if (url_matcher_conditions_.get())
      condition_sets->push_back(url_matcher_conditions_);
  }

  
  
  bool has_url_matcher_condition_set() const {
    return url_matcher_conditions_.get() != NULL;
  }

  
  const std::vector<std::string>& css_selectors() const {
    return css_selectors_;
  }

 private:
  scoped_refptr<url_matcher::URLMatcherConditionSet> url_matcher_conditions_;
  std::vector<std::string> css_selectors_;

  DISALLOW_COPY_AND_ASSIGN(ContentCondition);
};

typedef DeclarativeConditionSet<ContentCondition> ContentConditionSet;

}  

#endif  

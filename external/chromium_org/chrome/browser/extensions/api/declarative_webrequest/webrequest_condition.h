// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_WEBREQUEST_WEBREQUEST_CONDITION_H_
#define CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_WEBREQUEST_WEBREQUEST_CONDITION_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/linked_ptr.h"
#include "chrome/browser/extensions/api/declarative/declarative_rule.h"
#include "chrome/browser/extensions/api/declarative_webrequest/webrequest_condition_attribute.h"
#include "components/url_matcher/url_matcher.h"
#include "net/http/http_response_headers.h"

namespace extensions {

struct WebRequestData {
  WebRequestData(net::URLRequest* request, RequestStage stage);
  WebRequestData(
      net::URLRequest* request,
      RequestStage stage,
      const net::HttpResponseHeaders* original_response_headers);
  ~WebRequestData();

  
  net::URLRequest* request;
  
  RequestStage stage;
  
  
  const net::HttpResponseHeaders* original_response_headers;
};

struct WebRequestDataWithMatchIds {
  explicit WebRequestDataWithMatchIds(const WebRequestData* request_data);
  ~WebRequestDataWithMatchIds();

  const WebRequestData* data;
  std::set<url_matcher::URLMatcherConditionSet::ID> url_match_ids;
  std::set<url_matcher::URLMatcherConditionSet::ID> first_party_url_match_ids;
};

class WebRequestCondition {
 public:
  typedef WebRequestDataWithMatchIds MatchData;

  WebRequestCondition(
      scoped_refptr<url_matcher::URLMatcherConditionSet> url_matcher_conditions,
      scoped_refptr<url_matcher::URLMatcherConditionSet>
          first_party_url_matcher_conditions,
      const WebRequestConditionAttributes& condition_attributes);
  ~WebRequestCondition();

  
  
  static scoped_ptr<WebRequestCondition> Create(
      const Extension* extension,
      url_matcher::URLMatcherConditionFactory* url_matcher_condition_factory,
      const base::Value& condition,
      std::string* error);

  
  bool IsFulfilled(const MatchData& request_data) const;

  
  void GetURLMatcherConditionSets(
      url_matcher::URLMatcherConditionSet::Vector* condition_sets) const;

  
  
  
  int stages() const { return applicable_request_stages_; }

 private:
  
  scoped_refptr<url_matcher::URLMatcherConditionSet> url_matcher_conditions_;
  scoped_refptr<url_matcher::URLMatcherConditionSet>
      first_party_url_matcher_conditions_;

  
  WebRequestConditionAttributes condition_attributes_;

  
  
  int applicable_request_stages_;

  DISALLOW_COPY_AND_ASSIGN(WebRequestCondition);
};

typedef DeclarativeConditionSet<WebRequestCondition> WebRequestConditionSet;

}  

#endif  

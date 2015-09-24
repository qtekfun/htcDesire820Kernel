// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_URL_MATCHER_URL_MATCHER_FACTORY_H_
#define COMPONENTS_URL_MATCHER_URL_MATCHER_FACTORY_H_

#include <string>

#include "base/basictypes.h"
#include "components/url_matcher/url_matcher.h"
#include "components/url_matcher/url_matcher_export.h"

namespace base {
class DictionaryValue;
class Value;
}

namespace url_matcher {

class URL_MATCHER_EXPORT URLMatcherFactory {
 public:
  
  
  
  
  
  
  
  
  
  
  static scoped_refptr<URLMatcherConditionSet> CreateFromURLFilterDictionary(
      URLMatcherConditionFactory* url_matcher_condition_factory,
      const base::DictionaryValue* url_filter_dict,
      URLMatcherConditionSet::ID id,
      std::string* error);

 private:
  
  
  static bool IsURLMatcherConditionAttribute(
      const std::string& condition_attribute_name);

  
  static URLMatcherCondition CreateURLMatcherCondition(
      URLMatcherConditionFactory* url_matcher_condition_factory,
      const std::string& condition_attribute_name,
      const base::Value* value,
      std::string* error);

  static scoped_ptr<URLMatcherSchemeFilter> CreateURLMatcherScheme(
      const base::Value* value, std::string* error);

  static scoped_ptr<URLMatcherPortFilter> CreateURLMatcherPorts(
      const base::Value* value, std::string* error);

  DISALLOW_IMPLICIT_CONSTRUCTORS(URLMatcherFactory);
};

}  

#endif  

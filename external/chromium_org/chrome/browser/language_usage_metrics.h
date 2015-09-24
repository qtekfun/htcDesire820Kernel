// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_LANGUAGE_USAGE_METRICS_H_
#define CHROME_BROWSER_LANGUAGE_USAGE_METRICS_H_

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"

class LanguageUsageMetrics {
 public:
  
  
  
  
  
  static void RecordAcceptLanguages(const std::string& accept_languages);

  
  
  
  static void RecordApplicationLanguage(const std::string& application_locale);

  
  
  
  
  
  
  
  static int ToLanguageCode(const std::string &locale);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(LanguageUsageMetrics);

  
  
  static void ParseAcceptLanguages(const std::string& accept_languages,
                                   std::set<int>* languages);

  FRIEND_TEST_ALL_PREFIXES(LanguageUsageMetricsTest, ParseAcceptLanguages);
};

#endif  

// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_SAFE_BROWSING_PHISHING_URL_FEATURE_EXTRACTOR_H_
#define CHROME_RENDERER_SAFE_BROWSING_PHISHING_URL_FEATURE_EXTRACTOR_H_

#include <string>
#include <vector>

#include "base/basictypes.h"

class GURL;

namespace safe_browsing {
class FeatureMap;

class PhishingUrlFeatureExtractor {
 public:
  PhishingUrlFeatureExtractor();
  ~PhishingUrlFeatureExtractor();

  
  
  bool ExtractFeatures(const GURL& url, FeatureMap* features);

 private:
  friend class PhishingUrlFeatureExtractorTest;

  static const size_t kMinPathComponentLength = 3;

  
  
  
  static void SplitStringIntoLongAlphanumTokens(
      const std::string& full,
      std::vector<std::string>* tokens);

  DISALLOW_COPY_AND_ASSIGN(PhishingUrlFeatureExtractor);
};

}  

#endif  

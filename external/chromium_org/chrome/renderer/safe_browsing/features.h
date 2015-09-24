// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_SAFE_BROWSING_FEATURES_H_
#define CHROME_RENDERER_SAFE_BROWSING_FEATURES_H_

#include <string>
#include "base/basictypes.h"
#include "base/containers/hash_tables.h"

namespace safe_browsing {

class FeatureMap {
 public:
  FeatureMap();
  ~FeatureMap();

  
  
  
  bool AddBooleanFeature(const std::string& name);

  
  
  
  
  bool AddRealFeature(const std::string& name, double value);

  
  const base::hash_map<std::string, double>& features() const {
    return features_;
  }

  
  void Clear();

  
  
  
  static const size_t kMaxFeatureMapSize;

 private:
  base::hash_map<std::string, double> features_;

  DISALLOW_COPY_AND_ASSIGN(FeatureMap);
};

namespace features {


extern const char kUrlHostIsIpAddress[];
extern const char kUrlTldToken[];
extern const char kUrlDomainToken[];
extern const char kUrlOtherHostToken[];


extern const char kUrlNumOtherHostTokensGTOne[];
extern const char kUrlNumOtherHostTokensGTThree[];


extern const char kUrlPathToken[];


extern const char kPageHasForms[];
extern const char kPageActionOtherDomainFreq[];

extern const char kPageHasTextInputs[];
extern const char kPageHasPswdInputs[];
extern const char kPageHasRadioInputs[];
extern const char kPageHasCheckInputs[];


extern const char kPageExternalLinksFreq[];
extern const char kPageLinkDomain[];
extern const char kPageSecureLinksFreq[];


extern const char kPageNumScriptTagsGTOne[];
extern const char kPageNumScriptTagsGTSix[];


extern const char kPageImgOtherDomainFreq[];


extern const char kPageTerm[];

}  
}  

#endif  

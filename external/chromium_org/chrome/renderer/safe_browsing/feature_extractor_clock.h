// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_SAFE_BROWSING_FEATURE_EXTRACTOR_CLOCK_H_
#define CHROME_RENDERER_SAFE_BROWSING_FEATURE_EXTRACTOR_CLOCK_H_

#include "base/time/time.h"

namespace safe_browsing {

class FeatureExtractorClock {
 public:
  FeatureExtractorClock() {}
  virtual ~FeatureExtractorClock();

  
  virtual base::TimeTicks Now();

 private:
  DISALLOW_COPY_AND_ASSIGN(FeatureExtractorClock);
};

}  

#endif  

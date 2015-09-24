// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_DATE_TIME_SUGGESTION_H_
#define CONTENT_COMMON_DATE_TIME_SUGGESTION_H_

#include <vector>

#include "base/strings/string16.h"
#include "content/common/content_export.h"
#include "third_party/skia/include/core/SkColor.h"

namespace content {

struct CONTENT_EXPORT DateTimeSuggestion {
  DateTimeSuggestion() {}

  
  double value;
  
  base::string16 localized_value;
  
  base::string16 label;
};

}  

#endif  

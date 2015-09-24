// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_COLOR_SUGGESTION_H_
#define CONTENT_PUBLIC_COMMON_COLOR_SUGGESTION_H_

#include "base/strings/string16.h"
#include "content/common/content_export.h"
#include "third_party/skia/include/core/SkColor.h"

namespace blink {
struct WebColorSuggestion;
}

namespace content {

struct CONTENT_EXPORT ColorSuggestion {
  ColorSuggestion() {}
  explicit ColorSuggestion(const blink::WebColorSuggestion& suggestion);

  SkColor color;
  base::string16 label;
};

}  

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_FAVICON_URL_
#define CONTENT_PUBLIC_COMMON_FAVICON_URL_

#include "content/common/content_export.h"
#include "url/gurl.h"

namespace content {

struct CONTENT_EXPORT FaviconURL {
  
  enum IconType {
    INVALID_ICON = 0x0,
    FAVICON = 1 << 0,
    TOUCH_ICON = 1 << 1,
    TOUCH_PRECOMPOSED_ICON = 1 << 2
  };

  FaviconURL();
  FaviconURL(const GURL& url, IconType type);
  ~FaviconURL();

  
  GURL icon_url;

  
  IconType icon_type;
};

} 

#endif  

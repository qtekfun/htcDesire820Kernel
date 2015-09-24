// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_FAVICON_STATUS_H_
#define CONTENT_PUBLIC_BROWSER_FAVICON_STATUS_H_

#include "content/common/content_export.h"
#include "ui/gfx/image/image.h"
#include "url/gurl.h"

namespace content {

struct CONTENT_EXPORT FaviconStatus {
  FaviconStatus();

  
  
  bool valid;

  
  GURL url;

  
  
  
  
  gfx::Image image;

  
};

}  

#endif  

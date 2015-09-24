// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_FRAME_HOST_DEBUG_URLS_H_
#define CONTENT_BROWSER_FRAME_HOST_DEBUG_URLS_H_

#include "content/public/common/page_transition_types.h"

class GURL;

namespace content {

bool HandleDebugURL(const GURL& url, PageTransition transition);

bool IsRendererDebugURL(const GURL& url);

}  

#endif  

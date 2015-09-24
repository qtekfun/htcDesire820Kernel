// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GPU_COMPOSITOR_UTIL_H_
#define CONTENT_BROWSER_GPU_COMPOSITOR_UTIL_H_

#include "base/values.h"
#include "content/common/content_export.h"

namespace content {

CONTENT_EXPORT bool IsThreadedCompositingEnabled();

CONTENT_EXPORT bool IsForceCompositingModeEnabled();

CONTENT_EXPORT bool IsDelegatedRendererEnabled();

CONTENT_EXPORT bool IsDeadlineSchedulingEnabled();

CONTENT_EXPORT base::Value* GetFeatureStatus();
CONTENT_EXPORT base::Value* GetProblems();
CONTENT_EXPORT base::Value* GetDriverBugWorkarounds();

}  

#endif  

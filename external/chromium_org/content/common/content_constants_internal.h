// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_CONTENT_CONSTANTS_INTERNAL_H_
#define CONTENT_COMMON_CONTENT_CONSTANTS_INTERNAL_H_

#include "base/basictypes.h"
#include "content/common/content_export.h"

namespace content {

CONTENT_EXPORT extern const int kHungRendererDelayMs;

extern const uint16 kMaxPluginSideLength;
extern const uint32 kMaxPluginSize;

CONTENT_EXPORT extern const int kTraceEventBrowserProcessSortIndex;
CONTENT_EXPORT extern const int kTraceEventRendererProcessSortIndex;
CONTENT_EXPORT extern const int kTraceEventPluginProcessSortIndex;
CONTENT_EXPORT extern const int kTraceEventPpapiProcessSortIndex;
CONTENT_EXPORT extern const int kTraceEventPpapiBrokerProcessSortIndex;
CONTENT_EXPORT extern const int kTraceEventGpuProcessSortIndex;

CONTENT_EXPORT extern const int kTraceEventRendererMainThreadSortIndex;

} 

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_PUBLIC_COMMON_CONTENT_CONSTANTS_H_
#define CONTENT_PUBLIC_COMMON_CONTENT_CONSTANTS_H_

#include <stddef.h>         

#include "base/files/file_path.h"
#include "content/common/content_export.h"

namespace content {

CONTENT_EXPORT extern const base::FilePath::CharType kAppCacheDirname[];
CONTENT_EXPORT extern const base::FilePath::CharType kPepperDataDirname[];

CONTENT_EXPORT extern const char kBrowserPluginMimeType[];

CONTENT_EXPORT extern const char kFlashPluginName[];
CONTENT_EXPORT extern const char kFlashPluginSwfMimeType[];
CONTENT_EXPORT extern const char kFlashPluginSwfExtension[];
CONTENT_EXPORT extern const char kFlashPluginSwfDescription[];
CONTENT_EXPORT extern const char kFlashPluginSplMimeType[];
CONTENT_EXPORT extern const char kFlashPluginSplExtension[];
CONTENT_EXPORT extern const char kFlashPluginSplDescription[];

CONTENT_EXPORT extern const size_t kMaxRendererProcessCount;

extern const int kMaxSessionHistoryEntries;

extern const size_t kMaxTitleChars;

CONTENT_EXPORT extern const size_t kMaxURLDisplayChars;

extern const char kStatsFilename[];
extern const int kStatsMaxThreads;
extern const int kStatsMaxCounters;

CONTENT_EXPORT extern const int kHistogramSynchronizerReservedSequenceNumber;

CONTENT_EXPORT extern const char kFlashHwVideoDecodeFieldTrialName[];
CONTENT_EXPORT extern const char kFlashHwVideoDecodeFieldTrialEnabledName[];

}  

#endif  

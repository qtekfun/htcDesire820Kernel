// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_STATS_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_STATS_H_

#include "content/public/browser/download_danger_type.h"

void RecordDownloadShelfClose(int size, int in_progress, bool autoclose);

enum ChromeDownloadCountTypes {
  
  
  CHROME_DOWNLOAD_COUNT_UNUSED_0 = 0,
  CHROME_DOWNLOAD_COUNT_UNUSED_1,
  CHROME_DOWNLOAD_COUNT_UNUSED_2,
  CHROME_DOWNLOAD_COUNT_UNUSED_3,

  
  
  CHROME_DOWNLOAD_COUNT_BLOCKED_BY_THROTTLING,

  CHROME_DOWNLOAD_COUNT_TYPES_LAST_ENTRY
};

enum ChromeDownloadSource {
  
  DOWNLOAD_INITIATED_BY_NAVIGATION = 0,

  
  DOWNLOAD_INITIATED_BY_CONTEXT_MENU,

  
  DOWNLOAD_INITIATED_BY_WEBSTORE_INSTALLER,

  
  DOWNLOAD_INITIATED_BY_IMAGE_BURNER,

  
  DOWNLOAD_INITIATED_BY_PLUGIN_INSTALLER,

  
  DOWNLOAD_INITIATED_BY_PDF_SAVE,

  
  DOWNLOAD_INITIATED_BY_EXTENSION,

  CHROME_DOWNLOAD_SOURCE_LAST_ENTRY,
};

enum ChromeDownloadOpenMethod {
  
  
  DOWNLOAD_OPEN_METHOD_DEFAULT_PLATFORM = 0,

  
  DOWNLOAD_OPEN_METHOD_DEFAULT_BROWSER,

  
  
  DOWNLOAD_OPEN_METHOD_USER_PLATFORM,

  DOWNLOAD_OPEN_METHOD_LAST_ENTRY
};

void RecordDownloadCount(ChromeDownloadCountTypes type);

void RecordDownloadSource(ChromeDownloadSource source);

void RecordDangerousDownloadWarningShown(
    content::DownloadDangerType danger_type);

void RecordOpenedDangerousConfirmDialog(
    content::DownloadDangerType danger_type);

void RecordDownloadOpenMethod(ChromeDownloadOpenMethod open_method);

#endif  

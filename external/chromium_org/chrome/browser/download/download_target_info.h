// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_TARGET_INFO_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_TARGET_INFO_H_

#include "base/files/file_path.h"
#include "content/public/browser/download_danger_type.h"
#include "content/public/browser/download_item.h"

struct DownloadTargetInfo {
  DownloadTargetInfo();
  ~DownloadTargetInfo();

  
  
  // existing file at this path should be overwritten.
  base::FilePath target_path;

  
  
  
  content::DownloadItem::TargetDisposition target_disposition;

  
  content::DownloadDangerType danger_type;

  // Suggested intermediate path. The downloaded bytes should be written to this
  
  
  
  base::FilePath intermediate_path;

  
  
  
  std::string mime_type;

  
  
  
  
  bool is_filetype_handled_securely;
};

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_PATH_RESERVATION_TRACKER_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_PATH_RESERVATION_TRACKER_H_

#include "base/callback_forward.h"

namespace base {
class FilePath;
}

namespace content {
class DownloadItem;
}

class DownloadPathReservationTracker {
 public:
  
  
  
  
  
  
  
  
  
  typedef base::Callback<void(const base::FilePath& target_path,
                              bool target_path_verified)> ReservedPathCallback;

  
  
  static const int kMaxUniqueFiles = 100;

  enum FilenameConflictAction {
    UNIQUIFY,
    OVERWRITE,
    PROMPT,
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static void GetReservedPath(
      content::DownloadItem* download_item,
      const base::FilePath& requested_target_path,
      const base::FilePath& default_download_path,
      bool create_directory,
      FilenameConflictAction conflict_action,
      const ReservedPathCallback& callback);

  
  
  static bool IsPathInUseForTesting(const base::FilePath& path);
};

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_METADATA_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_METADATA_H_

#include "base/basictypes.h"
#include "base/time/time.h"
#include "chrome/browser/chromeos/drive/file_errors.h"

namespace drive {

struct FileSystemMetadata {
  FileSystemMetadata();
  ~FileSystemMetadata();

  
  
  int64 largest_changestamp;

  
  bool refreshing;

  
  base::Time last_update_check_time;

  
  FileError last_update_check_error;
};

}  

#endif  

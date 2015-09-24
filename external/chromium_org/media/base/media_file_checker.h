// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_MEDIA_FILE_CHECKER_H_
#define MEDIA_BASE_MEDIA_FILE_CHECKER_H_

#include "base/basictypes.h"
#include "base/files/scoped_platform_file_closer.h"
#include "base/platform_file.h"
#include "media/base/media_export.h"

namespace base {
class TimeDelta;
}

namespace media {

class MEDIA_EXPORT MediaFileChecker {
 public:
  explicit MediaFileChecker(const base::PlatformFile& file);
  ~MediaFileChecker();

  
  
  
  bool Start(base::TimeDelta check_time);

 private:
  base::PlatformFile file_;
  base::ScopedPlatformFileCloser file_closer_;

  DISALLOW_COPY_AND_ASSIGN(MediaFileChecker);
};

}  

#endif  

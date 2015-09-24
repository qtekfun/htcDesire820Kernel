// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_EXTENSIONS_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_EXTENSIONS_H_

#include <string>

#include "base/files/file_path.h"

namespace download_util {

enum DownloadDangerLevel {
  NOT_DANGEROUS,
  ALLOW_ON_USER_GESTURE,
  DANGEROUS
};

DownloadDangerLevel GetFileDangerLevel(const base::FilePath& path);

bool IsExecutableMimeType(const std::string& mime_type);

}  

#endif  

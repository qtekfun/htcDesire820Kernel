// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_FILE_ICON_EXTRACTOR_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_FILE_ICON_EXTRACTOR_H_

#include <string>

#include "base/callback.h"
#include "base/files/file_path.h"
#include "chrome/browser/icon_loader.h"

class DownloadFileIconExtractor {
 public:
  
  
  
  typedef base::Callback<void(const std::string&)> IconURLCallback;

  virtual ~DownloadFileIconExtractor() {}

  
  
  virtual bool ExtractIconURLForPath(const base::FilePath& path,
                                     IconLoader::IconSize icon_size,
                                     IconURLCallback callback) = 0;
};

#endif  

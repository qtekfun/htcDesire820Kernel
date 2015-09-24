// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_EXTENSIONS_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_EXTENSIONS_H_
#pragma once

#include <string>

#include "base/file_path.h"

namespace download_util {

enum DownloadDangerLevel {
  NotDangerous,
  AllowOnUserGesture,
  Dangerous
};

DownloadDangerLevel GetFileDangerLevel(const FilePath& path);

DownloadDangerLevel GetFileExtensionDangerLevel(
    const FilePath::StringType& extension);

bool IsFileSafe(const FilePath& path);

bool IsExecutableMimeType(const std::string& mime_type);

}  

#endif  

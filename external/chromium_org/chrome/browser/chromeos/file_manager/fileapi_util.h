// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FILE_MANAGER_FILEAPI_UTIL_H_
#define CHROME_BROWSER_CHROMEOS_FILE_MANAGER_FILEAPI_UTIL_H_

#include <string>
#include "url/gurl.h"

class Profile;

namespace base {
class FilePath;
}

namespace content {
class RenderViewHost;
}

namespace fileapi {
class FileSystemContext;
}

namespace file_manager {
namespace util {

fileapi::FileSystemContext* GetFileSystemContextForExtensionId(
    Profile* profile,
    const std::string& extension_id);

fileapi::FileSystemContext* GetFileSystemContextForRenderViewHost(
    Profile* profile,
    content::RenderViewHost* render_view_host);

GURL ConvertRelativeFilePathToFileSystemUrl(const base::FilePath& relative_path,
                                            const std::string& extension_id);

bool ConvertAbsoluteFilePathToFileSystemUrl(
    Profile* profile,
    const base::FilePath& absolute_path,
    const std::string& extension_id,
    GURL* url);

bool ConvertAbsoluteFilePathToRelativeFileSystemPath(
    Profile* profile,
    const std::string& extension_id,
    const base::FilePath& absolute_path,
    base::FilePath* relative_path);

}  
}  

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_PEPPER_PEPPER_SECURITY_HELPER_H_
#define CONTENT_BROWSER_RENDERER_HOST_PEPPER_PEPPER_SECURITY_HELPER_H_

#include "base/files/file_path.h"
#include "content/common/content_export.h"
#include "webkit/browser/fileapi/file_system_url.h"

namespace content {

CONTENT_EXPORT bool CanOpenWithPepperFlags(int pp_open_flags,
                                           int child_id,
                                           const base::FilePath& file);

CONTENT_EXPORT bool CanOpenFileSystemURLWithPepperFlags(
    int pp_open_flags,
    int child_id,
    const fileapi::FileSystemURL& url);

}  

#endif  

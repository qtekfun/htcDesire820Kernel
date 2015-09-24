// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOWNLOAD_FILE_METADATA_LINUX_H_
#define CONTENT_BROWSER_DOWNLOAD_FILE_METADATA_LINUX_H_

#include "content/common/content_export.h"

class GURL;

namespace base {
class FilePath;
}

namespace content {

CONTENT_EXPORT extern const char kSourceURLAttrName[];
CONTENT_EXPORT extern const char kReferrerURLAttrName[];

CONTENT_EXPORT void AddOriginMetadataToFile(const base::FilePath& file,
                                            const GURL& source,
                                            const GURL& referrer);

}  

#endif  

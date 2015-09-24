// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_SAFE_BROWSING_DOWNLOAD_PROTECTION_UTIL_H_
#define CHROME_COMMON_SAFE_BROWSING_DOWNLOAD_PROTECTION_UTIL_H_

namespace base {
class FilePath;
}

namespace safe_browsing {
namespace download_protection_util {

bool IsBinaryFile(const base::FilePath& file);

bool IsArchiveFile(const base::FilePath& file);

}  
}  

#endif  

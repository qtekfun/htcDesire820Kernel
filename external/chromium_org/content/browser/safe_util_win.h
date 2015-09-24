// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_SAFE_UTIL_WIN_H_
#define CONTENT_COMMON_SAFE_UTIL_WIN_H_

#include <string>
#include <windows.h>

class GURL;

namespace base {
class FilePath;
}

namespace content {

HRESULT AVScanFile(const base::FilePath& full_path,
                   const std::string& source_url,
                   const GUID& client_guid);
}  

#endif  

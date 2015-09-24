// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_FILE_UTIL_H_
#define EXTENSIONS_COMMON_FILE_UTIL_H_

class GURL;

namespace base {
class FilePath;
}

namespace extensions {
namespace file_util {

base::FilePath ExtensionURLToRelativeFilePath(const GURL& url);

base::FilePath ExtensionResourceURLToFilePath(const GURL& url,
                                              const base::FilePath& root);

}  
}  

#endif  

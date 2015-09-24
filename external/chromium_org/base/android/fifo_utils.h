// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_ANDROID_FIFO_UTILS_H_
#define BASE_ANDROID_FIFO_UTILS_H_

#include <stdio.h>

#include "base/base_export.h"
#include "base/basictypes.h"

namespace base {

class FilePath;

namespace android {

BASE_EXPORT bool CreateFIFO(const FilePath& path, int mode);

BASE_EXPORT bool RedirectStream(FILE* stream,
                                const FilePath& path,
                                const char* mode);

}  
}  

#endif  

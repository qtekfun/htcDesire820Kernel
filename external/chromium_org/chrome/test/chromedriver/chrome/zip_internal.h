// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_CHROMEDRIVER_CHROME_ZIP_INTERNAL_H_
#define CHROME_TEST_CHROMEDRIVER_CHROME_ZIP_INTERNAL_H_

#if defined(OS_WIN)
#include <windows.h>
#endif

#include <string>

#if defined(USE_SYSTEM_MINIZIP)
#include <minizip/unzip.h>
#include <minizip/zip.h>
#else
#include "third_party/zlib/contrib/minizip/unzip.h"
#include "third_party/zlib/contrib/minizip/zip.h"
#endif

namespace zip {
namespace internal {

unzFile OpenForUnzipping(const std::string& file_name_utf8);

#if defined(OS_POSIX)
unzFile OpenFdForUnzipping(int zip_fd);
#endif

#if defined(OS_WIN)
unzFile OpenHandleForUnzipping(HANDLE zip_handle);
#endif

unzFile PreprareMemoryForUnzipping(const std::string& data);

zipFile OpenForZipping(const std::string& file_name_utf8, int append_flag);

#if defined(OS_POSIX)
zipFile OpenFdForZipping(int zip_fd, int append_flag);
#endif

const int kZipMaxPath = 256;
const int kZipBufSize = 8192;

}  
}  

#endif  

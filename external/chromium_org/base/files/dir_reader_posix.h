// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_FILES_DIR_READER_POSIX_H_
#define BASE_FILES_DIR_READER_POSIX_H_

#include "build/build_config.h"



#if defined(OS_LINUX)
#include "base/files/dir_reader_linux.h"
#else
#include "base/files/dir_reader_fallback.h"
#endif

namespace base {

#if defined(OS_LINUX)
typedef DirReaderLinux DirReaderPosix;
#else
typedef DirReaderFallback DirReaderPosix;
#endif

}  

#endif 

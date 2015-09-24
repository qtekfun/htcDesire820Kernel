// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_IPC_CONSTANTS_H_
#define REMOTING_HOST_IPC_CONSTANTS_H_

#include "base/files/file_path.h"

namespace remoting {

extern const char kDaemonPipeSwitchName[];

extern const base::FilePath::CharType kHostBinaryName[];

extern const base::FilePath::CharType kDesktopBinaryName[];

bool GetInstalledBinaryPath(const base::FilePath::StringType& binary,
                            base::FilePath* full_path);

}  

#endif  

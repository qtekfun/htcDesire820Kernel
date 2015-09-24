// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_LINUX_UTIL_H_
#define BASE_LINUX_UTIL_H_

#include <stdint.h>
#include <sys/types.h>

#include <string>

#include "base/base_export.h"

namespace base {

BASE_EXPORT extern const char kFindInodeSwitch[];

BASE_EXPORT extern char g_linux_distro[];

BASE_EXPORT std::string GetLinuxDistro();

BASE_EXPORT void SetLinuxDistro(const std::string& distro);

BASE_EXPORT bool FileDescriptorGetInode(ino_t* inode_out, int fd);

BASE_EXPORT bool FindProcessHoldingSocket(pid_t* pid_out, ino_t socket_inode);

BASE_EXPORT pid_t FindThreadIDWithSyscall(pid_t pid,
                                          const std::string& expected_data,
                                          bool* syscall_supported);

}  

#endif  

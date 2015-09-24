// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_LINUX_UTIL_H_
#define BASE_LINUX_UTIL_H_
#pragma once

#include <stdint.h>
#include <sys/types.h>

#include <string>

namespace base {

static const char kFindInodeSwitch[] = "--find-inode";

extern char g_linux_distro[];

std::string GetLinuxDistro();

void SetLinuxDistro(const std::string& distro);

bool FileDescriptorGetInode(ino_t* inode_out, int fd);

bool FindProcessHoldingSocket(pid_t* pid_out, ino_t socket_inode);

pid_t FindThreadIDWithSyscall(pid_t pid, const std::string& expected_data);

}  

#endif  

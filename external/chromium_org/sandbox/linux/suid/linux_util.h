// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef SANDBOX_LINUX_SUID_LINUX_UTIL_H_
#define SANDBOX_LINUX_SUID_LINUX_UTIL_H_

#include <stdbool.h>
#include <sys/types.h>

static const char kFindInodeSwitch[] = "--find-inode";
static const char kSuidSandboxGetApiSwitch[] = "--get-api";

bool FindProcessHoldingSocket(pid_t* pid_out, ino_t socket_inode);

#endif  

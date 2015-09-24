// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_SET_PROCESS_TITLE_LINUX_H_
#define CONTENT_COMMON_SET_PROCESS_TITLE_LINUX_H_

void setproctitle(const char* fmt, ...);

void setproctitle_init(const char** main_argv);

#endif  

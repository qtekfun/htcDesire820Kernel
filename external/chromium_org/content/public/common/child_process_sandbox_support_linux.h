// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PULIC_COMMON_CHILD_PROCESS_SANDBOX_SUPPORT_LINUX_H_
#define CONTENT_PULIC_COMMON_CHILD_PROCESS_SANDBOX_SUPPORT_LINUX_H_

#include <stdint.h>
#include <string>

#include "content/common/content_export.h"

namespace content {

CONTENT_EXPORT int MakeSharedMemorySegmentViaIPC(size_t length,
                                                 bool executable);

CONTENT_EXPORT int MatchFontWithFallback(const std::string& face, bool bold,
                                         bool italic, int charset);

CONTENT_EXPORT bool GetFontTable(int fd, uint32_t table_tag, off_t offset,
                                 uint8_t* output, size_t* output_length);

};  

#endif  

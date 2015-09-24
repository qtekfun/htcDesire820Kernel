// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_ANDROID_COMMON_NET_H_
#define TOOLS_ANDROID_COMMON_NET_H_

#include <string>

namespace tools {

int DisableNagle(int socket);

int DeferAccept(int socket);

std::string DumpBinary(const char* buffer, size_t length);

}  

#endif  


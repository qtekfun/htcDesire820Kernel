// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_ZAP_H_
#define NET_BASE_ZAP_H_

#include <string>
#include "base/strings/string16.h"

namespace net {

// time that people can access them once they are written to disk.

void ZapBuf(void* buf, size_t buf_len);

void ZapString(std::string* s);

void ZapString(base::string16* s);

}  

#endif  

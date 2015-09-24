// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_DRIVE_REQUEST_UTIL_H_
#define GOOGLE_APIS_DRIVE_REQUEST_UTIL_H_

#include <string>

namespace google_apis {
namespace util {

extern const char kIfMatchAllHeader[];

std::string GenerateIfMatchHeader(const std::string& etag);

}  
}  

#endif  

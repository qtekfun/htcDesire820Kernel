// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_DRIVE_TIME_UTIL_H_
#define GOOGLE_APIS_DRIVE_TIME_UTIL_H_

#include <string>

#include "base/strings/string_piece.h"

namespace base {
class Time;
}  

namespace google_apis {
namespace util {

bool GetTimeFromString(const base::StringPiece& raw_value, base::Time* time);

std::string FormatTimeAsString(const base::Time& time);

std::string FormatTimeAsStringLocaltime(const base::Time& time);

}  
}  

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_FTP_FTP_UTIL_H_
#define NET_FTP_FTP_UTIL_H_
#pragma once

#include <string>

#include "base/string16.h"

namespace base {
class Time;
}

namespace net {

class FtpUtil {
 public:
  
  static std::string UnixFilePathToVMS(const std::string& unix_path);

  
  static std::string UnixDirectoryPathToVMS(const std::string& unix_path);

  
  static std::string VMSPathToUnix(const std::string& vms_path);

  
  
  
  static bool AbbreviatedMonthToNumber(const string16& text, int* number);

  
  
  
  
  static bool LsDateListingToTime(const string16& month,
                                  const string16& day,
                                  const string16& rest,
                                  const base::Time& current_time,
                                  base::Time* result);

  
  
  static string16 GetStringPartAfterColumns(const string16& text, int columns);
};

}  

#endif  

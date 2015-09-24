// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_FTP_FTP_UTIL_H_
#define NET_FTP_FTP_UTIL_H_

#include <string>

#include "base/strings/string16.h"
#include "net/base/net_export.h"

namespace base {
class Time;
}

namespace net {

class NET_EXPORT_PRIVATE FtpUtil {
 public:
  
  static std::string UnixFilePathToVMS(const std::string& unix_path);

  
  static std::string UnixDirectoryPathToVMS(const std::string& unix_path);

  
  static std::string VMSPathToUnix(const std::string& vms_path);

  
  
  
  static bool AbbreviatedMonthToNumber(const base::string16& text, int* number);

  
  
  
  
  static bool LsDateListingToTime(const base::string16& month,
                                  const base::string16& day,
                                  const base::string16& rest,
                                  const base::Time& current_time,
                                  base::Time* result);

  
  static bool WindowsDateListingToTime(const base::string16& date,
                                       const base::string16& time,
                                       base::Time* result);

  
  
  static base::string16 GetStringPartAfterColumns(const base::string16& text,
                                                  int columns);
};

}  

#endif  

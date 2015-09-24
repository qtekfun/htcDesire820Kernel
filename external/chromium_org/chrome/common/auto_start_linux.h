// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_AUTO_START_LINUX_H_
#define CHROME_COMMON_AUTO_START_LINUX_H_

#include <string>

#include "base/basictypes.h"

class AutoStart {
 public:
  
  
  static bool AddApplication(const std::string& autostart_filename,
                             const std::string& application_name,
                             const std::string& command_line,
                             bool is_terminal_app);
  
  static bool Remove(const std::string& autostart_filename);
  
  static bool GetAutostartFileContents(const std::string& autostart_filename,
                                       std::string* contents);
  
  static bool GetAutostartFileValue(const std::string& autostart_filename,
                                    const std::string& value_name,
                                    std::string* value);
 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(AutoStart);
};

#endif  

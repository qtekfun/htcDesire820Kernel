// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_CHROME_RESTART_REQUEST_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_CHROME_RESTART_REQUEST_H_

#include <string>

class CommandLine;
class GURL;

namespace chromeos {

std::string GetOffTheRecordCommandLine(
    const GURL& start_url,
    const CommandLine& base_command_line,
    CommandLine* command_line);

void RestartChrome(const std::string& command_line);

}  

#endif  

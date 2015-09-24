// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_FIRST_RUN_UPGRADE_UTIL_H_
#define CHROME_BROWSER_FIRST_RUN_UPGRADE_UTIL_H_

#include "build/build_config.h"

#if defined(OS_WIN)
#include <string>
#endif

#if !defined(OS_CHROMEOS)

class CommandLine;

namespace upgrade_util {

bool RelaunchChromeBrowser(const CommandLine& command_line);

#if defined(OS_WIN)

extern const char kRelaunchModeMetro[];
extern const char kRelaunchModeDesktop[];
extern const char kRelaunchModeDefault[];

enum RelaunchMode {
  RELAUNCH_MODE_METRO = 0,
  RELAUNCH_MODE_DESKTOP = 1,
  
  RELAUNCH_MODE_DEFAULT = 2,
};

std::string RelaunchModeEnumToString(const RelaunchMode& relaunch_mode);

RelaunchMode RelaunchModeStringToEnum(const std::string& relaunch_mode);

bool RelaunchChromeWithMode(const CommandLine& command_line,
                            const RelaunchMode& relaunch_mode);

#endif

#if !defined(OS_MACOSX)

void SetNewCommandLine(CommandLine* new_command_line);

void RelaunchChromeBrowserWithNewCommandLineIfNeeded();

bool IsUpdatePendingRestart();

#endif  

}  

#endif  

#endif  

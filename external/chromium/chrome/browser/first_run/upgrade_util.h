// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_FIRST_RUN_UPGRADE_UTIL_H_
#define CHROME_BROWSER_FIRST_RUN_UPGRADE_UTIL_H_
#pragma once

class CommandLine;

namespace upgrade_util {

void SetNewCommandLine(CommandLine* new_command_line);

void RelaunchChromeBrowserWithNewCommandLineIfNeeded();

bool RelaunchChromeBrowser(const CommandLine& command_line);

bool IsUpdatePendingRestart();

}  

#endif  

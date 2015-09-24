// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_BROWSER_MAIN_WIN_H_
#define CHROME_BROWSER_BROWSER_MAIN_WIN_H_
#pragma once

class CommandLine;

int DoUninstallTasks(bool chrome_still_running);

void PrepareRestartOnCrashEnviroment(const CommandLine& parsed_command_line);

bool RegisterApplicationRestart(const CommandLine& parsed_command_line);

int HandleIconsCommands(const CommandLine& parsed_command_line);

bool CheckMachineLevelInstall();

#endif  

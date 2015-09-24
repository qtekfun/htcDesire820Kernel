// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_FIRST_RUN_UPGRADE_UTIL_WIN_H_
#define CHROME_BROWSER_FIRST_RUN_UPGRADE_UTIL_WIN_H_

class CommandLine;

namespace upgrade_util {

bool SwapNewChromeExeIfPresent();

bool DoUpgradeTasks(const CommandLine& command_line);

}  

#endif  

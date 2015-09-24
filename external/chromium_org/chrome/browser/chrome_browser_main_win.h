// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_CHROME_BROWSER_MAIN_WIN_H_
#define CHROME_BROWSER_CHROME_BROWSER_MAIN_WIN_H_

#include "chrome/browser/chrome_browser_main.h"

class CommandLine;

int DoUninstallTasks(bool chrome_still_running);

class ChromeBrowserMainPartsWin : public ChromeBrowserMainParts {
 public:
  explicit ChromeBrowserMainPartsWin(
      const content::MainFunctionParams& parameters);

  virtual ~ChromeBrowserMainPartsWin();

  
  virtual void ToolkitInitialized() OVERRIDE;
  virtual void PreMainMessageLoopStart() OVERRIDE;
  virtual int PreCreateThreads() OVERRIDE;

  
  virtual void ShowMissingLocaleMessageBox() OVERRIDE;
  virtual void PostBrowserStart() OVERRIDE;

  
  
  
  
  static void PrepareRestartOnCrashEnviroment(
      const CommandLine& parsed_command_line);

  
  
  static void RegisterApplicationRestart(
      const CommandLine& parsed_command_line);

  
  
  
  
  static int HandleIconsCommands(const CommandLine& parsed_command_line);

  
  
  
  
  static bool CheckMachineLevelInstall();

  
  
  static void SetupInstallerUtilStrings();

 private:
  DISALLOW_COPY_AND_ASSIGN(ChromeBrowserMainPartsWin);
};

#endif  

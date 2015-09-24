// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_LIFETIME_APPLICATION_LIFETIME_H_
#define CHROME_BROWSER_LIFETIME_APPLICATION_LIFETIME_H_

#include "base/compiler_specific.h"

class Browser;

namespace chrome {

void AttemptUserExit();

void StartShutdownTracing();

void AttemptRestart();

#if defined(OS_WIN)
enum AshExecutionStatus {
  ASH_KEEP_RUNNING,
  ASH_TERMINATE,
};

void ActivateDesktopHelper(AshExecutionStatus ash_execution_status);

void AttemptRestartWithModeSwitch();
void AttemptRestartToDesktopMode();
void AttemptRestartToMetroMode();
#endif

void AttemptExit();

#if defined(OS_CHROMEOS)
void ExitCleanly();
#endif

void CloseAllBrowsersAndQuit();

void CloseAllBrowsers();

void SessionEnding();

void StartKeepAlive();

void EndKeepAlive();

bool WillKeepAlive();

void NotifyAppTerminating();

void NotifyAndTerminate(bool fast_path);

void OnAppExiting();

void HandleAppExitingForPlatform();

bool ShouldStartShutdown(Browser* browser);

}  

#endif  

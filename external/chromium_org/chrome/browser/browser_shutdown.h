// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSER_SHUTDOWN_H__
#define CHROME_BROWSER_BROWSER_SHUTDOWN_H__

class PrefRegistrySimple;

namespace browser_shutdown {

enum ShutdownType {
  
  NOT_VALID = 0,
  
  WINDOW_CLOSE,
  
  BROWSER_EXIT,
  
  END_SESSION
};

void RegisterPrefs(PrefRegistrySimple* registry);

void OnShutdownStarting(ShutdownType type);

ShutdownType GetShutdownType();

bool ShutdownPreThreadsStop();

void ShutdownPostThreadsStop(bool restart_last_session);

void ReadLastShutdownInfo();


void SetTryingToQuit(bool quitting);

bool IsTryingToQuit();

bool ShuttingDownWithoutClosingBrowsers();

void SetShuttingDownWithoutClosingBrowsers(bool without_close);

}  

#endif  

// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSER_SHUTDOWN_H__
#define CHROME_BROWSER_BROWSER_SHUTDOWN_H__
#pragma once

class PrefService;

namespace browser_shutdown {

extern bool delete_resources_on_shutdown;

enum ShutdownType {
  
  NOT_VALID = 0,
  
  WINDOW_CLOSE,
  
  BROWSER_EXIT,
  
  END_SESSION
};

void RegisterPrefs(PrefService* local_state);

void OnShutdownStarting(ShutdownType type);

ShutdownType GetShutdownType();

void Shutdown();

void ReadLastShutdownInfo();


void SetTryingToQuit(bool quitting);

bool IsTryingToQuit();

bool ShuttingDownWithoutClosingBrowsers();

}  

#endif  

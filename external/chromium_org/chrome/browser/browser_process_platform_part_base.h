// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSER_PROCESS_PLATFORM_PART_BASE_H_
#define CHROME_BROWSER_BROWSER_PROCESS_PLATFORM_PART_BASE_H_

#include "base/basictypes.h"

class CommandLine;

class BrowserProcessPlatformPartBase {
 public:
  BrowserProcessPlatformPartBase();
  virtual ~BrowserProcessPlatformPartBase();

  
  
  virtual void PlatformSpecificCommandLineProcessing(
      const CommandLine& command_line);

  
  virtual void StartTearDown();

  
  virtual void AttemptExit();

  
  virtual void PreMainMessageLoopRun();

 private:
  DISALLOW_COPY_AND_ASSIGN(BrowserProcessPlatformPartBase);
};

#endif  

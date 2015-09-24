// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_AUTOMATION_CHROME_FRAME_AUTOMATION_PROVIDER_WIN_H_
#define CHROME_BROWSER_AUTOMATION_CHROME_FRAME_AUTOMATION_PROVIDER_WIN_H_

#include "base/basictypes.h"
#include "chrome/browser/automation/automation_provider.h"

class Profile;

class ChromeFrameAutomationProvider : public AutomationProvider {
 public:
  explicit ChromeFrameAutomationProvider(Profile* profile);
  virtual ~ChromeFrameAutomationProvider();

  
  virtual bool OnMessageReceived(const IPC::Message& message);

 protected:
  
  virtual void OnUnhandledMessage(const IPC::Message& message);

  
  bool IsValidMessage(uint32 type);

 private:
  DISALLOW_COPY_AND_ASSIGN(ChromeFrameAutomationProvider);
};

#endif  


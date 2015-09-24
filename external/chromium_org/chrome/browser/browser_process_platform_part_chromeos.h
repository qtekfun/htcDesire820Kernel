// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSER_PROCESS_PLATFORM_PART_CHROMEOS_H_
#define CHROME_BROWSER_BROWSER_PROCESS_PLATFORM_PART_CHROMEOS_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/browser_process_platform_part_base.h"

namespace chromeos {
class OomPriorityManager;
class ProfileHelper;
}

namespace chromeos {
namespace system {
class AutomaticRebootManager;
}
}

class BrowserProcessPlatformPart : public BrowserProcessPlatformPartBase,
                                   public base::NonThreadSafe {
 public:
  BrowserProcessPlatformPart();
  virtual ~BrowserProcessPlatformPart();

  void InitializeAutomaticRebootManager();
  void ShutdownAutomaticRebootManager();

  
  virtual chromeos::OomPriorityManager* oom_priority_manager();

  
  
  virtual chromeos::ProfileHelper* profile_helper();

  
  virtual void StartTearDown() OVERRIDE;

  chromeos::system::AutomaticRebootManager* automatic_reboot_manager() {
    return automatic_reboot_manager_.get();
  }

 protected:
  virtual void CreateProfileHelper();

  bool created_profile_helper_;
  scoped_ptr<chromeos::ProfileHelper> profile_helper_;

 private:
  scoped_ptr<chromeos::OomPriorityManager> oom_priority_manager_;

  scoped_ptr<chromeos::system::AutomaticRebootManager>
      automatic_reboot_manager_;

  DISALLOW_COPY_AND_ASSIGN(BrowserProcessPlatformPart);
};

#endif  

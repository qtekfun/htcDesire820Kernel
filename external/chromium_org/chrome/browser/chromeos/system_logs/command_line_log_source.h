// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_SYSTEM_LOGS_COMMAND_LINE_LOG_SOURCE_H_
#define CHROME_BROWSER_CHROMEOS_SYSTEM_LOGS_COMMAND_LINE_LOG_SOURCE_H_

#include "chrome/browser/chromeos/system_logs/system_logs_fetcher_base.h"

namespace chromeos {

class CommandLineLogSource : public SystemLogsSource {
 public:
  CommandLineLogSource() {}
  virtual ~CommandLineLogSource() {}

  
  virtual void Fetch(const SysLogsSourceCallback& callback) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(CommandLineLogSource);
};

}  

#endif  

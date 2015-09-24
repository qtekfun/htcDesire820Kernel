// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_SYSTEM_LOGS_SCRUBBED_SYSTEM_LOGS_FETCHER_H_
#define CHROME_BROWSER_CHROMEOS_SYSTEM_LOGS_SCRUBBED_SYSTEM_LOGS_FETCHER_H_

#include "chrome/browser/chromeos/system_logs/system_logs_fetcher_base.h"

namespace chromeos {

class ScrubbedSystemLogsFetcher : public SystemLogsFetcherBase {
 public:
  ScrubbedSystemLogsFetcher();
  ~ScrubbedSystemLogsFetcher();

 private:

  DISALLOW_COPY_AND_ASSIGN(ScrubbedSystemLogsFetcher);
};

}  

#endif  


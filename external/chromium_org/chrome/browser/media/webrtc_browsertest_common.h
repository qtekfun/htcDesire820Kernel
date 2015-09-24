// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_WEBRTC_BROWSERTEST_COMMON_H_
#define CHROME_BROWSER_MEDIA_WEBRTC_BROWSERTEST_COMMON_H_

#include <string>

#include "base/process/process_handle.h"

namespace content {
class WebContents;
}

bool SleepInJavascript(content::WebContents* tab_contents, int timeout_msec);

bool PollingWaitUntil(const std::string& javascript,
                      const std::string& evaluates_to,
                      content::WebContents* tab_contents);
bool PollingWaitUntil(const std::string& javascript,
                      const std::string& evaluates_to,
                      content::WebContents* tab_contents,
                      int poll_interval_msec);

class PeerConnectionServerRunner {
 public:
  static const char kDefaultPort[];

  PeerConnectionServerRunner(): server_pid_(0) {}
  ~PeerConnectionServerRunner() {}

  
  bool Start();

  
  bool Stop();

  static void KillAllPeerConnectionServersOnCurrentSystem();

 private:
  base::ProcessHandle server_pid_;
};

#endif  

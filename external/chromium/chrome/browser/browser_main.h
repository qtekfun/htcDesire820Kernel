// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSER_MAIN_H_
#define CHROME_BROWSER_BROWSER_MAIN_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/metrics/field_trial.h"
#include "base/tracked_objects.h"

class BrowserThread;
class CommandLine;
class HighResolutionTimerManager;
struct MainFunctionParams;
class MessageLoop;
class MetricsService;

namespace net {
class NetworkChangeNotifier;
}

namespace ui {
class SystemMonitor;
}

class BrowserMainParts {
 public:
  
  
  static BrowserMainParts* CreateBrowserMainParts(
      const MainFunctionParams& parameters);

  virtual ~BrowserMainParts();

  
  void EarlyInitialization();
  void MainMessageLoopStart();

  void SetupFieldTrials();

 protected:
  explicit BrowserMainParts(const MainFunctionParams& parameters);

  
  const MainFunctionParams& parameters() const {
    return parameters_;
  }
  const CommandLine& parsed_command_line() const {
    return parsed_command_line_;
  }
  MessageLoop& main_message_loop() const {
    return *main_message_loop_;
  }

  
  
  virtual void PreEarlyInitialization() {}
  virtual void PostEarlyInitialization() {}
  virtual void PreMainMessageLoopStart() {}
  virtual void PostMainMessageLoopStart() {}

 private:
  

  
  void ConnectionFieldTrial();

  
  void SocketTimeoutFieldTrial();

  
  void ProxyConnectionsFieldTrial();

  
  void SpdyFieldTrial();

  
  
  void ConnectBackupJobsFieldTrial();

  
  void SSLFalseStartFieldTrial();

  
  virtual void InitializeSSL() = 0;

  

  void InitializeMainThread();

  

  const MainFunctionParams& parameters_;
  const CommandLine& parsed_command_line_;

#if defined(TRACK_ALL_TASK_OBJECTS)
  
  
  
  tracked_objects::AutoTracking tracking_objects_;
#endif

  
  base::FieldTrialList field_trial_;

  
  scoped_ptr<MessageLoop> main_message_loop_;
  scoped_ptr<ui::SystemMonitor> system_monitor_;
  scoped_ptr<HighResolutionTimerManager> hi_res_timer_manager_;
  scoped_ptr<net::NetworkChangeNotifier> network_change_notifier_;
  scoped_ptr<BrowserThread> main_thread_;

  DISALLOW_COPY_AND_ASSIGN(BrowserMainParts);
};


void DidEndMainMessageLoop();

void RecordBreakpadStatusUMA(MetricsService* metrics);

void WarnAboutMinimumSystemRequirements();

void RecordBrowserStartupTime();

#endif  

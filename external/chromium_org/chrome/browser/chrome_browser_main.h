// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROME_BROWSER_MAIN_H_
#define CHROME_BROWSER_CHROME_BROWSER_MAIN_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/metrics/field_trial.h"
#include "base/tracked_objects.h"
#include "chrome/browser/chrome_browser_field_trials.h"
#include "chrome/browser/chrome_process_singleton.h"
#include "chrome/browser/first_run/first_run.h"
#include "chrome/browser/process_singleton.h"
#include "chrome/browser/task_profiler/auto_tracking.h"
#include "chrome/browser/ui/startup/startup_browser_creator.h"
#include "content/public/browser/browser_main_parts.h"
#include "content/public/common/main_function_params.h"

class ActiveTabTracker;
class BrowserProcessImpl;
class ChromeBrowserMainExtraParts;
class FieldTrialSynchronizer;
class MetricsService;
class PrefService;
class Profile;
class StartupBrowserCreator;
class StartupTimeBomb;
class ShutdownWatcherHelper;
class ThreeDAPIObserver;
class TranslateManager;

namespace chrome_browser {
extern const char kMissingLocaleDataTitle[];
extern const char kMissingLocaleDataMessage[];
}

namespace chrome_browser_metrics {
class TrackingSynchronizer;
}

namespace performance_monitor {
class StartupTimer;
}

class ChromeBrowserMainParts : public content::BrowserMainParts {
 public:
  virtual ~ChromeBrowserMainParts();

  
  virtual void AddParts(ChromeBrowserMainExtraParts* parts);

 protected:
  explicit ChromeBrowserMainParts(
      const content::MainFunctionParams& parameters);

  
  
  
  
  virtual void PreEarlyInitialization() OVERRIDE;
  virtual void PostEarlyInitialization() OVERRIDE;
  virtual void ToolkitInitialized() OVERRIDE;
  virtual void PreMainMessageLoopStart() OVERRIDE;
  virtual void PostMainMessageLoopStart() OVERRIDE;
  virtual int PreCreateThreads() OVERRIDE;
  virtual void PreMainMessageLoopRun() OVERRIDE;
  virtual bool MainMessageLoopRun(int* result_code) OVERRIDE;
  virtual void PostMainMessageLoopRun() OVERRIDE;
  virtual void PostDestroyThreads() OVERRIDE;

  
  
  virtual void PreProfileInit();
  virtual void PostProfileInit();
  virtual void PreBrowserStart();
  virtual void PostBrowserStart();

  
  virtual void ShowMissingLocaleMessageBox() = 0;

  const content::MainFunctionParams& parameters() const {
    return parameters_;
  }
  const CommandLine& parsed_command_line() const {
    return parsed_command_line_;
  }

  Profile* profile() { return profile_; }

  const PrefService* local_state() const { return local_state_; }

 private:
  

  
  
  
  void SetupMetricsAndFieldTrials();

  
  
  void StartMetricsRecording();

  
  bool IsMetricsReportingEnabled();

  
  void RecordBrowserStartupTime();

  
  
  
  void RecordPreReadExperimentTime(const char* name, base::TimeDelta time);

  

  int PreCreateThreadsImpl();
  int PreMainMessageLoopRunImpl();

  

  const content::MainFunctionParams parameters_;
  const CommandLine& parsed_command_line_;
  int result_code_;

  
  scoped_ptr<StartupTimeBomb> startup_watcher_;

  
  
  
  scoped_ptr<ShutdownWatcherHelper> shutdown_watcher_;

  
  
  
  scoped_ptr<performance_monitor::StartupTimer> startup_timer_;

  
  
  
  task_profiler::AutoTracking tracking_objects_;

  
  
  scoped_ptr<base::FieldTrialList> field_trial_list_;

  ChromeBrowserFieldTrials browser_field_trials_;

  
  
  std::vector<ChromeBrowserMainExtraParts*> chrome_extra_parts_;

  

  scoped_ptr<BrowserProcessImpl> browser_process_;
  scoped_refptr<chrome_browser_metrics::TrackingSynchronizer>
      tracking_synchronizer_;
#if !defined(OS_ANDROID)
  
  scoped_ptr<StartupBrowserCreator> browser_creator_;

  
  
  scoped_ptr<ChromeProcessSingleton> process_singleton_;

  
  scoped_ptr<first_run::MasterPrefs> master_prefs_;
#endif
  TranslateManager* translate_manager_;
  Profile* profile_;
  bool run_message_loop_;
  ProcessSingleton::NotifyResult notify_result_;
  scoped_ptr<ThreeDAPIObserver> three_d_observer_;

  
  scoped_refptr<FieldTrialSynchronizer> field_trial_synchronizer_;

  
  
  PrefService* local_state_;
  base::FilePath user_data_dir_;

#if !defined(OS_ANDROID)
  scoped_ptr<ActiveTabTracker> active_tab_tracker_;
#endif

  
  bool restart_last_session_;

  
  
  static bool disable_enforcing_cookie_policies_for_tests_;

  DISALLOW_COPY_AND_ASSIGN(ChromeBrowserMainParts);
};

#endif  

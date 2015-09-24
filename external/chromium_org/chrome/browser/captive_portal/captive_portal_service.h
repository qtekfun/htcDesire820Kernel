// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CAPTIVE_PORTAL_CAPTIVE_PORTAL_SERVICE_H_
#define CHROME_BROWSER_CAPTIVE_PORTAL_CAPTIVE_PORTAL_SERVICE_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_member.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "chrome/browser/captive_portal/captive_portal_detector.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "net/base/backoff_entry.h"
#include "url/gurl.h"

class Profile;

namespace captive_portal {

class CaptivePortalService : public BrowserContextKeyedService,
                             public base::NonThreadSafe {
 public:
  enum TestingState {
    NOT_TESTING,
    DISABLED_FOR_TESTING,  
    SKIP_OS_CHECK_FOR_TESTING  
                               
  };

  
  struct Results {
    
    Result previous_result;
    
    Result result;
  };

  explicit CaptivePortalService(Profile* profile);
  virtual ~CaptivePortalService();

  
  
  
  void DetectCaptivePortal();

  
  
  const GURL& test_url() const { return test_url_; }

  
  Result last_detection_result() const { return last_detection_result_; }

  
  
  bool enabled() const { return enabled_; }

  
  
  static void set_state_for_testing(TestingState testing_state) {
    testing_state_ = testing_state;
  }
  static TestingState get_state_for_testing() { return testing_state_; }

 private:
  friend class CaptivePortalServiceTest;
  friend class CaptivePortalBrowserTest;

  
  
  class RecheckBackoffEntry;

  enum State {
    
    STATE_IDLE,
    
    STATE_TIMER_RUNNING,
    
    STATE_CHECKING_FOR_PORTAL,
  };

  
  
  struct RecheckPolicy {
    
    RecheckPolicy();

    
    
    int initial_backoff_no_portal_ms;

    
    
    
    int initial_backoff_portal_ms;

    net::BackoffEntry::Policy backoff_policy;
  };

  
  
  void DetectCaptivePortalInternal();

  
  void OnPortalDetectionCompleted(
      const CaptivePortalDetector::Results& results);

  
  virtual void Shutdown() OVERRIDE;

  
  
  void OnResult(Result result);

  
  
  void ResetBackoffEntry(Result result);

  
  
  
  
  
  
  void UpdateEnabledState();

  
  base::TimeTicks GetCurrentTimeTicks() const;

  bool DetectionInProgress() const;

  
  bool TimerRunning() const;

  State state() const { return state_; }

  RecheckPolicy& recheck_policy() { return recheck_policy_; }

  void set_test_url(const GURL& test_url) { test_url_ = test_url; }

  
  void set_time_ticks_for_testing(const base::TimeTicks& time_ticks) {
    time_ticks_for_testing_ = time_ticks;
  }

  
  void advance_time_ticks_for_testing(const base::TimeDelta& delta) {
    time_ticks_for_testing_ += delta;
  }

  
  Profile* profile_;

  State state_;

  
  CaptivePortalDetector captive_portal_detector_;

  
  
  bool enabled_;

  
  Result last_detection_result_;

  
  int num_checks_with_same_result_;

  
  base::TimeTicks first_check_time_with_same_result_;

  
  base::TimeTicks last_check_time_;

  
  RecheckPolicy recheck_policy_;

  
  
  
  
  scoped_ptr<net::BackoffEntry> backoff_entry_;

  
  GURL test_url_;

  
  
  
  BooleanPrefMember resolve_errors_with_web_service_;

  base::OneShotTimer<CaptivePortalService> check_captive_portal_timer_;

  static TestingState testing_state_;

  
  base::TimeTicks time_ticks_for_testing_;

  DISALLOW_COPY_AND_ASSIGN(CaptivePortalService);
};

}  

#endif  

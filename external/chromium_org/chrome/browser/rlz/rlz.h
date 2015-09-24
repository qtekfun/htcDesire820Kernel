// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_RLZ_RLZ_H_
#define CHROME_BROWSER_RLZ_RLZ_H_

#include "build/build_config.h"

#if defined(ENABLE_RLZ)

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/singleton.h"
#include "base/strings/string16.h"
#include "base/threading/sequenced_worker_pool.h"
#include "base/time/time.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "rlz/lib/rlz_lib.h"

class Profile;
namespace net {
class URLRequestContextGetter;
}


class RLZTracker : public content::NotificationObserver {
 public:
  
  
  
  
  
  
  
  
  
  static bool InitRlzFromProfileDelayed(Profile* profile,
                                        bool first_run,
                                        bool send_ping_immediately,
                                        base::TimeDelta delay);

  
  
  
  static bool RecordProductEvent(rlz_lib::Product product,
                                 rlz_lib::AccessPoint point,
                                 rlz_lib::Event event_id);

  
  static const rlz_lib::AccessPoint CHROME_OMNIBOX;
  static const rlz_lib::AccessPoint CHROME_HOME_PAGE;

  
  
  
  
  
  static std::string GetAccessPointHttpHeader(rlz_lib::AccessPoint point);

  
  
  
  static bool GetAccessPointRlz(rlz_lib::AccessPoint point,
                                base::string16* rlz);

  
  static void CleanupRlz();

#if defined(OS_CHROMEOS)
  
  
  static void ClearRlzState();
#endif

  
  static RLZTracker* GetInstance();

  
  static void EnableZeroDelayForTesting();

  
  
 protected:
  RLZTracker();
  virtual ~RLZTracker();

  
  static bool InitRlzDelayed(bool first_run,
                             bool send_ping_immediately,
                             base::TimeDelta delay,
                             bool is_google_default_search,
                             bool is_google_homepage,
                             bool is_google_in_startpages);

  
  
  virtual void DelayedInit();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  void set_tracker(RLZTracker* tracker) {
    tracker_ = tracker;
  }

  
  
  
  void PingNowImpl();

 private:
  friend struct DefaultSingletonTraits<RLZTracker>;
  friend class base::RefCountedThreadSafe<RLZTracker>;

  
  bool Init(bool first_run,
            bool send_ping_immediately,
            base::TimeDelta delay,
            bool google_default_search,
            bool google_default_homepage,
            bool is_google_in_startpages);

  
  bool RecordProductEventImpl(rlz_lib::Product product,
                              rlz_lib::AccessPoint point,
                              rlz_lib::Event event_id);

  
  void RecordFirstSearch(rlz_lib::AccessPoint point);

  
  bool GetAccessPointRlzImpl(rlz_lib::AccessPoint point, base::string16* rlz);

  
  
  virtual void ScheduleDelayedInit(base::TimeDelta delay);

  
  
  virtual bool ScheduleRecordProductEvent(rlz_lib::Product product,
                                          rlz_lib::AccessPoint point,
                                          rlz_lib::Event event_id);

  
  
  virtual bool ScheduleRecordFirstSearch(rlz_lib::AccessPoint point);

  
  
  virtual void ScheduleFinancialPing();

  
  
  
  
  virtual bool ScheduleGetAccessPointRlz(rlz_lib::AccessPoint point);

  
  
  virtual bool SendFinancialPing(const std::string& brand,
                                 const base::string16& lang,
                                 const base::string16& referral);

#if defined(OS_CHROMEOS)
  
  void ClearRlzStateImpl();

  
  
  virtual bool ScheduleClearRlzState();
#endif

  
  
  static RLZTracker* tracker_;

  
  bool first_run_;
  bool send_ping_immediately_;
  bool is_google_default_search_;
  bool is_google_homepage_;
  bool is_google_in_startpages_;

  
  
  base::SequencedWorkerPool::SequenceToken worker_pool_token_;

  
  
  bool already_ran_;

  
  
  
  base::Lock cache_lock_;
  std::map<rlz_lib::AccessPoint, base::string16> rlz_cache_;

  
  bool omnibox_used_;
  bool homepage_used_;

  
  std::string brand_;
  std::string reactivation_brand_;

  content::NotificationRegistrar registrar_;

  
  base::TimeDelta min_init_delay_;

  DISALLOW_COPY_AND_ASSIGN(RLZTracker);
};

#endif  

#endif  

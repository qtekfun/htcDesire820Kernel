// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_BOOT_TIMES_LOADER_H_
#define CHROME_BROWSER_CHROMEOS_BOOT_TIMES_LOADER_H_
#pragma once

#include <string>

#include "base/atomic_sequence_num.h"
#include "base/callback.h"
#include "base/time.h"
#include "content/browser/cancelable_request.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

namespace chromeos {

class BootTimesLoader
    : public CancelableRequestProvider,
      public NotificationObserver {
 public:
  BootTimesLoader();

  
  typedef struct BootTimes {
    double firmware;           
    double pre_startup;        
    double x_started;          
    double chrome_exec;        
    double chrome_main;        
    double login_prompt_ready; 
    double system;             
    double chrome;             
    double total;              

    BootTimes() : firmware(0),
                  pre_startup(0),
                  x_started(0),
                  chrome_exec(0),
                  chrome_main(0),
                  login_prompt_ready(0),
                  system(0),
                  chrome(0),
                  total(0) {}
  } BootTimes;

  
  typedef Callback2<Handle, BootTimes>::Type GetBootTimesCallback;

  typedef CancelableRequest<GetBootTimesCallback> GetBootTimesRequest;

  static BootTimesLoader* Get();

  
  Handle GetBootTimes(
      CancelableRequestConsumerBase* consumer,
      GetBootTimesCallback* callback);

  
  
  
  
  void AddLoginTimeMarker(const std::string& marker_name, bool send_to_uma);

  
  
  
  
  void AddLogoutTimeMarker(const std::string& marker_name, bool send_to_uma);

  
  
  
  // Existing stats files will not be overwritten.
  void RecordCurrentStats(const std::string& name);

  
  
  void SaveChromeMainStats();

  
  // file thread. Existing stats files will not be overwritten.
  void RecordChromeMainStats();

  
  
  void RecordLoginAttempted();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  
  void WriteLogoutTimes();

 private:
  
  
  class Backend : public base::RefCountedThreadSafe<Backend> {
   public:
    Backend() {}

    void GetBootTimes(scoped_refptr<GetBootTimesRequest> request);

   private:
    friend class base::RefCountedThreadSafe<Backend>;

    ~Backend() {}

    DISALLOW_COPY_AND_ASSIGN(Backend);
  };

  class TimeMarker {
   public:
    TimeMarker(const std::string& name, bool send_to_uma)
        : name_(name),
          time_(base::Time::NowFromSystemTime()),
          send_to_uma_(send_to_uma) {}
    std::string name() const { return name_; }
    base::Time time() const { return time_; }
    bool send_to_uma() const { return send_to_uma_; }

   private:
    friend class std::vector<TimeMarker>;
    std::string name_;
    base::Time time_;
    bool send_to_uma_;
  };

  struct Stats {
   public:
    std::string uptime;
    std::string disk;
  };

  static void RecordStats(
      const std::string& name, const Stats& stats);
  static Stats GetCurrentStats();
  static void WriteTimes(const std::string base_name,
                         const std::string uma_name,
                         const std::string uma_prefix,
                         const std::vector<TimeMarker> login_times);

  
  Stats chrome_main_stats_;
  scoped_refptr<Backend> backend_;

  
  NotificationRegistrar registrar_;
  base::AtomicSequenceNumber num_tabs_;
  bool have_registered_;

  std::vector<TimeMarker> login_time_markers_;
  std::vector<TimeMarker> logout_time_markers_;

  DISALLOW_COPY_AND_ASSIGN(BootTimesLoader);
};

}  

#endif  

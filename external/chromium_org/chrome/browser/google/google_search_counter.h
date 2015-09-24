// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_GOOGLE_GOOGLE_SEARCH_COUNTER_H_
#define CHROME_BROWSER_GOOGLE_GOOGLE_SEARCH_COUNTER_H_

#include "base/memory/singleton.h"
#include "chrome/browser/google/google_search_metrics.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class GoogleSearchCounter : content::NotificationObserver {
 public:
  
  static void RegisterForNotifications();

  
  static GoogleSearchCounter* GetInstance();

 private:
  friend struct DefaultSingletonTraits<GoogleSearchCounter>;
  friend class GoogleSearchCounterTest;

  GoogleSearchCounter();
  virtual ~GoogleSearchCounter();

  void ProcessCommittedEntry(const content::NotificationSource& source,
                             const content::NotificationDetails& details);

  
  
  void SetSearchMetricsForTesting(GoogleSearchMetrics* search_metrics);

  
  void RegisterForNotificationsInternal();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  content::NotificationRegistrar registrar_;
  scoped_ptr<GoogleSearchMetrics> search_metrics_;

  DISALLOW_COPY_AND_ASSIGN(GoogleSearchCounter);
};

#endif  

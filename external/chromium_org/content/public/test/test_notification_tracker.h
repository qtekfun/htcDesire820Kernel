// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_TEST_TEST_NOTIFICATION_TRACKER_H_
#define CONTENT_PUBLIC_TEST_TEST_NOTIFICATION_TRACKER_H_

#include <vector>

#include "base/compiler_specific.h"
#include "content/public/browser/notification_details.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/notification_source.h"

namespace content {

class TestNotificationTracker : public NotificationObserver {
 public:
  
  struct Event {
    Event();
    Event(int t, NotificationSource s, NotificationDetails d);

    int type;
    NotificationSource source;
    NotificationDetails details;
  };

  
  
  TestNotificationTracker();

  virtual ~TestNotificationTracker();

  
  void ListenFor(int type, const NotificationSource& source);

  
  
  void ListenForAll(int type);

  
  void Reset();

  
  
  
  
  
  
  
  
  
  bool Check1AndReset(int type);
  bool Check2AndReset(int type1,
                      int type2);
  bool Check3AndReset(int type1,
                      int type2,
                      int type3);

  
  size_t size() const { return events_.size(); }

  
  
  const Event& at(size_t i) const { return events_[i]; }

 protected:
  virtual void Observe(int type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;
 private:
  NotificationRegistrar registrar_;

  
  std::vector<Event> events_;

  DISALLOW_COPY_AND_ASSIGN(TestNotificationTracker);
};

}  

#endif  

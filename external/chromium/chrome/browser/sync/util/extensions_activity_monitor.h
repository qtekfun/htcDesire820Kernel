// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_UTIL_EXTENSIONS_ACTIVITY_MONITOR_H_
#define CHROME_BROWSER_SYNC_UTIL_EXTENSIONS_ACTIVITY_MONITOR_H_
#pragma once

#include <map>

#include "base/message_loop.h"
#include "base/synchronization/lock.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

namespace browser_sync {

class ExtensionsActivityMonitor : public NotificationObserver {
 public:
  
  struct Record {
    Record() : bookmark_write_count(0U) {}

    
    
    std::string extension_id;

    
    
    uint32 bookmark_write_count;
  };

  typedef std::map<std::string, Record> Records;

  
  
  ExtensionsActivityMonitor();
  ~ExtensionsActivityMonitor();

  
  
  void GetAndClearRecords(Records* buffer);

  
  
  void PutRecords(const Records& records);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);
 private:
  Records records_;
  mutable base::Lock records_lock_;

  
  NotificationRegistrar registrar_;
};

}  

#endif  

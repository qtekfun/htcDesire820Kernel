// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UPGRADE_DETECTOR_H_
#define CHROME_BROWSER_UPGRADE_DETECTOR_H_
#pragma once

#include "base/timer.h"

template <typename T> struct DefaultSingletonTraits;
class PrefService;

class UpgradeDetector {
 public:
  
  enum UpgradeNotificationAnnoyanceLevel {
    UPGRADE_ANNOYANCE_NONE = 0,  
    UPGRADE_ANNOYANCE_LOW,       
    UPGRADE_ANNOYANCE_ELEVATED,  
    UPGRADE_ANNOYANCE_HIGH,      
    UPGRADE_ANNOYANCE_SEVERE,    
  };

  
  static UpgradeDetector* GetInstance();

  ~UpgradeDetector();

  static void RegisterPrefs(PrefService* prefs);

  bool notify_upgrade() { return notify_upgrade_; }

  UpgradeNotificationAnnoyanceLevel upgrade_notification_stage() const {
    return upgrade_notification_stage_;
  }

 private:
  friend struct DefaultSingletonTraits<UpgradeDetector>;

  UpgradeDetector();

  
  void CheckForUpgrade();

  
  
  void UpgradeDetected();

  
  
  
  void NotifyOnUpgrade();

  
  base::RepeatingTimer<UpgradeDetector> detect_upgrade_timer_;

  
  
  base::RepeatingTimer<UpgradeDetector> upgrade_notification_timer_;

  
  
  
  ScopedRunnableMethodFactory<UpgradeDetector> method_factory_;

  
  base::Time upgrade_detected_time_;

  
  bool is_dev_channel_;

  
  UpgradeNotificationAnnoyanceLevel upgrade_notification_stage_;

  
  
  bool notify_upgrade_;

  DISALLOW_COPY_AND_ASSIGN(UpgradeDetector);
};

#endif  

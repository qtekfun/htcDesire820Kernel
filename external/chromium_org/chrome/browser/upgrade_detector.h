// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UPGRADE_DETECTOR_H_
#define CHROME_BROWSER_UPGRADE_DETECTOR_H_

#include "base/timer/timer.h"
#include "chrome/browser/idle.h"
#include "ui/gfx/image/image.h"

class PrefRegistrySimple;

class UpgradeDetector {
 public:
  
  enum UpgradeNotificationAnnoyanceLevel {
    UPGRADE_ANNOYANCE_NONE = 0,  
    UPGRADE_ANNOYANCE_LOW,       
    UPGRADE_ANNOYANCE_ELEVATED,  
    UPGRADE_ANNOYANCE_HIGH,      
    UPGRADE_ANNOYANCE_SEVERE,    
    UPGRADE_ANNOYANCE_CRITICAL,  
  };

  
  enum UpgradeNotificationIconType {
    UPGRADE_ICON_TYPE_BADGE = 0,  
    UPGRADE_ICON_TYPE_MENU_ICON,  
  };

  
  static UpgradeDetector* GetInstance();

  virtual ~UpgradeDetector();

  static void RegisterPrefs(PrefRegistrySimple* registry);

  
  bool notify_upgrade() const { return notify_upgrade_; }

  
  bool is_outdated_install() const {
    return upgrade_available_ == UPGRADE_NEEDED_OUTDATED_INSTALL;
  }

  
  
  void acknowledge_critical_update() {
    critical_update_acknowledged_ = true;
  }

  
  bool critical_update_acknowledged() const {
    return critical_update_acknowledged_;
  }

  
  const base::Time& upgrade_detected_time() const {
    return upgrade_detected_time_;
  }

  
  
  
  
  
  int GetIconResourceID(UpgradeNotificationIconType type);

  UpgradeNotificationAnnoyanceLevel upgrade_notification_stage() const {
    return upgrade_notification_stage_;
  }

 protected:
  UpgradeDetector();

  
  void NotifyUpgradeDetected();

  
  void NotifyUpgradeRecommended();

  void set_upgrade_notification_stage(UpgradeNotificationAnnoyanceLevel stage) {
    upgrade_notification_stage_ = stage;
  }

  enum UpgradeAvailable {
    
    UPGRADE_AVAILABLE_NONE,
    
    UPGRADE_AVAILABLE_REGULAR,
    
    
    UPGRADE_AVAILABLE_CRITICAL,
    
    
    UPGRADE_NEEDED_OUTDATED_INSTALL,
  } upgrade_available_;

  
  bool critical_update_acknowledged_;

 private:
  
  void CheckIdle();

  
  
  void IdleCallback(IdleState state);

  
  base::Time upgrade_detected_time_;

  
  
  
  base::RepeatingTimer<UpgradeDetector> idle_check_timer_;

  
  UpgradeNotificationAnnoyanceLevel upgrade_notification_stage_;

  
  
  bool notify_upgrade_;

  DISALLOW_COPY_AND_ASSIGN(UpgradeDetector);
};

#endif  

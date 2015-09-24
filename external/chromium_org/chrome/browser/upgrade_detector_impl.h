// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UPGRADE_DETECTOR_IMPL_H_
#define CHROME_BROWSER_UPGRADE_DETECTOR_IMPL_H_

#include "base/memory/weak_ptr.h"
#include "base/timer/timer.h"
#include "chrome/browser/network_time/network_time_tracker.h"
#include "chrome/browser/upgrade_detector.h"

template <typename T> struct DefaultSingletonTraits;

class UpgradeDetectorImpl : public UpgradeDetector {
 public:
  virtual ~UpgradeDetectorImpl();

  
  static UpgradeDetectorImpl* GetInstance();

 private:
  friend struct DefaultSingletonTraits<UpgradeDetectorImpl>;

  UpgradeDetectorImpl();

  
  void StartTimerForUpgradeCheck();

  
  void CheckForUpgrade();

  
  
  void UpgradeDetected(UpgradeAvailable upgrade_available);

  
  bool DetectOutdatedInstall();

  
  
  
  void NotifyOnUpgrade();

  
  
  
  
  
  
  static void DetectUpgradeTask(
      base::WeakPtr<UpgradeDetectorImpl> upgrade_detector);

  
  base::RepeatingTimer<UpgradeDetectorImpl> detect_upgrade_timer_;

  
  
  base::RepeatingTimer<UpgradeDetectorImpl> upgrade_notification_timer_;

  
  
  
  base::WeakPtrFactory<UpgradeDetectorImpl> weak_factory_;

  
  bool is_unstable_channel_;

  
  base::Time build_date_;

  
  NetworkTimeTracker network_time_tracker_;

  DISALLOW_COPY_AND_ASSIGN(UpgradeDetectorImpl);
};


#endif  

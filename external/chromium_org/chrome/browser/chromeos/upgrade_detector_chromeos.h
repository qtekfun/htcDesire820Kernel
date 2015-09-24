// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_UPGRADE_DETECTOR_CHROMEOS_H_
#define CHROME_BROWSER_CHROMEOS_UPGRADE_DETECTOR_CHROMEOS_H_

#include "base/compiler_specific.h"
#include "base/timer/timer.h"
#include "chrome/browser/upgrade_detector.h"
#include "chromeos/dbus/update_engine_client.h"

template <typename T> struct DefaultSingletonTraits;

class UpgradeDetectorChromeos : public UpgradeDetector,
                                public chromeos::UpdateEngineClient::Observer {
 public:
  virtual ~UpgradeDetectorChromeos();

  static UpgradeDetectorChromeos* GetInstance();

  
  
  void Init();

  
  
  void Shutdown();

 private:
  friend struct DefaultSingletonTraits<UpgradeDetectorChromeos>;

  UpgradeDetectorChromeos();

  
  virtual void UpdateStatusChanged(
      const chromeos::UpdateEngineClient::Status& status) OVERRIDE;

  
  
  
  void NotifyOnUpgrade();

  
  
  base::RepeatingTimer<UpgradeDetectorChromeos> upgrade_notification_timer_;
  bool initialized_;
};

#endif  

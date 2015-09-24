// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CAMERA_DETECTOR_H_
#define CHROME_BROWSER_CHROMEOS_CAMERA_DETECTOR_H_

#include "base/callback.h"

namespace chromeos {

class CameraDetector {
 public:
  enum CameraPresence {
    kCameraPresenceUnknown = 0,
    kCameraAbsent,
    kCameraPresent
  };

  
  
  static CameraPresence camera_presence() {
    return camera_presence_;
  }

  
  
  
  static void StartPresenceCheck(const base::Closure& check_done);

 private:
  
  static void OnPresenceCheckDone(const base::Closure& callback, bool present);

  
  static bool CheckPresence();

  
  static CameraPresence camera_presence_;

  static bool presence_check_in_progress_;

  DISALLOW_COPY_AND_ASSIGN(CameraDetector);
};

}  

#endif  

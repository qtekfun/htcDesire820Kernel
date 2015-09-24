// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_DISPLAY_RESOLUTION_NOTIFICATION_CONTROLLER_H_
#define ASH_DISPLAY_RESOLUTION_NOTIFICATION_CONTROLLER_H_

#include "ash/ash_export.h"
#include "ash/display/display_controller.h"
#include "base/callback.h"
#include "base/gtest_prod_util.h"
#include "base/timer/timer.h"
#include "ui/gfx/display_observer.h"
#include "ui/gfx/size.h"

namespace chromeos {
FORWARD_DECLARE_TEST(DisplayPreferencesTest, PreventStore);
}  

namespace views {
class Label;
class Widget;
}  

namespace ash {
namespace internal {
class ASH_EXPORT ResolutionNotificationController
    : public gfx::DisplayObserver,
      public DisplayController::Observer {
 public:
  ResolutionNotificationController();
  virtual ~ResolutionNotificationController();

  
  
  
  
  
  
  void SetDisplayResolutionAndNotify(
      int64 display_id,
      const gfx::Size& old_resolution,
      const gfx::Size& new_resolution,
      const base::Closure& accept_callback);

  
  
  bool DoesNotificationTimeout();

  
  
  
  void AcceptResolutionChange(bool close_notification);

  
  
  void RevertResolutionChange();

 private:
  friend class ResolutionNotificationControllerTest;
  FRIEND_TEST_ALL_PREFIXES(ResolutionNotificationControllerTest, Timeout);
  FRIEND_TEST_ALL_PREFIXES(chromeos::DisplayPreferencesTest, PreventStore);

  
  struct ResolutionChangeInfo;

  static const int kTimeoutInSec;
  static const char kNotificationId[];

  
  
  
  
  void CreateOrUpdateNotification(bool enable_spoken_feedback);

  
  void OnTimerTick();

  
  virtual void OnDisplayBoundsChanged(const gfx::Display& display) OVERRIDE;
  virtual void OnDisplayAdded(const gfx::Display& new_display) OVERRIDE;
  virtual void OnDisplayRemoved(const gfx::Display& old_display) OVERRIDE;

  
  virtual void OnDisplayConfigurationChanged() OVERRIDE;

  static void SuppressTimerForTest();

  scoped_ptr<ResolutionChangeInfo> change_info_;

  DISALLOW_COPY_AND_ASSIGN(ResolutionNotificationController);
};

}  
}  

#endif  

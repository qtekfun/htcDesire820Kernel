// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_USER_IMAGE_SCREEN_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_USER_IMAGE_SCREEN_H_
#pragma once

#include "chrome/browser/chromeos/login/camera_controller.h"
#include "chrome/browser/chromeos/login/user_image_view.h"
#include "chrome/browser/chromeos/login/view_screen.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "third_party/skia/include/core/SkBitmap.h"

namespace chromeos {

class UserImageScreen: public ViewScreen<UserImageView>,
                       public CameraController::Delegate,
                       public UserImageView::Delegate,
                       public NotificationObserver {
 public:
  explicit UserImageScreen(WizardScreenDelegate* delegate);
  virtual ~UserImageScreen();

  
  virtual void Refresh();
  virtual void Hide();
  virtual UserImageView* AllocateView();

  
  virtual void OnCaptureSuccess();
  virtual void OnCaptureFailure();

  
  virtual void StartCamera();
  virtual void StopCamera();
  virtual void OnPhotoTaken(const SkBitmap& image);
  virtual void OnDefaultImageSelected(int index);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  CameraController camera_controller_;

  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(UserImageScreen);
};

}  

#endif  



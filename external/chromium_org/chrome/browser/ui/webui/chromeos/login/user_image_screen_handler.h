// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_USER_IMAGE_SCREEN_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_USER_IMAGE_SCREEN_HANDLER_H_

#include <string>

#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "chrome/browser/chromeos/login/screens/user_image_screen_actor.h"
#include "chrome/browser/ui/webui/chromeos/login/base_screen_handler.h"
#include "chrome/browser/ui/webui/chromeos/login/oobe_ui.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/native_widget_types.h"

namespace chromeos {

class UserImageScreenHandler : public UserImageScreenActor,
                               public BaseScreenHandler {
 public:
  UserImageScreenHandler();
  virtual ~UserImageScreenHandler();

  
  virtual void Initialize() OVERRIDE;
  virtual void DeclareLocalizedValues(LocalizedValuesBuilder* builder) OVERRIDE;

  
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void SetDelegate(
      UserImageScreenActor::Delegate* screen) OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void PrepareToShow() OVERRIDE;

  virtual void SelectImage(int index) OVERRIDE;
  virtual void SendProfileImage(const std::string& data_url) OVERRIDE;
  virtual void OnProfileImageAbsent() OVERRIDE;

  virtual void SetProfilePictureEnabled(bool enabled) OVERRIDE;

  virtual void SetCameraPresent(bool enabled) OVERRIDE;

  virtual void HideCurtain() OVERRIDE;

 private:

  
  void HandleGetImages();

  
  void HandleScreenReady();

  
  void HandlePhotoTaken(const std::string& image_url);

  
  void HandleCheckCameraPresence();

  
  void HandleSelectImage(const std::string& image_url,
                         const std::string& image_type,
                         bool is_user_selection);

  
  void HandleImageAccepted();

  
  void HandleScreenShown();

  UserImageScreenActor::Delegate* screen_;

  
  bool show_on_init_;

  
  bool is_ready_;

  base::Time screen_show_time_;

  DISALLOW_COPY_AND_ASSIGN(UserImageScreenHandler);
};

}  

#endif  

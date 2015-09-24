// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_USER_IMAGE_SCREEN_ACTOR_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_USER_IMAGE_SCREEN_ACTOR_H_

#include <string>

class SkBitmap;

namespace gfx {
class ImageSkia;
}

namespace chromeos {

class UserImageScreenActor {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual void OnScreenReady() = 0;
    
    virtual void OnPhotoTaken(const std::string& raw_data) = 0;
    
    virtual void CheckCameraPresence() = 0;
    
    
    virtual void OnImageSelected(const std::string& image_url,
                                 const std::string& image_type,
                                 bool is_user_selection) = 0;
    
    virtual void OnImageAccepted() = 0;

    
    virtual void OnActorDestroyed(UserImageScreenActor* actor) = 0;

    virtual bool profile_picture_absent() = 0;
    virtual int selected_image() = 0;
    virtual std::string profile_picture_data_url() = 0;

  };

  virtual ~UserImageScreenActor() {}

  
  virtual void SetDelegate(Delegate* screen) = 0;

  
  virtual void PrepareToShow() = 0;

  
  virtual void Show() = 0;

  
  virtual void Hide() = 0;

  
  virtual void SelectImage(int index) = 0;

  
  virtual void SendProfileImage(const std::string& data_url) = 0;

  
  virtual void OnProfileImageAbsent() = 0;

  
  virtual void SetProfilePictureEnabled(bool enabled) = 0;

  
  virtual void SetCameraPresent(bool enabled) = 0;

  
  virtual void HideCurtain() = 0;
};

}  

#endif  

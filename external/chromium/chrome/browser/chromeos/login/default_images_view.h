// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_DEFAULT_IMAGES_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_DEFAULT_IMAGES_VIEW_H_
#pragma once

#include "views/controls/button/button.h"
#include "views/view.h"

#include <vector>

class SkBitmap;

namespace chromeos {

class UserImageButton;

class DefaultImagesView : public views::View,
                          public views::ButtonListener {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual void OnCaptureButtonClicked() = 0;

    
    virtual void OnImageSelected(int image_index) = 0;
  };

  explicit DefaultImagesView(Delegate* delegate);

  
  void Init();

  
  
  int GetDefaultImageIndex() const;

  
  void SetDefaultImageIndex(int index);

  
  void ClearSelection();

  
  virtual gfx::Size GetPreferredSize();

  
  virtual void ButtonPressed(views::Button* sender, const views::Event& event);

 private:
  
  void InitButton(int resource_id, UserImageButton* button) const;

  
  void InitLayout();

  
  
  std::vector<UserImageButton*> default_images_;

  
  int selected_image_index_;

  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(DefaultImagesView);
};

}  

#endif  



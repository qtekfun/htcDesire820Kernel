// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_USER_IMAGE_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_USER_IMAGE_VIEW_H_
#pragma once

#include "chrome/browser/chromeos/login/default_images_view.h"
#include "chrome/browser/chromeos/login/take_photo_view.h"
#include "views/controls/button/button.h"
#include "views/view.h"

class SkBitmap;

namespace views {
class Label;
class NativeButton;
}  

namespace chromeos {

class UserImageView : public views::View,
                      public views::ButtonListener,
                      public TakePhotoView::Delegate,
                      public DefaultImagesView::Delegate {
 public:
  
  class Delegate {
  public:
    virtual ~Delegate() {}

    
    virtual void StartCamera() = 0;

    
    virtual void StopCamera() = 0;

    
    
    virtual void OnPhotoTaken(const SkBitmap& image) = 0;

    
    
    virtual void OnDefaultImageSelected(int index) = 0;
  };

  explicit UserImageView(Delegate* delegate);
  virtual ~UserImageView();

  
  void Init();

  
  void UpdateVideoFrame(const SkBitmap& frame);

  
  
  
  void ShowCameraInitializing();

  
  
  void ShowCameraError();

  
  bool IsCapturing() const;

  
  virtual gfx::Size GetPreferredSize();

  
  virtual void ButtonPressed(views::Button* sender, const views::Event& event);

  
  virtual void OnCapturingStarted();
  virtual void OnCapturingStopped();

  
  virtual void OnCaptureButtonClicked();
  virtual void OnImageSelected(int image_index);

 private:
  
  void InitLayout();

  views::Label* title_label_;
  DefaultImagesView* default_images_view_;
  TakePhotoView* take_photo_view_;
  views::View* splitter_;
  views::NativeButton* ok_button_;

  
  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(UserImageView);
};

}  

#endif  

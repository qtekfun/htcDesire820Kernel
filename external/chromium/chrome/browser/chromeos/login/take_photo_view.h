// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_TAKE_PHOTO_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_TAKE_PHOTO_VIEW_H_
#pragma once

#include "views/controls/button/button.h"
#include "views/view.h"

class SkBitmap;

namespace views {
class ImageButton;
class Label;
}  

namespace chromeos {

class CameraImageView;

class TakePhotoView : public views::View,
                      public views::ButtonListener {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual void OnCapturingStarted() = 0;

    
    virtual void OnCapturingStopped() = 0;
  };

  explicit TakePhotoView(Delegate* delegate);
  virtual ~TakePhotoView();

  
  void Init();

  
  void UpdateVideoFrame(const SkBitmap& frame);

  
  
  
  void ShowCameraInitializing();

  
  
  void ShowCameraError();

  
  const SkBitmap& GetImage() const;

  
  void SetImage(SkBitmap* image);

  
  virtual gfx::Size GetPreferredSize();

  
  virtual void ButtonPressed(views::Button* sender, const views::Event& event);

  bool is_capturing() const { return is_capturing_; }

  void set_show_title(bool show) { show_title_ = show; }

 private:
  
  void InitLayout();

  views::Label* title_label_;
  views::ImageButton* snapshot_button_;
  CameraImageView* user_image_;

  
  
  bool is_capturing_;

  
  bool show_title_;

  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(TakePhotoView);
};

}  

#endif  

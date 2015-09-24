// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_CAPTCHA_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_CAPTCHA_VIEW_H_
#pragma once

#include <string>

#include "chrome/browser/chromeos/login/image_decoder.h"
#include "googleurl/src/gurl.h"
#include "views/controls/button/button.h"
#include "views/controls/textfield/textfield_controller.h"
#include "views/window/dialog_delegate.h"

namespace views {
class ImageView;
class TextButton;
class View;
class Window;
}  

namespace chromeos {

class CaptchaView : public views::View,
                    public views::DialogDelegate,
                    public views::TextfieldController,
                    public ImageDecoder::Delegate,
                    public views::ButtonListener {
 public:
  class Delegate {
   public:
    
    virtual void OnCaptchaEntered(const std::string& captcha) = 0;

   protected:
     virtual ~Delegate() {}
  };

  
  
  CaptchaView(const GURL& captcha_url, bool is_standalone);
  virtual ~CaptchaView() {}

  
  virtual bool Accept();
  virtual bool IsModal() const { return true; }
  virtual views::View* GetContentsView() { return this; }
  virtual std::wstring GetWindowTitle() const;

  
  virtual void ContentsChanged(views::Textfield* sender,
                               const string16& new_contents) {}
  virtual bool HandleKeyEvent(views::Textfield* sender,
                              const views::KeyEvent& key_event);

  
  virtual void OnImageDecoded(const ImageDecoder* decoder,
                              const SkBitmap& decoded_image);

  
  virtual void ButtonPressed(views::Button* sender, const views::Event& event);

  
  void Init();

  void set_delegate(Delegate* delegate) {
    delegate_ = delegate;
  }

  
  
  void SetCaptchaURL(const GURL& captcha_url);

 protected:
  
  virtual gfx::Size GetPreferredSize();
  virtual void ViewHierarchyChanged(bool is_add,
                                    views::View* parent,
                                    views::View* child);

 private:
  Delegate* delegate_;
  GURL captcha_url_;
  views::ImageView* captcha_image_;
  views::Textfield* captcha_textfield_;

  
  
  bool is_standalone_;

  
  views::TextButton* ok_button_;

  DISALLOW_COPY_AND_ASSIGN(CaptchaView);
};

}  

#endif  

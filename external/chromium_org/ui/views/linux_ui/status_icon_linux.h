// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_LINUX_UI_STATUS_ICON_LINUX_H_
#define UI_VIEWS_LINUX_UI_STATUS_ICON_LINUX_H_

#include "base/strings/string16.h"
#include "ui/views/views_export.h"

namespace gfx {
class ImageSkia;
}

namespace ui {
class MenuModel;
}  

namespace views {

class VIEWS_EXPORT StatusIconLinux {
 public:
  class Delegate {
   public:
    virtual void OnClick() = 0;
    virtual bool HasClickAction() = 0;

   protected:
    virtual ~Delegate();
  };

  StatusIconLinux();
  virtual ~StatusIconLinux();

  virtual void SetImage(const gfx::ImageSkia& image) = 0;
  virtual void SetPressedImage(const gfx::ImageSkia& image) = 0;
  virtual void SetToolTip(const string16& tool_tip) = 0;

  
  
  
  virtual void UpdatePlatformContextMenu(ui::MenuModel* model) = 0;

  
  
  
  virtual void RefreshPlatformContextMenu();

  Delegate* delegate() { return delegate_; }
  void set_delegate(Delegate* delegate) { delegate_ = delegate; }

 private:
  Delegate* delegate_;
};

}  

#endif  

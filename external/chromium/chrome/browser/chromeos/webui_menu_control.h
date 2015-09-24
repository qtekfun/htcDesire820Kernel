// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_WEBUI_MENU_CONTROL_H_
#define CHROME_BROWSER_CHROMEOS_WEBUI_MENU_CONTROL_H_
#pragma once

namespace gfx {
class Size;
}  

namespace ui {
class MenuModel;
}  

namespace chromeos {

class WebUIMenuControl {
 public:
  enum ActivationMode {
    ACTIVATE_NO_CLOSE,   
    CLOSE_AND_ACTIVATE,  
  };
  virtual ~WebUIMenuControl() {}

  
  virtual ui::MenuModel* GetMenuModel() = 0;

  
  virtual void Activate(ui::MenuModel* model,
                        int index,
                        ActivationMode activation_mode) = 0;

  
  virtual void CloseAll() = 0;

  
  virtual void CloseSubmenu() = 0;

  
  virtual void MoveInputToParent() = 0;

  
  virtual void MoveInputToSubmenu() = 0;

  
  
  virtual void OnLoad() = 0;

  
  
  
  virtual void OpenSubmenu(int index, int y_top) =0;

  
  virtual void SetSize(const gfx::Size& size) = 0;
};

}  

#endif  

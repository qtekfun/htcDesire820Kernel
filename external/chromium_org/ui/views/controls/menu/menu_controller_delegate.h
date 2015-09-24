// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_MENU_MENU_CONTROLLER_DELEGATE_H_
#define UI_VIEWS_CONTROLS_MENU_MENU_CONTROLLER_DELEGATE_H_

namespace views {

class MenuItemView;

namespace internal {

class MenuControllerDelegate {
 public:
  enum NotifyType {
    NOTIFY_DELEGATE,
    DONT_NOTIFY_DELEGATE
  };

  
  
  virtual void DropMenuClosed(NotifyType type, MenuItemView* menu) = 0;

  
  virtual void SiblingMenuCreated(MenuItemView* menu) = 0;

 protected:
  virtual ~MenuControllerDelegate() {}
};

}  

}  

#endif  

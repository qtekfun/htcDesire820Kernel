// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_STATUS_NETWORK_MENU_H_
#define CHROME_BROWSER_CHROMEOS_STATUS_NETWORK_MENU_H_

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "ui/gfx/native_widget_types.h"  

class Browser;

namespace ui {
class MenuModel;
}

namespace views {
class MenuItemView;
class MenuButton;
class MenuRunner;
}

namespace chromeos {

class NetworkMenuModel;


class NetworkMenu {
 public:
  class Delegate {
   public:
    virtual ~Delegate();
    virtual gfx::NativeWindow GetNativeWindow() const = 0;
    virtual void OpenButtonOptions() = 0;
    virtual bool ShouldOpenButtonOptions() const = 0;
    virtual void OnConnectToNetworkRequested(
        const std::string& service_path) = 0;
  };

  explicit NetworkMenu(Delegate* delegate);
  virtual ~NetworkMenu();

  
  ui::MenuModel* GetMenuModel();

  
  virtual void UpdateMenu();

 private:
  friend class NetworkMenuModel;

  
  Delegate* delegate() const { return delegate_; }

  
  Delegate* delegate_;

  
  bool refreshing_menu_;

  
  scoped_ptr<NetworkMenuModel> main_menu_model_;

  
  
  
  base::WeakPtrFactory<NetworkMenu> weak_pointer_factory_;

  DISALLOW_COPY_AND_ASSIGN(NetworkMenu);
};

}  

#endif  

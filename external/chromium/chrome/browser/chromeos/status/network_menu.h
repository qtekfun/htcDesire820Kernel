// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_STATUS_NETWORK_MENU_H_
#define CHROME_BROWSER_CHROMEOS_STATUS_NETWORK_MENU_H_
#pragma once

#include <string>
#include <vector>

#include "chrome/browser/chromeos/options/network_config_view.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/base/models/menu_model.h"
#include "ui/gfx/native_widget_types.h"
#include "views/controls/menu/view_menu_delegate.h"

namespace gfx {
class Canvas;
}

namespace views {
class Menu2;
}

namespace chromeos {

class NetworkMenu;

class NetworkMenuModel : public ui::MenuModel {
 public:
  struct NetworkInfo {
    NetworkInfo() :
        need_passphrase(false), remembered(true), auto_connect(true) {}
    
    std::string network_type;
    
    std::string status;
    
    std::string message;
    
    std::string ip_address;
    
    std::string passphrase;
    
    bool need_passphrase;
    
    bool remembered;
    
    bool auto_connect;
  };

  explicit NetworkMenuModel(NetworkMenu* owner) : owner_(owner) {}
  virtual ~NetworkMenuModel() {}

  
  
  
  bool ConnectToNetworkAt(int index,
                          const std::string& passphrase,
                          const std::string& ssid,
                          int remember) const;

  
  virtual void InitMenuItems(bool is_browser_mode,
                             bool should_open_button_options) = 0;

  
  virtual bool HasIcons() const  { return true; }
  virtual int GetItemCount() const;
  virtual ui::MenuModel::ItemType GetTypeAt(int index) const;
  virtual int GetCommandIdAt(int index) const { return index; }
  virtual string16 GetLabelAt(int index) const;
  virtual bool IsItemDynamicAt(int index) const { return true; }
  virtual const gfx::Font* GetLabelFontAt(int index) const;
  virtual bool GetAcceleratorAt(int index,
      ui::Accelerator* accelerator) const { return false; }
  virtual bool IsItemCheckedAt(int index) const;
  virtual int GetGroupIdAt(int index) const { return 0; }
  virtual bool GetIconAt(int index, SkBitmap* icon);
  virtual ui::ButtonMenuItemModel* GetButtonMenuItemAt(int index) const {
    return NULL;
  }
  virtual bool IsEnabledAt(int index) const;
  virtual ui::MenuModel* GetSubmenuModelAt(int index) const;
  virtual void HighlightChangedTo(int index) {}
  virtual void ActivatedAt(int index);
  virtual void MenuWillShow() {}
  virtual void SetMenuModelDelegate(ui::MenuModelDelegate* delegate) {}

 protected:
  enum MenuItemFlags {
    FLAG_NONE              = 0,
    FLAG_DISABLED          = 1 << 0,
    FLAG_TOGGLE_ETHERNET   = 1 << 1,
    FLAG_TOGGLE_WIFI       = 1 << 2,
    FLAG_TOGGLE_CELLULAR   = 1 << 3,
    FLAG_TOGGLE_OFFLINE    = 1 << 4,
    FLAG_ASSOCIATED        = 1 << 5,
    FLAG_ETHERNET          = 1 << 6,
    FLAG_WIFI              = 1 << 7,
    FLAG_CELLULAR          = 1 << 8,
    FLAG_OPTIONS           = 1 << 9,
    FLAG_ADD_WIFI          = 1 << 10,
    FLAG_ADD_CELLULAR      = 1 << 11,
    FLAG_VPN               = 1 << 12,
    FLAG_ADD_VPN           = 1 << 13,
    FLAG_DISCONNECT_VPN    = 1 << 14,
    FLAG_VIEW_ACCOUNT      = 1 << 15,
  };

  struct MenuItem {
    MenuItem()
        : type(ui::MenuModel::TYPE_SEPARATOR),
          sub_menu_model(NULL),
          flags(0) {}
    MenuItem(ui::MenuModel::ItemType type, string16 label, SkBitmap icon,
             const std::string& service_path, int flags)
        : type(type),
          label(label),
          icon(icon),
          service_path(service_path),
          sub_menu_model(NULL),
          flags(flags) {}
    MenuItem(ui::MenuModel::ItemType type, string16 label, SkBitmap icon,
             NetworkMenuModel* sub_menu_model, int flags)
        : type(type),
          label(label),
          icon(icon),
          sub_menu_model(sub_menu_model),
          flags(flags) {}

    ui::MenuModel::ItemType type;
    string16 label;
    SkBitmap icon;
    std::string service_path;
    NetworkMenuModel* sub_menu_model;  
    int flags;
  };
  typedef std::vector<MenuItem> MenuItemVector;

  
  MenuItemVector menu_items_;

  NetworkMenu* owner_;  

  
  std::string top_up_url_;

  
  
  std::string carrier_id_;

 private:
  
  void ShowNetworkConfigView(NetworkConfigView* view) const;

  void ActivateCellular(const CellularNetwork* cellular) const;
  void ShowOther(ConnectionType type) const;
  void ShowOtherCellular() const;

  DISALLOW_COPY_AND_ASSIGN(NetworkMenuModel);
};

class NetworkMenu : public views::ViewMenuDelegate {
 public:
  NetworkMenu();
  virtual ~NetworkMenu();

  void SetFirstLevelMenuWidth(int width);

  
  void CancelMenu();

  virtual bool IsBrowserMode() const = 0;

  
  

  
  
  
  
  static const SkBitmap* IconForNetworkStrength(const WifiNetwork* wifi,
                                                bool black);
  
  
  
  
  static const SkBitmap* IconForNetworkStrength(const CellularNetwork* cellular,
                                                bool black);
  
  
  
  
  
  static const SkBitmap* IconForNetworkConnecting(double animation_value,
                                                  bool black);

  
  
  
  static const SkBitmap* BadgeForNetworkTechnology(
      const CellularNetwork* cellular);
  
  
  
  static const SkBitmap* BadgeForRoamingStatus(const CellularNetwork* cellular);
  
  
  
  
  
  static const SkBitmap* BadgeForPrivateNetworkStatus(const Network* network);

  
  
  
  
  static SkBitmap IconForDisplay(const SkBitmap* icon, const SkBitmap* badge);
  
  
  
  
  
  static SkBitmap IconForDisplay(const SkBitmap* icon,
                                 const SkBitmap* bottom_right_badge,
                                 const SkBitmap* top_left_badge,
                                 const SkBitmap* bottom_left_badge);

 protected:
  virtual gfx::NativeWindow GetNativeWindow() const = 0;
  virtual void OpenButtonOptions() = 0;
  virtual bool ShouldOpenButtonOptions() const = 0;

  
  virtual void OnConnectNetwork(const Network* network,
                                SkBitmap selected_icon_) {}

  
  void ShowTabbedNetworkSettings(const Network* network) const;

  
  void UpdateMenu();

 private:
  friend class NetworkMenuModel;

  
  virtual void RunMenu(views::View* source, const gfx::Point& pt);

  
  bool refreshing_menu_;

  
  static const int kNumBarsImages;

  
  static const int kBarsImages[];
  static const int kBarsImagesBlack[];
  static const int kBarsImagesOrange[];
  
  

  
  static SkBitmap kAnimatingImages[];
  static SkBitmap kAnimatingImagesBlack[];

  
  scoped_ptr<views::Menu2> network_menu_;

  scoped_ptr<NetworkMenuModel> main_menu_model_;

  
  int min_width_;

  
  bool use_settings_ui_;

  DISALLOW_COPY_AND_ASSIGN(NetworkMenu);
};

}  

#endif  

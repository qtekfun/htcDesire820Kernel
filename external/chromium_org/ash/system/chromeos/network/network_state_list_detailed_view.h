// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_CHROMEOS_NETWORK_NETWORK_STATE_LIST_DETAILED_VIEW_H
#define ASH_SYSTEM_CHROMEOS_NETWORK_NETWORK_STATE_LIST_DETAILED_VIEW_H

#include <map>
#include <string>
#include <vector>

#include "ash/system/chromeos/network/network_detailed_view.h"
#include "ash/system/chromeos/network/network_icon.h"
#include "ash/system/chromeos/network/network_icon_animation_observer.h"
#include "ash/system/tray/view_click_listener.h"
#include "ash/system/user/login_status.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "ui/views/controls/button/button.h"

namespace chromeos {
class NetworkTypePattern;
}

namespace views {
class BubbleDelegateView;
}

namespace ash {

class SystemTrayItem;

namespace internal {

class HoverHighlightView;
class TrayPopupLabelButton;

namespace tray {

struct NetworkInfo;

class NetworkStateListDetailedView
    : public NetworkDetailedView,
      public views::ButtonListener,
      public ViewClickListener,
      public network_icon::AnimationObserver,
      public base::SupportsWeakPtr<NetworkStateListDetailedView> {
 public:
  enum ListType {
    LIST_TYPE_NETWORK,
    LIST_TYPE_DEBUG_PREFERRED,
    LIST_TYPE_VPN
  };

  NetworkStateListDetailedView(SystemTrayItem* owner,
                               ListType list_type,
                               user::LoginStatus login);
  virtual ~NetworkStateListDetailedView();

  
  virtual void Init() OVERRIDE;
  virtual DetailedViewType GetViewType() const OVERRIDE;
  virtual void ManagerChanged() OVERRIDE;
  virtual void NetworkListChanged() OVERRIDE;
  virtual void NetworkServiceChanged(
      const chromeos::NetworkState* network) OVERRIDE;

  
  virtual void NetworkIconChanged() OVERRIDE;

 protected:
  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void OnViewClicked(views::View* sender) OVERRIDE;

 private:
  class InfoBubble;

  typedef std::map<views::View*, std::string> NetworkMap;
  typedef std::map<std::string, HoverHighlightView*> ServicePathMap;

  
  void CreateHeaderEntry();
  void CreateHeaderButtons();
  void CreateNetworkExtra();

  
  void UpdateHeaderButtons();
  void UpdateTechnologyButton(TrayPopupHeaderButton* button,
                              const chromeos::NetworkTypePattern& technology);

  void UpdateNetworks(
      const chromeos::NetworkStateHandler::NetworkStateList& networks);
  void UpdatePreferred(
      const chromeos::NetworkStateHandler::FavoriteStateList& favorites);
  void UpdateNetworkList();
  bool CreateOrUpdateInfoLabel(
      int index, const base::string16& text, views::Label** label);
  bool UpdateNetworkChild(int index, const NetworkInfo* info);
  bool OrderChild(views::View* view, int index);
  bool UpdateNetworkListEntries(std::set<std::string>* new_service_paths);
  void UpdateNetworkExtra();

  
  
  void CreateSettingsEntry();

  
  void ToggleInfoBubble();
  bool ResetInfoBubble();
  void OnInfoBubbleDestroyed();
  views::View* CreateNetworkInfoView();

  
  void CallRequestScan();

  
  void ToggleMobile();

  
  ListType list_type_;

  
  user::LoginStatus login_;

  
  NetworkMap network_map_;

  
  ServicePathMap service_path_map_;

  
  ScopedVector<NetworkInfo> network_list_;

  
  TrayPopupHeaderButton* info_icon_;
  TrayPopupHeaderButton* button_wifi_;
  TrayPopupHeaderButton* button_mobile_;
  TrayPopupLabelButton* other_wifi_;
  TrayPopupLabelButton* turn_on_wifi_;
  TrayPopupLabelButton* other_mobile_;
  TrayPopupLabelButton* other_vpn_;
  TrayPopupLabelButton* toggle_debug_preferred_networks_;
  TrayPopupLabelButton* settings_;
  TrayPopupLabelButton* proxy_settings_;
  views::Label* scanning_view_;
  views::Label* no_wifi_networks_view_;
  views::Label* no_cellular_networks_view_;

  
  views::BubbleDelegateView* info_bubble_;

  DISALLOW_COPY_AND_ASSIGN(NetworkStateListDetailedView);
};

}  
}  
}  

#endif  
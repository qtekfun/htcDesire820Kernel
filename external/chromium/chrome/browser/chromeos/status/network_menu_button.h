// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_STATUS_NETWORK_MENU_BUTTON_H_
#define CHROME_BROWSER_CHROMEOS_STATUS_NETWORK_MENU_BUTTON_H_
#pragma once

#include <string>

#include "base/task.h"
#include "base/timer.h"
#include "chrome/browser/chromeos/cros/network_library.h"
#include "chrome/browser/chromeos/customization_document.h"
#include "chrome/browser/chromeos/login/message_bubble.h"
#include "chrome/browser/chromeos/status/network_menu.h"
#include "chrome/browser/chromeos/status/status_area_button.h"
#include "ui/base/animation/throb_animation.h"

namespace gfx {
class Canvas;
}

namespace chromeos {

class StatusAreaHost;

class NetworkMenuButton : public StatusAreaButton,
                          public NetworkMenu,
                          public NetworkLibrary::NetworkDeviceObserver,
                          public NetworkLibrary::NetworkManagerObserver,
                          public NetworkLibrary::NetworkObserver,
                          public NetworkLibrary::CellularDataPlanObserver,
                          public MessageBubbleDelegate {
 public:
  explicit NetworkMenuButton(StatusAreaHost* host);
  virtual ~NetworkMenuButton();

  
  virtual void AnimationProgressed(const ui::Animation* animation);

  
  virtual void OnNetworkDeviceChanged(NetworkLibrary* cros,
                                      const NetworkDevice* device);
  
  virtual void OnNetworkManagerChanged(NetworkLibrary* cros);
  
  virtual void OnNetworkChanged(NetworkLibrary* cros, const Network* network);
  
  virtual void OnCellularDataPlanChanged(NetworkLibrary* cros);

  
  virtual bool IsBrowserMode() const;

 protected:
  
  virtual gfx::NativeWindow GetNativeWindow() const;
  virtual void OpenButtonOptions();
  virtual bool ShouldOpenButtonOptions() const;

  
  virtual void OnLocaleChanged() OVERRIDE;

  
  virtual void BubbleClosing(Bubble* bubble, bool closed_by_escape) {
    mobile_data_bubble_ = NULL;
  }
  virtual bool CloseOnEscape() { return true; }
  virtual bool FadeInOnShow() { return false; }
  virtual void OnHelpLinkActivated();

 private:
  
  
  const ServicesCustomizationDocument::CarrierDeal* GetCarrierDeal(
      NetworkLibrary* cros);

  
  void SetIconAndBadges(const SkBitmap* icon,
                        const SkBitmap* right_badge,
                        const SkBitmap* left_badge);
  
  void SetIconOnly(const SkBitmap* icon);
  
  void SetBadgesOnly(const SkBitmap* right_badge, const SkBitmap* left_badge);
  
  void SetNetworkIcon(NetworkLibrary* cros, const Network* network);

  
  
  
  void RefreshNetworkDeviceObserver(NetworkLibrary* cros);

  
  
  void RefreshNetworkObserver(NetworkLibrary* cros);

  
  void ShowOptionalMobileDataPromoNotification(NetworkLibrary* cros);

  
  std::string cellular_device_path_;

  
  const SkBitmap* icon_;
  
  const SkBitmap* right_badge_;
  
  const SkBitmap* left_badge_;

  
  MessageBubble* mobile_data_bubble_;

  
  
  bool check_for_promo_;

  
  bool was_sim_locked_;

  
  ui::ThrobAnimation animation_connecting_;

  
  static const int kThrobDuration;

  
  
  std::string active_network_;

  
  std::string deal_url_;

  
  ScopedRunnableMethodFactory<NetworkMenuButton> method_factory_;

  DISALLOW_COPY_AND_ASSIGN(NetworkMenuButton);
};

}  

#endif  

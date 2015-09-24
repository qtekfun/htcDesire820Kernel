// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_NETWORK_SCREEN_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_NETWORK_SCREEN_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "base/string16.h"
#include "base/task.h"
#include "base/timer.h"
#include "chrome/browser/chromeos/cros/network_library.h"
#include "chrome/browser/chromeos/login/keyboard_switch_menu.h"
#include "chrome/browser/chromeos/login/language_switch_menu.h"
#include "chrome/browser/chromeos/login/message_bubble.h"
#include "chrome/browser/chromeos/login/network_screen_delegate.h"
#include "chrome/browser/chromeos/login/view_screen.h"
#include "chrome/browser/chromeos/options/network_config_view.h"

class WizardScreenDelegate;

namespace chromeos {

class HelpAppLauncher;
class NetworkSelectionView;

class NetworkScreen : public ViewScreen<NetworkSelectionView>,
                      public MessageBubbleDelegate,
                      public NetworkScreenDelegate {
 public:
  explicit NetworkScreen(WizardScreenDelegate* delegate);
  virtual ~NetworkScreen();

  
  virtual void ClearErrors();
  virtual bool is_error_shown() { return bubble_ != NULL; }
  virtual LanguageSwitchMenu* language_switch_menu() {
    return &language_switch_menu_;
  }
  virtual KeyboardSwitchMenu* keyboard_switch_menu() {
    return &keyboard_switch_menu_;
  }
  virtual gfx::Size size() const { return GetScreenSize(); }

  
  virtual void ButtonPressed(views::Button* sender, const views::Event& event);

  
  virtual void OnNetworkManagerChanged(NetworkLibrary* network_lib);

 protected:
  
  
  void Refresh();

 private:
  FRIEND_TEST(NetworkScreenTest, Timeout);

  
  virtual void CreateView();
  virtual NetworkSelectionView* AllocateView();

  
  virtual void BubbleClosing(Bubble* bubble, bool closed_by_escape) {
    bubble_ = NULL;
  }
  virtual bool CloseOnEscape() { return true; }
  virtual bool FadeInOnShow() { return false; }
  virtual void OnHelpLinkActivated();

  
  void SubscribeNetworkNotification();

  
  void UnsubscribeNetworkNotification();

  
  void NotifyOnConnection();

  
  void OnConnectionTimeout();

  
  void UpdateStatus(NetworkLibrary* network);

  
  void StopWaitingForConnection(const string16& network_id);

  
  void WaitForConnection(const string16& network_id);

  
  bool is_network_subscribed_;

  
  string16 network_id_;

  
  
  bool continue_pressed_;

  
  base::OneShotTimer<NetworkScreen> connection_timer_;

  LanguageSwitchMenu language_switch_menu_;
  KeyboardSwitchMenu keyboard_switch_menu_;

  
  
  MessageBubble* bubble_;

  
  scoped_refptr<HelpAppLauncher> help_app_;

  DISALLOW_COPY_AND_ASSIGN(NetworkScreen);
};

}  

#endif  

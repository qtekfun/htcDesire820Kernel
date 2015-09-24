// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_NETWORK_SCREEN_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_NETWORK_SCREEN_H_

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "base/timer/timer.h"
#include "chrome/browser/chromeos/login/screens/network_screen_actor.h"
#include "chrome/browser/chromeos/login/screens/wizard_screen.h"
#include "chromeos/network/network_state_handler_observer.h"

namespace chromeos {

namespace login {
class NetworkStateHelper;
}  

class NetworkScreen : public WizardScreen,
                      public NetworkStateHandlerObserver,
                      public NetworkScreenActor::Delegate {
 public:
  NetworkScreen(ScreenObserver* screen_observer, NetworkScreenActor* actor);
  virtual ~NetworkScreen();

  
  virtual void PrepareToShow() OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual std::string GetName() const OVERRIDE;

  
  virtual void NetworkConnectionStateChanged(
      const NetworkState* network) OVERRIDE;
  virtual void DefaultNetworkChanged(const NetworkState* network) OVERRIDE;

  
  virtual void OnActorDestroyed(NetworkScreenActor* actor) OVERRIDE;
  virtual void OnContinuePressed() OVERRIDE;

  NetworkScreenActor* actor() const { return actor_; }

 protected:
  
  
  virtual void Refresh();

 private:
  friend class NetworkScreenTest;
  FRIEND_TEST_ALL_PREFIXES(NetworkScreenTest, Timeout);
  FRIEND_TEST_ALL_PREFIXES(NetworkScreenTest, CanConnect);

  
  
  void SetNetworkStateHelperForTest(login::NetworkStateHelper* helper);

  
  void SubscribeNetworkNotification();

  
  void UnsubscribeNetworkNotification();

  
  void NotifyOnConnection();

  
  void OnConnectionTimeout();

  
  void UpdateStatus();

  
  void StopWaitingForConnection(const base::string16& network_id);

  
  void WaitForConnection(const base::string16& network_id);

  
  bool is_network_subscribed_;

  
  base::string16 network_id_;

  
  
  bool continue_pressed_;

  
  base::OneShotTimer<NetworkScreen> connection_timer_;

  NetworkScreenActor* actor_;
  scoped_ptr<login::NetworkStateHelper> network_state_helper_;

  DISALLOW_COPY_AND_ASSIGN(NetworkScreen);
};

}  

#endif  

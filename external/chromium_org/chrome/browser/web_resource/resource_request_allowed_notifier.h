// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEB_RESOURCE_RESOURCE_REQUEST_ALLOWED_NOTIFIER_H_
#define CHROME_BROWSER_WEB_RESOURCE_RESOURCE_REQUEST_ALLOWED_NOTIFIER_H_

#include "chrome/browser/web_resource/eula_accepted_notifier.h"
#include "net/base/network_change_notifier.h"

class ResourceRequestAllowedNotifier
    : public EulaAcceptedNotifier::Observer,
      public net::NetworkChangeNotifier::ConnectionTypeObserver {
 public:
  
  class Observer {
   public:
    virtual void OnResourceRequestsAllowed() = 0;
  };

  
  enum State {
    ALLOWED,
    DISALLOWED_EULA_NOT_ACCEPTED,
    DISALLOWED_NETWORK_DOWN,
    DISALLOWED_COMMAND_LINE_DISABLED,
  };

  ResourceRequestAllowedNotifier();
  virtual ~ResourceRequestAllowedNotifier();

  
  
  
  
  void Init(Observer* observer);

  
  
  
  
  
  virtual State GetResourceRequestsAllowedState();

  
  
  bool ResourceRequestsAllowed();

  void SetWaitingForNetworkForTesting(bool waiting);
  void SetWaitingForEulaForTesting(bool waiting);
  void SetObserverRequestedForTesting(bool requested);

 protected:
  
  
  void MaybeNotifyObserver();

 private:
  
  
  virtual EulaAcceptedNotifier* CreateEulaNotifier();

  
  virtual void OnEulaAccepted() OVERRIDE;

  
  virtual void OnConnectionTypeChanged(
      net::NetworkChangeNotifier::ConnectionType type) OVERRIDE;

  
  
  
  bool observer_requested_permission_;

  
  bool waiting_for_network_;

  
  bool waiting_for_user_to_accept_eula_;

  
  scoped_ptr<EulaAcceptedNotifier> eula_notifier_;

  
  Observer* observer_;

  DISALLOW_COPY_AND_ASSIGN(ResourceRequestAllowedNotifier);
};

#endif  

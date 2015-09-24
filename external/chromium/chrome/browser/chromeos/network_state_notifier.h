// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_NETWORK_STATE_NOTIFIER_H_
#define CHROME_BROWSER_CHROMEOS_NETWORK_STATE_NOTIFIER_H_
#pragma once

#include "chrome/browser/chromeos/cros/network_library.h"

#include "base/memory/singleton.h"
#include "base/task.h"
#include "base/time.h"

namespace chromeos {

class NetworkStateDetails {
 public:
  enum State {
    UNKNOWN = 0,
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
  };

  State state() const {
    return state_;
  }

 private:
  friend class NetworkStateNotifier;

  explicit NetworkStateDetails(State state)
      : state_(state) {
  }

  State state_;

  DISALLOW_COPY_AND_ASSIGN(NetworkStateDetails);
};

class NetworkStateNotifier : public NetworkLibrary::NetworkManagerObserver {
 public:
  
  static NetworkStateNotifier* GetInstance();

  
  
  static base::TimeDelta GetOfflineDuration();

  
  static bool is_connected() {
    return GetInstance()->state_ == NetworkStateDetails::CONNECTED;
  }

  
  virtual void OnNetworkManagerChanged(NetworkLibrary* cros);

 private:
  friend struct DefaultSingletonTraits<NetworkStateNotifier>;

  
  static NetworkStateDetails::State RetrieveState();

  NetworkStateNotifier();
  virtual ~NetworkStateNotifier();

  
  
  void UpdateNetworkState(NetworkStateDetails::State new_state);

  
  ScopedRunnableMethodFactory<NetworkStateNotifier> task_factory_;

  
  NetworkStateDetails::State state_;

  
  base::Time offline_start_time_;

  DISALLOW_COPY_AND_ASSIGN(NetworkStateNotifier);
};

}  

#endif  

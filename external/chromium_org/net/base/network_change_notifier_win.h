// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_NETWORK_CHANGE_NOTIFIER_WIN_H_
#define NET_BASE_NETWORK_CHANGE_NOTIFIER_WIN_H_

#include <windows.h>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/timer/timer.h"
#include "base/win/object_watcher.h"
#include "net/base/net_export.h"
#include "net/base/network_change_notifier.h"

namespace net {

class NET_EXPORT_PRIVATE NetworkChangeNotifierWin
    : public NetworkChangeNotifier,
      public base::win::ObjectWatcher::Delegate,
      NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  NetworkChangeNotifierWin();

  
  
  
  
  
  
  
  void WatchForAddressChange();

 protected:
  virtual ~NetworkChangeNotifierWin();

  
  bool is_watching() { return is_watching_; }
  void set_is_watching(bool is_watching) { is_watching_ = is_watching; }
  int sequential_failures() { return sequential_failures_; }

 private:
  class DnsConfigServiceThread;
  friend class NetworkChangeNotifierWinTest;

  
  virtual ConnectionType GetCurrentConnectionType() const OVERRIDE;

  
  
  virtual void OnObjectSignaled(HANDLE object) OVERRIDE;

  
  
  ConnectionType RecomputeCurrentConnectionType() const;

  void SetCurrentConnectionType(ConnectionType connection_type);

  
  
  
  void NotifyObservers();

  
  void NotifyParentOfConnectionTypeChange();

  
  
  
  
  virtual bool WatchForAddressChangeInternal();

  static NetworkChangeCalculatorParams NetworkChangeCalculatorParamsWin();

  
  

  
  
  
  bool is_watching_;

  base::win::ObjectWatcher addr_watcher_;
  OVERLAPPED addr_overlapped_;

  base::OneShotTimer<NetworkChangeNotifierWin> timer_;

  
  int sequential_failures_;

  
  base::WeakPtrFactory<NetworkChangeNotifierWin> weak_factory_;

  
  scoped_ptr<DnsConfigServiceThread> dns_config_service_thread_;

  mutable base::Lock last_computed_connection_type_lock_;
  ConnectionType last_computed_connection_type_;

  
  
  bool last_announced_offline_;
  
  int offline_polls_;

  DISALLOW_COPY_AND_ASSIGN(NetworkChangeNotifierWin);
};

}  

#endif  

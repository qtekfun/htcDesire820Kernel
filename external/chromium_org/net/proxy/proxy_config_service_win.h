// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_CONFIG_SERVICE_WIN_H_
#define NET_PROXY_PROXY_CONFIG_SERVICE_WIN_H_

#include <windows.h>
#include <winhttp.h>

#include <vector>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/win/object_watcher.h"
#include "net/proxy/polling_proxy_config_service.h"

namespace net {

class NET_EXPORT_PRIVATE ProxyConfigServiceWin
    : public PollingProxyConfigService,
      public base::win::ObjectWatcher::Delegate {
 public:
  ProxyConfigServiceWin();
  virtual ~ProxyConfigServiceWin();

  
  virtual void AddObserver(Observer* observer) OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(ProxyConfigServiceWinTest, SetFromIEConfig);
  class KeyEntry;
  typedef std::vector<KeyEntry*> KeyEntryList;

  
  void StartWatchingRegistryForChanges();

  
  
  bool AddKeyToWatchList(HKEY rootkey, const wchar_t* subkey);

  
  
  virtual void OnObjectSignaled(HANDLE object) OVERRIDE;

  static void GetCurrentProxyConfig(ProxyConfig* config);

  
  static void SetFromIEConfig(
      ProxyConfig* config,
      const WINHTTP_CURRENT_USER_IE_PROXY_CONFIG& ie_config);

  KeyEntryList keys_to_watch_;
};

}  

#endif  

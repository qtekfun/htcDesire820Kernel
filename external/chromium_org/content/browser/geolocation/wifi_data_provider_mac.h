// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GEOLOCATION_WIFI_DATA_PROVIDER_MAC_H_
#define CONTENT_BROWSER_GEOLOCATION_WIFI_DATA_PROVIDER_MAC_H_

#include "content/browser/geolocation/wifi_data_provider_common.h"

namespace content {

class MacWifiDataProvider : public WifiDataProviderCommon {
 public:
  MacWifiDataProvider();

 private:
  virtual ~MacWifiDataProvider();

  
  virtual WlanApiInterface* NewWlanApi() OVERRIDE;
  virtual WifiPollingPolicy* NewPollingPolicy() OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(MacWifiDataProvider);
};

WifiDataProviderCommon::WlanApiInterface* NewCoreWlanApi();

} 

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GEOLOCATION_WIFI_DATA_PROVIDER_COMMON_H_
#define CONTENT_BROWSER_GEOLOCATION_WIFI_DATA_PROVIDER_COMMON_H_

#include <assert.h>

#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "content/browser/geolocation/wifi_data_provider.h"
#include "content/browser/geolocation/wifi_polling_policy.h"
#include "content/common/content_export.h"

namespace content {

base::string16 MacAddressAsString16(const uint8 mac_as_int[6]);

class CONTENT_EXPORT WifiDataProviderCommon : public WifiDataProviderImplBase {
 public:
  
  
  class WlanApiInterface {
   public:
    virtual ~WlanApiInterface() {}
    
    virtual bool GetAccessPointData(WifiData::AccessPointDataSet* data) = 0;
  };

  WifiDataProviderCommon();

  
  virtual void StartDataProvider() OVERRIDE;
  virtual void StopDataProvider() OVERRIDE;
  virtual bool GetData(WifiData* data) OVERRIDE;

 protected:
  virtual ~WifiDataProviderCommon();

  
  virtual WlanApiInterface* NewWlanApi() = 0;

  
  virtual WifiPollingPolicy* NewPollingPolicy() = 0;

 private:
  
  void DoWifiScanTask();

  
  void ScheduleNextScan(int interval);

  WifiData wifi_data_;

  
  bool is_first_scan_complete_;

  
  scoped_ptr<WlanApiInterface> wlan_api_;

  
  scoped_ptr<WifiPollingPolicy> polling_policy_;

  
  base::WeakPtrFactory<WifiDataProviderCommon> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(WifiDataProviderCommon);
};

}  

#endif  

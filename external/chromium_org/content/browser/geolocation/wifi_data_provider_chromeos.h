// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GEOLOCATION_WIFI_DATA_PROVIDER_CHROMEOS_H_
#define CONTENT_BROWSER_GEOLOCATION_WIFI_DATA_PROVIDER_CHROMEOS_H_

#include "base/compiler_specific.h"
#include "content/browser/geolocation/wifi_data_provider.h"
#include "content/browser/geolocation/wifi_polling_policy.h"

namespace content {

class CONTENT_EXPORT WifiDataProviderChromeOs
    : public WifiDataProviderImplBase {
 public:
  WifiDataProviderChromeOs();

  
  virtual void StartDataProvider() OVERRIDE;
  virtual void StopDataProvider() OVERRIDE;
  virtual bool GetData(WifiData* data) OVERRIDE;

 private:
  friend class GeolocationChromeOsWifiDataProviderTest;
  virtual ~WifiDataProviderChromeOs();

  
  void DoWifiScanTaskOnUIThread();  
  void DoStartTaskOnUIThread();

  
  void DidWifiScanTaskNoResults();
  void DidWifiScanTask(const WifiData& new_data);

  
  void ScheduleNextScan(int interval);

  
  void ScheduleStart();

  
  void ScheduleStop();

  
  bool GetAccessPointData(WifiData::AccessPointDataSet* data);

  
  scoped_ptr<WifiPollingPolicy> polling_policy_;

  
  WifiData wifi_data_;

  
  bool started_;

  
  bool is_first_scan_complete_;

  DISALLOW_COPY_AND_ASSIGN(WifiDataProviderChromeOs);
};

}  

#endif  

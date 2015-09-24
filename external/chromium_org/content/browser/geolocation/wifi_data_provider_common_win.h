// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GEOLOCATION_WIFI_DATA_PROVIDER_COMMON_WIN_H_
#define CONTENT_BROWSER_GEOLOCATION_WIFI_DATA_PROVIDER_COMMON_WIN_H_

#include <windows.h>
#include <ntddndis.h>

#include "content/browser/geolocation/wifi_data_provider.h"

namespace content {

int GetDataFromBssIdList(const NDIS_802_11_BSSID_LIST& bss_id_list,
                         int list_size,
                         WifiData::AccessPointDataSet* data);

}  

#endif  

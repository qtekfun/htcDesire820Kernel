// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GEOLOCATION_WIFI_DATA_H_
#define CONTENT_BROWSER_GEOLOCATION_WIFI_DATA_H_

#include <set>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "content/common/content_export.h"

namespace content {

struct CONTENT_EXPORT AccessPointData {
  AccessPointData();
  ~AccessPointData();

  
  base::string16 mac_address;
  int radio_signal_strength;  
  int channel;
  int signal_to_noise;  
  base::string16 ssid;   
};

struct AccessPointDataLess {
  bool operator()(const AccessPointData& data1,
                  const AccessPointData& data2) const {
    return data1.mac_address < data2.mac_address;
  }
};

struct CONTENT_EXPORT WifiData {
  WifiData();
  ~WifiData();

  
  bool DiffersSignificantly(const WifiData& other) const;

  
  
  typedef std::set<AccessPointData, AccessPointDataLess> AccessPointDataSet;
  AccessPointDataSet access_point_data;
};

}  

#endif  

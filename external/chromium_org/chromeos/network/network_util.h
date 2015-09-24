// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_NETWORK_UTIL_H_
#define CHROMEOS_NETWORK_NETWORK_UTIL_H_


#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/time/time.h"
#include "base/values.h"
#include "chromeos/chromeos_export.h"

namespace chromeos {

struct CHROMEOS_EXPORT WifiAccessPoint {
  WifiAccessPoint();
  ~WifiAccessPoint();
  std::string ssid;  
  std::string mac_address;  
  base::Time timestamp;  
  int signal_strength;  
  int signal_to_noise;  
  int channel;  
};

struct CHROMEOS_EXPORT CellularScanResult {
  CellularScanResult();
  ~CellularScanResult();
  std::string status;  
                       
  std::string network_id;  
  std::string short_name;  
  std::string long_name;  
  std::string technology;  
};

typedef std::vector<WifiAccessPoint> WifiAccessPointVector;

enum NetworkMethodErrorType {
  NETWORK_METHOD_ERROR_NONE = 0,
  NETWORK_METHOD_ERROR_LOCAL = 1,
  NETWORK_METHOD_ERROR_REMOTE = 2,
};

typedef base::Callback<void(
    const std::string& path,
    NetworkMethodErrorType error,
    const std::string& error_message)> NetworkOperationCallback;

namespace network_util {

CHROMEOS_EXPORT std::string PrefixLengthToNetmask(int32 prefix_length);

CHROMEOS_EXPORT int32 NetmaskToPrefixLength(const std::string& netmask);

CHROMEOS_EXPORT bool ParseCellularScanResults(
    const ListValue& list, std::vector<CellularScanResult>* scan_results);

}  
}  

#endif  

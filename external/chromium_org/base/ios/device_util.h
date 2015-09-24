// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_IOS_DEVICE_UTIL_H_
#define BASE_IOS_DEVICE_UTIL_H_

#include <string>

namespace ios {
namespace device_util {

std::string GetPlatform();

bool IsRunningOnHighRamDevice();

bool IsSingleCoreDevice();

std::string GetMacAddress(const std::string& interface_name);

std::string GetRandomId();

std::string GetDeviceIdentifier(const char* salt);

}  
}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_CHROMEOS_NETWORK_NETWORK_CONNECT_H
#define ASH_SYSTEM_CHROMEOS_NETWORK_NETWORK_CONNECT_H

#include <string>

#include "ash/ash_export.h"
#include "base/strings/string16.h"
#include "ui/gfx/native_widget_types.h"  

namespace base {
class DictionaryValue;
}

namespace chromeos {
class NetworkTypePattern;
}

namespace ash {
namespace network_connect {

ASH_EXPORT extern const char kNetworkConnectNotificationId[];
ASH_EXPORT extern const char kNetworkActivateNotificationId[];

ASH_EXPORT extern const char kErrorActivateFailed[];

ASH_EXPORT void ConnectToNetwork(const std::string& service_path,
                                 gfx::NativeWindow parent_window);

ASH_EXPORT void SetTechnologyEnabled(
    const chromeos::NetworkTypePattern& technology,
    bool enabled_state);

ASH_EXPORT void ActivateCellular(const std::string& service_path);

ASH_EXPORT void ShowMobileSetup(const std::string& service_path);

ASH_EXPORT void ConfigureNetworkAndConnect(
    const std::string& service_path,
    const base::DictionaryValue& properties,
    bool shared);

ASH_EXPORT void CreateConfigurationAndConnect(base::DictionaryValue* properties,
                                              bool shared);

ASH_EXPORT base::string16 ErrorString(const std::string& error,
                                      const std::string& service_path);

ASH_EXPORT void ShowNetworkSettings(const std::string& service_path);

}  
}  

#endif  

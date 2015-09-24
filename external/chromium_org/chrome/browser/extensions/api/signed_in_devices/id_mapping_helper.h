// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_SIGNED_IN_DEVICES_ID_MAPPING_HELPER_H__
#define CHROME_BROWSER_EXTENSIONS_API_SIGNED_IN_DEVICES_ID_MAPPING_HELPER_H__

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"

namespace base {
class DictionaryValue;
}  

namespace browser_sync {
class DeviceInfo;
}  

class Profile;

namespace extensions {


std::string GetPublicIdFromGUID(
    const base::DictionaryValue& id_mapping,
    const std::string& guid);

std::string GetGUIDFromPublicId(
    const base::DictionaryValue& id_mapping,
    const std::string& id);

void CreateMappingForUnmappedDevices(
    std::vector<browser_sync::DeviceInfo*>* device_info,
    base::DictionaryValue* value);

scoped_ptr<browser_sync::DeviceInfo> GetDeviceInfoForClientId(
    const std::string& client_id,
    const std::string& extension_id,
    Profile* profile);

}  

#endif  

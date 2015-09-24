// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_EXTENSION_SYNC_H_
#define CHROME_BROWSER_SYNC_GLUE_EXTENSION_SYNC_H_
#pragma once


#include <map>
#include <string>

class Extension;
class ExtensionServiceInterface;
class Profile;
class ProfileSyncService;

namespace sync_api {
struct UserShare;
}  

namespace sync_pb {
class ExtensionSpecifics;
}  

namespace browser_sync {

class ExtensionData;
struct ExtensionSyncTraits;

typedef std::map<std::string, ExtensionData> ExtensionDataMap;

bool RootNodeHasChildren(const char* tag,
                         sync_api::UserShare* user_share,
                         bool* has_children);

bool SlurpExtensionData(const ExtensionSyncTraits& traits,
                        const ExtensionServiceInterface& extensions_service,
                        sync_api::UserShare* user_share,
                        ExtensionDataMap* extension_data_map);

bool FlushExtensionData(const ExtensionSyncTraits& traits,
                        const ExtensionDataMap& extension_data_map,
                        ExtensionServiceInterface* extensions_service,
                        sync_api::UserShare* user_share);

bool UpdateServerData(const ExtensionSyncTraits& traits,
                      const Extension& extension,
                      const ExtensionServiceInterface& extensions_service,
                      sync_api::UserShare* user_share,
                      std::string* error);

void RemoveServerData(const ExtensionSyncTraits& traits,
                      const std::string& id,
                      sync_api::UserShare* user_share);

}  

#endif  

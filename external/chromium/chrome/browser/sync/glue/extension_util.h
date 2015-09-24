// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_EXTENSION_UTIL_H_
#define CHROME_BROWSER_SYNC_GLUE_EXTENSION_UTIL_H_
#pragma once


#include <string>

class Extension;
class ExtensionPrefs;
class ExtensionServiceInterface;
struct ExtensionSyncData;
struct UninstalledExtensionInfo;

namespace sync_pb {
class ExtensionSpecifics;
}  

namespace browser_sync {

bool IsExtensionValid(const Extension& extension);

std::string ExtensionSpecificsToString(
    const sync_pb::ExtensionSpecifics& specifics);

bool IsExtensionSpecificsValid(
    const sync_pb::ExtensionSpecifics& specifics);

void DcheckIsExtensionSpecificsValid(
    const sync_pb::ExtensionSpecifics& specifics);

bool AreExtensionSpecificsEqual(const sync_pb::ExtensionSpecifics& a,
                                const sync_pb::ExtensionSpecifics& b);

bool IsExtensionSpecificsUnset(
    const sync_pb::ExtensionSpecifics& specifics);

void CopyUserProperties(
    const sync_pb::ExtensionSpecifics& specifics,
    sync_pb::ExtensionSpecifics* dest_specifics);

void CopyNonUserProperties(
    const sync_pb::ExtensionSpecifics& specifics,
    sync_pb::ExtensionSpecifics* dest_specifics);

bool AreExtensionSpecificsUserPropertiesEqual(
    const sync_pb::ExtensionSpecifics& a,
    const sync_pb::ExtensionSpecifics& b);

bool AreExtensionSpecificsNonUserPropertiesEqual(
    const sync_pb::ExtensionSpecifics& a,
    const sync_pb::ExtensionSpecifics& b);

void GetExtensionSpecifics(const Extension& extension,
                           const ExtensionServiceInterface& extension_service,
                           sync_pb::ExtensionSpecifics* specifics);

void MergeExtensionSpecifics(
    const sync_pb::ExtensionSpecifics& specifics,
    bool merge_user_properties,
    sync_pb::ExtensionSpecifics* merged_specifics);

bool GetExtensionSyncData(
    const sync_pb::ExtensionSpecifics& specifics,
    ExtensionSyncData* sync_data);

}  

#endif  

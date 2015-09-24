// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_SYNC_FILE_SYSTEM_SYNC_FILE_SYSTEM_API_HELPERS_H_
#define CHROME_BROWSER_EXTENSIONS_API_SYNC_FILE_SYSTEM_SYNC_FILE_SYSTEM_API_HELPERS_H_

#include "chrome/browser/sync_file_system/conflict_resolution_policy.h"
#include "chrome/browser/sync_file_system/sync_action.h"
#include "chrome/browser/sync_file_system/sync_direction.h"
#include "chrome/browser/sync_file_system/sync_file_status.h"
#include "chrome/browser/sync_file_system/sync_file_type.h"
#include "chrome/browser/sync_file_system/sync_service_state.h"
#include "chrome/common/extensions/api/sync_file_system.h"

namespace fileapi {
class FileSystemURL;
}

namespace base {
class DictionaryValue;
}

namespace extensions {

api::sync_file_system::ServiceStatus SyncServiceStateToExtensionEnum(
    sync_file_system::SyncServiceState state);

api::sync_file_system::FileStatus SyncFileStatusToExtensionEnum(
    sync_file_system::SyncFileStatus status);

api::sync_file_system::SyncAction SyncActionToExtensionEnum(
    sync_file_system::SyncAction action);

api::sync_file_system::SyncDirection SyncDirectionToExtensionEnum(
    sync_file_system::SyncDirection direction);

api::sync_file_system::ConflictResolutionPolicy
ConflictResolutionPolicyToExtensionEnum(
    sync_file_system::ConflictResolutionPolicy policy);

sync_file_system::ConflictResolutionPolicy
ExtensionEnumToConflictResolutionPolicy(
    api::sync_file_system::ConflictResolutionPolicy);

base::DictionaryValue* CreateDictionaryValueForFileSystemEntry(
    const fileapi::FileSystemURL& url,
    sync_file_system::SyncFileType file_type);

}  

#endif  

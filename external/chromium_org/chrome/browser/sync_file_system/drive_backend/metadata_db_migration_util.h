// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_DRIVE_BACKEND_METADATA_DB_MIGRATION_UTIL_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_DRIVE_BACKEND_METADATA_DB_MIGRATION_UTIL_H_

#include <string>

#include "chrome/browser/sync_file_system/sync_file_system.pb.h"
#include "chrome/browser/sync_file_system/sync_status_code.h"
#include "third_party/leveldatabase/src/include/leveldb/db.h"

class GURL;

namespace sync_file_system {
namespace drive_backend {

bool ParseV0FormatFileSystemURL(const GURL& url,
                                GURL* origin,
                                base::FilePath* path);

std::string AddWapiFilePrefix(const std::string& resource_id);

std::string AddWapiFolderPrefix(const std::string& resource_id);

std::string AddWapiIdPrefix(const std::string& resource_id,
                            DriveMetadata_ResourceType type);

std::string RemoveWapiIdPrefix(const std::string& resource_id);

SyncStatusCode MigrateDatabaseFromV0ToV1(leveldb::DB* db);

SyncStatusCode MigrateDatabaseFromV1ToV2(leveldb::DB* db);

}  
}  

#endif  

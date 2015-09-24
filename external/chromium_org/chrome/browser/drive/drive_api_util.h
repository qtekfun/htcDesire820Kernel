// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DRIVE_DRIVE_API_UTIL_H_
#define CHROME_BROWSER_DRIVE_DRIVE_API_UTIL_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/drive/drive_service_interface.h"
#include "google_apis/drive/drive_common_callbacks.h"
#include "google_apis/drive/drive_entry_kinds.h"
#include "google_apis/drive/gdata_errorcode.h"

class GURL;

namespace base {
class FilePath;
class Value;
}  

namespace google_apis {
class AccountMetadata;
class AppIcon;
class AppList;
class AppResource;
class ChangeList;
class ChangeResource;
class DriveAppIcon;
class FileList;
class FileResource;
class InstalledApp;
class ResourceEntry;
class ResourceList;
}  

namespace drive {
namespace util {

bool IsDriveV2ApiEnabled();

std::string EscapeQueryStringValue(const std::string& str);

std::string TranslateQuery(const std::string& original_query);

std::string ExtractResourceIdFromUrl(const GURL& url);

std::string CanonicalizeResourceId(const std::string& resource_id);

ResourceIdCanonicalizer GetIdentityResourceIdCanonicalizer();


extern const char kDocsListScope[];
extern const char kDriveAppsScope[];

void ParseShareUrlAndRun(const google_apis::GetShareUrlCallback& callback,
                         google_apis::GDataErrorCode error,
                         scoped_ptr<base::Value> value);

scoped_ptr<google_apis::AboutResource>
ConvertAccountMetadataToAboutResource(
    const google_apis::AccountMetadata& account_metadata,
    const std::string& root_resource_id);

scoped_ptr<google_apis::AppList>
ConvertAccountMetadataToAppList(
    const google_apis::AccountMetadata& account_metadata);

scoped_ptr<google_apis::FileResource>
ConvertResourceEntryToFileResource(const google_apis::ResourceEntry& entry);

google_apis::DriveEntryKind GetKind(
    const google_apis::FileResource& file_resource);

scoped_ptr<google_apis::ResourceEntry>
ConvertFileResourceToResourceEntry(
    const google_apis::FileResource& file_resource);

scoped_ptr<google_apis::ResourceEntry>
ConvertChangeResourceToResourceEntry(
    const google_apis::ChangeResource& change_resource);

scoped_ptr<google_apis::ResourceList>
ConvertFileListToResourceList(const google_apis::FileList& file_list);

scoped_ptr<google_apis::ResourceList>
ConvertChangeListToResourceList(const google_apis::ChangeList& change_list);

std::string GetMd5Digest(const base::FilePath& file_path);

extern const char kWapiRootDirectoryResourceId[];

}  
}  

#endif  

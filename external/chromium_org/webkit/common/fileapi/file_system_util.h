// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_COMMON_FILEAPI_FILE_SYSTEM_UTIL_H_
#define WEBKIT_COMMON_FILEAPI_FILE_SYSTEM_UTIL_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/platform_file.h"
#include "third_party/WebKit/public/platform/WebFileError.h"
#include "third_party/WebKit/public/platform/WebFileSystemType.h"
#include "webkit/common/fileapi/file_system_info.h"
#include "webkit/common/fileapi/file_system_types.h"
#include "webkit/common/quota/quota_types.h"
#include "webkit/common/webkit_storage_common_export.h"

class GURL;

namespace fileapi {

WEBKIT_STORAGE_COMMON_EXPORT extern const char kPersistentDir[];
WEBKIT_STORAGE_COMMON_EXPORT extern const char kTemporaryDir[];
WEBKIT_STORAGE_COMMON_EXPORT extern const char kExternalDir[];
WEBKIT_STORAGE_COMMON_EXPORT extern const char kIsolatedDir[];
WEBKIT_STORAGE_COMMON_EXPORT extern const char kTestDir[];

class WEBKIT_STORAGE_COMMON_EXPORT VirtualPath {
 public:
  static const base::FilePath::CharType kRoot[];
  static const base::FilePath::CharType kSeparator;

  
  
  
  
  static base::FilePath BaseName(const base::FilePath& virtual_path);

  
  
  static base::FilePath DirName(const base::FilePath& virtual_path);

  
  
  
  
  static void GetComponents(
      const base::FilePath& path,
      std::vector<base::FilePath::StringType>* components);

  static void GetComponentsUTF8Unsafe(
      const base::FilePath& path, std::vector<std::string>* components);

  
  
  static base::FilePath::StringType GetNormalizedFilePath(
      const base::FilePath& path);

  
  static bool IsAbsolute(const base::FilePath::StringType& path);

  
  static bool IsRootPath(const base::FilePath& path);
};

WEBKIT_STORAGE_COMMON_EXPORT GURL GetFileSystemRootURI(const GURL& origin_url,
                                                       FileSystemType type);

WEBKIT_STORAGE_COMMON_EXPORT std::string
GetFileSystemName(const GURL& origin_url, FileSystemType type);

WEBKIT_STORAGE_COMMON_EXPORT FileSystemType
QuotaStorageTypeToFileSystemType(quota::StorageType storage_type);

WEBKIT_STORAGE_COMMON_EXPORT quota::StorageType
FileSystemTypeToQuotaStorageType(FileSystemType type);

WEBKIT_STORAGE_COMMON_EXPORT std::string
GetFileSystemTypeString(FileSystemType type);

WEBKIT_STORAGE_COMMON_EXPORT bool GetFileSystemPublicType(
    std::string type_string,
    blink::WebFileSystemType* type);

WEBKIT_STORAGE_COMMON_EXPORT std::string FilePathToString(
    const base::FilePath& file_path);

WEBKIT_STORAGE_COMMON_EXPORT base::FilePath StringToFilePath(
    const std::string& file_path_string);

WEBKIT_STORAGE_COMMON_EXPORT blink::WebFileError
PlatformFileErrorToWebFileError(base::PlatformFileError error_code);

WEBKIT_STORAGE_COMMON_EXPORT std::string GetIsolatedFileSystemName(
    const GURL& origin_url,
    const std::string& filesystem_id);

WEBKIT_STORAGE_COMMON_EXPORT bool CrackIsolatedFileSystemName(
    const std::string& filesystem_name,
    std::string* filesystem_id);

WEBKIT_STORAGE_COMMON_EXPORT bool ValidateIsolatedFileSystemId(
    const std::string& filesystem_id);

WEBKIT_STORAGE_COMMON_EXPORT std::string GetIsolatedFileSystemRootURIString(
    const GURL& origin_url,
    const std::string& filesystem_id,
    const std::string& optional_root_name);

WEBKIT_STORAGE_COMMON_EXPORT std::string GetExternalFileSystemRootURIString(
    const GURL& origin_url,
    const std::string& mount_name);

WEBKIT_STORAGE_COMMON_EXPORT base::PlatformFileError
NetErrorToPlatformFileError(int error);

#if defined(OS_CHROMEOS)
WEBKIT_STORAGE_COMMON_EXPORT FileSystemInfo
GetFileSystemInfoForChromeOS(const GURL& origin_url);
#endif

}  

#endif  

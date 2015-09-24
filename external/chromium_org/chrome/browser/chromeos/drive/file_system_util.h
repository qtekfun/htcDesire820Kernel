// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_UTIL_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_UTIL_H_

#include <string>

#include "base/callback_forward.h"
#include "chrome/browser/chromeos/drive/file_errors.h"
#include "url/gurl.h"

class Profile;

namespace base {
class FilePath;
}

namespace fileapi {
class FileSystemURL;
}

namespace drive {

class DriveAppRegistry;
class DriveServiceInterface;
class FileSystemInterface;
class ResourceEntry;

namespace util {

const char kDriveGrandRootLocalId[] = "<drive>";

const char kDriveOtherDirLocalId[] = "<other>";

const char kDriveTrashDirLocalId[] = "<trash>";

const base::FilePath::CharType kDriveGrandRootDirName[] =
    FILE_PATH_LITERAL("drive");

const base::FilePath::CharType kDriveMyDriveRootDirName[] =
    FILE_PATH_LITERAL("root");

const base::FilePath::CharType kDriveOtherDirName[] =
    FILE_PATH_LITERAL("other");

const base::FilePath::CharType kDriveTrashDirName[] =
    FILE_PATH_LITERAL("trash");

const base::FilePath& GetDriveGrandRootPath();

const base::FilePath& GetDriveMyDriveRootPath();

const base::FilePath& GetDriveMountPointPath();

FileSystemInterface* GetFileSystemByProfile(Profile* profile);

FileSystemInterface* GetFileSystemByProfileId(void* profile_id);

DriveAppRegistry* GetDriveAppRegistryByProfile(Profile* profile);

DriveServiceInterface* GetDriveServiceByProfile(Profile* profile);

bool IsSpecialResourceId(const std::string& resource_id);

ResourceEntry CreateMyDriveRootEntry(const std::string& root_resource_id);

const std::string& GetDriveMountPointPathAsString();

GURL FilePathToDriveURL(const base::FilePath& path);

base::FilePath DriveURLToFilePath(const GURL& url);

void MaybeSetDriveURL(Profile* profile, const base::FilePath& path, GURL* url);

bool IsUnderDriveMountPoint(const base::FilePath& path);

bool NeedsNamespaceMigration(const base::FilePath& path);

base::FilePath ConvertToMyDriveNamespace(const base::FilePath& path);

base::FilePath ExtractDrivePath(const base::FilePath& path);

base::FilePath ExtractDrivePathFromFileSystemUrl(
    const fileapi::FileSystemURL& url);

std::string EscapeCacheFileName(const std::string& filename);

std::string UnescapeCacheFileName(const std::string& filename);

std::string NormalizeFileName(const std::string& input);

base::FilePath GetCacheRootPath(Profile* profile);

typedef base::Callback<void (FileError, const base::FilePath& path)>
    PrepareWritableFileCallback;

// After |callback| returns, the written content is synchronized to the server.
void PrepareWritableFileAndRun(Profile* profile,
                               const base::FilePath& path,
                               const PrepareWritableFileCallback& callback);

void EnsureDirectoryExists(Profile* profile,
                           const base::FilePath& directory,
                           const FileOperationCallback& callback);

void EmptyFileOperationCallback(FileError error);

struct DestroyHelper {
  template<typename T>
  void operator()(T* object) const {
    if (object)
      object->Destroy();
  }
};

bool CreateGDocFile(const base::FilePath& file_path,
                    const GURL& url,
                    const std::string& resource_id);

bool HasGDocFileExtension(const base::FilePath& file_path);

GURL ReadUrlFromGDocFile(const base::FilePath& file_path);

std::string ReadResourceIdFromGDocFile(const base::FilePath& file_path);

bool IsDriveEnabledForProfile(Profile* profile);

enum ConnectionStatusType {
  
  
  DRIVE_DISCONNECTED_NOSERVICE,
  
  DRIVE_DISCONNECTED_NONETWORK,
  
  DRIVE_DISCONNECTED_NOTREADY,
  
  DRIVE_CONNECTED_METERED,
  
  
  DRIVE_CONNECTED,
};

ConnectionStatusType GetDriveConnectionStatus(Profile* profile);

}  
}  

#endif  

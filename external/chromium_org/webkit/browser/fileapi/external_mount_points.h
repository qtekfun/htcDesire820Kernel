// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_EXTERNAL_MOUNT_POINTS_H_
#define WEBKIT_BROWSER_FILEAPI_EXTERNAL_MOUNT_POINTS_H_

#include <map>
#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "webkit/browser/fileapi/mount_points.h"
#include "webkit/browser/webkit_storage_browser_export.h"
#include "webkit/common/fileapi/file_system_mount_option.h"
#include "webkit/common/fileapi/file_system_types.h"

namespace base {
class FilePath;
}

namespace fileapi {
class FileSystemURL;
}

namespace fileapi {

class WEBKIT_STORAGE_BROWSER_EXPORT ExternalMountPoints
    : public base::RefCountedThreadSafe<ExternalMountPoints>,
      public MountPoints {
 public:
  static ExternalMountPoints* GetSystemInstance();
  static scoped_refptr<ExternalMountPoints> CreateRefCounted();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool RegisterFileSystem(const std::string& mount_name,
                          FileSystemType type,
                          const FileSystemMountOption& mount_option,
                          const base::FilePath& path);

  
  virtual bool HandlesFileSystemMountType(FileSystemType type) const OVERRIDE;
  virtual bool RevokeFileSystem(const std::string& mount_name) OVERRIDE;
  virtual bool GetRegisteredPath(const std::string& mount_name,
                                 base::FilePath* path) const OVERRIDE;
  virtual bool CrackVirtualPath(
      const base::FilePath& virtual_path,
      std::string* mount_name,
      FileSystemType* type,
      base::FilePath* path,
      FileSystemMountOption* mount_option) const OVERRIDE;
  virtual FileSystemURL CrackURL(const GURL& url) const OVERRIDE;
  virtual FileSystemURL CreateCrackedFileSystemURL(
      const GURL& origin,
      FileSystemType type,
      const base::FilePath& path) const OVERRIDE;

  
  void AddMountPointInfosTo(std::vector<MountPointInfo>* mount_points) const;

  
  
  
  
  
  
  
  
  
  bool GetVirtualPath(const base::FilePath& absolute_path,
                      base::FilePath* virtual_path) const;

  
  base::FilePath CreateVirtualRootPath(const std::string& mount_name) const;

  FileSystemURL CreateExternalFileSystemURL(
      const GURL& origin,
      const std::string& mount_name,
      const base::FilePath& path) const;

  
  void RevokeAllFileSystems();

 private:
  friend class base::RefCountedThreadSafe<ExternalMountPoints>;

  
  class Instance;

  typedef std::map<std::string, Instance*> NameToInstance;

  
  typedef std::map<base::FilePath, std::string> PathToName;

  
  ExternalMountPoints();
  virtual ~ExternalMountPoints();

  
  virtual FileSystemURL CrackFileSystemURL(
      const FileSystemURL& url) const OVERRIDE;

  
  
  
  
  
  
  
  
  bool ValidateNewMountPoint(const std::string& mount_name,
                             const base::FilePath& path);

  
  mutable base::Lock lock_;

  NameToInstance instance_map_;
  PathToName path_to_name_map_;

  DISALLOW_COPY_AND_ASSIGN(ExternalMountPoints);
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_ISOLATED_CONTEXT_H_
#define WEBKIT_BROWSER_FILEAPI_ISOLATED_CONTEXT_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/lazy_instance.h"
#include "base/memory/singleton.h"
#include "base/synchronization/lock.h"
#include "webkit/browser/fileapi/mount_points.h"
#include "webkit/browser/webkit_storage_browser_export.h"
#include "webkit/common/fileapi/file_system_types.h"

namespace fileapi {
class FileSystemURL;
}

namespace fileapi {

class WEBKIT_STORAGE_BROWSER_EXPORT IsolatedContext : public MountPoints {
 public:
  class WEBKIT_STORAGE_BROWSER_EXPORT FileInfoSet {
   public:
    FileInfoSet();
    ~FileInfoSet();

    
    
    
    
    bool AddPath(const base::FilePath& path, std::string* registered_name);

    
    
    
    bool AddPathWithName(const base::FilePath& path, const std::string& name);

    const std::set<MountPointInfo>& fileset() const { return fileset_; }

   private:
    std::set<MountPointInfo> fileset_;
  };

  
  static IsolatedContext* GetInstance();

  
  
  
  static bool IsIsolatedType(FileSystemType type);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  std::string RegisterDraggedFileSystem(const FileInfoSet& files);

  
  
  
  
  
  
  std::string RegisterFileSystemForPath(FileSystemType type,
                                        const base::FilePath& path,
                                        std::string* register_name);

  
  
  
  
  
  std::string RegisterFileSystemForVirtualPath(
      FileSystemType type,
      const std::string& register_name,
      const base::FilePath& cracked_path_prefix);

  
  
  
  
  
  
  
  
  void RevokeFileSystemByPath(const base::FilePath& path);

  
  void AddReference(const std::string& filesystem_id);

  
  
  
  
  void RemoveReference(const std::string& filesystem_id);

  
  
  
  
  
  bool GetDraggedFileInfo(const std::string& filesystem_id,
                          std::vector<MountPointInfo>* files) const;

  
  virtual bool HandlesFileSystemMountType(FileSystemType type) const OVERRIDE;
  virtual bool RevokeFileSystem(const std::string& filesystem_id) OVERRIDE;
  virtual bool GetRegisteredPath(const std::string& filesystem_id,
                                 base::FilePath* path) const OVERRIDE;
  virtual bool CrackVirtualPath(
      const base::FilePath& virtual_path,
      std::string* filesystem_id,
      FileSystemType* type,
      base::FilePath* path,
      FileSystemMountOption* mount_option) const OVERRIDE;
  virtual FileSystemURL CrackURL(const GURL& url) const OVERRIDE;
  virtual FileSystemURL CreateCrackedFileSystemURL(
      const GURL& origin,
      FileSystemType type,
      const base::FilePath& path) const OVERRIDE;

  
  base::FilePath CreateVirtualRootPath(const std::string& filesystem_id) const;

 private:
  friend struct base::DefaultLazyInstanceTraits<IsolatedContext>;

  
  class Instance;

  typedef std::map<std::string, Instance*> IDToInstance;

  
  typedef std::map<base::FilePath, std::set<std::string> > PathToID;

  
  IsolatedContext();
  virtual ~IsolatedContext();

  
  virtual FileSystemURL CrackFileSystemURL(
      const FileSystemURL& url) const OVERRIDE;

  
  
  bool UnregisterFileSystem(const std::string& filesystem_id);

  
  std::string GetNewFileSystemId() const;

  
  mutable base::Lock lock_;

  IDToInstance instance_map_;
  PathToID path_to_id_map_;

  DISALLOW_COPY_AND_ASSIGN(IsolatedContext);
};

}  

#endif  

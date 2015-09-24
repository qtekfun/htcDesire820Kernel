// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_FILE_SYSTEM_URL_H_
#define WEBKIT_BROWSER_FILEAPI_FILE_SYSTEM_URL_H_

#include <set>
#include <string>

#include "base/platform_file.h"
#include "url/gurl.h"
#include "webkit/browser/webkit_storage_browser_export.h"
#include "webkit/common/fileapi/file_system_mount_option.h"
#include "webkit/common/fileapi/file_system_types.h"

namespace fileapi {

class WEBKIT_STORAGE_BROWSER_EXPORT FileSystemURL {
 public:
  FileSystemURL();
  ~FileSystemURL();

  
  
  static FileSystemURL CreateForTest(const GURL& url);
  static FileSystemURL CreateForTest(const GURL& origin,
                                     FileSystemType mount_type,
                                     const base::FilePath& virtual_path);

  
  static bool ParseFileSystemSchemeURL(const GURL& url,
                                       GURL* origin,
                                       FileSystemType* mount_type,
                                       base::FilePath* virtual_path);

  
  bool is_valid() const { return is_valid_; }

  
  const GURL& origin() const { return origin_; }

  
  FileSystemType type() const { return type_; }

  
  const base::FilePath& path() const { return path_; }

  
  
  
  const base::FilePath& virtual_path() const { return virtual_path_; }

  
  
  const std::string& filesystem_id() const { return filesystem_id_; }
  const std::string& mount_filesystem_id() const {
    return mount_filesystem_id_;
  }

  FileSystemType mount_type() const { return mount_type_; }

  const FileSystemMountOption& mount_option() const { return mount_option_; }

  
  GURL ToGURL() const;

  std::string DebugString() const;

  
  bool IsParent(const FileSystemURL& child) const;

  bool IsInSameFileSystem(const FileSystemURL& other) const;

  bool operator==(const FileSystemURL& that) const;

  bool operator!=(const FileSystemURL& that) const {
    return !(*this == that);
  }

  struct WEBKIT_STORAGE_BROWSER_EXPORT Comparator {
    bool operator() (const FileSystemURL& lhs, const FileSystemURL& rhs) const;
  };

 private:
  friend class FileSystemContext;
  friend class ExternalMountPoints;
  friend class IsolatedContext;

  explicit FileSystemURL(const GURL& filesystem_url);
  FileSystemURL(const GURL& origin,
                FileSystemType mount_type,
                const base::FilePath& virtual_path);
  
  FileSystemURL(const GURL& origin,
                FileSystemType mount_type,
                const base::FilePath& virtual_path,
                const std::string& mount_filesystem_id,
                FileSystemType cracked_type,
                const base::FilePath& cracked_path,
                const std::string& filesystem_id,
                const FileSystemMountOption& mount_option);

  bool is_valid_;

  
  GURL origin_;
  FileSystemType mount_type_;
  base::FilePath virtual_path_;

  
  
  
  
  
  std::string mount_filesystem_id_;
  FileSystemType type_;
  base::FilePath path_;
  std::string filesystem_id_;
  FileSystemMountOption mount_option_;
};

typedef std::set<FileSystemURL, FileSystemURL::Comparator> FileSystemURLSet;

}  

#endif  

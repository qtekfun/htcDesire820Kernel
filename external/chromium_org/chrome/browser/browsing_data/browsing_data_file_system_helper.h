// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSING_DATA_BROWSING_DATA_FILE_SYSTEM_HELPER_H_
#define CHROME_BROWSER_BROWSING_DATA_BROWSING_DATA_FILE_SYSTEM_HELPER_H_

#include <list>
#include <map>
#include <string>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "chrome/common/url_constants.h"
#include "url/gurl.h"
#include "webkit/common/fileapi/file_system_types.h"

namespace fileapi {
class FileSystemContext;
}

class Profile;

class BrowsingDataFileSystemHelper
    : public base::RefCountedThreadSafe<BrowsingDataFileSystemHelper> {
 public:
  
  
  struct FileSystemInfo {
    FileSystemInfo(const GURL& origin);
    ~FileSystemInfo();

    
    GURL origin;
    
    std::map<fileapi::FileSystemType, int64> usage_map;
  };

  
  
  
  
  
  
  
  static BrowsingDataFileSystemHelper* Create(
      fileapi::FileSystemContext* file_system_context);

  
  
  
  
  
  
  
  virtual void StartFetching(const base::Callback<
      void(const std::list<FileSystemInfo>&)>& callback) = 0;

  
  
  
  virtual void DeleteFileSystemOrigin(const GURL& origin) = 0;

 protected:
  friend class base::RefCountedThreadSafe<BrowsingDataFileSystemHelper>;

  BrowsingDataFileSystemHelper() {}
  virtual ~BrowsingDataFileSystemHelper() {}
};

class CannedBrowsingDataFileSystemHelper
    : public BrowsingDataFileSystemHelper {
 public:
  
  
  explicit CannedBrowsingDataFileSystemHelper(Profile* profile);

  
  
  
  
  
  
  
  
  CannedBrowsingDataFileSystemHelper* Clone();

  
  
  
  
  void AddFileSystem(const GURL& origin,
                     fileapi::FileSystemType type,
                     int64 size);

  
  void Reset();

  
  bool empty() const;

  
  size_t GetFileSystemCount() const;

  
  const std::list<FileSystemInfo>& GetFileSystemInfo() {
    return file_system_info_;
  }

  
  virtual void StartFetching(const base::Callback<
      void(const std::list<FileSystemInfo>&)>& callback) OVERRIDE;

  
  
  
  
  virtual void DeleteFileSystemOrigin(const GURL& origin) OVERRIDE {}

 private:
  
  CannedBrowsingDataFileSystemHelper();
  virtual ~CannedBrowsingDataFileSystemHelper();

  
  
  void NotifyOnUIThread();

  
  
  
  
  
  std::list<FileSystemInfo> file_system_info_;

  
  
  base::Callback<void(const std::list<FileSystemInfo>&)> completion_callback_;

  
  
  
  
  bool is_fetching_;

  DISALLOW_COPY_AND_ASSIGN(CannedBrowsingDataFileSystemHelper);
};

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_OBFUSCATED_FILE_UTIL_H_
#define WEBKIT_BROWSER_FILEAPI_OBFUSCATED_FILE_UTIL_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/files/file_util_proxy.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/platform_file.h"
#include "webkit/browser/fileapi/file_system_file_util.h"
#include "webkit/browser/fileapi/file_system_url.h"
#include "webkit/browser/fileapi/sandbox_directory_database.h"
#include "webkit/browser/fileapi/sandbox_file_system_backend_delegate.h"
#include "webkit/browser/webkit_storage_browser_export.h"
#include "webkit/common/blob/shareable_file_reference.h"
#include "webkit/common/fileapi/file_system_types.h"

namespace base {
class SequencedTaskRunner;
class TimeTicks;
}

namespace quota {
class SpecialStoragePolicy;
}

class GURL;

namespace fileapi {

class FileSystemOperationContext;
class SandboxOriginDatabaseInterface;
class TimedTaskHelper;

class WEBKIT_STORAGE_BROWSER_EXPORT_PRIVATE ObfuscatedFileUtil
    : public FileSystemFileUtil {
 public:
  
  
  class AbstractOriginEnumerator {
   public:
    virtual ~AbstractOriginEnumerator() {}

    
    virtual GURL Next() = 0;

    
    
    virtual bool HasTypeDirectory(const std::string& type_string) const = 0;
  };

  typedef base::Callback<std::string(const FileSystemURL&)>
      GetTypeStringForURLCallback;

  
  
  
  
  
  
  
  
  
  
  
  
  ObfuscatedFileUtil(
      quota::SpecialStoragePolicy* special_storage_policy,
      const base::FilePath& file_system_directory,
      base::SequencedTaskRunner* file_task_runner,
      const GetTypeStringForURLCallback& get_type_string_for_url,
      const std::set<std::string>& known_type_strings,
      SandboxFileSystemBackendDelegate* sandbox_delegate);
  virtual ~ObfuscatedFileUtil();

  
  virtual base::PlatformFileError CreateOrOpen(
      FileSystemOperationContext* context,
      const FileSystemURL& url,
      int file_flags,
      base::PlatformFile* file_handle,
      bool* created) OVERRIDE;
  virtual base::PlatformFileError Close(
      FileSystemOperationContext* context,
      base::PlatformFile file) OVERRIDE;
  virtual base::PlatformFileError EnsureFileExists(
      FileSystemOperationContext* context,
      const FileSystemURL& url, bool* created) OVERRIDE;
  virtual base::PlatformFileError CreateDirectory(
      FileSystemOperationContext* context,
      const FileSystemURL& url,
      bool exclusive,
      bool recursive) OVERRIDE;
  virtual base::PlatformFileError GetFileInfo(
      FileSystemOperationContext* context,
      const FileSystemURL& url,
      base::PlatformFileInfo* file_info,
      base::FilePath* platform_file) OVERRIDE;
  virtual scoped_ptr<AbstractFileEnumerator> CreateFileEnumerator(
      FileSystemOperationContext* context,
      const FileSystemURL& root_url) OVERRIDE;
  virtual base::PlatformFileError GetLocalFilePath(
      FileSystemOperationContext* context,
      const FileSystemURL& file_system_url,
      base::FilePath* local_path) OVERRIDE;
  virtual base::PlatformFileError Touch(
      FileSystemOperationContext* context,
      const FileSystemURL& url,
      const base::Time& last_access_time,
      const base::Time& last_modified_time) OVERRIDE;
  virtual base::PlatformFileError Truncate(
      FileSystemOperationContext* context,
      const FileSystemURL& url,
      int64 length) OVERRIDE;
  virtual base::PlatformFileError CopyOrMoveFile(
      FileSystemOperationContext* context,
      const FileSystemURL& src_url,
      const FileSystemURL& dest_url,
      CopyOrMoveOption option,
      bool copy) OVERRIDE;
  virtual base::PlatformFileError CopyInForeignFile(
        FileSystemOperationContext* context,
        const base::FilePath& src_file_path,
        const FileSystemURL& dest_url) OVERRIDE;
  virtual base::PlatformFileError DeleteFile(
      FileSystemOperationContext* context,
      const FileSystemURL& url) OVERRIDE;
  virtual base::PlatformFileError DeleteDirectory(
      FileSystemOperationContext* context,
      const FileSystemURL& url) OVERRIDE;
  virtual webkit_blob::ScopedFile CreateSnapshotFile(
      FileSystemOperationContext* context,
      const FileSystemURL& url,
      base::PlatformFileError* error,
      base::PlatformFileInfo* file_info,
      base::FilePath* platform_path) OVERRIDE;

  
  scoped_ptr<AbstractFileEnumerator> CreateFileEnumerator(
      FileSystemOperationContext* context,
      const FileSystemURL& root_url,
      bool recursive);

  
  bool IsDirectoryEmpty(
      FileSystemOperationContext* context,
      const FileSystemURL& url);

  
  
  
  
  
  
  
  
  
  base::FilePath GetDirectoryForOriginAndType(
      const GURL& origin,
      const std::string& type_string,
      bool create,
      base::PlatformFileError* error_code);

  
  
  
  bool DeleteDirectoryForOriginAndType(
      const GURL& origin,
      const std::string& type_string);

  
  
  
  AbstractOriginEnumerator* CreateOriginEnumerator();

  
  
  bool DestroyDirectoryDatabase(const GURL& origin,
                                const std::string& type_string);

  
  
  
  
  
  static int64 ComputeFilePathCost(const base::FilePath& path);

  
  
  
  
  void MaybePrepopulateDatabase(
      const std::vector<std::string>& type_strings_to_prepopulate);

 private:
  typedef SandboxDirectoryDatabase::FileId FileId;
  typedef SandboxDirectoryDatabase::FileInfo FileInfo;

  friend class ObfuscatedFileEnumerator;
  friend class ObfuscatedFileUtilTest;
  friend class QuotaBackendImplTest;
  FRIEND_TEST_ALL_PREFIXES(ObfuscatedFileUtilTest, MaybeDropDatabasesAliveCase);
  FRIEND_TEST_ALL_PREFIXES(ObfuscatedFileUtilTest,
                           MaybeDropDatabasesAlreadyDeletedCase);
  FRIEND_TEST_ALL_PREFIXES(ObfuscatedFileUtilTest,
                           DestroyDirectoryDatabase_Isolated);
  FRIEND_TEST_ALL_PREFIXES(ObfuscatedFileUtilTest,
                           GetDirectoryDatabase_Isolated);
  FRIEND_TEST_ALL_PREFIXES(ObfuscatedFileUtilTest,
                           MigrationBackFromIsolated);

  
  
  
  static ObfuscatedFileUtil* CreateForTesting(
      quota::SpecialStoragePolicy* special_storage_policy,
      const base::FilePath& file_system_directory,
      base::SequencedTaskRunner* file_task_runner);

  base::FilePath GetDirectoryForURL(
      const FileSystemURL& url,
      bool create,
      base::PlatformFileError* error_code);

  
  std::string CallGetTypeStringForURL(const FileSystemURL& url);

  base::PlatformFileError GetFileInfoInternal(
      SandboxDirectoryDatabase* db,
      FileSystemOperationContext* context,
      const FileSystemURL& url,
      FileId file_id,
      FileInfo* local_info,
      base::PlatformFileInfo* file_info,
      base::FilePath* platform_file_path);

  
  
  
  
  
  
  
  
  
  
  
  base::PlatformFileError CreateFile(
      FileSystemOperationContext* context,
      const base::FilePath& source_file_path,
      const FileSystemURL& dest_url,
      FileInfo* dest_file_info,
      int file_flags,
      base::PlatformFile* handle);

  
  
  
  base::FilePath DataPathToLocalPath(
      const FileSystemURL& url,
      const base::FilePath& data_file_path);

  std::string GetDirectoryDatabaseKey(const GURL& origin,
                                      const std::string& type_string);

  
  
  
  SandboxDirectoryDatabase* GetDirectoryDatabase(const FileSystemURL& url,
                                                 bool create);

  
  
  base::FilePath GetDirectoryForOrigin(const GURL& origin,
                                       bool create,
                                       base::PlatformFileError* error_code);

  void InvalidateUsageCache(FileSystemOperationContext* context,
                            const GURL& origin,
                            FileSystemType type);

  void MarkUsed();
  void DropDatabases();

  
  
  bool InitOriginDatabase(const GURL& origin_hint, bool create);

  base::PlatformFileError GenerateNewLocalPath(
      SandboxDirectoryDatabase* db,
      FileSystemOperationContext* context,
      const FileSystemURL& url,
      base::FilePath* local_path);

  base::PlatformFileError CreateOrOpenInternal(
      FileSystemOperationContext* context,
      const FileSystemURL& url,
      int file_flags,
      base::PlatformFile* file_handle,
      bool* created);

  bool HasIsolatedStorage(const GURL& origin);

  typedef std::map<std::string, SandboxDirectoryDatabase*> DirectoryMap;
  DirectoryMap directories_;
  scoped_ptr<SandboxOriginDatabaseInterface> origin_database_;
  scoped_refptr<quota::SpecialStoragePolicy> special_storage_policy_;
  base::FilePath file_system_directory_;

  
  int64 db_flush_delay_seconds_;

  scoped_refptr<base::SequencedTaskRunner> file_task_runner_;
  scoped_ptr<TimedTaskHelper> timer_;

  GetTypeStringForURLCallback get_type_string_for_url_;
  std::set<std::string> known_type_strings_;

  
  SandboxFileSystemBackendDelegate* sandbox_delegate_;

  DISALLOW_COPY_AND_ASSIGN(ObfuscatedFileUtil);
};

}  

#endif  

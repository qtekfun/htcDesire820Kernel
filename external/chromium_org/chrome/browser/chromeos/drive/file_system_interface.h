// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_INTERFACE_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_INTERFACE_H_

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/chromeos/drive/drive.pb.h"
#include "chrome/browser/chromeos/drive/file_system_metadata.h"
#include "chrome/browser/chromeos/drive/resource_metadata.h"
#include "google_apis/drive/base_requests.h"

namespace drive {

class FileSystemObserver;

struct SearchResultInfo {
  SearchResultInfo(const base::FilePath& path, bool is_directory)
      : path(path),
        is_directory(is_directory) {
  }

  base::FilePath path;
  bool is_directory;
};

struct MetadataSearchResult {
  MetadataSearchResult(const base::FilePath& in_path,
                       const ResourceEntry& in_entry,
                       const std::string& in_highlighted_base_name)
      : path(in_path),
        entry(in_entry),
        highlighted_base_name(in_highlighted_base_name) {
  }

  
  base::FilePath path;
  ResourceEntry entry;

  
  
  
  
  
  
  
  
  
  std::string highlighted_base_name;
};

typedef std::vector<MetadataSearchResult> MetadataSearchResultVector;

typedef base::Callback<void(FileError error,
                            const base::FilePath& file_path,
                            scoped_ptr<ResourceEntry> entry)> GetFileCallback;

typedef base::Callback<void(FileError error,
                            scoped_ptr<ResourceEntry> entry,
                            const base::FilePath& local_file,
                            const base::Closure& cancel_download_closure)>
    GetFileContentInitializedCallback;

typedef base::Callback<void(
    FileError error,
    const GURL& next_link,
    scoped_ptr<std::vector<SearchResultInfo> > result_paths)> SearchCallback;

typedef base::Callback<void(
    FileError error,
    scoped_ptr<MetadataSearchResultVector> result)> SearchMetadataCallback;

typedef base::Callback<void(FileError error,
                            const base::FilePath& file_path,
                            const base::Closure& close_callback)>
    OpenFileCallback;

typedef base::Callback<void(FileError error,
                            int64 bytes_total,
                            int64 bytes_used)> GetAvailableSpaceCallback;

typedef base::Callback<void(FileError error,
                            const GURL& share_url)> GetShareUrlCallback;

typedef base::Callback<void(const FileSystemMetadata&)>
    GetFilesystemMetadataCallback;

typedef base::Callback<void(FileError error,
                            const base::FilePath& file_path)>
    MarkMountedCallback;

typedef base::Callback<void(bool success, const FileCacheEntry& cache_entry)>
    GetCacheEntryCallback;

enum OpenMode {
  
  OPEN_FILE,

  
  CREATE_FILE,

  
  OPEN_OR_CREATE_FILE,
};

enum ContextType {
  USER_INITIATED,
  BACKGROUND,
  
  NUM_CONTEXT_TYPES,
};

struct ClientContext {
  explicit ClientContext(ContextType in_type) : type(in_type) {}
  ContextType type;
};

enum SearchMetadataOptions {
  SEARCH_METADATA_ALL = 0,
  SEARCH_METADATA_EXCLUDE_HOSTED_DOCUMENTS = 1,
  SEARCH_METADATA_EXCLUDE_DIRECTORIES = 1 << 1,
  SEARCH_METADATA_SHARED_WITH_ME = 1 << 2,
  SEARCH_METADATA_OFFLINE = 1 << 3,
};

class FileSystemInterface {
 public:
  virtual ~FileSystemInterface() {}

  
  virtual void AddObserver(FileSystemObserver* observer) = 0;
  virtual void RemoveObserver(FileSystemObserver* observer) = 0;

  
  virtual void CheckForUpdates() = 0;

  
  
  
  
  
  
  virtual void TransferFileFromLocalToRemote(
      const base::FilePath& local_src_file_path,
      const base::FilePath& remote_dest_file_path,
      const FileOperationCallback& callback) = 0;

  
  
  
  
  
  
  
  
  
  virtual void OpenFile(const base::FilePath& file_path,
                        OpenMode open_mode,
                        const std::string& mime_type,
                        const OpenFileCallback& callback) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void Copy(const base::FilePath& src_file_path,
                    const base::FilePath& dest_file_path,
                    bool preserve_last_modified,
                    const FileOperationCallback& callback) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void Move(const base::FilePath& src_file_path,
                    const base::FilePath& dest_file_path,
                    bool preserve_last_modified,
                    const FileOperationCallback& callback) = 0;

  
  
  
  
  
  
  
  virtual void Remove(const base::FilePath& file_path,
                      bool is_recursive,
                      const FileOperationCallback& callback) = 0;

  
  
  
  
  
  
  virtual void CreateDirectory(const base::FilePath& directory_path,
                               bool is_exclusive,
                               bool is_recursive,
                               const FileOperationCallback& callback) = 0;

  
  
  
  
  
  
  
  
  
  virtual void CreateFile(const base::FilePath& file_path,
                          bool is_exclusive,
                          const std::string& mime_type,
                          const FileOperationCallback& callback) = 0;

  
  
  
  
  
  
  
  virtual void TouchFile(const base::FilePath& file_path,
                         const base::Time& last_access_time,
                         const base::Time& last_modified_time,
                         const FileOperationCallback& callback) = 0;

  
  
  
  virtual void TruncateFile(const base::FilePath& file_path,
                            int64 length,
                            const FileOperationCallback& callback) = 0;

  
  
  
  virtual void Pin(const base::FilePath& file_path,
                   const FileOperationCallback& callback) = 0;

  
  
  
  virtual void Unpin(const base::FilePath& file_path,
                     const FileOperationCallback& callback) = 0;

  
  
  
  
  
  virtual void GetFile(const base::FilePath& file_path,
                       const GetFileCallback& callback) = 0;

  
  
  
  
  
  
  virtual void GetFileForSaving(const base::FilePath& file_path,
                                const GetFileCallback& callback) = 0;

  
  
  
  
  
  
  
  
  
  
  
  virtual void GetFileContent(
      const base::FilePath& file_path,
      const GetFileContentInitializedCallback& initialized_callback,
      const google_apis::GetContentCallback& get_content_callback,
      const FileOperationCallback& completion_callback) = 0;

  
  
  
  
  virtual void GetResourceEntry(const base::FilePath& file_path,
                                const GetResourceEntryCallback& callback) = 0;

  
  
  
  
  virtual void ReadDirectory(const base::FilePath& file_path,
                             const ReadDirectoryCallback& callback) = 0;

  
  
  
  
  
  
  
  virtual void Search(const std::string& search_query,
                      const GURL& next_link,
                      const SearchCallback& callback) = 0;

  
  
  
  
  
  
  
  virtual void SearchMetadata(const std::string& query,
                              int options,
                              int at_most_num_matches,
                              const SearchMetadataCallback& callback) = 0;

  
  
  virtual void GetAvailableSpace(const GetAvailableSpaceCallback& callback) = 0;

  
  
  virtual void GetShareUrl(
      const base::FilePath& file_path,
      const GURL& embed_origin,
      const GetShareUrlCallback& callback) = 0;

  
  
  virtual void GetMetadata(
      const GetFilesystemMetadataCallback& callback) = 0;

  
  
  
  virtual void MarkCacheFileAsMounted(const base::FilePath& drive_file_path,
                                      const MarkMountedCallback& callback) = 0;

  
  
  
  
  virtual void MarkCacheFileAsUnmounted(
      const base::FilePath& cache_file_path,
      const FileOperationCallback& callback) = 0;

  
  
  
  
  virtual void GetCacheEntry(const base::FilePath& drive_file_path,
                             const GetCacheEntryCallback& callback) = 0;

  
  virtual void Reload(const FileOperationCallback& callback) = 0;
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_RESOURCE_METADATA_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_RESOURCE_METADATA_H_

#include <set>
#include <string>
#include <vector>

#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/drive/file_errors.h"
#include "chrome/browser/chromeos/drive/resource_metadata_storage.h"

namespace base {
class SequencedTaskRunner;
}

namespace drive {

typedef std::vector<ResourceEntry> ResourceEntryVector;

typedef base::Callback<void(FileError error,
                            scoped_ptr<ResourceEntry> entry)>
    GetResourceEntryCallback;

typedef base::Callback<void(FileError error,
                            scoped_ptr<ResourceEntryVector> entries)>
    ReadDirectoryCallback;

typedef base::Callback<void(const ResourceEntry& entry)> IterateCallback;

namespace internal {

class ResourceMetadata {
 public:
  typedef ResourceMetadataStorage::Iterator Iterator;

  ResourceMetadata(
      ResourceMetadataStorage* storage,
      scoped_refptr<base::SequencedTaskRunner> blocking_task_runner);

  
  
  FileError Initialize() WARN_UNUSED_RESULT;

  
  
  
  void Destroy();

  
  FileError Reset();

  
  int64 GetLargestChangestamp();

  
  FileError SetLargestChangestamp(int64 value);

  
  FileError AddEntry(const ResourceEntry& entry, std::string* out_id);

  
  FileError RemoveEntry(const std::string& id);

  
  FileError GetResourceEntryById(const std::string& id,
                                 ResourceEntry* out_entry);

  
  
  
  void GetResourceEntryByPathOnUIThread(
      const base::FilePath& file_path,
      const GetResourceEntryCallback& callback);

  
  FileError GetResourceEntryByPath(const base::FilePath& file_path,
                                   ResourceEntry* out_entry);

  
  
  
  void ReadDirectoryByPathOnUIThread(const base::FilePath& file_path,
                                     const ReadDirectoryCallback& callback);

  
  FileError ReadDirectoryByPath(const base::FilePath& file_path,
                                ResourceEntryVector* out_entries);

  
  FileError RefreshEntry(const ResourceEntry& entry);

  
  void GetSubDirectoriesRecursively(const std::string& id,
                                    std::set<base::FilePath>* sub_directories);

  
  
  
  std::string GetChildId(const std::string& parent_local_id,
                         const std::string& base_name);

  
  scoped_ptr<Iterator> GetIterator();

  
  base::FilePath GetFilePath(const std::string& id);

  
  FileError GetIdByPath(const base::FilePath& file_path, std::string* out_id);

  
  FileError GetIdByResourceId(const std::string& resource_id,
                              std::string* out_local_id);

 private:
  
  ~ResourceMetadata();

  
  bool SetUpDefaultEntries();

  
  void DestroyOnBlockingPool();

  
  
  
  
  bool PutEntryUnderDirectory(const ResourceEntry& entry);

  
  bool RemoveEntryRecursively(const std::string& id);

  scoped_refptr<base::SequencedTaskRunner> blocking_task_runner_;

  ResourceMetadataStorage* storage_;

  
  
  base::WeakPtrFactory<ResourceMetadata> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ResourceMetadata);
};

}  
}  

#endif  

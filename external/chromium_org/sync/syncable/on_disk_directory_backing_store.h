// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_SYNCABLE_ON_DISK_DIRECTORY_BACKING_STORE_H_
#define SYNC_SYNCABLE_ON_DISK_DIRECTORY_BACKING_STORE_H_

#include "base/files/file_path.h"
#include "sync/base/sync_export.h"
#include "sync/syncable/directory_backing_store.h"

namespace syncer {
namespace syncable {

class SYNC_EXPORT_PRIVATE OnDiskDirectoryBackingStore
    : public DirectoryBackingStore {
 public:
  OnDiskDirectoryBackingStore(const std::string& dir_name,
                              const base::FilePath& backing_filepath);
  virtual ~OnDiskDirectoryBackingStore();
  virtual DirOpenResult Load(
      Directory::MetahandlesMap* handles_map,
      JournalIndex* delete_journals,
      Directory::KernelLoadInfo* kernel_load_info) OVERRIDE;

  
  
  DirOpenResult TryLoad(
      Directory::MetahandlesMap* handles_map,
      JournalIndex* delete_journals,
      Directory::KernelLoadInfo* kernel_load_info);

 protected:
  
  virtual void ReportFirstTryOpenFailure();

 private:
  FRIEND_TEST_ALL_PREFIXES(DirectoryBackingStoreTest, MinorCorruption);

  bool allow_failure_for_test_;
  base::FilePath backing_filepath_;

  DISALLOW_COPY_AND_ASSIGN(OnDiskDirectoryBackingStore);
};

}  
}  

#endif  

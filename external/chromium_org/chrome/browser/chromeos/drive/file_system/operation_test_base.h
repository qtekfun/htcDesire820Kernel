// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_OPERATION_TEST_BASE_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_OPERATION_TEST_BASE_H_

#include <set>

#include "base/files/scoped_temp_dir.h"
#include "chrome/browser/chromeos/drive/change_list_loader.h"
#include "chrome/browser/chromeos/drive/drive.pb.h"
#include "chrome/browser/chromeos/drive/file_system/operation_observer.h"
#include "chrome/browser/chromeos/drive/test_util.h"
#include "content/public/test/test_browser_thread_bundle.h"
#include "testing/gtest/include/gtest/gtest.h"

class TestingPrefServiceSimple;

namespace base {
class SequencedTaskRunner;
}  

namespace drive {

class FakeDriveService;
class FakeFreeDiskSpaceGetter;
class JobScheduler;

namespace internal {
class FileCache;
class ResourceMetadata;
class ResourceMetadataStorage;
}  

namespace file_system {

class OperationTestBase : public testing::Test {
 protected:
  
  class LoggingObserver : public OperationObserver {
   public:
    LoggingObserver();
    ~LoggingObserver();

    
    virtual void OnDirectoryChangedByOperation(
        const base::FilePath& path) OVERRIDE;
    virtual void OnCacheFileUploadNeededByOperation(
        const std::string& local_id) OVERRIDE;
    virtual void OnEntryUpdatedByOperation(
        const std::string& local_id) OVERRIDE;

    
    const std::set<base::FilePath>& get_changed_paths() {
      return changed_paths_;
    }

    
    const std::set<std::string>& upload_needed_local_ids() const {
      return upload_needed_local_ids_;
    }

    
    const std::set<std::string>& updated_local_ids() const {
      return updated_local_ids_;
    }

   private:
    std::set<base::FilePath> changed_paths_;
    std::set<std::string> upload_needed_local_ids_;
    std::set<std::string> updated_local_ids_;
  };

  OperationTestBase();
  explicit OperationTestBase(int test_thread_bundle_options);
  virtual ~OperationTestBase();

  
  virtual void SetUp() OVERRIDE;

  
  base::FilePath temp_dir() const { return temp_dir_.path(); }

  
  
  FileError GetLocalResourceEntry(const base::FilePath& path,
                                  ResourceEntry* entry);

  
  
  FileError GetLocalResourceEntryById(const std::string& local_id,
                                      ResourceEntry* entry);

  
  std::string GetLocalId(const base::FilePath& path);

  
  
  FileError CheckForUpdates();

  
  FakeDriveService* fake_service() {
    return fake_drive_service_.get();
  }
  LoggingObserver* observer() { return &observer_; }
  JobScheduler* scheduler() { return scheduler_.get(); }
  base::SequencedTaskRunner* blocking_task_runner() {
    return blocking_task_runner_.get();
  }
  internal::ResourceMetadata* metadata() { return metadata_.get(); }
  FakeFreeDiskSpaceGetter* fake_free_disk_space_getter() {
    return fake_free_disk_space_getter_.get();
  }
  internal::FileCache* cache() { return cache_.get(); }

 private:
  content::TestBrowserThreadBundle thread_bundle_;
  scoped_refptr<base::SequencedTaskRunner> blocking_task_runner_;
  scoped_ptr<TestingPrefServiceSimple> pref_service_;
  base::ScopedTempDir temp_dir_;

  LoggingObserver observer_;
  scoped_ptr<FakeDriveService> fake_drive_service_;
  scoped_ptr<JobScheduler> scheduler_;
  scoped_ptr<internal::ResourceMetadataStorage,
             test_util::DestroyHelperForTests> metadata_storage_;
  scoped_ptr<internal::ResourceMetadata, test_util::DestroyHelperForTests>
      metadata_;
  scoped_ptr<FakeFreeDiskSpaceGetter> fake_free_disk_space_getter_;
  scoped_ptr<internal::FileCache, test_util::DestroyHelperForTests> cache_;
  scoped_ptr<internal::ChangeListLoader> change_list_loader_;
};

}  
}  

#endif  

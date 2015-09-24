// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef APPS_SAVED_FILES_SERVICE_H_
#define APPS_SAVED_FILES_SERVICE_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/stl_util.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class Profile;
class SavedFilesServiceUnitTest;
FORWARD_DECLARE_TEST(SavedFilesServiceUnitTest, RetainTwoFilesTest);
FORWARD_DECLARE_TEST(SavedFilesServiceUnitTest, EvictionTest);
FORWARD_DECLARE_TEST(SavedFilesServiceUnitTest, SequenceNumberCompactionTest);

namespace extensions {
class Extension;
}

namespace apps {

struct SavedFileEntry {
  SavedFileEntry();

  SavedFileEntry(const std::string& id,
                 const base::FilePath& path,
                 bool is_directory,
                 int sequence_number);

  
  std::string id;

  
  base::FilePath path;

  
  bool is_directory;

  
  
  int sequence_number;
};

class SavedFilesService : public BrowserContextKeyedService,
                          public content::NotificationObserver {
 public:
  explicit SavedFilesService(Profile* profile);
  virtual ~SavedFilesService();

  static SavedFilesService* Get(Profile* profile);

  
  
  
  void RegisterFileEntry(const std::string& extension_id,
                         const std::string& id,
                         const base::FilePath& file_path,
                         bool is_directory);

  
  
  
  
  
  void EnqueueFileEntry(const std::string& extension_id, const std::string& id);

  
  bool IsRegistered(const std::string& extension_id, const std::string& id);

  
  
  const SavedFileEntry* GetFileEntry(const std::string& extension_id,
                                     const std::string& id);

  
  std::vector<SavedFileEntry> GetAllFileEntries(
      const std::string& extension_id);

  
  
  void ClearQueueIfNoRetainPermission(const extensions::Extension* extension);

  
  void ClearQueue(const extensions::Extension* extension);

 private:
  FRIEND_TEST_ALL_PREFIXES(::SavedFilesServiceUnitTest, RetainTwoFilesTest);
  FRIEND_TEST_ALL_PREFIXES(::SavedFilesServiceUnitTest, EvictionTest);
  FRIEND_TEST_ALL_PREFIXES(::SavedFilesServiceUnitTest,
                           SequenceNumberCompactionTest);
  friend class ::SavedFilesServiceUnitTest;

  
  class SavedFiles;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  SavedFiles* Get(const std::string& extension_id) const;

  
  SavedFiles* GetOrInsert(const std::string& extension_id);

  
  void Clear(const std::string& extension_id);

  static void SetMaxSequenceNumberForTest(int max_value);
  static void ClearMaxSequenceNumberForTest();
  static void SetLruSizeForTest(int size);
  static void ClearLruSizeForTest();

  std::map<std::string, SavedFiles*> extension_id_to_saved_files_;
  STLValueDeleter<std::map<std::string, SavedFiles*> >
      extension_id_to_saved_files_deleter_;
  content::NotificationRegistrar registrar_;
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(SavedFilesService);
};

}  

#endif  

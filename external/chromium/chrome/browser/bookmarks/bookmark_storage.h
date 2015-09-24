// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_STORAGE_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_STORAGE_H_
#pragma once

#include "base/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/bookmarks/bookmark_index.h"
#include "chrome/common/important_file_writer.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class BookmarkModel;
class BookmarkNode;
class Profile;

class BookmarkLoadDetails {
 public:
  BookmarkLoadDetails(BookmarkNode* bb_node,
                      BookmarkNode* other_folder_node,
                      BookmarkIndex* index,
                      int64 max_id);
  ~BookmarkLoadDetails();

  BookmarkNode* bb_node() { return bb_node_.get(); }
  BookmarkNode* release_bb_node() { return bb_node_.release(); }
  BookmarkNode* other_folder_node() { return other_folder_node_.get(); }
  BookmarkNode* release_other_folder_node() {
    return other_folder_node_.release();
  }
  BookmarkIndex* index() { return index_.get(); }
  BookmarkIndex* release_index() { return index_.release(); }

  
  void set_max_id(int64 max_id) { max_id_ = max_id; }
  int64 max_id() const { return max_id_; }

  
  void set_computed_checksum(const std::string& value) {
    computed_checksum_ = value;
  }
  const std::string& computed_checksum() const { return computed_checksum_; }

  
  void set_stored_checksum(const std::string& value) {
    stored_checksum_ = value;
  }
  const std::string& stored_checksum() const { return stored_checksum_; }

  
  void set_ids_reassigned(bool value) { ids_reassigned_ = value; }
  bool ids_reassigned() const { return ids_reassigned_; }

 private:
  scoped_ptr<BookmarkNode> bb_node_;
  scoped_ptr<BookmarkNode> other_folder_node_;
  scoped_ptr<BookmarkIndex> index_;
  int64 max_id_;
  std::string computed_checksum_;
  std::string stored_checksum_;
  bool ids_reassigned_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkLoadDetails);
};

class BookmarkStorage : public NotificationObserver,
                        public ImportantFileWriter::DataSerializer,
                        public base::RefCountedThreadSafe<BookmarkStorage> {
 public:
  
  BookmarkStorage(Profile* profile, BookmarkModel* model);

  
  
  void LoadBookmarks(BookmarkLoadDetails* details);

  
  void ScheduleSave();

  
  
  void BookmarkModelDeleted();

  
  virtual bool SerializeData(std::string* output);

 private:
  friend class base::RefCountedThreadSafe<BookmarkStorage>;

  ~BookmarkStorage();

  class LoadTask;

  
  
  
  void OnLoadFinished(bool file_exists,
                      const FilePath& path);

  
  void DoLoadBookmarks(const FilePath& file);

  // Load bookmarks data from the file written by history (StarredURLDatabase).
  void MigrateFromHistory();

  // Called when history has written the file with bookmarks data. Loads data
  
  void OnHistoryFinishedWriting();

  
  
  void FinishHistoryMigration();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  bool SaveNow();

  
  Profile* profile_;

  
  BookmarkModel* model_;

  
  ImportantFileWriter writer_;

  
  NotificationRegistrar notification_registrar_;

  
  const FilePath tmp_history_path_;

  
  scoped_ptr<BookmarkLoadDetails> details_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkStorage);
};

#endif  

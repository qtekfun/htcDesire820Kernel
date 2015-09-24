// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_STORAGE_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_STORAGE_H_

#include "base/files/important_file_writer.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/bookmarks/bookmark_model.h"

class BookmarkIndex;
class BookmarkModel;
class BookmarkPermanentNode;

namespace base {
class SequencedTaskRunner;
}

namespace content {
class BrowserContext;
}

class BookmarkLoadDetails {
 public:
  BookmarkLoadDetails(BookmarkPermanentNode* bb_node,
                      BookmarkPermanentNode* other_folder_node,
                      BookmarkPermanentNode* mobile_folder_node,
                      BookmarkIndex* index,
                      int64 max_id);
  ~BookmarkLoadDetails();

  BookmarkPermanentNode* bb_node() { return bb_node_.get(); }
  BookmarkPermanentNode* release_bb_node() { return bb_node_.release(); }
  BookmarkPermanentNode* mobile_folder_node() {
    return mobile_folder_node_.get();
  }
  BookmarkPermanentNode* release_mobile_folder_node() {
    return mobile_folder_node_.release();
  }
  BookmarkPermanentNode* other_folder_node() {
    return other_folder_node_.get();
  }
  BookmarkPermanentNode* release_other_folder_node() {
    return other_folder_node_.release();
  }
  BookmarkIndex* index() { return index_.get(); }
  BookmarkIndex* release_index() { return index_.release(); }

  const BookmarkNode::MetaInfoMap& model_meta_info_map() const {
    return model_meta_info_map_;
  }
  void set_model_meta_info_map(const BookmarkNode::MetaInfoMap& meta_info_map) {
    model_meta_info_map_ = meta_info_map;
  }

  int64 model_sync_transaction_version() const {
    return model_sync_transaction_version_;
  }
  void set_model_sync_transaction_version(int64 sync_transaction_version) {
    model_sync_transaction_version_ = sync_transaction_version;
  }

  
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
  scoped_ptr<BookmarkPermanentNode> bb_node_;
  scoped_ptr<BookmarkPermanentNode> other_folder_node_;
  scoped_ptr<BookmarkPermanentNode> mobile_folder_node_;
  scoped_ptr<BookmarkIndex> index_;
  BookmarkNode::MetaInfoMap model_meta_info_map_;
  int64 model_sync_transaction_version_;
  int64 max_id_;
  std::string computed_checksum_;
  std::string stored_checksum_;
  bool ids_reassigned_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkLoadDetails);
};

class BookmarkStorage : public base::ImportantFileWriter::DataSerializer,
                        public base::RefCountedThreadSafe<BookmarkStorage> {
 public:
  
  BookmarkStorage(content::BrowserContext* context,
                  BookmarkModel* model,
                  base::SequencedTaskRunner* sequenced_task_runner);

  
  
  void LoadBookmarks(BookmarkLoadDetails* details);

  
  void ScheduleSave();

  
  
  void BookmarkModelDeleted();

  
  void OnLoadFinished();

  
  virtual bool SerializeData(std::string* output) OVERRIDE;

 private:
  friend class base::RefCountedThreadSafe<BookmarkStorage>;

  virtual ~BookmarkStorage();

  
  
  bool SaveNow();

  
  BookmarkModel* model_;

  
  base::ImportantFileWriter writer_;

  
  scoped_ptr<BookmarkLoadDetails> details_;

  
  scoped_refptr<base::SequencedTaskRunner> sequenced_task_runner_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkStorage);
};

#endif  

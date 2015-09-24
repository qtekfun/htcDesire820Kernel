// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_CODEC_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_CODEC_H_

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/md5.h"
#include "base/strings/string16.h"
#include "chrome/browser/bookmarks/bookmark_model.h"

class BookmarkModel;
class BookmarkNode;

namespace base {
class DictionaryValue;
class ListValue;
class Value;
}

// into JSON values. The encoded values are written to disk via the
class BookmarkCodec {
 public:
  
  
  
  
  BookmarkCodec();
  ~BookmarkCodec();

  
  
  
  
  base::Value* Encode(BookmarkModel* model);

  
  
  base::Value* Encode(const BookmarkNode* bookmark_bar_node,
                      const BookmarkNode* other_folder_node,
                      const BookmarkNode* mobile_folder_node,
                      const BookmarkNode::MetaInfoMap* model_meta_info_map,
                      int64 sync_transaction_version);

  
  
  
  
  
  bool Decode(BookmarkNode* bb_node,
              BookmarkNode* other_folder_node,
              BookmarkNode* mobile_folder_node,
              int64* max_node_id,
              const base::Value& value);

  
  const std::string& computed_checksum() const { return computed_checksum_; }

  
  
  
  
  
  const std::string& stored_checksum() const { return stored_checksum_; }

  
  const BookmarkNode::MetaInfoMap& model_meta_info_map() const {
    return model_meta_info_map_;
  }

  
  int64 model_sync_transaction_version() const {
    return model_sync_transaction_version_;
  }

  
  
  bool ids_reassigned() const { return ids_reassigned_; }

  // Names of the various keys written to the Value.
  static const char* kRootsKey;
  static const char* kRootFolderNameKey;
  static const char* kOtherBookmarkFolderNameKey;
  static const char* kMobileBookmarkFolderNameKey;
  static const char* kVersionKey;
  static const char* kChecksumKey;
  static const char* kIdKey;
  static const char* kTypeKey;
  static const char* kNameKey;
  static const char* kDateAddedKey;
  static const char* kURLKey;
  static const char* kDateModifiedKey;
  static const char* kChildrenKey;
  static const char* kMetaInfo;
  static const char* kSyncTransactionVersion;

  
  static const char* kTypeURL;
  static const char* kTypeFolder;

 private:
  
  
  base::Value* EncodeNode(const BookmarkNode* node);

  
  
  base::Value* EncodeMetaInfo(const BookmarkNode::MetaInfoMap& meta_info_map);

  
  bool DecodeHelper(BookmarkNode* bb_node,
                    BookmarkNode* other_folder_node,
                    BookmarkNode* mobile_folder_node,
                    const base::Value& value);

  
  bool DecodeChildren(const base::ListValue& child_value_list,
                      BookmarkNode* parent);

  
  void ReassignIDs(BookmarkNode* bb_node,
                   BookmarkNode* other_node,
                   BookmarkNode* mobile_node);

  
  void ReassignIDsHelper(BookmarkNode* node);

  
  
  
  
  bool DecodeNode(const base::DictionaryValue& value,
                  BookmarkNode* parent,
                  BookmarkNode* node);

  
  
  
  
  
  bool DecodeMetaInfo(const base::DictionaryValue& value,
                      BookmarkNode::MetaInfoMap* meta_info_map,
                      int64* sync_transaction_version);

  
  
  
  void DecodeMetaInfoHelper(const base::DictionaryValue& dict,
                            const std::string& prefix,
                            BookmarkNode::MetaInfoMap* meta_info_map);

  
  void UpdateChecksum(const std::string& str);
  void UpdateChecksum(const base::string16& str);

  
  
  
  
  
  
  void UpdateChecksumWithUrlNode(const std::string& id,
                                 const base::string16& title,
                                 const std::string& url);
  void UpdateChecksumWithFolderNode(const std::string& id,
                                    const base::string16& title);

  
  void InitializeChecksum();
  void FinalizeChecksum();

  
  bool ids_reassigned_;

  
  
  bool ids_valid_;

  
  
  std::set<int64> ids_;

  
  base::MD5Context md5_context_;

  
  std::string computed_checksum_;
  std::string stored_checksum_;

  
  int64 maximum_id_;

  
  BookmarkNode::MetaInfoMap model_meta_info_map_;

  
  int64 model_sync_transaction_version_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkCodec);
};

#endif  

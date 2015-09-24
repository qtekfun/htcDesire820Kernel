// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// into JSON values. The encoded values are written to disk via the

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_CODEC_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_CODEC_H_
#pragma once

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/md5.h"
#include "base/string16.h"

class BookmarkModel;
class BookmarkNode;
class DictionaryValue;
class ListValue;
class Value;


class BookmarkCodec {
 public:
  
  
  
  
  BookmarkCodec();
  ~BookmarkCodec();

  
  
  
  
  Value* Encode(BookmarkModel* model);

  
  
  
  
  Value* Encode(const BookmarkNode* bookmark_bar_node,
                const BookmarkNode* other_folder_node);

  
  
  
  
  
  bool Decode(BookmarkNode* bb_node,
              BookmarkNode* other_folder_node,
              int64* max_node_id,
              const Value& value);

  
  const std::string& computed_checksum() const { return computed_checksum_; }

  
  
  
  
  
  const std::string& stored_checksum() const { return stored_checksum_; }

  
  
  bool ids_reassigned() const { return ids_reassigned_; }

  // Names of the various keys written to the Value.
  static const char* kRootsKey;
  static const char* kRootFolderNameKey;
  static const char* kOtherBookmarkFolderNameKey;
  static const char* kVersionKey;
  static const char* kChecksumKey;
  static const char* kIdKey;
  static const char* kTypeKey;
  static const char* kNameKey;
  static const char* kDateAddedKey;
  static const char* kURLKey;
  static const char* kDateModifiedKey;
  static const char* kChildrenKey;

  
  static const char* kTypeURL;
  static const char* kTypeFolder;

 private:
  
  
  Value* EncodeNode(const BookmarkNode* node);

  
  bool DecodeHelper(BookmarkNode* bb_node,
                    BookmarkNode* other_folder_node,
                    const Value& value);

  
  bool DecodeChildren(const ListValue& child_value_list,
                      BookmarkNode* parent);

  
  void ReassignIDs(BookmarkNode* bb_node, BookmarkNode* other_node);

  
  void ReassignIDsHelper(BookmarkNode* node);

  
  
  
  
  bool DecodeNode(const DictionaryValue& value,
                  BookmarkNode* parent,
                  BookmarkNode* node);

  
  void UpdateChecksum(const std::string& str);
  void UpdateChecksum(const string16& str);

  
  
  
  
  
  
  void UpdateChecksumWithUrlNode(const std::string& id,
                                 const string16& title,
                                 const std::string& url);
  void UpdateChecksumWithFolderNode(const std::string& id,
                                    const string16& title);

  
  void InitializeChecksum();
  void FinalizeChecksum();

  
  bool ids_reassigned_;

  
  
  bool ids_valid_;

  
  
  std::set<int64> ids_;

  
  MD5Context md5_context_;

  
  std::string computed_checksum_;
  std::string stored_checksum_;

  
  int64 maximum_id_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkCodec);
};

#endif  

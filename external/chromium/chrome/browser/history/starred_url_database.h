// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_STARRED_URL_DATABASE_H_
#define CHROME_BROWSER_HISTORY_STARRED_URL_DATABASE_H_
#pragma once

#include <set>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/string16.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/history/url_database.h"
#include "ui/base/models/tree_node_model.h"

class FilePath;

namespace sql {
class Connection;
}

namespace history {

class StarredURLDatabase : public URLDatabase {
 public:
  
  
  StarredURLDatabase();
  virtual ~StarredURLDatabase();

 protected:
  
  friend class HistoryTest;
  friend class StarredURLDatabaseTest;
  FRIEND_TEST_ALL_PREFIXES(HistoryTest, CreateStarFolder);

  
  bool MigrateBookmarksToFile(const FilePath& path);

  
  virtual sql::Connection& GetDB() = 0;

 private:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool EnsureStarredIntegrity();

  
  bool GetAllStarredEntries(std::vector<StarredEntry>* entries);

  
  
  
  
  bool UpdateStarredEntryRow(StarID star_id,
                             const string16& title,
                             UIStarID parent_folder_id,
                             int visual_order,
                             base::Time date_modified);

  
  
  
  
  bool AdjustStarredVisualOrder(UIStarID parent_folder_id,
                                int start_visual_order,
                                int delta);

  
  
  
  
  StarID CreateStarredEntryRow(URLID url_id,
                               UIStarID folder_id,
                               UIStarID parent_folder_id,
                               const string16& title,
                               const base::Time& date_added,
                               int visual_order,
                               StarredEntry::Type type);

  
  
  
  
  bool DeleteStarredEntryRow(StarID star_id);

  
  bool GetStarredEntry(StarID star_id, StarredEntry* entry);

  
  
  
  
  
  
  
  StarID CreateStarredEntry(StarredEntry* entry);

  
  typedef ui::TreeNodeWithValue<history::StarredEntry> StarredNode;

  
  UIStarID GetMaxFolderID();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool BuildStarNodes(
      std::set<StarredNode*>* roots,
      std::set<StarID>* folders_with_duplicate_ids,
      std::set<StarredNode*>* unparented_urls,
      std::set<StarID>* empty_url_ids);

  
  
  
  bool EnsureVisualOrder(StarredNode* node);

  
  
  StarredNode* GetNodeByType(
      const std::set<StarredNode*>& nodes,
      StarredEntry::Type type);

  
  
  
  
  
  
  
  
  
  bool EnsureStarredIntegrityImpl(
      std::set<StarredNode*>* roots,
      const std::set<StarID>& folders_with_duplicate_ids,
      std::set<StarredNode*>* unparented_urls,
      const std::set<StarID>& empty_url_ids);

  
  
  
  
  
  bool Move(StarredNode* source, StarredNode* new_parent);

  
  
  bool MigrateBookmarksToFileImpl(const FilePath& path);

  DISALLOW_COPY_AND_ASSIGN(StarredURLDatabase);
};

}  

#endif  

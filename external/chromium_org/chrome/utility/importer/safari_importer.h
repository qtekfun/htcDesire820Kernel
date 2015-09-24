// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_UTILITY_IMPORTER_SAFARI_IMPORTER_H_
#define CHROME_UTILITY_IMPORTER_SAFARI_IMPORTER_H_

#include <map>
#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "chrome/common/importer/importer_url_row.h"
#include "chrome/utility/importer/importer.h"

#if __OBJC__
@class NSDictionary;
@class NSString;
#else
class NSDictionary;
class NSString;
#endif

class GURL;
struct ImportedBookmarkEntry;
struct ImportedFaviconUsage;

namespace sql {
class Connection;
}

class SafariImporter : public Importer {
 public:
  
  
  explicit SafariImporter(const base::FilePath& library_dir);

  
  virtual void StartImport(const importer::SourceProfile& source_profile,
                           uint16 items,
                           ImporterBridge* bridge) OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(SafariImporterTest, BookmarkImport);
  FRIEND_TEST_ALL_PREFIXES(SafariImporterTest, FaviconImport);
  FRIEND_TEST_ALL_PREFIXES(SafariImporterTest, HistoryImport);

  virtual ~SafariImporter();

  
  
  
  typedef std::map<int64, std::set<GURL> > FaviconMap;

  void ImportBookmarks();
  void ImportPasswords();
  void ImportHistory();

  
  void ParseBookmarks(const string16& toolbar_name,
                      std::vector<ImportedBookmarkEntry>* bookmarks);

  
  
  
  
  
  void RecursiveReadBookmarksFolder(
      NSDictionary* bookmark_folder,
      const std::vector<string16>& parent_path_elements,
      bool is_in_toolbar,
      const string16& toolbar_name,
      std::vector<ImportedBookmarkEntry>* out_bookmarks);

  
  
  double HistoryTimeToEpochTime(NSString* history_time);

  
  void ParseHistoryItems(std::vector<ImporterURLRow>* history_items);

  
  bool OpenDatabase(sql::Connection* db);

  
  void ImportFaviconURLs(sql::Connection* db, FaviconMap* favicon_map);

  
  void LoadFaviconData(sql::Connection* db,
                       const FaviconMap& favicon_map,
                       std::vector<ImportedFaviconUsage>* favicons);

  base::FilePath library_dir_;

  DISALLOW_COPY_AND_ASSIGN(SafariImporter);
};

#endif  

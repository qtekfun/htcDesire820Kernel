// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMPORTER_SAFARI_IMPORTER_H_
#define CHROME_BROWSER_IMPORTER_SAFARI_IMPORTER_H_
#pragma once

#include <map>
#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/file_path.h"
#include "base/gtest_prod_util.h"
#include "chrome/browser/importer/importer.h"
#include "chrome/browser/importer/profile_writer.h"

#if __OBJC__
@class NSDictionary;
@class NSString;
#else
class NSDictionary;
class NSString;
#endif

class GURL;

namespace history {
class URLRow;
struct ImportedFaviconUsage;
}

namespace sql {
class Connection;
}

class SafariImporter : public Importer {
 public:
  
  
  explicit SafariImporter(const FilePath& library_dir);

  
  virtual void StartImport(const importer::SourceProfile& source_profile,
                           uint16 items,
                           ImporterBridge* bridge) OVERRIDE;

 
 
 
 
 
 static bool CanImport(const FilePath& library_dir, uint16* services_supported);

 private:
  FRIEND_TEST_ALL_PREFIXES(SafariImporterTest, BookmarkImport);
  FRIEND_TEST_ALL_PREFIXES(SafariImporterTest, FaviconImport);
  FRIEND_TEST_ALL_PREFIXES(SafariImporterTest, HistoryImport);

  virtual ~SafariImporter();

  
  
  
  typedef std::map<int64, std::set<GURL> > FaviconMap;

  void ImportBookmarks();
  void ImportPasswords();
  void ImportHistory();

  
  void ParseBookmarks(std::vector<ProfileWriter::BookmarkEntry>* bookmarks);

  
  
  
  
  
  void RecursiveReadBookmarksFolder(
      NSDictionary* bookmark_folder,
      const std::vector<string16>& parent_path_elements,
      bool is_in_toolbar,
      std::vector<ProfileWriter::BookmarkEntry>* out_bookmarks);

  
  
  double HistoryTimeToEpochTime(NSString* history_time);

  
  void ParseHistoryItems(std::vector<history::URLRow>* history_items);

  
  bool OpenDatabase(sql::Connection* db);

  
  void ImportFaviconURLs(sql::Connection* db, FaviconMap* favicon_map);

  
  void LoadFaviconData(sql::Connection* db,
                       const FaviconMap& favicon_map,
                       std::vector<history::ImportedFaviconUsage>* favicons);

  FilePath library_dir_;

  DISALLOW_COPY_AND_ASSIGN(SafariImporter);
};

#endif  

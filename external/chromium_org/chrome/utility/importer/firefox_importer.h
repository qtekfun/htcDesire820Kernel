// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_UTILITY_IMPORTER_FIREFOX_IMPORTER_H_
#define CHROME_UTILITY_IMPORTER_FIREFOX_IMPORTER_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "chrome/utility/importer/importer.h"

class GURL;
struct ImportedFaviconUsage;

namespace sql {
class Connection;
}

class FirefoxImporter : public Importer {
 public:
  FirefoxImporter();

  
  virtual void StartImport(const importer::SourceProfile& source_profile,
                           uint16 items,
                           ImporterBridge* bridge) OVERRIDE;

 private:
  typedef std::map<int64, std::set<GURL> > FaviconMap;

  virtual ~FirefoxImporter();

  void ImportBookmarks();
  void ImportPasswords();
  void ImportHistory();
  void ImportSearchEngines();
  
  
  void ImportHomepage();
  void GetSearchEnginesXMLData(std::vector<std::string>* search_engine_data);

  
  struct BookmarkItem;
  typedef std::vector<BookmarkItem*> BookmarkList;

  
  void LoadRootNodeID(sql::Connection* db, int* toolbar_folder_id,
                      int* menu_folder_id, int* unsorted_folder_id);

  
  void LoadLivemarkIDs(sql::Connection* db, std::set<int>* livemark);

  
  
  void GetTopBookmarkFolder(sql::Connection* db,
                            int folder_id,
                            BookmarkList* list);

  
  void GetWholeBookmarkFolder(sql::Connection* db, BookmarkList* list,
                              size_t position, bool* empty_folder);

  
  
  void LoadFavicons(sql::Connection* db,
                    const FaviconMap& favicon_map,
                    std::vector<ImportedFaviconUsage>* favicons);

  base::FilePath source_path_;
  base::FilePath app_path_;

#if defined(OS_POSIX)
  
  std::string locale_;
#endif

  DISALLOW_COPY_AND_ASSIGN(FirefoxImporter);
};

#endif  

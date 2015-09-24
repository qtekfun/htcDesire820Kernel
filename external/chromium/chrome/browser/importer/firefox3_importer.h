// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMPORTER_FIREFOX3_IMPORTER_H_
#define CHROME_BROWSER_IMPORTER_FIREFOX3_IMPORTER_H_
#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/file_path.h"
#include "chrome/browser/importer/importer.h"

class GURL;

namespace history {
struct ImportedFaviconUsage;
}

namespace sql {
class Connection;
}

class Firefox3Importer : public Importer {
 public:
  Firefox3Importer();

  
  virtual void StartImport(const importer::SourceProfile& source_profile,
                           uint16 items,
                           ImporterBridge* bridge) OVERRIDE;

 private:
  typedef std::map<int64, std::set<GURL> > FaviconMap;

  virtual ~Firefox3Importer();

  void ImportBookmarks();
  void ImportPasswords();
  void ImportHistory();
  void ImportSearchEngines();
  
  
  void ImportHomepage();
  void GetSearchEnginesXMLFiles(std::vector<FilePath>* files);

  
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
                    std::vector<history::ImportedFaviconUsage>* favicons);

  FilePath source_path_;
  FilePath app_path_;

#if defined(OS_LINUX)
  
  
  std::string locale_;
#endif

  DISALLOW_COPY_AND_ASSIGN(Firefox3Importer);
};

#endif  

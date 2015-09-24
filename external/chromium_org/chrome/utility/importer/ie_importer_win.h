// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_UTILITY_IMPORTER_IE_IMPORTER_WIN_H_
#define CHROME_UTILITY_IMPORTER_IE_IMPORTER_WIN_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/strings/string16.h"
#include "chrome/utility/importer/importer.h"

struct ImportedBookmarkEntry;
struct ImportedFaviconUsage;

class IEImporter : public Importer {
 public:
  IEImporter();

  
  virtual void StartImport(const importer::SourceProfile& source_profile,
                           uint16 items,
                           ImporterBridge* bridge) OVERRIDE;

 private:
  typedef std::vector<ImportedBookmarkEntry> BookmarkVector;

  
  struct FavoritesInfo {
    base::FilePath path;
    string16 links_folder;
  };

  
  static const GUID kPStoreAutocompleteGUID;

  
  static const GUID kUnittestGUID;

  FRIEND_TEST_ALL_PREFIXES(ImporterTest, IEImporter);

  virtual ~IEImporter();

  void ImportFavorites();

  
  void ImportHistory();

  
  void ImportPasswordsIE6();

  
  void ImportPasswordsIE7();

  void ImportSearchEngines();

  
  
  
  
  void ImportHomepage();

  
  bool GetFavoritesInfo(FavoritesInfo* info);

  
  
  void ParseFavoritesFolder(
      const FavoritesInfo& info,
      BookmarkVector* bookmarks,
      std::vector<ImportedFaviconUsage>* favicons);

  
  int CurrentIEVersion() const;

  
  
  base::FilePath source_path_;

  DISALLOW_COPY_AND_ASSIGN(IEImporter);
};

#endif  

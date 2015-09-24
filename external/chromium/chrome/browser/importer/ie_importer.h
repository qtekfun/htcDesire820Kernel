// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMPORTER_IE_IMPORTER_H_
#define CHROME_BROWSER_IMPORTER_IE_IMPORTER_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/file_path.h"
#include "chrome/browser/importer/importer.h"
#include "chrome/browser/importer/profile_writer.h"

class IEImporter : public Importer {
 public:
  IEImporter();

  
  virtual void StartImport(const importer::SourceProfile& source_profile,
                           uint16 items,
                           ImporterBridge* bridge) OVERRIDE;

 private:
  typedef std::vector<ProfileWriter::BookmarkEntry> BookmarkVector;

  
  struct FavoritesInfo {
    FilePath path;
    std::wstring links_folder;
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

  
  
  std::wstring ResolveInternetShortcut(const std::wstring& file);

  
  bool GetFavoritesInfo(FavoritesInfo* info);

  
  
  void ParseFavoritesFolder(const FavoritesInfo& info,
                            BookmarkVector* bookmarks);

  
  int CurrentIEVersion() const;

  
  
  FilePath source_path_;

  DISALLOW_COPY_AND_ASSIGN(IEImporter);
};

#endif  

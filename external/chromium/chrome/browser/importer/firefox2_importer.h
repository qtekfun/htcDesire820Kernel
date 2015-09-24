// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMPORTER_FIREFOX2_IMPORTER_H_
#define CHROME_BROWSER_IMPORTER_FIREFOX2_IMPORTER_H_
#pragma once

#include <set>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/string16.h"
#include "chrome/browser/importer/importer.h"
#include "chrome/browser/importer/profile_writer.h"

class GURL;
class TemplateURL;

class Firefox2Importer : public Importer {
 public:
  Firefox2Importer();

  
  virtual void StartImport(const importer::SourceProfile& source_profile,
                           uint16 items,
                           ImporterBridge* bridge) OVERRIDE;

  
  
  static void LoadDefaultBookmarks(const FilePath& firefox_app_path,
                                   std::set<GURL>* urls);

  
  
  static TemplateURL* CreateTemplateURL(const string16& title,
                                        const string16& keyword,
                                        const GURL& url);

  
  
  
  static void ImportBookmarksFile(
      const FilePath& file_path,
      const std::set<GURL>& default_urls,
      bool import_to_bookmark_bar,
      const string16& first_folder_name,
      Importer* importer,
      std::vector<ProfileWriter::BookmarkEntry>* bookmarks,
      std::vector<TemplateURL*>* template_urls,
      std::vector<history::ImportedFaviconUsage>* favicons);

 private:
  FRIEND_TEST_ALL_PREFIXES(FirefoxImporterTest, Firefox2BookmarkParse);
  FRIEND_TEST_ALL_PREFIXES(FirefoxImporterTest, Firefox2CookesParse);
  FRIEND_TEST_ALL_PREFIXES(FirefoxImporterTest, Firefox2BookmarkFileImport);

  virtual ~Firefox2Importer();

  void ImportBookmarks();
  void ImportPasswords();
  void ImportHistory();
  void ImportSearchEngines();
  
  
  void ImportHomepage();

  
  
  void GetSearchEnginesXMLFiles(std::vector<FilePath>* files);

  
  
  
  
  
  
  
  
  
  
  
  
  static bool ParseCharsetFromLine(const std::string& line,
                                   std::string* charset);
  static bool ParseFolderNameFromLine(const std::string& line,
                                      const std::string& charset,
                                      string16* folder_name,
                                      bool* is_toolbar_folder,
                                      base::Time* add_date);
  
  
  
  static bool ParseBookmarkFromLine(const std::string& line,
                                    const std::string& charset,
                                    string16* title,
                                    GURL* url,
                                    GURL* favicon,
                                    string16* shortcut,
                                    base::Time* add_date,
                                    string16* post_data);
  
  
  
  
  
  
  
  
  static bool ParseMinimumBookmarkFromLine(const std::string& line,
                                           const std::string& charset,
                                           string16* title,
                                           GURL* url);

  
  
  static bool GetAttribute(const std::string& tag,
                           const std::string& attribute,
                           std::string* value);

  
  
  
  static void HTMLUnescape(string16* text);

  
  static void FindXMLFilesInDir(const FilePath& dir,
                                std::vector<FilePath>* xml_files);

  
  
  
  static void DataURLToFaviconUsage(
      const GURL& link_url,
      const GURL& favicon_data,
      std::vector<history::ImportedFaviconUsage>* favicons);

  FilePath source_path_;
  FilePath app_path_;
  
  bool parsing_bookmarks_html_file_;

  DISALLOW_COPY_AND_ASSIGN(Firefox2Importer);
};

#endif  

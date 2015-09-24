// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_IMPORTER_TOOLBAR_IMPORTER_H_
#define CHROME_BROWSER_IMPORTER_TOOLBAR_IMPORTER_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/string16.h"
#include "chrome/browser/importer/importer.h"
#include "chrome/browser/importer/profile_writer.h"
#include "chrome/common/net/url_fetcher.h"

class ImporterBridge;
class XmlReader;

class Toolbar5Importer : public URLFetcher::Delegate, public Importer {
 public:
  Toolbar5Importer();

  

  
  
  
  virtual void StartImport(const importer::SourceProfile& source_profile,
                           uint16 items,
                           ImporterBridge* bridge) OVERRIDE;

  
  
  virtual void Cancel() OVERRIDE;

  

  
  virtual void OnURLFetchComplete(const URLFetcher* source,
                                  const GURL& url,
                                  const net::URLRequestStatus& status,
                                  int response_code,
                                  const ResponseCookies& cookies,
                                  const std::string& data);

 private:
  FRIEND_TEST_ALL_PREFIXES(Toolbar5ImporterTest, BookmarkParse);

  virtual ~Toolbar5Importer();

  
  enum InternalStateEnum {
    NOT_USED = -1,
    INITIALIZED,
    GET_AUTHORIZATION_TOKEN,
    GET_BOOKMARKS,
    PARSE_BOOKMARKS,
    DONE
  };

  typedef std::vector<string16> BookmarkFolderType;

  
  static const char kT5AuthorizationTokenUrl[];
  static const char kT5FrontEndUrlTemplate[];

  
  static const char kRandomNumberToken[];
  static const char kAuthorizationToken[];
  static const char kAuthorizationTokenPrefix[];
  static const char kAuthorizationTokenSuffix[];
  static const char kMaxNumToken[];
  static const char kMaxTimestampToken[];

  
  static const char kXmlApiReplyXmlTag[];
  static const char kBookmarksXmlTag[];
  static const char kBookmarkXmlTag[];
  static const char kTitleXmlTag[];
  static const char kUrlXmlTag[];
  static const char kTimestampXmlTag[];
  static const char kLabelsXmlTag[];
  static const char kLabelsXmlCloseTag[];
  static const char kLabelXmlTag[];
  static const char kAttributesXmlTag[];

  
  
  
  
  
  void ContinueImport();
  void EndImport();
  void BeginImportBookmarks();
  void EndImportBookmarks();

  
  
  
  void GetAuthenticationFromServer();
  void GetBookmarkDataFromServer(const std::string& response);
  void GetBookmarksFromServerDataResponse(const std::string& response);

  
  bool ParseAuthenticationTokenResponse(const std::string& response,
                                        std::string* token);

  static bool ParseBookmarksFromReader(
      XmlReader* reader,
      std::vector<ProfileWriter::BookmarkEntry>* bookmarks,
      const string16& bookmark_group_string);

  static bool LocateNextOpenTag(XmlReader* reader);
  static bool LocateNextTagByName(XmlReader* reader, const std::string& tag);
  static bool LocateNextTagWithStopByName(
      XmlReader* reader,
      const std::string& tag,
      const std::string& stop);

  static bool ExtractBookmarkInformation(
      XmlReader* reader,
      ProfileWriter::BookmarkEntry* bookmark_entry,
      std::vector<BookmarkFolderType>* bookmark_folders,
      const string16& bookmark_group_string);
  static bool ExtractNamedValueFromXmlReader(XmlReader* reader,
                                             const std::string& name,
                                             std::string* buffer);
  static bool ExtractTitleFromXmlReader(XmlReader* reader,
                                        ProfileWriter::BookmarkEntry* entry);
  static bool ExtractUrlFromXmlReader(XmlReader* reader,
                                      ProfileWriter::BookmarkEntry* entry);
  static bool ExtractTimeFromXmlReader(XmlReader* reader,
                                       ProfileWriter::BookmarkEntry* entry);
  static bool ExtractFoldersFromXmlReader(
      XmlReader* reader,
      std::vector<BookmarkFolderType>* bookmark_folders,
      const string16& bookmark_group_string);

  
  void AddBookmarksToChrome(
      const std::vector<ProfileWriter::BookmarkEntry>& bookmarks);

  InternalStateEnum state_;

  
  uint16 items_to_import_;

  
  
  URLFetcher* token_fetcher_;
  URLFetcher* data_fetcher_;

  DISALLOW_COPY_AND_ASSIGN(Toolbar5Importer);
};

#endif  

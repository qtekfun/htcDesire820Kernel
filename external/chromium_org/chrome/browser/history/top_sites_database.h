// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_TOP_SITES_DATABASE_H_
#define CHROME_BROWSER_HISTORY_TOP_SITES_DATABASE_H_

#include <map>
#include <string>

#include "base/gtest_prod_util.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/history/url_database.h"  
#include "sql/meta_table.h"

namespace base {
class FilePath;
}

namespace sql {
class Connection;
}

namespace history {

class TopSitesDatabase {
 public:
  TopSitesDatabase();
  ~TopSitesDatabase();

  
  
  bool Init(const base::FilePath& db_name);

  

  
  
  void GetPageThumbnails(MostVisitedURLList* urls,
                         std::map<GURL, Images>* thumbnails);

  
  
  
  
  void SetPageThumbnail(const MostVisitedURL& url,
                        int new_rank,
                        const Images& thumbnail);

  
  
  void UpdatePageRank(const MostVisitedURL& url, int new_rank);

  
  bool GetPageThumbnail(const GURL& url, Images* thumbnail);

  
  bool RemoveURL(const MostVisitedURL& url);

 private:
  FRIEND_TEST_ALL_PREFIXES(TopSitesDatabaseTest, Version1);
  FRIEND_TEST_ALL_PREFIXES(TopSitesDatabaseTest, Version2);
  FRIEND_TEST_ALL_PREFIXES(TopSitesDatabaseTest, Version3);
  FRIEND_TEST_ALL_PREFIXES(TopSitesDatabaseTest, Recovery1);
  FRIEND_TEST_ALL_PREFIXES(TopSitesDatabaseTest, Recovery2);
  FRIEND_TEST_ALL_PREFIXES(TopSitesDatabaseTest, Recovery3);
  FRIEND_TEST_ALL_PREFIXES(TopSitesDatabaseTest, AddRemoveEditThumbnails);

  
  
  static const int kRankOfForcedURL;

  
  static const int kRankOfNonExistingURL;

  
  
  bool UpgradeToVersion3();

  
  void AddPageThumbnail(const MostVisitedURL& url,
                        int new_rank,
                        const Images& thumbnail);

  
  void UpdatePageRankNoTransaction(const MostVisitedURL& url, int new_rank);

  
  
  bool UpdatePageThumbnail(const MostVisitedURL& url,
                           const Images& thumbnail);

  
  int GetURLRank(const MostVisitedURL& url);

  
  
  
  bool InitImpl(const base::FilePath& db_name);

  sql::Connection* CreateDB(const base::FilePath& db_name);

  scoped_ptr<sql::Connection> db_;
  sql::MetaTable meta_table_;

  DISALLOW_COPY_AND_ASSIGN(TopSitesDatabase);
};

}  

#endif  

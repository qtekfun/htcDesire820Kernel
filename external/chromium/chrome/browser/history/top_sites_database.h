// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_TOP_SITES_DATABASE_H_
#define CHROME_BROWSER_HISTORY_TOP_SITES_DATABASE_H_
#pragma once

#include <map>
#include <string>

#include "app/sql/meta_table.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/history/url_database.h"  

class FilePath;
class RefCountedMemory;
class SkBitmap;

namespace app {
class Connection;
}

namespace history {

class TopSitesDatabase {
 public:
  TopSitesDatabase();
  ~TopSitesDatabase();

  
  
  bool Init(const FilePath& db_name);

  
  
  
  bool may_need_history_migration() const {
    return may_need_history_migration_;
  }

  

  
  
  void GetPageThumbnails(MostVisitedURLList* urls,
                         std::map<GURL, Images>* thumbnails);

  
  
  
  
  void SetPageThumbnail(const MostVisitedURL& url,
                        int new_rank,
                        const Images& thumbnail);

  
  
  void UpdatePageRank(const MostVisitedURL& url, int new_rank);

  
  bool GetPageThumbnail(const GURL& url, Images* thumbnail);

  
  bool RemoveURL(const MostVisitedURL& url);

 private:
  
  
  bool InitThumbnailTable();

  
  void AddPageThumbnail(const MostVisitedURL& url,
                        int new_rank,
                        const Images& thumbnail);

  
  void UpdatePageRankNoTransaction(const MostVisitedURL& url, int new_rank);

  
  void UpdatePageThumbnail(const MostVisitedURL& url,
                           const Images& thumbnail);

  
  int GetURLRank(const MostVisitedURL& url);

  
  int GetRowCount();

  sql::Connection* CreateDB(const FilePath& db_name);

  
  static std::string GetRedirects(const MostVisitedURL& url);

  
  static void SetRedirects(const std::string& redirects, MostVisitedURL* url);

  scoped_ptr<sql::Connection> db_;
  sql::MetaTable meta_table_;

  
  bool may_need_history_migration_;

  DISALLOW_COPY_AND_ASSIGN(TopSitesDatabase);
};

}  

#endif  

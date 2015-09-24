// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_THUMBNAIL_DATABASE_H_
#define CHROME_BROWSER_HISTORY_THUMBNAIL_DATABASE_H_
#pragma once

#include <vector>

#include "app/sql/connection.h"
#include "app/sql/init_status.h"
#include "app/sql/meta_table.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/history/history_types.h"

class FilePath;
class RefCountedMemory;
struct ThumbnailScore;
class SkBitmap;

namespace base {
class Time;
}

namespace history {

class ExpireHistoryBackend;
class HistoryPublisher;

class ThumbnailDatabase {
 public:
  ThumbnailDatabase();
  ~ThumbnailDatabase();

  
  
  sql::InitStatus Init(const FilePath& db_name,
                       const HistoryPublisher* history_publisher,
                       URLDatabase* url_database);

  
  
  
  
  static sql::InitStatus OpenDatabase(sql::Connection* db,
                                      const FilePath& db_name);

  
  void BeginTransaction();
  void CommitTransaction();
  int transaction_nesting() const {
    return db_.transaction_nesting();
  }

  
  
  void Vacuum();

  

  
  
  
  void SetPageThumbnail(const GURL& url,
                        URLID id,
                        const SkBitmap& thumbnail,
                        const ThumbnailScore& score,
                        base::Time time);

  
  
  bool GetPageThumbnail(URLID id, std::vector<unsigned char>* data);

  
  bool DeleteThumbnail(URLID id);

  
  
  
  bool ThumbnailScoreForId(URLID id, ThumbnailScore* score);

  
  
  bool RecreateThumbnailTable();

  

  
  
  
  bool SetFavicon(FaviconID icon_id,
                  scoped_refptr<RefCountedMemory> icon_data,
                  base::Time time);

  
  bool SetFaviconLastUpdateTime(FaviconID icon_id, base::Time time);

  
  
  
  
  
  
  
  FaviconID GetFaviconIDForFaviconURL(const GURL& icon_url,
                                      int required_icon_type,
                                      IconType* icon_type);

  
  
  bool GetFavicon(FaviconID icon_id,
                  base::Time* last_updated,
                  std::vector<unsigned char>* png_icon_data,
                  GURL* icon_url);

  
  FaviconID AddFavicon(const GURL& icon_url, IconType icon_type);

  
  bool DeleteFavicon(FaviconID id);

  
  
  
  
  
  
  bool GetIconMappingForPageURL(const GURL& page_url,
                                IconType required_icon_type,
                                IconMapping* icon_mapping);

  
  
  
  bool GetIconMappingsForPageURL(const GURL& page_url,
                                 std::vector<IconMapping>* mapping_data);

  
  
  IconMappingID AddIconMapping(const GURL& page_url, FaviconID icon_id);

  
  
  
  bool UpdateIconMapping(IconMappingID mapping_id, FaviconID icon_id);

  
  
  bool DeleteIconMappings(const GURL& page_url);

  
  bool HasMappingFor(FaviconID id);

  
  
  
  
  
  
  
  bool InitTemporaryIconMappingTable() {
    return InitIconMappingTable(&db_, true);
  }

  
  
  
  
  
  
  
  IconMappingID AddToTemporaryIconMappingTable(const GURL& page_url,
                                               const FaviconID icon_id);

  
  
  
  bool CommitTemporaryIconMappingTable();

  

  
  
  
  
  bool InitTemporaryFaviconsTable() {
    return InitFaviconsTable(&db_, true);
  }

  
  
  
  
  
  
  FaviconID CopyToTemporaryFaviconTable(FaviconID source);

  
  
  
  bool CommitTemporaryFaviconTable();

  
  
  bool NeedsMigrationToTopSites();

  
  bool RenameAndDropThumbnails(const FilePath& old_db_file,
                               const FilePath& new_db_file);

 private:
  friend class ExpireHistoryBackend;
  FRIEND_TEST_ALL_PREFIXES(ThumbnailDatabaseTest, UpgradeToVersion4);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, MigrationIconMapping);

  
  
  bool InitThumbnailTable();

  
  
  
  
  
  
  
  bool InitFaviconsTable(sql::Connection* db, bool is_temporary);

  
  bool UpgradeToVersion3();

  
  bool UpgradeToVersion4();

  
  
  bool MigrateIconMappingData(URLDatabase* url_db);

  
  
  
  
  void InitFaviconsIndex();

  
  
  bool InitIconMappingTable(sql::Connection* db, bool is_temporary);

  
  
  
  
  
  void InitIconMappingIndex();

  
  
  
  IconMappingID AddIconMapping(const GURL& page_url,
                               FaviconID icon_id,
                               bool is_temporary);

  sql::Connection db_;
  sql::MetaTable meta_table_;

  
  
  
  
  const HistoryPublisher* history_publisher_;

  
  
  bool use_top_sites_;
};

}  

#endif  

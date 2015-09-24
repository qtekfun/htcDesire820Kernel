// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_PRECACHE_CORE_PRECACHE_URL_TABLE_H_
#define COMPONENTS_PRECACHE_CORE_PRECACHE_URL_TABLE_H_

#include <map>

#include "base/basictypes.h"
#include "base/time/time.h"
#include "url/gurl.h"

namespace sql {
class Connection;
}

namespace precache {

class PrecacheURLTable {
 public:
  PrecacheURLTable();
  ~PrecacheURLTable();

  
  
  
  bool Init(sql::Connection* db);

  
  
  void AddURL(const GURL& url, const base::Time& precache_time);

  
  bool HasURL(const GURL& url);

  
  void DeleteURL(const GURL& url);

  
  void DeleteAllPrecachedBefore(const base::Time& delete_end);

  
  void GetAllDataForTesting(std::map<GURL, base::Time>* map);

 private:
  bool CreateTableIfNonExistent();

  
  sql::Connection* db_;

  DISALLOW_COPY_AND_ASSIGN(PrecacheURLTable);
};

}  

#endif  

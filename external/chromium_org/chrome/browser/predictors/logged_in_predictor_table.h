// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREDICTORS_LOGGED_IN_PREDICTOR_TABLE_H_
#define CHROME_BROWSER_PREDICTORS_LOGGED_IN_PREDICTOR_TABLE_H_

#include <string>

#include "base/containers/hash_tables.h"
#include "base/time/time.h"
#include "chrome/browser/predictors/predictor_table_base.h"
#include "url/gurl.h"

namespace sql {
class Statement;
}

namespace predictors {

class LoggedInPredictorTable : public PredictorTableBase {
 public:
  typedef base::hash_map<std::string, int64> LoggedInStateMap;

  
  
  void AddDomainFromURL(const GURL& url);
  
  void DeleteDomainFromURL(const GURL& url);
  
  void DeleteDomain(const std::string& domain);
  
  
  void HasUserLoggedIn(const GURL& url, bool* is_present,
                       bool* lookup_succeeded);
  void DeleteAllCreatedBetween(const base::Time& delete_begin,
                               const base::Time& delete_end);
  void GetAllData(LoggedInStateMap* state_map);

  static std::string GetKey(const GURL& url);
  static std::string GetKeyFromDomain(const std::string& domain);

 private:
  friend class PredictorDatabaseInternal;

  LoggedInPredictorTable();
  virtual ~LoggedInPredictorTable();

  
  virtual void CreateTableIfNonExistent() OVERRIDE;
  virtual void LogDatabaseStats() OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(LoggedInPredictorTable);
};

}  

#endif  

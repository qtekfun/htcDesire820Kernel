// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREDICTORS_AUTOCOMPLETE_ACTION_PREDICTOR_TABLE_H_
#define CHROME_BROWSER_PREDICTORS_AUTOCOMPLETE_ACTION_PREDICTOR_TABLE_H_

#include <string>
#include <vector>

#include "base/strings/string16.h"
#include "chrome/browser/predictors/predictor_table_base.h"
#include "url/gurl.h"

namespace predictors {

class AutocompleteActionPredictorTable : public PredictorTableBase {
 public:
  struct Row {
    
    
    
    
    typedef std::string Id;

    Row();

    
    Row(const Id& id,
        const base::string16& user_text,
        const GURL& url,
        int number_of_hits,
        int number_of_misses);

    Row(const Row& row);

    Id id;
    base::string16 user_text;
    GURL url;
    int number_of_hits;
    int number_of_misses;
  };

  typedef std::vector<Row> Rows;

  
  void GetRow(const Row::Id& id, Row* row);
  void GetAllRows(Rows* row_buffer);
  void AddRow(const Row& row);
  void UpdateRow(const Row& row);
  void AddAndUpdateRows(const Rows& rows_to_add, const Rows& rows_to_update);
  void DeleteRows(const std::vector<Row::Id>& id_list);
  void DeleteAllRows();

 private:
  friend class PredictorDatabaseInternal;

  AutocompleteActionPredictorTable();
  virtual ~AutocompleteActionPredictorTable();

  
  virtual void CreateTableIfNonExistent() OVERRIDE;
  virtual void LogDatabaseStats() OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(AutocompleteActionPredictorTable);
};

}  

#endif  

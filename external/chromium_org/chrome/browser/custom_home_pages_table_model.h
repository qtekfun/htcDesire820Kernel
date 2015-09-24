// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CUSTOM_HOME_PAGES_TABLE_MODEL_H_
#define CHROME_BROWSER_CUSTOM_HOME_PAGES_TABLE_MODEL_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "chrome/browser/history/history_service.h"
#include "ui/base/models/table_model.h"

class GURL;
class Profile;

namespace ui {
class TableModelObserver;
}


class CustomHomePagesTableModel : public ui::TableModel {
 public:
  explicit CustomHomePagesTableModel(Profile* profile);
  virtual ~CustomHomePagesTableModel();

  
  void SetURLs(const std::vector<GURL>& urls);

  
  
  
  void MoveURLs(int insert_before, const std::vector<int>& index_list);

  
  void Add(int index, const GURL& url);

  
  void Remove(int index);

  
  
  void SetToCurrentlyOpenPages();

  
  std::vector<GURL> GetURLs();

  
  virtual int RowCount() OVERRIDE;
  virtual base::string16 GetText(int row, int column_id) OVERRIDE;
  virtual base::string16 GetTooltip(int row) OVERRIDE;
  virtual void SetObserver(ui::TableModelObserver* observer) OVERRIDE;

 private:
  
  
  struct Entry;

  
  void LoadTitle(Entry* entry);

  
  
  void OnGotTitle(HistoryService::Handle handle,
                  bool found_url,
                  const history::URLRow* row,
                  history::VisitVector* visits);

  
  
  Entry* GetEntryByLoadHandle(CancelableRequestProvider::Handle Entry::* member,
                              CancelableRequestProvider::Handle handle,
                              int* entry_index);

  
  base::string16 FormattedURL(int row) const;

  
  std::vector<Entry> entries_;

  
  Profile* profile_;

  ui::TableModelObserver* observer_;

  
  CancelableRequestConsumer history_query_consumer_;

  DISALLOW_COPY_AND_ASSIGN(CustomHomePagesTableModel);
};

#endif  

// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CUSTOM_HOME_PAGES_TABLE_MODEL_H_
#define CHROME_BROWSER_CUSTOM_HOME_PAGES_TABLE_MODEL_H_
#pragma once

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "chrome/browser/history/history.h"
#include "chrome/browser/favicon_service.h"
#include "ui/base/models/table_model.h"

class GURL;
class Profile;
class SkBitmap;

namespace ui {
class TableModelObserver;
}


class CustomHomePagesTableModel : public ui::TableModel {
 public:
  explicit CustomHomePagesTableModel(Profile* profile);
  virtual ~CustomHomePagesTableModel();

  
  void SetURLs(const std::vector<GURL>& urls);

  
  void Add(int index, const GURL& url);

  
  void Remove(int index);

  
  
  void SetToCurrentlyOpenPages();

  
  std::vector<GURL> GetURLs();

  
  virtual int RowCount() OVERRIDE;
  virtual string16 GetText(int row, int column_id) OVERRIDE;
  virtual SkBitmap GetIcon(int row) OVERRIDE;
  virtual string16 GetTooltip(int row) OVERRIDE;
  virtual void SetObserver(ui::TableModelObserver* observer) OVERRIDE;

 private:
  
  
  struct Entry;

  
  void LoadTitleAndFavicon(Entry* entry);

  
  
  void OnGotTitle(HistoryService::Handle handle,
                  bool found_url,
                  const history::URLRow* row,
                  history::VisitVector* visits);

  
  
  void OnGotFavicon(FaviconService::Handle handle,
                    history::FaviconData favicon);

  
  
  Entry* GetEntryByLoadHandle(CancelableRequestProvider::Handle Entry::* member,
                              CancelableRequestProvider::Handle handle,
                              int* entry_index);

  
  string16 FormattedURL(int row) const;

  
  std::vector<Entry> entries_;

  
  SkBitmap* default_favicon_;

  
  Profile* profile_;

  ui::TableModelObserver* observer_;

  
  CancelableRequestConsumer query_consumer_;

  DISALLOW_COPY_AND_ASSIGN(CustomHomePagesTableModel);
};

#endif  

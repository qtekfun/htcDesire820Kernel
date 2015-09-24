// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POSSIBLE_URL_MODEL_H_
#define CHROME_BROWSER_POSSIBLE_URL_MODEL_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "chrome/browser/history/history.h"
#include "ui/base/models/table_model.h"

class SkBitmap;


class PossibleURLModel : public ui::TableModel {
 public:
  PossibleURLModel();
  virtual ~PossibleURLModel();

  void Reload(Profile *profile);

  void OnHistoryQueryComplete(HistoryService::Handle h,
                              history::QueryResults* result);

  const GURL& GetURL(int row);
  const std::wstring& GetTitle(int row);

  virtual void OnFaviconAvailable(FaviconService::Handle h,
                                  history::FaviconData favicon);

  
  virtual int RowCount() OVERRIDE;
  virtual string16 GetText(int row, int col_id) OVERRIDE;
  virtual SkBitmap GetIcon(int row) OVERRIDE;
  virtual int CompareValues(int row1, int row2, int column_id) OVERRIDE;
  virtual void SetObserver(ui::TableModelObserver* observer) OVERRIDE;

 private:
  
  Profile* profile_;

  
  ui::TableModelObserver* observer_;

  
  CancelableRequestConsumerT<size_t, NULL> consumer_;

  
  struct Result;
  std::vector<Result> results_;

  
  typedef std::map<size_t, SkBitmap> FaviconMap;
  FaviconMap favicon_map_;

  DISALLOW_COPY_AND_ASSIGN(PossibleURLModel);
};

#endif  

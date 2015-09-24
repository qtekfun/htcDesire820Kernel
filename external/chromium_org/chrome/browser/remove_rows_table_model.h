// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_REMOVE_ROWS_TABLE_MODEL_H_
#define CHROME_BROWSER_REMOVE_ROWS_TABLE_MODEL_H_

#include <set>

#include "ui/base/models/table_model.h"

class RemoveRowsTableModel : public ui::TableModel {
 public:
  typedef std::set<size_t> Rows;

  
  virtual bool CanRemoveRows(const Rows& rows) const = 0;

  
  virtual void RemoveRows(const Rows& rows) = 0;

  
  virtual void RemoveAll() = 0;
};

#endif  


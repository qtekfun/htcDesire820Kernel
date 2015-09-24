// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_AUTOFILL_SIMPLE_GRID_LAYOUT_H_
#define CHROME_BROWSER_UI_COCOA_AUTOFILL_SIMPLE_GRID_LAYOUT_H_

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"

class Column;
class ColumnSet;
class Row;
class ViewState;

class SimpleGridLayout {
 public:
  SimpleGridLayout(NSView* host);
  ~SimpleGridLayout();

  
  
  
  
  ColumnSet* AddColumnSet(int id);

  
  ColumnSet* GetColumnSet(int id);

  
  
  
  void AddPaddingRow(int size);

  
  void StartRow(float vertical_resize, int column_set_id);

  
  
  
  ColumnSet* AddRow();

  
  
  void SkipColumns(int col_count);

  
  
  
  void AddView(NSView* view);

  
  void Layout(NSView* superView);

  void SizeRowsAndColumns(float width);

  
  void SkipPaddingColumns();

  
  ColumnSet* GetLastValidColumnSet();

  
  float GetRowHeight(int row);

  
  float GetRowLocation(int row_index) const;

  
  float GetPreferredHeightForWidth(float with);

  int num_rows() const { return static_cast<int>(rows_.size()); }

  
  int next_column() { return next_column_; }
  void AdvanceColumn() { next_column_++; }

 private:
  
  void AddRow(Row* row);

  
  int next_column_;

  int current_auto_id_;  
  ScopedVector<ViewState> view_states_;
  ScopedVector<ColumnSet> column_sets_;
  ScopedVector<Row> rows_;

  NSView* host_;
};

class ColumnSet {
 public:
  explicit ColumnSet(int id);
  ~ColumnSet();

  void AddPaddingColumn(int fixed_width);
  void AddColumn(float resize_percent);

  void CalculateSize(float width);
  void ResetColumnXCoordinates();

  
  int id() const { return id_; }

  int num_columns() const { return static_cast<int>(columns_.size()); }

  
  float GetColumnWidth(int column);

  Column* GetColumn(int column_index) {
    DCHECK(column_index >=0 && column_index < num_columns());
    return columns_[column_index];
  }

  
  float ColumnLocation(int column_index);

 private:
  float CalculateRemainingWidth(float width);
  void DistributeRemainingWidth(float width);

  ScopedVector<Column> columns_;
  int id_;
};

#endif  

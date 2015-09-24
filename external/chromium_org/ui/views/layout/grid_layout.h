// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_LAYOUT_GRID_LAYOUT_H_
#define UI_VIEWS_LAYOUT_GRID_LAYOUT_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "ui/gfx/insets.h"
#include "ui/views/layout/layout_manager.h"
#include "ui/views/view.h"

namespace views {

class Column;
class ColumnSet;
class Row;
class View;

struct ViewState;

class VIEWS_EXPORT GridLayout : public LayoutManager {
 public:
  
  enum Alignment {
    
    
    LEADING,

    
    CENTER,

    
    
    TRAILING,

    
    FILL,

    
    
    BASELINE
  };

  
  enum SizeType {
    
    FIXED,

    
    USE_PREF
  };

  explicit GridLayout(View* host);
  virtual ~GridLayout();

  
  static GridLayout* CreatePanel(View* host);

  
  void SetInsets(int top, int left, int bottom, int right);
  void SetInsets(const gfx::Insets& insets);

  
  
  
  
  ColumnSet* AddColumnSet(int id);

  
  ColumnSet* GetColumnSet(int id);

  
  
  
  void AddPaddingRow(float vertical_resize, int size);

  
  void StartRowWithPadding(float vertical_resize, int column_set_id,
                           float padding_resize, int padding);

  
  void StartRow(float vertical_resize, int column_set_id);

  
  
  void SkipColumns(int col_count);

  
  
  
  
  void AddView(View* view);

  
  
  
  void AddView(View* view, int col_span, int row_span);

  
  
  
  void AddView(View* view, int col_span, int row_span, Alignment h_align,
               Alignment v_align);

  
  
  
  
  
  void AddView(View* view, int col_span, int row_span,
               Alignment h_align, Alignment v_align,
               int pref_width, int pref_height);

  
  
  virtual void Installed(View* host) OVERRIDE;

  
  
  virtual void Uninstalled(View* host) OVERRIDE;

  
  virtual void ViewAdded(View* host, View* view) OVERRIDE;

  
  virtual void ViewRemoved(View* host, View* view) OVERRIDE;

  
  virtual void Layout(View* host) OVERRIDE;

  
  virtual gfx::Size GetPreferredSize(View* host) OVERRIDE;

  virtual int GetPreferredHeightForWidth(View* host, int width) OVERRIDE;

  void set_minimum_size(const gfx::Size& size) { minimum_size_ = size; }

 private:
  
  
  
  
  void SizeRowsAndColumns(bool layout, int width, int height, gfx::Size* pref);

  
  
  void CalculateMasterColumnsIfNecessary();

  
  
  void AddViewState(ViewState* view_state);

  
  
  void AddRow(Row* row);

  
  
  void UpdateRemainingHeightFromRows(ViewState* state);

  
  
  
  void DistributeRemainingHeight(ViewState* state);

  
  void SkipPaddingColumns();

  
  ColumnSet* GetLastValidColumnSet();

  
  View* const host_;

  
  bool calculated_master_columns_;

  
  
  int remaining_row_span_;

  
  int current_row_;

  
  int next_column_;

  
  ColumnSet* current_row_col_set_;

  
  gfx::Insets insets_;

  
  bool adding_view_;

  
  std::vector<ViewState*> view_states_;

  
  std::vector<ColumnSet*> column_sets_;

  
  std::vector<Row*> rows_;

  
  gfx::Size minimum_size_;

  DISALLOW_COPY_AND_ASSIGN(GridLayout);
};

class VIEWS_EXPORT ColumnSet {
 public:
  ~ColumnSet();

  
  
  
  
  
  
  void AddPaddingColumn(float resize_percent, int width);

  
  
  
  
  
  
  
  
  
  void AddColumn(GridLayout::Alignment h_align,
                 GridLayout::Alignment v_align,
                 float resize_percent,
                 GridLayout::SizeType size_type,
                 int fixed_width,
                 int min_width);

  
  
  
  
  
  void LinkColumnSizes(int first, ...);

  
  int id() const { return id_; }

  int num_columns() const { return static_cast<int>(columns_.size()); }

 private:
  friend class GridLayout;

  explicit ColumnSet(int id);

  void AddColumn(GridLayout::Alignment h_align,
                 GridLayout::Alignment v_align,
                 float resize_percent,
                 GridLayout::SizeType size_type,
                 int fixed_width,
                 int min_width,
                 bool is_padding);

  void AddViewState(ViewState* view_state);

  
  void CalculateMasterColumns();
  void AccumulateMasterColumns();

  
  void UnifySameSizedColumnSizes();

  
  
  void UpdateRemainingWidth(ViewState* view_state);

  
  
  void DistributeRemainingWidth(ViewState* view_state);

  
  int LayoutWidth();

  
  int GetColumnWidth(int start_col, int col_span);

  
  
  void ResetColumnXCoordinates();

  
  
  void CalculateSize();

  
  void Resize(int delta);

  
  const int id_;

  
  std::vector<Column*> columns_;

  
  
  std::vector<ViewState*> view_states_;

  
  
  std::vector<Column*> master_columns_;

  DISALLOW_COPY_AND_ASSIGN(ColumnSet);
};

}  

#endif  

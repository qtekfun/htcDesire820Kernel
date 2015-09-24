// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_TABLE_TABLE_VIEW_VIEWS_H_
#define UI_VIEWS_CONTROLS_TABLE_TABLE_VIEW_VIEWS_H_

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "ui/base/models/list_selection_model.h"
#include "ui/base/models/table_model.h"
#include "ui/base/models/table_model_observer.h"
#include "ui/gfx/font.h"
#include "ui/views/view.h"
#include "ui/views/views_export.h"

namespace views {

struct GroupRange;
class TableGrouper;
class TableHeader;
class TableViewObserver;
class TableViewRowBackgroundPainter;
class TableViewTestHelper;

enum TableTypes {
  TEXT_ONLY = 0,
  ICON_AND_TEXT,
};

class VIEWS_EXPORT TableView
    : public views::View,
      public ui::TableModelObserver {
 public:
  
  struct VIEWS_EXPORT VisibleColumn {
    VisibleColumn();
    ~VisibleColumn();

    
    ui::TableColumn column;

    
    int x;

    
    int width;
  };

  
  struct VIEWS_EXPORT SortDescriptor {
    SortDescriptor() : column_id(-1), ascending(true) {}
    SortDescriptor(int column_id, bool ascending)
        : column_id(column_id),
          ascending(ascending) {}

    
    int column_id;

    
    bool ascending;
  };

  typedef std::vector<SortDescriptor> SortDescriptors;

  
  
  
  TableView(ui::TableModel* model,
            const std::vector<ui::TableColumn>& columns,
            TableTypes table_type,
            bool single_selection);
  virtual ~TableView();

  
  
  
  
  void SetModel(ui::TableModel* model);
  ui::TableModel* model() const { return model_; }

  
  View* CreateParentIfNecessary();

  void SetRowBackgroundPainter(
      scoped_ptr<TableViewRowBackgroundPainter> painter);

  
  
  void SetGrouper(TableGrouper* grouper);

  
  int RowCount() const;

  
  
  int SelectedRowCount();

  
  void Select(int model_row);

  
  int FirstSelectedRow();

  const ui::ListSelectionModel& selection_model() const {
    return selection_model_;
  }

  
  void SetColumnVisibility(int id, bool is_visible);
  bool IsColumnVisible(int id) const;

  
  void AddColumn(const ui::TableColumn& col);

  
  
  bool HasColumn(int id) const;

  
  void SetObserver(TableViewObserver* observer) {
    table_view_observer_ = observer;
  }
  TableViewObserver* observer() const { return table_view_observer_; }

  const std::vector<VisibleColumn>& visible_columns() const {
    return visible_columns_;
  }

  
  void SetVisibleColumnWidth(int index, int width);

  
  void ToggleSortOrder(int visible_column_index);

  const SortDescriptors& sort_descriptors() const { return sort_descriptors_; }
  bool is_sorted() const { return !sort_descriptors_.empty(); }

  
  int ModelToView(int model_index) const;

  
  int ViewToModel(int view_index) const;

  int row_height() const { return row_height_; }

  
  virtual void Layout() OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual bool GetTooltipText(const gfx::Point& p,
                              string16* tooltip) const OVERRIDE;
  virtual bool GetTooltipTextOrigin(const gfx::Point& p,
                                    gfx::Point* loc) const OVERRIDE;

  
  virtual void OnModelChanged() OVERRIDE;
  virtual void OnItemsChanged(int start, int length) OVERRIDE;
  virtual void OnItemsAdded(int start, int length) OVERRIDE;
  virtual void OnItemsRemoved(int start, int length) OVERRIDE;

 protected:
  
  virtual gfx::Point GetKeyboardContextMenuLocation() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;

 private:
  friend class TableViewTestHelper;
  struct GroupSortHelper;
  struct SortHelper;

  
  
  
  
  struct VIEWS_EXPORT PaintRegion {
    PaintRegion();
    ~PaintRegion();

    int min_row;
    int max_row;
    int min_column;
    int max_column;
  };

  
  
  enum AdvanceDirection {
    ADVANCE_DECREMENT,
    ADVANCE_INCREMENT,
  };

  
  void NumRowsChanged();

  
  void SetSortDescriptors(const SortDescriptors& sort_descriptors);

  
  
  void SortItemsAndUpdateMapping();

  
  
  
  int CompareRows(int model_row1, int model_row2);

  
  gfx::Rect GetRowBounds(int row) const;

  
  
  gfx::Rect GetCellBounds(int row, int visible_column_index) const;

  
  
  
  void AdjustCellBoundsForText(int visible_column_index,
                               gfx::Rect* bounds) const;

  
  void CreateHeaderIfNecessary();

  
  void UpdateVisibleColumnSizes();

  
  
  PaintRegion GetPaintRegion(const gfx::Rect& bounds) const;

  
  
  gfx::Rect GetPaintBounds(gfx::Canvas* canvas) const;

  
  void SchedulePaintForSelection();

  
  ui::TableColumn FindColumnByID(int id) const;

  
  void SelectByViewIndex(int view_index);

  
  void SetSelectionModel(const ui::ListSelectionModel& new_selection);

  
  void AdvanceSelection(AdvanceDirection direction);

  
  void ConfigureSelectionModelForEvent(const ui::LocatedEvent& event,
                                       ui::ListSelectionModel* model) const;

  
  
  
  void SelectRowsInRangeFrom(int view_index,
                             bool select,
                             ui::ListSelectionModel* model) const;

  
  
  
  GroupRange GetGroupRange(int model_index) const;

  
  
  
  bool GetTooltipImpl(const gfx::Point& location,
                      string16* tooltip,
                      gfx::Point* tooltip_origin) const;

  ui::TableModel* model_;

  std::vector<ui::TableColumn> columns_;

  
  
  std::vector<VisibleColumn> visible_columns_;

  
  
  TableHeader* header_;

  const TableTypes table_type_;

  const bool single_selection_;

  
  TableViewObserver* table_view_observer_;

  
  ui::ListSelectionModel selection_model_;

  gfx::Font font_;

  int row_height_;

  
  
  int last_parent_width_;

  
  int layout_width_;

  
  SortDescriptors sort_descriptors_;

  
  std::vector<int> view_to_model_;
  std::vector<int> model_to_view_;

  scoped_ptr<TableViewRowBackgroundPainter> row_background_painter_;

  TableGrouper* grouper_;

  
  bool in_set_visible_column_width_;

  DISALLOW_COPY_AND_ASSIGN(TableView);
};

}  

#endif  

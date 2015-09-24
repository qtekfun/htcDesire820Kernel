// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_TABLE_TABLE_HEADER_H_
#define UI_VIEWS_CONTROLS_TABLE_TABLE_HEADER_H_

#include "ui/gfx/font.h"
#include "ui/views/view.h"
#include "ui/views/views_export.h"

namespace views {

class TableView;

class VIEWS_EXPORT TableHeader : public views::View {
 public:
  
  static const int kHorizontalPadding;

  
  static const int kSortIndicatorWidth;

  explicit TableHeader(TableView* table);
  virtual ~TableHeader();

  const gfx::Font& font() const { return font_; }

  
  virtual void Layout() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual gfx::NativeCursor GetCursor(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

 private:
  
  struct ColumnResizeDetails {
    ColumnResizeDetails() : column_index(0), initial_x(0), initial_width(0) {}

    
    int column_index;

    
    int initial_x;

    
    int initial_width;
  };

  
  
  bool StartResize(const ui::LocatedEvent& event);

  
  
  void ContinueResize(const ui::LocatedEvent& event);

  
  void ToggleSortOrder(const ui::LocatedEvent& event);

  
  
  int GetResizeColumn(int x) const;

  bool is_resizing() const { return resize_details_.get() != NULL; }

  const gfx::Font font_;

  TableView* table_;

  
  scoped_ptr<ColumnResizeDetails> resize_details_;

  DISALLOW_COPY_AND_ASSIGN(TableHeader);
};

}  

#endif  

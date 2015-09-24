// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_TABLE_TABLE_UTILS_H_
#define UI_VIEWS_CONTROLS_TABLE_TABLE_UTILS_H_

#include <vector>

#include "ui/base/models/table_model.h"
#include "ui/views/views_export.h"

namespace gfx {
class Font;
}

namespace views {

class TableView;

VIEWS_EXPORT extern const int kUnspecifiedColumnWidth;

VIEWS_EXPORT int WidthForContent(const gfx::Font& header_font,
                                 const gfx::Font& content_font,
                                 int padding,
                                 int header_padding,
                                 const ui::TableColumn& column,
                                 ui::TableModel* model);

VIEWS_EXPORT std::vector<int> CalculateTableColumnSizes(
    int width,
    int first_column_padding,
    const gfx::Font& header_font,
    const gfx::Font& content_font,
    int padding,
    int header_padding,
    const std::vector<ui::TableColumn>& columns,
    ui::TableModel* model);

int TableColumnAlignmentToCanvasAlignment(ui::TableColumn::Alignment alignment);

int GetClosestVisibleColumnIndex(const TableView* table, int x);

}  

#endif  

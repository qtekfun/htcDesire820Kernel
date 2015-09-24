// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_MODELS_TABLE_MODEL_H_
#define UI_BASE_MODELS_TABLE_MODEL_H_

#include <vector>

#include "base/strings/string16.h"
#include "third_party/icu/source/i18n/unicode/coll.h"
#include "ui/base/ui_export.h"

namespace gfx {
class ImageSkia;
}

namespace ui {

class TableModelObserver;

class UI_EXPORT TableModel {
 public:
  
  struct Group {
    
    base::string16 title;

    
    int id;
  };
  typedef std::vector<Group> Groups;

  
  virtual int RowCount() = 0;

  
  virtual base::string16 GetText(int row, int column_id) = 0;

  
  
  
  
  virtual gfx::ImageSkia GetIcon(int row);

  
  
  
  virtual base::string16 GetTooltip(int row);

  
  virtual bool ShouldIndent(int row);

  
  
  
  
  
  
  virtual bool HasGroups();

  
  
  virtual Groups GetGroups();

  
  
  virtual int GetGroupID(int row);

  
  
  virtual void SetObserver(TableModelObserver* observer) = 0;

  
  
  
  
  
  
  virtual int CompareValues(int row1, int row2, int column_id);

  
  void ClearCollator();

 protected:
  virtual ~TableModel() {}

  
  icu::Collator* GetCollator();
};

struct UI_EXPORT TableColumn {
  enum Alignment {
    LEFT, RIGHT, CENTER
  };

  TableColumn();
  TableColumn(int id, Alignment alignment, int width, float percent);

  
  int id;

  
  base::string16 title;

  
  Alignment alignment;

  
  
  
  
  
  
  
  
  
  
  
  
  
  int width;
  float percent;

  
  
  
  int min_visible_width;

  
  bool sortable;
};

}  

#endif  

// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_LANGUAGE_ORDER_TABLE_MODEL_H_
#define CHROME_BROWSER_LANGUAGE_ORDER_TABLE_MODEL_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/base/models/table_model.h"

namespace ui {
class TableModelObserver;
}

class LanguageOrderTableModel : public ui::TableModel {
 public:
  LanguageOrderTableModel();

  virtual ~LanguageOrderTableModel();

  
  void SetAcceptLanguagesString(const std::string& language_list);

  
  bool Add(const std::string& language);

  
  void Remove(int index);

  
  
  int GetIndex(const std::string& language);

  
  void MoveDown(int index);

  
  void MoveUp(int index);

  
  std::string GetLanguageList();

  
  virtual int RowCount() OVERRIDE;
  virtual string16 GetText(int row, int column_id) OVERRIDE;
  virtual void SetObserver(ui::TableModelObserver* observer) OVERRIDE;

 private:
  
  std::vector<std::string> languages_;
  std::string comma_separated_language_list_;

  ui::TableModelObserver* observer_;

  DISALLOW_COPY_AND_ASSIGN(LanguageOrderTableModel);
};

#endif  

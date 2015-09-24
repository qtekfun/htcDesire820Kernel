// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_WRENCH_MENU_H_
#define CHROME_BROWSER_UI_VIEWS_WRENCH_MENU_H_
#pragma once

#include <map>
#include <utility>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "ui/base/models/menu_model.h"
#include "views/controls/menu/menu_delegate.h"

class Browser;

namespace views {
class MenuButton;
class MenuItemView;
class View;
}  

class WrenchMenu : public base::RefCounted<WrenchMenu>,
                   public views::MenuDelegate {
 public:
  explicit WrenchMenu(Browser* browser);

  void Init(ui::MenuModel* model);

  
  void RunMenu(views::MenuButton* host);

  
  virtual bool IsItemChecked(int id) const;
  virtual bool IsCommandEnabled(int id) const;
  virtual void ExecuteCommand(int id);
  virtual bool GetAccelerator(int id, views::Accelerator* accelerator);

 private:
  friend class base::RefCounted<WrenchMenu>;

  class CutCopyPasteView;
  class ZoomView;

  typedef std::pair<ui::MenuModel*,int> Entry;
  typedef std::map<int,Entry> IDToEntry;

  ~WrenchMenu();

  
  
  
  void PopulateMenu(views::MenuItemView* parent,
                    ui::MenuModel* model,
                    int* next_id);

  
  
  views::MenuItemView* AppendMenuItem(views::MenuItemView* parent,
                                      ui::MenuModel* model,
                                      int index,
                                      ui::MenuModel::ItemType menu_type,
                                      int* next_id);

  
  
  void CancelAndEvaluate(ui::MenuModel* model, int index);

  
  scoped_ptr<views::MenuItemView> root_;

  
  
  IDToEntry id_to_entry_;

  
  Browser* browser_;

  
  
  
  
  ui::MenuModel* selected_menu_model_;
  int selected_index_;

  DISALLOW_COPY_AND_ASSIGN(WrenchMenu);
};

#endif  

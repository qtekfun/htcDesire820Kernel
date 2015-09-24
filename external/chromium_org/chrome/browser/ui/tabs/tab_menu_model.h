// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TABS_TAB_MENU_MODEL_H_
#define CHROME_BROWSER_UI_TABS_TAB_MENU_MODEL_H_

#include "ui/base/models/simple_menu_model.h"

class TabStripModel;

class TabMenuModel : public ui::SimpleMenuModel {
 public:
  TabMenuModel(ui::SimpleMenuModel::Delegate* delegate,
               TabStripModel* tab_strip,
               int index);
  virtual ~TabMenuModel() {}

 private:
  void Build(TabStripModel* tab_strip, int index);

  DISALLOW_COPY_AND_ASSIGN(TabMenuModel);
};

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_TABS_TAB_STRIP_MENU_CONTROLLER_H_
#define CHROME_BROWSER_UI_GTK_TABS_TAB_STRIP_MENU_CONTROLLER_H_

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/gtk/menu_gtk.h"
#include "chrome/browser/ui/tabs/tab_menu_model.h"

class TabGtk;
class TabStripModel;

namespace gfx {
class Point;
}

namespace ui {
class Accelerator;
}

class TabStripMenuController : public ui::SimpleMenuModel::Delegate,
                               public MenuGtk::Delegate {
 public:
  
  
  
  TabStripMenuController(TabGtk* tab, TabStripModel* model, int index);
  virtual ~TabStripMenuController();
  void RunMenu(const gfx::Point& point, guint32 event_time);
  void Cancel();

 private:
  
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

  
  virtual GtkWidget* GetImageForCommandId(int command_id) const OVERRIDE;

  
  scoped_ptr<MenuGtk> menu_;

  
  
  TabGtk* tab_;

  
  TabMenuModel model_;

  DISALLOW_COPY_AND_ASSIGN(TabStripMenuController);
};

#endif  

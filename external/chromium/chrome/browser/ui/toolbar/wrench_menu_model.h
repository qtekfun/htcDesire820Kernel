// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TOOLBAR_WRENCH_MENU_MODEL_H_
#define CHROME_BROWSER_UI_TOOLBAR_WRENCH_MENU_MODEL_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/tabs/tab_strip_model_observer.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/base/models/accelerator.h"
#include "ui/base/models/button_menu_item_model.h"
#include "ui/base/models/simple_menu_model.h"

class Browser;
class TabStripModel;

namespace {
class MockWrenchMenuModel;
}  

class EncodingMenuModel : public ui::SimpleMenuModel,
                          public ui::SimpleMenuModel::Delegate {
 public:
  explicit EncodingMenuModel(Browser* browser);
  virtual ~EncodingMenuModel();

  
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual void ExecuteCommand(int command_id) OVERRIDE;

 private:
  void Build();

  Browser* browser_;  

  DISALLOW_COPY_AND_ASSIGN(EncodingMenuModel);
};

class ZoomMenuModel : public ui::SimpleMenuModel {
 public:
  explicit ZoomMenuModel(ui::SimpleMenuModel::Delegate* delegate);
  virtual ~ZoomMenuModel();

 private:
  void Build();

  DISALLOW_COPY_AND_ASSIGN(ZoomMenuModel);
};

class ToolsMenuModel : public ui::SimpleMenuModel {
 public:
  ToolsMenuModel(ui::SimpleMenuModel::Delegate* delegate, Browser* browser);
  virtual ~ToolsMenuModel();

 private:
  void Build(Browser* browser);

  scoped_ptr<EncodingMenuModel> encoding_menu_model_;

  DISALLOW_COPY_AND_ASSIGN(ToolsMenuModel);
};

class WrenchMenuModel : public ui::SimpleMenuModel,
                        public ui::SimpleMenuModel::Delegate,
                        public ui::ButtonMenuItemModel::Delegate,
                        public TabStripModelObserver,
                        public NotificationObserver {
 public:
  WrenchMenuModel(ui::AcceleratorProvider* provider, Browser* browser);
  virtual ~WrenchMenuModel();

  
  virtual bool DoesCommandIdDismissMenu(int command_id) const OVERRIDE;

  
  virtual bool IsItemForCommandIdDynamic(int command_id) const OVERRIDE;
  virtual string16 GetLabelForCommandId(int command_id) const OVERRIDE;
  virtual bool GetIconForCommandId(int command_id,
                                   SkBitmap* icon) const OVERRIDE;
  virtual void ExecuteCommand(int command_id) OVERRIDE;
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool IsCommandIdVisible(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;

  
  virtual void TabSelectedAt(TabContentsWrapper* old_contents,
                             TabContentsWrapper* new_contents,
                             int index,
                             bool user_gesture) OVERRIDE;
  virtual void TabReplacedAt(TabStripModel* tab_strip_model,
                             TabContentsWrapper* old_contents,
                             TabContentsWrapper* new_contents,
                             int index) OVERRIDE;
  virtual void TabStripModelDeleted() OVERRIDE;

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  
  Browser* browser() const { return browser_; }

  
  void UpdateZoomControls();

 private:
  
  friend class ::MockWrenchMenuModel;
  WrenchMenuModel();

  void Build();

  
  
  void CreateCutCopyPaste();
  void CreateZoomFullscreen();

  string16 GetSyncMenuLabel() const;

  
  scoped_ptr<ui::ButtonMenuItemModel> edit_menu_item_model_;
  scoped_ptr<ui::ButtonMenuItemModel> zoom_menu_item_model_;

  
  string16 zoom_label_;

  
  scoped_ptr<ToolsMenuModel> tools_menu_model_;

  ui::AcceleratorProvider* provider_;  

  Browser* browser_;  
  TabStripModel* tabstrip_model_; 

  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(WrenchMenuModel);
};

#endif  

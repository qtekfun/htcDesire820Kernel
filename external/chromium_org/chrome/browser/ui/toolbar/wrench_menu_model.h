// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TOOLBAR_WRENCH_MENU_MODEL_H_
#define CHROME_BROWSER_UI_TOOLBAR_WRENCH_MENU_MODEL_H_

#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "content/public/browser/host_zoom_map.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/base/models/button_menu_item_model.h"
#include "ui/base/models/simple_menu_model.h"

class BookmarkSubMenuModel;
class Browser;
class RecentTabsSubMenuModel;
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
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

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
                        public content::NotificationObserver {
 public:
  
  
  static const int kMinBookmarkCommandId = 1;
  static const int kMaxBookmarkCommandId = 1000;

  
  
  static const int kMinRecentTabsCommandId = 1001;
  static const int kMaxRecentTabsCommandId = 1200;

  
  WrenchMenuModel(ui::AcceleratorProvider* provider,
                  Browser* browser,
                  bool is_new_menu);
  virtual ~WrenchMenuModel();

  
  virtual bool DoesCommandIdDismissMenu(int command_id) const OVERRIDE;

  
  virtual bool IsItemForCommandIdDynamic(int command_id) const OVERRIDE;
  virtual base::string16 GetLabelForCommandId(int command_id) const OVERRIDE;
  virtual bool GetIconForCommandId(int command_id,
                                   gfx::Image* icon) const OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool IsCommandIdVisible(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;

  
  virtual void ActiveTabChanged(content::WebContents* old_contents,
                                content::WebContents* new_contents,
                                int index,
                                int reason) OVERRIDE;
  virtual void TabReplacedAt(TabStripModel* tab_strip_model,
                             content::WebContents* old_contents,
                             content::WebContents* new_contents,
                             int index) OVERRIDE;
  virtual void TabStripModelDeleted() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  Browser* browser() const { return browser_; }

  BookmarkSubMenuModel* bookmark_sub_menu_model() const {
    return bookmark_sub_menu_model_.get();
  }

  
  void UpdateZoomControls();

 private:
  
  friend class ::MockWrenchMenuModel;
  WrenchMenuModel();

  void Build(bool is_new_menu);

  void AddGlobalErrorMenuItems();

  
  
  
  
  void CreateCutCopyPasteMenu(bool new_menu);

  
  
  
  void CreateZoomMenu(bool new_menu);

  void OnZoomLevelChanged(const content::HostZoomMap::ZoomLevelChange& change);

  bool ShouldShowNewIncognitoWindowMenuItem();
  bool ShouldShowNewWindowMenuItem();

  
  scoped_ptr<ui::ButtonMenuItemModel> edit_menu_item_model_;
  scoped_ptr<ui::ButtonMenuItemModel> zoom_menu_item_model_;

  
  base::string16 zoom_label_;

  
  scoped_ptr<ToolsMenuModel> tools_menu_model_;

  
  scoped_ptr<BookmarkSubMenuModel> bookmark_sub_menu_model_;

  
  scoped_ptr<RecentTabsSubMenuModel> recent_tabs_sub_menu_model_;

  ui::AcceleratorProvider* provider_;  

  Browser* browser_;  
  TabStripModel* tab_strip_model_; 

  scoped_ptr<content::HostZoomMap::Subscription> zoom_subscription_;
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(WrenchMenuModel);
};

#endif  

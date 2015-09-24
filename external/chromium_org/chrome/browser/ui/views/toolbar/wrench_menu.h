// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TOOLBAR_WRENCH_MENU_H_
#define CHROME_BROWSER_UI_VIEWS_TOOLBAR_WRENCH_MENU_H_

#include <map>
#include <utility>

#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "chrome/browser/bookmarks/base_bookmark_model_observer.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/models/menu_model.h"
#include "ui/views/controls/menu/menu_delegate.h"

class BookmarkMenuDelegate;
class Browser;
class WrenchMenuObserver;

namespace ui {
class NativeTheme;
}

namespace views {
class MenuButton;
struct MenuConfig;
class MenuItemView;
class MenuRunner;
class View;
}  

class WrenchMenu : public views::MenuDelegate,
                   public BaseBookmarkModelObserver,
                   public content::NotificationObserver {
 public:
  
  WrenchMenu(Browser* browser,
             bool use_new_menu,
             bool supports_new_separators);
  virtual ~WrenchMenu();

  void Init(ui::MenuModel* model);

  
  void RunMenu(views::MenuButton* host);

  
  bool IsShowing();

  const views::MenuConfig& GetMenuConfig() const;

  bool use_new_menu() const { return use_new_menu_; }

  void AddObserver(WrenchMenuObserver* observer);
  void RemoveObserver(WrenchMenuObserver* observer);

  
  virtual const gfx::Font* GetLabelFont(int command_id) const OVERRIDE;
  virtual bool GetForegroundColor(int command_id,
                                  bool is_hovered,
                                  SkColor* override_color) const OVERRIDE;
  virtual base::string16 GetTooltipText(int command_id,
                                  const gfx::Point& p) const OVERRIDE;
  virtual bool IsTriggerableEvent(views::MenuItemView* menu,
                                  const ui::Event& e) OVERRIDE;
  virtual bool GetDropFormats(
      views::MenuItemView* menu,
      int* formats,
      std::set<ui::OSExchangeData::CustomFormat>* custom_formats) OVERRIDE;
  virtual bool AreDropTypesRequired(views::MenuItemView* menu) OVERRIDE;
  virtual bool CanDrop(views::MenuItemView* menu,
                       const ui::OSExchangeData& data) OVERRIDE;
  virtual int GetDropOperation(views::MenuItemView* item,
                               const ui::DropTargetEvent& event,
                               DropPosition* position) OVERRIDE;
  virtual int OnPerformDrop(views::MenuItemView* menu,
                            DropPosition position,
                            const ui::DropTargetEvent& event) OVERRIDE;
  virtual bool ShowContextMenu(views::MenuItemView* source,
                               int command_id,
                               const gfx::Point& p,
                               ui::MenuSourceType source_type) OVERRIDE;
  virtual bool CanDrag(views::MenuItemView* menu) OVERRIDE;
  virtual void WriteDragData(views::MenuItemView* sender,
                             ui::OSExchangeData* data) OVERRIDE;
  virtual int GetDragOperations(views::MenuItemView* sender) OVERRIDE;
  virtual int GetMaxWidthForMenu(views::MenuItemView* menu) OVERRIDE;
  virtual bool IsItemChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandEnabled(int command_id) const OVERRIDE;
  virtual void ExecuteCommand(int command_id, int mouse_event_flags) OVERRIDE;
  virtual bool GetAccelerator(int command_id,
                              ui::Accelerator* accelerator) OVERRIDE;
  virtual void WillShowMenu(views::MenuItemView* menu) OVERRIDE;
  virtual void WillHideMenu(views::MenuItemView* menu) OVERRIDE;

  
  virtual void BookmarkModelChanged() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  class CutCopyPasteView;
  class RecentTabsMenuModelDelegate;
  class ZoomView;

  typedef std::pair<ui::MenuModel*,int> Entry;
  typedef std::map<int,Entry> CommandIDToEntry;

  const ui::NativeTheme* GetNativeTheme() const;

  
  
  void PopulateMenu(views::MenuItemView* parent,
                    ui::MenuModel* model);

  
  
  
  
  
  
  
  
  
  views::MenuItemView* AddMenuItem(views::MenuItemView* parent,
                                   int menu_index,
                                   ui::MenuModel* model,
                                   int model_index,
                                   ui::MenuModel::ItemType menu_type,
                                   int height);

  
  
  void CancelAndEvaluate(ui::MenuModel* model, int index);

  
  
  void CreateBookmarkMenu();

  
  
  int ModelIndexFromCommandId(int command_id) const;

  
  views::MenuItemView* root_;

  scoped_ptr<views::MenuRunner> menu_runner_;

  
  
  CommandIDToEntry command_id_to_entry_;

  
  Browser* browser_;

  
  
  
  
  ui::MenuModel* selected_menu_model_;
  int selected_index_;

  
  scoped_ptr<BookmarkMenuDelegate> bookmark_menu_delegate_;

  
  views::MenuItemView* bookmark_menu_;

  
  views::MenuItemView* feedback_menu_item_;

  
  scoped_ptr<RecentTabsMenuModelDelegate> recent_tabs_menu_model_delegate_;

  content::NotificationRegistrar registrar_;

  const bool use_new_menu_;

  const bool supports_new_separators_;

  ObserverList<WrenchMenuObserver> observer_list_;

  DISALLOW_COPY_AND_ASSIGN(WrenchMenu);
};

#endif  

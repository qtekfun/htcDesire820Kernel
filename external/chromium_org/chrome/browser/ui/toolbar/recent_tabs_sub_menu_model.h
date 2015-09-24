// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TOOLBAR_RECENT_TABS_SUB_MENU_MODEL_H_
#define CHROME_BROWSER_UI_TOOLBAR_RECENT_TABS_SUB_MENU_MODEL_H_

#include <set>

#include "base/memory/weak_ptr.h"
#include "chrome/browser/favicon/favicon_service.h"
#include "chrome/browser/sessions/tab_restore_service.h"
#include "chrome/browser/sessions/tab_restore_service_observer.h"
#include "chrome/browser/sync/glue/synced_session.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/base/models/simple_menu_model.h"

class Browser;
struct SessionTab;

namespace browser_sync {
class OpenTabsUIDelegate;
}

namespace chrome {
struct FaviconImageResult;
}

namespace gfx {
class Image;
}

namespace ui {
class AcceleratorProvider;
}

class RecentTabsSubMenuModel : public ui::SimpleMenuModel,
                               public ui::SimpleMenuModel::Delegate,
                               public TabRestoreServiceObserver {
 public:
  
  
  static const int kRecentlyClosedHeaderCommandId;
  static const int kDisabledRecentlyClosedHeaderCommandId;

  
  
  RecentTabsSubMenuModel(ui::AcceleratorProvider* accelerator_provider,
                         Browser* browser,
                         browser_sync::OpenTabsUIDelegate* open_tabs_delegate);
  virtual ~RecentTabsSubMenuModel();

  
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;
  virtual const gfx::Font* GetLabelFontAt(int index) const OVERRIDE;

  int GetMaxWidthForItemAtIndex(int item_index) const;
  bool GetURLAndTitleForItemAtIndex(int index,
                                    std::string* url,
                                    base::string16* title);

 private:
  struct TabNavigationItem;
  typedef std::vector<TabNavigationItem> TabNavigationItems;

  typedef std::vector<SessionID::id_type> WindowItems;

  
  void Build();

  
  void BuildLocalEntries();

  
  void BuildTabsFromOtherDevices();

  
  
  void BuildLocalTabItem(int seesion_id,
                         const base::string16& title,
                         const GURL& url,
                         int curr_model_index);

  
  
  void BuildLocalWindowItem(const SessionID::id_type& window_id,
                            int num_tabs,
                            int curr_model_index);

  
  void BuildOtherDevicesTabItem(const std::string& session_tag,
                                const SessionTab& tab);

  
  void AddDeviceFavicon(int index_in_menu,
                        browser_sync::SyncedSession::DeviceType device_type);

  
  
  void AddTabFavicon(int command_id, const GURL& url);
  void OnFaviconDataAvailable(int command_id,
                              const chrome::FaviconImageResult& image_result);

  
  void ClearLocalEntries();

  
  
  
  int CommandIdToTabVectorIndex(int command_id, TabNavigationItems** tab_items);

  
  
  
  
  
  browser_sync::OpenTabsUIDelegate* GetOpenTabsUIDelegate();

  
  virtual void TabRestoreServiceChanged(TabRestoreService* service) OVERRIDE;
  virtual void TabRestoreServiceDestroyed(TabRestoreService* service) OVERRIDE;

  Browser* browser_;  

  browser_sync::OpenTabsUIDelegate* open_tabs_delegate_;  

  
  ui::Accelerator reopen_closed_tab_accelerator_;

  
  
  
  
  TabNavigationItems local_tab_navigation_items_;

  
  
  
  TabNavigationItems other_devices_tab_navigation_items_;

  
  
  
  
  WindowItems local_window_items_;

  
  
  int last_local_model_index_;

  gfx::Image default_favicon_;

  CancelableTaskTracker local_tab_cancelable_task_tracker_;
  CancelableTaskTracker other_devices_tab_cancelable_task_tracker_;

  base::WeakPtrFactory<RecentTabsSubMenuModel> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(RecentTabsSubMenuModel);
};

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_GLOBAL_MENU_BAR_X11_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_GLOBAL_MENU_BAR_X11_H_

#include <map>
#include <string>

#include "base/compiler_specific.h"
#include "base/prefs/pref_change_registrar.h"
#include "chrome/browser/command_observer.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/sessions/tab_restore_service.h"
#include "chrome/browser/sessions/tab_restore_service_observer.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/glib/glib_signal.h"
#include "ui/views/widget/desktop_aura/desktop_root_window_host_observer_x11.h"

typedef struct _DbusmenuMenuitem DbusmenuMenuitem;
typedef struct _DbusmenuServer   DbusmenuServer;

namespace history {
class TopSites;
}

namespace ui {
class Accelerator;
}

class Browser;
class BrowserView;
class Profile;

class BrowserDesktopRootWindowHostX11;
struct GlobalMenuBarCommand;

class GlobalMenuBarX11 : public CommandObserver,
                         public content::NotificationObserver,
                         public TabRestoreServiceObserver,
                         public views::DesktopRootWindowHostObserverX11 {
 public:
  GlobalMenuBarX11(BrowserView* browser_view,
                   BrowserDesktopRootWindowHostX11* host);
  virtual ~GlobalMenuBarX11();

  
  static std::string GetPathForWindow(unsigned long xid);

 private:
  struct HistoryItem;
  typedef std::map<int, DbusmenuMenuitem*> CommandIDMenuItemMap;

  
  DbusmenuMenuitem* BuildSeparator();

  
  
  DbusmenuMenuitem* BuildMenuItem(const std::string& label, int tag_id);

  
  void InitServer(unsigned long xid);

  
  void Disable();

  
  
  DbusmenuMenuitem* BuildStaticMenu(DbusmenuMenuitem* parent,
                                    int menu_str_id,
                                    GlobalMenuBarCommand* commands);

  
  void RegisterAccelerator(DbusmenuMenuitem* item,
                           const ui::Accelerator& accelerator);

  
  HistoryItem* HistoryItemForTab(const TabRestoreService::Tab& entry);

  
  void AddHistoryItemToMenu(HistoryItem* item,
                            DbusmenuMenuitem* menu,
                            int tag,
                            int index);

  
  void GetTopSitesData();

  
  void OnTopSitesReceived(const history::MostVisitedURLList& visited_list);

  
  void OnBookmarkBarVisibilityChanged();

  
  int GetIndexOfMenuItemWithTag(DbusmenuMenuitem* menu, int tag_id);

  
  
  
  void ClearMenuSection(DbusmenuMenuitem* menu, int tag_id);

  
  static void DeleteHistoryItem(void* void_item);

  
  virtual void EnabledStateChangedForCommand(int id, bool enabled) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void TabRestoreServiceChanged(TabRestoreService* service) OVERRIDE;
  virtual void TabRestoreServiceDestroyed(TabRestoreService* service) OVERRIDE;

  
  virtual void OnWindowMapped(unsigned long xid) OVERRIDE;
  virtual void OnWindowUnmapped(unsigned long xid) OVERRIDE;

  CHROMEG_CALLBACK_1(GlobalMenuBarX11, void, OnItemActivated, DbusmenuMenuitem*,
                     unsigned int);
  CHROMEG_CALLBACK_1(GlobalMenuBarX11, void, OnHistoryItemActivated,
                     DbusmenuMenuitem*, unsigned int);
  CHROMEG_CALLBACK_0(GlobalMenuBarX11, void, OnHistoryMenuAboutToShow,
                     DbusmenuMenuitem*);

  Browser* browser_;
  Profile* profile_;
  BrowserView* browser_view_;
  BrowserDesktopRootWindowHostX11* host_;

  
  
  CommandIDMenuItemMap id_to_menu_item_;

  DbusmenuServer* server_;
  DbusmenuMenuitem* root_item_;
  DbusmenuMenuitem* history_menu_;

  
  PrefChangeRegistrar pref_change_registrar_;

  history::TopSites* top_sites_;

  TabRestoreService* tab_restore_service_;  

  content::NotificationRegistrar registrar_;

  
  base::WeakPtrFactory<GlobalMenuBarX11> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(GlobalMenuBarX11);
};

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_GLOBAL_HISTORY_MENU_H_
#define CHROME_BROWSER_UI_GTK_GLOBAL_HISTORY_MENU_H_

#include <map>

#include "base/compiler_specific.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/sessions/tab_restore_service.h"
#include "chrome/browser/sessions/tab_restore_service_observer.h"
#include "chrome/browser/ui/gtk/global_menu_owner.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"

class Browser;
class Profile;

namespace history {
class TopSites;
}

typedef struct _GdkPixbuf GdkPixbuf;

class GlobalHistoryMenu : public GlobalMenuOwner,
                          public content::NotificationObserver,
                          public TabRestoreServiceObserver {
 public:
  explicit GlobalHistoryMenu(Browser* browser);
  virtual ~GlobalHistoryMenu();

  
  
  virtual void Init(GtkWidget* history_menu,
                    GtkWidget* history_menu_item) OVERRIDE;

 private:
  class HistoryItem;
  struct ClearMenuClosure;
  struct GetIndexClosure;

  typedef std::map<GtkWidget*, HistoryItem*> MenuItemToHistoryMap;

  
  void GetTopSitesData();

  
  void OnTopSitesReceived(const history::MostVisitedURLList& visited_list);

  
  
  HistoryItem* HistoryItemForMenuItem(GtkWidget* menu_item);

  
  bool HasValidHistoryItemForTab(const TabRestoreService::Tab& entry);

  
  HistoryItem* HistoryItemForTab(const TabRestoreService::Tab& entry);

  
  GtkWidget* AddHistoryItemToMenu(HistoryItem* item,
                                  GtkWidget* menu,
                                  int tag,
                                  int index);

  
  int GetIndexOfMenuItemWithTag(GtkWidget* menu, int tag_id);

  
  
  
  void ClearMenuSection(GtkWidget* menu, int tag);

  
  static void GetIndexCallback(GtkWidget* widget, GetIndexClosure* closure);

  
  static void ClearMenuCallback(GtkWidget* widget, ClearMenuClosure* closure);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void TabRestoreServiceChanged(TabRestoreService* service) OVERRIDE;
  virtual void TabRestoreServiceDestroyed(TabRestoreService* service) OVERRIDE;

  CHROMEGTK_CALLBACK_0(GlobalHistoryMenu, void, OnRecentlyClosedItemActivated);

  
  
  
  
  
  
  CHROMEGTK_CALLBACK_0(GlobalHistoryMenu, void, OnMenuActivate);

  Browser* browser_;
  Profile* profile_;

  
  
  GtkWidget* history_menu_;

  history::TopSites* top_sites_;

  TabRestoreService* tab_restore_service_;  

  
  MenuItemToHistoryMap menu_item_history_map_;

  content::NotificationRegistrar registrar_;

  
  base::WeakPtrFactory<GlobalHistoryMenu> weak_ptr_factory_;
};

#endif  

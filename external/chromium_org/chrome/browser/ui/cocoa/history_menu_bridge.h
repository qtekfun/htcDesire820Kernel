// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_HISTORY_MENU_BRIDGE_H_
#define CHROME_BROWSER_UI_COCOA_HISTORY_MENU_BRIDGE_H_

#import <Cocoa/Cocoa.h>
#include <map>
#include <vector>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/common/cancelable_request.h"
#import "chrome/browser/favicon/favicon_service.h"
#include "chrome/browser/history/history_service.h"
#include "chrome/browser/sessions/session_id.h"
#include "chrome/browser/sessions/tab_restore_service.h"
#include "chrome/browser/sessions/tab_restore_service_observer.h"
#import "chrome/browser/ui/cocoa/main_menu_item.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "content/public/browser/notification_observer.h"

class NotificationRegistrar;
class PageUsageData;
class Profile;
class TabRestoreService;
@class HistoryMenuCocoaController;

namespace {
class HistoryMenuBridgeTest;
}

namespace chrome {
struct FaviconImageResult;
}

class HistoryMenuBridge : public content::NotificationObserver,
                          public TabRestoreServiceObserver,
                          public MainMenuItem {
 public:
  
  
  struct HistoryItem {
   public:
    HistoryItem();
    
    HistoryItem(const HistoryItem& copy);
    ~HistoryItem();

    
    base::string16 title;
    
    GURL url;
    
    base::scoped_nsobject<NSImage> icon;

    
    
    
    bool icon_requested;
    
    CancelableTaskTracker::TaskId icon_task_id;

    
    
    
    
    
    base::scoped_nsobject<NSMenuItem> menu_item;

    
    
    
    
    
    SessionID::id_type session_id;

    
    
    
    
    std::vector<HistoryItem*> tabs;

   private:
    
    void operator=(const HistoryItem&);
  };

  
  
  
  
  enum Tags {
    kRecentlyClosedSeparator = 400,  
    kRecentlyClosedTitle = 401,  
    kRecentlyClosed = 420,  
    kVisitedSeparator = 440,  
    kVisitedTitle = 441,  
    kVisited = 460,  
    kShowFullSeparator = 480  
  };

  explicit HistoryMenuBridge(Profile* profile);
  virtual ~HistoryMenuBridge();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void TabRestoreServiceChanged(TabRestoreService* service) OVERRIDE;
  virtual void TabRestoreServiceDestroyed(TabRestoreService* service) OVERRIDE;

  
  virtual void ResetMenu() OVERRIDE;
  virtual void BuildMenu() OVERRIDE;

  
  
  HistoryItem* HistoryItemForMenuItem(NSMenuItem* item);

  
  
  HistoryService* service();
  Profile* profile();

 protected:
  
  virtual NSMenu* HistoryMenu();

  
  
  
  
  
  void ClearMenuSection(NSMenu* menu, NSInteger tag);

  
  
  
  
  NSMenuItem* AddItemToMenu(HistoryItem* item,
                            NSMenu* menu,
                            NSInteger tag,
                            NSInteger index);

  
  
  
  void Init();

  
  void CreateMenu();

  
  
  void OnVisitedHistoryResults(CancelableRequestProvider::Handle handle,
                               history::QueryResults* results);

  
  
  HistoryItem* HistoryItemForTab(const TabRestoreService::Tab& entry);

  
  
  void GetFaviconForHistoryItem(HistoryItem* item);

  
  
  
  
  void GotFaviconData(HistoryItem* item,
                      const chrome::FaviconImageResult& image_result);

  
  
  void CancelFaviconRequest(HistoryItem* item);

 private:
  friend class ::HistoryMenuBridgeTest;
  friend class HistoryMenuCocoaControllerTest;

  base::scoped_nsobject<HistoryMenuCocoaController> controller_;  

  Profile* profile_;  
  HistoryService* history_service_;  
  TabRestoreService* tab_restore_service_;  

  content::NotificationRegistrar registrar_;
  CancelableRequestConsumer cancelable_request_consumer_;
  CancelableTaskTracker cancelable_task_tracker_;

  
  
  std::map<NSMenuItem*, HistoryItem*> menu_item_map_;

  
  CancelableRequestConsumerTSimple<HistoryItem*> favicon_consumer_;

  
  
  
  
  bool create_in_progress_;
  bool need_recreate_;

  
  base::scoped_nsobject<NSImage> default_favicon_;

  DISALLOW_COPY_AND_ASSIGN(HistoryMenuBridge);
};

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_HISTORY_MENU_BRIDGE_H_
#define CHROME_BROWSER_UI_COCOA_HISTORY_MENU_BRIDGE_H_
#pragma once

#import <Cocoa/Cocoa.h>
#include <map>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_nsobject.h"
#import "chrome/browser/favicon_service.h"
#include "chrome/browser/history/history.h"
#include "chrome/browser/sessions/session_id.h"
#include "chrome/browser/sessions/tab_restore_service.h"
#include "chrome/browser/sessions/tab_restore_service_observer.h"
#include "content/browser/cancelable_request.h"
#include "content/common/notification_observer.h"

class NavigationEntry;
class NotificationRegistrar;
class PageUsageData;
class Profile;
class TabNavigationEntry;
class TabRestoreService;
@class HistoryMenuCocoaController;

namespace {

class HistoryMenuBridgeTest;

}

class HistoryMenuBridge : public NotificationObserver,
                          public TabRestoreServiceObserver {
 public:
  
  
  struct HistoryItem {
   public:
    HistoryItem();
    
    HistoryItem(const HistoryItem& copy);
    ~HistoryItem();

    
    string16 title;
    
    GURL url;
    
    scoped_nsobject<NSImage> icon;

    
    
    
    bool icon_requested;
    
    FaviconService::Handle icon_handle;

    
    
    
    
    
    scoped_nsobject<NSMenuItem> menu_item;

    
    
    
    
    
    SessionID::id_type session_id;

    
    
    
    
    std::vector<HistoryItem*> tabs;

   private:
    
    void operator=(const HistoryItem&);
  };

  
  
  
  
  enum Tags {
    kMostVisitedSeparator = 400,  
    kMostVisitedTitle = 401,  
    kMostVisited = 420,  
    kRecentlyClosedSeparator = 440,  
    kRecentlyClosedTitle = 441,  
    kRecentlyClosed = 460,  
    kShowFullSeparator = 480  
  };

  explicit HistoryMenuBridge(Profile* profile);
  virtual ~HistoryMenuBridge();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual void TabRestoreServiceChanged(TabRestoreService* service);
  virtual void TabRestoreServiceDestroyed(TabRestoreService* service);

  
  
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
                               std::vector<PageUsageData*>* results);

  
  
  HistoryItem* HistoryItemForTab(const TabRestoreService::Tab& entry);

  
  
  void GetFaviconForHistoryItem(HistoryItem* item);

  
  
  
  
  void GotFaviconData(FaviconService::Handle handle,
                      history::FaviconData favicon);

  
  
  void CancelFaviconRequest(HistoryItem* item);

 private:
  friend class ::HistoryMenuBridgeTest;
  friend class HistoryMenuCocoaControllerTest;

  scoped_nsobject<HistoryMenuCocoaController> controller_;  

  Profile* profile_;  
  HistoryService* history_service_;  
  TabRestoreService* tab_restore_service_;  

  NotificationRegistrar registrar_;
  CancelableRequestConsumer cancelable_request_consumer_;

  
  
  std::map<NSMenuItem*, HistoryItem*> menu_item_map_;

  
  CancelableRequestConsumerTSimple<HistoryItem*> favicon_consumer_;

  
  
  
  
  bool create_in_progress_;
  bool need_recreate_;

  
  scoped_nsobject<NSImage> default_favicon_;

  DISALLOW_COPY_AND_ASSIGN(HistoryMenuBridge);
};

#endif  

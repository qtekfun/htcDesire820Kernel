// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_BROWSER_EVENT_ROUTER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_BROWSER_EVENT_ROUTER_H_
#pragma once

#include <map>
#include <string>

#include "base/basictypes.h"
#include "chrome/browser/extensions/extension_tabs_module.h"
#include "chrome/browser/tabs/tab_strip_model_observer.h"
#include "chrome/browser/ui/browser_list.h"
#include "content/common/notification_registrar.h"
#if defined(TOOLKIT_VIEWS)
#include "views/focus/focus_manager.h"
#include "views/view.h"
#elif defined(TOOLKIT_GTK)
#include "ui/base/x/active_window_watcher_x.h"
#endif

class ExtensionBrowserEventRouter : public TabStripModelObserver,
#if defined(TOOLKIT_VIEWS)
                                    public views::WidgetFocusChangeListener,
#elif defined(TOOLKIT_GTK)
                                    public ui::ActiveWindowWatcherX::Observer,
#endif
                                    public BrowserList::Observer,
                                    public NotificationObserver {
 public:
  explicit ExtensionBrowserEventRouter(Profile* profile);
  ~ExtensionBrowserEventRouter();

  
  void Init();

  
  virtual void OnBrowserAdded(const Browser* browser);
  virtual void OnBrowserRemoved(const Browser* browser);
  virtual void OnBrowserSetLastActive(const Browser* browser);

#if defined(TOOLKIT_VIEWS)
  virtual void NativeFocusWillChange(gfx::NativeView focused_before,
                                     gfx::NativeView focused_now);
#elif defined(TOOLKIT_GTK)
  virtual void ActiveWindowChanged(GdkWindow* active_window);
#endif

  
  
  void OnBrowserWindowReady(const Browser* browser);

  
  virtual void TabInsertedAt(TabContentsWrapper* contents, int index,
                             bool foreground);
  virtual void TabClosingAt(TabStripModel* tab_strip_model,
                            TabContentsWrapper* contents,
                            int index);
  virtual void TabDetachedAt(TabContentsWrapper* contents, int index);
  virtual void TabSelectedAt(TabContentsWrapper* old_contents,
                             TabContentsWrapper* new_contents,
                             int index,
                             bool user_gesture);
  virtual void TabMoved(TabContentsWrapper* contents, int from_index,
                        int to_index);
  virtual void TabChangedAt(TabContentsWrapper* contents, int index,
                            TabChangeType change_type);
  virtual void TabReplacedAt(TabStripModel* tab_strip_model,
                             TabContentsWrapper* old_contents,
                             TabContentsWrapper* new_contents,
                             int index);
  virtual void TabPinnedStateChanged(TabContentsWrapper* contents, int index);
  virtual void TabStripEmpty();

  
  void PageActionExecuted(Profile* profile,
                          const std::string& extension_id,
                          const std::string& page_action_id,
                          int tab_id,
                          const std::string& url,
                          int button);
  
  void BrowserActionExecuted(Profile* profile,
                             const std::string& extension_id,
                             Browser* browser);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);
 private:
  
  void TabCreatedAt(TabContents* contents, int index, bool foreground);

  
  
  void TabUpdated(TabContents* contents, bool did_navigate);

  
  
  void DispatchTabUpdatedEvent(TabContents* contents,
                               DictionaryValue* changed_properties);

  
  
  
  void DispatchOldPageActionEvent(Profile* profile,
    const std::string& extension_id,
    const std::string& page_action_id,
    int tab_id,
    const std::string& url,
    int button);

  
  
  void RegisterForBrowserNotifications(const Browser* browser);

  
  
  void RegisterForTabNotifications(TabContents* contents);

  
  void UnregisterForTabNotifications(TabContents* contents);

  NotificationRegistrar registrar_;

  bool initialized_;

  
  
  
  
  
  class TabEntry {
   public:
    
    
    
    TabEntry();

    
    
    
    
    
    
    DictionaryValue* UpdateLoadState(const TabContents* contents);

    
    
    
    DictionaryValue* DidNavigate(const TabContents* contents);

   private:
    
    
    
    
    bool complete_waiting_on_load_;

    GURL url_;
  };

  
  
  TabEntry* GetTabEntry(const TabContents* contents);

  std::map<int, TabEntry> tab_entries_;

  
  
  int focused_window_id_;

  
  
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionBrowserEventRouter);
};

#endif  

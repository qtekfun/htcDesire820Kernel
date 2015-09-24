// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_TAB_CLOSEABLE_STATE_WATCHER_H_
#define CHROME_BROWSER_CHROMEOS_TAB_CLOSEABLE_STATE_WATCHER_H_
#pragma once

#include <vector>

#include "chrome/browser/tab_closeable_state_watcher.h"
#include "chrome/browser/tabs/tab_strip_model_observer.h"
#include "chrome/browser/ui/browser_list.h"
#include "content/common/notification_registrar.h"

namespace chromeos {


class TabCloseableStateWatcher : public ::TabCloseableStateWatcher,
                                 public BrowserList::Observer,
                                 public NotificationObserver {
 public:
  TabCloseableStateWatcher();
  virtual ~TabCloseableStateWatcher();

  
  virtual bool CanCloseTab(const Browser* browser) const;
  virtual bool CanCloseBrowser(Browser* browser);
  virtual void OnWindowCloseCanceled(Browser* browser);

 private:
  enum BrowserActionType {
    NONE = 0,         
    OPEN_WINDOW = 1,  
    OPEN_NTP = 2,     
  };

  
  virtual void OnBrowserAdded(const Browser* browser);
  virtual void OnBrowserRemoved(const Browser* browser);

  
  virtual void Observe(NotificationType type, const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  
  virtual void OnTabStripChanged(const Browser* browser, bool closing_last_tab);

  

  
  
  
  
  
  void CheckAndUpdateState(const Browser* browser_to_check);

  
  void SetCloseableState(bool closeable);

  
  
  bool CanCloseBrowserImpl(const Browser* browser,
                           BrowserActionType* action_type);

  

  
  
  bool can_close_tab_;

  
  
  bool signing_off_;

  
  bool guest_session_;

  
  
  
  bool waiting_for_browser_;

  NotificationRegistrar notification_registrar_;

  
  
  
  
  class TabStripWatcher : public TabStripModelObserver {
   public:
    TabStripWatcher(TabCloseableStateWatcher* main_watcher,
                    const Browser* browser);
    virtual ~TabStripWatcher();

    
    virtual void TabInsertedAt(TabContentsWrapper* contents, int index,
                               bool foreground);
    virtual void TabClosingAt(TabStripModel* tab_strip_model,
                              TabContentsWrapper* contents,
                              int index);
    virtual void TabDetachedAt(TabContentsWrapper* contents, int index);
    virtual void TabChangedAt(TabContentsWrapper* contents, int index,
                              TabChangeType change_type);

    const Browser* browser() const {
      return browser_;
    }

   private:
    TabCloseableStateWatcher* main_watcher_;
    const Browser* browser_;

    DISALLOW_COPY_AND_ASSIGN(TabStripWatcher);
  };

  friend class TabStripWatcher;

  std::vector<TabStripWatcher*> tabstrip_watchers_;

  DISALLOW_COPY_AND_ASSIGN(TabCloseableStateWatcher);
};

}  

#endif  

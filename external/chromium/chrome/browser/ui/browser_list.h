// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BROWSER_LIST_H_
#define CHROME_BROWSER_UI_BROWSER_LIST_H_
#pragma once

#include <vector>

#include "base/observer_list.h"
#include "chrome/browser/ui/browser.h"

class BrowserList {
 public:
  typedef std::vector<Browser*> BrowserVector;
  typedef BrowserVector::iterator iterator;
  typedef BrowserVector::const_iterator const_iterator;
  typedef BrowserVector::const_reverse_iterator const_reverse_iterator;

  
  
  class Observer {
   public:
    
    virtual void OnBrowserAdded(const Browser* browser) = 0;

    
    virtual void OnBrowserRemoved(const Browser* browser) = 0;

    
    virtual void OnBrowserSetLastActive(const Browser* browser) {}

   protected:
    virtual ~Observer() {}
  };

  
  
  
  static void AddBrowser(Browser* browser);
  static void RemoveBrowser(Browser* browser);

  static void AddObserver(Observer* observer);
  static void RemoveObserver(Observer* observer);

  
  
  static void SetLastActive(Browser* browser);

  
  
  
  
  
  
  
  
  
  static Browser* GetLastActive();

  
  
  
  static Browser* GetLastActiveWithProfile(Profile *profile);

  
  
  
  
  
  static Browser* FindBrowserWithType(Profile* p, Browser::Type t,
                                      bool match_incognito);

  
  
  
  
  
  static Browser* FindBrowserWithFeature(Profile* p,
                                         Browser::WindowFeature feature);

  
  
  
  static Browser* FindBrowserWithProfile(Profile* p);

  
  
  static Browser* FindBrowserWithID(SessionID::id_type desired_id);

  
  
  
  
  static bool CanRestartForUpdate();

  
  static void Exit();

  
  
  
  
  static void CloseAllBrowsersAndExit();

  
  
  
  static void CloseAllBrowsers();

  
  static void SessionEnding();

  
  static bool HasBrowserWithProfile(Profile* profile);

  
  
  
  
  static void StartKeepAlive();

  
  
  static void EndKeepAlive();

  
  
  static bool WillKeepAlive();

  
  
  static const_iterator begin() { return browsers_.begin(); }
  static const_iterator end() { return browsers_.end(); }

  static bool empty() { return browsers_.empty(); }
  static size_t size() { return browsers_.size(); }

  
  
  
  
  static const_reverse_iterator begin_last_active() {
    return last_active_browsers_.rbegin();
  }

  static const_reverse_iterator end_last_active() {
    return last_active_browsers_.rend();
  }

  
  
  static size_t GetBrowserCount(Profile* p);

  
  
  static size_t GetBrowserCountForType(Profile* p, Browser::Type type);

  
  static bool IsOffTheRecordSessionActive();

  
  
  static void NotifyAndTerminate(bool fast_path);

  
  static void AllBrowsersClosedAndAppExiting();

 private:
  
  static void RemoveBrowserFrom(Browser* browser, BrowserVector* browser_list);
  static void MarkAsCleanShutdown();
#if defined(OS_CHROMEOS)
  static bool NeedBeforeUnloadFired();
  static bool PendingDownloads();
  static void NotifyWindowManagerAboutSignout();

  static bool signout_;
#endif

  static BrowserVector browsers_;
  static BrowserVector last_active_browsers_;
  static ObserverList<Observer> observers_;

  
  
  static int keep_alive_count_;
};

class TabContentsWrapper;

class TabContentsIterator {
 public:
  TabContentsIterator();

  
  bool done() const {
    return cur_ == NULL;
  }

  
  
  Browser* browser() const {
    return *browser_iterator_;
  }

  
  TabContentsWrapper* operator->() const {
    return cur_;
  }
  TabContentsWrapper* operator*() const {
    return cur_;
  }

  
  TabContentsWrapper* operator++() {  
    Advance();
    return cur_;
  }
  TabContentsWrapper* operator++(int) {  
    TabContentsWrapper* tmp = cur_;
    Advance();
    return tmp;
  }

 private:
  
  
  
  void Advance();

  
  BrowserList::const_iterator browser_iterator_;

  
  int web_view_index_;

  
  
  
  TabContentsWrapper* cur_;
};

#endif  

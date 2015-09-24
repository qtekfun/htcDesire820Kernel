// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BROWSER_LIST_H_
#define CHROME_BROWSER_UI_BROWSER_LIST_H_

#include <vector>

#include "base/basictypes.h"
#include "base/lazy_instance.h"
#include "base/observer_list.h"
#include "chrome/browser/ui/host_desktop.h"

class Browser;
class Profile;

namespace chrome {
class BrowserListObserver;
}

class BrowserList {
 public:
  typedef std::vector<Browser*> BrowserVector;
  typedef BrowserVector::const_iterator const_iterator;
  typedef BrowserVector::const_reverse_iterator const_reverse_iterator;

  
  Browser* GetLastActive() const;

  
  
  const_iterator begin() const { return browsers_.begin(); }
  const_iterator end() const { return browsers_.end(); }

  bool empty() const { return browsers_.empty(); }
  size_t size() const { return browsers_.size(); }

  Browser* get(size_t index) const { return browsers_[index]; }

  
  
  
  
  const_reverse_iterator begin_last_active() const {
    return last_active_browsers_.rbegin();
  }
  const_reverse_iterator end_last_active() const {
    return last_active_browsers_.rend();
  }

  static BrowserList* GetInstance(chrome::HostDesktopType type);

  
  
  
  static void AddBrowser(Browser* browser);
  static void RemoveBrowser(Browser* browser);

  
  
  
  static void AddObserver(chrome::BrowserListObserver* observer);
  static void RemoveObserver(chrome::BrowserListObserver* observer);

  
  
  
  
  static void SetLastActive(Browser* browser);

  
  static void CloseAllBrowsersWithProfile(Profile* profile);

  
  
  static bool IsOffTheRecordSessionActive();

  
  
  static bool IsOffTheRecordSessionActiveForProfile(Profile* profile);

 private:
  BrowserList();
  ~BrowserList();

  
  static void RemoveBrowserFrom(Browser* browser, BrowserVector* browser_list);

  
  BrowserVector browsers_;
  
  
  BrowserVector last_active_browsers_;

  
  
  static base::LazyInstance<ObserverList<chrome::BrowserListObserver> >::Leaky
      observers_;

  
  static BrowserList* native_instance_;
  static BrowserList* ash_instance_;

  DISALLOW_COPY_AND_ASSIGN(BrowserList);
};

#endif  

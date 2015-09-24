// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SESSIONS_TAB_RESTORE_SERVICE_OBSERVER_H_
#define CHROME_BROWSER_SESSIONS_TAB_RESTORE_SERVICE_OBSERVER_H_

class TabRestoreService;

class TabRestoreServiceObserver {
 public:
  
  virtual void TabRestoreServiceChanged(TabRestoreService* service) = 0;

  
  
  virtual void TabRestoreServiceDestroyed(TabRestoreService* service) = 0;

  
  virtual void TabRestoreServiceLoaded(TabRestoreService* service) {}

 protected:
  virtual ~TabRestoreServiceObserver() {}
};

#endif  

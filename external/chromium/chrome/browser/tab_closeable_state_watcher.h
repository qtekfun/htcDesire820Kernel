// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CLOSEABLE_STATE_WATCHER_H_
#define CHROME_BROWSER_TAB_CLOSEABLE_STATE_WATCHER_H_
#pragma once

#include "base/basictypes.h"

class Browser;


class TabCloseableStateWatcher {
 public:
  TabCloseableStateWatcher() {}
  virtual ~TabCloseableStateWatcher() {}

  
  
  static TabCloseableStateWatcher* Create();

  
  
  
  
  
  
  
  
  virtual bool CanCloseTab(const Browser* browser) const;

  
  
  virtual bool CanCloseBrowser(Browser* browser);

  
  
  
  virtual void OnWindowCloseCanceled(Browser* browser) {}

 private:
  DISALLOW_COPY_AND_ASSIGN(TabCloseableStateWatcher);
};

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BROWSER_LIST_OBSERVER_H_
#define CHROME_BROWSER_UI_BROWSER_LIST_OBSERVER_H_

class Browser;

namespace chrome {

class BrowserListObserver {
  public:
  
  virtual void OnBrowserAdded(Browser* browser) {}

  
  virtual void OnBrowserRemoved(Browser* browser) {}

  
  virtual void OnBrowserSetLastActive(Browser* browser) {}

 protected:
  virtual ~BrowserListObserver() {}
};

}  

#endif  

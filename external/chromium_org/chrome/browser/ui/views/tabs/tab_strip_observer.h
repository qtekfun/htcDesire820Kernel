// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TABS_TAB_STRIP_OBSERVER_H_
#define CHROME_BROWSER_UI_VIEWS_TABS_TAB_STRIP_OBSERVER_H_

#include "chrome/browser/ui/views/chrome_views_export.h"

class TabStrip;

class CHROME_VIEWS_EXPORT TabStripObserver {
 public:
  
  virtual void TabStripAddedTabAt(TabStrip* tab_strip, int index);

  
  virtual void TabStripMovedTab(TabStrip* tab_strip,
                                int from_index,
                                int to_index);

  
  virtual void TabStripRemovedTabAt(TabStrip* tab_strip, int index);

  
  
  virtual void TabStripDeleted(TabStrip* tab_strip);

 protected:
  virtual ~TabStripObserver() {}
};

#endif  

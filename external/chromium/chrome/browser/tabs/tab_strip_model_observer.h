// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TABS_TAB_STRIP_MODEL_OBSERVER_H_
#define CHROME_BROWSER_TABS_TAB_STRIP_MODEL_OBSERVER_H_
#pragma once

class TabContentsWrapper;
class TabStripModel;

class TabStripModelObserver {
 public:
  
  enum TabChangeType {
    
    LOADING_ONLY,

    
    TITLE_NOT_LOADING,

    
    ALL
  };

  
  
  
  virtual void TabInsertedAt(TabContentsWrapper* contents,
                             int index,
                             bool foreground);

  
  
  virtual void TabClosingAt(TabStripModel* tab_strip_model,
                            TabContentsWrapper* contents,
                            int index);

  
  
  
  virtual void TabDetachedAt(TabContentsWrapper* contents, int index);

  
  
  
  virtual void TabDeselected(TabContentsWrapper* contents);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void TabSelectedAt(TabContentsWrapper* old_contents,
                             TabContentsWrapper* new_contents,
                             int index,
                             bool user_gesture);

  
  virtual void TabMoved(TabContentsWrapper* contents,
                        int from_index,
                        int to_index);

  
  
  
  
  
  virtual void TabChangedAt(TabContentsWrapper* contents,
                            int index,
                            TabChangeType change_type);

  
  
  virtual void TabReplacedAt(TabStripModel* tab_strip_model,
                             TabContentsWrapper* old_contents,
                             TabContentsWrapper* new_contents,
                             int index);

  
  
  virtual void TabPinnedStateChanged(TabContentsWrapper* contents, int index);

  
  
  
  
  virtual void TabMiniStateChanged(TabContentsWrapper* contents, int index);

  
  
  
  virtual void TabBlockedStateChanged(TabContentsWrapper* contents, int index);

  
  
  
  virtual void TabStripEmpty();

  
  
  virtual void TabStripModelDeleted();

 protected:
  virtual ~TabStripModelObserver() {}
};

#endif  

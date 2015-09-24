// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TABS_TAB_STRIP_MODEL_OBSERVER_H_
#define CHROME_BROWSER_UI_TABS_TAB_STRIP_MODEL_OBSERVER_H_

class TabStripModel;

namespace content {
class WebContents;
}

namespace ui {
class ListSelectionModel;
}

class TabStripModelObserver {
 public:
  
  enum TabChangeType {
    
    LOADING_ONLY,

    
    TITLE_NOT_LOADING,

    
    ALL
  };

  enum ChangeReason {
    
    
    CHANGE_REASON_NONE = 0,
    
    CHANGE_REASON_REPLACED = 1 << 0,
    
    CHANGE_REASON_USER_GESTURE = 1 << 1,
  };

  
  
  
  virtual void TabInsertedAt(content::WebContents* contents,
                             int index,
                             bool foreground);

  
  
  virtual void TabClosingAt(TabStripModel* tab_strip_model,
                            content::WebContents* contents,
                            int index);

  
  
  
  
  virtual void TabDetachedAt(content::WebContents* contents, int index);

  
  
  
  virtual void TabDeactivated(content::WebContents* contents);

  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void ActiveTabChanged(content::WebContents* old_contents,
                                content::WebContents* new_contents,
                                int index,
                                int reason);

  
  
  
  
  virtual void TabSelectionChanged(TabStripModel* tab_strip_model,
                                   const ui::ListSelectionModel& old_model);

  
  virtual void TabMoved(content::WebContents* contents,
                        int from_index,
                        int to_index);

  
  
  
  
  
  virtual void TabChangedAt(content::WebContents* contents,
                            int index,
                            TabChangeType change_type);

  
  
  
  virtual void TabReplacedAt(TabStripModel* tab_strip_model,
                             content::WebContents* old_contents,
                             content::WebContents* new_contents,
                             int index);

  
  
  virtual void TabPinnedStateChanged(content::WebContents* contents, int index);

  
  
  
  
  virtual void TabMiniStateChanged(content::WebContents* contents, int index);

  
  
  
  virtual void TabBlockedStateChanged(content::WebContents* contents,
                                      int index);

  
  
  
  virtual void TabStripEmpty();

  
  
  virtual void TabStripModelDeleted();

 protected:
  virtual ~TabStripModelObserver() {}
};

#endif  

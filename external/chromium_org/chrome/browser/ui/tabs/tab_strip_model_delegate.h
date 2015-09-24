// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TABS_TAB_STRIP_MODEL_DELEGATE_H_
#define CHROME_BROWSER_UI_TABS_TAB_STRIP_MODEL_DELEGATE_H_

#include <vector>

#include "content/public/common/page_transition_types.h"

class Browser;
class DockInfo;
class GURL;

namespace content {
class WebContents;
}

namespace gfx {
class Rect;
}

class TabStripModelDelegate {
 public:
  enum {
    TAB_MOVE_ACTION = 1,
    TAB_TEAROFF_ACTION = 2
  };

  enum RestoreTabType {
    RESTORE_NONE,
    RESTORE_TAB,
    RESTORE_WINDOW
  };

  virtual ~TabStripModelDelegate() {}

  
  
  
  virtual void AddTabAt(const GURL& url, int index, bool foreground) = 0;

  
  
  
  
  
  
  
  
  
  
  struct NewStripContents {
    
    content::WebContents* web_contents;
    
    int add_types;
  };
  virtual Browser* CreateNewStripWithContents(
      const std::vector<NewStripContents>& contentses,
      const gfx::Rect& window_bounds,
      const DockInfo& dock_info,
      bool maximize) = 0;

  
  
  
  virtual void WillAddWebContents(content::WebContents* contents) = 0;

  
  virtual int GetDragActions() const = 0;

  
  virtual bool CanDuplicateContentsAt(int index) = 0;

  
  
  virtual void DuplicateContentsAt(int index) = 0;

  
  
  virtual void CloseFrameAfterDragSession() = 0;

  
  
  virtual void CreateHistoricalTab(content::WebContents* contents) = 0;

  
  
  
  
  
  virtual bool RunUnloadListenerBeforeClosing(
      content::WebContents* contents) = 0;

  
  
  virtual bool ShouldRunUnloadListenerBeforeClosing(
      content::WebContents* contents) = 0;

  
  virtual RestoreTabType GetRestoreTabType() = 0;

  
  virtual void RestoreTab() = 0;

  
  
  virtual bool CanBookmarkAllTabs() const = 0;

  
  virtual void BookmarkAllTabs() = 0;
};

#endif  

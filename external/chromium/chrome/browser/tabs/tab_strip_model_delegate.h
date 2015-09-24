// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TABS_TAB_STRIP_MODEL_DELEGATE_H_
#define CHROME_BROWSER_TABS_TAB_STRIP_MODEL_DELEGATE_H_
#pragma once

#include "content/common/page_transition_types.h"

class Browser;
class DockInfo;
class GURL;
class Profile;
class SiteInstance;
class TabContents;
class TabContentsWrapper;
namespace gfx {
class Rect;
}

class TabStripModelDelegate {
 public:
  enum {
    TAB_MOVE_ACTION = 1,
    TAB_TEAROFF_ACTION = 2
  };

  
  virtual TabContentsWrapper* AddBlankTab(bool foreground) = 0;
  virtual TabContentsWrapper* AddBlankTabAt(int index, bool foreground) = 0;

  
  
  
  
  
  
  virtual Browser* CreateNewStripWithContents(TabContentsWrapper* contents,
                                              const gfx::Rect& window_bounds,
                                              const DockInfo& dock_info,
                                              bool maximize) = 0;

  
  virtual int GetDragActions() const = 0;

  
  
  
  
  
  virtual TabContentsWrapper* CreateTabContentsForURL(
      const GURL& url,
      const GURL& referrer,
      Profile* profile,
      PageTransition::Type transition,
      bool defer_load,
      SiteInstance* instance) const = 0;

  
  virtual bool CanDuplicateContentsAt(int index) = 0;

  
  
  virtual void DuplicateContentsAt(int index) = 0;

  
  
  virtual void CloseFrameAfterDragSession() = 0;

  
  
  virtual void CreateHistoricalTab(TabContentsWrapper* contents) = 0;

  
  
  
  
  
  virtual bool RunUnloadListenerBeforeClosing(TabContentsWrapper* contents) = 0;

  
  virtual bool CanRestoreTab() = 0;

  
  virtual void RestoreTab() = 0;

  
  virtual bool CanCloseContentsAt(int index) = 0;

  
  
  virtual bool CanBookmarkAllTabs() const = 0;

  
  virtual void BookmarkAllTabs() = 0;

  
  virtual bool CanCloseTab() const = 0;

  
  virtual bool UseVerticalTabs() const = 0;

  
  virtual void ToggleUseVerticalTabs() = 0;

  
  virtual bool LargeIconsPermitted() const = 0;

 protected:
  virtual ~TabStripModelDelegate() {}
};

#endif  

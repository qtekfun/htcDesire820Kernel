// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TABS_TAB_STRIP_CONTROLLER_H_
#define CHROME_BROWSER_UI_VIEWS_TABS_TAB_STRIP_CONTROLLER_H_

#include "base/strings/string16.h"
#include "chrome/browser/ui/views/tabs/tab_strip_types.h"
#include "ui/base/ui_base_types.h"

class GURL;
class Tab;
class TabStrip;

namespace gfx {
class Point;
}

namespace ui {
class ListSelectionModel;
}

class TabStripController {
 public:
  virtual ~TabStripController() {}

  
  virtual const ui::ListSelectionModel& GetSelectionModel() = 0;

  
  virtual int GetCount() const = 0;

  
  virtual bool IsValidIndex(int index) const = 0;

  
  
  virtual bool IsActiveTab(int index) const = 0;

  
  virtual int GetActiveIndex() const = 0;

  
  virtual bool IsTabSelected(int index) const = 0;

  
  virtual bool IsTabPinned(int index) const = 0;

  
  virtual bool IsNewTabPage(int index) const = 0;

  
  virtual void SelectTab(int index) = 0;

  
  virtual void ExtendSelectionTo(int index) = 0;

  
  virtual void ToggleSelected(int index) = 0;

  
  virtual void AddSelectionFromAnchorTo(int index) = 0;

  
  virtual void CloseTab(int index, CloseTabSource source) = 0;

  
  virtual void ShowContextMenuForTab(Tab* tab,
                                     const gfx::Point& p,
                                     ui::MenuSourceType source_type) = 0;

  
  virtual void UpdateLoadingAnimations() = 0;

  
  
  
  
  virtual int HasAvailableDragActions() const = 0;

  
  virtual void OnDropIndexUpdate(int index, bool drop_before) = 0;

  
  virtual void PerformDrop(bool drop_before, int index, const GURL& url) = 0;

  
  
  virtual bool IsCompatibleWith(TabStrip* other) const = 0;

  
  virtual void CreateNewTab() = 0;

  
  
  
  virtual void CreateNewTabWithLocation(const base::string16& location) = 0;

  
  virtual bool IsIncognito() = 0;

  
  virtual void LayoutTypeMaybeChanged() = 0;

  
  virtual void OnStartedDraggingTabs() = 0;

  
  
  
  virtual void OnStoppedDraggingTabs() = 0;

  
  
  virtual void CheckFileSupported(const GURL& url) = 0;
};

#endif  

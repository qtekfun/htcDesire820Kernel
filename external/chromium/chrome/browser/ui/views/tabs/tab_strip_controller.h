// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TABS_TAB_STRIP_CONTROLLER_H_
#define CHROME_BROWSER_UI_VIEWS_TABS_TAB_STRIP_CONTROLLER_H_
#pragma once

class BaseTab;
class BaseTabStrip;
class GURL;

namespace gfx {
class Point;
}

class TabStripController {
 public:
  virtual ~TabStripController() {}

  
  virtual int GetCount() const = 0;

  
  virtual bool IsValidIndex(int index) const = 0;

  
  
  virtual bool IsActiveTab(int index) const = 0;

  
  virtual bool IsTabSelected(int index) const = 0;

  
  virtual bool IsTabPinned(int index) const = 0;

  
  virtual bool IsTabCloseable(int index) const = 0;

  
  virtual bool IsNewTabPage(int index) const = 0;

  
  virtual void SelectTab(int index) = 0;

  
  virtual void ExtendSelectionTo(int index) = 0;

  
  virtual void ToggleSelected(int index) = 0;

  
  virtual void AddSelectionFromAnchorTo(int index) = 0;

  
  virtual void CloseTab(int index) = 0;

  
  virtual void ShowContextMenuForTab(BaseTab* tab, const gfx::Point& p) = 0;

  
  virtual void UpdateLoadingAnimations() = 0;

  
  
  
  
  virtual int HasAvailableDragActions() const = 0;

  
  virtual void PerformDrop(bool drop_before, int index, const GURL& url) = 0;

  
  
  virtual bool IsCompatibleWith(BaseTabStrip* other) const = 0;

  
  virtual void CreateNewTab() = 0;
};

#endif  

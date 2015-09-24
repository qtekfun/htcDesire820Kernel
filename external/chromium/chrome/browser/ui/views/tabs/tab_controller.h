// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TABS_TAB_CONTROLLER_H_
#define CHROME_BROWSER_UI_VIEWS_TABS_TAB_CONTROLLER_H_
#pragma once

class BaseTab;

namespace gfx {
class Point;
}
namespace views {
class MouseEvent;
}

class TabController {
 public:
  
  virtual void SelectTab(BaseTab* tab) = 0;

  
  virtual void ExtendSelectionTo(BaseTab* tab) = 0;

  
  virtual void ToggleSelected(BaseTab* tab) = 0;

  
  virtual void AddSelectionFromAnchorTo(BaseTab* tab) = 0;

  
  virtual void CloseTab(BaseTab* tab) = 0;

  
  virtual void ShowContextMenuForTab(BaseTab* tab, const gfx::Point& p) = 0;

  
  
  virtual bool IsActiveTab(const BaseTab* tab) const = 0;

  
  virtual bool IsTabSelected(const BaseTab* tab) const = 0;

  
  virtual bool IsTabPinned(const BaseTab* tab) const = 0;

  
  virtual bool IsTabCloseable(const BaseTab* tab) const = 0;

  
  virtual void MaybeStartDrag(BaseTab* tab, const views::MouseEvent& event) = 0;

  
  virtual void ContinueDrag(const views::MouseEvent& event) = 0;

  
  
  
  virtual bool EndDrag(bool canceled) = 0;

  
  
  virtual BaseTab* GetTabAt(BaseTab* tab,
                            const gfx::Point& tab_in_tab_coordinates) = 0;

 protected:
  virtual ~TabController() {}
};

#endif  

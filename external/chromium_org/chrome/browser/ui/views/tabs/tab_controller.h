// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TABS_TAB_CONTROLLER_H_
#define CHROME_BROWSER_UI_VIEWS_TABS_TAB_CONTROLLER_H_

#include "chrome/browser/ui/views/tabs/tab_strip_types.h"

class Tab;

namespace gfx {
class Point;
}
namespace ui {
class ListSelectionModel;
class LocatedEvent;
class MouseEvent;
}
namespace views {
class View;
}

class TabController {
 public:
  virtual const ui::ListSelectionModel& GetSelectionModel() = 0;

  
  virtual bool SupportsMultipleSelection() = 0;

  
  virtual void SelectTab(Tab* tab) = 0;

  
  virtual void ExtendSelectionTo(Tab* tab) = 0;

  
  virtual void ToggleSelected(Tab* tab) = 0;

  
  virtual void AddSelectionFromAnchorTo(Tab* tab) = 0;

  
  virtual void CloseTab(Tab* tab, CloseTabSource source) = 0;

  
  virtual void ShowContextMenuForTab(Tab* tab,
                                     const gfx::Point& p,
                                     ui::MenuSourceType source_type) = 0;

  
  
  virtual bool IsActiveTab(const Tab* tab) const = 0;

  
  virtual bool IsTabSelected(const Tab* tab) const = 0;

  
  virtual bool IsTabPinned(const Tab* tab) const = 0;

  
  virtual void MaybeStartDrag(
      Tab* tab,
      const ui::LocatedEvent& event,
      const ui::ListSelectionModel& original_selection) = 0;

  
  virtual void ContinueDrag(views::View* view,
                            const ui::LocatedEvent& event) = 0;

  
  virtual bool EndDrag(EndDragReason reason) = 0;

  
  
  virtual Tab* GetTabAt(Tab* tab,
                        const gfx::Point& tab_in_tab_coordinates) = 0;

  
  virtual void OnMouseEventInTab(views::View* source,
                                 const ui::MouseEvent& event) = 0;

  
  
  
  virtual bool ShouldPaintTab(const Tab* tab, gfx::Rect* clip) = 0;

  
  virtual bool IsImmersiveStyle() const = 0;

 protected:
  virtual ~TabController() {}
};

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FRAME_DOM_BROWSER_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_FRAME_DOM_BROWSER_VIEW_H_
#pragma once

#include <vector>

#include "base/compiler_specific.h"
#include "chrome/browser/chromeos/frame/browser_view.h"

namespace chromeos {

class DOMBrowserView : public chromeos::BrowserView {
 public:
  explicit DOMBrowserView(Browser* browser);
  virtual ~DOMBrowserView();

  
  static BrowserWindow* CreateDOMWindow(Browser* browser);

  
  virtual void WindowMoveOrResizeStarted() OVERRIDE;
  virtual gfx::Rect GetToolbarBounds() const OVERRIDE;
  virtual int GetTabStripHeight() const OVERRIDE;
  virtual bool IsTabStripVisible() const OVERRIDE;
  virtual bool AcceleratorPressed(const views::Accelerator& accelerator)
      OVERRIDE;
  virtual void SetStarredState(bool is_starred) OVERRIDE;
  virtual LocationBar* GetLocationBar() const OVERRIDE;
  virtual void SetFocusToLocationBar(bool select_all) OVERRIDE;
  virtual void UpdateReloadStopState(bool is_loading, bool force) OVERRIDE;
  virtual void UpdateToolbar(TabContentsWrapper* contents,
                             bool should_restore_state) OVERRIDE;
  virtual void FocusToolbar() OVERRIDE;
  virtual void FocusAppMenu() OVERRIDE;
  virtual void ShowBookmarkBubble(const GURL& url, bool already_bookmarked)
      OVERRIDE;
  virtual void ShowAppMenu() OVERRIDE;
  virtual LocationBarView* GetLocationBarView() const OVERRIDE;
  virtual ToolbarView* GetToolbarView() const OVERRIDE;
  virtual bool ShouldShowOffTheRecordAvatar() const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(int command_id,
                                          ui::Accelerator* accelerator)
      OVERRIDE;

  
  bool IsToolbarVisible() const OVERRIDE;

 protected:
  
  virtual void GetAccessiblePanes(
      std::vector<AccessiblePaneView*>* panes) OVERRIDE;
  virtual void PaintChildren(gfx::Canvas* canvas) OVERRIDE;
  virtual void InitTabStrip(TabStripModel* model) OVERRIDE;
  virtual views::LayoutManager* CreateLayoutManager() const OVERRIDE;
  virtual ToolbarView* CreateToolbar() const OVERRIDE;
  virtual void LoadingAnimationCallback() OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(DOMBrowserView);
};

}  

#endif  

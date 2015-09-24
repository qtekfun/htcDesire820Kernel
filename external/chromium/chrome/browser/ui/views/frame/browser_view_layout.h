// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_VIEW_LAYOUT_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_VIEW_LAYOUT_H_
#pragma once

#include "base/basictypes.h"
#include "ui/gfx/rect.h"
#include "views/layout/layout_manager.h"

class AbstractTabStripView;
class BookmarkBarView;
class Browser;
class BrowserView;
class ContentsContainer;
class DownloadShelfView;
class TabContentsContainer;
class ToolbarView;

namespace gfx {
class Point;
class Size;
}

namespace views {
class SingleSplitView;
}

class BrowserViewLayout : public views::LayoutManager {
 public:
  BrowserViewLayout();
  virtual ~BrowserViewLayout();

  
  virtual gfx::Size GetMinimumSize();

  
  virtual gfx::Rect GetFindBarBoundingBox() const;

  
  
  virtual bool IsPositionInWindowCaption(const gfx::Point& point);

  
  
  
  
  virtual int NonClientHitTest(const gfx::Point& point);

  
  virtual void Installed(views::View* host);
  virtual void Uninstalled(views::View* host);
  virtual void ViewAdded(views::View* host, views::View* view);
  virtual void ViewRemoved(views::View* host, views::View* view);
  virtual void Layout(views::View* host);
  virtual gfx::Size GetPreferredSize(views::View* host);

 protected:
  Browser* browser();
  const Browser* browser() const;

  
  
  virtual int LayoutTabStrip();

  
  
  virtual int LayoutToolbar(int top);
  virtual int LayoutBookmarkAndInfoBars(int top);
  int LayoutBookmarkBar(int top);
  int LayoutInfoBar(int top);

  
  
  
  
  void UpdateReservedContentsRect(const gfx::Rect& browser_reserved_rect,
                                  TabContentsContainer* source,
                                  const gfx::Rect& future_source_bounds,
                                  const gfx::Point& future_parent_offset);

  
  
  void LayoutTabContents(int top, int bottom);

  
  
  
  int GetTopMarginForActiveContent();

  
  
  int LayoutDownloadShelf(int bottom);

  
  bool InfobarVisible() const;

  
  void set_vertical_layout_rect(const gfx::Rect& bounds) {
    vertical_layout_rect_ = bounds;
  }
  const gfx::Rect& vertical_layout_rect() const {
    return vertical_layout_rect_;
  }

  
  AbstractTabStripView* tabstrip_;
  ToolbarView* toolbar_;
  views::SingleSplitView* contents_split_;
  ContentsContainer* contents_container_;
  views::View* infobar_container_;
  DownloadShelfView* download_shelf_;
  BookmarkBarView* active_bookmark_bar_;

  BrowserView* browser_view_;

  
  
  
  
  gfx::Rect vertical_layout_rect_;

  
  int find_bar_y_;

  DISALLOW_COPY_AND_ASSIGN(BrowserViewLayout);
};

#endif  

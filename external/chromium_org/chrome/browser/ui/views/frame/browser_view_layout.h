// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_VIEW_LAYOUT_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_VIEW_LAYOUT_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "ui/gfx/rect.h"
#include "ui/views/layout/layout_manager.h"

class BookmarkBarView;
class Browser;
class BrowserViewLayoutDelegate;
class ContentsContainer;
class ImmersiveModeController;
class InfoBarContainerView;
class TabContentsContainer;
class TabStrip;

namespace gfx {
class Point;
class Size;
}

namespace views {
class ClientView;
class SingleSplitView;
}

namespace web_modal {
class WebContentsModalDialogHost;
}

class BrowserViewLayout : public views::LayoutManager {
 public:
  
  static const int kToolbarTabStripVerticalOverlap;

  BrowserViewLayout();
  virtual ~BrowserViewLayout();

  
  
  void Init(BrowserViewLayoutDelegate* delegate,
            Browser* browser,
            views::ClientView* browser_view,
            views::View* top_container,
            TabStrip* tab_strip,
            views::View* toolbar,
            InfoBarContainerView* infobar_container,
            views::View* contents_split,
            ContentsContainer* contents_container,
            ImmersiveModeController* immersive_mode_controller);

  
  void set_tab_strip(TabStrip* tab_strip) {
    tab_strip_ = tab_strip;
  }
  void set_bookmark_bar(BookmarkBarView* bookmark_bar) {
    bookmark_bar_ = bookmark_bar;
  }
  void set_download_shelf(views::View* download_shelf) {
    download_shelf_ = download_shelf;
  }

  web_modal::WebContentsModalDialogHost* GetWebContentsModalDialogHost();

  
  gfx::Size GetMinimumSize();

  
  gfx::Rect GetFindBarBoundingBox() const;

  
  
  
  
  int NonClientHitTest(const gfx::Point& point);

  
  virtual void Layout(views::View* host) OVERRIDE;
  virtual gfx::Size GetPreferredSize(views::View* host) OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(BrowserViewLayoutTest, BrowserViewLayout);
  FRIEND_TEST_ALL_PREFIXES(BrowserViewLayoutTest, Layout);
  FRIEND_TEST_ALL_PREFIXES(BrowserViewLayoutTest, LayoutDownloadShelf);
  class WebContentsModalDialogHostViews;

  Browser* browser() { return browser_; }

  
  
  int LayoutTabStripRegion(int top);
  int LayoutToolbar(int top);
  int LayoutBookmarkAndInfoBars(int top, int browser_view_y);
  int LayoutBookmarkBar(int top);
  int LayoutInfoBar(int top);

  
  
  
  void LayoutContentsSplitView(int top, int bottom);

  
  
  void UpdateTopContainerBounds();

  
  
  int GetContentsOffsetForBookmarkBar();

  
  
  
  int GetTopMarginForActiveContent();

  
  
  int LayoutDownloadShelf(int bottom);

  
  bool InfobarVisible() const;

  
  scoped_ptr<BrowserViewLayoutDelegate> delegate_;

  
  Browser* browser_;

  
  views::ClientView* browser_view_;

  
  
  
  views::View* top_container_;
  TabStrip* tab_strip_;
  views::View* toolbar_;
  BookmarkBarView* bookmark_bar_;
  InfoBarContainerView* infobar_container_;
  views::View* contents_split_;
  ContentsContainer* contents_container_;
  views::View* download_shelf_;

  ImmersiveModeController* immersive_mode_controller_;

  
  
  
  
  gfx::Rect vertical_layout_rect_;

  
  scoped_ptr<WebContentsModalDialogHostViews> dialog_host_;

  
  gfx::Point latest_dialog_position_;

  
  
  int web_contents_modal_dialog_top_y_;

  DISALLOW_COPY_AND_ASSIGN(BrowserViewLayout);
};

#endif  

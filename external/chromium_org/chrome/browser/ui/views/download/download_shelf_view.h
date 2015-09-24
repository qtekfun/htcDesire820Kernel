// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_DOWNLOAD_DOWNLOAD_SHELF_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_DOWNLOAD_DOWNLOAD_SHELF_VIEW_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/download/download_shelf.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/views/accessible_pane_view.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/link_listener.h"
#include "ui/views/mouse_watcher.h"

class Browser;
class BrowserView;
class DownloadItemView;

namespace content {
class DownloadItem;
class PageNavigator;
}

namespace gfx {
class SlideAnimation;
}

namespace views {
class ImageButton;
class ImageView;
}

class DownloadShelfView : public views::AccessiblePaneView,
                          public gfx::AnimationDelegate,
                          public DownloadShelf,
                          public views::ButtonListener,
                          public views::LinkListener,
                          public views::MouseWatcherListener {
 public:
  DownloadShelfView(Browser* browser, BrowserView* parent);
  virtual ~DownloadShelfView();

  
  void OpenedDownload(DownloadItemView* view);

  
  
  content::PageNavigator* GetNavigator();

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;

  
  
  virtual void LinkClicked(views::Link* source, int event_flags) OVERRIDE;

  
  
  
  virtual void ButtonPressed(views::Button* button,
                             const ui::Event& event) OVERRIDE;

  
  virtual bool IsShowing() const OVERRIDE;
  virtual bool IsClosing() const OVERRIDE;
  virtual Browser* browser() const OVERRIDE;

  
  virtual void MouseMovedOutOfHost() OVERRIDE;

  
  
  void RemoveDownloadView(views::View* view);

 protected:
  
  virtual void DoAddDownload(content::DownloadItem* download) OVERRIDE;
  virtual void DoShow() OVERRIDE;
  virtual void DoClose(CloseReason reason) OVERRIDE;

  
  virtual views::View* GetDefaultFocusableChild() OVERRIDE;

 private:
  
  
  
  void AddDownloadView(DownloadItemView* view);

  
  virtual void OnPaintBorder(gfx::Canvas* canvas) OVERRIDE;

  
  bool CanFitFirstDownloadItem();

  
  void UpdateColorsFromTheme();

  
  virtual void OnThemeChanged() OVERRIDE;

  
  void Closed();

  
  
  bool CanAutoClose();

  
  Browser* browser_;

  
  scoped_ptr<gfx::SlideAnimation> new_item_animation_;

  
  scoped_ptr<gfx::SlideAnimation> shelf_animation_;

  
  
  std::vector<DownloadItemView*> download_views_;

  
  views::ImageView* arrow_image_;

  
  
  views::Link* show_all_view_;

  
  
  views::ImageButton* close_button_;

  
  BrowserView* parent_;

  views::MouseWatcher mouse_watcher_;

  DISALLOW_COPY_AND_ASSIGN(DownloadShelfView);
};

#endif  

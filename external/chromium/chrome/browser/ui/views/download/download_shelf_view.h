// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_DOWNLOAD_DOWNLOAD_SHELF_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_DOWNLOAD_DOWNLOAD_SHELF_VIEW_H_
#pragma once

#include "chrome/browser/download/download_shelf.h"
#include "chrome/browser/ui/views/accessible_pane_view.h"
#include "ui/base/animation/animation_delegate.h"
#include "views/controls/button/button.h"
#include "views/controls/link.h"
#include "views/mouse_watcher.h"

class BaseDownloadItemModel;
class Browser;
class BrowserView;
class DownloadAnimation;
class DownloadItemView;

namespace ui {
class SlideAnimation;
}

namespace views {
class ImageButton;
class ImageView;
}

class DownloadShelfView : public AccessiblePaneView,
                          public ui::AnimationDelegate,
                          public DownloadShelf,
                          public views::ButtonListener,
                          public views::LinkController,
                          public views::MouseWatcherListener {
 public:
  DownloadShelfView(Browser* browser, BrowserView* parent);
  virtual ~DownloadShelfView();

  
  void OpenedDownload(DownloadItemView* view);

  
  virtual gfx::Size GetPreferredSize();
  virtual void Layout();
  virtual void OnPaint(gfx::Canvas* canvas);

  
  virtual void AnimationProgressed(const ui::Animation* animation);
  virtual void AnimationEnded(const ui::Animation* animation);

  
  
  virtual void LinkActivated(views::Link* source, int event_flags);

  
  
  
  virtual void ButtonPressed(views::Button* button, const views::Event& event);

  
  virtual void AddDownload(BaseDownloadItemModel* download_model);
  virtual bool IsShowing() const;
  virtual bool IsClosing() const;
  virtual void Show();
  virtual void Close();
  virtual Browser* browser() const;

  
  virtual void MouseMovedOutOfView();

  
  virtual void FocusWillChange(View* focused_before,
                               View* focused_now);

  
  
  void RemoveDownloadView(views::View* view);

 protected:
  
  virtual views::View* GetDefaultFocusableChild();

 private:
  void Init();

  
  
  
  void AddDownloadView(DownloadItemView* view);

  
  virtual void OnPaintBorder(gfx::Canvas* canvas);

  
  bool CanFitFirstDownloadItem();

  
  void UpdateButtonColors();

  
  virtual void OnThemeChanged();

  
  void Closed();

  
  
  bool CanAutoClose();

  
  
  
  void SchedulePaintForDownloadItem(views::View* view);

  
  
  gfx::Rect GetFocusRectBounds(const DownloadItemView* download_item_view);

  
  Browser* browser_;

  
  scoped_ptr<ui::SlideAnimation> new_item_animation_;

  
  scoped_ptr<ui::SlideAnimation> shelf_animation_;

  
  
  std::vector<DownloadItemView*> download_views_;

  
  views::ImageView* arrow_image_;

  
  
  views::Link* show_all_view_;

  
  
  views::ImageButton* close_button_;

  
  BrowserView* parent_;

  views::MouseWatcher mouse_watcher_;

  DISALLOW_COPY_AND_ASSIGN(DownloadShelfView);
};

#endif  

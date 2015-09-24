// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_DOWNLOAD_DOWNLOAD_ITEM_VIEW_H__
#define CHROME_BROWSER_UI_VIEWS_DOWNLOAD_DOWNLOAD_ITEM_VIEW_H__
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/time.h"
#include "base/timer.h"
#include "chrome/browser/download/download_item.h"
#include "chrome/browser/download/download_manager.h"
#include "chrome/browser/icon_manager.h"
#include "content/browser/cancelable_request.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/gfx/font.h"
#include "views/controls/button/button.h"
#include "views/events/event.h"
#include "views/view.h"

class BaseDownloadItemModel;
class DownloadShelfView;
class SkBitmap;
class DownloadShelfContextMenuWin;

namespace gfx {
class Image;
}

namespace ui {
class SlideAnimation;
}

namespace views {
class Label;
class NativeButton;
}

class DownloadItemView : public views::ButtonListener,
                         public views::View,
                         public DownloadItem::Observer,
                         public ui::AnimationDelegate {
 public:
  DownloadItemView(DownloadItem* download,
                   DownloadShelfView* parent,
                   BaseDownloadItemModel* model);
  virtual ~DownloadItemView();

  
  void UpdateDownloadProgress();
  void StartDownloadProgress();
  void StopDownloadProgress();

  
  void OnExtractIconComplete(IconManager::Handle handle, gfx::Image* icon);

  
  DownloadItem* download() const { return download_; }

  
  virtual void OnDownloadUpdated(DownloadItem* download) OVERRIDE;
  virtual void OnDownloadOpened(DownloadItem* download) OVERRIDE;

  
  virtual void Layout() OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual bool OnMousePressed(const views::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;
  virtual void OnMouseMoved(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const views::MouseEvent& event) OVERRIDE;
  virtual bool OnKeyPressed(const views::KeyEvent& event) OVERRIDE;
  virtual bool GetTooltipText(const gfx::Point& p,
                              std::wstring* tooltip) OVERRIDE;
  virtual void ShowContextMenu(const gfx::Point& p,
                               bool is_mouse_gesture) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const views::Event& event) OVERRIDE;

  
  virtual void AnimationProgressed(const ui::Animation* animation) OVERRIDE;

 protected:
  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

 private:
  enum State {
    NORMAL = 0,
    HOT,
    PUSHED,
    DANGEROUS
  };

  
  struct BodyImageSet {
    SkBitmap* top_left;
    SkBitmap* left;
    SkBitmap* bottom_left;
    SkBitmap* top;
    SkBitmap* center;
    SkBitmap* bottom;
    SkBitmap* top_right;
    SkBitmap* right;
    SkBitmap* bottom_right;
  };

  
  struct DropDownImageSet {
    SkBitmap* top;
    SkBitmap* center;
    SkBitmap* bottom;
  };

  void OpenDownload();

  void LoadIcon();

  
  
  void PaintBitmaps(gfx::Canvas* canvas,
                    const SkBitmap* top_bitmap,
                    const SkBitmap* center_bitmap,
                    const SkBitmap* bottom_bitmap,
                    int x,
                    int y,
                    int height,
                    int width);

  
  void SetState(State body_state, State drop_down_state);

  
  bool IsDangerousMode() { return body_state_ == DANGEROUS; }

  
  void ClearDangerousMode();

  
  
  gfx::Size GetButtonSize();

  
  
  
  void SizeLabelToMinWidth();

  
  
  void Reenable();

  
  
  bool InDropDownButtonXCoordinateRange(int x);

  
  
  
  void UpdateAccessibleName();

  
  BodyImageSet normal_body_image_set_;
  BodyImageSet hot_body_image_set_;
  BodyImageSet pushed_body_image_set_;
  BodyImageSet dangerous_mode_body_image_set_;
  DropDownImageSet normal_drop_down_image_set_;
  DropDownImageSet hot_drop_down_image_set_;
  DropDownImageSet pushed_drop_down_image_set_;

  
  const SkBitmap* warning_icon_;

  
  DownloadItem* download_;

  
  DownloadShelfView* parent_;

  
  std::wstring status_text_;
  bool show_status_text_;

  
  gfx::Font font_;

  
  std::wstring tooltip_text_;

  
  State body_state_;
  State drop_down_state_;

  
  int progress_angle_;

  
  int drop_down_x_left_;
  int drop_down_x_right_;

  
  bool drop_down_pressed_;

  
  int box_height_;

  
  int box_y_;

  
  bool dragging_;

  
  bool starting_drag_;

  
  gfx::Point drag_start_point_;

  
  CancelableRequestConsumerT<int, 0> icon_consumer_;

  
  
  
  scoped_ptr<BaseDownloadItemModel> model_;

  
  scoped_ptr<ui::SlideAnimation> body_hover_animation_;
  scoped_ptr<ui::SlideAnimation> drop_hover_animation_;

  
  scoped_ptr<ui::SlideAnimation> complete_animation_;

  
  base::RepeatingTimer<DownloadItemView> progress_timer_;

  
  views::NativeButton* save_button_;
  views::NativeButton* discard_button_;

  
  views::Label* dangerous_download_label_;

  
  bool dangerous_download_label_sized_;

  
  gfx::Size cached_button_size_;

  
  bool disabled_while_opening_;

  
  base::Time creation_time_;

  
  
  ScopedRunnableMethodFactory<DownloadItemView> reenable_method_factory_;

  
  scoped_ptr<DownloadShelfContextMenuWin> context_menu_;

  
  
  
  bool* deleted_;

  
  string16 accessible_name_;

  DISALLOW_COPY_AND_ASSIGN(DownloadItemView);
};

#endif  

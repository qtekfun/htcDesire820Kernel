// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_DOWNLOAD_DOWNLOAD_ITEM_VIEW_H__
#define CHROME_BROWSER_UI_VIEWS_DOWNLOAD_DOWNLOAD_ITEM_VIEW_H__

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string_util.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "chrome/browser/download/download_item_model.h"
#include "chrome/browser/icon_manager.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "content/public/browser/download_item.h"
#include "content/public/browser/download_manager.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/font_list.h"
#include "ui/views/context_menu_controller.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/view.h"

class DownloadShelfView;
class DownloadShelfContextMenuView;

namespace gfx {
class Image;
class ImageSkia;
class SlideAnimation;
}

namespace views {
class Label;
class LabelButton;
}

class DownloadItemView : public views::ButtonListener,
                         public views::View,
                         public views::ContextMenuController,
                         public content::DownloadItem::Observer,
                         public gfx::AnimationDelegate {
 public:
  DownloadItemView(content::DownloadItem* download, DownloadShelfView* parent);
  virtual ~DownloadItemView();

  
  void UpdateDownloadProgress();
  void StartDownloadProgress();
  void StopDownloadProgress();

  
  void OnExtractIconComplete(gfx::Image* icon);

  
  content::DownloadItem* download() { return model_.download(); }

  
  virtual void OnDownloadUpdated(content::DownloadItem* download) OVERRIDE;
  virtual void OnDownloadOpened(content::DownloadItem* download) OVERRIDE;
  virtual void OnDownloadDestroyed(content::DownloadItem* download) OVERRIDE;

  
  virtual void Layout() OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;
  virtual void OnMouseMoved(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual bool GetTooltipText(const gfx::Point& p,
                              base::string16* tooltip) const OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual void OnThemeChanged() OVERRIDE;

  
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  
  virtual void ShowContextMenuForView(View* source,
                                      const gfx::Point& point,
                                      ui::MenuSourceType source_type) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;

 protected:
  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnPaintBackground(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;

 private:
  enum State {
    NORMAL = 0,
    HOT,
    PUSHED
  };

  enum Mode {
    NORMAL_MODE = 0,        
    DANGEROUS_MODE,         
    MALICIOUS_MODE          
  };

  
  struct BodyImageSet {
    gfx::ImageSkia* top_left;
    gfx::ImageSkia* left;
    gfx::ImageSkia* bottom_left;
    gfx::ImageSkia* top;
    gfx::ImageSkia* center;
    gfx::ImageSkia* bottom;
    gfx::ImageSkia* top_right;
    gfx::ImageSkia* right;
    gfx::ImageSkia* bottom_right;
  };

  
  struct DropDownImageSet {
    gfx::ImageSkia* top;
    gfx::ImageSkia* center;
    gfx::ImageSkia* bottom;
  };

  void OpenDownload();

  
  
  
  bool BeginDownloadFeedback();

  void LoadIcon();
  void LoadIconIfItemPathChanged();

  
  void UpdateColorsFromTheme();

  
  
  void ShowContextMenuImpl(const gfx::Point& point,
                           ui::MenuSourceType source_type);

  
  void HandlePressEvent(const ui::LocatedEvent& event, bool active_event);
  void HandleClickEvent(const ui::LocatedEvent& event, bool active_event);

  
  
  void PaintImages(gfx::Canvas* canvas,
                    const gfx::ImageSkia* top_image,
                    const gfx::ImageSkia* center_image,
                    const gfx::ImageSkia* bottom_image,
                    int x,
                    int y,
                    int height,
                    int width);

  
  void SetState(State body_state, State drop_down_state);

  
  bool IsShowingWarningDialog() const {
    return mode_ == DANGEROUS_MODE || mode_ == MALICIOUS_MODE;
  }

  
  void ClearWarningDialog();

  
  
  void ShowWarningDialog();

  
  
  gfx::Size GetButtonSize();

  
  
  
  void SizeLabelToMinWidth();

  
  
  void Reenable();

  
  void ReleaseDropDown();

  
  
  bool InDropDownButtonXCoordinateRange(int x);

  
  
  
  void UpdateAccessibleName();

  
  void UpdateDropDownButtonPosition();

  
  
  void AnimateStateTransition(State from, State to,
                              gfx::SlideAnimation* animation);

  
  BodyImageSet normal_body_image_set_;
  BodyImageSet hot_body_image_set_;
  BodyImageSet pushed_body_image_set_;
  BodyImageSet dangerous_mode_body_image_set_;
  BodyImageSet malicious_mode_body_image_set_;
  DropDownImageSet normal_drop_down_image_set_;
  DropDownImageSet hot_drop_down_image_set_;
  DropDownImageSet pushed_drop_down_image_set_;

  
  const gfx::ImageSkia* warning_icon_;

  
  DownloadShelfView* shelf_;

  
  base::string16 status_text_;

  
  gfx::FontList font_list_;

  
  base::string16 tooltip_text_;

  
  State body_state_;
  State drop_down_state_;

  
  Mode mode_;

  
  int progress_angle_;

  
  int drop_down_x_left_;
  int drop_down_x_right_;

  
  bool drop_down_pressed_;

  
  int box_height_;

  
  int box_y_;

  
  bool dragging_;

  
  bool starting_drag_;

  
  gfx::Point drag_start_point_;

  
  CancelableTaskTracker cancelable_task_tracker_;

  
  DownloadItemModel model_;

  
  scoped_ptr<gfx::SlideAnimation> body_hover_animation_;
  scoped_ptr<gfx::SlideAnimation> drop_hover_animation_;

  
  scoped_ptr<gfx::SlideAnimation> complete_animation_;

  
  base::RepeatingTimer<DownloadItemView> progress_timer_;

  
  views::LabelButton* save_button_;
  views::LabelButton* discard_button_;

  
  views::Label* dangerous_download_label_;

  
  bool dangerous_download_label_sized_;

  
  gfx::Size cached_button_size_;

  
  bool disabled_while_opening_;

  
  base::Time creation_time_;

  
  base::Time time_download_warning_shown_;

  
  
  base::WeakPtrFactory<DownloadItemView> weak_ptr_factory_;

  
  scoped_ptr<DownloadShelfContextMenuView> context_menu_;

  
  base::string16 accessible_name_;

  
  
  
  base::FilePath last_download_item_path_;

  DISALLOW_COPY_AND_ASSIGN(DownloadItemView);
};

#endif  

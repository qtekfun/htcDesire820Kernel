// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_TRAY_TRAY_BACKGROUND_VIEW_H_
#define ASH_SYSTEM_TRAY_TRAY_BACKGROUND_VIEW_H_

#include "ash/ash_export.h"
#include "ash/shelf/background_animator.h"
#include "ash/shelf/shelf_types.h"
#include "ash/system/tray/actionable_view.h"
#include "ui/views/bubble/tray_bubble_view.h"

namespace ash {
namespace internal {

class ShelfLayoutManager;
class StatusAreaWidget;
class TrayEventFilter;
class TrayBackground;

class ASH_EXPORT TrayBackgroundView : public ActionableView,
                                      public BackgroundAnimatorDelegate {
 public:
  static const char kViewClassName[];

  
  
  class TrayContainer : public views::View {
   public:
    explicit TrayContainer(ShelfAlignment alignment);
    virtual ~TrayContainer() {}

    void SetAlignment(ShelfAlignment alignment);

    void set_size(const gfx::Size& size) { size_ = size; }

    
    virtual gfx::Size GetPreferredSize() OVERRIDE;

   protected:
    
    virtual void ChildPreferredSizeChanged(views::View* child) OVERRIDE;
    virtual void ChildVisibilityChanged(View* child) OVERRIDE;
    virtual void ViewHierarchyChanged(
        const ViewHierarchyChangedDetails& details) OVERRIDE;

   private:
    void UpdateLayout();

    ShelfAlignment alignment_;
    gfx::Size size_;

    DISALLOW_COPY_AND_ASSIGN(TrayContainer);
  };

  explicit TrayBackgroundView(internal::StatusAreaWidget* status_area_widget);
  virtual ~TrayBackgroundView();

  
  virtual void Initialize();

  
  virtual const char* GetClassName() const OVERRIDE;
  virtual void OnMouseEntered(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;
  virtual void ChildPreferredSizeChanged(views::View* child) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual void AboutToRequestFocusFromTabTraversal(bool reverse) OVERRIDE;

  
  virtual bool PerformAction(const ui::Event& event) OVERRIDE;
  virtual gfx::Rect GetFocusBounds() OVERRIDE;

  
  virtual void UpdateBackground(int alpha) OVERRIDE;

  
  virtual void SetShelfAlignment(ShelfAlignment alignment);

  
  virtual void AnchorUpdated() {}

  
  virtual base::string16 GetAccessibleNameForTray() = 0;

  
  virtual void BubbleResized(const views::TrayBubbleView* bubble_view) {}

  
  
  virtual void HideBubbleWithView(const views::TrayBubbleView* bubble_view) = 0;

  
  
  virtual bool ClickedOutsideBubble() = 0;

  
  void SetContents(views::View* contents);

  
  void SetContentsBackground();

  
  
  void SetPaintsBackground(bool value,
                           BackgroundAnimatorChangeType change_type);

  
  void InitializeBubbleAnimations(views::Widget* bubble_widget);

  
  aura::Window* GetBubbleWindowContainer() const;

  
  gfx::Rect GetBubbleAnchorRect(
      views::Widget* anchor_widget,
      views::TrayBubbleView::AnchorType anchor_type,
      views::TrayBubbleView::AnchorAlignment anchor_alignment) const;

  
  views::TrayBubbleView::AnchorAlignment GetAnchorAlignment() const;

  
  void SetDrawBackgroundAsActive(bool visible);

  
  bool draw_background_as_active() const {return draw_background_as_active_; }

  StatusAreaWidget* status_area_widget() {
    return status_area_widget_;
  }
  const StatusAreaWidget* status_area_widget() const {
    return status_area_widget_;
  }
  TrayContainer* tray_container() const { return tray_container_; }
  ShelfAlignment shelf_alignment() const { return shelf_alignment_; }
  TrayEventFilter* tray_event_filter() { return tray_event_filter_.get(); }

  ShelfLayoutManager* GetShelfLayoutManager();

  
  void UpdateBubbleViewArrow(views::TrayBubbleView* bubble_view);

 private:
  class TrayWidgetObserver;

  
  
  void SetBorder();

  
  StatusAreaWidget* status_area_widget_;

  
  TrayContainer* tray_container_;

  
  ShelfAlignment shelf_alignment_;

  
  internal::TrayBackground* background_;

  
  internal::BackgroundAnimator hide_background_animator_;
  internal::BackgroundAnimator hover_background_animator_;

  
  bool hovered_;

  
  
  bool draw_background_as_active_;

  scoped_ptr<TrayWidgetObserver> widget_observer_;
  scoped_ptr<TrayEventFilter> tray_event_filter_;

  DISALLOW_COPY_AND_ASSIGN(TrayBackgroundView);
};

}  
}  

#endif  

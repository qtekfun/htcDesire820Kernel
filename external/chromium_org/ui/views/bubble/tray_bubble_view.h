// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_BUBBLE_TRAY_BUBBLE_VIEW_H_
#define UI_VIEWS_BUBBLE_TRAY_BUBBLE_VIEW_H_

#include "base/memory/scoped_ptr.h"
#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/mouse_watcher.h"
#include "ui/views/views_export.h"


namespace ui {
class LocatedEvent;
}

namespace views {
class View;
class Widget;
}

namespace views {

namespace internal {
class TrayBubbleBorder;
class TrayBubbleContentMask;
}

class VIEWS_EXPORT TrayBubbleView : public views::BubbleDelegateView,
                                    public views::MouseWatcherListener {
 public:
  
  
  
  enum AnchorType {
    ANCHOR_TYPE_TRAY,
    ANCHOR_TYPE_BUBBLE,
  };

  
  
  enum AnchorAlignment {
    ANCHOR_ALIGNMENT_BOTTOM,
    ANCHOR_ALIGNMENT_LEFT,
    ANCHOR_ALIGNMENT_RIGHT,
    ANCHOR_ALIGNMENT_TOP
  };

  class VIEWS_EXPORT Delegate {
   public:
    typedef TrayBubbleView::AnchorType AnchorType;
    typedef TrayBubbleView::AnchorAlignment AnchorAlignment;

    Delegate() {}
    virtual ~Delegate() {}

    
    
    virtual void BubbleViewDestroyed() = 0;

    
    
    
    virtual void OnMouseEnteredView() = 0;
    virtual void OnMouseExitedView() = 0;

    
    
    virtual string16 GetAccessibleNameForBubble() = 0;

    
    
    virtual gfx::Rect GetAnchorRect(views::Widget* anchor_widget,
                                    AnchorType anchor_type,
                                    AnchorAlignment anchor_alignment) = 0;

    
    
    virtual void HideBubble(const TrayBubbleView* bubble_view) = 0;

   private:
    DISALLOW_COPY_AND_ASSIGN(Delegate);
  };

  struct VIEWS_EXPORT InitParams {
    static const int kArrowDefaultOffset;

    InitParams(AnchorType anchor_type,
               AnchorAlignment anchor_alignment,
               int min_width,
               int max_width);
    AnchorType anchor_type;
    AnchorAlignment anchor_alignment;
    int min_width;
    int max_width;
    int max_height;
    bool can_activate;
    bool close_on_deactivate;
    SkColor arrow_color;
    bool first_item_has_no_margin;
    views::BubbleBorder::Arrow arrow;
    int arrow_offset;
    views::BubbleBorder::ArrowPaintType arrow_paint_type;
    views::BubbleBorder::Shadow shadow;
    views::BubbleBorder::BubbleAlignment arrow_alignment;
  };

  
  static TrayBubbleView* Create(gfx::NativeView parent_window,
                                views::View* anchor,
                                Delegate* delegate,
                                InitParams* init_params);

  virtual ~TrayBubbleView();

  
  
  void InitializeAndShowBubble();

  
  void UpdateBubble();

  
  void SetMaxHeight(int height);

  
  void SetWidth(int width);

  
  void SetArrowPaintType(views::BubbleBorder::ArrowPaintType arrow_paint_type);

  
  gfx::Insets GetBorderInsets() const;

  
  void reset_delegate() { delegate_ = NULL; }

  Delegate* delegate() { return delegate_; }

  void set_gesture_dragging(bool dragging) { is_gesture_dragging_ = dragging; }
  bool is_gesture_dragging() const { return is_gesture_dragging_; }

  
  virtual bool CanActivate() const OVERRIDE;
  virtual views::NonClientFrameView* CreateNonClientFrameView(
      views::Widget* widget) OVERRIDE;
  virtual bool WidgetHasHitTestMask() const OVERRIDE;
  virtual void GetWidgetHitTestMask(gfx::Path* mask) const OVERRIDE;

  
  virtual gfx::Rect GetAnchorRect() OVERRIDE;

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual gfx::Size GetMaximumSize() OVERRIDE;
  virtual int GetHeightForWidth(int width) OVERRIDE;
  virtual void OnMouseEntered(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  virtual void MouseMovedOutOfHost() OVERRIDE;

 protected:
  TrayBubbleView(gfx::NativeView parent_window,
                 views::View* anchor,
                 Delegate* delegate,
                 const InitParams& init_params);

  
  virtual void Init() OVERRIDE;

  
  virtual void ChildPreferredSizeChanged(View* child) OVERRIDE;
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;

 private:
  InitParams params_;
  Delegate* delegate_;
  int preferred_width_;
  internal::TrayBubbleBorder* bubble_border_;
  scoped_ptr<internal::TrayBubbleContentMask> bubble_content_mask_;
  bool is_gesture_dragging_;

  
  
  bool mouse_actively_entered_;

  
  scoped_ptr<MouseWatcher> mouse_watcher_;

  DISALLOW_COPY_AND_ASSIGN(TrayBubbleView);
};

}  

#endif  

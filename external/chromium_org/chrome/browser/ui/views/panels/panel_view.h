// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_PANELS_PANEL_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_PANELS_PANEL_VIEW_H_

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/panels/native_panel.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/views/widget/widget_delegate.h"
#include "ui/views/widget/widget_observer.h"

#if defined(OS_WIN)
#include "ui/base/win/hwnd_subclass.h"
#endif

class Panel;
class PanelBoundsAnimation;
class PanelFrameView;
class TaskbarWindowThumbnailerWin;

namespace views {
class WebView;
}

class PanelView : public NativePanel,
                  public views::WidgetObserver,
                  public views::WidgetDelegateView,
#if defined(OS_WIN)
                  public ui::HWNDMessageFilter,
#endif
                  public gfx::AnimationDelegate {
 public:
  
  static const int kResizeInsideBoundsSize = 5;

  PanelView(Panel* panel, const gfx::Rect& bounds, bool always_on_top);
  virtual ~PanelView();

  
  virtual void ShowPanel() OVERRIDE;
  virtual void ShowPanelInactive() OVERRIDE;
  virtual gfx::Rect GetPanelBounds() const OVERRIDE;
  virtual void SetPanelBounds(const gfx::Rect& bounds) OVERRIDE;
  virtual void SetPanelBoundsInstantly(const gfx::Rect& bounds) OVERRIDE;
  virtual void ClosePanel() OVERRIDE;
  virtual void ActivatePanel() OVERRIDE;
  virtual void DeactivatePanel() OVERRIDE;
  virtual bool IsPanelActive() const OVERRIDE;
  virtual void PreventActivationByOS(bool prevent_activation) OVERRIDE;
  virtual gfx::NativeWindow GetNativePanelWindow() OVERRIDE;
  virtual void UpdatePanelTitleBar() OVERRIDE;
  virtual void UpdatePanelLoadingAnimations(bool should_animate) OVERRIDE;
  virtual void PanelWebContentsFocused(content::WebContents* contents) OVERRIDE;
  virtual void PanelCut() OVERRIDE;
  virtual void PanelCopy() OVERRIDE;
  virtual void PanelPaste() OVERRIDE;
  virtual void DrawAttention(bool draw_attention) OVERRIDE;
  virtual bool IsDrawingAttention() const OVERRIDE;
  virtual void HandlePanelKeyboardEvent(
      const content::NativeWebKeyboardEvent& event) OVERRIDE;
  virtual void FullScreenModeChanged(bool is_full_screen) OVERRIDE;
  virtual bool IsPanelAlwaysOnTop() const OVERRIDE;
  virtual void SetPanelAlwaysOnTop(bool on_top) OVERRIDE;
  virtual void UpdatePanelMinimizeRestoreButtonVisibility() OVERRIDE;
  virtual void SetWindowCornerStyle(panel::CornerStyle corner_style) OVERRIDE;
  virtual void PanelExpansionStateChanging(
      Panel::ExpansionState old_state,
      Panel::ExpansionState new_state) OVERRIDE;
  virtual void AttachWebContents(content::WebContents* contents) OVERRIDE;
  virtual void DetachWebContents(content::WebContents* contents) OVERRIDE;
  virtual gfx::Size WindowSizeFromContentSize(
      const gfx::Size& content_size) const OVERRIDE;
  virtual gfx::Size ContentSizeFromWindowSize(
      const gfx::Size& window_size) const OVERRIDE;
  virtual int TitleOnlyHeight() const OVERRIDE;
  virtual void MinimizePanelBySystem() OVERRIDE;
  virtual bool IsPanelMinimizedBySystem() const OVERRIDE;
  virtual bool IsPanelShownOnActiveDesktop() const OVERRIDE;
  virtual void ShowShadow(bool show) OVERRIDE;
  virtual NativePanelTesting* CreateNativePanelTesting() OVERRIDE;

  
  virtual gfx::Size GetMinimumSize() OVERRIDE;
  virtual gfx::Size GetMaximumSize() OVERRIDE;

  
  
  bool OnTitlebarMousePressed(const gfx::Point& mouse_location);
  bool OnTitlebarMouseDragged(const gfx::Point& mouse_location);
  bool OnTitlebarMouseReleased(panel::ClickModifier modifier);
  bool OnTitlebarMouseCaptureLost();

  PanelFrameView* GetFrameView() const;
  bool IsAnimatingBounds() const;

  
  
  
  
  bool IsWithinResizingArea(const gfx::Point& mouse_location) const;

  Panel* panel() const { return panel_.get(); }
  views::Widget* window() const { return window_; }
  bool force_to_paint_as_inactive() const {
    return force_to_paint_as_inactive_;
  }

  
  
  void set_cached_bounds_directly(const gfx::Rect& bounds) { bounds_ = bounds; }

 private:
  enum MouseDraggingState {
    NO_DRAGGING,
    DRAGGING_STARTED,
    DRAGGING_ENDED
  };

  
  virtual void OnDisplayChanged() OVERRIDE;
  virtual void OnWorkAreaChanged() OVERRIDE;
  virtual bool WillProcessWorkAreaChange() const OVERRIDE;
  virtual views::View* GetContentsView() OVERRIDE;
  virtual views::NonClientFrameView* CreateNonClientFrameView(
      views::Widget* widget) OVERRIDE;
  virtual bool CanResize() const OVERRIDE;
  virtual bool CanMaximize() const OVERRIDE;
  virtual views::Widget* GetWidget() OVERRIDE;
  virtual const views::Widget* GetWidget() const OVERRIDE;
  virtual base::string16 GetWindowTitle() const OVERRIDE;
  virtual gfx::ImageSkia GetWindowAppIcon() OVERRIDE;
  virtual gfx::ImageSkia GetWindowIcon() OVERRIDE;
  virtual void WindowClosing() OVERRIDE;
  virtual void DeleteDelegate() OVERRIDE;
  virtual void OnWindowBeginUserBoundsChange() OVERRIDE;
  virtual void OnWindowEndUserBoundsChange() OVERRIDE;

  
  virtual void Layout() OVERRIDE;
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;

  
  virtual void OnWidgetDestroying(views::Widget* widget) OVERRIDE;
  virtual void OnWidgetActivationChanged(views::Widget* widget,
                                         bool active) OVERRIDE;
  virtual void OnWidgetBoundsChanged(views::Widget* widget,
                                     const gfx::Rect& new_bounds) OVERRIDE;

  
#if defined(OS_WIN)
  virtual bool FilterMessage(HWND hwnd,
                             UINT message,
                             WPARAM w_param,
                             LPARAM l_param,
                             LRESULT* l_result) OVERRIDE;
#endif

  
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;

  void UpdateLoadingAnimations(bool should_animate);
  void UpdateWindowTitle();
  void UpdateWindowIcon();
  void SetBoundsInternal(const gfx::Rect& bounds, bool animate);
  bool EndDragging(bool cancelled);
  void OnViewWasResized();

  
  
  
  void SetWidgetBounds(const gfx::Rect& new_bounds);

#if defined(OS_WIN)
  
  
  
  void UpdateWindowAttribute(int attribute_index,
                             int attribute_value_to_set,
                             int attribute_value_to_reset,
                             bool update_frame);
#endif

  scoped_ptr<Panel> panel_;
  gfx::Rect bounds_;

  
  
  views::Widget* window_;

  
  bool window_closed_;

  
  
  views::WebView* web_view_;

  
  bool always_on_top_;

  
  bool focused_;

  
  bool user_resizing_;

#if defined(OS_WIN)
  
  bool user_resizing_interior_stacked_panel_edge_;

  
  gfx::Size original_full_size_of_resizing_panel_;

  
  
  gfx::Size original_full_size_of_panel_below_resizing_panel_;
#endif


  
  bool mouse_pressed_;

  
  
  
  gfx::Point last_mouse_location_;

  
  
  MouseDraggingState mouse_dragging_state_;

  
  scoped_ptr<PanelBoundsAnimation> bounds_animator_;
  gfx::Rect animation_start_bounds_;

  
  bool is_drawing_attention_;

  
  
  bool force_to_paint_as_inactive_;

  
  
  views::View* old_focused_view_;

#if defined(OS_WIN)
  
  scoped_ptr<TaskbarWindowThumbnailerWin> thumbnailer_;
#endif

  DISALLOW_COPY_AND_ASSIGN(PanelView);
};

#endif  

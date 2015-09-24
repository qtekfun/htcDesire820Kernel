// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_PANELS_PANEL_STACK_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_PANELS_PANEL_STACK_VIEW_H_

#include <list>
#include <map>
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/panels/native_panel_stack_window.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/views/focus/widget_focus_manager.h"
#include "ui/views/widget/widget_delegate.h"
#include "ui/views/widget/widget_observer.h"

#if defined(OS_WIN)
#include "chrome/browser/ui/views/panels/taskbar_window_thumbnailer_win.h"
#include "ui/base/win/hwnd_subclass.h"
#endif

namespace gfx {
class LinearAnimation;
}
namespace views {
class Widget;
}

class PanelStackView : public NativePanelStackWindow,
                       public views::WidgetFocusChangeListener,
#if defined(OS_WIN)
                       public ui::HWNDMessageFilter,
                       public TaskbarWindowThumbnailerDelegateWin,
#endif
                       public gfx::AnimationDelegate {
 public:
  explicit PanelStackView(NativePanelStackWindowDelegate* delegate);
  virtual ~PanelStackView();

 protected:
  
  virtual void Close() OVERRIDE;
  virtual void AddPanel(Panel* panel) OVERRIDE;
  virtual void RemovePanel(Panel* panel) OVERRIDE;
  virtual void MergeWith(NativePanelStackWindow* another) OVERRIDE;
  virtual bool IsEmpty() const OVERRIDE;
  virtual bool HasPanel(Panel* panel) const OVERRIDE;
  virtual void MovePanelsBy(const gfx::Vector2d& delta) OVERRIDE;
  virtual void BeginBatchUpdatePanelBounds(bool animate) OVERRIDE;
  virtual void AddPanelBoundsForBatchUpdate(
      Panel* panel, const gfx::Rect& new_bounds) OVERRIDE;
  virtual void EndBatchUpdatePanelBounds() OVERRIDE;
  virtual bool IsAnimatingPanelBounds() const OVERRIDE;
  virtual void Minimize() OVERRIDE;
  virtual bool IsMinimized() const OVERRIDE;
  virtual void DrawSystemAttention(bool draw_attention) OVERRIDE;
  virtual void OnPanelActivated(Panel* panel) OVERRIDE;

 private:
  typedef std::list<Panel*> Panels;

  
  typedef std::map<Panel*, gfx::Rect> BoundsUpdates;

  
  virtual void OnNativeFocusChange(gfx::NativeView focused_before,
                                   gfx::NativeView focused_now) OVERRIDE;

  
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationCanceled(const gfx::Animation* animation) OVERRIDE;

  
  void UpdatePanelsBounds();

  
  void NotifyBoundsUpdateCompleted();

  
  gfx::Rect GetStackWindowBounds() const;
  void UpdateStackWindowBounds();

  views::Widget* CreateWindowWithBounds(const gfx::Rect& bounds);
  void EnsureWindowCreated();

  
  
  
  static void MakeStackWindowOwnPanelWindow(Panel* panel,
                                            PanelStackView* stack_window);

#if defined(OS_WIN)
  
  virtual bool FilterMessage(HWND hwnd,
                             UINT message,
                             WPARAM w_param,
                             LPARAM l_param,
                             LRESULT* l_result) OVERRIDE;

  
  virtual std::vector<HWND> GetSnapshotWindowHandles() const OVERRIDE;

  
  
  void RefreshLivePreviewThumbnail();

  
  void DeferUpdateNativeWindowBounds(HDWP defer_window_pos_info,
                                     views::Widget* window,
                                     const gfx::Rect& bounds);
#endif

  NativePanelStackWindowDelegate* delegate_;

  views::Widget* window_;  

  
  Panels panels_;

  
  
  bool is_drawing_attention_;

#if defined(OS_WIN)
  
  
  
  scoped_ptr<TaskbarWindowThumbnailerWin> thumbnailer_;
#endif

  
  bool animate_bounds_updates_;
  bool bounds_updates_started_;
  BoundsUpdates bounds_updates_;

  
  scoped_ptr<gfx::LinearAnimation> bounds_animator_;

  DISALLOW_COPY_AND_ASSIGN(PanelStackView);
};

#endif  

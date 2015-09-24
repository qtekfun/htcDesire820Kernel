// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_PANELS_PANEL_GTK_H_
#define CHROME_BROWSER_UI_GTK_PANELS_PANEL_GTK_H_

#include <gtk/gtk.h>
#include <map>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer/timer.h"
#include "chrome/browser/ui/panels/native_panel.h"
#include "chrome/browser/ui/panels/panel_constants.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/x/active_window_watcher_x_observer.h"
#include "ui/gfx/rect.h"

class Panel;
class PanelTitlebarGtk;
class PanelDragGtk;
class GtkNativePanelTesting;

namespace gfx {
class Image;
}

class PanelGtk : public NativePanel,
                 public ui::ActiveWindowWatcherXObserver {
 public:
  enum PaintState {
    PAINT_AS_ACTIVE,
    PAINT_AS_INACTIVE,
    PAINT_AS_MINIMIZED,
    PAINT_FOR_ATTENTION
  };

  PanelGtk(Panel* panel, const gfx::Rect& bounds, bool always_on_top);
  virtual ~PanelGtk();

  void Init();

  
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
  virtual bool IsPanelAlwaysOnTop() const OVERRIDE;
  virtual void SetPanelAlwaysOnTop(bool on_top) OVERRIDE;
  virtual void UpdatePanelMinimizeRestoreButtonVisibility() OVERRIDE;
  virtual void SetWindowCornerStyle(panel::CornerStyle corner_style) OVERRIDE;
  virtual void MinimizePanelBySystem() OVERRIDE;
  virtual bool IsPanelMinimizedBySystem() const OVERRIDE;
  virtual bool IsPanelShownOnActiveDesktop() const OVERRIDE;
  virtual void ShowShadow(bool show) OVERRIDE;

  virtual NativePanelTesting* CreateNativePanelTesting() OVERRIDE;

  
  virtual void ActiveWindowChanged(GdkWindow* active_window) OVERRIDE;

  Panel* panel() const { return panel_.get(); }
  PaintState paint_state() const { return paint_state_; }
  PanelTitlebarGtk* titlebar() const { return titlebar_.get(); }

 private:
  friend class GtkNativePanelTesting;

  
  void UpdateWindowShape();

  
  
  
  bool GetWindowEdge(int x, int y, GdkWindowEdge* edge) const;

  
  void ConnectAccelerators();
  void DisconnectAccelerators();

  
  gfx::Image GetFrameBackground() const;

  
  void RevealPanel();

  
  void EnsureDragHelperCreated();

  void SetBoundsInternal(const gfx::Rect& bounds);

  void LoadingAnimationCallback();

  
  gfx::Size GetNonClientFrameSize() const;

  
  void InvalidateWindow();

  
  
  static gboolean OnGtkAccelerator(GtkAccelGroup* accel_group,
                                   GObject* acceleratable,
                                   guint keyval,
                                   GdkModifierType modifier,
                                   void* user_data);

  CHROMEGTK_CALLBACK_1(PanelGtk, gboolean, OnMainWindowDeleteEvent,
                       GdkEvent*);
  CHROMEGTK_CALLBACK_0(PanelGtk, void, OnMainWindowDestroy);
  CHROMEGTK_CALLBACK_1(PanelGtk, gboolean, OnConfigure, GdkEventConfigure*);
  CHROMEGTK_CALLBACK_1(PanelGtk, gboolean, OnWindowState, GdkEventWindowState*);
  
  CHROMEGTK_CALLBACK_1(PanelGtk, gboolean, OnCustomFrameExpose,
                       GdkEventExpose*);
  
  CHROMEGTK_CALLBACK_1(PanelGtk, gboolean, OnKeyPress, GdkEventKey*);
  
  
  CHROMEGTK_CALLBACK_1(PanelGtk, gboolean, OnMouseMoveEvent,
                       GdkEventMotion*);
  CHROMEGTK_CALLBACK_1(PanelGtk, gboolean, OnButtonPressEvent,
                       GdkEventButton*);
  CHROMEGTK_CALLBACK_1(PanelGtk, gboolean,
                       OnTitlebarButtonPressEvent, GdkEventButton*);
  CHROMEGTK_CALLBACK_1(PanelGtk, gboolean,
                       OnTitlebarButtonReleaseEvent, GdkEventButton*);

  scoped_ptr<Panel> panel_;
  gfx::Rect bounds_;

  
  bool always_on_top_;

  
  bool is_shown_;

  scoped_ptr<PanelDragGtk> drag_helper_;

  
  
  gfx::Size configure_size_;

  
  PaintState paint_state_;

  
  bool is_drawing_attention_;

  
  base::RepeatingTimer<PanelGtk> loading_animation_timer_;

  
  
  GdkCursor* frame_cursor_;

  
  
  
  bool is_active_;

  
  
  
  bool is_minimized_;

  
  GtkWindow* window_;
  
  
  GtkWidget* window_container_;
  
  GtkWidget* window_vbox_;
  
  GtkWidget* render_area_event_box_;
  
  GtkWidget* contents_expanded_;

  
  GtkAccelGroup* accel_group_;

  
  scoped_ptr<PanelTitlebarGtk> titlebar_;

  
  panel::CornerStyle corner_style_;

  DISALLOW_COPY_AND_ASSIGN(PanelGtk);
};

#endif  

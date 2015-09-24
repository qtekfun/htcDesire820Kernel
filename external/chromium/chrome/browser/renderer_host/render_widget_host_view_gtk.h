// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_GTK_H_
#define CHROME_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_GTK_H_
#pragma once

#include <gdk/gdk.h>

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/time.h"
#include "chrome/browser/ui/gtk/owned_widget_gtk.h"
#include "content/browser/renderer_host/render_widget_host_view.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/base/animation/slide_animation.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"
#include "webkit/glue/webcursor.h"
#include "webkit/plugins/npapi/gtk_plugin_container_manager.h"

class RenderWidgetHost;
class GtkIMContextWrapper;
class GtkKeyBindingsHandler;
#if !defined(TOOLKIT_VIEWS)
class MenuGtk;
#endif
struct NativeWebKeyboardEvent;

#if defined(OS_CHROMEOS)
namespace views {
class TooltipWindowGtk;
}
#endif  

typedef struct _GtkClipboard GtkClipboard;
typedef struct _GtkSelectionData GtkSelectionData;

class RenderWidgetHostViewGtk : public RenderWidgetHostView,
                                public ui::AnimationDelegate {
 public:
  explicit RenderWidgetHostViewGtk(RenderWidgetHost* widget);
  ~RenderWidgetHostViewGtk();

  
  void InitAsChild();

  
  virtual void InitAsPopup(RenderWidgetHostView* parent_host_view,
                           const gfx::Rect& pos);
  virtual void InitAsFullscreen();
  virtual RenderWidgetHost* GetRenderWidgetHost() const;
  virtual void DidBecomeSelected();
  virtual void WasHidden();
  virtual void SetSize(const gfx::Size& size);
  virtual void SetBounds(const gfx::Rect& rect);
  virtual gfx::NativeView GetNativeView();
  virtual void MovePluginWindows(
      const std::vector<webkit::npapi::WebPluginGeometry>& moves);
  virtual void Focus();
  virtual void Blur();
  virtual bool HasFocus();
  virtual void Show();
  virtual void Hide();
  virtual bool IsShowing();
  virtual gfx::Rect GetViewBounds() const;
  virtual void UpdateCursor(const WebCursor& cursor);
  virtual void SetIsLoading(bool is_loading);
  virtual void ImeUpdateTextInputState(WebKit::WebTextInputType type,
                                       const gfx::Rect& caret_rect);
  virtual void ImeCancelComposition();
  virtual void DidUpdateBackingStore(
      const gfx::Rect& scroll_rect, int scroll_dx, int scroll_dy,
      const std::vector<gfx::Rect>& copy_rects);
  virtual void RenderViewGone(base::TerminationStatus status,
                              int error_code);
  virtual void Destroy();
  virtual void WillDestroyRenderWidget(RenderWidgetHost* rwh) {}
  virtual void SetTooltipText(const std::wstring& tooltip_text);
  virtual void SelectionChanged(const std::string& text);
  virtual void ShowingContextMenu(bool showing);
  virtual BackingStore* AllocBackingStore(const gfx::Size& size);
  virtual void SetBackground(const SkBitmap& background);
  virtual void CreatePluginContainer(gfx::PluginWindowHandle id);
  virtual void DestroyPluginContainer(gfx::PluginWindowHandle id);
  virtual void SetVisuallyDeemphasized(const SkColor* color, bool animate);
  virtual bool ContainsNativeView(gfx::NativeView native_view) const;

  virtual void AcceleratedCompositingActivated(bool activated);
  virtual gfx::PluginWindowHandle GetCompositingSurface();

  
  virtual void AnimationEnded(const ui::Animation* animation);
  virtual void AnimationProgressed(const ui::Animation* animation);
  virtual void AnimationCanceled(const ui::Animation* animation);

  gfx::NativeView native_view() const { return view_.get(); }

  
  
  
  
  void ModifyEventForEdgeDragging(GtkWidget* widget, GdkEventMotion* event);
  void Paint(const gfx::Rect&);

  
  
  
  
  
  void ForwardKeyboardEvent(const NativeWebKeyboardEvent& event);

  GdkEventButton* last_mouse_down() const {
    return last_mouse_down_;
  }

#if !defined(TOOLKIT_VIEWS)
  
  
  void AppendInputMethodsContextMenu(MenuGtk* menu);
#endif

 private:
  friend class RenderWidgetHostViewGtkWidget;

  CHROMEGTK_CALLBACK_1(RenderWidgetHostViewGtk,
                       gboolean,
                       OnWindowStateEvent,
                       GdkEventWindowState*);

  CHROMEGTK_CALLBACK_0(RenderWidgetHostViewGtk,
                       void,
                       OnDestroy);

  
  
  bool NeedsInputGrab();

  
  
  bool IsPopup() const;

  
  void DoSharedInit();

  
  
  void DoPopupOrFullscreenInit(GtkWindow* window, const gfx::Rect& bounds);

  
  void ShowCurrentCursor();

  void set_last_mouse_down(GdkEventButton* event);

  
  RenderWidgetHost* host_;

  
  OwnedWidgetGtk view_;

  
  
  
  bool about_to_validate_and_paint_;

  
  gfx::Rect invalid_rect_;

  
  bool is_hidden_;

  
  bool is_loading_;

  
  WebCursor current_cursor_;

  
  bool is_showing_context_menu_;

  
  
  
  base::TimeTicks whiteout_start_time_;

  
  base::TimeTicks tab_switch_paint_time_;

  
  
  SkColor overlay_color_;

  
  
  ui::SlideAnimation overlay_animation_;

  
  GtkWidget* parent_;

  
  bool is_popup_first_mouse_release_;

  
  
  bool was_focused_before_grab_;

  
  
  
  bool do_x_grab_;

  
  bool is_fullscreen_;

  
  
  unsigned long destroy_handler_id_;

  
  scoped_ptr<GtkIMContextWrapper> im_context_;

  
  
  scoped_ptr<GtkKeyBindingsHandler> key_bindings_handler_;

  
  webkit::npapi::GtkPluginContainerManager plugin_container_manager_;

  
  
  gfx::Size requested_size_;

  
  
  
  int dragged_at_horizontal_edge_;

  
  
  
  int dragged_at_vertical_edge_;

  gfx::PluginWindowHandle compositing_surface_;

  
  
  GdkEventButton* last_mouse_down_;

#if defined(OS_CHROMEOS)
  
  scoped_ptr<views::TooltipWindowGtk> tooltip_window_;
#endif  
};

#endif  

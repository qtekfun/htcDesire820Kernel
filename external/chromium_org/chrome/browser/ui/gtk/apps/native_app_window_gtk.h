// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_APPS_NATIVE_APP_WINDOW_GTK_H_
#define CHROME_BROWSER_UI_GTK_APPS_NATIVE_APP_WINDOW_GTK_H_

#include <gtk/gtk.h>

#include "apps/shell_window.h"
#include "apps/ui/native_app_window.h"
#include "base/observer_list.h"
#include "base/timer/timer.h"
#include "chrome/browser/ui/gtk/extensions/extension_view_gtk.h"
#include "content/public/browser/web_contents_observer.h"
#include "third_party/skia/include/core/SkRegion.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/x/active_window_watcher_x_observer.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/x/x11_atom_cache.h"

class ExtensionKeybindingRegistryGtk;
class Profile;

namespace extensions {
class Extension;
}

class NativeAppWindowGtk : public apps::NativeAppWindow,
                           public ExtensionViewGtk::Container,
                           public ui::ActiveWindowWatcherXObserver,
                           public content::WebContentsObserver {
 public:
  NativeAppWindowGtk(apps::ShellWindow* shell_window,
                     const apps::ShellWindow::CreateParams& params);

  
  virtual bool IsActive() const OVERRIDE;
  virtual bool IsMaximized() const OVERRIDE;
  virtual bool IsMinimized() const OVERRIDE;
  virtual bool IsFullscreen() const OVERRIDE;
  virtual gfx::NativeWindow GetNativeWindow() OVERRIDE;
  virtual gfx::Rect GetRestoredBounds() const OVERRIDE;
  virtual ui::WindowShowState GetRestoredState() const OVERRIDE;
  virtual gfx::Rect GetBounds() const OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void ShowInactive() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual void Activate() OVERRIDE;
  virtual void Deactivate() OVERRIDE;
  virtual void Maximize() OVERRIDE;
  virtual void Minimize() OVERRIDE;
  virtual void Restore() OVERRIDE;
  virtual void SetBounds(const gfx::Rect& bounds) OVERRIDE;
  virtual void FlashFrame(bool flash) OVERRIDE;
  virtual bool IsAlwaysOnTop() const OVERRIDE;
  virtual void SetAlwaysOnTop(bool always_on_top) OVERRIDE;

  
  virtual void ActiveWindowChanged(GdkWindow* active_window) OVERRIDE;

  
  virtual void RenderViewHostChanged(
      content::RenderViewHost* old_host,
      content::RenderViewHost* new_host) OVERRIDE;

 private:
  
  virtual void SetFullscreen(int fullscreen_types) OVERRIDE;
  virtual bool IsFullscreenOrPending() const OVERRIDE;
  virtual bool IsDetached() const OVERRIDE;
  virtual void UpdateWindowIcon() OVERRIDE;
  virtual void UpdateWindowTitle() OVERRIDE;
  virtual void UpdateDraggableRegions(
      const std::vector<extensions::DraggableRegion>& regions) OVERRIDE;
  virtual SkRegion* GetDraggableRegion() OVERRIDE;
  virtual void UpdateShape(scoped_ptr<SkRegion> region) OVERRIDE;
  virtual void HandleKeyboardEvent(
      const content::NativeWebKeyboardEvent& event) OVERRIDE;
  virtual bool IsFrameless() const OVERRIDE;
  virtual gfx::Insets GetFrameInsets() const OVERRIDE;
  virtual void HideWithApp() OVERRIDE;
  virtual void ShowWithApp() OVERRIDE;
  
  virtual void UpdateWindowMinMaxSize() OVERRIDE;

  
  virtual gfx::NativeView GetHostView() const OVERRIDE;
  virtual gfx::Point GetDialogPosition(const gfx::Size& size) OVERRIDE;
  virtual gfx::Size GetMaximumDialogSize() OVERRIDE;
  virtual void AddObserver(
      web_modal::ModalDialogHostObserver* observer) OVERRIDE;
  virtual void RemoveObserver(
      web_modal::ModalDialogHostObserver* observer) OVERRIDE;

  content::WebContents* web_contents() const {
    return shell_window_->web_contents();
  }
  const extensions::Extension* extension() const {
    return shell_window_->extension();
  }

  virtual ~NativeAppWindowGtk();

  
  
  
  bool GetWindowEdge(int x, int y, GdkWindowEdge* edge);

  CHROMEGTK_CALLBACK_1(NativeAppWindowGtk, gboolean, OnMainWindowDeleteEvent,
                       GdkEvent*);
  CHROMEGTK_CALLBACK_1(NativeAppWindowGtk, gboolean, OnConfigure,
                       GdkEventConfigure*);
  CHROMEGTK_CALLBACK_1(NativeAppWindowGtk, gboolean, OnWindowState,
                       GdkEventWindowState*);
  CHROMEGTK_CALLBACK_1(NativeAppWindowGtk, gboolean, OnMouseMoveEvent,
                       GdkEventMotion*);
  CHROMEGTK_CALLBACK_1(NativeAppWindowGtk, gboolean, OnButtonPress,
                       GdkEventButton*);
  
  CHROMEG_CALLBACK_1(NativeAppWindowGtk, GdkFilterReturn,
                     OnXEvent, GdkXEvent*, GdkEvent*);

  void OnConfigureDebounced();

  apps::ShellWindow* shell_window_;  

  GtkWindow* window_;
  GdkWindowState state_;

  
  
  
  bool is_active_;

  
  gfx::Rect bounds_;

  
  gfx::Rect restored_bounds_;

  
  
  bool content_thinks_its_fullscreen_;

  
  bool maximize_pending_;

  
  
  scoped_ptr<SkRegion> draggable_region_;

  
  
  bool suppress_window_raise_;

  
  bool frameless_;

  
  bool resizable_;

  
  
  bool always_on_top_;

  
  
  GdkCursor* frame_cursor_;

  
  base::OneShotTimer<NativeAppWindowGtk> window_configure_debounce_timer_;

  
  
  
  scoped_ptr<ExtensionKeybindingRegistryGtk> extension_keybinding_registry_;

  
  
  ObserverList<web_modal::ModalDialogHostObserver> observer_list_;

  ui::X11AtomCache atom_cache_;

  
  bool is_x_event_listened_;

  DISALLOW_COPY_AND_ASSIGN(NativeAppWindowGtk);
};

#endif  

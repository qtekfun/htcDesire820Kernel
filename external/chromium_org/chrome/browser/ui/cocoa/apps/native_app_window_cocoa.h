// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_APPS_NATIVE_APP_WINDOW_COCOA_H_
#define CHROME_BROWSER_UI_COCOA_APPS_NATIVE_APP_WINDOW_COCOA_H_

#import <Cocoa/Cocoa.h>
#include <vector>

#include "apps/shell_window.h"
#include "apps/ui/native_app_window.h"
#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/browser_command_executor.h"
#include "content/public/browser/web_contents_observer.h"
#include "extensions/common/draggable_region.h"
#include "ui/gfx/rect.h"

class ExtensionKeybindingRegistryCocoa;
class Profile;
class NativeAppWindowCocoa;
@class ShellNSWindow;
class SkRegion;

@interface NativeAppWindowController : NSWindowController
                                      <NSWindowDelegate,
                                       BrowserCommandExecutor> {
 @private
  NativeAppWindowCocoa* appWindow_;  
}

@property(assign, nonatomic) NativeAppWindowCocoa* appWindow;

- (BOOL)handledByExtensionCommand:(NSEvent*)event;

@end

class NativeAppWindowCocoa : public apps::NativeAppWindow,
                             public content::WebContentsObserver {
 public:
  NativeAppWindowCocoa(apps::ShellWindow* shell_window,
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

  
  void WindowWillClose();

  
  void WindowDidBecomeKey();

  
  void WindowDidResignKey();

  
  
  void WindowDidFinishResize();

  
  
  void WindowDidResize();

  
  void WindowDidMove();

  
  void WindowDidMiniaturize();

  
  void WindowDidDeminiaturize();

  
  void WindowWillZoom();

  
  bool HandledByExtensionCommand(NSEvent* event);

  
  void HandleMouseEvent(NSEvent* event);

  
  
  bool IsWithinDraggableRegion(NSPoint point) const;

  NSRect restored_bounds() const { return restored_bounds_; }
  bool use_system_drag() const { return use_system_drag_; }

 protected:
  
  virtual void SetFullscreen(int fullscreen_types) OVERRIDE;
  virtual bool IsFullscreenOrPending() const OVERRIDE;
  virtual bool IsDetached() const OVERRIDE;
  virtual void UpdateWindowIcon() OVERRIDE;
  virtual void UpdateWindowTitle() OVERRIDE;
  virtual void UpdateShape(scoped_ptr<SkRegion> region) OVERRIDE;
  virtual void UpdateDraggableRegions(
      const std::vector<extensions::DraggableRegion>& regions) OVERRIDE;
  virtual SkRegion* GetDraggableRegion() OVERRIDE;
  virtual void HandleKeyboardEvent(
      const content::NativeWebKeyboardEvent& event) OVERRIDE;
  virtual bool IsFrameless() const OVERRIDE;
  virtual gfx::Insets GetFrameInsets() const OVERRIDE;

  
  
  
  virtual void ShowWithApp() OVERRIDE;
  virtual void HideWithApp() OVERRIDE;
  
  virtual void UpdateWindowMinMaxSize() OVERRIDE;

  
  virtual void RenderViewHostChanged(
      content::RenderViewHost* old_host,
      content::RenderViewHost* new_host) OVERRIDE;

  virtual void SetAlwaysOnTop(bool always_on_top) OVERRIDE;

  
  virtual gfx::NativeView GetHostView() const OVERRIDE;
  virtual gfx::Point GetDialogPosition(const gfx::Size& size) OVERRIDE;
  virtual gfx::Size GetMaximumDialogSize() OVERRIDE;
  virtual void AddObserver(
      web_modal::ModalDialogHostObserver* observer) OVERRIDE;
  virtual void RemoveObserver(
      web_modal::ModalDialogHostObserver* observer) OVERRIDE;

 private:
  virtual ~NativeAppWindowCocoa();

  ShellNSWindow* window() const;

  content::WebContents* web_contents() const {
    return shell_window_->web_contents();
  }
  const extensions::Extension* extension() const {
    return shell_window_->extension();
  }

  
  
  
  
  NSUInteger GetWindowStyleMask() const;

  void InstallView();
  void UninstallView();
  void InstallDraggableRegionViews();
  void UpdateDraggableRegionsForSystemDrag(
      const std::vector<extensions::DraggableRegion>& regions,
      const extensions::DraggableRegion* draggable_area);
  void UpdateDraggableRegionsForCustomDrag(
      const std::vector<extensions::DraggableRegion>& regions);

  
  void UpdateRestoredBounds();

  
  void HideWithoutMarkingHidden();

  apps::ShellWindow* shell_window_; 

  bool has_frame_;

  
  
  bool is_hidden_;
  
  
  bool is_hidden_with_app_;

  bool is_maximized_;
  bool is_fullscreen_;
  NSRect restored_bounds_;

  bool shows_resize_controls_;
  bool shows_fullscreen_controls_;

  base::scoped_nsobject<NativeAppWindowController> window_controller_;
  NSInteger attention_request_id_;  

  
  
  bool use_system_drag_;

  
  
  std::vector<gfx::Rect> system_drag_exclude_areas_;

  
  
  scoped_ptr<SkRegion> draggable_region_;  

  
  
  NSPoint last_mouse_location_;

  
  
  scoped_ptr<ExtensionKeybindingRegistryCocoa> extension_keybinding_registry_;

  DISALLOW_COPY_AND_ASSIGN(NativeAppWindowCocoa);
};

#endif  

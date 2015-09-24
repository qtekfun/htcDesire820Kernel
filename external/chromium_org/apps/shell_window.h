// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef APPS_SHELL_WINDOW_H_
#define APPS_SHELL_WINDOW_H_

#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/extensions/extension_icon_image.h"
#include "chrome/browser/extensions/extension_keybinding_registry.h"
#include "chrome/browser/sessions/session_id.h"
#include "components/web_modal/web_contents_modal_dialog_manager_delegate.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_delegate.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/common/console_message_level.h"
#include "ui/base/ui_base_types.h"  
#include "ui/gfx/image/image.h"
#include "ui/gfx/rect.h"

class GURL;
class Profile;
class SkRegion;

namespace content {
class WebContents;
}

namespace extensions {
class Extension;
class PlatformAppBrowserTest;
class WindowController;

struct DraggableRegion;
}

namespace ui {
class BaseWindow;
}

namespace apps {

class NativeAppWindow;

class ShellWindowContents {
 public:
  ShellWindowContents() {}
  virtual ~ShellWindowContents() {}

  
  virtual void Initialize(Profile* profile, const GURL& url) = 0;

  
  virtual void LoadContents(int32 creator_process_id) = 0;

  
  virtual void NativeWindowChanged(NativeAppWindow* native_app_window) = 0;

  
  virtual void NativeWindowClosed() = 0;

  virtual content::WebContents* GetWebContents() const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(ShellWindowContents);
};

class ShellWindow : public content::NotificationObserver,
                    public content::WebContentsDelegate,
                    public content::WebContentsObserver,
                    public web_modal::WebContentsModalDialogManagerDelegate,
                    public extensions::ExtensionKeybindingRegistry::Delegate,
                    public extensions::IconImage::Observer {
 public:
  enum WindowType {
    WINDOW_TYPE_DEFAULT  = 1 << 0,  
    WINDOW_TYPE_PANEL    = 1 << 1,  
    WINDOW_TYPE_V1_PANEL = 1 << 2,  
                                    
  };

  enum Frame {
    FRAME_CHROME,  
    FRAME_NONE,  
  };

  enum FullscreenType {
    
    FULLSCREEN_TYPE_NONE = 0,

    
    FULLSCREEN_TYPE_WINDOW_API = 1 << 0,

    
    FULLSCREEN_TYPE_HTML_API = 1 << 1,

    
    
    FULLSCREEN_TYPE_OS = 1 << 2,

    
    
    FULLSCREEN_TYPE_FORCED = 1 << 3,
  };

  class SizeConstraints {
   public:
    
    
    enum { kUnboundedSize = 0 };

    SizeConstraints();
    SizeConstraints(const gfx::Size& min_size, const gfx::Size& max_size);
    ~SizeConstraints();

    
    gfx::Size ClampSize(gfx::Size size) const;

    
    
    
    
    bool HasMinimumSize() const;
    bool HasMaximumSize() const;

    
    
    bool HasFixedSize() const;

    gfx::Size GetMaximumSize() const;
    gfx::Size GetMinimumSize() const;

    void set_minimum_size(const gfx::Size& min_size);
    void set_maximum_size(const gfx::Size& max_size);

   private:
    gfx::Size minimum_size_;
    gfx::Size maximum_size_;
  };

  struct CreateParams {
    CreateParams();
    ~CreateParams();

    WindowType window_type;
    Frame frame;
    bool transparent_background;  

    
    
    
    
    gfx::Rect bounds;

    gfx::Size minimum_size;
    gfx::Size maximum_size;

    std::string window_key;

    
    int32 creator_process_id;

    
    ui::WindowShowState state;

    
    bool hidden;

    
    bool resizable;

    
    bool focused;

    
    
    bool always_on_top;
  };

  class Delegate {
   public:
    virtual ~Delegate();

    
    virtual void InitWebContents(content::WebContents* web_contents) = 0;
    virtual NativeAppWindow* CreateNativeAppWindow(
        ShellWindow* window,
        const CreateParams& params) = 0;

    
    virtual content::WebContents* OpenURLFromTab(
        Profile* profile,
        content::WebContents* source,
        const content::OpenURLParams& params) = 0;
    virtual void AddNewContents(Profile* profile,
                                content::WebContents* new_contents,
                                WindowOpenDisposition disposition,
                                const gfx::Rect& initial_pos,
                                bool user_gesture,
                                bool* was_blocked) = 0;

    
    virtual content::ColorChooser* ShowColorChooser(
        content::WebContents* web_contents,
        SkColor initial_color) = 0;
    virtual void RunFileChooser(content::WebContents* tab,
                                const content::FileChooserParams& params) = 0;
    virtual void RequestMediaAccessPermission(
        content::WebContents* web_contents,
        const content::MediaStreamRequest& request,
        const content::MediaResponseCallback& callback,
      const extensions::Extension* extension) = 0;
    virtual int PreferredIconSize() = 0;

    
    virtual void SetWebContentsBlocked(content::WebContents* web_contents,
                                       bool blocked) = 0;
    virtual bool IsWebContentsVisible(content::WebContents* web_contents) = 0;
  };

  
  
  static SkRegion* RawDraggableRegionsToSkRegion(
      const std::vector<extensions::DraggableRegion>& regions);

  
  
  
  
  ShellWindow(Profile* profile,
              Delegate* delegate,
              const extensions::Extension* extension);

  
  
  void Init(const GURL& url,
            ShellWindowContents* shell_window_contents,
            const CreateParams& params);


  const std::string& window_key() const { return window_key_; }
  const SessionID& session_id() const { return session_id_; }
  const extensions::Extension* extension() const { return extension_; }
  const std::string& extension_id() const { return extension_id_; }
  content::WebContents* web_contents() const;
  WindowType window_type() const { return window_type_; }
  bool window_type_is_panel() const {
    return (window_type_ == WINDOW_TYPE_PANEL ||
            window_type_ == WINDOW_TYPE_V1_PANEL);
  }
  Profile* profile() const { return profile_; }
  const gfx::Image& app_icon() const { return app_icon_; }
  const GURL& app_icon_url() { return app_icon_url_; }

  NativeAppWindow* GetBaseWindow();
  gfx::NativeWindow GetNativeWindow();

  
  gfx::Rect GetClientBounds() const;

  
  
  string16 GetTitle() const;

  
  
  void OnNativeClose();

  
  
  void OnNativeWindowChanged();

  
  void OnNativeWindowActivated();

  
  void SetAppIconUrl(const GURL& icon_url);

  
  void UpdateShape(scoped_ptr<SkRegion> region);

  
  void UpdateDraggableRegions(
      const std::vector<extensions::DraggableRegion>& regions);

  
  
  void UpdateAppIcon(const gfx::Image& image);

  
  
  void Fullscreen();
  void Maximize();
  void Minimize();
  void Restore();

  
  void OSFullscreen();

  
  
  void ForcedFullscreen();

  
  void SetMinimumSize(const gfx::Size& min_size);
  void SetMaximumSize(const gfx::Size& max_size);

  enum ShowType {
    SHOW_ACTIVE,
    SHOW_INACTIVE
  };

  
  
  void Show(ShowType show_type);

  
  
  void Hide();

  ShellWindowContents* shell_window_contents_for_test() {
    return shell_window_contents_.get();
  }

  
  const SizeConstraints& size_constraints() const {
    return size_constraints_;
  }

  
  
  void SetAlwaysOnTop(bool always_on_top);

  
  
  
  bool IsAlwaysOnTop() const;

 protected:
  virtual ~ShellWindow();

 private:
  
  friend class extensions::PlatformAppBrowserTest;

  
  virtual void CloseContents(content::WebContents* contents) OVERRIDE;
  virtual bool ShouldSuppressDialogs() OVERRIDE;
  virtual content::ColorChooser* OpenColorChooser(
      content::WebContents* web_contents,
      SkColor color,
      const std::vector<content::ColorSuggestion>& suggestions) OVERRIDE;
  virtual void RunFileChooser(
      content::WebContents* tab,
      const content::FileChooserParams& params) OVERRIDE;
  virtual bool IsPopupOrPanel(
      const content::WebContents* source) const OVERRIDE;
  virtual void MoveContents(
      content::WebContents* source, const gfx::Rect& pos) OVERRIDE;
  virtual void NavigationStateChanged(const content::WebContents* source,
                                      unsigned changed_flags) OVERRIDE;
  virtual void ToggleFullscreenModeForTab(content::WebContents* source,
                                          bool enter_fullscreen) OVERRIDE;
  virtual bool IsFullscreenForTabOrPending(
      const content::WebContents* source) const OVERRIDE;
  virtual void RequestMediaAccessPermission(
      content::WebContents* web_contents,
      const content::MediaStreamRequest& request,
      const content::MediaResponseCallback& callback) OVERRIDE;
  virtual content::WebContents* OpenURLFromTab(
      content::WebContents* source,
      const content::OpenURLParams& params) OVERRIDE;
  virtual void AddNewContents(content::WebContents* source,
                              content::WebContents* new_contents,
                              WindowOpenDisposition disposition,
                              const gfx::Rect& initial_pos,
                              bool user_gesture,
                              bool* was_blocked) OVERRIDE;
  virtual bool PreHandleKeyboardEvent(
      content::WebContents* source,
      const content::NativeWebKeyboardEvent& event,
      bool* is_keyboard_shortcut) OVERRIDE;
  virtual void HandleKeyboardEvent(
      content::WebContents* source,
      const content::NativeWebKeyboardEvent& event) OVERRIDE;
  virtual void RequestToLockMouse(content::WebContents* web_contents,
                                  bool user_gesture,
                                  bool last_unlocked_by_target) OVERRIDE;

  
  virtual void DidFirstVisuallyNonEmptyPaint(int32 page_id) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void SetWebContentsBlocked(content::WebContents* web_contents,
                                     bool blocked) OVERRIDE;
  virtual bool IsWebContentsVisible(
      content::WebContents* web_contents) OVERRIDE;

  
  void AddMessageToDevToolsConsole(content::ConsoleMessageLevel level,
                                   const std::string& message);

  
  void SaveWindowPosition();

  
  
  void AdjustBoundsToBeVisibleOnScreen(
      const gfx::Rect& cached_bounds,
      const gfx::Rect& cached_screen_bounds,
      const gfx::Rect& current_screen_bounds,
      const gfx::Size& minimum_size,
      gfx::Rect* bounds) const;

  
  
  
  CreateParams LoadDefaultsAndConstrain(CreateParams params) const;

  
  void UpdateExtensionAppIcon();

  
  void OnSizeConstraintsChanged();

  
  void SetNativeWindowFullscreen(int fullscreen_types);

  
  virtual extensions::ActiveTabPermissionGranter*
      GetActiveTabPermissionGranter() OVERRIDE;

  
  virtual web_modal::WebContentsModalDialogHost*
      GetWebContentsModalDialogHost() OVERRIDE;

  
  void DidDownloadFavicon(int id,
                          int http_status_code,
                          const GURL& image_url,
                          const std::vector<SkBitmap>& bitmaps,
                          const std::vector<gfx::Size>& original_bitmap_sizes);

  
  virtual void OnExtensionIconImageChanged(
      extensions::IconImage* image) OVERRIDE;

  Profile* profile_;  
  
  const extensions::Extension* extension_;
  const std::string extension_id_;

  
  
  std::string window_key_;

  const SessionID session_id_;
  WindowType window_type_;
  content::NotificationRegistrar registrar_;

  
  gfx::Image app_icon_;

  
  
  GURL app_icon_url_;

  
  scoped_ptr<extensions::IconImage> app_icon_image_;

  scoped_ptr<NativeAppWindow> native_app_window_;
  scoped_ptr<ShellWindowContents> shell_window_contents_;
  scoped_ptr<Delegate> delegate_;

  base::WeakPtrFactory<ShellWindow> image_loader_ptr_factory_;

  
  int fullscreen_types_;

  
  SizeConstraints size_constraints_;

  
  
  bool show_on_first_paint_;

  
  bool first_paint_complete_;

  
  ShowType delayed_show_type_;

  
  
  
  bool cached_always_on_top_;

  DISALLOW_COPY_AND_ASSIGN(ShellWindow);
};

}  

#endif  

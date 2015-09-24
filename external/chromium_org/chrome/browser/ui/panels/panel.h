// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_PANELS_PANEL_H_
#define CHROME_BROWSER_UI_PANELS_PANEL_H_

#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/command_updater.h"
#include "chrome/browser/command_updater_delegate.h"
#include "chrome/browser/sessions/session_id.h"
#include "chrome/browser/ui/panels/panel_constants.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/base_window.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/rect.h"

class GURL;
class NativePanel;
class PanelCollection;
class PanelHost;
class PanelManager;
class Profile;
class StackedPanelCollection;

namespace content {
class WebContents;
struct NativeWebKeyboardEvent;
}

namespace extensions {
class Extension;
class WindowController;
}

class Panel : public ui::BaseWindow,
              public CommandUpdaterDelegate,
              public content::NotificationObserver {
 public:
  enum ExpansionState {
    
    EXPANDED,
    
    TITLE_ONLY,
    
    MINIMIZED
  };

  
  enum AttentionMode {
    
    
    USE_PANEL_ATTENTION = 0x01,
    
    
    
    USE_SYSTEM_ATTENTION = 0x02
  };

  virtual ~Panel();

  
  PanelManager* manager() const;

  const std::string& app_name() const { return app_name_; }
  const gfx::Image& app_icon() const { return app_icon_; }
  const SessionID& session_id() const { return session_id_; }
  extensions::WindowController* extension_window_controller() const {
    return extension_window_controller_.get();
  }
  const std::string extension_id() const;

  CommandUpdater* command_updater();
  Profile* profile() const;

  const extensions::Extension* GetExtension() const;

  
  
  content::WebContents* GetWebContents() const;

  void SetExpansionState(ExpansionState new_expansion_state);

  bool IsDrawingAttention() const;

  
  
  
  
  
  void FullScreenModeChanged(bool is_full_screen);

  int TitleOnlyHeight() const;

  
  
  bool CanShowMinimizeButton() const;
  bool CanShowRestoreButton() const;

  
  virtual bool IsActive() const OVERRIDE;
  virtual bool IsMaximized() const OVERRIDE;
  virtual bool IsMinimized() const OVERRIDE;
  virtual bool IsFullscreen() const OVERRIDE;
  virtual gfx::NativeWindow GetNativeWindow() OVERRIDE;
  virtual gfx::Rect GetRestoredBounds() const OVERRIDE;
  virtual ui::WindowShowState GetRestoredState() const OVERRIDE;
  virtual gfx::Rect GetBounds() const OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void ShowInactive() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual void Activate() OVERRIDE;
  virtual void Deactivate() OVERRIDE;
  virtual void Maximize() OVERRIDE;
  virtual void Minimize() OVERRIDE;
  virtual void Restore() OVERRIDE;
  virtual void SetBounds(const gfx::Rect& bounds) OVERRIDE;
  virtual void FlashFrame(bool flash) OVERRIDE;
  virtual bool IsAlwaysOnTop() const OVERRIDE;
  virtual void SetAlwaysOnTop(bool on_top) OVERRIDE;

  
  virtual void ExecuteCommandWithDisposition(
      int id,
      WindowOpenDisposition disposition) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  static NativePanel* CreateNativePanel(Panel* panel,
                                        const gfx::Rect& bounds,
                                        bool always_on_top);

  NativePanel* native_panel() const { return native_panel_; }

  
  
  
  void OnTitlebarClicked(panel::ClickModifier modifier);
  void OnMinimizeButtonClicked(panel::ClickModifier modifier);
  void OnRestoreButtonClicked(panel::ClickModifier modifier);

  
  
  void OnWindowSizeAvailable();

  
  void OnNativePanelClosed();

  
  
  
  
  PanelCollection* collection() const { return collection_; }

  
  void set_collection(PanelCollection* new_collection) {
    collection_ = new_collection;
  }

  StackedPanelCollection* stack() const;

  ExpansionState expansion_state() const { return expansion_state_; }
  const gfx::Size& min_size() const { return min_size_; }
  const gfx::Size& max_size() const { return max_size_; }
  bool auto_resizable() const { return auto_resizable_; }

  bool in_preview_mode() const { return in_preview_mode_; }

  panel::Resizability CanResizeByMouse() const;

  AttentionMode attention_mode() const { return attention_mode_; }
  void set_attention_mode(AttentionMode attention_mode) {
    attention_mode_ = attention_mode;
  }

  
  
  gfx::Size full_size() const { return full_size_; }
  void set_full_size(const gfx::Size& size) { full_size_ = size; }

  
  
  bool initialized() const { return initialized_; }
  void Initialize(const GURL& url, const gfx::Rect& bounds, bool always_on_top);

  
  
  
  
  void SetPanelBounds(const gfx::Rect& bounds);

  
  void SetPanelBoundsInstantly(const gfx::Rect& bounds);

  
  
  
  
  void LimitSizeToWorkArea(const gfx::Rect& work_area);

  
  void SetAutoResizable(bool resizable);

  
  
  void EnableWebContentsAutoResize(content::WebContents* web_contents);

  
  
  void OnContentsAutoResized(const gfx::Size& new_content_size);

  
  
  void OnWindowResizedByMouse(const gfx::Rect& new_bounds);

  
  void SetSizeRange(const gfx::Size& min_size, const gfx::Size& max_size);

  
  
  
  
  
  void IncreaseMaxSize(const gfx::Size& desired_panel_size);

  
  void HandleKeyboardEvent(const content::NativeWebKeyboardEvent& event);

  
  
  void SetPreviewMode(bool in_preview_mode);

  
  void UpdateMinimizeRestoreButtonVisibility();

  
  gfx::Size ClampSize(const gfx::Size& size) const;

  
  
  void OnActiveStateChanged(bool active);

  
  void OnPanelStartUserResizing();
  void OnPanelEndUserResizing();

  
  bool ShouldCloseWindow();

  
  
  void OnWindowClosing();

  
  
  bool ExecuteCommandIfEnabled(int id);

  
  base::string16 GetWindowTitle() const;

  
  gfx::Image GetCurrentPageIcon() const;

  
  void UpdateTitleBar();

  
  void LoadingStateChanged(bool is_loading);

  
  void WebContentsFocused(content::WebContents* contents);

  
  void MoveByInstantly(const gfx::Vector2d& delta_origin);

  
  void SetWindowCornerStyle(panel::CornerStyle corner_style);

  
  void MinimizeBySystem();

  bool IsMinimizedBySystem() const;

  
  
  bool IsShownOnActiveDesktop() const;

  
  void ShowShadow(bool show);

 protected:
  
  
  
  
  Panel(Profile* profile, const std::string& app_name,
        const gfx::Size& min_size, const gfx::Size& max_size);

 private:
  friend class PanelManager;
  friend class PanelBrowserTest;

  enum MaxSizePolicy {
    
    DEFAULT_MAX_SIZE,
    
    CUSTOM_MAX_SIZE
  };

  void OnImageLoaded(const gfx::Image& image);

  
  void InitCommandState();

  
  void ConfigureAutoResize(content::WebContents* web_contents);

  
  void UpdateAppIcon();

  
  static void FormatTitleForDisplay(base::string16* title);

  
  
  
  const std::string app_name_;

  Profile* profile_;

  
  
  PanelCollection* collection_;  

  bool initialized_;

  
  
  gfx::Size full_size_;

  
  gfx::Size min_size_;

  
  
  gfx::Size max_size_;

  MaxSizePolicy max_size_policy_;

  
  bool auto_resizable_;

  
  
  
  
  bool in_preview_mode_;

  
  
  NativePanel* native_panel_;  

  AttentionMode attention_mode_;

  ExpansionState expansion_state_;

  
  CommandUpdater command_updater_;

  content::NotificationRegistrar registrar_;
  const SessionID session_id_;
  scoped_ptr<extensions::WindowController> extension_window_controller_;
  scoped_ptr<PanelHost> panel_host_;

  
  gfx::Image app_icon_;

  base::WeakPtrFactory<Panel> image_loader_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(Panel);
};

#endif  

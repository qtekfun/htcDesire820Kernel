// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_WEBUI_LOGIN_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_WEBUI_LOGIN_VIEW_H_

#include <map>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "chrome/browser/extensions/signin/scoped_gaia_auth_extension.h"
#include "chrome/browser/ui/chrome_web_modal_dialog_manager_delegate.h"
#include "components/web_modal/web_contents_modal_dialog_host.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_delegate.h"
#include "content/public/browser/web_contents_observer.h"
#include "ui/views/controls/webview/unhandled_keyboard_event_handler.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_delegate.h"

class GURL;

namespace content {
class WebUI;
}

namespace views {
class View;
class WebView;
class Widget;
}

namespace chromeos {

class WebUILoginView : public views::View,
                       public content::WebContentsDelegate,
                       public content::WebContentsObserver,
                       public content::NotificationObserver,
                       public ChromeWebModalDialogManagerDelegate,
                       public web_modal::WebContentsModalDialogHost {
 public:
  
  static const char kViewClassName[];

  WebUILoginView();
  virtual ~WebUILoginView();

  
  virtual void Init();

  
  virtual bool AcceleratorPressed(
      const ui::Accelerator& accelerator) OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;

  
  virtual web_modal::WebContentsModalDialogHost*
      GetWebContentsModalDialogHost() OVERRIDE;

  
  virtual gfx::NativeView GetHostView() const OVERRIDE;
  virtual gfx::Point GetDialogPosition(const gfx::Size& size) OVERRIDE;
  virtual gfx::Size GetMaximumDialogSize() OVERRIDE;
  virtual void AddObserver(
      web_modal::ModalDialogHostObserver* observer) OVERRIDE;
  virtual void RemoveObserver(
      web_modal::ModalDialogHostObserver* observer) OVERRIDE;

  
  gfx::NativeWindow GetNativeWindow() const;

  
  void LoadURL(const GURL& url);

  
  content::WebUI* GetWebUI();

  
  content::WebContents* GetWebContents();

  
  void OpenProxySettings();

  
  void OnPostponedShow();

  
  void SetStatusAreaVisible(bool visible);

  
  void SetUIEnabled(bool enabled);

  void set_is_hidden(bool hidden) { is_hidden_ = hidden; }

  bool webui_visible() const { return webui_visible_; }

  
  void set_should_emit_login_prompt_visible(bool emit) {
    should_emit_login_prompt_visible_ = emit;
  }

 protected:
  
  virtual void Layout() OVERRIDE;
  virtual void OnLocaleChanged() OVERRIDE;
  virtual void ChildPreferredSizeChanged(View* child) OVERRIDE;
  virtual void AboutToRequestFocusFromTabTraversal(bool reverse) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  views::WebView* webui_login_;

 private:
  
  typedef std::map<ui::Accelerator, std::string> AccelMap;

  
  virtual bool HandleContextMenu(
      const content::ContextMenuParams& params) OVERRIDE;
  virtual void HandleKeyboardEvent(
      content::WebContents* source,
      const content::NativeWebKeyboardEvent& event) OVERRIDE;
  virtual bool IsPopupOrPanel(
      const content::WebContents* source) const OVERRIDE;
  virtual bool TakeFocus(content::WebContents* source, bool reverse) OVERRIDE;
  virtual void RequestMediaAccessPermission(
      content::WebContents* web_contents,
      const content::MediaStreamRequest& request,
      const content::MediaResponseCallback& callback) OVERRIDE;

  
  virtual void DidFailProvisionalLoad(
      int64 frame_id,
      const base::string16& frame_unique_name,
      bool is_main_frame,
      const GURL& validated_url,
      int error_code,
      const base::string16& error_description,
      content::RenderViewHost* render_view_host) OVERRIDE;

  
  
  
  
  void OnLoginPromptVisible();

  
  
  void ReturnFocus(bool reverse);

  content::NotificationRegistrar registrar_;

  
  views::UnhandledKeyboardEventHandler unhandled_keyboard_event_handler_;

  
  AccelMap accel_map_;

  
  bool is_hidden_;

  
  bool webui_visible_;

  
  
  bool should_emit_login_prompt_visible_;

  
  bool forward_keyboard_event_;

  scoped_ptr<ScopedGaiaAuthExtension> auth_extension_;

  ObserverList<web_modal::ModalDialogHostObserver> observer_list_;

  DISALLOW_COPY_AND_ASSIGN(WebUILoginView);
};

}  

#endif  

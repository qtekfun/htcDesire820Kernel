// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_EXTERNAL_TAB_CONTAINER_WIN_H_
#define CHROME_BROWSER_UI_VIEWS_EXTERNAL_TAB_CONTAINER_WIN_H_

#include <map>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/lazy_instance.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/automation/automation_resource_message_filter.h"
#include "chrome/browser/external_tab/external_tab_container.h"
#include "chrome/browser/infobars/infobar_container.h"
#include "chrome/browser/net/chrome_url_request_context.h"
#include "content/public/browser/navigation_type.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_delegate.h"
#include "content/public/browser/web_contents_observer.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/views/controls/webview/unhandled_keyboard_event_handler.h"
#include "ui/views/widget/widget_observer.h"

class AutomationProvider;
class Browser;
class Profile;
class TabContentsContainer;
class RenderViewContextMenuViews;
struct NavigationInfo;

namespace ui {
class ViewProp;
}

namespace views {
class View;
class WebView;
class Widget;
}

#if defined(USE_AURA)
class ContainerWindow;
#endif

class ExternalTabContainerWin : public ExternalTabContainer,
                                public content::WebContentsDelegate,
                                public content::WebContentsObserver,
                                public content::NotificationObserver,
                                public views::WidgetObserver,
                                public ui::AcceleratorTarget,
                                public InfoBarContainer::Delegate {
 public:
  typedef std::map<uintptr_t,
                   scoped_refptr<ExternalTabContainerWin> > PendingTabs;

  ExternalTabContainerWin(AutomationProvider* automation,
                          AutomationResourceMessageFilter* filter);

  static scoped_refptr<ExternalTabContainer> RemovePendingExternalTab(
      uintptr_t cookie);

  
  virtual bool Init(Profile* profile,
                    HWND parent,
                    const gfx::Rect& bounds,
                    DWORD style,
                    bool load_requests_via_automation,
                    bool handle_top_level_requests,
                    content::WebContents* existing_contents,
                    const GURL& initial_url,
                    const GURL& referrer,
                    bool infobars_enabled,
                    bool supports_full_tab_mode) OVERRIDE;
  virtual void Uninitialize() OVERRIDE;
  virtual bool Reinitialize(AutomationProvider* automation_provider,
                            AutomationResourceMessageFilter* filter,
                            HWND parent_window) OVERRIDE;
  virtual content::WebContents* GetWebContents() const OVERRIDE;
  virtual HWND GetExternalTabHWND() const OVERRIDE;
  virtual HWND GetContentHWND() const OVERRIDE;
  virtual void SetTabHandle(int handle) OVERRIDE;
  virtual int GetTabHandle() const OVERRIDE;
  virtual bool ExecuteContextMenuCommand(int command) OVERRIDE;
  virtual void RunUnloadHandlers(IPC::Message* reply_message) OVERRIDE;
  virtual void ProcessUnhandledAccelerator(const MSG& msg) OVERRIDE;
  virtual void FocusThroughTabTraversal(bool reverse,
                                        bool restore_focus_to_view) OVERRIDE;

  
  virtual content::WebContents* OpenURLFromTab(
      content::WebContents* source,
      const content::OpenURLParams& params) OVERRIDE;
  virtual void NavigationStateChanged(const content::WebContents* source,
                                      unsigned changed_flags) OVERRIDE;
  virtual void AddNewContents(content::WebContents* source,
                              content::WebContents* new_contents,
                              WindowOpenDisposition disposition,
                              const gfx::Rect& initial_pos,
                              bool user_gesture,
                              bool* was_blocked) OVERRIDE;
  virtual void CloseContents(content::WebContents* source) OVERRIDE;
  virtual void MoveContents(content::WebContents* source,
                            const gfx::Rect& pos) OVERRIDE;
  virtual bool IsPopupOrPanel(
      const content::WebContents* source) const OVERRIDE;
  virtual void UpdateTargetURL(content::WebContents* source, int32 page_id,
                               const GURL& url) OVERRIDE;
  virtual void ContentsZoomChange(bool zoom_in) OVERRIDE;
  virtual void WebContentsCreated(content::WebContents* source_contents,
                                  int64 source_frame_id,
                                  const base::string16& frame_name,
                                  const GURL& target_url,
                                  content::WebContents* new_contents) OVERRIDE;
  virtual bool PreHandleKeyboardEvent(
      content::WebContents* source,
      const content::NativeWebKeyboardEvent& event,
      bool* is_keyboard_shortcut) OVERRIDE;
  virtual void HandleKeyboardEvent(
      content::WebContents* source,
      const content::NativeWebKeyboardEvent& event) OVERRIDE;
  virtual bool TakeFocus(content::WebContents* source, bool reverse) OVERRIDE;
  virtual void WebContentsFocused(content::WebContents* contents) OVERRIDE;
  virtual void CanDownload(content::RenderViewHost* render_view_host,
                           int request_id,
                           const std::string& request_method,
                           const base::Callback<void(bool)>& callback) OVERRIDE;
  virtual bool OnGoToEntryOffset(int offset) OVERRIDE;
  virtual bool HandleContextMenu(
      const content::ContextMenuParams& params) OVERRIDE;
  virtual void BeforeUnloadFired(content::WebContents* tab,
                                 bool proceed,
                                 bool* proceed_to_fire_unload) OVERRIDE;
  virtual content::JavaScriptDialogManager*
      GetJavaScriptDialogManager() OVERRIDE;
  virtual void ShowRepostFormWarningDialog(
      content::WebContents* source) OVERRIDE;
  virtual content::ColorChooser* OpenColorChooser(
      content::WebContents* web_contents,
      SkColor color,
      const std::vector<content::ColorSuggestion>& suggestions) OVERRIDE;
  virtual void RunFileChooser(
      content::WebContents* tab,
      const content::FileChooserParams& params) OVERRIDE;
  virtual void EnumerateDirectory(content::WebContents* tab,
                                  int request_id,
                                  const base::FilePath& path) OVERRIDE;
  virtual void RequestMediaAccessPermission(
      content::WebContents* web_contents,
      const content::MediaStreamRequest& request,
      const content::MediaResponseCallback& callback) OVERRIDE;
  virtual bool RequestPpapiBrokerPermission(
      content::WebContents* web_contents,
      const GURL& url,
      const base::FilePath& plugin_path,
      const base::Callback<void(bool)>& callback) OVERRIDE;

  void RegisterRenderViewHost(content::RenderViewHost* render_view_host);
  void UnregisterRenderViewHost(content::RenderViewHost* render_view_host);

  
  virtual void RenderViewDeleted(
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void DidFailProvisionalLoad(
      int64 frame_id,
      const base::string16& frame_unique_name,
      bool is_main_frame,
      const GURL& validated_url,
      int error_code,
      const base::string16& error_description,
      content::RenderViewHost* render_view_host) OVERRIDE;

  
  void OnForwardMessageToExternalHost(const std::string& message,
                                      const std::string& origin,
                                      const std::string& target);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details);

  
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;
  virtual bool CanHandleAccelerators() const OVERRIDE;

  void set_pending(bool pending) { pending_ = pending; }
  bool pending() const { return pending_; }

  void set_is_popup_window(bool is_popup_window) {
    is_popup_window_ = is_popup_window;
  }

  
  virtual SkColor GetInfoBarSeparatorColor() const OVERRIDE;
  virtual void InfoBarContainerStateChanged(bool is_animating) OVERRIDE;
  virtual bool DrawInfoBarArrows(int* x) const OVERRIDE;

 protected:
  virtual ~ExternalTabContainerWin();

  
  virtual void OnWidgetCreated(views::Widget* widget) OVERRIDE;
  virtual void OnWidgetDestroying(views::Widget* widget) OVERRIDE;
  virtual void OnWidgetDestroyed(views::Widget* widget) OVERRIDE;

  bool InitNavigationInfo(NavigationInfo* nav_info,
                          content::NavigationType nav_type,
                          int relative_offset);
  void Navigate(const GURL& url, const GURL& referrer);

  
  
  void RegisterRenderViewHostForAutomation(
      bool pending_view,
      content::RenderViewHost* render_view_host);

  
  
  bool ProcessUnhandledKeyStroke(HWND window, UINT message, WPARAM wparam,
                                 LPARAM lparam);

  void LoadAccelerators();

  
  void ServicePendingOpenURLRequests();

  
  void OnReinitialize();

  
  
  void SetupExternalTabView();

  views::Widget* widget_;
  scoped_ptr<content::WebContents> web_contents_;
  scoped_refptr<AutomationProvider> automation_;

  content::NotificationRegistrar registrar_;

  
  views::WebView* tab_contents_container_;

  int tab_handle_;
  
  
  
  
  bool ignore_next_load_notification_;

  scoped_ptr<RenderViewContextMenuViews> external_context_menu_;

  
  scoped_refptr<AutomationResourceMessageFilter>
      automation_resource_message_filter_;

  
  bool load_requests_via_automation_;

  
  bool handle_top_level_requests_;

  
  
  
  bool route_all_top_level_navigations_;

  
  static base::LazyInstance<PendingTabs> pending_tabs_;

  
  
  base::WeakPtrFactory<ExternalTabContainerWin> weak_factory_;

  
  scoped_refptr<ChromeURLRequestContextGetter> request_context_;

  views::UnhandledKeyboardEventHandler unhandled_keyboard_event_handler_;

  
  std::map<ui::Accelerator, int> accelerator_table_;

  
  
  
  
  
  
  
  
  
  
  
  std::vector<content::OpenURLParams> pending_open_url_requests_;

  
  
  bool pending_;

  views::FocusManager* focus_manager_;

  views::View* external_tab_view_;

  IPC::Message* unload_reply_message_;

  scoped_ptr<ui::ViewProp> prop_;

  
  bool is_popup_window_;

#if defined(USE_AURA)
  base::WeakPtr<ContainerWindow> tab_container_window_;
#endif

  DISALLOW_COPY_AND_ASSIGN(ExternalTabContainerWin);
};

class TemporaryPopupExternalTabContainerWin : public ExternalTabContainerWin {
 public:
  TemporaryPopupExternalTabContainerWin(
      AutomationProvider* automation,
      AutomationResourceMessageFilter* filter);
  virtual ~TemporaryPopupExternalTabContainerWin();

  virtual bool OnGoToEntryOffset(int offset) {
    NOTREACHED();
    return false;
  }

  virtual bool ProcessUnhandledKeyStroke(HWND window, UINT message,
                                         WPARAM wparam, LPARAM lparam) {
    NOTREACHED();
    return false;
  }

  virtual void Observe(int type, const content::NotificationSource& source,
                       const content::NotificationDetails& details) {}

  virtual content::WebContents* OpenURLFromTab(
    content::WebContents* source,
    const content::OpenURLParams& params) OVERRIDE;

  virtual void NavigationStateChanged(const content::WebContents* source,
                                      unsigned changed_flags) {
    NOTREACHED();
  }

  virtual void CloseContents(content::WebContents* source) {
    NOTREACHED();
  }

  virtual void UpdateTargetURL(content::WebContents* source, int32 page_id,
                               const GURL& url) {
    NOTREACHED();
  }

  void ForwardMessageToExternalHost(const std::string& message,
                                    const std::string& origin,
                                    const std::string& target) {
    NOTREACHED();
  }

  virtual bool TakeFocus(bool reverse) {
    NOTREACHED();
    return false;
  }

  virtual bool HandleContextMenu(const content::ContextMenuParams& params) {
    NOTREACHED();
    return false;
  }

  virtual void BeforeUnloadFired(content::WebContents* tab, bool proceed,
                                 bool* proceed_to_fire_unload) {
    NOTREACHED();
  }
};

#endif  

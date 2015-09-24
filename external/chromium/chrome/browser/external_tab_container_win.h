// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTERNAL_TAB_CONTAINER_WIN_H_
#define CHROME_BROWSER_EXTERNAL_TAB_CONTAINER_WIN_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/lazy_instance.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/automation/automation_resource_message_filter.h"
#include "chrome/browser/net/chrome_url_request_context.h"
#include "chrome/browser/ui/views/frame/browser_bubble_host.h"
#include "chrome/browser/ui/views/infobars/infobar_container.h"
#include "chrome/browser/ui/views/unhandled_keyboard_event_handler.h"
#include "content/browser/tab_contents/tab_contents_delegate.h"
#include "content/common/navigation_types.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "views/accelerator.h"
#include "views/widget/widget_win.h"

class AutomationProvider;
class Browser;
class Profile;
class TabContentsContainer;
class TabContentsWrapper;
class RenderViewContextMenuViews;
struct NavigationInfo;

namespace ui {
class ViewProp;
}

class ExternalTabContainer : public TabContentsDelegate,
                             public NotificationObserver,
                             public views::WidgetWin,
                             public base::RefCounted<ExternalTabContainer>,
                             public views::AcceleratorTarget,
                             public InfoBarContainer::Delegate,
                             public BrowserBubbleHost {
 public:
  typedef std::map<uintptr_t, scoped_refptr<ExternalTabContainer> > PendingTabs;

  ExternalTabContainer(AutomationProvider* automation,
      AutomationResourceMessageFilter* filter);

  TabContents* tab_contents() const;

  
  void SetTabHandle(int handle);

  int tab_handle() const {
    return tab_handle_;
  }

  bool Init(Profile* profile,
            HWND parent,
            const gfx::Rect& bounds,
            DWORD style,
            bool load_requests_via_automation,
            bool handle_top_level_requests,
            TabContentsWrapper* existing_tab_contents,
            const GURL& initial_url,
            const GURL& referrer,
            bool infobars_enabled,
            bool supports_full_tab_mode);

  
  
  
  
  
  void Uninitialize();

  
  
  
  bool Reinitialize(AutomationProvider* automation_provider,
                    AutomationResourceMessageFilter* filter,
                    gfx::NativeWindow parent_window);

  
  
  void ProcessUnhandledAccelerator(const MSG& msg);

  
  void FocusThroughTabTraversal(bool reverse, bool restore_focus_to_view);

  
  
  static bool IsExternalTabContainer(HWND window);

  
  
  
  static ExternalTabContainer* GetExternalContainerFromNativeWindow(
      gfx::NativeView native_window);

  
  
  static ExternalTabContainer* GetContainerForTab(HWND tab_window);

  
  virtual void OpenURLFromTab(TabContents* source,
                              const GURL& url,
                              const GURL& referrer,
                              WindowOpenDisposition disposition,
                              PageTransition::Type transition);
  virtual void NavigationStateChanged(const TabContents* source,
                                      unsigned changed_flags);
  virtual void AddNewContents(TabContents* source,
                              TabContents* new_contents,
                              WindowOpenDisposition disposition,
                              const gfx::Rect& initial_pos,
                              bool user_gesture);
  virtual void ActivateContents(TabContents* contents);
  virtual void DeactivateContents(TabContents* contents);
  virtual void LoadingStateChanged(TabContents* source);
  virtual void CloseContents(TabContents* source);
  virtual void MoveContents(TabContents* source, const gfx::Rect& pos);
  virtual bool IsPopup(const TabContents* source) const;
  virtual void UpdateTargetURL(TabContents* source, const GURL& url);
  virtual void ContentsZoomChange(bool zoom_in);
  virtual void ForwardMessageToExternalHost(const std::string& message,
                                            const std::string& origin,
                                            const std::string& target);
  virtual bool IsExternalTabContainer() const;
  virtual gfx::NativeWindow GetFrameNativeWindow();

  virtual bool PreHandleKeyboardEvent(const NativeWebKeyboardEvent& event,
                                      bool* is_keyboard_shortcut);
  virtual void HandleKeyboardEvent(const NativeWebKeyboardEvent& event);

  virtual bool TakeFocus(bool reverse);

  virtual bool CanDownload(int request_id);

  virtual bool OnGoToEntryOffset(int offset);

  virtual void ShowPageInfo(Profile* profile,
                            const GURL& url,
                            const NavigationEntry::SSLStatus& ssl,
                            bool show_history);

  
  
  virtual bool HandleContextMenu(const ContextMenuParams& params);

  
  
  virtual bool ExecuteContextMenuCommand(int command);

  
  
  
  
  virtual void ShowHtmlDialog(HtmlDialogUIDelegate* delegate,
                              gfx::NativeWindow parent_window);

  virtual void BeforeUnloadFired(TabContents* tab,
                                 bool proceed,
                                 bool* proceed_to_fire_unload);

  void ShowRepostFormWarningDialog(TabContents* tab_contents);

  
  virtual void RegisterRenderViewHost(RenderViewHost* render_view_host);
  virtual void UnregisterRenderViewHost(RenderViewHost* render_view_host);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  
  static scoped_refptr<ExternalTabContainer> RemovePendingTab(uintptr_t cookie);

  
  virtual views::Window* GetWindow();

  
  bool AcceleratorPressed(const views::Accelerator& accelerator);

  bool pending() const {
    return pending_;
  }

  void set_pending(bool pending) {
    pending_ = pending;
  }

  void set_is_popup_window(bool is_popup_window) {
    is_popup_window_ = is_popup_window;
  }

  
  virtual SkColor GetInfoBarSeparatorColor() const OVERRIDE;
  virtual void InfoBarContainerStateChanged(bool is_animating) OVERRIDE;
  virtual bool DrawInfoBarArrows(int* x) const OVERRIDE;

  virtual void TabContentsCreated(TabContents* new_contents);

  virtual bool infobars_enabled();

  void RunUnloadHandlers(IPC::Message* reply_message);

 protected:
  ~ExternalTabContainer();
  
  virtual LRESULT OnCreate(LPCREATESTRUCT create_struct);
  virtual void OnDestroy();
  virtual void OnFinalMessage(HWND window);

  bool InitNavigationInfo(NavigationInfo* nav_info,
                          NavigationType::Type nav_type,
                          int relative_offset);
  void Navigate(const GURL& url, const GURL& referrer);

  friend class base::RefCounted<ExternalTabContainer>;

  
  
  void RegisterRenderViewHostForAutomation(RenderViewHost* render_view_host,
                                           bool pending_view);

  
  
  
  
  
  
  
  
  
  struct PendingTopLevelNavigation {
    GURL url;
    GURL referrer;
    WindowOpenDisposition disposition;
    PageTransition::Type transition;
  };

  
  
  bool ProcessUnhandledKeyStroke(HWND window, UINT message, WPARAM wparam,
                                 LPARAM lparam);

  void LoadAccelerators();

  
  void ServicePendingOpenURLRequests();

  
  void OnReinitialize();

  
  void SetupExternalTabView();

  scoped_ptr<TabContentsWrapper> tab_contents_;
  scoped_refptr<AutomationProvider> automation_;

  NotificationRegistrar registrar_;

  
  TabContentsContainer* tab_contents_container_;

  int tab_handle_;
  
  
  
  
  bool ignore_next_load_notification_;

  scoped_ptr<RenderViewContextMenuViews> external_context_menu_;

  
  scoped_refptr<AutomationResourceMessageFilter>
      automation_resource_message_filter_;

  
  bool load_requests_via_automation_;

  
  bool handle_top_level_requests_;

  
  scoped_ptr<Browser> browser_;

  
  static base::LazyInstance<PendingTabs> pending_tabs_;

  
  
  ScopedRunnableMethodFactory<ExternalTabContainer> external_method_factory_;

  
  scoped_refptr<ChromeURLRequestContextGetter> request_context_;

  UnhandledKeyboardEventHandler unhandled_keyboard_event_handler_;

  
  std::map<views::Accelerator, int> accelerator_table_;

  
  
  std::vector<PendingTopLevelNavigation> pending_open_url_requests_;

  
  
  bool pending_;

  
  bool infobars_enabled_;

  views::FocusManager* focus_manager_;

  views::View* external_tab_view_;

  IPC::Message* unload_reply_message_;

  
  
  
  bool route_all_top_level_navigations_;

  scoped_ptr<ui::ViewProp> prop_;

  
  bool is_popup_window_;

  DISALLOW_COPY_AND_ASSIGN(ExternalTabContainer);
};

class TemporaryPopupExternalTabContainer : public ExternalTabContainer {
 public:
  TemporaryPopupExternalTabContainer(AutomationProvider* automation,
      AutomationResourceMessageFilter* filter);
  virtual ~TemporaryPopupExternalTabContainer();

  virtual bool OnGoToEntryOffset(int offset) {
    NOTREACHED();
    return false;
  }

  virtual bool ProcessUnhandledKeyStroke(HWND window, UINT message,
                                         WPARAM wparam, LPARAM lparam) {
    NOTREACHED();
    return false;
  }

  virtual void Observe(NotificationType type, const NotificationSource& source,
                       const NotificationDetails& details) {}

  virtual void OpenURLFromTab(TabContents* source, const GURL& url,
                              const GURL& referrer,
                              WindowOpenDisposition disposition,
                              PageTransition::Type transition);

  virtual void NavigationStateChanged(const TabContents* source,
                                      unsigned changed_flags) {
    NOTREACHED();
  }

  virtual void CloseContents(TabContents* source) {
    NOTREACHED();
  }

  virtual void UpdateTargetURL(TabContents* source, const GURL& url) {
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

  virtual bool HandleContextMenu(const ContextMenuParams& params) {
    NOTREACHED();
    return false;
  }

  virtual void BeforeUnloadFired(TabContents* tab, bool proceed,
                                 bool* proceed_to_fire_unload) {
    NOTREACHED();
  }
};

#endif  

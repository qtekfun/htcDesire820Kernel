// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_FRAME_HOST_INTERSTITIAL_PAGE_IMPL_H_
#define CONTENT_BROWSER_FRAME_HOST_INTERSTITIAL_PAGE_IMPL_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "content/browser/frame_host/frame_tree.h"
#include "content/browser/frame_host/navigator_delegate.h"
#include "content/browser/frame_host/render_frame_host_delegate.h"
#include "content/browser/renderer_host/render_view_host_delegate.h"
#include "content/browser/renderer_host/render_widget_host_delegate.h"
#include "content/public/browser/interstitial_page.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/common/renderer_preferences.h"
#include "url/gurl.h"

namespace content {
class NavigationEntry;
class NavigationControllerImpl;
class RenderViewHostImpl;
class RenderWidgetHostView;
class WebContentsView;
class WebContentsImpl;

enum ResourceRequestAction {
  BLOCK,
  RESUME,
  CANCEL
};

class CONTENT_EXPORT InterstitialPageImpl
    : public NON_EXPORTED_BASE(InterstitialPage),
      public NotificationObserver,
      public WebContentsObserver,
      public NON_EXPORTED_BASE(RenderFrameHostDelegate),
      public RenderViewHostDelegate,
      public RenderWidgetHostDelegate,
      public NON_EXPORTED_BASE(NavigatorDelegate) {
 public:
  
  enum ActionState {
    NO_ACTION,           
    PROCEED_ACTION,      
    DONT_PROCEED_ACTION  
  };

  InterstitialPageImpl(WebContents* web_contents,
                       RenderWidgetHostDelegate* render_widget_host_delegate,
                       bool new_navigation,
                       const GURL& url,
                       InterstitialPageDelegate* delegate);
  virtual ~InterstitialPageImpl();

  
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void DontProceed() OVERRIDE;
  virtual void Proceed() OVERRIDE;
  virtual RenderViewHost* GetRenderViewHostForTesting() const OVERRIDE;
  virtual InterstitialPageDelegate* GetDelegateForTesting() OVERRIDE;
  virtual void DontCreateViewForTesting() OVERRIDE;
  virtual void SetSize(const gfx::Size& size) OVERRIDE;
  virtual void Focus() OVERRIDE;

  
  
  
  void CancelForNavigation();

  
  
  void FocusThroughTabTraversal(bool reverse);

  RenderWidgetHostView* GetView();

  
  void set_reload_on_dont_proceed(bool value) {
    reload_on_dont_proceed_ = value;
  }
  bool reload_on_dont_proceed() const { return reload_on_dont_proceed_; }

#if defined(OS_ANDROID)
  
  
  RenderViewHost* GetRenderViewHost() const;
#endif

 protected:
  
  virtual void Observe(int type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  
  virtual void WebContentsDestroyed(WebContents* web_contents) OVERRIDE;
  virtual void NavigationEntryCommitted(
      const LoadCommittedDetails& load_details) OVERRIDE;

  

  
  virtual RenderViewHostDelegateView* GetDelegateView() OVERRIDE;
  virtual const GURL& GetURL() const OVERRIDE;
  virtual void RenderViewTerminated(RenderViewHost* render_view_host,
                                    base::TerminationStatus status,
                                    int error_code) OVERRIDE;
  virtual void DidNavigate(
      RenderViewHost* render_view_host,
      const ViewHostMsg_FrameNavigate_Params& params) OVERRIDE;
  virtual void UpdateTitle(RenderViewHost* render_view_host,
                           int32 page_id,
                           const base::string16& title,
                           base::i18n::TextDirection title_direction) OVERRIDE;
  virtual RendererPreferences GetRendererPrefs(
      BrowserContext* browser_context) const OVERRIDE;
  virtual WebPreferences GetWebkitPrefs() OVERRIDE;
  virtual gfx::Rect GetRootWindowResizerRect() const OVERRIDE;
  virtual void CreateNewWindow(
      int render_process_id,
      int route_id,
      int main_frame_route_id,
      const ViewHostMsg_CreateWindow_Params& params,
      SessionStorageNamespace* session_storage_namespace) OVERRIDE;
  virtual void CreateNewWidget(int render_process_id,
                               int route_id,
                               blink::WebPopupType popup_type) OVERRIDE;
  virtual void CreateNewFullscreenWidget(int render_process_id,
                                         int route_id) OVERRIDE;
  virtual void ShowCreatedWindow(int route_id,
                                 WindowOpenDisposition disposition,
                                 const gfx::Rect& initial_pos,
                                 bool user_gesture) OVERRIDE;
  virtual void ShowCreatedWidget(int route_id,
                                 const gfx::Rect& initial_pos) OVERRIDE;
  virtual void ShowCreatedFullscreenWidget(int route_id) OVERRIDE;

  virtual SessionStorageNamespace* GetSessionStorageNamespace(
      SiteInstance* instance) OVERRIDE;

  virtual FrameTree* GetFrameTree() OVERRIDE;

  
  virtual void RenderWidgetDeleted(
      RenderWidgetHostImpl* render_widget_host) OVERRIDE;
  virtual bool PreHandleKeyboardEvent(
      const NativeWebKeyboardEvent& event,
      bool* is_keyboard_shortcut) OVERRIDE;
  virtual void HandleKeyboardEvent(
      const NativeWebKeyboardEvent& event) OVERRIDE;
#if defined(OS_WIN) && defined(USE_AURA)
  virtual gfx::NativeViewAccessible GetParentNativeViewAccessible() OVERRIDE;
#endif

  bool enabled() const { return enabled_; }
  WebContents* web_contents() const;
  const GURL& url() const { return url_; }

  
  
  virtual RenderViewHost* CreateRenderViewHost();

  
  
  virtual WebContentsView* CreateWebContentsView();

  
  NotificationRegistrar notification_registrar_;

 private:
  class InterstitialPageRVHDelegateView;

  
  
  
  void Disable();

  
  void Shutdown(RenderViewHostImpl* render_view_host);

  void OnNavigatingAwayOrTabClosing();

  
  
  
  void TakeActionOnResourceDispatcher(ResourceRequestAction action);

  
  
  
  WebContents* web_contents_;

  
  NavigationControllerImpl* controller_;

  
  RenderWidgetHostDelegate* render_widget_host_delegate_;

  
  GURL url_;

  
  
  bool new_navigation_;

  
  
  
  
  bool should_discard_pending_nav_entry_;

  
  
  
  
  bool reload_on_dont_proceed_;

  
  bool enabled_;

  
  ActionState action_taken_;

  
  
  
  RenderViewHostImpl* render_view_host_;

  
  FrameTree frame_tree_;

  
  int original_child_id_;
  int original_rvh_id_;

  
  
  bool should_revert_web_contents_title_;

  
  
  bool web_contents_was_loading_;

  
  
  bool resource_dispatcher_host_notified_;

  
  
  base::string16 original_web_contents_title_;

  
  scoped_ptr<InterstitialPageRVHDelegateView> rvh_delegate_view_;

  
  mutable RendererPreferences renderer_preferences_;

  bool create_view_;

  scoped_ptr<InterstitialPageDelegate> delegate_;

  base::WeakPtrFactory<InterstitialPageImpl> weak_ptr_factory_;

  scoped_refptr<SessionStorageNamespace> session_storage_namespace_;

  DISALLOW_COPY_AND_ASSIGN(InterstitialPageImpl);
};

}  

#endif  

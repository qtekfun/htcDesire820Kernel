// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_FRAME_HOST_RENDER_FRAME_HOST_MANAGER_H_
#define CONTENT_BROWSER_FRAME_HOST_RENDER_FRAME_HOST_MANAGER_H_

#include "base/basictypes.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "content/browser/renderer_host/render_view_host_delegate.h"
#include "content/browser/site_instance_impl.h"
#include "content/common/content_export.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/common/referrer.h"


namespace content {
class BrowserContext;
class InterstitialPageImpl;
class NavigationControllerImpl;
class NavigationEntry;
class NavigationEntryImpl;
class RenderFrameHostDelegate;
class RenderFrameHostManagerTest;
class RenderViewHost;
class RenderViewHostImpl;
class RenderWidgetHostDelegate;
class RenderWidgetHostView;
class TestWebContents;
class WebUIImpl;

class CONTENT_EXPORT RenderFrameHostManager
    : public RenderViewHostDelegate::RendererManagement,
      public NotificationObserver {
 public:
  
  
  
  
  
  
  
  
  
  class CONTENT_EXPORT Delegate {
   public:
    
    
    
    
    
    
    
    
    virtual bool CreateRenderViewForRenderManager(
        RenderViewHost* render_view_host, int opener_route_id) = 0;
    virtual void BeforeUnloadFiredFromRenderManager(
        bool proceed, const base::TimeTicks& proceed_time,
        bool* proceed_to_fire_unload) = 0;
    virtual void RenderProcessGoneFromRenderManager(
        RenderViewHost* render_view_host) = 0;
    virtual void UpdateRenderViewSizeForRenderManager() = 0;
    virtual void CancelModalDialogsForRenderManager() = 0;
    virtual void NotifySwappedFromRenderManager(
        RenderViewHost* old_host, RenderViewHost* new_host) = 0;
    virtual NavigationControllerImpl&
        GetControllerForRenderManager() = 0;

    
    
    
    
    virtual int CreateOpenerRenderViewsForRenderManager(
        SiteInstance* instance) = 0;

    
    
    
    virtual WebUIImpl* CreateWebUIForRenderManager(const GURL& url) = 0;

    
    
    virtual NavigationEntry*
        GetLastCommittedNavigationEntryForRenderManager() = 0;

    
    
    
    virtual bool FocusLocationBarByDefault() = 0;

    
    virtual void SetFocusToLocationBar(bool select_all) = 0;

    
    virtual void CreateViewAndSetSizeForRVH(RenderViewHost* rvh) = 0;

    
    
    virtual bool IsHidden() = 0;

   protected:
    virtual ~Delegate() {}
  };

  
  
  
  
  
  
  RenderFrameHostManager(
      RenderFrameHostDelegate* render_frame_delegate,
      RenderViewHostDelegate* render_view_delegate,
      RenderWidgetHostDelegate* render_widget_delegate,
      Delegate* delegate);
  virtual ~RenderFrameHostManager();

  
  void Init(BrowserContext* browser_context,
            SiteInstance* site_instance,
            int routing_id,
            int main_frame_routing_id);

  
  
  
  
  
  RenderViewHostImpl* current_host() const;

  
  
  RenderWidgetHostView* GetRenderWidgetHostView() const;

  
  RenderViewHostImpl* pending_render_view_host() const;

  
  WebUIImpl* web_ui() const { return web_ui_.get(); }

  
  WebUIImpl* pending_web_ui() const {
    return pending_web_ui_.get() ? pending_web_ui_.get() :
                                   pending_and_current_web_ui_.get();
  }

  
  
  void SetPendingWebUI(const NavigationEntryImpl& entry);

  
  
  
  
  RenderViewHostImpl* Navigate(const NavigationEntryImpl& entry);

  
  
  void Stop();

  
  
  
  void SetIsLoading(bool is_loading);

  
  
  
  bool ShouldCloseTabOnUnresponsiveRenderer();

  
  
  void SwappedOut(RenderViewHost* render_view_host);

  
  void DidNavigateMainFrame(RenderViewHost* render_view_host);

  
  void DidDisownOpener(RenderViewHost* render_view_host);

  
  
  
  int CreateRenderView(SiteInstance* instance,
                       int opener_route_id,
                       bool swapped_out,
                       bool hidden);

  
  void RendererAbortedProvisionalLoad(RenderViewHost* render_view_host);

  
  
  
  
  void set_interstitial_page(InterstitialPageImpl* interstitial_page) {
    DCHECK(!interstitial_page_ && interstitial_page);
    interstitial_page_ = interstitial_page;
  }

  
  void remove_interstitial_page() {
    DCHECK(interstitial_page_);
    interstitial_page_ = NULL;
  }

  
  
  InterstitialPageImpl* interstitial_page() const { return interstitial_page_; }

  
  virtual void ShouldClosePage(
      bool for_cross_site_transition,
      bool proceed,
      const base::TimeTicks& proceed_time) OVERRIDE;
  virtual void OnCrossSiteResponse(
      RenderViewHost* pending_render_view_host,
      const GlobalRequestID& global_request_id,
      bool is_transfer,
      const std::vector<GURL>& transfer_url_chain,
      const Referrer& referrer,
      PageTransition page_transition,
      int64 frame_id,
      bool should_replace_current_entry) OVERRIDE;

  
  virtual void Observe(int type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  
  void RenderViewDeleted(RenderViewHost* rvh);

  
  
  bool IsOnSwappedOutList(RenderViewHost* rvh) const;

  
  RenderViewHostImpl* GetSwappedOutRenderViewHost(SiteInstance* instance);

  
  
  
  void SwapOutOldPage();

 private:
  friend class RenderFrameHostManagerTest;
  friend class TestWebContents;

  
  
  struct PendingNavigationParams {
    PendingNavigationParams();
    PendingNavigationParams(const GlobalRequestID& global_request_id,
                            bool is_transfer,
                            const std::vector<GURL>& transfer_url,
                            Referrer referrer,
                            PageTransition page_transition,
                            int64 frame_id,
                            bool should_replace_current_entry);
    ~PendingNavigationParams();

    
    
    GlobalRequestID global_request_id;

    
    
    
    bool is_transfer;

    
    
    
    std::vector<GURL> transfer_url_chain;

    
    
    Referrer referrer;

    
    
    PageTransition page_transition;

    
    int64 frame_id;

    
    
    bool should_replace_current_entry;
  };

  
  
  
  bool ShouldTransitionCrossSite();

  
  
  
  
  
  
  bool ShouldSwapBrowsingInstancesForNavigation(
      const NavigationEntry* current_entry,
      const NavigationEntryImpl* new_entry) const;

  
  
  bool ShouldReuseWebUI(
      const NavigationEntry* current_entry,
      const NavigationEntryImpl* new_entry) const;

  
  
  
  
  SiteInstance* GetSiteInstanceForEntry(
      const NavigationEntryImpl& entry,
      SiteInstance* current_instance,
      bool force_browsing_instance_swap);

  
  bool InitRenderView(RenderViewHost* render_view_host, int opener_route_id);

  
  
  
  void CommitPending();

  
  
  
  void ShutdownRenderViewHostsInSiteInstance(int32 site_instance_id);

  
  void CancelPending();

  RenderViewHostImpl* UpdateRendererStateForNavigate(
      const NavigationEntryImpl& entry);

  
  
  void RendererProcessClosing(RenderProcessHost* render_process_host);

  
  Delegate* delegate_;

  
  
  
  bool cross_navigation_pending_;

  
  
  RenderFrameHostDelegate* render_frame_delegate_;
  RenderViewHostDelegate* render_view_delegate_;
  RenderWidgetHostDelegate* render_widget_delegate_;

  
  
  
  RenderViewHostImpl* render_view_host_;
  scoped_ptr<WebUIImpl> web_ui_;

  
  
  
  
  
  
  
  
  
  
  RenderViewHostImpl* pending_render_view_host_;

  
  scoped_ptr<PendingNavigationParams> pending_nav_params_;

  
  
  
  
  scoped_ptr<WebUIImpl> pending_web_ui_;
  base::WeakPtr<WebUIImpl> pending_and_current_web_ui_;

  
  
  typedef base::hash_map<int32, RenderViewHostImpl*> RenderViewHostMap;
  RenderViewHostMap swapped_out_hosts_;

  
  
  InterstitialPageImpl* interstitial_page_;

  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(RenderFrameHostManager);
};

}  

#endif  

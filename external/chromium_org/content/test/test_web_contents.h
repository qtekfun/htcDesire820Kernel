// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_TEST_WEB_CONTENTS_H_
#define CONTENT_TEST_TEST_WEB_CONTENTS_H_

#include "content/browser/web_contents/web_contents_impl.h"
#include "content/public/common/page_transition_types.h"
#include "content/public/test/web_contents_tester.h"
#include "webkit/common/webpreferences.h"

class SiteInstanceImpl;

namespace content {

class RenderViewHost;
class TestRenderViewHost;
class WebContentsTester;

class TestWebContents : public WebContentsImpl, public WebContentsTester {
 public:
  virtual ~TestWebContents();

  static TestWebContents* Create(BrowserContext* browser_context,
                                 SiteInstance* instance);

  
  virtual void CommitPendingNavigation() OVERRIDE;
  virtual RenderViewHost* GetPendingRenderViewHost() const OVERRIDE;
  virtual void NavigateAndCommit(const GURL& url) OVERRIDE;
  virtual void TestSetIsLoading(bool value) OVERRIDE;
  virtual void ProceedWithCrossSiteNavigation() OVERRIDE;
  virtual void TestDidNavigate(RenderViewHost* render_view_host,
                               int page_id,
                               const GURL& url,
                               PageTransition transition) OVERRIDE;
  virtual void TestDidNavigateWithReferrer(RenderViewHost* render_view_host,
                                           int page_id,
                                           const GURL& url,
                                           const Referrer& referrer,
                                           PageTransition transition) OVERRIDE;
  virtual WebPreferences TestGetWebkitPrefs() OVERRIDE;

  TestRenderViewHost* pending_test_rvh() const;

  
  bool cross_navigation_pending() {
    return GetRenderManager()->cross_navigation_pending_;
  }

  
  
  bool ShouldTransitionCrossSite() { return transition_cross_site; }

  
  virtual bool CreateRenderViewForRenderManager(
      RenderViewHost* render_view_host, int opener_route_id) OVERRIDE;
  virtual void UpdateRenderViewSizeForRenderManager() OVERRIDE {}

  
  
  virtual WebContents* Clone() OVERRIDE;

  
  bool transition_cross_site;

  
  virtual RenderViewHostDelegateView* GetDelegateView() OVERRIDE;
  void set_delegate_view(RenderViewHostDelegateView* view) {
    delegate_view_override_ = view;
  }

  
  void SetOpener(TestWebContents* opener);

  
  void AddPendingContents(TestWebContents* contents);

  
  
  
  void ExpectSetHistoryLengthAndPrune(const SiteInstance* site_instance,
                                      int history_length,
                                      int32 min_page_id);

  
  
  virtual void SetHistoryLengthAndPrune(const SiteInstance* site_instance,
                                        int history_length,
                                        int32 min_page_id) OVERRIDE;

  void TestDidFinishLoad(int64 frame_id, const GURL& url, bool is_main_frame);
  void TestDidFailLoadWithError(int64 frame_id,
                                const GURL& url,
                                bool is_main_frame,
                                int error_code,
                                const base::string16& error_description);

 protected:
  
  explicit TestWebContents(BrowserContext* browser_context);

 private:
  
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


  RenderViewHostDelegateView* delegate_view_override_;

  
  bool expect_set_history_length_and_prune_;
  scoped_refptr<const SiteInstanceImpl>
    expect_set_history_length_and_prune_site_instance_;
  int expect_set_history_length_and_prune_history_length_;
  int32 expect_set_history_length_and_prune_min_page_id_;
};

}  

#endif  

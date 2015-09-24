// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CAPTIVE_PORTAL_CAPTIVE_PORTAL_TAB_HELPER_H_
#define CHROME_BROWSER_CAPTIVE_PORTAL_CAPTIVE_PORTAL_TAB_HELPER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/captive_portal/captive_portal_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"
#include "webkit/common/resource_type.h"

class GURL;
class Profile;

namespace content {
  class WebContents;
}

namespace net {
class SSLInfo;
}

namespace captive_portal {

class CaptivePortalLoginDetector;
class CaptivePortalTabReloader;

class CaptivePortalTabHelper
    : public content::WebContentsObserver,
      public content::NotificationObserver,
      public base::NonThreadSafe,
      public content::WebContentsUserData<CaptivePortalTabHelper> {
 public:
  virtual ~CaptivePortalTabHelper();

  
  virtual void RenderViewDeleted(
      content::RenderViewHost* render_view_host) OVERRIDE;

  virtual void DidStartProvisionalLoadForFrame(
      int64 frame_id,
      int64 parent_frame_id,
      bool is_main_frame,
      const GURL& validated_url,
      bool is_error_page,
      bool is_iframe_srcdoc,
      content::RenderViewHost* render_view_host) OVERRIDE;

  virtual void DidCommitProvisionalLoadForFrame(
      int64 frame_id,
      const base::string16& frame_unique_name,
      bool is_main_frame,
      const GURL& url,
      content::PageTransition transition_type,
      content::RenderViewHost* render_view_host) OVERRIDE;

  virtual void DidFailProvisionalLoad(
      int64 frame_id,
      const base::string16& frame_unique_name,
      bool is_main_frame,
      const GURL& validated_url,
      int error_code,
      const base::string16& error_description,
      content::RenderViewHost* render_view_host) OVERRIDE;

  virtual void DidStopLoading(
      content::RenderViewHost* render_view_host) OVERRIDE;

  
  virtual void Observe(
      int type,
      const content::NotificationSource& source,
      const content::NotificationDetails& details) OVERRIDE;

  
  void OnSSLCertError(const net::SSLInfo& ssl_info);

  
  
  bool IsLoginTab() const;

 private:
  friend class CaptivePortalBrowserTest;
  friend class CaptivePortalTabHelperTest;

  friend class content::WebContentsUserData<CaptivePortalTabHelper>;
  explicit CaptivePortalTabHelper(content::WebContents* web_contents);

  
  void OnRedirect(int child_id,
                  ResourceType::Type resource_type,
                  const GURL& new_url);

  
  void OnCaptivePortalResults(Result previous_result, Result result);

  void OnLoadAborted();

  
  
  void SetIsLoginTab();

  
  void SetTabReloaderForTest(CaptivePortalTabReloader* tab_reloader);

  const content::RenderViewHost* provisional_render_view_host() const {
    return provisional_render_view_host_;
  }

  CaptivePortalTabReloader* GetTabReloaderForTest();

  
  void OpenLoginTab();

  Profile* profile_;

  
  scoped_ptr<CaptivePortalTabReloader> tab_reloader_;
  scoped_ptr<CaptivePortalLoginDetector> login_detector_;

  content::WebContents* web_contents_;

  
  
  
  int pending_error_code_;

  
  
  
  
  content::RenderViewHost* provisional_render_view_host_;

  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(CaptivePortalTabHelper);
};

}  

#endif  

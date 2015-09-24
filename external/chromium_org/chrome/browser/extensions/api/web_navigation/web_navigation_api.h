// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_EXTENSIONS_API_WEB_NAVIGATION_WEB_NAVIGATION_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_WEB_NAVIGATION_WEB_NAVIGATION_API_H_

#include <map>
#include <set>

#include "base/compiler_specific.h"
#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"
#include "chrome/browser/extensions/api/web_navigation/frame_navigation_state.h"
#include "chrome/browser/extensions/chrome_extension_function.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser_list_observer.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"
#include "extensions/browser/event_router.h"
#include "url/gurl.h"

struct RetargetingDetails;

namespace extensions {

class WebNavigationTabObserver
    : public content::NotificationObserver,
      public content::WebContentsObserver,
      public content::WebContentsUserData<WebNavigationTabObserver> {
 public:
  virtual ~WebNavigationTabObserver();

  
  static WebNavigationTabObserver* Get(content::WebContents* web_contents);

  const FrameNavigationState& frame_navigation_state() const {
    return navigation_state_;
  }

  content::RenderViewHost* GetRenderViewHostInProcess(int process_id) const;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void RenderViewDeleted(
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void AboutToNavigateRenderView(
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidStartProvisionalLoadForFrame(
      int64 frame_num,
      int64 parent_frame_num,
      bool is_main_frame,
      const GURL& validated_url,
      bool is_error_page,
      bool is_iframe_srcdoc,
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidCommitProvisionalLoadForFrame(
      int64 frame_num,
      const base::string16& frame_unique_name,
      bool is_main_frame,
      const GURL& url,
      content::PageTransition transition_type,
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidFailProvisionalLoad(
      int64 frame_num,
      const base::string16& frame_unique_name,
      bool is_main_frame,
      const GURL& validated_url,
      int error_code,
      const base::string16& error_description,
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void DocumentLoadedInFrame(
      int64 frame_num,
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidFinishLoad(
      int64 frame_num,
      const GURL& validated_url,
      bool is_main_frame,
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidFailLoad(
      int64 frame_num,
      const GURL& validated_url,
      bool is_main_frame,
      int error_code,
      const base::string16& error_description,
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidOpenRequestedURL(content::WebContents* new_contents,
                                   const GURL& url,
                                   const content::Referrer& referrer,
                                   WindowOpenDisposition disposition,
                                   content::PageTransition transition,
                                   int64 source_frame_num) OVERRIDE;
  virtual void FrameDetached(content::RenderViewHost* render_view_host,
                             int64 frame_num) OVERRIDE;
  virtual void WebContentsDestroyed(content::WebContents* tab) OVERRIDE;

 private:
  explicit WebNavigationTabObserver(content::WebContents* web_contents);
  friend class content::WebContentsUserData<WebNavigationTabObserver>;

  
  
  bool IsReferenceFragmentNavigation(FrameNavigationState::FrameID frame_id,
                                     const GURL& url);

  
  
  
  
  void SendErrorEvents(content::WebContents* web_contents,
                       content::RenderViewHost* render_view_host,
                       FrameNavigationState::FrameID id_to_skip);

  
  FrameNavigationState navigation_state_;

  
  content::NotificationRegistrar registrar_;

  
  content::RenderViewHost* render_view_host_;

  
  
  content::RenderViewHost* pending_render_view_host_;

  DISALLOW_COPY_AND_ASSIGN(WebNavigationTabObserver);
};

class WebNavigationEventRouter : public TabStripModelObserver,
                                 public chrome::BrowserListObserver,
                                 public content::NotificationObserver {
 public:
  explicit WebNavigationEventRouter(Profile* profile);
  virtual ~WebNavigationEventRouter();

 private:
  
  struct PendingWebContents{
    PendingWebContents();
    PendingWebContents(content::WebContents* source_web_contents,
                       int64 source_frame_id,
                       bool source_frame_is_main_frame,
                       content::WebContents* target_web_contents,
                       const GURL& target_url);
    ~PendingWebContents();

    content::WebContents* source_web_contents;
    int64 source_frame_id;
    bool source_frame_is_main_frame;
    content::WebContents* target_web_contents;
    GURL target_url;
  };

  
  virtual void TabReplacedAt(TabStripModel* tab_strip_model,
                             content::WebContents* old_contents,
                             content::WebContents* new_contents,
                             int index) OVERRIDE;

  
  virtual void OnBrowserAdded(Browser* browser) OVERRIDE;
  virtual void OnBrowserRemoved(Browser* browser) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
  void Retargeting(const RetargetingDetails* details);

  
  
  void TabAdded(content::WebContents* tab);

  
  
  void TabDestroyed(content::WebContents* tab);

  
  
  std::map<content::WebContents*, PendingWebContents> pending_web_contents_;

  
  content::NotificationRegistrar registrar_;

  
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(WebNavigationEventRouter);
};

class WebNavigationGetFrameFunction : public ChromeSyncExtensionFunction {
  virtual ~WebNavigationGetFrameFunction() {}
  virtual bool RunImpl() OVERRIDE;
  DECLARE_EXTENSION_FUNCTION("webNavigation.getFrame", WEBNAVIGATION_GETFRAME)
};

class WebNavigationGetAllFramesFunction : public ChromeSyncExtensionFunction {
  virtual ~WebNavigationGetAllFramesFunction() {}
  virtual bool RunImpl() OVERRIDE;
  DECLARE_EXTENSION_FUNCTION("webNavigation.getAllFrames",
                             WEBNAVIGATION_GETALLFRAMES)
};

class WebNavigationAPI : public ProfileKeyedAPI,
                         public extensions::EventRouter::Observer {
 public:
  explicit WebNavigationAPI(Profile* profile);
  virtual ~WebNavigationAPI();

  
  virtual void Shutdown() OVERRIDE;

  
  static ProfileKeyedAPIFactory<WebNavigationAPI>* GetFactoryInstance();

  
  virtual void OnListenerAdded(const extensions::EventListenerInfo& details)
      OVERRIDE;

 private:
  friend class ProfileKeyedAPIFactory<WebNavigationAPI>;

  Profile* profile_;

  
  static const char* service_name() {
    return "WebNavigationAPI";
  }
  static const bool kServiceIsNULLWhileTesting = true;

  
  scoped_ptr<WebNavigationEventRouter> web_navigation_event_router_;

  DISALLOW_COPY_AND_ASSIGN(WebNavigationAPI);
};

}  

#endif  

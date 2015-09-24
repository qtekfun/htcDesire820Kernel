// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_IDENTITY_WEB_AUTH_FLOW_H_
#define CHROME_BROWSER_EXTENSIONS_API_IDENTITY_WEB_AUTH_FLOW_H_

#include <string>

#include "apps/shell_window_registry.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_observer.h"
#include "ui/gfx/rect.h"
#include "url/gurl.h"

class Profile;
class WebAuthFlowTest;

namespace content {
class NotificationDetails;
class NotificationSource;
class RenderViewHost;
class WebContents;
}

namespace extensions {

class WebAuthFlow : public content::NotificationObserver,
                    public content::WebContentsObserver,
                    public apps::ShellWindowRegistry::Observer {
 public:
  enum Mode {
    INTERACTIVE,  
    SILENT        
  };

  enum Failure {
    WINDOW_CLOSED,  
    INTERACTION_REQUIRED,  
    LOAD_FAILED
  };

  class Delegate {
   public:
    
    
    virtual void OnAuthFlowFailure(Failure failure) = 0;
    
    
    virtual void OnAuthFlowURLChange(const GURL& redirect_url) = 0;
    
    virtual void OnAuthFlowTitleChange(const std::string& title) = 0;

   protected:
    virtual ~Delegate() {}
  };

  
  
  WebAuthFlow(Delegate* delegate,
              Profile* profile,
              const GURL& provider_url,
              Mode mode);

  virtual ~WebAuthFlow();

  
  virtual void Start();

  
  void DetachDelegateAndDelete();

 private:
  friend class ::WebAuthFlowTest;

  
  virtual void OnShellWindowAdded(apps::ShellWindow* shell_window) OVERRIDE;
  virtual void OnShellWindowIconChanged(apps::ShellWindow* shell_window)
      OVERRIDE;
  virtual void OnShellWindowRemoved(apps::ShellWindow* shell_window) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void DidStopLoading(content::RenderViewHost* render_view_host)
      OVERRIDE;
  virtual void DidNavigateMainFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;
  virtual void RenderProcessGone(base::TerminationStatus status) OVERRIDE;
  virtual void DidStartProvisionalLoadForFrame(
      int64 frame_id,
      int64 parent_frame_id,
      bool is_main_frame,
      const GURL& validated_url,
      bool is_error_page,
      bool is_iframe_srcdoc,
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidFailProvisionalLoad(int64 frame_id,
                                      const base::string16& frame_unique_name,
                                      bool is_main_frame,
                                      const GURL& validated_url,
                                      int error_code,
                                      const base::string16& error_description,
                                      content::RenderViewHost* render_view_host)
      OVERRIDE;

  void BeforeUrlLoaded(const GURL& url);
  void AfterUrlLoaded();

  Delegate* delegate_;
  Profile* profile_;
  GURL provider_url_;
  Mode mode_;

  apps::ShellWindow* shell_window_;
  std::string shell_window_key_;
  bool embedded_window_created_;

  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(WebAuthFlow);
};

}  

#endif  

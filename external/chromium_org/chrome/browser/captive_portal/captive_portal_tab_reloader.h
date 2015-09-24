// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CAPTIVE_PORTAL_CAPTIVE_PORTAL_TAB_RELOADER_H_
#define CHROME_BROWSER_CAPTIVE_PORTAL_CAPTIVE_PORTAL_TAB_RELOADER_H_

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "chrome/browser/captive_portal/captive_portal_service.h"

class Profile;

namespace content {
class WebContents;
}

namespace net {
class SSLInfo;
}

namespace captive_portal {

class CaptivePortalTabReloader {
 public:
  enum State {
    STATE_NONE,
    
    
    
    STATE_TIMER_RUNNING,
    
    
    
    
    
    
    
    STATE_MAYBE_BROKEN_BY_PORTAL,
    
    
    
    
    
    
    STATE_BROKEN_BY_PORTAL,
    
    
    
    
    
    
    STATE_NEEDS_RELOAD,
  };

  
  typedef base::Callback<void()> OpenLoginTabCallback;

  
  
  
  CaptivePortalTabReloader(Profile* profile,
                           content::WebContents* web_contents,
                           const OpenLoginTabCallback& open_login_tab_callback);

  virtual ~CaptivePortalTabReloader();

  

  
  
  
  
  virtual void OnLoadStart(bool is_ssl);

  
  
  
  
  
  
  
  
  virtual void OnLoadCommitted(int net_error);

  
  
  virtual void OnAbort();

  
  virtual void OnRedirect(bool is_ssl);

  
  virtual void OnCaptivePortalResults(Result previous_result, Result result);

  
  void OnSSLCertError(const net::SSLInfo& ssl_info);

 protected:
  

  State state() const { return state_; }

  content::WebContents* web_contents() { return web_contents_; }

  void set_slow_ssl_load_time(base::TimeDelta slow_ssl_load_time) {
    slow_ssl_load_time_ = slow_ssl_load_time;
  }

  
  
  
  
  base::OneShotTimer<CaptivePortalTabReloader> slow_ssl_load_timer_;

 private:
  friend class CaptivePortalBrowserTest;

  
  
  void SetState(State new_state);

  
  
  void OnSlowSSLConnect();

  
  
  
  
  void ReloadTabIfNeeded();

  
  virtual void ReloadTab();

  
  
  
  virtual void MaybeOpenCaptivePortalLoginTab();

  
  
  virtual void CheckForCaptivePortal();

  Profile* profile_;
  content::WebContents* web_contents_;

  State state_;

  
  
  
  
  bool provisional_main_frame_load_;

  
  
  bool ssl_url_in_redirect_chain_;

  
  
  base::TimeDelta slow_ssl_load_time_;

  const OpenLoginTabCallback open_login_tab_callback_;

  base::WeakPtrFactory<CaptivePortalTabReloader> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(CaptivePortalTabReloader);
};

}  

#endif  

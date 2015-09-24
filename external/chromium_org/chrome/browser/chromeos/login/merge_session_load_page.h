// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_MERGE_SESSION_LOAD_PAGE_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_MERGE_SESSION_LOAD_PAGE_H_

#include <string>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "chrome/browser/chromeos/login/oauth2_login_manager.h"
#include "content/public/browser/interstitial_page_delegate.h"
#include "url/gurl.h"

namespace base {
class DictionaryValue;
}

namespace content {
class InterstitialPage;
class WebContents;
}

namespace extensions {
class Extension;
}

namespace chromeos {

class MergeSessionLoadPage
    : public content::InterstitialPageDelegate,
      public OAuth2LoginManager::Observer {
 public:
  
  
  typedef base::Closure CompletionCallback;

  
  
  MergeSessionLoadPage(content::WebContents* web_contents,
                       const GURL& url,
                       const CompletionCallback& callback);

  void Show();

 protected:
  virtual ~MergeSessionLoadPage();

 private:
  friend class TestMergeSessionLoadPage;

  
  virtual std::string GetHTMLContents() OVERRIDE;
  virtual void CommandReceived(const std::string& command) OVERRIDE;
  virtual void OverrideRendererPrefs(
      content::RendererPreferences* prefs) OVERRIDE;
  virtual void OnProceed() OVERRIDE;
  virtual void OnDontProceed() OVERRIDE;

  
  virtual void OnSessionRestoreStateChanged(
      Profile* user_profile,
      OAuth2LoginManager::SessionRestoreState state) OVERRIDE;

  void NotifyBlockingPageComplete();

  
  OAuth2LoginManager* GetOAuth2LoginManager();

  CompletionCallback callback_;

  
  bool proceeded_;

  content::WebContents* web_contents_;
  GURL url_;
  content::InterstitialPage* interstitial_page_;  

  DISALLOW_COPY_AND_ASSIGN(MergeSessionLoadPage);
};

}  

#endif  

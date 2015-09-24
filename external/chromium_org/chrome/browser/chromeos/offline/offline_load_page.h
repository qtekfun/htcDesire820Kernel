// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_OFFLINE_OFFLINE_LOAD_PAGE_H_
#define CHROME_BROWSER_CHROMEOS_OFFLINE_OFFLINE_LOAD_PAGE_H_

#include <string>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "content/public/browser/interstitial_page_delegate.h"
#include "net/base/network_change_notifier.h"
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

class OfflineLoadPage
    : public content::InterstitialPageDelegate,
      public net::NetworkChangeNotifier::ConnectionTypeObserver {
 public:
  
  
  typedef base::Callback<void(bool )> CompletionCallback;

  
  
  OfflineLoadPage(content::WebContents* web_contents, const GURL& url,
                  const CompletionCallback& callback);

  void Show();

 protected:
  virtual ~OfflineLoadPage();

  
  virtual void NotifyBlockingPageComplete(bool proceed);

 private:
  friend class TestOfflineLoadPage;

  
  virtual std::string GetHTMLContents() OVERRIDE;
  virtual void CommandReceived(const std::string& command) OVERRIDE;
  virtual void OverrideRendererPrefs(
      content::RendererPreferences* prefs) OVERRIDE;
  virtual void OnProceed() OVERRIDE;
  virtual void OnDontProceed() OVERRIDE;

  
  virtual void OnConnectionTypeChanged(
      net::NetworkChangeNotifier::ConnectionType type) OVERRIDE;

  CompletionCallback callback_;

  
  bool proceeded_;

  content::WebContents* web_contents_;
  GURL url_;
  content::InterstitialPage* interstitial_page_;  

  DISALLOW_COPY_AND_ASSIGN(OfflineLoadPage);
};

}  

#endif  

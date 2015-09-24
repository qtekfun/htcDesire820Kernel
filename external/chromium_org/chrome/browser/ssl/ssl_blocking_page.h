// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SSL_SSL_BLOCKING_PAGE_H_
#define CHROME_BROWSER_SSL_SSL_BLOCKING_PAGE_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "chrome/browser/history/history_service.h"
#include "content/public/browser/interstitial_page_delegate.h"
#include "net/ssl/ssl_info.h"
#include "url/gurl.h"

namespace base {
class DictionaryValue;
}

namespace content {
class InterstitialPage;
class WebContents;
}

class SSLBlockingPage : public content::InterstitialPageDelegate {
 public:
  SSLBlockingPage(
      content::WebContents* web_contents,
      int cert_error,
      const net::SSLInfo& ssl_info,
      const GURL& request_url,
      bool overridable,
      bool strict_enforcement,
      const base::Callback<void(bool)>& callback);
  virtual ~SSLBlockingPage();

  
  
  
  
  static void SetExtraInfo(base::DictionaryValue* strings,
                           const std::vector<base::string16>& extra_info);

 protected:
  
  virtual std::string GetHTMLContents() OVERRIDE;
  virtual void CommandReceived(const std::string& command) OVERRIDE;
  virtual void OverrideEntry(content::NavigationEntry* entry) OVERRIDE;
  virtual void OverrideRendererPrefs(
      content::RendererPreferences* prefs) OVERRIDE;
  virtual void OnProceed() OVERRIDE;
  virtual void OnDontProceed() OVERRIDE;

 private:
  void NotifyDenyCertificate();
  void NotifyAllowCertificate();

  
  void OnGotHistoryCount(HistoryService::Handle handle,
                         bool success,
                         int num_visits,
                         base::Time first_visit);

  base::Callback<void(bool)> callback_;

  content::WebContents* web_contents_;
  int cert_error_;
  net::SSLInfo ssl_info_;
  GURL request_url_;
  
  bool overridable_;
  
  bool strict_enforcement_;
  content::InterstitialPage* interstitial_page_;  
  
  bool internal_;
  
  int num_visits_;
  
  CancelableRequestConsumer request_consumer_;

  
  std::string trialCondition_;

  DISALLOW_COPY_AND_ASSIGN(SSLBlockingPage);
};

#endif  

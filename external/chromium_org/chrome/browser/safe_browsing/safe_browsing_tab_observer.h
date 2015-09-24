// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_TAB_OBSERVER_H_
#define CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_TAB_OBSERVER_H_

#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "content/public/browser/web_contents_user_data.h"

namespace content {
class WebContents;
}

namespace safe_browsing {

class ClientSideDetectionHost;

class SafeBrowsingTabObserver
    : public content::WebContentsUserData<SafeBrowsingTabObserver> {
 public:
  virtual ~SafeBrowsingTabObserver();

  ClientSideDetectionHost* detection_host() {
    return safebrowsing_detection_host_.get();
  }

 private:
  explicit SafeBrowsingTabObserver(content::WebContents* web_contents);
  friend class content::WebContentsUserData<SafeBrowsingTabObserver>;

  

  
  
  void UpdateSafebrowsingDetectionHost();

  
  scoped_ptr<ClientSideDetectionHost> safebrowsing_detection_host_;

  
  content::WebContents* web_contents_;

  PrefChangeRegistrar pref_change_registrar_;

  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingTabObserver);
};

}  

#endif  

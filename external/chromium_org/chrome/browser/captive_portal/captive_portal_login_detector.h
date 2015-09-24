// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CAPTIVE_PORTAL_CAPTIVE_PORTAL_LOGIN_DETECTOR_H_
#define CHROME_BROWSER_CAPTIVE_PORTAL_CAPTIVE_PORTAL_LOGIN_DETECTOR_H_

#include "base/basictypes.h"
#include "chrome/browser/captive_portal/captive_portal_service.h"

class Profile;

namespace captive_portal {

class CaptivePortalLoginDetector {
 public:
  explicit CaptivePortalLoginDetector(Profile* profile);

  ~CaptivePortalLoginDetector();

  void OnStoppedLoading();
  void OnCaptivePortalResults(Result previous_result, Result result);

  bool is_login_tab() const { return is_login_tab_; }
  void SetIsLoginTab();

 private:
  Profile* profile_;

  
  
  bool is_login_tab_;

  
  
  bool first_login_tab_load_;

  DISALLOW_COPY_AND_ASSIGN(CaptivePortalLoginDetector);
};

}  

#endif  

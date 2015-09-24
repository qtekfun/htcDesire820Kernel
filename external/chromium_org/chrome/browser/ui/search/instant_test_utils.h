// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SEARCH_INSTANT_TEST_UTILS_H_
#define CHROME_BROWSER_UI_SEARCH_INSTANT_TEST_UTILS_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/metrics/field_trial.h"
#include "base/run_loop.h"
#include "base/strings/string16.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_instant_controller.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/omnibox/location_bar.h"
#include "chrome/browser/ui/search/instant_controller.h"
#include "chrome/common/search_types.h"
#include "net/test/spawned_test_server/spawned_test_server.h"
#include "url/gurl.h"

class BrowserInstantController;
class InstantController;
class InstantModel;
class OmniboxView;

namespace content {
class WebContents;
};

class InstantTestBase {
 protected:
  InstantTestBase();
  virtual ~InstantTestBase();

 protected:
  void SetupInstant(Browser* browser);
  void Init(const GURL& instant_url, bool init_suggestions_url);

  void SetInstantURL(const std::string& url);

  void set_browser(Browser* browser) {
    browser_ = browser;
  }

  BrowserInstantController* browser_instant() {
    return browser_->instant_controller();
  }

  InstantController* instant() {
    return browser_->instant_controller()->instant();
  }

  OmniboxView* omnibox() {
    return browser_->window()->GetLocationBar()->GetOmniboxView();
  }

  const GURL& instant_url() const { return instant_url_; }

  net::SpawnedTestServer& https_test_server() { return https_test_server_; }

  void KillInstantRenderView();

  void FocusOmnibox();
  void FocusOmniboxAndWaitForInstantNTPSupport();

  void SetOmniboxText(const std::string& text);

  void PressEnterAndWaitForNavigation();

  bool GetBoolFromJS(content::WebContents* contents,
                     const std::string& script,
                     bool* result) WARN_UNUSED_RESULT;
  bool GetIntFromJS(content::WebContents* contents,
                    const std::string& script,
                    int* result) WARN_UNUSED_RESULT;
  bool GetStringFromJS(content::WebContents* contents,
                       const std::string& script,
                       std::string* result) WARN_UNUSED_RESULT;
  bool ExecuteScript(const std::string& script) WARN_UNUSED_RESULT;
  bool CheckVisibilityIs(content::WebContents* contents,
                         bool expected) WARN_UNUSED_RESULT;

  std::string GetOmniboxText();

  
  
  
  bool LoadImage(content::RenderViewHost* rvh,
                 const std::string& image,
                 bool* loaded);

  
  base::string16 GetBlueText();

 private:
  GURL instant_url_;

  Browser* browser_;

  
  net::SpawnedTestServer https_test_server_;

  
  bool init_suggestions_url_;

  DISALLOW_COPY_AND_ASSIGN(InstantTestBase);
};

#endif  

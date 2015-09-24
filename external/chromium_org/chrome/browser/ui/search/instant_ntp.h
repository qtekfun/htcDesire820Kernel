// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SEARCH_INSTANT_NTP_H_
#define CHROME_BROWSER_UI_SEARCH_INSTANT_NTP_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/search/instant_loader.h"
#include "chrome/browser/ui/search/instant_page.h"

class InstantNTPPrerenderer;
class Profile;

namespace content {
class RenderViewHost;
class WebContents;
}

class InstantNTP : public InstantPage,
                   public InstantLoader::Delegate {
 public:
  InstantNTP(InstantNTPPrerenderer* delegate,
             const std::string& instant_url,
             Profile* profile);
  virtual ~InstantNTP();

  
  
  
  void InitContents(const base::Closure& on_stale_callback);

  
  
  scoped_ptr<content::WebContents> ReleaseContents();

 private:
  FRIEND_TEST_ALL_PREFIXES(InstantExtendedNetworkTest,
                           NTPReactsToNetworkChanges);

  
  virtual void RenderProcessGone(
      base::TerminationStatus status) OVERRIDE;

  
  virtual void OnSwappedContents() OVERRIDE;
  virtual content::WebContents* OpenURLFromTab(
      content::WebContents* source,
      const content::OpenURLParams& params) OVERRIDE;
  virtual void LoadCompletedMainFrame() OVERRIDE;

  InstantLoader loader_;
  InstantNTPPrerenderer* const ntp_prerenderer_;

  DISALLOW_COPY_AND_ASSIGN(InstantNTP);
};

#endif  

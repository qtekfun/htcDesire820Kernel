// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SEARCH_INSTANT_TAB_H_
#define CHROME_BROWSER_UI_SEARCH_INSTANT_TAB_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/ui/search/instant_page.h"

class Profile;

class InstantTab : public InstantPage {
 public:
  InstantTab(InstantPage::Delegate* delegate, Profile* profile);
  virtual ~InstantTab();

  
  
  void Init(content::WebContents* contents);

  
  static void EmitNtpStatistics(content::WebContents* contents);

 private:
  
  virtual bool ShouldProcessAboutToNavigateMainFrame() OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(InstantTab);
};

#endif  

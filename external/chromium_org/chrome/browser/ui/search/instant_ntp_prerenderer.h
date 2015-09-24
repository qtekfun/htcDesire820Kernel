// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SEARCH_INSTANT_NTP_PRERENDERER_H_
#define CHROME_BROWSER_UI_SEARCH_INSTANT_NTP_PRERENDERER_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "chrome/browser/search/instant_service_observer.h"
#include "chrome/browser/ui/search/instant_page.h"
#include "content/public/browser/web_contents.h"
#include "net/base/network_change_notifier.h"

class InstantNTP;
class InstantService;
class PrefService;
class Profile;

class InstantNTPPrerenderer
    : public InstantPage::Delegate,
      public net::NetworkChangeNotifier::NetworkChangeObserver,
      public InstantServiceObserver {
 public:
  InstantNTPPrerenderer(Profile* profile, InstantService* instant_service,
      PrefService* prefs);
  virtual ~InstantNTPPrerenderer();

  
  void ReloadInstantNTP();

  
  
  scoped_ptr<content::WebContents> ReleaseNTPContents() WARN_UNUSED_RESULT;

  
  content::WebContents* GetNTPContents() const;

  
  void DeleteNTPContents();

  
  void RenderProcessGone();

  
  void LoadCompletedMainFrame();

 protected:
  
  
  virtual std::string GetLocalInstantURL() const;

  
  
  
  
  
  virtual std::string GetInstantURL() const;

  
  virtual bool IsJavascriptEnabled() const;

  
  virtual bool InStartup() const;

  
  virtual InstantNTP* ntp() const;

  Profile* profile() const {
    return profile_;
  }

 private:
  friend class InstantExtendedTest;
  friend class InstantNTPPrerendererTest;
  friend class InstantTestBase;

  FRIEND_TEST_ALL_PREFIXES(InstantExtendedNetworkTest,
                           NTPReactsToNetworkChanges);
  FRIEND_TEST_ALL_PREFIXES(InstantNTPPrerendererTest,
                           PrefersRemoteNTPOnStartup);
  FRIEND_TEST_ALL_PREFIXES(InstantNTPPrerendererTest,
                           SwitchesToLocalNTPIfNoInstantSupport);
  FRIEND_TEST_ALL_PREFIXES(InstantNTPPrerendererTest,
                           SwitchesToLocalNTPIfPathBad);
  FRIEND_TEST_ALL_PREFIXES(InstantNTPPrerendererTest,
                           DoesNotSwitchToLocalNTPIfOnCurrentNTP);
  FRIEND_TEST_ALL_PREFIXES(InstantNTPPrerendererTest,
                           DoesNotSwitchToLocalNTPIfOnLocalNTP);
  FRIEND_TEST_ALL_PREFIXES(InstantNTPPrerendererTest,
                           SwitchesToLocalNTPIfJSDisabled);
  FRIEND_TEST_ALL_PREFIXES(InstantNTPPrerendererTest,
                           SwitchesToLocalNTPIfNoNTPReady);
  FRIEND_TEST_ALL_PREFIXES(InstantNTPPrerendererTest,
                           IsJavascriptEnabled);
  FRIEND_TEST_ALL_PREFIXES(InstantNTPPrerendererTest,
                           IsJavascriptEnabledChecksContentSettings);
  FRIEND_TEST_ALL_PREFIXES(InstantNTPPrerendererTest,
                           IsJavascriptEnabledChecksPrefs);
  FRIEND_TEST_ALL_PREFIXES(InstantExtendedManualTest, MANUAL_ShowsGoogleNTP);
  FRIEND_TEST_ALL_PREFIXES(InstantExtendedManualTest,
                           MANUAL_SearchesFromFakebox);
  FRIEND_TEST_ALL_PREFIXES(InstantExtendedTest, ProcessIsolation);

  
  
  virtual void OnNetworkChanged(net::NetworkChangeNotifier::ConnectionType type)
      OVERRIDE;

  
  virtual void InstantSupportDetermined(const content::WebContents* contents,
                                        bool supports_instant) OVERRIDE;
  virtual void InstantPageAboutToNavigateMainFrame(
      const content::WebContents* contents,
      const GURL& url) OVERRIDE;
  virtual void InstantPageLoadFailed(content::WebContents* contents) OVERRIDE;

  
  virtual void DefaultSearchProviderChanged() OVERRIDE;
  virtual void GoogleURLUpdated() OVERRIDE;

  
  void ResetNTP(const std::string& instant_url);

  
  
  bool PageIsCurrent() const;

  
  bool ShouldSwitchToLocalNTP() const;

  Profile* profile_;

  
  scoped_ptr<InstantNTP> ntp_;

  PrefChangeRegistrar profile_pref_registrar_;

  DISALLOW_COPY_AND_ASSIGN(InstantNTPPrerenderer);
};

#endif  

// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILE_RESETTER_PROFILE_RESETTER_H_
#define CHROME_BROWSER_PROFILE_RESETTER_PROFILE_RESETTER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/browsing_data/browsing_data_remover.h"
#include "chrome/browser/profile_resetter/brandcoded_default_settings.h"
#include "chrome/browser/search_engines/template_url_service.h"

class Profile;

class ProfileResetter : public base::NonThreadSafe,
                        public BrowsingDataRemover::Observer {
 public:
  
  enum Resettable {
    DEFAULT_SEARCH_ENGINE = 1 << 0,
    HOMEPAGE = 1 << 1,
    CONTENT_SETTINGS = 1 << 2,
    COOKIES_AND_SITE_DATA = 1 << 3,
    EXTENSIONS = 1 << 4,
    STARTUP_PAGES = 1 << 5,
    PINNED_TABS = 1 << 6,
    
    
    ALL = DEFAULT_SEARCH_ENGINE | HOMEPAGE | CONTENT_SETTINGS |
          COOKIES_AND_SITE_DATA | EXTENSIONS | STARTUP_PAGES | PINNED_TABS
  };

  
  typedef uint32 ResettableFlags;

  COMPILE_ASSERT(sizeof(ResettableFlags) == sizeof(Resettable),
                 type_ResettableFlags_doesnt_match_Resettable);

  explicit ProfileResetter(Profile* profile);
  virtual ~ProfileResetter();

  
  
  
  void Reset(ResettableFlags resettable_flags,
             scoped_ptr<BrandcodedDefaultSettings> master_settings,
             const base::Closure& callback);

  bool IsActive() const;

 private:
  
  
  void MarkAsDone(Resettable resettable);

  void ResetDefaultSearchEngine();
  void ResetHomepage();
  void ResetContentSettings();
  void ResetCookiesAndSiteData();
  void ResetExtensions();
  void ResetStartupPages();
  void ResetPinnedTabs();

  
  virtual void OnBrowsingDataRemoverDone() OVERRIDE;

  
  void OnTemplateURLServiceLoaded();

  Profile* const profile_;
  scoped_ptr<BrandcodedDefaultSettings> master_settings_;
  TemplateURLService* template_url_service_;

  
  
  ResettableFlags pending_reset_flags_;

  
  base::Closure callback_;

  
  
  BrowsingDataRemover* cookies_remover_;

  scoped_ptr<TemplateURLService::Subscription> template_url_service_sub_;

  DISALLOW_COPY_AND_ASSIGN(ProfileResetter);
};

#endif  

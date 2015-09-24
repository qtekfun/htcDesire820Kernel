// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_INSTANT_UNITTEST_BASE_H_
#define CHROME_BROWSER_SEARCH_INSTANT_UNITTEST_BASE_H_

#include <string>

#include "base/metrics/field_trial.h"
#include "chrome/browser/search/instant_service.h"
#include "chrome/browser/search_engines/template_url_service.h"
#include "chrome/test/base/browser_with_test_window_test.h"

class InstantServiceObserver;

class InstantUnitTestBase : public BrowserWithTestWindowTest {
 protected:
  InstantUnitTestBase();
  virtual ~InstantUnitTestBase();

  virtual void SetUp() OVERRIDE;
  virtual void TearDown() OVERRIDE;

  virtual void SetUpWithoutCacheableNTP();
  virtual void SetUpHelper();

  
  
  
  
  
  void SetDefaultSearchProvider(const std::string& base_url);

  
  
  
  void NotifyGoogleBaseURLUpdate(const std::string& new_google_base_url);

  bool IsInstantServiceObserver(InstantServiceObserver* observer);

  InstantService* instant_service_;
  TemplateURLService* template_url_service_;
  scoped_ptr<base::FieldTrialList> field_trial_list_;
};

#endif  

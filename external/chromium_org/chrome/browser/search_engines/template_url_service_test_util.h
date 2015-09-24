// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_SERVICE_TEST_UTIL_H_
#define CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_SERVICE_TEST_UTIL_H_

#include <string>

#include "base/basictypes.h"
#include "base/files/scoped_temp_dir.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/strings/string16.h"
#include "chrome/browser/search_engines/template_url_service_observer.h"
#include "chrome/test/base/testing_browser_process.h"
#include "content/public/test/test_browser_thread_bundle.h"

class GURL;
class TemplateURLService;
class TestingProfile;
class TestingTemplateURLService;
class TestingProfile;
class WebDataService;

class TemplateURLServiceTestUtilBase : public TemplateURLServiceObserver {
 public:
  TemplateURLServiceTestUtilBase();
  virtual ~TemplateURLServiceTestUtilBase();

  void CreateTemplateUrlService();

  
  virtual void OnTemplateURLServiceChanged() OVERRIDE;

  
  int GetObserverCount();

  
  void ResetObserverCount();

  
  void VerifyLoad();

  
  
  
  void ChangeModelToLoadState();

  
  void ClearModel();

  
  void ResetModel(bool verify_load);

  
  
  base::string16 GetAndClearSearchTerm();

  
  void SetGoogleBaseURL(const GURL& base_url) const;

  
  
  
  void SetManagedDefaultSearchPreferences(
      bool enabled,
      const std::string& name,
      const std::string& keyword,
      const std::string& search_url,
      const std::string& suggest_url,
      const std::string& icon_url,
      const std::string& encodings,
      const std::string& alternate_url,
      const std::string& search_terms_replacement_key);

  
  
  void RemoveManagedDefaultSearchPreferences();

  
  TemplateURLService* model() const;

  
  virtual TestingProfile* profile() const = 0;

 private:
  int changed_count_;

  DISALLOW_COPY_AND_ASSIGN(TemplateURLServiceTestUtilBase);
};

class TemplateURLServiceTestUtil : public TemplateURLServiceTestUtilBase {
 public:
  TemplateURLServiceTestUtil();
  virtual ~TemplateURLServiceTestUtil();

  
  
  void SetUp();

  
  
  void TearDown();

  
  virtual TestingProfile* profile() const OVERRIDE;

 private:
  
  
  content::TestBrowserThreadBundle thread_bundle_;
  scoped_ptr<TestingProfile> profile_;
  base::ScopedTempDir temp_dir_;

  DISALLOW_COPY_AND_ASSIGN(TemplateURLServiceTestUtil);
};

#endif  

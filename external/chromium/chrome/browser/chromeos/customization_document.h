// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CUSTOMIZATION_DOCUMENT_H_
#define CHROME_BROWSER_CHROMEOS_CUSTOMIZATION_DOCUMENT_H_
#pragma once

#include <map>
#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "base/timer.h"
#include "base/values.h"
#include "chrome/common/net/url_fetcher.h"
#include "googleurl/src/gurl.h"

class DictionaryValue;
class FilePath;
class ListValue;
class PrefService;

namespace base {
  class Time;
}

namespace chromeos {

class SystemAccess;

class CustomizationDocument {
 public:
  virtual ~CustomizationDocument() {}

  
  bool IsReady() const { return root_.get(); }

 protected:
  CustomizationDocument() {}

  virtual bool LoadManifestFromFile(const FilePath& manifest_path);
  virtual bool LoadManifestFromString(const std::string& manifest);

  std::string GetLocaleSpecificString(const std::string& locale,
                                      const std::string& dictionary_name,
                                      const std::string& entry_name) const;

  scoped_ptr<DictionaryValue> root_;

 private:
  DISALLOW_COPY_AND_ASSIGN(CustomizationDocument);
};

class StartupCustomizationDocument : public CustomizationDocument {
 public:
  static StartupCustomizationDocument* GetInstance();

  const std::string& initial_locale() const { return initial_locale_; }
  const std::string& initial_timezone() const { return initial_timezone_; }
  const std::string& keyboard_layout() const { return keyboard_layout_; }
  const std::string& registration_url() const { return registration_url_; }

  std::string GetHelpPage(const std::string& locale) const;
  std::string GetEULAPage(const std::string& locale) const;

 private:
  FRIEND_TEST(StartupCustomizationDocumentTest, Basic);
  FRIEND_TEST(StartupCustomizationDocumentTest, VPD);
  FRIEND_TEST(StartupCustomizationDocumentTest, BadManifest);
  friend struct DefaultSingletonTraits<StartupCustomizationDocument>;

  
  StartupCustomizationDocument();

  
  StartupCustomizationDocument(SystemAccess* system_access,
                               const std::string& manifest);

  void Init(SystemAccess* system_access);

  
  void InitFromMachineStatistic(const char* attr, std::string* value);

  std::string initial_locale_;
  std::string initial_timezone_;
  std::string keyboard_layout_;
  std::string registration_url_;

  DISALLOW_COPY_AND_ASSIGN(StartupCustomizationDocument);
};

class ServicesCustomizationDocument : public CustomizationDocument,
                                      private URLFetcher::Delegate {
 public:
  
  struct CarrierDeal {
    explicit CarrierDeal(DictionaryValue* deal_dict);

    
    
    
    std::string GetLocalizedString(const std::string& locale,
                                   const std::string& id) const;

    std::string deal_locale;
    std::string top_up_url;
    int notification_count;
    base::Time expire_date;
    DictionaryValue* localized_strings;
  };

  
  typedef std::map<std::string, CarrierDeal*> CarrierDeals;

  static ServicesCustomizationDocument* GetInstance();

  
  static void RegisterPrefs(PrefService* local_state);

  
  
  static bool WasApplied();

  
  void StartFetching();

  
  
  bool ApplyCustomization();

  std::string GetInitialStartPage(const std::string& locale) const;
  std::string GetSupportPage(const std::string& locale) const;

  
  
  
  const ServicesCustomizationDocument::CarrierDeal* GetCarrierDeal(
      const std::string& carrier_id, bool check_restrictions) const;

 protected:
  virtual bool LoadManifestFromString(const std::string& manifest) OVERRIDE;

 private:
  FRIEND_TEST(ServicesCustomizationDocumentTest, Basic);
  FRIEND_TEST(ServicesCustomizationDocumentTest, BadManifest);
  FRIEND_TEST(ServicesCustomizationDocumentTest, DealOtherLocale);
  FRIEND_TEST(ServicesCustomizationDocumentTest, NoDealRestrictions);
  FRIEND_TEST(ServicesCustomizationDocumentTest, OldDeal);
  friend struct DefaultSingletonTraits<ServicesCustomizationDocument>;

  
  ServicesCustomizationDocument();

  
  ServicesCustomizationDocument(const std::string& manifest,
                                const std::string& initial_locale);

  
  static void SetApplied(bool val);

  
  virtual void OnURLFetchComplete(const URLFetcher* source,
                                  const GURL& url,
                                  const net::URLRequestStatus& status,
                                  int response_code,
                                  const ResponseCookies& cookies,
                                  const std::string& data);

  
  void StartFileFetch();

  
  void ReadFileInBackground(const FilePath& file);

  
  GURL url_;

  
  scoped_ptr<URLFetcher> url_fetcher_;

  
  base::OneShotTimer<ServicesCustomizationDocument> retry_timer_;

  
  int num_retries_;

  
  CarrierDeals carrier_deals_;

  
  std::string initial_locale_;

  DISALLOW_COPY_AND_ASSIGN(ServicesCustomizationDocument);
};

}  

#endif  

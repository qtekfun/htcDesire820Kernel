// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CUSTOMIZATION_DOCUMENT_H_
#define CHROME_BROWSER_CHROMEOS_CUSTOMIZATION_DOCUMENT_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "base/timer/timer.h"
#include "base/values.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "url/gurl.h"

class PrefRegistrySimple;

namespace base {
class DictionaryValue;
class FilePath;
}

namespace net {
class URLFetcher;
}

namespace chromeos {

namespace system {
class StatisticsProvider;
}  

class CustomizationDocument {
 public:
  virtual ~CustomizationDocument();

  
  bool IsReady() const { return root_.get(); }

 protected:
  explicit CustomizationDocument(const std::string& accepted_version);

  virtual bool LoadManifestFromFile(const base::FilePath& manifest_path);
  virtual bool LoadManifestFromString(const std::string& manifest);

  std::string GetLocaleSpecificString(const std::string& locale,
                                      const std::string& dictionary_name,
                                      const std::string& entry_name) const;

  scoped_ptr<base::DictionaryValue> root_;

  
  
  std::string accepted_version_;

 private:
  DISALLOW_COPY_AND_ASSIGN(CustomizationDocument);
};

class StartupCustomizationDocument : public CustomizationDocument {
 public:
  static StartupCustomizationDocument* GetInstance();

  std::string GetHelpPage(const std::string& locale) const;
  std::string GetEULAPage(const std::string& locale) const;

  const std::string& registration_url() const { return registration_url_; }

  
  
  const std::string& initial_locale() const { return initial_locale_; }
  const std::string& initial_timezone() const { return initial_timezone_; }
  const std::string& keyboard_layout() const { return keyboard_layout_; }

 private:
  FRIEND_TEST_ALL_PREFIXES(StartupCustomizationDocumentTest, Basic);
  FRIEND_TEST_ALL_PREFIXES(StartupCustomizationDocumentTest, VPD);
  FRIEND_TEST_ALL_PREFIXES(StartupCustomizationDocumentTest, BadManifest);
  friend struct DefaultSingletonTraits<StartupCustomizationDocument>;

  
  StartupCustomizationDocument();

  
  StartupCustomizationDocument(system::StatisticsProvider* provider,
                               const std::string& manifest);

  virtual ~StartupCustomizationDocument();

  void Init(system::StatisticsProvider* provider);

  
  void InitFromMachineStatistic(const char* attr, std::string* value);

  std::string initial_locale_;
  std::string initial_timezone_;
  std::string keyboard_layout_;
  std::string registration_url_;

  DISALLOW_COPY_AND_ASSIGN(StartupCustomizationDocument);
};

class ServicesCustomizationDocument : public CustomizationDocument,
                                      private net::URLFetcherDelegate {
 public:
  static ServicesCustomizationDocument* GetInstance();

  
  static void RegisterPrefs(PrefRegistrySimple* registry);

  
  
  static bool WasApplied();

  
  void StartFetching();

  
  
  bool ApplyCustomization();

  std::string GetInitialStartPage(const std::string& locale) const;
  std::string GetSupportPage(const std::string& locale) const;

 private:
  FRIEND_TEST_ALL_PREFIXES(ServicesCustomizationDocumentTest, Basic);
  FRIEND_TEST_ALL_PREFIXES(ServicesCustomizationDocumentTest, BadManifest);
  friend struct DefaultSingletonTraits<ServicesCustomizationDocument>;

  
  ServicesCustomizationDocument();

  
  explicit ServicesCustomizationDocument(const std::string& manifest);

  virtual ~ServicesCustomizationDocument();

  
  static void SetApplied(bool val);

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  void StartFileFetch();

  
  void ReadFileInBackground(const base::FilePath& file);

  
  GURL url_;

  
  scoped_ptr<net::URLFetcher> url_fetcher_;

  
  base::OneShotTimer<ServicesCustomizationDocument> retry_timer_;

  
  int num_retries_;

  DISALLOW_COPY_AND_ASSIGN(ServicesCustomizationDocument);
};

}  

#endif  

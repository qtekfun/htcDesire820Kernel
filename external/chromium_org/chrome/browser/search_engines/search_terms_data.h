// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_ENGINES_SEARCH_TERMS_DATA_H_
#define CHROME_BROWSER_SEARCH_ENGINES_SEARCH_TERMS_DATA_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/strings/string16.h"

class Profile;

class SearchTermsData {
 public:
  SearchTermsData();
  virtual ~SearchTermsData();

  
  
  virtual std::string GoogleBaseURLValue() const;

  
  
  std::string GoogleBaseSuggestURLValue() const;

  
  
  virtual std::string GetApplicationLocale() const;

  
  
  virtual base::string16 GetRlzParameterValue() const;

  
  
  virtual std::string GetSearchClient() const;

  
  
  
  virtual std::string GetSuggestClient() const;

  
  
  
  
  virtual std::string GetSuggestRequestIdentifier() const;

  
  
  
  
  
  
  
  
  virtual std::string ForceInstantResultsParam(bool for_prerender) const;

  
  
  
  
  virtual std::string InstantExtendedEnabledParam() const;

  
  
  
  
  
  virtual std::string NTPIsThemedParam() const;

 private:
  DISALLOW_COPY_AND_ASSIGN(SearchTermsData);
};

class UIThreadSearchTermsData : public SearchTermsData {
 public:
  
  
  
  explicit UIThreadSearchTermsData(Profile* profile);

  virtual std::string GoogleBaseURLValue() const OVERRIDE;
  virtual std::string GetApplicationLocale() const OVERRIDE;
  virtual base::string16 GetRlzParameterValue() const OVERRIDE;
  virtual std::string GetSearchClient() const OVERRIDE;
  virtual std::string GetSuggestClient() const OVERRIDE;
  virtual std::string GetSuggestRequestIdentifier() const OVERRIDE;
  virtual std::string ForceInstantResultsParam(
      bool for_prerender) const OVERRIDE;
  virtual std::string InstantExtendedEnabledParam() const OVERRIDE;
  virtual std::string NTPIsThemedParam() const OVERRIDE;

  
  
  static void SetGoogleBaseURL(const std::string& base_url);

 private:
  static std::string* google_base_url_;
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(UIThreadSearchTermsData);
};

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_ENGINES_SEARCH_TERMS_DATA_H_
#define CHROME_BROWSER_SEARCH_ENGINES_SEARCH_TERMS_DATA_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/string16.h"

class SearchTermsData {
 public:
  SearchTermsData();
  virtual ~SearchTermsData();

  
  std::string GoogleBaseSuggestURLValue() const;

  
  virtual std::string GoogleBaseURLValue() const = 0;

  
  virtual std::string GetApplicationLocale() const = 0;

#if defined(OS_WIN) && defined(GOOGLE_CHROME_BUILD)
  
  virtual string16 GetRlzParameterValue() const = 0;
#endif

 private:
  DISALLOW_COPY_AND_ASSIGN(SearchTermsData);
};

class UIThreadSearchTermsData : public SearchTermsData {
 public:
  UIThreadSearchTermsData();

  
  virtual std::string GoogleBaseURLValue() const;
  virtual std::string GetApplicationLocale() const;
#if defined(OS_WIN) && defined(GOOGLE_CHROME_BUILD)
  virtual string16 GetRlzParameterValue() const;
#endif

  
  
  static void SetGoogleBaseURL(std::string* google_base_url);

 private:
  static std::string* google_base_url_;

  DISALLOW_COPY_AND_ASSIGN(UIThreadSearchTermsData);
};

#endif  

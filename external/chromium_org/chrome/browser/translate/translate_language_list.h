// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TRANSLATE_TRANSLATE_LANGUAGE_LIST_H_
#define CHROME_BROWSER_TRANSLATE_TRANSLATE_LANGUAGE_LIST_H_

#include <set>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "chrome/browser/web_resource/resource_request_allowed_notifier.h"

class TranslateURLFetcher;

class TranslateLanguageList : public ResourceRequestAllowedNotifier::Observer {
 public:
  static const int kFetcherId = 1;

  TranslateLanguageList();
  virtual ~TranslateLanguageList();

  
  
  base::Time last_updated() { return last_updated_; }

  
  
  void GetSupportedLanguages(std::vector<std::string>* languages);

  
  
  std::string GetLanguageCode(const std::string& chrome_locale);

  
  
  bool IsSupportedLanguage(const std::string& language);

  
  
  bool IsAlphaLanguage(const std::string& language);

  
  
  
  void RequestLanguageList();

  
  virtual void OnResourceRequestsAllowed() OVERRIDE;

  
  static void DisableUpdate();

  
  static const char kLanguageListCallbackName[];
  static const char kTargetLanguagesKey[];
  static const char kAlphaLanguagesKey[];

 private:
  
  void OnLanguageListFetchComplete(int id,
                                   bool success,
                                   const std::string& data);

  
  std::set<std::string> all_supported_languages_;

  
  std::set<std::string> alpha_languages_;

  
  
  scoped_ptr<TranslateURLFetcher> language_list_fetcher_;

  
  base::Time last_updated_;

  
  ResourceRequestAllowedNotifier resource_request_allowed_notifier_;

  DISALLOW_COPY_AND_ASSIGN(TranslateLanguageList);
};

#endif  

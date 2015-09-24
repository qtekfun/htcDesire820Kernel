// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TRANSLATE_TRANSLATE_SCRIPT_H_
#define CHROME_BROWSER_TRANSLATE_TRANSLATE_SCRIPT_H_

#include <string>

#include "base/callback.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"

class TranslateScriptTest;
class TranslateURLFetcher;

class TranslateScript {
 public:
  typedef base::Callback<void(bool, const std::string&)> Callback;

  static const int kFetcherId = 0;

  TranslateScript();
  virtual ~TranslateScript();

  
  const std::string& data() { return data_; }

  
  
  
  void set_expiration_delay(int delay_ms) {
    expiration_delay_ = base::TimeDelta::FromMilliseconds(delay_ms);
  }

  
  void Clear() { data_.clear(); }

  
  
  void Request(const Callback& callback);

  
  bool HasPendingRequest() const { return fetcher_.get() != NULL; }

 private:
  friend class TranslateScriptTest;
  FRIEND_TEST_ALL_PREFIXES(TranslateScriptTest, CheckScriptParameters);
  FRIEND_TEST_ALL_PREFIXES(TranslateScriptTest, CheckScriptURL);

  static const char kScriptURL[];
  static const char kRequestHeader[];

  
  static const char kAlwaysUseSslQueryName[];
  static const char kAlwaysUseSslQueryValue[];

  
  static const char kCallbackQueryName[];
  static const char kCallbackQueryValue[];

  
  static const char kCssLoaderCallbackQueryName[];
  static const char kCssLoaderCallbackQueryValue[];

  
  
  static const char kJavascriptLoaderCallbackQueryName[];
  static const char kJavascriptLoaderCallbackQueryValue[];

  
  void OnScriptFetchComplete(int id, bool success, const std::string& data);

  
  scoped_ptr<TranslateURLFetcher> fetcher_;

  
  std::string data_;

  
  
  base::TimeDelta expiration_delay_;

  
  Callback callback_;

  base::WeakPtrFactory<TranslateScript> weak_method_factory_;

  DISALLOW_COPY_AND_ASSIGN(TranslateScript);
};

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_SEARCH_SEARCH_PROVIDER_H_
#define CHROME_BROWSER_UI_APP_LIST_SEARCH_SEARCH_PROVIDER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/strings/string16.h"

namespace app_list {

class ChromeSearchResult;

class SearchProvider {
 public:
  typedef ScopedVector<ChromeSearchResult> Results;
  typedef base::Closure ResultChangedCallback;

  SearchProvider();
  virtual ~SearchProvider();

  
  virtual void Start(const base::string16& query) = 0;

  
  virtual void Stop() = 0;

  void set_result_changed_callback(const ResultChangedCallback& callback) {
    result_changed_callback_ = callback;
  }

  const Results& results() const { return results_; }

 protected:
  
  void Add(scoped_ptr<ChromeSearchResult> result);
  void ClearResults();

 private:
  void FireResultChanged();

  ResultChangedCallback result_changed_callback_;
  Results results_;

  DISALLOW_COPY_AND_ASSIGN(SearchProvider);
};

}  

#endif  

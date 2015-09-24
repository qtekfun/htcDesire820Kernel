// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_SEARCH_CHROME_SEARCH_RESULT_H_
#define CHROME_BROWSER_UI_APP_LIST_SEARCH_CHROME_SEARCH_RESULT_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "ui/app_list/search_result.h"

namespace app_list {

enum ChromeSearchResultType {
  
  OMNIBOX_SEARCH_RESULT,
  
  APP_SEARCH_RESULT,
  
  WEBSTORE_SEARCH_RESULT,
  
  SEARCH_WEBSTORE_SEARCH_RESULT,
  
  SEARCH_PEOPLE_SEARCH_RESULT,
  SEARCH_RESULT_TYPE_BOUNDARY
};

class ChromeSearchResult : public SearchResult {
 public:
  ChromeSearchResult() : relevance_(0.0) {}
  virtual ~ChromeSearchResult() {}

  
  virtual void Open(int event_flags) = 0;

  
  virtual void InvokeAction(int action_index, int event_flags) = 0;

  
  virtual scoped_ptr<ChromeSearchResult> Duplicate() = 0;

  virtual ChromeSearchResultType GetType() = 0;

  const std::string& id() const { return id_; }
  double relevance() { return relevance_; }

 protected:
  void set_id(const std::string& id) { id_ = id; }
  void set_relevance(double relevance) { relevance_ = relevance; }

 private:
  std::string id_;
  double relevance_;

  DISALLOW_COPY_AND_ASSIGN(ChromeSearchResult);
};

}  

#endif  

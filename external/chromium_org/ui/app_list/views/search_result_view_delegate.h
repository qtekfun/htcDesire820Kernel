// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_SEARCH_RESULT_VIEW_DELEGATE_H_
#define UI_APP_LIST_VIEWS_SEARCH_RESULT_VIEW_DELEGATE_H_

namespace app_list {

class SearchResultView;

class SearchResultViewDelegate {
 public:
  
  virtual void SearchResultActivated(SearchResultView* view,
                                     int event_flags) = 0;

  
  
  virtual void SearchResultActionActivated(SearchResultView* view,
                                           size_t action_index,
                                           int event_flags) = 0;

  
  virtual void OnSearchResultInstalled(SearchResultView* view) = 0;

  
  virtual void OnSearchResultUninstalled(SearchResultView* view) = 0;

 protected:
  virtual ~SearchResultViewDelegate() {}
};

}  

#endif  

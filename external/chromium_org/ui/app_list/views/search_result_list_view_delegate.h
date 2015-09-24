// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_SEARCH_RESULT_LIST_VIEW_DELEGATE_H_
#define UI_APP_LIST_VIEWS_SEARCH_RESULT_LIST_VIEW_DELEGATE_H_

#include "ui/app_list/app_list_export.h"

namespace app_list {

class SearchResult;

class APP_LIST_EXPORT SearchResultListViewDelegate {
 public:
  
  
  
  virtual void OpenResult(SearchResult* result,
                          int event_flags) = 0;

  
  
  virtual void InvokeResultAction(SearchResult* result,
                                  int action_index,
                                  int event_flags) = 0;

  
  virtual void OnResultInstalled(SearchResult* result) = 0;

  
  virtual void OnResultUninstalled(SearchResult* result) = 0;

 protected:
  virtual ~SearchResultListViewDelegate() {}
};

}  

#endif  

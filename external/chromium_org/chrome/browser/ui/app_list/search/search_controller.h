// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_SEARCH_SEARCH_CONTROLLER_H_
#define CHROME_BROWSER_UI_APP_LIST_SEARCH_SEARCH_CONTROLLER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/timer/timer.h"
#include "chrome/browser/ui/app_list/search/mixer.h"
#include "ui/app_list/app_list_model.h"

class AppListControllerDelegate;
class Profile;

namespace app_list {

class History;
class SearchBoxModel;
class SearchProvider;
class SearchResult;

class SearchController {
 public:
  SearchController(Profile* profile,
                   SearchBoxModel* search_box,
                   AppListModel::SearchResults* results,
                   AppListControllerDelegate* list_controller);
  ~SearchController();

  void Init();

  void Start();
  void Stop();

  void OpenResult(SearchResult* result, int event_flags);
  void InvokeResultAction(SearchResult* result,
                          int action_index,
                          int event_flags);

 private:
  typedef ScopedVector<SearchProvider> Providers;

  
  void AddProvider(Mixer::GroupId group,
                   scoped_ptr<SearchProvider> provider);

  
  void OnResultsChanged();

  Profile* profile_;
  SearchBoxModel* search_box_;
  AppListControllerDelegate* list_controller_;

  bool dispatching_query_;
  Providers providers_;
  scoped_ptr<Mixer> mixer_;
  History* history_;  

  base::OneShotTimer<SearchController> stop_timer_;

  DISALLOW_COPY_AND_ASSIGN(SearchController);
};

}  

#endif  

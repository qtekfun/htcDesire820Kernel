// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_SEARCH_MIXER_H_
#define CHROME_BROWSER_UI_APP_LIST_SEARCH_MIXER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_vector.h"
#include "chrome/browser/ui/app_list/search/history_types.h"
#include "ui/app_list/app_list_model.h"

namespace app_list {

class SearchProvider;

class Mixer {
 public:
  
  
  enum GroupId {
    MAIN_GROUP = 0,
    OMNIBOX_GROUP = 1,
    WEBSTORE_GROUP = 2,
    PEOPLE_GROUP = 3,
  };

  explicit Mixer(AppListModel::SearchResults* ui_results);
  ~Mixer();

  
  void Init();

  
  void AddProviderToGroup(GroupId group, SearchProvider* provider);

  
  void MixAndPublish(const KnownResults& known_results);

 private:
  class Group;
  typedef ScopedVector<Group> Groups;

  void FetchResults(const KnownResults& known_results);

  AppListModel::SearchResults* ui_results_;  
  Groups groups_;

  DISALLOW_COPY_AND_ASSIGN(Mixer);
};

}  

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_TEST_APP_LIST_TEST_MODEL_H_
#define UI_APP_LIST_TEST_APP_LIST_TEST_MODEL_H_

#include <string>

#include "ui/app_list/app_list_item_model.h"
#include "ui/app_list/app_list_model.h"

namespace app_list {

namespace test {

class AppListTestModel : public AppListModel {
 public:
  class AppListTestItemModel : public AppListItemModel {
   public:
    AppListTestItemModel(const std::string& id, AppListTestModel* model);
    virtual ~AppListTestItemModel();
    virtual void Activate(int event_flags) OVERRIDE;
    virtual const char* GetAppType() const OVERRIDE;

    void SetPosition(const syncer::StringOrdinal& new_position);

   private:
    AppListTestModel* model_;

    DISALLOW_COPY_AND_ASSIGN(AppListTestItemModel);
  };

  AppListTestModel();

  
  std::string GetItemName(int id);

  
  void PopulateApps(int n);

  
  void PopulateAppWithId(int id);

  
  std::string GetModelContent();

  
  AppListTestItemModel* CreateItem(const std::string& title,
                                   const std::string& full_name);

  
  void CreateAndAddItem(const std::string& title, const std::string& full_name);

  
  void CreateAndAddItem(const std::string& title);

  
  void HighlightItemAt(int index);

  int activate_count() { return activate_count_; }
  AppListItemModel* last_activated() { return last_activated_; }

  static const char kAppType[];

 private:
  void ItemActivated(AppListTestItemModel* item);

  int activate_count_;
  AppListItemModel* last_activated_;

  DISALLOW_COPY_AND_ASSIGN(AppListTestModel);
};

}  
}  

#endif  

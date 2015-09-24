// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_TEST_APP_LIST_CONTROLLER_TEST_API_H_
#define ASH_TEST_APP_LIST_CONTROLLER_TEST_API_H_

#include "base/basictypes.h"

namespace app_list {
class AppListView;
class AppsGridView;
}

namespace ash {
class Shell;
namespace internal {
class AppListController;
}

namespace test {

class AppListControllerTestApi {
 public:
  explicit AppListControllerTestApi(Shell* shell);

  
  app_list::AppsGridView* GetRootGridView();

  app_list::AppListView* view();

 private:
  internal::AppListController* app_list_controller_;  

  DISALLOW_COPY_AND_ASSIGN(AppListControllerTestApi);
};

}  
}  

#endif  

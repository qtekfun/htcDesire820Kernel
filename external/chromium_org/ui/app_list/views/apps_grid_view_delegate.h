// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_APPS_GRID_VIEW_DELEGATE_H_
#define UI_APP_LIST_VIEWS_APPS_GRID_VIEW_DELEGATE_H_

#include <string>

#include "base/callback_forward.h"
#include "ui/app_list/app_list_export.h"

namespace base {
class FilePath;
}

namespace app_list {

class AppListItemModel;

class APP_LIST_EXPORT AppsGridViewDelegate {
 public:
  
  
  virtual void ActivateApp(AppListItemModel* item, int event_flags) = 0;

  
  
  virtual void GetShortcutPathForApp(
      const std::string& app_id,
      const base::Callback<void(const base::FilePath&)>& callback) = 0;

 protected:
  virtual ~AppsGridViewDelegate() {}
};

}  

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_FAST_SHOW_PICKLER_H_
#define CHROME_BROWSER_UI_APP_LIST_FAST_SHOW_PICKLER_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/pickle.h"
#include "ui/app_list/app_list_model.h"

class FastShowPickler {
 public:
  
  static const int kVersion;

  
  
  static scoped_ptr<Pickle> PickleAppListModelForFastShow(
      app_list::AppListModel* model);

  
  
  static scoped_ptr<app_list::AppListModel> UnpickleAppListModelForFastShow(
      Pickle* pickle);

  
  
  static void CopyOver(
      app_list::AppListModel* src, app_list::AppListModel* dest);
};

#endif  

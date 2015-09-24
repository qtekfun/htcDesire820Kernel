// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_SEARCH_RESULT_OBSERVER_H_
#define UI_APP_LIST_SEARCH_RESULT_OBSERVER_H_

#include "ui/app_list/app_list_export.h"

namespace app_list {

class APP_LIST_EXPORT SearchResultObserver {
 public:
  
  virtual void OnIconChanged() = 0;

  
  virtual void OnActionsChanged() = 0;

  
  virtual void OnIsInstallingChanged() = 0;

  
  virtual void OnPercentDownloadedChanged() = 0;

  
  virtual void OnItemInstalled() = 0;

  
  virtual void OnItemUninstalled() = 0;

 protected:
  virtual ~SearchResultObserver() {}
};

}  

#endif  

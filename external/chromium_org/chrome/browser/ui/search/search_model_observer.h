// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SEARCH_SEARCH_MODEL_OBSERVER_H_
#define CHROME_BROWSER_UI_SEARCH_SEARCH_MODEL_OBSERVER_H_

#include "chrome/browser/ui/search/search_model.h"

class SearchModelObserver {
 public:
  
  virtual void ModelChanged(const SearchModel::State& old_state,
                            const SearchModel::State& new_state) = 0;

 protected:
  virtual ~SearchModelObserver() {}
};

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_INSTANT_SERVICE_OBSERVER_H_
#define CHROME_BROWSER_SEARCH_INSTANT_SERVICE_OBSERVER_H_

#include <vector>

struct InstantMostVisitedItem;
struct ThemeBackgroundInfo;

class InstantServiceObserver {
 public:
  
  virtual void ThemeInfoChanged(const ThemeBackgroundInfo&);

  
  virtual void MostVisitedItemsChanged(
      const std::vector<InstantMostVisitedItem>&);

  
  virtual void DefaultSearchProviderChanged();

  
  
  
  virtual void GoogleURLUpdated();

 protected:
  virtual ~InstantServiceObserver() {}
};

#endif  

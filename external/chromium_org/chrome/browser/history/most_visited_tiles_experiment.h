// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_MOST_VISITED_TILES_EXPERIMENT_H_
#define CHROME_BROWSER_HISTORY_MOST_VISITED_TILES_EXPERIMENT_H_

#include "base/values.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/common/instant_types.h"

namespace history {

enum NtpTileExperimentActions {
  NTP_TILE_EXPERIMENT_ACTION_REMOVED_URL = 0,
  NTP_TILE_EXPERIMENT_ACTION_DID_NOT_REMOVE_URL = 1,
  NTP_TILE_EXPERIMENT_ACTION_TOO_FEW_URLS_TILES_1_8 = 2,
  NTP_TILE_EXPERIMENT_ACTION_TOO_FEW_URLS_TILES_1_4 = 3,
  
  NUM_NTP_TILE_EXPERIMENT_ACTIONS
};

class MostVisitedTilesExperiment {
 public:
  
  static void MaybeShuffle(MostVisitedURLList* data);

  
  
  
  
  
  
  
  static bool IsDontShowOpenURLsEnabled();

  
  
  static void RemoveItemsMatchingOpenTabs(
      const std::set<std::string>& open_urls,
      std::vector<InstantMostVisitedItem>* items);

  
  
  static void RemovePageValuesMatchingOpenTabs(
      const std::set<std::string>& open_urls,
      base::ListValue* pages_value);

 private:
  
  
  static void LogInHistogram(NtpTileExperimentActions action);

  
  static bool ShouldRemoveURL(const std::set<std::string>& open_urls,
                       const std::string& url,
                       const size_t size);

  DISALLOW_IMPLICIT_CONSTRUCTORS(MostVisitedTilesExperiment);
};

}  

#endif  

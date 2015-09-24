// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_SEARCH_HISTORY_DATA_H_
#define CHROME_BROWSER_UI_APP_LIST_SEARCH_HISTORY_DATA_H_

#include <deque>
#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "chrome/browser/ui/app_list/search/history_types.h"

namespace app_list {

class HistoryDataObserver;
class HistoryDataStore;

class HistoryData : public base::SupportsWeakPtr<HistoryData> {
 public:
  typedef std::deque<std::string> SecondaryDeque;

  
  struct Data {
    Data();
    ~Data();

    
    std::string primary;

    
    SecondaryDeque secondary;

    
    base::Time update_time;
  };
  typedef std::map<std::string, Data> Associations;

  
  
  
  
  HistoryData(HistoryDataStore* store,
              size_t max_primary,
              size_t max_secondary);
  ~HistoryData();

  
  void Add(const std::string& query, const std::string& result_id);

  
  
  scoped_ptr<KnownResults> GetKnownResults(const std::string& query) const;

  void AddObserver(HistoryDataObserver* observer);
  void RemoveObserver(HistoryDataObserver* observer);

  const Associations& associations() const { return associations_; }

 private:
  
  void OnStoreLoaded(scoped_ptr<Associations> loaded_data);

  
  void TrimEntries();

  HistoryDataStore* store_;  
  const size_t max_primary_;
  const size_t max_secondary_;
  ObserverList<HistoryDataObserver, true> observers_;

  Associations associations_;

  DISALLOW_COPY_AND_ASSIGN(HistoryData);
};

}  

#endif  

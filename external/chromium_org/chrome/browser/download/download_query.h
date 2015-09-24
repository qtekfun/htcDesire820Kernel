// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_QUERY_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_QUERY_H_

#include <map>
#include <string>
#include <vector>

#include "base/callback_forward.h"
#include "content/public/browser/download_item.h"

namespace base {
class Value;
}

class DownloadQuery {
 public:
  typedef std::vector<content::DownloadItem*> DownloadVector;

  
  
  
  typedef base::Callback<bool(const content::DownloadItem&)> FilterCallback;

  
  enum FilterType {
    FILTER_BYTES_RECEIVED,       
    FILTER_DANGER_ACCEPTED,      
    FILTER_ENDED_AFTER,          
    FILTER_ENDED_BEFORE,         
    FILTER_END_TIME,             
    FILTER_EXISTS,               
    FILTER_FILENAME,             
    FILTER_FILENAME_REGEX,       
    FILTER_MIME,                 
    FILTER_PAUSED,               
    FILTER_QUERY,                
    FILTER_STARTED_AFTER,        
    FILTER_STARTED_BEFORE,       
    FILTER_START_TIME,           
    FILTER_TOTAL_BYTES,          
    FILTER_TOTAL_BYTES_GREATER,  
    FILTER_TOTAL_BYTES_LESS,     
    FILTER_URL,                  
    FILTER_URL_REGEX,            
  };

  enum SortType {
    SORT_BYTES_RECEIVED,
    SORT_DANGER,
    SORT_DANGER_ACCEPTED,
    SORT_END_TIME,
    SORT_EXISTS,
    SORT_FILENAME,
    SORT_MIME,
    SORT_PAUSED,
    SORT_START_TIME,
    SORT_STATE,
    SORT_TOTAL_BYTES,
    SORT_URL,
  };

  enum SortDirection {
    ASCENDING,
    DESCENDING,
  };

  DownloadQuery();
  ~DownloadQuery();

  
  
  
  
  
  
  
  
  
  bool AddFilter(const FilterCallback& filter);
  bool AddFilter(FilterType type, const base::Value& value);
  void AddFilter(content::DownloadDangerType danger);
  void AddFilter(content::DownloadItem::DownloadState state);

  
  
  
  
  
  
  
  
  
  void AddSorter(SortType type, SortDirection direction);

  
  void Limit(size_t limit) { limit_ = limit; }

  
  
  template <typename InputIterator>
  void Search(InputIterator iter, const InputIterator last,
              DownloadVector* results) const {
    results->clear();
    for (; iter != last; ++iter) {
      if (Matches(**iter)) results->push_back(*iter);
    }
    FinishSearch(results);
  }

 private:
  struct Sorter;
  class DownloadComparator;
  typedef std::vector<FilterCallback> FilterCallbackVector;
  typedef std::vector<Sorter> SorterVector;

  bool FilterRegex(const std::string& regex_str,
                   const base::Callback<std::string(
                       const content::DownloadItem&)>& accessor);
  bool Matches(const content::DownloadItem& item) const;
  void FinishSearch(DownloadVector* results) const;

  FilterCallbackVector filters_;
  SorterVector sorters_;
  size_t limit_;

  DISALLOW_COPY_AND_ASSIGN(DownloadQuery);
};

#endif  

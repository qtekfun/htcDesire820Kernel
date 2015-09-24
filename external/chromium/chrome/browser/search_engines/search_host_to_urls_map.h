// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_ENGINES_SEARCH_HOST_TO_URLS_MAP_H_
#define CHROME_BROWSER_SEARCH_ENGINES_SEARCH_HOST_TO_URLS_MAP_H_
#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"

class SearchTermsData;
class TemplateURL;

class SearchHostToURLsMap {
 public:
  typedef std::set<const TemplateURL*> TemplateURLSet;

  SearchHostToURLsMap();
  ~SearchHostToURLsMap();

  
  void Init(const std::vector<const TemplateURL*>& template_urls,
            const SearchTermsData& search_terms_data);

  
  
  void Add(const TemplateURL* template_url,
           const SearchTermsData& search_terms_data);

  
  void Remove(const TemplateURL* template_url);

  
  
  
  void Update(const TemplateURL* existing_turl,
              const TemplateURL& new_values,
              const SearchTermsData& search_terms_data);

  
  void UpdateGoogleBaseURLs(const SearchTermsData& search_terms_data);

  
  
  const TemplateURL* GetTemplateURLForHost(const std::string& host) const;

  
  
  const TemplateURLSet* GetURLsForHost(const std::string& host) const;

 private:
  friend class SearchHostToURLsMapTest;

  typedef std::map<std::string, TemplateURLSet> HostToURLsMap;

  
  
  
  void RemoveByPointer(const TemplateURL* template_url);

  
  HostToURLsMap host_to_urls_map_;

  
  std::string default_search_origin_;

  
  bool initialized_;

  DISALLOW_COPY_AND_ASSIGN(SearchHostToURLsMap);
};

#endif  

// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_HISTORY_HISTORY_NOTIFICATIONS_H__
#define CHROME_BROWSER_HISTORY_HISTORY_NOTIFICATIONS_H__
#pragma once

#include <set>
#include <vector>

#include "googleurl/src/gurl.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/search_engines/template_url_id.h"

namespace history {

struct HistoryDetails {
 public:
  virtual ~HistoryDetails() {}
};

struct URLVisitedDetails : public HistoryDetails {
  URLVisitedDetails();
  virtual ~URLVisitedDetails();

  PageTransition::Type transition;
  URLRow row;

  
  
  
  
  history::RedirectList redirects;
};

struct URLsModifiedDetails : public HistoryDetails {
  URLsModifiedDetails();
  virtual ~URLsModifiedDetails();

  
  std::vector<URLRow> changed_urls;
};

struct URLsDeletedDetails : public HistoryDetails {
  URLsDeletedDetails();
  virtual ~URLsDeletedDetails();

  
  bool all_history;

  
  
  
  std::set<GURL> urls;
};

struct URLsStarredDetails : public HistoryDetails {
  explicit URLsStarredDetails(bool being_starred);
  virtual ~URLsStarredDetails();

  
  
  bool starred;

  
  std::set<GURL> changed_urls;
};

struct FaviconChangeDetails : public HistoryDetails {
  FaviconChangeDetails();
  virtual ~FaviconChangeDetails();

  std::set<GURL> urls;
};

struct KeywordSearchTermDetails : public HistoryDetails {
  KeywordSearchTermDetails();
  ~KeywordSearchTermDetails();

  GURL url;
  TemplateURLID keyword_id;
  string16 term;
};

}  

#endif  

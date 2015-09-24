// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_HISTORY_HISTORY_NOTIFICATIONS_H__
#define CHROME_BROWSER_HISTORY_HISTORY_NOTIFICATIONS_H__

#include <set>

#include "chrome/browser/history/history_details.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/search_engines/template_url_id.h"
#include "url/gurl.h"

namespace history {

struct URLVisitedDetails : public HistoryDetails {
  URLVisitedDetails();
  virtual ~URLVisitedDetails();

  content::PageTransition transition;
  URLRow row;

  
  
  
  
  history::RedirectList redirects;
};

struct URLsModifiedDetails : public HistoryDetails {
  URLsModifiedDetails();
  virtual ~URLsModifiedDetails();

  
  URLRows changed_urls;
};

struct URLsDeletedDetails : public HistoryDetails {
  URLsDeletedDetails();
  virtual ~URLsDeletedDetails();

  
  bool all_history;

  
  
  bool archived;

  
  
  URLRows rows;

  
  
  std::set<GURL> favicon_urls;
};

struct KeywordSearchUpdatedDetails : public HistoryDetails {
  KeywordSearchUpdatedDetails(const GURL& url,
                              TemplateURLID keyword_id,
                              const base::string16& term);
  virtual ~KeywordSearchUpdatedDetails();

  GURL url;
  TemplateURLID keyword_id;
  base::string16 term;
};

struct KeywordSearchDeletedDetails : public HistoryDetails {
  explicit KeywordSearchDeletedDetails(const GURL& url);
  virtual ~KeywordSearchDeletedDetails();

  GURL url;
};

}  

#endif  

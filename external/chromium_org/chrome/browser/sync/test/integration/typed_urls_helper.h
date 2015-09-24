// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_TEST_INTEGRATION_TYPED_URLS_HELPER_H_
#define CHROME_BROWSER_SYNC_TEST_INTEGRATION_TYPED_URLS_HELPER_H_

#include <vector>

#include "chrome/browser/history/history_types.h"
#include "content/public/common/page_transition_types.h"

namespace base {
class Time;
}

namespace typed_urls_helper {

history::URLRows GetTypedUrlsFromClient(int index);

bool GetUrlFromClient(int index, const GURL& url, history::URLRow* row);

history::VisitVector GetVisitsFromClient(int index, history::URLID id);

void RemoveVisitsFromClient(int index, const history::VisitVector& visits);

void AddUrlToHistory(int index, const GURL& url);

void AddUrlToHistoryWithTransition(int index,
                                   const GURL& url,
                                   content::PageTransition transition,
                                   history::VisitSource source);

void AddUrlToHistoryWithTimestamp(int index,
                                  const GURL& url,
                                  content::PageTransition transition,
                                  history::VisitSource source,
                                  const base::Time& timestamp);

void DeleteUrlFromHistory(int index, const GURL& url);

void DeleteUrlsFromHistory(int index, const std::vector<GURL>& urls);

void AssertAllProfilesHaveSameURLsAsVerifier();

void AssertURLRowVectorsAreEqual(const history::URLRows& left,
                                 const history::URLRows& right);

void AssertURLRowsAreEqual(const history::URLRow& left,
                           const history::URLRow& right);

bool AreVisitsEqual(const history::VisitVector& visit1,
                    const history::VisitVector& visit2);

bool AreVisitsUnique(const history::VisitVector& visits);

base::Time GetTimestamp();

}  

#endif  

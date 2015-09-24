// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_HISTORY_HISTORY_MARSHALING_H_
#define CHROME_BROWSER_HISTORY_HISTORY_MARSHALING_H_

#include "base/memory/scoped_vector.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/favicon/favicon_service.h"
#include "chrome/browser/history/history_service.h"
#include "chrome/browser/history/page_usage_data.h"

#if defined(OS_ANDROID)
#include "chrome/browser/history/history_marshaling_android.h"
#endif

namespace history {


typedef CancelableRequest1<HistoryService::QueryURLCallback,
                           Tuple2<URLRow, VisitVector> >
    QueryURLRequest;

typedef CancelableRequest1<HistoryService::QueryHistoryCallback,
                           QueryResults>
    QueryHistoryRequest;

typedef CancelableRequest1<HistoryService::QueryRedirectsCallback,
                           history::RedirectList>
    QueryRedirectsRequest;

typedef CancelableRequest<HistoryService::GetVisibleVisitCountToHostCallback>
    GetVisibleVisitCountToHostRequest;

typedef CancelableRequest1<HistoryService::QueryTopURLsAndRedirectsCallback,
                           Tuple2<std::vector<GURL>,
                                  history::RedirectMap> >
    QueryTopURLsAndRedirectsRequest;

typedef CancelableRequest1<HistoryService::QueryMostVisitedURLsCallback,
                           history::MostVisitedURLList>
    QueryMostVisitedURLsRequest;

typedef CancelableRequest1<HistoryService::QueryFilteredURLsCallback,
                           history::FilteredURLList>
    QueryFilteredURLsRequest;


typedef CancelableRequest1<HistoryService::SegmentQueryCallback,
                           ScopedVector<PageUsageData> >
    QuerySegmentUsageRequest;


typedef
    CancelableRequest1<HistoryService::GetMostRecentKeywordSearchTermsCallback,
                       std::vector<KeywordSearchTermVisit> >
    GetMostRecentKeywordSearchTermsRequest;


typedef CancelableRequest1<base::Closure, scoped_refptr<HistoryDBTask> >
    HistoryDBTaskRequest;

}  

#endif  

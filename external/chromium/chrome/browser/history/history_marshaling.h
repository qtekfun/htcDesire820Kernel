// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_HISTORY_HISTORY_MARSHALING_H__
#define CHROME_BROWSER_HISTORY_HISTORY_MARSHALING_H__
#pragma once

#include "base/memory/scoped_vector.h"
#include "chrome/browser/favicon_service.h"
#include "chrome/browser/history/history.h"
#include "chrome/browser/history/page_usage_data.h"
#include "content/browser/cancelable_request.h"

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

typedef CancelableRequest<HistoryService::GetVisitCountToHostCallback>
    GetVisitCountToHostRequest;

typedef CancelableRequest1<HistoryService::QueryTopURLsAndRedirectsCallback,
                           Tuple2<std::vector<GURL>,
                                  history::RedirectMap> >
    QueryTopURLsAndRedirectsRequest;

typedef CancelableRequest1<HistoryService::QueryMostVisitedURLsCallback,
                           history::MostVisitedURLList>
    QueryMostVisitedURLsRequest;


typedef CancelableRequest<HistoryService::ThumbnailDataCallback>
    GetPageThumbnailRequest;


typedef CancelableRequest<FaviconService::FaviconDataCallback>
    GetFaviconRequest;


typedef CancelableRequest1<HistoryService::DownloadQueryCallback,
                           std::vector<DownloadCreateInfo> >
    DownloadQueryRequest;

typedef CancelableRequest<HistoryService::DownloadCreateCallback>
    DownloadCreateRequest;


typedef CancelableRequest<HistoryService::ExpireHistoryCallback>
    ExpireHistoryRequest;


typedef CancelableRequest1<HistoryService::SegmentQueryCallback,
                           ScopedVector<PageUsageData> >
    QuerySegmentUsageRequest;


typedef
    CancelableRequest1<HistoryService::GetMostRecentKeywordSearchTermsCallback,
                       std::vector<KeywordSearchTermVisit> >
    GetMostRecentKeywordSearchTermsRequest;


typedef CancelableRequest1<HistoryService::HistoryDBTaskCallback,
                           scoped_refptr<HistoryDBTask> >
    HistoryDBTaskRequest;

}  

#endif  

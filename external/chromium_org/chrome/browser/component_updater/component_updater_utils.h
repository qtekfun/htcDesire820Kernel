// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_COMPONENT_UPDATER_COMPONENT_UPDATER_UTILS_H_
#define CHROME_BROWSER_COMPONENT_UPDATER_COMPONENT_UPDATER_UTILS_H_

#include <string>

class GURL;

namespace base {
class FilePath;
}

namespace net {
class URLFetcher;
class URLFetcherDelegate;
class URLRequestContextGetter;
}

namespace component_updater {

struct CrxUpdateItem;


std::string BuildProtocolRequest(const std::string& request_body,
                                 const std::string& additional_attributes);

net::URLFetcher* SendProtocolRequest(
    const GURL& url,
    const std::string& protocol_request,
    net::URLFetcherDelegate* url_fetcher_delegate,
    net::URLRequestContextGetter* url_request_context_getter);

bool FetchSuccess(const net::URLFetcher& fetcher);

int GetFetchError(const net::URLFetcher& fetcher);

bool HasDiffUpdate(const CrxUpdateItem* update_item);

bool IsHttpServerError(int status_code);

bool DeleteFileAndEmptyParentDirectory(const base::FilePath& filepath);

}  

#endif  


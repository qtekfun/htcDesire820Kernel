// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_EXTENSIONS_ACTIVITY_LOG_WEB_REQUEST_CONSTANTS_H_
#define CHROME_BROWSER_EXTENSIONS_ACTIVITY_LOG_WEB_REQUEST_CONSTANTS_H_

namespace activity_log_web_request_constants {

extern const char kCancelKey[];
extern const char kNewUrlKey[];
extern const char kModifiedRequestHeadersKey[];
extern const char kDeletedRequestHeadersKey[];
extern const char kAddedRequestHeadersKey[];
extern const char kDeletedResponseHeadersKey[];
extern const char kAuthCredentialsKey[];
extern const char kResponseCookieModificationsKey[];

extern const char kCookieModificationTypeKey[];
extern const char kCookieModificationAdd[];
extern const char kCookieModificationEdit[];
extern const char kCookieModificationRemove[];
extern const char kCookieFilterNameKey[];
extern const char kCookieFilterDomainKey[];
extern const char kCookieModNameKey[];
extern const char kCookieModDomainKey[];

}  

#endif  

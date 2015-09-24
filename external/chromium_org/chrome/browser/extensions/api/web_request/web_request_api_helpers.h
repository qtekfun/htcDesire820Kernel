// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_EXTENSIONS_API_WEB_REQUEST_WEB_REQUEST_API_HELPERS_H_
#define CHROME_BROWSER_EXTENSIONS_API_WEB_REQUEST_WEB_REQUEST_API_HELPERS_H_

#include <list>
#include <set>
#include <string>

#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "chrome/browser/extensions/extension_warning_set.h"
#include "net/base/auth.h"
#include "net/http/http_request_headers.h"
#include "net/http/http_response_headers.h"
#include "url/gurl.h"
#include "webkit/common/resource_type.h"

namespace base {
class ListValue;
class Value;
}

namespace extensions {
class Extension;
}

namespace net {
class BoundNetLog;
class URLRequest;
}

namespace extension_web_request_api_helpers {

typedef std::pair<std::string, std::string> ResponseHeader;
typedef std::vector<ResponseHeader> ResponseHeaders;

struct RequestCookie {
  RequestCookie();
  ~RequestCookie();
  scoped_ptr<std::string> name;
  scoped_ptr<std::string> value;
 private:
  DISALLOW_COPY_AND_ASSIGN(RequestCookie);
};

bool NullableEquals(const RequestCookie* a, const RequestCookie* b);

struct ResponseCookie {
  ResponseCookie();
  ~ResponseCookie();
  scoped_ptr<std::string> name;
  scoped_ptr<std::string> value;
  scoped_ptr<std::string> expires;
  scoped_ptr<int> max_age;
  scoped_ptr<std::string> domain;
  scoped_ptr<std::string> path;
  scoped_ptr<bool> secure;
  scoped_ptr<bool> http_only;
 private:
  DISALLOW_COPY_AND_ASSIGN(ResponseCookie);
};

bool NullableEquals(const ResponseCookie* a, const ResponseCookie* b);

struct FilterResponseCookie : ResponseCookie {
  FilterResponseCookie();
  ~FilterResponseCookie();
  scoped_ptr<int> age_lower_bound;
  scoped_ptr<int> age_upper_bound;
  scoped_ptr<bool> session_cookie;
 private:
  DISALLOW_COPY_AND_ASSIGN(FilterResponseCookie);
};

bool NullableEquals(const FilterResponseCookie* a,
                    const FilterResponseCookie* b);

enum CookieModificationType {
  ADD,
  EDIT,
  REMOVE,
};

struct RequestCookieModification {
  RequestCookieModification();
  ~RequestCookieModification();
  CookieModificationType type;
  
  scoped_ptr<RequestCookie> filter;
  
  scoped_ptr<RequestCookie> modification;
 private:
  DISALLOW_COPY_AND_ASSIGN(RequestCookieModification);
};

bool NullableEquals(const RequestCookieModification* a,
                    const RequestCookieModification* b);

struct ResponseCookieModification {
  ResponseCookieModification();
  ~ResponseCookieModification();
  CookieModificationType type;
  
  scoped_ptr<FilterResponseCookie> filter;
  
  scoped_ptr<ResponseCookie> modification;
 private:
  DISALLOW_COPY_AND_ASSIGN(ResponseCookieModification);
};

bool NullableEquals(const ResponseCookieModification* a,
                    const ResponseCookieModification* b);

typedef std::vector<linked_ptr<RequestCookieModification> >
    RequestCookieModifications;
typedef std::vector<linked_ptr<ResponseCookieModification> >
    ResponseCookieModifications;

struct EventResponseDelta {
  
  std::string extension_id;

  
  
  
  base::Time extension_install_time;

  
  bool cancel;
  GURL new_url;

  
  net::HttpRequestHeaders modified_request_headers;

  
  std::vector<std::string> deleted_request_headers;

  
  
  ResponseHeaders added_response_headers;

  
  ResponseHeaders deleted_response_headers;

  
  scoped_ptr<net::AuthCredentials> auth_credentials;

  
  RequestCookieModifications request_cookie_modifications;

  
  ResponseCookieModifications response_cookie_modifications;

  
  
  std::set<std::string> messages_to_extension;

  EventResponseDelta(const std::string& extension_id,
                     const base::Time& extension_install_time);
  ~EventResponseDelta();

  DISALLOW_COPY_AND_ASSIGN(EventResponseDelta);
};

typedef std::list<linked_ptr<EventResponseDelta> > EventResponseDeltas;

bool InDecreasingExtensionInstallationTimeOrder(
    const linked_ptr<EventResponseDelta>& a,
    const linked_ptr<EventResponseDelta>& b);

base::ListValue* StringToCharList(const std::string& s);

bool CharListToString(const base::ListValue* list, std::string* out);


EventResponseDelta* CalculateOnBeforeRequestDelta(
    const std::string& extension_id,
    const base::Time& extension_install_time,
    bool cancel,
    const GURL& new_url);
EventResponseDelta* CalculateOnBeforeSendHeadersDelta(
    const std::string& extension_id,
    const base::Time& extension_install_time,
    bool cancel,
    net::HttpRequestHeaders* old_headers,
    net::HttpRequestHeaders* new_headers);
EventResponseDelta* CalculateOnHeadersReceivedDelta(
    const std::string& extension_id,
    const base::Time& extension_install_time,
    bool cancel,
    const net::HttpResponseHeaders* old_response_headers,
    ResponseHeaders* new_response_headers);
EventResponseDelta* CalculateOnAuthRequiredDelta(
    const std::string& extension_id,
    const base::Time& extension_install_time,
    bool cancel,
    scoped_ptr<net::AuthCredentials>* auth_credentials);


void MergeCancelOfResponses(
    const EventResponseDeltas& deltas,
    bool* canceled,
    const net::BoundNetLog* net_log);
void MergeOnBeforeRequestResponses(
    const EventResponseDeltas& deltas,
    GURL* new_url,
    extensions::ExtensionWarningSet* conflicting_extensions,
    const net::BoundNetLog* net_log);
void MergeCookiesInOnBeforeSendHeadersResponses(
    const EventResponseDeltas& deltas,
    net::HttpRequestHeaders* request_headers,
    extensions::ExtensionWarningSet* conflicting_extensions,
    const net::BoundNetLog* net_log);
void MergeOnBeforeSendHeadersResponses(
    const EventResponseDeltas& deltas,
    net::HttpRequestHeaders* request_headers,
    extensions::ExtensionWarningSet* conflicting_extensions,
    const net::BoundNetLog* net_log);
void MergeCookiesInOnHeadersReceivedResponses(
    const EventResponseDeltas& deltas,
    const net::HttpResponseHeaders* original_response_headers,
    scoped_refptr<net::HttpResponseHeaders>* override_response_headers,
    extensions::ExtensionWarningSet* conflicting_extensions,
    const net::BoundNetLog* net_log);
void MergeOnHeadersReceivedResponses(
    const EventResponseDeltas& deltas,
    const net::HttpResponseHeaders* original_response_headers,
    scoped_refptr<net::HttpResponseHeaders>* override_response_headers,
    extensions::ExtensionWarningSet* conflicting_extensions,
    const net::BoundNetLog* net_log);
bool MergeOnAuthRequiredResponses(
    const EventResponseDeltas& deltas,
    net::AuthCredentials* auth_credentials,
    extensions::ExtensionWarningSet* conflicting_extensions,
    const net::BoundNetLog* net_log);

bool IsRelevantResourceType(ResourceType::Type type);

const char* ResourceTypeToString(ResourceType::Type type);

bool ParseResourceType(const std::string& type_str,
                       ResourceType::Type* type);

void ClearCacheOnNavigation();

void NotifyWebRequestAPIUsed(
    void* profile_id,
    scoped_refptr<const extensions::Extension> extension);

}  

#endif  
